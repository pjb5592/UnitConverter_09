# PRD — UnitConverter_09 (C++ 길이 단위 변환 학습 시스템)

**문서 버전:** 1.1  
**워크스페이스:** `d:\DEV\UnitConverter_09`  
**기반:** Phase 4 Epic · User Journey · User Stories (US-01~07) · Gherkin 8 시나리오 · `.cursorrules`  
**범위:** CLI 길이 변환·설정 로드·동적 등록·다중 출력 포맷·BCE + Catch2 TDD 학습

---

# 1. 프로젝트 개요

## 1.1 한 줄 목적문 (What / Who / Why)

| 요소 | 내용 |
|------|------|
| **What** | meter 허브 기준 길이 변환 CLI, Catch2 계약 테스트, BCE(entity/control/boundary/data) 구조 |
| **Who** | C++17·클린 아키텍처·TDD를 6시간 AI 실습으로 학습하는 개발자 |
| **Why** | 환산 절차 숙달이 아니라 **계약·테스트·레이어 분리**로 요구 변경 시 회귀 없이 확장하는 설계 역량을 확보하기 위해 |

## 1.2 배경 및 문제 정의 (관찰 관점)

Epic **「확장 가능한 C++ 단위 변환 학습 시스템」** 의 출발점은 README·단일 변환기다. `meter:2.5` 입력 시 feet·yard 출력은 되지만, 비율·`if (unit==...)`·stdout이 한 경로에 묶여 단위·포맷·설정 변경 시 **수정 범위를 예측할 수 없다**. README는 OCP·SRP·검증·JSON 설정·동적 등록·다중 출력을 요구하나, **통과/실패를 숫자와 문장으로 재현하는 계약**이 없으면 “맞는 것 같다”로 끝난다. Phase 4는 이 간극을 Epic 성공 기준(SC)과 Gherkin으로 메우는 단계다.

## 1.3 목표 (측정 가능) — Phase 4 Epic SC 매핑

| ID | Epic SC | 목표 | 통과 측정 (누가·무엇) |
|----|---------|------|----------------------|
| **G-01** | SC-01 | 레이어 커버리지 | entity≥95%, boundary≥85%, data≥90%, control≥80%, overall≥88% (§4.3) |
| **G-02** | SC-02 | 계약·불변식 테스트 | Catch2 DT/BT/IT 100% PASS; Gherkin §9.1 필수 시나리오 전부 일치 |
| **G-03** | SC-03 | 환산·표시 정확도 | DT: README 비율·ε≤1e-9; `meter:2.5` table에 feet=`8.2`, yard=`2.7` |
| **G-04** | SC-04 | 회귀 보호 | GREEN 후 Catch2 전체 0 실패; LOCK 6종 변경 시 TC·Gherkin 동시 diff |
| **G-05** | SC-05 | 동적 확장 | `1 cubit = 0.4572 meter` 후 `cubit:1` 출력 줄 수=등록 단위 수, meter≈0.5(1자리) |
| **G-06** | SC-06 | 실습 완료 | README Activities 1~5 완료 + 회고에 G-01~G-07 달성률 표 |
| **G-07** | — | BCE 구조·OCP | `src/entity|control|boundary|data` 존재; entity→boundary/data include 0; F-09 if-체인 0건 |

## 1.4 비목표 (Non-Goal) — 경계

| ID | 비목표 | 경계 (하지 않음) |
|----|--------|------------------|
| **NG-01** | UI·네트워크 | GUI/REST/gRPC 미제공; stdin/stdout CLI만 |
| **NG-02** | 도메인 확장 | 길이 외 물리량·환율·단위 체계 자동 변환 없음 |
| **NG-03** | 운영·인프라 | 상용 SLA, 다국어, RBAC, 원격 CI 정의는 저장소 밖 |
| **NG-04** | 알고리즘 연구 | 최적화·고정소수점 라이브러리 교체가 목적이 아님; **계약 준수**가 목적 |
| **NG-05** | 레거시 동시 정본 | `UnitConverter.cpp`는 분석용 출발점; **인수 정본은 BCE+Catch2 산출물** |

