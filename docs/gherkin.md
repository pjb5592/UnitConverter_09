# Gherkin 시나리오 — PRD §9.1 (Sc.1~13)

Catch2/BDD 매핑용 문서. 구현 코드 없음.

```gherkin
Feature: Length unit conversion CLI
  Background:
    Given factorToMeter meter 1.0 feet 3.28084 yard 1.09361
    And display rounding half-away-from-zero 1 decimal

  # Sc.1
  Scenario: Convert meter preserves LHS
    When convert "meter:2.5" format table
    Then exit 0 and 3 lines and "2.5 meter = 8.2 feet"

  # Sc.2
  Scenario: Convert feet preserves rounded LHS
    When convert "feet:3.28084" format table
    Then every line starts with "3.3 feet ="

  # Sc.3-5,6,7,8 — see PRD §9.1 table
  # Sc.9 empty line ERR-PARSE-EMPTY
  # Sc.10 meter:0 all zero
  # Sc.11 csv header and N rows
  # Sc.12 register cubit duplicate reject
  # Sc.13 inch stderr JSON exact
```

전문 및 Then 절: `docs/PRD.md` §9.1 · 부록 A.
