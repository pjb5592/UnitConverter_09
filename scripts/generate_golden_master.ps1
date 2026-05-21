# Golden Master 기준 파일 생성 스크립트 (Windows / PowerShell)
#
# 목적:
#   unit_converter CLI의 stdout을 4개 시나리오로 캡처하여
#   tests/golden_master_expected.txt 를 생성·갱신합니다.
#
# 캡처 방식 (시나리오별):
#   echo <입력> | .\build\unit_converter.exe  → stdout에서 변환 줄만 추출
#   (프롬프트 "Insert value for converting..." 는 기준에서 제외)
#
# 사용법:
#   .\scripts\generate_golden_master.ps1
#   .\scripts\generate_golden_master.ps1 -BuildDir build-cov
#
# 생성 후 버전 관리:
#   git add tests/golden_master_expected.txt
#   git commit -m "chore: update golden master baseline"

param(
    [string]$BuildDir = "build",
    [string]$OutputFile = "tests/golden_master_expected.txt"
)

$ErrorActionPreference = "Stop"
$RepoRoot = Split-Path -Parent $PSScriptRoot
Set-Location $RepoRoot

$Exe = Join-Path $BuildDir "unit_converter.exe"
if (-not (Test-Path $Exe)) {
    Write-Host "실행 파일 없음: $Exe — CMake 빌드 후 다시 실행하세요."
    Write-Host "  cmake -B $BuildDir -G Ninja"
    Write-Host "  cmake --build $BuildDir --target unit_converter"
    exit 1
}

$Prompt = "Insert value for converting (ex: meter:2.5): "
$Scenarios = @("meter:2.5", "feet:1.0", "yard:1.0", "meter:0.0")
$Blocks = New-Object System.Collections.Generic.List[string]

foreach ($input in $Scenarios) {
    $raw = $input | & $Exe 2>&1 | Out-String
    $lines = New-Object System.Collections.Generic.List[string]
    foreach ($line in ($raw -split "`r?`n")) {
        if ([string]::IsNullOrWhiteSpace($line)) { continue }
        if ($line.Contains($Prompt)) {
            $remainder = $line.Substring($line.IndexOf($Prompt) + $Prompt.Length)
            if (-not [string]::IsNullOrWhiteSpace($remainder)) {
                $lines.Add($remainder.TrimEnd())
            }
            continue
        }
        if ($line.Contains(" = ")) {
            $lines.Add($line.TrimEnd())
        }
    }
    $block = "[{0}]`n{1}" -f $input, ($lines -join "`n")
    $Blocks.Add($block)
}

$document = ($Blocks -join "`n---`n") + "`n"
$outPath = Join-Path $RepoRoot $OutputFile
$outDir = Split-Path -Parent $outPath
if (-not (Test-Path $outDir)) { New-Item -ItemType Directory -Path $outDir | Out-Null }
[System.IO.File]::WriteAllText($outPath, $document, [System.Text.UTF8Encoding]::new($false))

Write-Host "Golden Master 기준 파일 생성 완료: $outPath"
Write-Host "버전 관리: git add tests/golden_master_expected.txt"
