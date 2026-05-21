# 리팩토링 계획서 — UnitConverter_09

| 항목 | 내용 |
|------|------|
| **문서 버전** | 1.0 |
| **작성** | Step 12 `pre-refactor-test-gap-analysis` 산출 |
| **일자** | 2026-05-21 |
| **워크플로** | 리팩토링 우선 (RED → GREEN 완료 후 REFACTOR) |
| **정본** | [PRD.md](PRD.md) · [test_plan.md](test_plan.md) · [.cursorrules](../.cursorrules) |
| **관련 Report** | `Report/12.pre-refactor-test-gap-analysis-report-2026-05-21.md` |

---

## 1. 목적·범위

### 1.1 목적

- **ECB(Entity–Control–Boundary)** 레이어를 계약에 맞게 정렬한다.
- `main()`의 SRP 위반·이중 오케스트레이션(`ConversionUseCase` 미사용)을 제거한다.
- 신규 단위 추가 시 **설정(JSON) 등록만**으로 동작하도록 OCP를 유지한다.
- REFACTOR 전후 **Catch2 + Golden Master**로 회귀를 잡는다.

### 1.2 범위

| 포함 | 제외 |
|------|------|
| `main` → UseCase 위임, 출력 계약 통일, `UnitConverter` 정리 | CSV·REGISTER CLI (P2, 별도 스프린트) |
| 에러·설정 경로 정합, `UnitCatalogJson` 테스트 보강 | PRD MESSAGE-LOCK 전면 변경(별도 PR) |
| F02 비율 단일 출처 유지 | 레거시 `UnitConverter.cpp` 인수 정본화 (NG-05) |

### 1.3 현재 아키텍처 스냅샷

```
Boundary   InputParser, OutputFormatter  (+ main이 I/O·포맷 일부 직접 수행)
Control    ConversionUseCase             (구현됨, main 미사용)
Entity     UnitCatalog, LengthConversionEngine, DisplayRounder
Data       ConfigLoader, UnitCatalogJson → config/units.json
```

**핵심 갭**

| ID | 문제 |
|----|------|
| R-01 | PRD/GM **1자리** vs `ui_track`/`OutputFormatter` **6자리** 이중 계약 |
| R-02 | `main`이 `ConversionUseCase`·`OutputFormatter` 우회 |
| R-07 | `UnitConverter`: `convert`/`registerUnit` = static catalog, `convertAll` = 매번 default |

**이미 충족 (신규 설계 최소화)**

- 단위별 `if (unit == "feet")` 체인 **없음** → `UnitCatalog` 루프 (R-L2)
- `src`에 `3.28084` / `1.09361` 리터럴 **없음** → `config/units.json` (R-L3, F02)
- meter 허브 환산 → `LengthConversionEngine::convert` (R-L4)

---

## 2. 리팩토링 대상 목록 (우선순위 순)

| 순번 | 대상 | 문제 | 적용 기법 | 우선순위 |
|------|------|------|-----------|----------|
| 1 | **출력 계약 통일** | GM 1자리 vs UseCase 6자리 | 전략 패턴 (`TableFormatter`), 계약 결정 후 단일 Formatter | **P0** |
| 2 | **`main.cpp`** | SRP 위반·Control/Boundary 혼재 | Thin Controller — `ConversionUseCase` 위임 | **P0** |
| 3 | **`entity/UnitConverter.cpp`** | static catalog vs `convertAll` 불일치 | Catalog 주입·퍼사드 Deprecate/정리 | **P0** |
| 4 | **에러 처리** (`main`, Parser, ConfigLoader) | stderr vs 예외, `ConfigLoadError` 삼키기, `stod` 미처리 | `ErrorPresenter`, 구체 예외 (MESSAGE-LOCK) | **P1** |
| 5 | **`ConfigLoader` + `UnitCatalogJson`** | `readFile`/regex 중복, YAML/JSON 이원화 | `UnitConfigRepository` Extract Class | **P1** |
| 6 | **`ConversionUseCase.cpp`** | `convertTable`/`convertJson` 중복, 매직 5/6 | Extract Method, Named Constant | **P2** |
| 7 | **`InputParser.cpp`** | `parse()` 33줄, 광범위 `catch` | Extract Method, 구체 예외 | **P2** |
| 8 | **`UnitDefinition` + 카탈로그** | 빈 `.cpp` TU, VO 빈약 | VO 강화(선택), 헤더-only | **P3** |
| 9 | **레거시 `UnitConverter` API** | CLI 미사용, 테스트만 의존 | Deprecate → Engine + Catalog | **P3** |

---

## 3. 리팩토링 후보 목록 (R-U / R-L)

