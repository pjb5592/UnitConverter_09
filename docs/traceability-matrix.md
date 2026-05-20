# Story ↔ To-Do ↔ PRD 추적 매트릭스

**기준:** Phase 4 User Stories · `docs/TODO.md` v1.2 · `docs/PRD.md` v1.1  
**갱신:** README vs PRD 검토·누락/고아 To-Do 반영 후

---

## 1. User Story 추적표

| Story ID | Story 제목 | 연관 To-Do ID | PRD 항목 | 상태 | 완료 기준 |
|----------|------------|---------------|----------|------|-----------|
| US-01 | 입력 검증 | TD-07, TD-09, TD-10, TD-18, TD-20 | F-02, F-07, §3.4, AC-01,09,10,13 | ⬜ | BT/IT: 형식·숫자·음수·빈줄·대문자 unit·REGISTER 형식 오류 거절; stderr JSON exact |
| US-02 | 레지스트리·OCP | TD-02, TD-05, TD-06, TD-16, TD-21 | F-03, F-09, G-07, AC-07, ORDER-LOCK | ⬜ | Catalog 순회; if-체인 0; include 0; 출력 ORDER-LOCK IT PASS |
| US-03 | 환산 정확도 | TD-03, TD-04, TD-11, TD-12, TD-17 | F-01, F-03, F-04, F-07, AC-02,10, ε-LOCK | ⬜ | ε≤1e-9; meter:2.5→8.2/2.7; feet:3.28084→LHS 3.3 feet |
| US-04 | 출력 포맷 | TD-11, TD-13, TD-14, TD-16, TD-22 | F-01, F-06, §6, AC-02,05,11, ORDER-LOCK | ⬜ | table/json/csv N줄·스키마·ROUND/SRC; format 진입 계약 |
| US-05 | 설정 로드·실패 | TD-03, TD-04, TD-19 | F-04, §5.2, AC-04, Sc.7 | ⬜ | 3비율; 없음/깨짐/중복/factor≤0→ERR-DATA-LOAD |
| US-06 | 동적 단위 등록 | TD-13, TD-20, TD-18 | F-05, AC-06,12, G-05, Sc.12 | ⬜ | ACK; cubit N+1; 중복·INVALID_FACTOR 거절 |
| US-07 | 미지원 단위 | TD-08, TD-09 | F-02, AC-03,13, Sc.6 | ⬜ | inch:1→Unknown unit; 변환 0줄 |

---

## 2. To-Do ID 목록 (전체)

### 🔴 필수 (Must)

| ID | To-Do | Story | PRD |
|----|-------|-------|-----|
| TD-01 | CMake·Catch2 골격 | (횡단) | §4.1, G-02 |
| TD-02 | BCE 디렉터리·의존 방향 | US-02 | G-07, §4.2 |
| TD-03 | config/units.json 기본 3단위 | US-03, US-05 | F-04, §5.1 |
| TD-04 | 설정 로드 실패(없음·깨짐·중복·factor≤0) | US-05 | F-04, AC-04, §5.2 |
| TD-05 | UnitCatalog + meter 허브 | US-02, US-03 | F-03, ε-LOCK |
| TD-06 | if-체인 제거(F-09) | US-02 | F-09, AC-07 |
| TD-07 | InputParser F-02 전 경로 | US-01 | AC-01,09, Sc.3~5,9 |
| TD-08 | 미등록 단위 거절 | US-07 | AC-03, Sc.6 |
| TD-09 | ErrorPresenter + Error JSON | US-01, US-07 | §3.4, AC-13 |
| TD-10 | Table CONVERT meter:2.5 | US-03, US-04 | AC-02, Sc.1 |
| TD-11 | F-07 zero | US-01, US-03 | AC-10, Sc.10 |
| TD-12 | Control 유스케이스 연결 | US-03,04,06 | §4.2, F-01 |
| TD-16 | ORDER-LOCK 출력 순서 | US-02, US-04 | §7.2, R-09 |
| TD-17 | feet:3.28084 → 3.3 feet LHS | US-03 | AC-02, Sc.2, §6.5 |
| TD-18 | REGISTER 형식·도메인 실패 | US-01, US-06 | F-05, AC-12 |
| TD-19 | (merged TD-04) | US-05 | — |
| TD-20 | (merged TD-18) | US-06 | — |

### 🟡 권장 (Should)

| ID | To-Do | Story | PRD |
|----|-------|-------|-----|
| TD-13 | REGISTER 동적 등록 성공 경로 | US-06 | AC-06, G-05 |
| TD-14 | JSON 출력 | US-04 | AC-05, Sc.8 |
| TD-15 | CSV 출력 | US-04 | AC-11, Sc.11 |
| TD-21 | Gherkin Sc.1~13 TC | (횡단) | §9.1, G-02 |
| TD-22 | 출력 format CLI 계약 | US-04 | F-06 |
| TD-23 | 레이어 커버리지 | (횡단) | G-01, AC-08 |
| TD-24 | REFACTOR + LOCK | (횡단) | G-04, §7.2 |
| TD-25 | docs/gherkin.md 정리 | (횡단) | §9.1 |

### 🟢 선택 / 🔵 부채 / ✅ Done

| ID | 구분 | Story | PRD |
|----|------|-------|-----|
| TD-30~34 | 선택 F-08,YAML,argv,InMemory,clang | — | v2 |
| TD-40~44 | 기술 부채 | NG-05 등 | — |
| TD-50~55 | Done 문서화 | — | PRD, rules, README |

---

## 3. 누락·고아 해소 기록

| 이슈 (검토 시) | 조치 |
|----------------|------|
| ORDER-LOCK To-Do 없음 | **TD-16** Must 추가 |
| 설정 중복·factor≤0 | **TD-04** 확장 |
| feet Sc.2 | **TD-17** Must 추가 |
| ERR-PARSE-REGISTER | **TD-18** Must 추가 |
| format CLI US-04 | **TD-22** Should 추가 |
| 고아 CMake·Gherkin 등 | §2 횡단·PRD G-02 명시 |
| README AC 표 없음 | README §인수 기준 + PRD C-10 |
| 고아 회귀 R-01~12 | `TODO.md` 회귀표 ↔ Story 매핑 열 추가 |

---

## 4. 회귀 체크 ↔ Story

| R-ID | LOCK/항목 | Story |
|------|-----------|-------|
| R-01 | 계약 테스트 | 전체 |
| R-02 | Gherkin Sc.1~13 | 전체 |
| R-03 | 커버리지 G-01 | 전체 |
| R-04 | ε-LOCK | US-03 |
| R-05 | ROUND-LOCK | US-03, US-04 |
| R-06 | MESSAGE-LOCK | US-01, US-05, US-06 |
| R-07 | NEG-LOCK | US-01 |
| R-08 | SRC-LOCK | US-03, US-04 |
| R-09 | ORDER-LOCK | US-02, US-04 |
| R-10 | README 부록 C | (문서) |
| R-11 | forbidden F01~07 | US-02 |
| R-12 | LOCK 변경 정책 | (프로세스) |
