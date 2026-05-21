#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <limits>
#include <string>

#include "entity/DisplayRounder.hpp"
#include "entity/LengthConversionEngine.hpp"
#include "entity/UnitCatalog.hpp"
#include "entity/UnitConverter.hpp"
#include "fixtures/TestConstants.hpp"

using Catch::Approx;

namespace {

entity::LengthConversionEngine makeEngine() {
    return entity::LengthConversionEngine(entity::UnitCatalog::withDefaultUnits());
}

}  // namespace

TEST_CASE("test_convert_meter_to_feet_happy_returns_8202", "[convert][happy]") {
    // Given: 1 meter = 3.28084 feet (hub factor feet=3.28084)
    const entity::LengthConversionEngine engine = makeEngine();

    // When: convert 2.5 meter to feet
    const double result = engine.convert("meter", 2.5, "feet");

    // Then: 2.5 * 3.28084 = 8.20210 (ε=1e-5)
    REQUIRE(result == Approx(8.20210).margin(test_constants::kEpsilonRelaxed));
}

TEST_CASE("test_convert_meter_to_yard_happy_returns_2734", "[convert][happy]") {
    // Given: 1 meter = 1.09361 yard (hub factor yard=1.09361)
    const entity::LengthConversionEngine engine = makeEngine();

    // When: convert 2.5 meter to yard
    const double result = engine.convert("meter", 2.5, "yard");

    // Then: 2.5 * 1.09361 = 2.734025
    REQUIRE(result == Approx(2.734025).margin(test_constants::kEpsilonRelaxed));
}

TEST_CASE("test_convert_feet_to_meter_reverse_returns_030480", "[convert][happy]") {
    // Given: 3.28084 feet = 1 meter (inverse: 1 foot = 1/3.28084 meter)
    const entity::LengthConversionEngine engine = makeEngine();

    // When: convert 1.0 feet to meter
    const double result = engine.convert("feet", 1.0, "meter");

    // Then: 1 / 3.28084 ≈ 0.30480
    REQUIRE(result == Approx(0.30480).margin(test_constants::kEpsilonRelaxed));
}

TEST_CASE("test_convert_meter_to_meter_identity_returns_input", "[convert][happy]") {
    // Given: 1 meter = 1.0 meter (factor 1.0)
    const entity::LengthConversionEngine engine = makeEngine();

    // When: convert 2.5 meter to meter
    const double result = engine.convert("meter", 2.5, "meter");

    // Then: identity
    REQUIRE(result == Approx(2.5).margin(test_constants::kEpsilonStrict));
}

TEST_CASE("test_convert_meter_to_feet_unit_value_returns_820_display", "[convert][happy]") {
    // Given: ROUND-LOCK 1 decimal; 1 meter = 3.28084 feet
    const entity::LengthConversionEngine engine = makeEngine();

    // When: display convert 2.5 meter to feet
    const double display = engine.convertDisplay("meter", 2.5, "feet");

    // Then: 8.2 feet (PRD AC-02)
    REQUIRE(display == Approx(8.2).margin(test_constants::kEpsilonStrict));
}

TEST_CASE("test_convert_meter_to_yard_display_returns_27", "[convert][happy]") {
    // Given: 1 meter = 1.09361 yard
    const entity::LengthConversionEngine engine = makeEngine();

    // When: display convert 2.5 meter to yard
    const double display = engine.convertDisplay("meter", 2.5, "yard");

    // Then: 2.7 yard
    REQUIRE(display == Approx(2.7).margin(test_constants::kEpsilonStrict));
}

TEST_CASE("test_convert_boundary_zero_meter_all_targets_zero", "[convert][boundary]") {
    // Given: value = 0 (NEG-POLICY-02)
    const entity::LengthConversionEngine engine = makeEngine();

    // When: convertAll from meter
    const auto rows = engine.convertAll("meter", 0.0);

    // Then: every target is 0.0
    REQUIRE(rows.size() == 3);
    for (const auto& row : rows) {
        REQUIRE(row.second == Approx(0.0).margin(test_constants::kEpsilonStrict));
    }
}

TEST_CASE("test_convert_boundary_large_meter_value_finite", "[convert][boundary]") {
    // Given: very large value 1e100 meter
    const entity::LengthConversionEngine engine = makeEngine();

    // When: convert to feet (1 meter = 3.28084 feet)
    const double result = engine.convert("meter", 1e100, "feet");

    // Then: finite positive
    REQUIRE(std::isfinite(result));
    REQUIRE(result > 0.0);
}

TEST_CASE("test_convert_boundary_six_decimal_precision_meter_feet", "[convert][boundary]") {
    // Given: 1 meter = 3.28084 feet; input with 6 decimal places
    const entity::LengthConversionEngine engine = makeEngine();
    const double input = 1.123456;

    // When: convert to feet
    const double result = engine.convert("meter", input, "feet");

    // Then: 1.123456 * 3.28084 to 6 decimal tolerance
    const double expected = input * test_constants::kMeterToFeetFactor;
    REQUIRE(result == Approx(expected).epsilon(1e-6));
}

TEST_CASE("test_convert_boundary_small_positive_meter_yard", "[convert][boundary]") {
    // Given: 1 meter = 1.09361 yard
    const entity::LengthConversionEngine engine = makeEngine();

    // When: convert tiny positive value
    const double result = engine.convert("meter", 1e-6, "yard");

    // Then: proportional tiny output
    REQUIRE(result == Approx(1e-6 * test_constants::kMeterToYardFactor)
                                .margin(test_constants::kEpsilonRelaxed));
}

TEST_CASE("test_convert_boundary_feet_fractional_meter_hub", "[convert][boundary]") {
    // Given: 3.28084 feet = 1 meter
    const entity::LengthConversionEngine engine = makeEngine();

    // When: convert 3.28084 feet to yard via meter hub
    const double result = engine.convert("feet", 3.28084, "yard");

    // Then: equals 1 meter expressed in yards (1.09361)
    REQUIRE(result == Approx(1.09361).margin(test_constants::kEpsilonRelaxed));
}

TEST_CASE("test_display_rounder_non_finite_passthrough", "[convert][boundary]") {
    // Given: non-finite values (no rounding applied)
    const double nanValue = std::numeric_limits<double>::quiet_NaN();
    const double infValue = std::numeric_limits<double>::infinity();

    // When / Then
    REQUIRE_FALSE(std::isfinite(entity::roundOneDecimal(nanValue)));
    REQUIRE(std::isnan(entity::roundOneDecimal(nanValue)));
    REQUIRE(entity::roundOneDecimal(infValue) == infValue);
}

TEST_CASE("test_convert_boundary_display_rounds_half_away", "[convert][boundary]") {
    // Given: display rounding 1 digit half-away-from-zero
    const entity::LengthConversionEngine engine = makeEngine();

    // When: convert 3.28084 feet to meter display
    const double display = engine.convertDisplay("feet", 3.28084, "meter");

    // Then: 1.0 meter display from PRD Sc.2 style
    REQUIRE(display == Approx(1.0).margin(test_constants::kEpsilonStrict));
}

TEST_CASE("test_unit_converter_facade_delegates_to_shared_catalog", "[convert][facade]") {
    const double meters = entity::UnitConverter::convert("meter", 2.5, "meter");

    REQUIRE(meters == Approx(2.5).margin(test_constants::kEpsilonStrict));
}