| ID | 제안 | 현재 상태 | 계획 조치 | Phase |
|----|------|-----------|-----------|-------|
| **R-U1** | 입력 파싱 → `InputParser` | 부분 완료 | `main` → `parseConvertLine` only | 2 |
| **R-U2** | 예외 메시지·코드 상수화 | 미완 | `ErrorCodes` + `ErrorPresenter` | 3 |
| **R-U3** | 출력 → `OutputFormatter` | 부분 완료 | `main` 루프 제거, `convertTable` + write | 2 |
| **R-L1** | 비율 → `ConversionRule` VO | `UnitDefinition` 존재 | 검증을 VO로 이동(선택) | 4 |
| **R-L2** | if-else → `UnitRegistry` | **충족** (`UnitCatalog`) | static catalog 일원화만 | — |
| **R-L3** | 매직 3.28084/1.09361 → 설정 | **충족** (`units.json`) | F02 grep 회귀 유지 | — |
| **R-L4** | `convert()` 허브 단일화 | **충족** (`LengthConversionEngine`) | `UnitConverter` 위임 정리 | 4 |

### 3.1 ECB 목표 구조

```
┌─────────────────────────────────────────┐
│ Boundary: CliApp                         │
│  stdin/stdout/stderr, format, errors     │
└──────────────┬──────────────────────────┘
               │
┌──────────────▼──────────────────────────┐
│ Control: ConversionUseCase               │
│  parseConvertLine → convertAll/loop      │
│  OutputFormatter                         │
└──────────────┬──────────────────────────┘
               │
┌──────────────▼──────────────────────────┐
│ Entity: UnitCatalog + Engine + Rounder   │
└──────────────▲──────────────────────────┘
               │
┌──────────────┴──────────────────────────┐
│ Data: ConfigLoader / UnitCatalogJson     │
└─────────────────────────────────────────┘
```

### 3.2 `main()` 이동 매핑

| `main` 현재 책임 | 이동 대상 | 레이어 |
|------------------|-----------|--------|
| `loadFromJson("config/units.json")` | composition root → `ConversionUseCase(catalog)` | Data + Boundary |
| `getline`, 프롬프트 | `CliReader` (신규 또는 Boundary) | Boundary |
| `parse` + `hasUnit` | `parseConvertLine` | Boundary + Control |
| `roundOneDecimal` + `convertDisplay` 루프 | `convertTable` | Control → Entity |
| `cout` 줄 조립 | `OutputFormatter::formatTableLine` | Boundary |
| `catch` + `cerr` | `ErrorPresenter` | Boundary |

### 3.3 신규 단위 추가 (OCP)

| 변경 | 필수 |
|------|------|
| `config/units.json`에 `{ name, factorToMeter }` | ✅ |
| `src` 소스·단위 if-else | ❌ |
| 테스트·골든 마스터 기대값 | ✅ (계약 고정) |

---

## 4. 단계별 실행 계획

### Phase 0 — 게이트 (코드 변경 전)

| 작업 | 산출 |
|------|------|
| **R-01** 결정 | 정본: 1자리(PRD/GM) vs 6자리(RED) — 문서·테스트 동시 정렬 |
| TC ID 매핑표 | README TC-A/B ↔ `ui_track`/`logic_track` 태그 정리 |
| 베이스라인 | `ctest` 기록 (단위 6/6 + GM) |

### Phase 1 — 테스트 선행 (RED → GREEN)

「5. 테스트 선행 필요 항목」 구현 후 Phase 2 진입.

### Phase 2 — ECB 얇은 main (P0)

1. `main` → `ConversionUseCase::convertTable`
2. stdout → `OutputFormatter` (R-U3)
3. `parseConvertLine` (R-U1)
4. Golden Master 4시나리오 PASS (R-08, CRLF 정합 포함)

### Phase 3 — 에러·설정 (P1)

1. R-U2 `ErrorPresenter` + MESSAGE-LOCK
2. 설정 경로 `resolveDefaultUnitsJsonPath` 통일
3. `UnitCatalogJson` `stod` → `CatalogJsonError`
4. CLI 실패 E2E (unknown, bad config, EOF)

### Phase 4 — 정리 (P2~P3)

1. `UnitConverter` static 제거·테스트 이전
2. `ConversionUseCase` 중복 Extract Method
3. ConfigLoader / UnitCatalogJson DRY
4. (선택) `UnitDefinition.cpp` 제거

---

## 5. 테스트 선행 필요 항목

리팩터 **전** 또는 **Phase 2 직전** Catch2 RED → GREEN.

| ID | 항목 | 목적 | 추가 위치(안) |
|----|------|------|----------------|
| **T-01** | 출력 계약 스냅샷 | R-01 결정 후 table 줄 전문 lock | `ui_track` 또는 integration |
| **T-02** | `main` 경로 E2E | UseCase 통합 후 CLI 회귀 | GM 확장 / subprocess IT |
| **T-03** | `registerUnit` → `convertAll` | R-07 | `logic_track` 또는 `catalog_registration` |
| **T-04** | `UnitCatalogJson` content | invalid JSON, factor≤0 | `unit_catalog_json_tests` (신규) |
| **T-05** | CLI `parsec:1.0` | unknown unit exit·0줄 | GM helper / boundary IT |
| **T-06** | CLI bad/missing config | AC-04 | config IT |
| **T-07** | stdin EOF | getline 실패 exit 1 | boundary IT |
| **T-08** | ui_track 6자리 정렬 | R-01=1자리 시 | `ui_track_red_tests` 갱신 |

