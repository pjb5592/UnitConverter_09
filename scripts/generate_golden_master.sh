#!/usr/bin/env bash
# Golden Master 기준 파일 생성 스크립트 (Linux / macOS / Git Bash)
#
# 목적:
#   unit_converter CLI stdout을 4개 시나리오로 캡처하여
#   tests/golden_master_expected.txt 를 생성·갱신합니다.
#
# 사용법:
#   ./scripts/generate_golden_master.sh
#   BUILD_DIR=build-cov ./scripts/generate_golden_master.sh
#
# 생성 후:
#   git add tests/golden_master_expected.txt

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-build}"
EXE="${REPO_ROOT}/${BUILD_DIR}/unit_converter"
OUTPUT="${REPO_ROOT}/tests/golden_master_expected.txt"
PROMPT='Insert value for converting (ex: meter:2.5): '

if [[ ! -x "$EXE" && ! -f "${EXE}.exe" ]]; then
  echo "실행 파일 없음: $EXE — cmake --build ${BUILD_DIR} --target unit_converter 후 재실행"
  exit 1
fi
[[ -x "$EXE" ]] || EXE="${EXE}.exe"

scenarios=( "meter:2.5" "feet:1.0" "yard:1.0" "meter:0.0" )
blocks=()

for input in "${scenarios[@]}"; do
  raw="$("$EXE" <<<"$input" 2>&1 || true)"
  lines=()
  while IFS= read -r line; do
    [[ -z "$line" ]] && continue
    if [[ "$line" == *"$PROMPT"* ]]; then
      remainder="${line#*"$PROMPT"}"
      [[ -n "$remainder" ]] && lines+=("$remainder")
      continue
    fi
    if [[ "$line" == *" = "* ]]; then
      lines+=("$line")
    fi
  done <<<"$raw"
  block="[${input}]"$'\n'"$(printf '%s\n' "${lines[@]}")"
  blocks+=("$block")
done

{
  for i in "${!blocks[@]}"; do
    printf '%s' "${blocks[$i]}"
    if [[ $i -lt $((${#blocks[@]} - 1)) ]]; then
      printf '\n---\n'
    fi
  done
  printf '\n'
} >"$OUTPUT"

echo "Golden Master 기준 파일 생성 완료: $OUTPUT"
echo "버전 관리: git add tests/golden_master_expected.txt"