---

# 2. 사용자 및 이해관계자

## 2.1 타깃 사용자 (페르소나 1명)

| 항목 | 내용 |
|------|------|
| **이름** | 김학습 (가명) |
| **역할** | C++17, 클린 아키텍처·TDD 교육 수강생 |
| **Pain** | 단위 추가·리팩터마다 수동 재실행; 실패 기준이 “눈으로 맞음” |
| **Opportunity** | Catch2·Gherkin·ERR 코드로 **판사**를 코드에 두고 AI는 초안만 활용 |
| **성공 정의** | AC-01~13(§7) 전부 체크; 데모 4종+등록·zero·csv 통과 |

## 2.2 이해관계자 (저장소 범위)

| 역할 | 기대 | 검증 참여 |
|------|------|-----------|
| 학습자 | 구현·RED→GREEN→REFACTOR | Catch2·커버리지·데모 |
| 동료 리뷰어 | 계약·의존 방향 | BT 스냅샷·include 검토 |
| 강사 | 6시간 Activities·인수 | G-06·회고 |

## 2.3 주요 사용 시나리오 (Phase 4 Journey → 3 흐름)

| ID | Journey | 시나리오 (한 줄) | 측정 가능 완료 조건 |
|----|---------|------------------|---------------------|
| **S-01** | Awareness→Entry | 레거시 분석 후 Gherkin·US·RED TC 확정 | DT/BT ID ≥1개씩 RED 실패 기록 |
| **S-02** | Action | Entity→Data→Boundary→Control 순 GREEN | `meter:2.5` exit 0, table 3줄, LHS `2.5 meter =` |
| **S-03** | Validation→Outcome | 실패 경로·등록·json·csv 데모 후 회고 | Gherkin Sc.3~7·9~13 + US-06 + AC-01~13 표 제출 |

---

# 3. 기능 요구사항

## 3.1 핵심 기능 목록 (우선순위)

| ID | 기능 | 우선순위 | Story |
|----|------|----------|-------|
| **F-01** | CONVERT: `unit:value` → 등록 전 단위 출력 | **필수** | US-03, US-04 |
| **F-02** | 입력 검증(형식·숫자·빈줄·음수·미등록) | **필수** | US-01, US-07 |
| **F-03** | UnitCatalog + meter 허브 환산(레지스트리·OCP) | **필수** | US-02, US-03 |
| **F-09** | 단위 분기 금지: if-체인 대신 Catalog 순회만 | **필수** | US-02 |
| **F-04** | 기동 시 `config/units.json` 로드 | **필수** | US-05 |
| **F-05** | REGISTER: `1 unit = factor meter` | **권장** | US-06 |
| **F-06** | 출력 format: `table`(기본) / `json` / `csv` | **권장** | US-04 |
| **F-07** | `value=0` 전 단위 0 출력 (NEG-POLICY-02) | **필수** | US-01, US-03 |
| **F-08** | QUIT 종료 | **선택** | — |

## 3.2 기능별 입·출력 계약 (문자열)

### F-01 CONVERT

| 항목 | 계약 |
|------|------|
| 입력 | `{unit}:{value}` — unit=`[a-z][a-z0-9_]*`; value=10진 `stod` 성공 토큰 |
| 성공 exit | `0` |
| 성공 stdout | format별 §6; **줄 수 N** = 등록 단위 수 |
| SRC-LOCK | 매 줄 LHS `{srcVal} {srcUnit}` — **srcUnit**은 입력 그대로; **srcVal**은 ROUND-LOCK 1자리(§6.5) |

### F-02 검증 실패 (변환 0줄)