**유지 (삭제·완화 금지)**

- `parser_tests`, `conversion_tests`, `catalog_registration_tests`, `config_loader_tests`
- `logic_track_red_tests`, `ui_track_red_tests`
- Golden Master GM-TC-01~04

---

## 6. 리팩토링 후 검증 방법

### 6.1 빌드

```powershell
cd D:\DEV\UnitConverter_09
cmake -S . -B build
cmake --build build
```

### 6.2 단위·RED (6 스위트)

```powershell
ctest --test-dir build --output-on-failure
```

| CTest | 대상 |
|-------|------|
| `unit_conversion_tests` | Engine, DisplayRounder |
| `unit_boundary_tests` | InputParser |
| `unit_catalog_tests` | UnitCatalog |
| `unit_config_tests` | ConfigLoader |
| `unit_logic_red_tests` | UnitConverter 퍼사드 |
| `unit_ui_red_tests` | ConversionUseCase |

### 6.3 Golden Master (필수)

```powershell
ctest --test-dir build -R GoldenMaster --output-on-failure
```

### 6.4 F02 정적 점검

```powershell
rg "3\.28084|1\.09361" src
```

(매칭 0건 기대)

### 6.5 수동 스모크

```powershell
echo meter:2.5 | .\build\unit_converter.exe
```

1자리 계약 시: `tests/golden_master_expected.txt`와 동일 3줄.

### 6.6 커버리지 게이트 (릴리스)

| 레이어 | 최소 % |
|--------|--------|
| entity | 95 |
| boundary | 85 |
| data | 90 |
| control | 80 |
| overall | 88 |

### 6.7 Definition of Done

- [x] `ctest` 9/9 PASS (8 unit + `unit_cli_integration_tests` + GoldenMaster)
- [x] Phase 해당 T-01~T-07 PASS (T-08 N/A — R-01 dual contract: ui 6자리 + CLI/GM 1자리)
- [x] F02 `src` grep 클린
- [x] README GM-09: REFACTOR 후 GM 재실행 ✅
- [x] lcov 게이트 — `scripts/run_coverage_gate.ps1` (entity ≥95%, boundary ≥85%, data ≥90%, control ≥80%, src ≥88%)

---

## 7. 리스크·주의사항

| 리스크 | 완화 |
|--------|------|
| R-01 없이 Phase 2 | GM vs ui_track 동시 실패 → T-01 선행 |
| `UnitConverter` 테스트 깨짐 | Catalog 주입 단계적 이전 |
| MESSAGE-LOCK 변경 | PRD bump + Catch2 + Gherkin 동시 PR |
| Windows GM CRLF | `GoldenMasterHelper` 정규화 또는 baseline LF 통일 |
| `loadWithDefaultsOnMissing` 변경 | TC-B-07·`config_loader_tests` 유지 |

---

## 8. README TC 번호 정리 (참고)

문서·테스트 태그 불일치 — REFACTOR 전 매핑표 고정 권고.

| README | README 설명 | 실제 테스트 |
|--------|-------------|-------------|
| TC-A-05 | `meter:abc` | `parser_tests` / ui TC-A-05 = SRC-LOCK |
| TC-A-06 | LHS 보존 | ui TC-A-06 = JSON |
| TC-B-03 | feet→meter | `conversion_tests` / logic TC-B-03 = convertAll |
| TC-B-04 | convertAll | logic TC-B-04 = registerUnit inch |

---

## 9. 관련 문서

| 문서 | 용도 |
|------|------|
| [test_plan.md](test_plan.md) | P0 BT/DT/IT, 경계 시나리오 |
| [defect_list.md](defect_list.md) | RED 결함 이력 |
| [traceability-matrix.md](traceability-matrix.md) | AC 추적 |
| [../Report/12.pre-refactor-test-gap-analysis-report-2026-05-21.md](../Report/12.pre-refactor-test-gap-analysis-report-2026-05-21.md) | Step 12 실행 보고 |

---

## 10. 요약

| 구분 | 내용 |
|------|------|
| P0 | R-01 출력 계약 · R-02 main→UseCase · R-07 UnitConverter · R-08 GM |
| 이미 있음 | R-L2 Registry, R-L3 JSON, R-L4 허브, R-U1/R-U3 부분 |
| 안전한 첫 리팩터 | `LengthConversionEngine`, `InputParser` |
| 위험 | `main`, `UnitCatalogJson`, 계약 미결정 상태의 Formatter |

**다음 액션:** Phase 0 R-01 결정 → T-03/T-04 RED → Phase 2 `main` 통합.
