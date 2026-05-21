# REFACTOR coverage gate — build-cov, ctest, lcov layer thresholds (docs/refactoring_plan.md §6.6)
param(
    [string]$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path,
    [int]$EntityMin = 95,
    [int]$BoundaryMin = 85,
    [int]$DataMin = 90,
    [int]$ControlMin = 80,
    [int]$SrcOverallMin = 88
)

$ErrorActionPreference = "Stop"
Set-Location $ProjectRoot

Write-Host "== Configure build-cov =="
cmake -S . -B build-cov -DCMAKE_BUILD_TYPE=Debug `
    "-DCMAKE_CXX_FLAGS=--coverage -O0 -g" `
    "-DCMAKE_EXE_LINKER_FLAGS=--coverage" | Out-Null

Write-Host "== Build =="
cmake --build build-cov | Out-Null

Write-Host "== ctest (9 suites + GoldenMaster) =="
ctest --test-dir build-cov --output-on-failure
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "== lcov capture =="
lcov --capture --directory build-cov --output-file build-cov/coverage.info --rc lcov_branch_coverage=0 | Out-Null
lcov --remove build-cov/coverage.info "*/tests/*" "*/catch2/*" "*/_deps/*" "*/CompilerIdCXX/*" `
    --output-file build-cov/coverage.filtered.info | Out-Null

function Get-LinePercent([string]$InfoFile) {
    $summary = lcov --summary $InfoFile 2>&1 | Out-String
    if ($summary -match "lines\.\.+:\s*([\d.]+)%") {
        return [double]$Matches[1]
    }
    throw "Failed to parse lcov summary for $InfoFile"
}

$layers = @(
    @{ Name = "entity";   Pattern = "*/src/entity/*";   Min = $EntityMin },
    @{ Name = "boundary"; Pattern = "*/src/boundary/*"; Min = $BoundaryMin },
    @{ Name = "data";     Pattern = "*/src/data/*";     Min = $DataMin },
    @{ Name = "control";  Pattern = "*/src/control/*";  Min = $ControlMin }
)

$failed = $false
foreach ($layer in $layers) {
    $out = Join-Path $ProjectRoot "build-cov\$($layer.Name).info"
    lcov --extract build-cov/coverage.filtered.info $layer.Pattern --output-file $out | Out-Null
    $pct = Get-LinePercent $out
    $ok = $pct -ge $layer.Min
    $status = if ($ok) { "PASS" } else { "FAIL"; $failed = $true }
    Write-Host ("{0,-10} {1,5:F1}% (min {2}%) {3}" -f $layer.Name, $pct, $layer.Min, $status)
}

lcov --extract build-cov/coverage.filtered.info "*/src/*" --output-file build-cov/src.info | Out-Null
$srcPct = Get-LinePercent "build-cov/src.info"
$srcOk = $srcPct -ge $SrcOverallMin
if (-not $srcOk) { $failed = $true }
Write-Host ("{0,-10} {1,5:F1}% (min {2}%) {3}" -f "src", $srcPct, $SrcOverallMin, $(if ($srcOk) { "PASS" } else { "FAIL" }))

Write-Host "== F02: no 3.28084/1.09361 literals in src =="
$hits = Get-ChildItem -Path "src" -Recurse -File | Select-String -Pattern "3\.28084|1\.09361"
if ($hits) {
    $hits | ForEach-Object { Write-Host $_.Path }
    $failed = $true
    Write-Host "F02 FAIL"
} else {
    Write-Host "F02 PASS"
}

if ($failed) { exit 1 }
Write-Host "Coverage gate: all thresholds met."