| 조건 | exit | code | message (전체 일치) |
|------|------|------|---------------------|
| `:` 없음 | 1 | ERR-PARSE-FORMAT | `Invalid format. Use unit:value (ex: meter:2.5)` |
| 비숫자·`2,5` 등 | 1 | ERR-PARSE-NUMBER | `Invalid number: {token}` |
| 빈 줄 | 1 | ERR-PARSE-EMPTY | `Empty input.` |
| value&lt;0 | 1 | ERR-DOMAIN-NEGATIVE-LENGTH | `Negative length not allowed: {value}` |
| 미등록 unit | 1 | ERR-DOMAIN-UNKNOWN-UNIT | `Unknown unit: {unit}` |
| stderr | 1 | — | Error JSON 1줄(§3.4); `field`는 `unit`/`value`/`path` 중 해당 시만 |
| `Meter:2.5` | 1 | ERR-PARSE-FORMAT | 대문자 unit 거절(US-02·C-04) |

### F-03 레지스트리·환산

| 항목 | 계약 |
|------|------|
| 환산 | `canonical = value × factorToMeter(source)`; `target = canonical ÷ factorToMeter(target)` |
| 중복 등록 | 동일 unit 재등록 시 카탈로그 size 불변 + DUPLICATE |

### F-09 if-체인 금지 (US-02·F06)

| 항목 | 계약 |
|------|------|
| 허용 | `UnitCatalog` 등록 목록 순회로 target 전개 |
| 금지 | `main`·`boundary`·`control`에 `if (unit == "feet")` 등 단위명 분기 |
| 검증 | 정적 검토 + Catch2: 신규 단위 추가 시 환산 루프 시그니처 변경 0 |

### F-04 설정 로드

| 항목 | 계약 |
|------|------|
| 입력 파일 | `config/units.json` |
| 성공 | meter factorToMeter=`1.0`; feet=`3.28084`; yard=`1.09361` (Background) |
| 실패 | exit `1`; `ERR-DATA-LOAD`; `Failed to load unit config: {path}`; 프롬프트 없음 |

### F-05 REGISTER

| 항목 | 계약 |
|------|------|
| 입력 | `1 {unit} = {factor} meter` |
| 성공 | exit `0`; stdout `registered: {unit} = {factor} meter` |
| 형식 불일치 | exit `1`; `ERR-PARSE-REGISTER`; message `Invalid register format. Use 1 unit = factor meter` |
| factor≤0 | `INVALID_FACTOR` |
| 중복 unit | `DUPLICATE_UNIT`; 카탈로그 불변 |

### F-06 출력 format

| format | 성공 stdout 계약 |
|--------|-------------------|
| table | `N`줄 `{srcVal} {srcUnit} = {tgtVal} {tgtUnit}`; tgtVal 1자리 반올림 |
| json | `source`+`conversions[]` 길이 `N`; Gherkin Sc.8 |
| csv | 헤더 1 + 데이터 `N`행; 컬럼 `source_unit,source_value,target_unit,target_value` |

### F-07 zero (NEG-POLICY-02)

| 항목 | 계약 |
|------|------|
| 입력 | `{unit}:0` |
| 성공 | exit `0`; `N`줄 모두 targetVal=`0.0` 표시; LHS는 입력 unit·`0` |

## 3.3 제약 사항 (Gherkin Background 정합)

| ID | 제약 |
|----|------|
| C-01 | factorToMeter: meter `1.0`, feet `3.28084`, yard `1.09361` |
| C-02 | 허브=meter; ε=`1e-9`; 표시=소수 1자리 half-away-from-zero |
| C-03 | NEG-POLICY-01: value≥0; Scenario 5 `meter:-1` 거절 |
| C-04 | unit은 소문자 `[a-z][a-z0-9_]*`만; `Meter` 등 대문자 → ERR-PARSE-FORMAT |
| C-05 | 에러 시 stdout 변환 0줄; table/json/csv 동일 stderr JSON(§3.4) |
| C-06 | 설정 정본은 JSON; YAML은 선택(README 호환) — Gherkin·DAT는 JSON만 검증 |

### 음수 입력 정책 (NEG-POLICY)

> **NEG-POLICY-01:** `value < 0` → 변환 없음, exit `1`, `ERR-DOMAIN-NEGATIVE-LENGTH`, `Negative length not allowed: {value}`  
> **NEG-POLICY-02:** `value = 0` → 허용, 전 단위 `0` 출력 (F-07)

