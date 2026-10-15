#!/usr/bin/env python3
import os
import re
import stat
import subprocess
import sys
import venv
from pathlib import Path

REQUIREMENTS_FILE = Path(__file__).resolve().parent / "requirements-format.txt"


def read_required_version() -> str:
    """Single source of truth for the clang-format version (shared with format.py and CI)."""
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


def get_venv_python(venv_dir: Path) -> Path:
    if os.name == "nt":
        candidate = venv_dir / "Scripts" / "python.exe"
    else:
        candidate = venv_dir / "bin" / "python"
    return candidate


def main() -> None:
    required_version = read_required_version()
    repo_root = Path(__file__).resolve().parent.parent
    venv_dir = repo_root / ".venv"
    venv_python = get_venv_python(venv_dir)

    print(f"Setting up virtual environment at {venv_dir}...")
    # Key on the interpreter, not the directory, so a half-created .venv gets repaired.
    if not venv_python.is_file():
        # --clear wipes a stale/broken .venv (e.g. dangling interpreter symlink after a Python upgrade).
        stale = venv_dir.exists()
        try:
            builder = venv.EnvBuilder(with_pip=True, clear=stale)
            builder.create(venv_dir)
        except Exception:
            subprocess.run(
                [sys.executable, "-m", "venv", *(["--clear"] if stale else []), str(venv_dir)],
                check=True,
            )

    if not venv_python.is_file():
        sys.stderr.write(f"Error: Python binary not found in virtual environment at {venv_python}\n")
        sys.exit(1)

    print(f"Installing clang-format=={required_version} inside .venv...")
    subprocess.run(
        [str(venv_python), "-m", "pip", "install", "--upgrade", "pip"],
        check=False,
    )
    subprocess.run(
        [str(venv_python), "-m", "pip", "install", "-r", str(REQUIREMENTS_FILE)],
        check=True,
    )

    print("Configuring git hooks path to .githooks...")
    subprocess.run(
        ["git", "config", "core.hooksPath", ".githooks"],
        cwd=repo_root,
        check=True,
    )

    rel_py = venv_python.relative_to(repo_root)
    print(f"Saving clangformat.python in git config ({rel_py.as_posix()})...")
    subprocess.run(
        ["git", "config", "--local", "clangformat.python", rel_py.as_posix()],
        cwd=repo_root,
        check=True,
    )
    # Drop the key used by earlier versions of this script (it lived in git's own format.* namespace).
    subprocess.run(
        ["git", "config", "--local", "--unset-all", "format.python"],
        cwd=repo_root,
        check=False,
    )

    hook_file = repo_root / ".githooks" / "pre-commit"
    if hook_file.is_file():
        st = os.stat(hook_file)
        os.chmod(hook_file, st.st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)

    print("\nSetup complete! The pre-commit hook will use .venv to auto-format staged C++ files.")


if __name__ == "__main__":
    main()