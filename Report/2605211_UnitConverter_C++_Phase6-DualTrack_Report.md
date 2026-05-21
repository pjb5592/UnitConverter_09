# UnitConverter (C++) — Phase 6 Dual-Track 작업 종합 보고서

| 항목 | 내용 |
|------|------|
| **파일명** | `Report/2605211_UnitConverter_C++_Phase6-DualTrack_Report.md` |
| **프로젝트** | `d:\DEV\UnitConverter_09` |
| **일자** | 2026-05-21 |
| **브랜치** | `refactoring` (최신: `a8f993a`) |
| **작업자** | pjb5592 \<pjb5592@gmail.com\> |
| **워크플로** | 리팩토링 우선 — RED → GREEN → REFACTOR |
| **정본** | `docs/PRD.md` v1.1 · `docs/TODO.md` · `docs/refactoring_plan.md` |

---

## 1. 작업 개요

### 1.1 프로젝트·브랜치·일정

| 항목 | 내용 |
|------|------|
| **프로젝트** | UnitConverter_09 — 길이 단위 변환 CLI (C++17, ECB) |
| **기간** | 2026-05-20 ~ 2026-05-21 |
| **주요 브랜치** | `red` → `green` → `refactoring` (현재 HEAD) |
| **작업자** | pjb5592 |
| **최종 검증** | `ctest` **9/9 PASS**, lcov 게이트 **PASS** |

### 1.2 단계별 브랜치 이력

| 단계 | 브랜치 | 대표 커밋·산출 | Report |
|------|--------|----------------|--------|
| 문서·골격 | `main` | PRD/README/TODO 정합 (TD-50~56) | `01`~`04` |
| RED | `red` | Dual-Track 12 TC, 링크 실패 확인 | `05` |
| GREEN (Logic) | `green` | TC-B-01, `UnitConverter` 최소 구현 | `06` |
| GREEN (완료) | `green` | Track A/B 12 TC PASS | `07` |
| REFACTOR·F02 | `green`/`refactoring` | Engine·Catalog·커버리지 게이트 | `08` |
| Golden Master | `green` | GM-TC-01~04, 승인·README | `09`~`11` |
| 사전 분석 | `refactoring` | 갭 분석·리팩토링 계획서 | `12` |
| REFACTOR·DoD | `refactoring` | CliApp, 13 REFACTOR 커밋, DoD | `13` |
| 커버리지 감사 | `refactoring` | Invariant·갭 분석 (코드 변경 없음) | `14` |

### 1.3 최종 아키텍처 (REFACTOR 후)

```
Boundary: CliApp, CliReader, InputParser, OutputFormatter, ErrorPresenter, ErrorCodes
Control:  ConversionUseCase (convertTable / convertDisplayTable / convertJson)
Entity:   UnitCatalog, UnitCatalogJson, LengthConversionEngine, DisplayRounder
Data:     ConfigLoader → UnitCatalogJson
main:     boundary::CliApp().run(cin, cout, cerr)
```

**R-01 dual contract (의도적 유지)**

| 경로 | API | 출력 소수 |
|------|-----|-----------|
| CLI / Golden Master | `convertDisplayTable` | **1자리** (ROUND-LOCK) |
| ui_track RED 계약 | `convertTable` | **6자리** |

---

## 2. 완료된 To-Do 항목 요약 (Phase 6 · `docs/TODO.md` 기준)

### 2.1 Phase 6 README 이관 (`PRD` 부록 C, TD-53)

| # | 항목 | 상태 |
|---|------|------|
| C-01~C-12 | ERR-*·NEG·SRC·BCE·커버리지·LOCK·등록·json/csv·AC·정본 | ✅ README 반영 완료 (TD-53) |

### 2.2 Must-Have (TD-01~18) — 구현·테스트 기준

