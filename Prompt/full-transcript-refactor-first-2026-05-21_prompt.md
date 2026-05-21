# Full Transcript — refactor-first workflow

**프로젝트:** UnitConverter_09  
**갱신:** 2026-05-21  
**브랜치:** red

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

이전: `Prompt/full-transcript-refactor-first-2026-05-20_prompt.md` (Step 01까지).
