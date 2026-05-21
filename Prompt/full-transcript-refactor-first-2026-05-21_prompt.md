# Full Transcript — refactor-first workflow

**프로젝트:** UnitConverter_09  
**갱신:** 2026-05-21  
**브랜치:** green

---

## 세션 요약 (누적)

| Step | slug | 산출 | 상태 |
|------|------|------|------|
| — | problem-definition | Epic·Invariant·Mom Test | 대화 |
| — | bce-design | BCE·Dual-Track·계약 | 대화 |
| — | red-test-list | Catch2 RED 45 TC 제목 | 대화 |
| — | cursorrules | `.cursorrules` YAML | ✅ |
| **01** | **docs-alignment-prd-readme** | PRD·README·TODO·traceability·갭 반영 | ✅ |
| **02** | **dev-strategy-branch-test-sample** | Git 브랜치 전략·테스트 플랜 샘플(AC-02) | ✅ |
| **03** | **catch2-bce-tests-green** | test_plan·BCE·Catch2 36 TC·ctest GREEN | ✅ |
| **04** | **red-defect-list** | defect_list.md·README 결함 체크·회귀 확인 | ✅ |
| **05** | **dual-track-red-tests** | Track A/B RED 12 TC·계약 헤더·링크 RED | ✅ |
| **06** | **track-b-green-tc-b-01** | `UnitConverter` meter→feet GREEN·TC-B-01 PASS | ✅ |
| **07** | **dual-track-green-complete** | Track A/B 12 TC GREEN·ctest 6/6·9 feat 커밋 | ✅ |
| **08** | **refactor-coverage-f02-gate** | F02·Engine 위임·Domain 96.3%·Boundary 94.6% | ✅ |
| **09** | **golden-master-approval-regression** | CLI stdout Golden Master·approve·ctest 7/7 | ✅ |
| **10** | **golden-master-gm-tc** | GM-TC-01~04·UnitConverter_test·GoldenMaster ctest | ✅ |
| **11** | **readme-golden-master-section** | README GM-01~09 체크리스트·목차 | ✅ |

---

## Step 11 상세 (2026-05-21)

**User:** README에 `## Golden Master 회귀 안전장치` (GM-01~09) — RED To-Do 아래 삽입.

**Assistant:** 목차 링크 + 섹션 본문 추가. GM-01~06은 Step 09~10 코드 완료, 문서는 학습용 `[ ]` 유지.

**검증:** 문서만 변경 · ctest **7/7** 회귀 유지 ✅

**다음:** GM-07 CI workflow · GM-08 required check · README `[x]` 동기화.

---

## Step 10 상세 (2026-05-21)

**User:** Golden Master 테스트 코드 — `input.txt`/`actual.txt` 캡처·섹션별 비교·TEST_F 스타일·`GoldenMaster` ctest·unified diff.

**Assistant:**
- `tests/test_golden_master.cpp` — `GoldenMasterTest` + GM-TC-01~04 (`TEST_CASE_METHOD`)
- `GoldenMasterHelper.hpp` — `extractSection`, `unifiedDiff`, per-scenario work dir
- CMake `UnitConverter_test`, `add_test(NAME GoldenMaster ...)`
- `.gitignore` work/ · 삭제 `golden_master_tests.cpp` (통합 1건)

**검증:** build ✅ · `ctest -R GoldenMaster` ✅ (4 cases, 24 assertions) · ctest **7/7** ✅

**다음:** README GM 절차 · IT·Gherkin · `green`→`main` PR.

---

## Step 09 상세 (2026-05-21)

**User:** Golden Master(Approval) — 4시나리오 기준 파일·approve 패턴·생성 스크립트·한글 설명.

