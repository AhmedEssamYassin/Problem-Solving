#!/usr/bin/env python3
from __future__ import annotations

import argparse
import os
import re
import shutil
import subprocess
import sys
import sysconfig
from collections.abc import Iterator
from pathlib import Path

EXTENSIONS = {".cpp", ".cc", ".cxx", ".c++", ".h", ".hh", ".hpp", ".hxx"}
BATCH_SIZE = 50

TOOLS_DIR = Path(__file__).resolve().parent
REPO_ROOT = TOOLS_DIR.parent
CONFIG_FILE = REPO_ROOT / ".clang-format"
REQUIREMENTS_FILE = TOOLS_DIR / "requirements-format.txt"

VERSION_RE = re.compile(r"\bversion\s+(\d+(?:\.\d+)*)")


def read_required_version() -> str:
    """Single source of truth for the clang-format version (shared with setup.py and CI)."""
    try:
        text = REQUIREMENTS_FILE.read_text(encoding="utf-8")
    except OSError as exc:
        sys.stderr.write(f"Error: cannot read {REQUIREMENTS_FILE}: {exc}\n")
        sys.exit(1)
    m = re.search(r"^\s*clang-format\s*==\s*(\d+(?:\.\d+)*)\s*$", text, re.MULTILINE)
    if not m:
        sys.stderr.write(f"Error: no 'clang-format==X.Y.Z' pin found in {REQUIREMENTS_FILE}\n")
        sys.exit(1)
    return m.group(1)


REQUIRED_VERSION = read_required_version()


def chunked(items: list, size: int = BATCH_SIZE) -> Iterator[list]:
    for i in range(0, len(items), size):
        yield items[i:i + size]


def find_clang_format() -> str:
    exe_name = "clang-format.exe" if os.name == "nt" else "clang-format"
    candidates: list[str] = []

    # 1. Check local .venv and current Python environment scripts directories
    script_dirs: list[Path] = [
        REPO_ROOT / ".venv" / "Scripts",
        REPO_ROOT / ".venv" / "bin",
        Path(sys.executable).parent,
    ]
    try:
        std_scripts = sysconfig.get_path("scripts")
        if std_scripts:
            script_dirs.append(Path(std_scripts))
    except (KeyError, AttributeError):
        pass

    try:
        user_scripts = sysconfig.get_path("scripts", f"{os.name}_user")
        if user_scripts:
            script_dirs.append(Path(user_scripts))
    except (KeyError, AttributeError):
        pass

    for sdir in script_dirs:
        candidate_bin = sdir / exe_name
        if candidate_bin.is_file() and str(candidate_bin) not in candidates:
            candidates.append(str(candidate_bin))

    # 2. Check system PATH
    which_cf = shutil.which("clang-format")
    if which_cf and which_cf not in candidates:
        candidates.append(which_cf)

    # 3. Fallback: VS Code cpptools extension (cross-platform paths)
    vscode_dirs = [
        Path.home() / ".vscode" / "extensions",
        Path.home() / ".vscode-server" / "extensions",
    ]
    for vdir in vscode_dirs:
        if vdir.is_dir():
            for ext in sorted(vdir.glob("ms-vscode.cpptools-*"), reverse=True):
                cf_path = ext / "LLVM" / "bin" / exe_name
                if cf_path.is_file() and str(cf_path) not in candidates:
                    candidates.append(str(cf_path))

    for cf in candidates:
        try:
            out = subprocess.check_output([cf, "--version"], text=True, stderr=subprocess.STDOUT).strip()
        except (OSError, subprocess.SubprocessError):
            # Not executable, broken, or crashed: try the next candidate.
            continue
        m = VERSION_RE.search(out)
        if m and m.group(1) == REQUIRED_VERSION:
            return cf

    sys.stderr.write(
        f"Error: clang-format {REQUIRED_VERSION} not found.\n"
        f"Run 'python tools/setup.py' to initialize the virtual environment and install the required version.\n"
    )
    sys.exit(1)


def format_files(cf: str, files: list[Path]) -> list[Path]:
    if not files:
        return []
    if not CONFIG_FILE.is_file():
        sys.stderr.write(f"Error: Root .clang-format not found at {CONFIG_FILE}\n")
        sys.exit(1)

    reformatted: list[Path] = []
    for batch in chunked(files):
        before = {p: p.read_bytes() for p in batch}
        res = subprocess.run([
            cf,
            "-i",
            f"-style=file:{CONFIG_FILE.as_posix()}",
            "-fallback-style=none",
            *[str(p) for p in batch],
        ])
        if res.returncode != 0:
            sys.stderr.write(
                f"Error: clang-format failed on a batch of {len(batch)} file(s) starting with {batch[0]}\n"
            )
            sys.exit(res.returncode)
        for p in batch:
            if p.read_bytes() != before[p]:
                reformatted.append(p)
                print(f"Formatted: {p}")
    if reformatted:
        print(f"Checked {len(files)} file(s), reformatted {len(reformatted)}.")
    else:
        print(f"Checked {len(files)} file(s), all properly formatted.")
    return reformatted


