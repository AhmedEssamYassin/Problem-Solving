param([string]$Path = ".", [switch]$StagedOnly)

# newest cpptools clang-format (numeric version sort)
$cf = Get-ChildItem "$env:USERPROFILE\.vscode\extensions" -Directory |
      Where-Object { $_.Name -match '^ms-vscode\.cpptools-\d' } |
      Sort-Object { [version]($_.Name -replace '^ms-vscode\.cpptools-(\d+(\.\d+)*).*', '$1') } |
      ForEach-Object { Join-Path $_.FullName "LLVM\bin\clang-format.exe" } |
      Where-Object { Test-Path $_ } |
      Select-Object -Last 1
if (-not $cf) { throw "clang-format.exe not found in VS Code extensions." }
Write-Host "Using: $cf" -ForegroundColor Cyan

function Format-One([string]$file) {
    $d = Split-Path -Parent $file
    $found = $false
    while ($d) {
        if ((Test-Path (Join-Path $d ".clang-format")) -or (Test-Path (Join-Path $d "_clang-format"))) { $found = $true; break }
        $d = Split-Path -Parent $d
    }
    if (-not $found) {
        Write-Host "Skipped (no .clang-format): $file" -ForegroundColor Yellow
        return
    }
    & $cf -i -style=file -fallback-style=none $file
    Write-Host "Formatted: $file"
}

if ($StagedOnly) {
    $root  = git rev-parse --show-toplevel
    $files = @(git -c core.quotepath=off diff --cached --name-only --diff-filter=ACMR |
               Where-Object { $_ -match '\.(cpp|cc|cxx|h|hpp)$' })
    if ($files.Count -eq 0) { exit 0 }

    # abort if any staged file also has unstaged edits (formatting would stage them too)
    $dirty = @(git -c core.quotepath=off diff --name-only)
    $bad = $files | Where-Object { $dirty -contains $_ }
    if ($bad) {
        Write-Host "Unstaged changes in: $($bad -join ', '). Stage or stash them first." -ForegroundColor Red
        exit 1
    }

    foreach ($f in $files) {
        $full = Join-Path $root $f
        Format-One $full
        git add -- $full
    }
    exit 0
}

if (Test-Path $Path -PathType Leaf) {
    Format-One (Resolve-Path $Path).Path
} else {
    Get-ChildItem $Path -Recurse -File -Include *.cpp,*.cc,*.cxx,*.h,*.hpp |
        Where-Object { $_.FullName -notmatch '\\\.git\\' } |
        ForEach-Object { Format-One $_.FullName }
}