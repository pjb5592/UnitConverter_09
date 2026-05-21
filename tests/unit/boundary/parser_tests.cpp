#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <string>

#include "boundary/InputParser.hpp"
#include "entity/UnitCatalog.hpp"

using Catch::Approx;

namespace {

const entity::UnitCatalog& defaultCatalog() {
    static const entity::UnitCatalog catalog = entity::UnitCatalog::withDefaultUnits();
    return catalog;
}

}  // namespace

TEST_CASE("test_parse_happy_meter_25_returns_value", "[parse][exception]") {
    // Given: valid line meter:2.5
    const std::string input = "meter:2.5";

    // When: parse
    const boundary::ParsedInput parsed = boundary::InputParser::parse(input);

    // Then: unit and value preserved
    REQUIRE(parsed.unit == "meter");
    REQUIRE(parsed.value == 2.5);
}

TEST_CASE("test_parse_exception_missing_colon_throws_invalid_argument", "[parse][exception]") {
    // Given: malformed input without ':'
    const std::string input = "meter2.5";

    // When / Then: std::invalid_argument
    REQUIRE_THROWS_AS(boundary::InputParser::parse(input), std::invalid_argument);
}

TEST_CASE("test_parse_exception_negative_meter_throws_invalid_argument", "[parse][exception]") {
    // Given: negative length meter:-1.0 (NEG-POLICY-01)
    const std::string input = "meter:-1.0";

    // When / Then
    REQUIRE_THROWS_AS(boundary::InputParser::parse(input), std::invalid_argument);
}

TEST_CASE("test_parse_exception_unknown_unit_parsec_throws", "[parse][exception]") {
    // Given: unknown unit parsec:1.0
    const std::string input = "parsec:1.0";

    // When / Then: catalog validation
    REQUIRE_THROWS_AS(boundary::InputParser::parseConvertLine(input, defaultCatalog()),
                      std::invalid_argument);
}

TEST_CASE("test_parse_exception_non_numeric_abc_throws", "[parse][exception]") {
    // Given: non-numeric token meter:abc
    const std::string input = "meter:abc";

    // When / Then
    REQUIRE_THROWS_AS(boundary::InputParser::parse(input), std::invalid_argument);
}

TEST_CASE("test_parse_exception_empty_line_throws", "[parse][exception]") {
    // Given: empty input
    const std::string input;

    // When / Then
    REQUIRE_THROWS_AS(boundary::InputParser::parse(input), std::invalid_argument);
}

TEST_CASE("test_parse_exception_uppercase_unit_throws", "[parse][exception]") {
    // Given: uppercase unit Meter:2.5 (C-04)
    const std::string input = "Meter:2.5";

    // When / Then
    REQUIRE_THROWS_AS(boundary::InputParser::parse(input), std::invalid_argument);
}

TEST_CASE("test_parse_exception_comma_decimal_throws", "[parse][exception]") {
    // Given: European decimal meter:2,5
    const std::string input = "meter:2,5";

    // When / Then
    REQUIRE_THROWS_AS(boundary::InputParser::parse(input), std::invalid_argument);
}

TEST_CASE("test_parse_happy_preserves_source_unit_for_display", "[parse][exception]") {
    // Given: SRC-LOCK requires unit name unchanged
    const std::string input = "feet:3.28084";

    // When
    const boundary::ParsedInput parsed = boundary::InputParser::parse(input);

    // Then
    REQUIRE(parsed.unit == "feet");
    REQUIRE(parsed.value == Approx(3.28084).margin(1e-9));
}