| ID | 항목 | 상태 | 근거 |
|----|------|------|------|
| TD-01 | CMake·Catch2 골격 | ✅ | 9 CTest 타깃, Catch2 FetchContent |
| TD-02 | BCE 디렉터리·의존 방향 | ✅ | `src/{entity,control,boundary,data}` |
| TD-03 | config/units.json 3단위 | ✅ | `config/units.json` |
| TD-04 | 설정 로드 실패 전형 | 🟡 | JSON/YAML·T-04/T-06 일부; **중복 name** CLI E2E 미완 |
| TD-05 | UnitCatalog + meter 허브 | ✅ | `LengthConversionEngine`, F02 리터럴 0건 |
| TD-06 | if-체인 제거 (F-09) | ✅ | `rg` if(unit) 0건, 카탈로그 루프 |
| TD-07 | InputParser | ✅ | `parser_tests` 10 TC |
| TD-08 | 미등록 단위 | ✅ | TC-A-04, T-05 CliApp |
| TD-09 | ErrorPresenter JSON | 🟡 | unknown unit MESSAGE-LOCK; **전체 ERR-* JSON** 미완 |
| TD-10 | Table meter:2.5 | ✅ | GM-TC-01, T-01, T-02 |
| TD-11 | zero meter:0 | ✅ | GM-TC-04, `conversion_tests` 경계 |
| TD-12 | Control 배선 | ✅ | `CliApp` = main 진입 |
| TD-16 | ORDER-LOCK | ✅ | `UnitCatalog::units()` 순 = 출력 순 |
| TD-17 | feet:3.28084 LHS 3.3 | ✅ | GM-TC-02, ROUND+SRC |
| TD-18 | REGISTER 실패 경로 | 🟡 | `catalog_registration_tests` BT; **CLI REGISTER** 미구현 |

> `docs/TODO.md` 체크박스는 아직 `[ ]`이나, 위 표는 **코드·ctest 실측** 기준입니다.

### 2.3 Should-Have (TD-13~25)

| ID | 항목 | 상태 |
|----|------|------|
| TD-13 | REGISTER 성공 cubit | 🟡 엔진·카탈로그 테스트만 |
| TD-14 | JSON format | ✅ `convertJson` + TC-A-06 |
| TD-15 | CSV format | ⬜ 미구현 (v2/별도 스프린트) |
| TD-21 | Gherkin Sc.1~13 | 🟡 GM·단위 TC 일부; 전 시나리오 GREEN 아님 |
| TD-22 | format CLI | ⬜ |
| TD-23 | 커버리지 게이트 | ✅ `run_coverage_gate.ps1` PASS |
| TD-24 | REFACTOR + LOCK | ✅ 9/9 ctest, GM, 스냅샷 유지 |
| TD-25 | docs/gherkin.md | ✅ 초안 (TD-56) |

### 2.4 마일스톤 (`docs/TODO.md` §🗓️)

| M | 내용 | 상태 |
|---|------|------|
| M0 | TD-50~55, RED | 🟡 문서·RED 완료, TODO 체크 미갱신 |
| M1 | TD-01~12,16~18, AC 핵심 | 🟡 대부분 구현, TD-04/09/18·AC 전체 미인수 |
| M2 | TD-10,17,23, GM | 🟡 GM·커버리지 ✅ |
| M3 | TD-13~15,21~25 | ⬜ |
| M4 | R-01~12 회고 | 🟡 본 보고서 §9 포함 |

### 2.5 `refactoring_plan.md` Phase·T-ID

| Phase / ID | 내용 | 상태 |
|------------|------|------|
| Phase 0 R-01 | dual contract 문서화 | ✅ |
| T-01~T-07 | 출력·CliApp·JSON·config·EOF | ✅ |
| T-08 | 6자리 단일화 | N/A (dual 유지) |
| Phase 2 R-02, R-U1/3 | main→UseCase, Formatter | ✅ |
| Phase 3 R-U2 | ErrorPresenter | ✅ (범위 내) |
| Phase 4 R-07 | UnitConverter 제거 | ✅ |
| §6.7 DoD | 9/9·F02·GM·lcov | ✅ |

---

## 3. RED 단계 결과

### 3.1 작성·고정한 테스트 목록

#### Track A — `tests/unit/boundary/ui_track_red_tests.cpp` (7건)

| ID | TEST_CASE | RED 의도 |
|----|-----------|------------|
| T-01 | `T-01_convert_display_table_matches_golden_master_lines` | 1자리 display 계약 (REFACTOR 후 추가) |
| TC-A-01 | `TC-A-01_happy_meter_25_returns_conversion_lines` | 6자리 표 happy path |
| TC-A-02 | `TC-A-02_missing_colon_throws_invalid_argument` | `:` 없음 |
| TC-A-03 | `TC-A-03_negative_meter_throws_invalid_argument` | 음수 거절 |
| TC-A-04 | `TC-A-04_unknown_parsec_throws_invalid_argument` | unknown unit |
| TC-A-05 | `TC-A-05_output_preserves_source_unit_and_value` | SRC-LOCK |
| TC-A-06 | `TC-A-06_json_output_matches_schema` | JSON 스키마 |