## 3.4 stderr Error JSON 계약 (전 포맷 공통)

| 필드 | 타입 | 필수 | 예 |
|------|------|------|-----|
| `code` | string | Y | `ERR-DOMAIN-UNKNOWN-UNIT` |
| `message` | string | Y | §3.2 message 패턴 **전체 일치** |
| `field` | string | N | `unit`, `value`, `path` |

- Catch2 BT: JSON 1줄 파싱 후 `code`·`message` exact match
- Gherkin: `stderr contains error code` + message should (Sc.3~7); Sc.9에서 JSON 필드 검증

---

# 4. 비기능 요구사항

## 4.1 기술 스택

| 항목 | 요구 |
|------|------|
| 언어 | C++17 이상 |
| 빌드 | CMake |
| 테스트 | Catch2 3.0 이상, AAA 패턴 |
| 포맷 | clang-format (LLVM, 4 space, column 100) |

## 4.2 아키텍처 원칙

| 원칙 | 요구 |
|------|------|
| **OCP** | 신규 단위·출력 포맷 추가 시 entity 환산 루프 수정 없이 레지스트리·Formatter 확장 |
| **SRP** | Parser / Formatter / Repository / Engine / Catalog / ErrorPresenter 단일 책임 |
| **BCE** | boundary(CLI·파싱·포맷) · control(유스케이스) · entity(환산·불변식) · data(설정 로드) |
| **의존성** | boundary→control→entity; data→entity 타입만; **금지:** entity→boundary/data, data→boundary |

## 4.3 테스트 커버리지 목표 (라인 %)

| 레이어 | 최소 |
|--------|------|
| entity (domain) | 95 |
| boundary | 85 |
| data | 90 |
| control | 80 |
| **overall** | **88** |

미달 시 “완료” 선언 금지; 해당 레이어 Catch2 TC 추가 후 재측정.

## 4.4 확장성 원칙

- 새 단위 = `UnitDefinition` 등록 1건 + (선택) 설정 JSON 1행; main/parser **if (unit=="...") 체인 추가 금지**.
- 새 출력 포맷 = Formatter 구현체 추가; Domain·환산식 **변경 없음**.
- 비율 상수 **단일 출처**: 설정 또는 등록 테이블; 소스 코드 리터럴 3.28084/1.09361 **금지**(F02).

---

# 5. 데이터 요구사항

## 5.1 단위 비율 상수 (meter 허브, factorToMeter)

| unit | factorToMeter | 의미 |
|------|---------------|------|
| meter | 1.0 | 기준 단위 (INV-D02) |
| feet | 3.28084 | 1 meter = 3.28084 feet (README) |
| yard | 1.09361 | 1 meter = 1.09361 yard (README) |

**불변식:** `canonical = inputValue × factorToMeter(source)`; `targetValue = canonical ÷ factorToMeter(target)`; feet↔yard 직접값과 meter 경유값 차이 ≤ 1e-9.

## 5.2 설정 외부화

| 항목 | 요구 |
|------|------|
| **기본** | `config/units.json` |
| **스키마** | `{ "units": [ { "name": string, "factorToMeter": number > 0 } ] }` |
| **필수** | name=`meter`且 factorToMeter=1.0 |
| **실패** | 파일 없음·JSON 파싱 실패·중복 name·factor≤0 → 기동 실패 ERR-DATA-LOAD |
| **테스트** | InMemory Repository로 Domain/Data TC; 통합은 TempConfigFileFixture |

YAML은 **선택** 확장; JSON이 Phase 4·Gherkin Background 정본.

## 5.3 동적 단위 등록 계약

| 항목 | 계약 |
|------|------|
| **문법** | `1 {unit} = {factor} meter` |
| **unit** | `[a-z][a-z0-9_]*`, 미등록명 |
| **factor** | 양수 실수, 0 및 음수 거절 |
| **효과** | 성공 직후 카탈로그 size +1; 이후 CONVERT·출력 N+1줄 |
| **중복** | 동일 unit 재등록 거절, 카탈로그 불변 |

