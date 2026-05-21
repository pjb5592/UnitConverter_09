#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <string>

#include "control/ConversionUseCase.hpp"
#include "entity/UnitCatalog.hpp"

namespace {

entity::UnitCatalog defaultCatalog() {
    return entity::UnitCatalog::withDefaultUnits();
}

control::ConversionUseCase makeUseCase() {
    return control::ConversionUseCase(defaultCatalog());
}

}  // namespace

// TC-A-01 — Happy Path: meter:2.5 → 변환 결과(표 형식) 반환
// Contract: 입력 파싱 후 최소 1줄 이상; feet 줄은 "2.5 meter = 8.202100 feet"
// Invariant: INV-UI-01 정상 입력은 std::invalid_argument 없이 결과 벡터 비어 있지 않음
TEST_CASE("TC-A-01_happy_meter_25_returns_conversion_lines", "[red][track-a][TC-A-01]") {
    // Given: catalog with meter/feet/yard; line "meter:2.5"
    const control::ConversionUseCase useCase = makeUseCase();
    const std::string input = "meter:2.5";

    // When: convertTable
    const std::vector<std::string> lines = useCase.convertTable(input);

    // Then: non-empty; contains feet line with 6-decimal target (user contract)
    REQUIRE_FALSE(lines.empty());
    bool hasFeetLine = false;
    for (const std::string& line : lines) {
        if (line.find("feet") != std::string::npos) {
            hasFeetLine = true;
            REQUIRE(line == "2.5 meter = 8.202100 feet");
        }
    }
    REQUIRE(hasFeetLine);
}

// TC-A-02 — malformed: no colon
// Contract: ":" 없으면 std::invalid_argument
// Invariant: INV-UI-02 형식 오류는 예외로만 표현(에러 코드 문자열 아님)
TEST_CASE("TC-A-02_missing_colon_throws_invalid_argument", "[red][track-a][TC-A-02]") {
    // Given: "meter2.5"
    const control::ConversionUseCase useCase = makeUseCase();
    const std::string input = "meter2.5";

    // When / Then
    REQUIRE_THROWS_AS(useCase.convertTable(input), std::invalid_argument);
}

// TC-A-03 — negative value
// Contract: meter:-1.0 → std::invalid_argument
// Invariant: INV-UI-03 음수 길이 값 거부 (NEG-POLICY)
TEST_CASE("TC-A-03_negative_meter_throws_invalid_argument", "[red][track-a][TC-A-03]") {
    // Given
    const control::ConversionUseCase useCase = makeUseCase();
    const std::string input = "meter:-1.0";

    // When / Then
    REQUIRE_THROWS_AS(useCase.convertTable(input), std::invalid_argument);
}

// TC-A-04 — unknown unit
// Contract: parsec:1.0 → std::invalid_argument
// Invariant: INV-UI-04 카탈로그에 없는 단위는 변환 시작 전 거부
TEST_CASE("TC-A-04_unknown_parsec_throws_invalid_argument", "[red][track-a][TC-A-04]") {
    // Given
    const control::ConversionUseCase useCase = makeUseCase();
    const std::string input = "parsec:1.0";

    // When / Then
    REQUIRE_THROWS_AS(useCase.convertTable(input), std::invalid_argument);
}

// TC-A-05 — SRC-LOCK: 출력 LHS에 원 입력 단위·값 보존
// Contract: "feet:3.28084" → 모든 줄이 "3.28084 feet =" 로 시작
// Invariant: INV-UI-05 sourceUnit/sourceValue 입력 그대로(표시 포맷 6자리)
TEST_CASE("TC-A-05_output_preserves_source_unit_and_value", "[red][track-a][TC-A-05]") {
    // Given
    const control::ConversionUseCase useCase = makeUseCase();
    const std::string input = "feet:3.28084";

    // When
    const std::vector<std::string> lines = useCase.convertTable(input);

    // Then: every line preserves source on LHS
    REQUIRE(lines.size() >= 3);
    for (const std::string& line : lines) {
        REQUIRE(line.find("3.280840 feet =") == 0);
    }
}

// TC-A-06 — JSON format request
// Contract: source + conversions[]; N=등록 단위 수; feet value 8.202100 for meter:2.5
// Invariant: INV-UI-06 JSON은 스키마 키(source, conversions) 및 배열 길이 N
TEST_CASE("TC-A-06_json_output_matches_schema", "[red][track-a][TC-A-06]") {
    // Given
    const control::ConversionUseCase useCase = makeUseCase();
    const std::string input = "meter:2.5";

    // When
    const std::string json = useCase.convertJson(input);

    // Then: minimal schema contract (exact substring lock for RED)
    REQUIRE(json.find(R"("source")") != std::string::npos);
    REQUIRE(json.find(R"("unit":"meter")") != std::string::npos);
    REQUIRE(json.find(R"("value":2.5)") != std::string::npos);
    REQUIRE(json.find(R"("conversions")") != std::string::npos);
    REQUIRE(json.find(R"("unit":"feet","value":8.202100)") != std::string::npos);
    REQUIRE(json.find(R"("unit":"yard","value":2.734025)") != std::string::npos);
}