#### Track B — `tests/unit/entity/logic_track_red_tests.cpp` (7건, Step 13에서 Engine 마이그레이션)

| ID | TEST_CASE | RED 의도 |
|----|-----------|------------|
| TC-B-01 | `TC-B-01_convert_meter_to_feet_within_1e5` | meter→feet ε |
| TC-B-02 | `TC-B-02_convert_meter_to_yard_within_1e5` | meter→yard |
| TC-B-03 | `TC-B-03_convert_all_returns_all_registered_units` | convertAll 3행 |
| T-03 | `T-03_register_unit_then_convert_all_uses_same_catalog` | R-07 catalog 일관성 |
| TC-B-04 | `TC-B-04_register_unit_then_convert` | registerUnit inch |
| TC-B-05 | `TC-B-05_load_config_valid_json_applies_ratios` | JSON 비율 |
| TC-B-06 | `TC-B-06_load_config_missing_path_keeps_defaults` | missing fallback |

#### Step 03 선행 RED (4 스위트, REFACTOR 전부터 존재)

| 스위트 | 대표 TC |
|--------|---------|
| `unit_conversion_tests` | meter/feet/yard, display, zero, 경계 13건 |
| `unit_boundary_tests` | `parser_tests` 10건 |
| `unit_catalog_tests` | `catalog_registration_tests` 8건 |
| `unit_config_tests` | `config_loader_tests` 8건 |

### 3.2 실패 확인 여부

| 시점 | build / ctest | RED 확인 |
|------|---------------|----------|
| **Step 05 (RED 전용)** | `unit_ui_red_tests`, `unit_logic_red_tests` **링크 실패** | ✅ 의도적 RED — `undefined reference` to `ConversionUseCase`, `UnitConverter`, `loadConfig` |
| Step 05 동시 | 기존 4 스위트 | 4/4 PASS (Engine·Parser 등 이미 GREEN) |
| **현재 (`refactoring`)** | 9/9 PASS | RED 테스트는 **GREEN 후에도 삭제·완화 없이** 통과 유지 |

**RED 링크 오류 요약 (Step 05)**

```
undefined reference to control::ConversionUseCase::*
undefined reference to entity::UnitConverter::*
undefined reference to data::ConfigLoader::loadConfig
```

---

## 4. GREEN 단계 결과

### 4.1 통과한 테스트

| 검증 시점 | ctest | 비고 |
|-----------|-------|------|
| Step 07 (`green`) | **6/6 PASS** | logic + ui RED 스위트 포함 |
| Step 08+ | 7/8+ (GM·catalog_json 추가) | F02·커버리지 |
| **현재 (`refactoring`)** | **9/9 PASS** | 아래 전체 |

**현재 CTest 목록 (2026-05-21 실측)**

```
unit_conversion_tests, unit_boundary_tests, unit_catalog_tests,
unit_config_tests, unit_catalog_json_tests, unit_logic_red_tests,
unit_ui_red_tests, unit_cli_integration_tests, GoldenMaster
```

### 4.2 GREEN 커밋 메시지 (Dual-Track, `green` 브랜치)

| 순번 | Track | TC | 커밋 | 메시지 |
|------|-------|-----|------|--------|
| 1 | B | TC-B-01 | `35a3570` | (Step 06) meter to feet |
| 2 | A | TC-A-02 | `6aae318` | validate missing colon |
| 3 | B | TC-B-02 | `83118fc` | meter to yard |
| 4 | A | TC-A-03 | `4c7a155` | validate negative value |
| 5 | B | TC-B-03† | `01bf238` | feet to meter reverse |
| 6 | A | TC-A-04 | `fa3dba7` | validate unknown unit |
| 7 | B | TC-B-03/04 | `aebb094` | convertAll and registerUnit |
| 8 | A | TC-A-01,05,06 | `2f44f9a` | boundary happy path |
| 9 | B | TC-B-05,06 | `86bcc09` | loadConfig with fallback |