**예시 (US-06):** `1 cubit = 0.4572 meter` → `cubit:1` → meter 표시 0.5 (1자리).

---

# 6. 출력 요구사항

## 6.1 콘솔 기본 포맷 (table, 기본값)

| 필드 | 규칙 |
|------|------|
| **한 줄 형식** | `{sourceValue} {sourceUnit} = {targetValue} {targetUnit}` |
| **sourceUnit** | 사용자 입력 단위명 **그대로**(대소문자 포함 입력 거절 전제) |
| **sourceValue** | ROUND-LOCK 적용 1자리(입력 `2.5`→`2.5`, `3.28084`→`3.3`) |
| **targetValue** | 환산값, **소수 1자리**, half-away-from-zero |
| **줄 수** | N = 등록 단위 수 |
| **순서** | 카탈로그 등록 순(bootstrap 순 + 등록 순) |

**README 예시 검증:** `meter:2.5` → `2.5 meter = 8.2 feet`, `2.5 meter = 2.7 yard`.

## 6.2 JSON 스키마 (확장, format=json)

```json
{
  "source": { "unit": "<inputUnit>", "value": <displayRounded> },
  "conversions": [
    { "unit": "<targetUnit>", "value": <displayRounded> }
  ]
}
```

- `conversions.length` = N
- `source.unit`·`source.value` = 입력 보존(표시 반올림 적용)
- Gherkin Scenario 8: `yard:1.09361` → source.value `1.1`, meter conversion `1.0` (ε 0.0001)

## 6.3 CSV 스키마 (확장, format=csv)

| 행 | 내용 |
|----|------|
| **헤더 1행** | `source_unit,source_value,target_unit,target_value` |
| **데이터 N행** | 각 target에 대해 입력 unit·value 동일(좌측 컬럼), target 환산값 1자리 |

## 6.4 Table vs JSON/CSV 공통

- 에러 시: stdout 변환 줄 0; stderr Error JSON 1줄 (§3.4)
- display 반올림 규칙 3포맷 **동일** (ROUND-LOCK)

## 6.5 SRC-LOCK vs ROUND-LOCK (명확화)

| LOCK | 적용 위치 | 규칙 | 예 |
|------|-----------|------|-----|
| **SRC-LOCK** | LHS `sourceUnit`·JSON `source.unit` | 입력 단위명 **변경 없음** | `feet:3.28084` → unit=`feet` |
| **ROUND-LOCK** | LHS `sourceValue`, 모든 `targetValue`, JSON 숫자 | half-away-from-zero **1자리** | `3.28084`→`3.3`; `2.5`→`8.2` feet |

Gherkin Sc.2: `feet:3.28084` → `3.3 feet =` (SRC+ROUND 동시 적용, 모순 아님).

---

# 7. 성공 지표

## 7.1 인수 기준 (체크박스, Story·Gherkin 정합)

- [ ] **AC-01 (US-01, Gherkin 3·4·5):** `:` 없음·`meter:2,5`·`meter:-1` 각 exit 1, 해당 ERR-* message **문자열 전체 일치**, 변환 줄 0
- [ ] **AC-02 (US-03, Gherkin 1·2, BG):** Background 3비율 로드; `meter:2.5` table 3줄, LHS `2.5 meter =` 유지, feet `8.2` yard `2.7`
- [ ] **AC-03 (US-07, Gherkin 6):** `inch:1` → `Unknown unit: inch`, exit 1
- [ ] **AC-04 (US-05, Gherkin 7):** 설정 없음/깨짐 → ERR-DATA-LOAD, convert 프롬프트 없음
- [ ] **AC-05 (US-04, Gherkin 8):** format=json 시 `source`·`conversions` 스키마·길이 N 충족
- [ ] **AC-06 (US-06, SC-05):** `1 cubit = 0.4572 meter` 등록 후 `cubit:1` 출력 N+1줄, meter 약 0.5(1자리)
- [ ] **AC-07 (US-02, G-07):** entity에 boundary/data include 없음; F-09 if-체인 0건
- [ ] **AC-08 (G-01):** 레이어 커버리지 entity≥95, boundary≥85, data≥90, control≥80, overall≥88
- [ ] **AC-09 (US-01, §9.1 Sc.9):** 빈 줄 → ERR-PARSE-EMPTY, 변환 0줄
- [ ] **AC-10 (US-03, §9.1 Sc.10):** `meter:0` exit 0, N줄 target `0.0`
- [ ] **AC-11 (US-04, §9.1 Sc.11):** format=csv 헤더+데이터 N행
- [ ] **AC-12 (US-06, §9.1 Sc.12):** REGISTER 성공 ACK; 중복·factor≤0 거절
- [ ] **AC-13 (§3.4, §9.1):** 실패 시 stderr Error JSON `code`·`message` exact match