def git_z(subcmd: str, *args: str, cwd: Path | None = None) -> list[str]:
    """Run a git subcommand with -z and return its NUL-separated output as paths.

    NUL separation is exact for any file name (spaces, quotes, backslashes, newlines).
    """
    out = subprocess.check_output(
        ["git", "-c", "core.quotepath=off", subcmd, "-z", *args],
        cwd=cwd,
    )
    return [os.fsdecode(part) for part in out.split(b"\0") if part]


def run_staged_only() -> None:
    try:
        root_str = subprocess.check_output(
            ["git", "rev-parse", "--show-toplevel"], text=True, encoding="utf-8"
        ).strip()
    except (OSError, subprocess.SubprocessError):
        sys.stderr.write("Error: Not inside a git repository.\n")
        sys.exit(1)

    root = Path(root_str)
    staged = git_z("diff", "--cached", "--name-only", "--diff-filter=ACMR", cwd=root)

    # Reject rogue override config files staged anywhere except repo root
    rogue = [
        f for f in staged
        if Path(f).name in {".clang-format", "_clang-format"} and f != ".clang-format"
    ]
    if rogue:
        sys.stderr.write(f"Error: Only root .clang-format is permitted. Found: {', '.join(rogue)}\n")
        sys.exit(1)

    # CI re-checks every file when the config changes; the hook only formats staged files.
    if ".clang-format" in staged:
        sys.stderr.write(
            "Warning: .clang-format changed. CI will check ALL files in this case; "
            "run 'python tools/format.py' to format the whole repository.\n"
        )

    target_rels = [f for f in staged if Path(f).suffix.lower() in EXTENSIONS]
    if not target_rels:
        sys.exit(0)

    # 'git commit <paths>' runs hooks against a temporary index (next-index-*.lock); re-staging can't reach the real one.
    if Path(os.environ.get("GIT_INDEX_FILE", "")).name.startswith("next-index-"):
        sys.stderr.write(
            "Error: 'git commit <paths>' is not supported by the format hook. "
            "Run 'git add <paths>' and then plain 'git commit'.\n"
        )
        sys.exit(1)

    unstaged = set(git_z("diff", "--name-only", cwd=root))
    bad = [f for f in target_rels if f in unstaged]
    if bad:
        sys.stderr.write(
            f"Unstaged changes in: {', '.join(bad)}. Stage or stash them first.\n"
        )
        sys.exit(1)

    existing = [rel for rel in target_rels if (root / rel).is_file()]
    if not existing:
        sys.exit(0)

    cf = find_clang_format()
    print(f"Using: {cf}")

    reformatted = set(format_files(cf, [root / rel for rel in existing]))
    if reformatted:
        reformatted_rels = [rel for rel in existing if (root / rel) in reformatted]
        for batch in chunked(reformatted_rels):
            subprocess.run(
                ["git", "--literal-pathspecs", "add", "--", *batch],
                cwd=root,
                check=True,
            )


def main() -> None:
    for stream in (sys.stdout, sys.stderr):
        try:
            stream.reconfigure(errors="backslashreplace")
        except (AttributeError, ValueError):
            pass

    parser = argparse.ArgumentParser(description="Format C/C++ files with clang-format.")
    parser.add_argument("path", nargs="?", default=str(REPO_ROOT), help="File or directory path to format.")
    parser.add_argument(
        "--staged-only", "-StagedOnly",
        action="store_true",
        help="Format only staged Git files and re-stage them.",
    )
    args = parser.parse_args()

    if args.staged_only:
        run_staged_only()
        return

    cf = find_clang_format()
    print(f"Using: {cf}")

    target = Path(args.path).resolve()
    if not target.exists():
        sys.stderr.write(f"Error: Path does not exist: {target}\n")
        sys.exit(1)

    if target.is_file():
        if target.suffix.lower() in EXTENSIONS:
            format_files(cf, [target])
        return

    # Collect candidates first, format afterwards, so a formatting error can never
    # trigger the fallback walk and format files twice.
    # Prefer git ls-files so .gitignore is respected (e.g. build/, .venv/).
    files: list[Path] | None
    try:
        rel = target.relative_to(REPO_ROOT).as_posix()
        # ':(literal)' stops directory names like 'lib[1]' from being read as globs.
        path_spec = "." if rel == "." else f":(literal){rel}"
        listed = git_z(
            "ls-files", "--cached", "--others", "--exclude-standard", "--", path_spec,
            cwd=REPO_ROOT,
        )
        files = [REPO_ROOT / f for f in dict.fromkeys(listed)]
    except (ValueError, OSError, subprocess.SubprocessError):
        files = None

    if files is None:
        files = []
        for root_dir, dirs, names in os.walk(target):
            dirs[:] = [d for d in dirs if not d.startswith(".") and d not in {"build", "dist", "out", "target", "venv"}]
            for name in names:
                files.append(Path(root_dir) / name)

    format_files(cf, [p for p in files if p.suffix.lower() in EXTENSIONS and p.is_file()])


if __name__ == "__main__":
    main()