† README TC-B-03(역변환) vs 테스트 태그 `TC-B-03`(convertAll) 번호 불일치 — Step 12 §8 참고.

**추가 GREEN·인프라 커밋**

| 커밋 | 메시지 |
|------|--------|
| `b45f77a` | feat(test): add CLI golden master approval regression |
| `4193f86` | refactor(green): F02 config catalog and coverage gate (Step 08) |
| `86bcc09` | feat(green): loadConfig with fallback |

### 4.3 수동·GM 앵커 (ROUND-LOCK 1자리)

```text
meter:2.5
→ 2.5 meter = 2.5 meter
→ 2.5 meter = 8.2 feet
→ 2.5 meter = 2.7 yard
```

---

## 5. Refactoring 결과

### 5.1 선택 항목 수행 내역 (`refactoring_plan.md` Phase 2~4)

| ID | 내용 | 상태 |
|----|------|------|
| R-02 | `main` → `ConversionUseCase` / `CliApp` | ✅ |
| R-U1 | `InputParser` extract, `parseConvertLine` | ✅ |
| R-U2 | `ErrorPresenter`, `ErrorCodes` | ✅ |
| R-U3 | `OutputFormatter`, `CliReader` | ✅ |
| R-07 / T-03 | catalog 일관성, `UnitConverter` 제거 | ✅ |
| R-01 | dual contract 문서·T-01 | ✅ |
| T-04 | `UnitCatalogJson` 예외·YAML | ✅ |
| T-02, T-05~T-07 | `cli_app_integration_tests` | ✅ |
| Phase 4 (선택) | `UnitDefinition.cpp` 빈 TU 제거 | ✅ |
| Phase 4 (선택) | ConfigLoader DRY → `UnitCatalogJson` | ✅ |
| Phase 4 (미선택) | `ConversionRule` VO (R-L1) | ⬜ |
| Phase 4 (미선택) | `ConversionUseCase` Extract Method (P2) | ⬜ |

### 5.2 주요 변경 파일

| 경로 | 변경 |
|------|------|
| `src/boundary/CliApp.{hpp,cpp}` | 신규 — thin boundary |
| `src/boundary/CliReader.{hpp,cpp}` | 신규 |
| `src/boundary/ErrorPresenter.{hpp,cpp}`, `ErrorCodes.hpp` | 신규 |
| `src/control/ConversionUseCase.*` | `convertDisplayTable`, `convertTable`, `convertJson` |
| `src/entity/UnitCatalogJson.*` | JSON/YAML·경로 |
| `src/entity/UnitConverter.*` | **삭제** |
| `src/main.cpp` | `CliApp().run` only |
| `tests/unit/boundary/cli_app_integration_tests.cpp` | 신규 |
| `tests/golden_master/GoldenMasterHelper.hpp` | CRLF normalize |
| `scripts/run_coverage_gate.ps1` | 신규 |

### 5.3 REFACTOR 커밋 메시지 (13+1, `refactoring` 브랜치)

| 커밋 | 메시지 |
|------|--------|
| `f01e18f` | refactor(domain): unify UnitConverter catalog and extract InputParser steps |
| `0866e6d` | refactor(control): thin main via convertDisplayTable and OutputFormatter |
| `c1ce18f` | refactor(data): DRY config load via UnitCatalogJson and add T-04 tests |
| `38b98c8` | refactor(boundary): add ErrorPresenter and move source value formatting |
| `3e3ac2a` | refactor(boundary): extract CliReader and InputParser value token |
| `858bc38` | refactor(domain): extract catalog helpers and display table formatting |
| `6fc9e49` | refactor(test): migrate logic_track to LengthConversionEngine |
| `90579b2` | refactor(boundary): unify parse path and table line formatting |
| `2d58f17` | refactor(domain): remove legacy UnitConverter facade |
| `995c811` | refactor(boundary): add CliApp and unify YAML config loading in entity |
| `0c75bc7` | refactor(boundary): document R-01 dual output contract and add T-01 tests |
| `daf132d` | test(boundary): add CliApp integration tests T-02 T-05 T-06 T-07 |
| `3d75d3f` | test(entity): raise catalog JSON coverage for lcov gate |
| `a8f993a` | docs(step13): dual-track REFACTOR DoD complete report and transcript |

