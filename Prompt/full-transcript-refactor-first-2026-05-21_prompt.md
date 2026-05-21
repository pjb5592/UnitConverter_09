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

---

## Step 02 상세 (2026-05-21)

**User:** Dual-Track TDD용 git 브랜치 전략 → README/PRD 기반 테스트 플랜 샘플 1건(`meter:2.5`) → [P] Export/git.

**Assistant:**
- `main`/`spec`/`red`/`green`/`refactor` + `feat/red|green|refactor/{logic|ui}/TD-##`
- 샘플: F-01·AC-02·G-03, 입력 `meter:2.5`, 출력 8.2 feet / 2.7 yard (ROUND-LOCK)
- Report/02.*, Prompt/02.*, full-transcript 갱신

**검증:** 문서·전략만; build/ctest N/A.

**다음:** M0 TD-01 CMake+Catch2; `feat/red/logic`+`feat/red/ui`로 AC-02 RED.

---

## Step 01 상세 (2026-05-20)

**User:** README vs PRD 검토 보고만 → 이후「모두 반영」코드 금지 → requirements.md 변경 취소 → [P] Report/Prompt/git.

**Assistant:**
- PRD·README·TODO·traceability·gherkin 정합
- 누락 To-Do(TD-16~18 등)·고아 To-Do 분류
- requirements.md 레거시 복원
- Report/01.*, Prompt Export

**검증:** 문서만; build/ctest N/A.

---

## 정본 문서 체인

```
docs/requirements.md (레거시 초안)
    → docs/PRD.md (요구 정본)
    → README.md (사용자·Phase 6)
    → docs/TODO.md + docs/traceability-matrix.md (실행)
    → docs/gherkin.md (검증 시나리오)
    → Step 02: 브랜치 전략·AC-02 샘플 (Prompt/Report Export)
```

---

## Export 인덱스

| Step | Report | Transcript |
|------|--------|------------|
| 01 | `Report/01.docs-alignment-prd-readme-report-2026-05-20.md` | `Prompt/01.docs-alignment-prd-readme-transcript-2026-05-20_prompt.md` |
| 02 | `Report/02.dev-strategy-branch-test-sample-report-2026-05-21.md` | `Prompt/02.dev-strategy-branch-test-sample-transcript-2026-05-21_prompt.md` |

이전: `Prompt/full-transcript-refactor-first-2026-05-20_prompt.md` (Step 01까지).