## 7.2 회귀 보호 규칙 (계약 변경 금지)

| LOCK ID | 고정 대상 | 변경 시 필수 조치 |
|---------|-----------|-------------------|
| **ε-LOCK** | factorToMeter README 값, ε=1e-9 | DT-*·Gherkin BG·AC-02 동시 수정 |
| **ROUND-LOCK** | 1자리 half-away-from-zero | BT-12·Gherkin 1·2·8·AC-02 동시 수정 |
| **MESSAGE-LOCK** | §3.2 message 패턴 전체 | BT-*·Gherkin 3~7·AC-01~04 스냅샷 갱신 |
| **ORDER-LOCK** | 출력 줄 = 카탈로그 등록 순 | IT-01·table TC 갱신 |
| **NEG-LOCK** | NEG-POLICY-01·02 | Gherkin Scenario 5·AC-01 |
| **SRC-LOCK** | LHS unit=입력 unit; LHS value=ROUND(입력) | §6.5·Gherkin 1·2·8·AC-02·AC-05 |

**정책:** LOCK 위반 변경은 “버그 수정”이 아니라 **계약 변경**으로 취급; PRD 버전·TC·Gherkin diff 필수.

---

# 8. 용어 정의 (Glossary)

| 용어 | 정의 |
|------|------|
| **factorToMeter** | 1해당단위가 몇 meter에 해당하는지 나타내는 양수 상수; 환산의 단일 기준 |
| **Canonical length** | meter 허브로 환산한 절대 길이; 모든 target 값은 여기서 파생 |
| **Unit catalog** | 등록된 `UnitDefinition` 집합; 크기 N이 출력 줄 수와 같음(INV-D05) |
| **Source-preservation** | 출력 좌변(또는 JSON `source`)이 사용자 입력 unit·value를 그대로 반영하는 표현 계약 |
| **Boundary contract** | stdin 한 줄·stdout 포맷·stderr Error JSON·exit code의 검증 가능 묶음 |
| **Domain error** | `UNKNOWN_UNIT`, `NEGATIVE_LENGTH` 등 코드만 entity가 반환; 문구는 Boundary가 조합 |
| **DISPLAY rounding** | 내부 계산 후 사용자 표시에만 소수 1자리 half-away-from-zero 적용 |
| **ERR-*** | `ERR-PARSE-*`, `ERR-DOMAIN-*`, `ERR-DATA-*` 고정 접두; stderr JSON `code` 필드 값 |
| **RED / GREEN / REFACTOR** | Catch2 실패→최소 통과→전 TC GREEN 후 구조 개선; REFACTOR 중 기능 추가 금지 |
| **BCE** | Boundary–Control–Entity(+Data); 의존성 방향이 아키텍처 검증의 1차 규칙 |

---

# 9. Phase 4 Gherkin 보강 요구

## 9.1 필수 시나리오 집합

G-02 통과 조건: **Sc.1~8**(Phase 4 정본) + **Sc.9~13**(v1.1 갭 해소) 전부 Catch2 또는 BDD 러너로 검증.