**Assistant:**
- `tests/golden_master_expected.txt` — `[입력]` 블록·`---` 구분·CLI ROUND-LOCK(1자리) 실측
- `GoldenMasterHelper.hpp` — 캡처·diff·approve
- `golden_master_tests.cpp` — 기준 없으면 생성, 있으면 비교·FAIL+diff
- `scripts/generate_golden_master.ps1` / `.sh`
- CMake `unit_golden_master_tests` (ctest #7)

**User:** `커밋해줘` → **`b45f77a`** feat(test) golden master (6 files).

**검증:** build ✅ · ctest **7/7 PASS** · approve 자동 생성 ✅ · 스크립트 재생성 ✅

**다음:** `.gitignore` golden 잔여 · README 절차 · IT·Gherkin · `green`→`main` PR.

---

## Step 08 상세 (2026-05-21)

**User:** TC-A/B 전체·ctest·lcov 게이트( Domain ≥95% · Boundary ≥85% · F02 · main 분리) 점검.

**Assistant (점검):** ctest 6/6·TC 전부 PASS. Domain **89%**·F02 **미달** (`LengthConversionRatios`, `withDefaultUnits` 리터럴). Boundary·`src/main.cpp` OK.

**User:** `진행`

**Assistant (REFACTOR):**
- `UnitCatalogJson` — `config/units.json` 단일 출처
- `UnitConverter` → `LengthConversionEngine` 위임; `LengthConversionRatios.hpp` 삭제
- `ConfigLoader` JSON 파서 entity 위임; 레거시 `UnitConverter.cpp` 스텁
- 커버리지 테스트: DisplayRounder 비유한수, `factorOf` unknown

**검증:** build ✅ · ctest **6/6** · Domain **96.3%** · Boundary **94.6%** · `src` 리터럴 0건

**다음:** ErrorPresenter · README/defect_list · IT · `green`→`main` PR.

---

## Step 07 상세 (2026-05-21)

**User:** 커밋 순서표 2~9번 — Dual-Track 교차 GREEN(TC별 최소 구현·커밋) 반복 `진행` → [P] Report/Prompt Export·push.

**Assistant:**
- Track A: `ConversionUseCase`·`OutputFormatter` — 검증(TC-A-02~04)·happy/JSON/SRC-LOCK(TC-A-01,05,06)
- Track B: `UnitConverter` 확장 — yard·역변환·convertAll·registerUnit·`loadConfig` fallback
- CMake: `unit_control`, `OutputFormatter.cpp`
- **9 feat(green) 커밋** (`35a3570`~`86bcc09`, Step 06 포함)

**검증:** build ✅ · ctest **6/6 PASS** · `[track-a]` 6 TC · `[track-b]` 6 TC

**다음:** REFACTOR(`UnitConverter`→Engine 위임)·ErrorPresenter·README/defect_list·커버리지·`green`→`main` PR.

---

## Step 06 상세 (2026-05-21)

**User:** TDD GREEN만 — TC-B-01 `convert("meter",2.5,"feet")` ≈ 8.20210 (ε 1e-5); 비율 상수화; REFACTOR·타 TC 금지.

**Assistant:**
- RED: `UnitConverter.cpp` 없음 → 링크 RED (`unit_logic_red_tests`)
- `LengthConversionRatios.hpp` — `kFeetPerMeter`, `kYardPerMeter`
- `UnitConverter.cpp` — meter→feet 분기만; `convertAll`/`registerUnit` 스텁
- `ConfigLoader::loadConfig` 링크 스텁 (`empty()`)
- CMake `unit_entity`에 `UnitConverter.cpp` 추가

**검증:** build ⚠️ (ui RED 링크 실패) · ctest 4/6 PASS · `[TC-B-01]` ✅ · `[TC-B-02]` FAIL(의도)

**다음:** TC-B-02 GREEN → 허브 위임 REFACTOR → Track A `ConversionUseCase` GREEN.

---

## Step 05 상세 (2026-05-21)

**User:** Dual-Track RED 전용 — Track A(UI) 6건 + Track B(Logic) 6건; 구현/GREEN/REFACTOR 금지; 6자리 출력·`invalid_argument` 계약.

**Assistant:**
- `ui_track_red_tests.cpp` — `ConversionUseCase` (TC-A-01~06, INV-UI-01~06)
- `logic_track_red_tests.cpp` — `UnitConverter`·`loadConfig` (TC-B-01~06, INV-D01~05)
- 선언만: `ConversionUseCase.hpp`, `OutputFormatter.hpp`, `UnitConverter.hpp`, `loadConfig()`
- CMake RED 스위트 2개; **링크 RED** (undefined reference)

**검증:** build ⚠️ RED 2타깃 링크 실패 · ctest 4/6 PASS(기존 GREEN) · CLI ✅

**다음:** GREEN Logic(`UnitConverter`·`loadConfig`) → GREEN UI(`ConversionUseCase`) → PRD ROUND vs 6자리 LOCK 정합.

---

## Step 04 상세 (2026-05-21)

**User:** QA 엔지니어 결함 분석(로그 미첨부) → QA 리드 `defect_list.md` + README 결함 체크 → [P] Export/git.

**Assistant:**
- RED 발견 결함 DEF-001~009 문서화 (`docs/defect_list.md`)
- README 결함 목록 연결 `[x]` · 관련 문서 링크
- 로컬 `ctest` 4/4 PASS (회귀 확인)
- Report/04.*, Prompt/04.*

**검증:** build ✅(변경 없음) · ctest ✅ · 문서 정합 ✅

**다음:** README Track A/B `[x]` 동기화 · REFACTOR(ErrorPresenter) · IT·gcov.

---

## Step 03 상세 (2026-05-21)

**User:** test_plan.md 작성 → README RED To-Do → Catch2 유형별 5+ 테스트·cmake Green → [P] Export/git.

**Assistant:**
- `docs/test_plan.md`, README RED 체크리스트
- `CMakeLists.txt`, `src/` BCE, `tests/` 36 TEST_CASE
- `config/units.json`, `.gitignore`
- ctest 4/4 PASS; CLI `meter:2.5` PRD 3줄
- Engine catalog 값 보관(세그폴트 수정)

**검증:** build ✅ · ctest ✅ · 수동 stdout ✅

**다음:** REFACTOR(ErrorPresenter)·IT·gcov 게이트·`green` 브랜치 merge.

---

## Step 02 상세 (2026-05-21)

**User:** Dual-Track TDD용 git 브랜치 전략 → README/PRD 기반 테스트 플랜 샘플 1건(`meter:2.5`) → [P] Export/git.

**Assistant:**
- `main`/`spec`/`red`/`green`/`refactor` + `feat/red|green|refactor/{logic|ui}/TD-##`
- 샘플: F-01·AC-02·G-03, 입력 `meter:2.5`, 출력 8.2 feet / 2.7 yard (ROUND-LOCK)
- Report/02.*, Prompt/02.*

**검증:** 문서·전략만; build/ctest N/A.

---

## Step 01 상세 (2026-05-20)

**User:** README vs PRD 검토 →「모두 반영」코드 금지 → requirements.md 복원 → [P] Export/git.

**Assistant:** PRD·README·TODO·traceability·gherkin 정합; Report/01.*

**검증:** 문서만; build/ctest N/A.

---

## 정본 문서 체인

```
docs/PRD.md
    → docs/test_plan.md (Step 03)
    → README.md (+ RED To-Do)
    → docs/defect_list.md (Step 04)
    → src/ + tests/ + config/units.json (인수 정본)
```

---

## Export 인덱스

| Step | Report | Transcript |
|------|--------|------------|
| 01 | `Report/01.docs-alignment-prd-readme-report-2026-05-20.md` | `Prompt/01.docs-alignment-prd-readme-transcript-2026-05-20_prompt.md` |
| 02 | `Report/02.dev-strategy-branch-test-sample-report-2026-05-21.md` | `Prompt/02.dev-strategy-branch-test-sample-transcript-2026-05-21_prompt.md` |
| 03 | `Report/03.catch2-bce-tests-green-report-2026-05-21.md` | `Prompt/03.catch2-bce-tests-green-transcript-2026-05-21_prompt.md` |
| 04 | `Report/04.red-defect-list-report-2026-05-21.md` | `Prompt/04.red-defect-list-transcript-2026-05-21_prompt.md` |
| 05 | `Report/05.dual-track-red-tests-report-2026-05-21.md` | `Prompt/05.dual-track-red-tests-transcript-2026-05-21_prompt.md` |
| 06 | `Report/06.track-b-green-tc-b-01-report-2026-05-21.md` | `Prompt/06.track-b-green-tc-b-01-transcript-2026-05-21_prompt.md` |
| 07 | `Report/07.dual-track-green-complete-report-2026-05-21.md` | `Prompt/07.dual-track-green-complete-transcript-2026-05-21_prompt.md` |
| 08 | `Report/08.refactor-coverage-f02-gate-report-2026-05-21.md` | `Prompt/08.refactor-coverage-f02-gate-transcript-2026-05-21_prompt.md` |
| 09 | `Report/09.golden-master-approval-report-2026-05-21.md` | `Prompt/09.golden-master-approval-transcript-2026-05-21_prompt.md` |
| 10 | `Report/10.golden-master-gm-tc-report-2026-05-21.md` | `Prompt/10.golden-master-gm-tc-transcript-2026-05-21_prompt.md` |
| 11 | `Report/11.readme-golden-master-section-report-2026-05-21.md` | `Prompt/11.readme-golden-master-section-transcript-2026-05-21_prompt.md` |

이전: `Prompt/full-transcript-refactor-first-2026-05-20_prompt.md` (Step 01까지).