### 5.4 회귀 테스트

| 검증 | 결과 |
|------|------|
| `ctest` 9/9 | ✅ PASS (실측 2026-05-21) |
| Golden Master GM-TC-01~04 | ✅ PASS |
| F02 `rg "3\.28084|1\.09361" src` | ✅ 0건 |
| RED 스위트 유지 | ✅ `unit_logic_red_tests`, `unit_ui_red_tests` PASS |
| Step 03 4스위트 | ✅ PASS |

---

## 6. 커버리지 현황 (레이어별)

**측정:** `scripts/run_coverage_gate.ps1` · `build-cov` · lcov (2026-05-21 재실행)

| 레이어 | Cover | PRD/게이트 최소 | 결과 |
|--------|-------|-----------------|------|
| **entity** (Domain) | **95.7%** | ≥95% | PASS |
| **boundary** | **94.8%** | ≥85% | PASS |
| **data** | **100.0%** | ≥90% | PASS |
| **control** | **100.0%** | ≥80% | PASS |
| **src 전체** | **96.2%** | ≥88% | PASS |
| **F02** (리터럴) | 0건 | 0건 | PASS |

### 6.1 파일별 미커버 갭 (Step 14 감사, Miss > 0)

| 파일 | Cover | 비고 |
|------|-------|------|
| `LengthConversionEngine.cpp` | 90.5% | `factor<=0` throw dead path |
| `UnitCatalogJson.cpp` | 95.2% | 비숫자 factor, YAML 분기 |
| `InputParser.cpp` | 93.5% | 빈 unit·특수문자 |
| `CliApp.cpp` | 92.0% | 3인자 `run`, 일부 catch |
| `OutputFormatter.cpp` | 97.2% | 정수 value token |

---

## 7. 미완료 항목 및 다음 단계 제안

### 7.1 미완료

| 영역 | 항목 |
|------|------|
| **PR/머지** | `refactoring` → `main` PR 미머지 |
| **TODO 문서** | `docs/TODO.md` M1~M3 체크박스 미갱신 |
| **AC 전체** | PRD AC-01~13 인수 완료 선언 전 |
| **CLI** | REGISTER·CSV·`--format`·QUIT (TD-13~15, TD-22, TD-30~32) |
| **CI** | GM GitHub Actions required check (README `[ ]`) |
| **문서** | README TC-A/B vs `ui_track`/`logic_track` ID 매핑표 (Step 12 §8) |
| **테스트 갭** | Step 14 제안 TC — CliApp 음수, `factor<=0` 엔진, InputParser 빈 unit |
| **선택 REFACTOR** | R-L1 `ConversionRule` VO, UseCase Extract Method |

### 7.2 다음 단계 (우선순위)

1. **`refactoring` → `main` PR** — 13 REFACTOR 커밋 리뷰·머지  
2. **`docs/TODO.md`·traceability-matrix** — 실측 완료 TD `[x]` 동기화  
3. **GM-07~08** — CI Golden Master required check  
4. **TD-04/09/18** — config 중복·Error JSON·REGISTER CLI (별도 스프린트)  
5. **Step 14 갭 TC** — boundary 94.8% → 여유 확보 (선택)  

---

## 8. 발견된 이슈 및 해결 방법

| ID | 이슈 | 해결 |
|----|------|------|
| DEF-001~004 | `LengthConversionEngine` stub → 변환 0 | 허브 `toCanonicalMeters` / `fromCanonicalMeters` 구현 |
| DEF-005 | Engine이 dangling `UnitCatalog&` | 멤버를 `UnitCatalog` 값 보관 |
| DEF-006~007 | REGISTER factor 역수·mile 스케일 | `factor = 1/ratio`, 테스트 Given 정합 |
| DEF-008 | display 1자리 미적용 | `DisplayRounder` + `convertDisplay` |
| DEF-009 | 레거시 `UnitConverter.cpp` | BCE 정본 분리; Step 13에서 퍼사드 **삭제** |
| R-01 | PRD 1자리 vs RED 6자리 충돌 | **dual contract** — `convertDisplayTable` vs `convertTable`, T-01·문서화 |
| R-07 | `UnitConverter` static vs `convertAll` 불일치 | Engine+Catalog 주입, 레거시 제거, T-03 |
| R-02 | `main`이 UseCase 우회 | `CliApp` + `convertDisplayTable` |
| Windows GM CRLF | 골든 파일 LF/CRLF 불일치 | `GoldenMasterHelper::normalizeLineEndings` |
| TC ID 불일치 | README TC-B-03/04 vs Catch2 태그 | 매핑표 작성 예정 (문서만) |
| lcov entity 95% 미달 | `UnitCatalogJson` 분기 미커버 | T-04 테스트 추가 (`3d75d3f`) → 95.7% |

