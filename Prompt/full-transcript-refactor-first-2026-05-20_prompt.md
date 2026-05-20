# Full Transcript — refactor-first workflow

**프로젝트:** UnitConverter_09  
**갱신:** 2026-05-20  
**브랜치:** spec

---

## 세션 요약 (누적)

| Step | slug | 산출 | 상태 |
|------|------|------|------|
| — | problem-definition | Epic·Invariant·Mom Test | 대화 |
| — | bce-design | BCE·Dual-Track·계약 | 대화 |
| — | red-test-list | Catch2 RED 45 TC 제목 | 대화 |
| — | cursorrules | `.cursorrules` YAML | ✅ |
| 05 | prd-package | `docs/PRD.md` | ✅ |
| 06 | readme-phase6 | `README.md` | ✅ |
| **07** | **docs-alignment-prd-readme** | TODO·traceability·갭 반영 | ✅ |

---

## Step 07 상세 (2026-05-20)

**User:** README vs PRD 검토 보고만 → 이후「모두 반영」코드 금지 → requirements.md 변경 취소 → [P] Report/Prompt/git.

**Assistant:**
- PRD·README·TODO·traceability·gherkin 정합
- 누락 To-Do(TD-16~18 등)·고아 To-Do 분류
- requirements.md 레거시 복원
- Report/07.*, Prompt Export, full-transcript 갱신

**검증:** 문서만; build/ctest N/A.

**다음:** M0 TD-01 CMake+Catch2 RED, Entity Domain (리팩토링 우선).

---

## 정본 문서 체인

```
docs/requirements.md (레거시 초안)
    → docs/PRD.md (요구 정본)
    → README.md (사용자·Phase 6)
    → docs/TODO.md + docs/traceability-matrix.md (실행)
    → docs/gherkin.md (검증 시나리오)
```

---

## Export 인덱스

| 파일 |
|------|
| `Prompt/07.docs-alignment-prd-readme-transcript-2026-05-20_prompt.md` |
| `Report/07.docs-alignment-prd-readme-report-2026-05-20.md` |

이전 단계 Export는 동일 `Prompt/` 패턴으로 추가 예정.