| Sc | When (요약) | Then (요약) |
|----|-------------|-------------|
| 9 | 빈 줄 convert | ERR-PARSE-EMPTY |
| 10 | `meter:0` table | exit 0; N줄 target `0.0` |
| 11 | `meter:2.5` csv | 헤더 1 + 데이터 N |
| 12 | `1 cubit = 0.4572 meter` 후 convert; 중복 등록 | ACK; DUPLICATE 거절 |
| 13 | `inch:1` | stderr JSON `code`+`message` exact |

---

## 부록 A: Phase 4 Gherkin 매핑

### A.1 기존 8 시나리오 (Phase 4 정본)

| Sc | 제목 | Story | PRD |
|----|------|-------|-----|
| 1 | meter happy + LHS | US-03, US-04 | F-01, AC-02 |
| 2 | feet happy + LHS 3.3 | US-03 | F-01, §6.5 |
| 3 | missing colon | US-01 | F-02, AC-01 |
| 4 | `meter:2,5` | US-01 | F-02, AC-01 |
| 5 | `meter:-1` | US-01 | NEG-POLICY, AC-01 |
| 6 | `inch:1` | US-07 | F-02, AC-03 |
| 7 | config invalid | US-05 | F-04, AC-04 |
| 8 | json source block | US-04 | F-06, AC-05 |

### A.2 보강 시나리오 (Sc.9~13)

§9.1·AC-09~13 참조.

---

## 부록 B: Phase 4 정합성 검토 반영 요약 (v1.1)

| 구분 | 조치 |
|------|------|
| Story 갭 | F-09 추가; F-07·빈줄·csv·REGISTER → Sc.9~12 |
| Gherkin 갭 | §9.1 보강 5 시나리오; Error JSON AC-13 |
| SRC vs ROUND | §6.5 분리; Sc.2 `3.3 feet` 해석 고정 |
| Epic SC 매핑 | G-01=커버리지, G-02=계약, G-07=BCE·F-09 |
| YAML | C-06: JSON 정본, YAML 선택 |

---

## 부록 C: Phase 6 README 이관 체크리스트

| # | PRD 출처 | README 반영 | 상태 |
|---|----------|-------------|------|
| C-01 | §3.2, §3.4 | ERR-* + Error JSON 필드 표 | ✅ |
| C-02 | NEG-POLICY | 음수·0 정책 전문 | ✅ |
| C-03 | §6.5 | SRC vs ROUND 표 | ✅ |
| C-04 | §4.1 | C++17, CMake, Catch2; g++=레거시 | ✅ |
| C-05 | §4.2, G-07 | BCE·include 금지 | ✅ |
| C-06 | G-01, §4.3 | 커버리지 % + AC-08 | ✅ |
| C-07 | §7.2 | LOCK 6종 + 변경 시 조치 | ✅ |
| C-08 | F-05, §5.3 | 등록·ACK·3종 실패 | ✅ |
| C-09 | §6.2, §6.3 | json·csv 예시 | ✅ |
| C-10 | §7.1 | AC-01~13 ↔ Activities | ✅ |
| C-11 | NG-05 | 인수 정본 vs 레거시 | ✅ |
| C-12 | C-06 | JSON 정본; YAML v2 | ✅ |

## 부록 E: Story ↔ To-Do 추적

상세: [docs/traceability-matrix.md](traceability-matrix.md) · 작업: [docs/TODO.md](TODO.md)

---

## 부록 D: Phase 4 추적 매트릭스

| PRD 절 | Epic/Journey | Story | Gherkin |
|--------|--------------|-------|---------|
| §1.3 G-01~G-07 | Epic SC-01~06 + G-07 | Journey S-01~03 | — |
| §3 F-01~F-09 | US-01~07 | Sc.1~13 (§9.1) |
| §5 | US-05·06 | BG, Sc.7 |
| §6 | US-04 | Sc.1·2·8·11 |
| §7 AC-01~13 | SC-* | US-* | Sc.* |
| 부록 C | — | — | README ✅ |
| 부록 E | US-01~07 | TD-01~25 | traceability-matrix.md |

**문서 상태:** v1.1.1 — README·TODO·추적 매트릭스 동기화. `src/`+AC-01~13 충족 시 인수 완료.