---

## 9. 생성형 AI 활용 회고

### 9.1 도움이 된 순간

| 상황 | 효과 |
|------|------|
| **Dual-Track RED 설계** | Track A(6자리 UI)·Track B(Engine/Config) 계약을 Catch2 태그·Invariant 주석으로 한 번에 고정 |
| **REFACTOR 커밋 단위 분해** | R-02→R-U2→CliApp 순 13커밋 — 리뷰·bisect 용이 |
| **갭 분석 → 계획서** | Step 12 `refactoring_plan.md` — Phase·T-ID·DoD 체크리스트로 REFACTOR 범위 통제 |
| **커버리지 게이트 스크립트** | lcov 레이어 임계·F02 grep 자동화 — 수동 누락 방지 |
| **defect_list ↔ TC 매핑** | RED 결함 9건을 TC-B/Engine 라인과 연결해 GREEN 수정 순서 결정 |
| **기존 Report 01~14 누적** | 본 종합 보고서 작성 시 단계별 실측값 재현 비용 절감 |

### 9.2 한계

| 한계 | 대응 |
|------|------|
| **TC 번호 vs README 불일치** | AI가 태그만 맞추고 README TC-B-03 의미(역변환 vs convertAll) 혼동 — **매핑표 수동 고정** 필요 |
| **TODO.md 체크 미갱신** | 구현 완료와 문서 `[ ]` 불일치 — 인수 전 **사람이 TODO·matrix 동기화** |
| **과도한 범위 제안** | CSV·REGISTER CLI를 REFACTOR에 끼우려 함 — `refactoring_plan.md` 범위 외로 컷 |
| **dead path 커버리지** | `factor<=0` 엔진 throw는 설계상 unreachable — 100% 집착 시 불필요 TC 위험 |
| **Windows·인코딩** | PowerShell `&&` 미지원, GM CRLF — 환경 명시 후 스크립트 분리 |

### 9.3 TC 작성 팁 (재사용)

1. **RED 먼저, 링크 실패로 확인** — 구현 없이 `undefined reference`가 가장 명확한 RED 신호.  
2. **태그 3종** — `[red][track-a][TC-A-01]` + Invariant ID → 필터·추적 용이.  
3. **Given/When/Then 주석** — AI·리뷰어가 계약 의도를 동일하게 읽음.  
4. **LOCK은 문자열 exact** — `8.202100`, `"Unknown unit: parsec"` 등 부분 매칭보다 전문 lock.  
5. **dual contract는 테스트 2개** — T-01(1자리) + TC-A-01(6자리)로 분리; 하나로 합치지 말 것.  
6. **통합은 CliApp 스트림** — subprocess 없이 `istringstream`/`ostringstream`으로 T-02/T-05~07.  
7. **REFACTOR 후 RED 삭제 금지** — G-04; RED 스위트가 회귀 방지망.  
8. **골든은 파일+CRLF normalize** — OS 차이를 Helper 한곳에서 처리.

---

## 10. 부록

### 10.1 관련 산출물 경로

| 유형 | 경로 |
|------|------|
| 본 보고서 | `Report/2605211_UnitConverter_C++_Phase6-DualTrack_Report.md` |
| 단계별 Report | `Report/01` ~ `Report/14` |
| 리팩토링 계획 | `docs/refactoring_plan.md` |
| 결함 목록 | `docs/defect_list.md` |
| Transcript | `Prompt/full-transcript-refactor-first-2026-05-21_prompt.md` |

### 10.2 재현 명령 (PowerShell)

```powershell
Set-Location "d:\DEV\UnitConverter_09"
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
powershell -ExecutionPolicy Bypass -File scripts/run_coverage_gate.ps1
```

---

*문서 생성: 2026-05-21 · 브랜치 `refactoring` · `ctest` 9/9 · lcov 게이트 PASS 기준.*
