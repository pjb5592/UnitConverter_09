# To-Do 리스트 — UnitConverter (C++)

**워크스페이스:** `d:\DEV\UnitConverter_09`  
**기준:** `docs/PRD.md` v1.1 · `docs/traceability-matrix.md`  
**v1.0 릴리스:** AC-01~10·13 + G-07 + G-03 + TD-16·17·18

**To-Do ID:** `TD-##` — 추적 매트릭스와 동일

---

## 🔴 필수 (Must-Have) — v1.0 릴리스 차단

| ID | 항목 | Story | PRD | 완료 기준 (누가→통과) |
|----|------|-------|-----|------------------------|
| TD-01 | CMake·Catch2 골격 | 횡단 | §4.1, G-02 | **학습자** → ctest 1회 이상·리포트 |
| TD-02 | BCE 디렉터리·의존 방향 | US-02 | G-07, §4.2 | **리뷰어** → include 0건 |
| TD-03 | config/units.json 3단위 | US-03,05 | F-04, §5.1 | **학습자** → DAT-01 3비율 |
| TD-04 | 설정 로드 실패 전형 | US-05 | F-04, AC-04 | **IT** → 없음/깨짐/**중복 name/factor≤0** → ERR-DATA-LOAD·프롬프트 없음 |
| TD-05 | UnitCatalog + meter 허브 | US-02,03 | F-03, ε-LOCK | **DT** → ε≤1e-9·리터럴 0건 |
| TD-06 | if-체인 제거 (F-09) | US-02 | F-09, AC-07 | **리뷰어** → if(unit) 0건 |
| TD-07 | InputParser F-02 | US-01 | AC-01,09 | **BT** → :, 2,5, -1, 빈줄, Meter 거절 |
| TD-08 | 미등록 단위 | US-07 | AC-03, Sc.6 | **BT/IT** → inch:1 Unknown |
| TD-09 | ErrorPresenter JSON | US-01,07 | §3.4, AC-13 | **BT** → code·message exact |
| TD-10 | Table meter:2.5 | US-03,04 | AC-02, Sc.1 | **BT/IT** → 3줄·8.2·2.7·LHS 2.5 meter |
| TD-11 | zero meter:0 | US-01,03 | AC-10, Sc.10 | **BT** → N줄 0.0 |
| TD-12 | Control 배선 | US-03,04,06 | §4.2, F-01 | **학습자** → E2E IT PASS |
| TD-16 | ORDER-LOCK 줄 순서 | US-02,04 | §7.2, R-09 | **IT** → 등록 순=출력 순 |
| TD-17 | feet:3.28084 LHS 3.3 | US-03 | Sc.2, §6.5 | **BT** → `3.3 feet =` (ROUND+SRC) |
| TD-18 | REGISTER 실패 경로 | US-01,06 | F-05, AC-12 | **BT** → ERR-PARSE-REGISTER·DUPLICATE·INVALID_FACTOR |

- [ ] TD-01 … TD-18 (위 표 전항목)

---

## 🟡 권장 (Should-Have)

| ID | 항목 | Story | PRD | 완료 기준 |
|----|------|-------|-----|-----------|
| TD-13 | REGISTER 성공 cubit | US-06 | AC-06, G-05 | ACK·cubit:1 N+1·0.5m |
| TD-14 | JSON format | US-04 | AC-05, Sc.8 | source+conversions N |
| TD-15 | CSV format | US-04 | AC-11, Sc.11 | 헤더+데이터 N |
| TD-21 | Gherkin Sc.1~13 TC | 횡단 | §9.1, G-02 | Sc.1~13 각 GREEN |
| TD-22 | format CLI 계약 | US-04 | F-06 | json/csv 선택 진입·TC |
| TD-23 | 커버리지 게이트 | 횡단 | G-01, AC-08 | 95/85/90/80/88% |
| TD-24 | REFACTOR + LOCK | 횡단 | G-04, §7.2 | Catch2 0·스냅샷 유지 |
| TD-25 | docs/gherkin.md | 횡단 | §9.1 | Sc.1~13 문서화 (✅ 문서 초안) |

- [ ] TD-13 … TD-25

---

## 🟢 선택 (v2.0) — Story 없음 · PRD만

| ID | 항목 | PRD | 기대 가치 |
|----|------|-----|-----------|
| TD-30 | QUIT (F-08) | F-08 | 대화형 종료 |
| TD-31 | YAML 로더 | C-06 | README YAML 호환 |
| TD-32 | argv --config/format | F-06 | 자동화 |
| TD-33 | InMemory Repository | §5.2 | 테스트 단순화 |
| TD-34 | clang-format 훅 | §4.1 | 스타일 일관 |

---

## 🔵 기술 부채

| ID | 문제 | 원인 | 해결 | Story |
|----|------|------|------|-------|
| TD-40 | 레거시 cpp 이중 정본 | NG-05 | README 정본=BCE | — |
| TD-41 | 매직넘버·catch(...) | 레거시 | F02/F03 0건 | US-02 |
| TD-42 | 문서·구현 분리 | src/ 없음 | M1~2 AC 구현 | 횡단 |
| TD-43 | Gherkin 8 vs 13 | 초안 | gherkin.md | 횡단 |
| TD-44 | (해소) README 계약 | 부록 C | ✅ README v2 동기화 | — |

---

## ✅ 완료 (Done)

| ID | 내용 | 완료일 | 산출물 |
|----|------|--------|--------|
| TD-50 | PRD v1.1 | 2026-05-20 | docs/PRD.md |
| TD-51 | .cursorrules | 2026-05-20 | .cursorrules |
| TD-52 | 레거시 cpp | (기존) | UnitConverter.cpp |
| TD-53 | README Phase 6 동기화 | 2026-05-20 | README.md (부록 C 12/12) |
| TD-56 | gherkin.md 초안 | 2026-05-20 | docs/gherkin.md |
| TD-54 | 추적 매트릭스 | 2026-05-20 | docs/traceability-matrix.md |
| TD-55 | Phase 4→PRD 정합 v1.1 | 2026-05-20 | PRD 부록 B |

---

## 📋 회귀 방지 (PRD §7.2) ↔ Story

| R | 항목 | Story | 담당 |
|---|------|-------|------|
| R-01 | DT+BT+IT 0 실패 | 전체 | 학습자 |
| R-02 | Sc.1~13 GREEN | 전체 | 학습자 |
| R-03 | G-01 커버리지 | 전체 | 학습자 |
| R-04 | ε-LOCK | US-03 | 학습자 |
| R-05 | ROUND-LOCK | US-03,04 | 학습자 |
| R-06 | MESSAGE-LOCK | US-01,05,06 | 리뷰어 |
| R-07 | NEG-LOCK | US-01 | 학습자 |
| R-08 | SRC-LOCK | US-03,04 | 리뷰어 |
| R-09 | ORDER-LOCK | US-02,04 | 학습자 |
| R-10 | README C-01~12 | — | 강사 |
| R-11 | F01~F07 | US-02 | 리뷰어 |
| R-12 | LOCK 변경 PR | — | 학습자 |

---

## 🗓️ 마일스톤

| M | 포함 TD/AC | 상태 |
|---|------------|------|
| M0 | TD-50~55, RED | 🟡 |
| M1 | TD-01~12,16~18, AC-01~04,07,09~10,13 | ⬜ |
| M2 | TD-10,17,23, G-02,03,07, Sc.1~8 | ⬜ |
| M3 | TD-13~15,21~25, AC-05,06,11,12 | ⬜ |
| M4 | R-01~12, G-06 회고 | ⬜ |

**다음:** M1 — TD-01 골격 → TD-05 Domain RED → TD-07 Boundary RED
