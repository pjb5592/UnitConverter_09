# 결함 목록 (Defect List)

| 항목 | 내용 |
|------|------|
| **문서 버전** | 1.0 |
| **기준일** | 2026-05-21 |
| **워크플로** | Dual-Track TDD · RED → GREEN |
| **정본 계약** | [PRD.md](PRD.md) v1.1 · [test_plan.md](test_plan.md) |
| **회귀 상태** | `ctest` 4/4 PASS (36 TEST_CASE) |

> RED 단계에서 Catch2·수동 검증으로 확인된 결함입니다.  
> **상태:** 아래 항목은 GREEN 단계에서 수정 완료되었으며, 현재 빌드·테스트는 통과합니다.

---

## 결함 테이블

| ID | Severity | 변환 타입 | 재현 절차 | 기대값 | 실제값 | 근본 원인 | 수정 요약 |
|----|----------|-----------|-----------|--------|--------|-----------|-----------|
| DEF-001 | Critical | meter→feet | `unit_conversion_tests` 실행 · `convert("meter", 2.5, "feet")` (TC-B-01) | 8.202100 (ε≤1e-5) | 0.000000 | `LengthConversionEngine::convert` 미구현(stub)·허브 경로(`toCanonicalMeters`→`fromCanonicalMeters`) 미연결 | `convert`에서 `meters = value/factor(from)` 후 `meters*factor(to)` 반환 (`LengthConversionEngine.cpp:24-28`) |
| DEF-002 | Critical | meter→yard | 동일 스위트 · `convert("meter", 2.5, "yard")` (TC-B-02) | 2.734025 | 0.000000 | DEF-001과 동일(전 단위 변환 무력화) | DEF-001과 동일 허브 2단계 구현 |
| DEF-003 | Critical | feet→meter | `convert("feet", 1.0, "meter")` (TC-B-03) | 0.30480 | 0.000000 | 역변환 시 canonical 분기 누락·곱셈만 사용 등 허브 역산 미적용 | `toCanonicalMeters`: `value/factor` · `fromCanonicalMeters`: `meters*factor` (`LengthConversionEngine.cpp:11-22`) |
| DEF-004 | Critical | convertAll | `convertAll("meter", 0.0)` (TC-B-04·경계) | 3행·각 target 0.0 | 0행 또는 비정상 값 | `convertAll` 미구현·카탈로그 순회 없음 | 카탈로그 `units()` 순회하며 `convert` 호출 (`LengthConversionEngine.cpp:35-42`) |
| DEF-005 | Critical | (런타임) cubit→meter | `catalog_registration_tests` · `makeEngine()` 후 `convert("cubit", 1.0, "meter")` | 0.4572 | 세그폴트·임의값 | `LengthConversionEngine`이 `const UnitCatalog&`만 보관 · `withDefaultUnits()` 임시 객체 소멸 후 dangling reference | 멤버를 `UnitCatalog catalog_` 값 보관으로 변경 (`LengthConversionEngine.hpp:26`) |
| DEF-006 | Major | cubit→meter | `registerUnit("cubit", 0.4572)` 직접 등록 후 변환 | 0.4572 | ≈2.19 (역수 오류) | REGISTER 문법(1 cubit = 0.4572 m)과 허브 factor(목표 단위 per meter) 혼동 | 등록 시 `factor = 1.0/0.4572` 저장 · 테스트 Given 주석 정합 (`catalog_registration_tests.cpp:13-15`) |
| DEF-007 | Major | mile→feet | `registerUnit("mile", 1609.34)` 후 `convert("mile", 1609.34, "feet")` | 3.28084 (≈1 m) | 스케일 오류 | mile factor를 “meter 수”가 아닌 허브 배율로 해석하지 않음 | `mile` hub factor `1609.34` · 입력 `1609.34`로 1 canonical meter 재현 (`catalog_registration_tests.cpp:74-85`) |
| DEF-008 | Minor | meter→feet (표시) | `convertDisplay("meter", 2.5, "feet")` (AC-02·ROUND-LOCK) | 8.2 | 8.20210 (raw) 또는 0.0 | `convertDisplay`·`roundOneDecimal` 미연결 | `roundOneDecimal(convert(...))` (`LengthConversionEngine.cpp:30-33`, `DisplayRounder.cpp`) |
| DEF-009 | Info | meter→feet (레거시) | 루트 `UnitConverter.cpp` 빌드 · `meter:2.5` stdin | 8.2 feet (1자리) | 8.20210… (미반올림) | 레거시 단일 파일·`if-else` 분기·ROUND-LOCK 미적용 (PRD 비정본) | BCE `src/` 정본 사용 · 레거시는 Act.1 분석용만 유지 (NG-05) |

---

## TC 추적 (README RED 체크리스트)

| 결함 ID | 연관 TC | Track |
|---------|---------|-------|
| DEF-001 | TC-B-01 | B — Domain |
| DEF-002 | TC-B-02 | B |
| DEF-003 | TC-B-03 | B |
| DEF-004 | TC-B-04 | B |
| DEF-005, DEF-006 | TC-B-05 | B |
| DEF-008 | TC-A-06 (표시)·AC-02 | A / 인수 |
| DEF-009 | — (레거시, 테스트 대상 외) | — |

---

## 재현·Green 확인 (PowerShell)

```powershell
Set-Location "d:\DEV\UnitConverter_09"
cmake -S . -B build
cmake --build build
Set-Location build
ctest --output-on-failure
```

**기대:** `100% tests passed, 0 tests failed out of 4`

수동 앵커 (CLI):

```text
meter:2.5
```

**기대 stdout (ROUND-LOCK):**

```text
2.5 meter = 2.5 meter
2.5 meter = 8.2 feet
2.5 meter = 2.7 yard
```

---

## 변경 이력

| 날짜 | 변경 |
|------|------|
| 2026-05-21 | v1.0 초안 — RED 발견 9건 기록, GREEN 수정 반영 |
