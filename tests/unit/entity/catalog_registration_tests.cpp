#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

#include "entity/LengthConversionEngine.hpp"
#include "entity/UnitCatalog.hpp"
#include "fixtures/TestConstants.hpp"

using Catch::Approx;

TEST_CASE("test_register_dynamic_cubit_then_convert_meter", "[register][dynamic]") {
    // Given: 1 cubit = 0.4572 meter → hub factor = 1/0.4572 (feet-per-meter convention)
    entity::UnitCatalog catalog = entity::UnitCatalog::withDefaultUnits();
    catalog.registerUnit("cubit", 1.0 / 0.4572);
    const entity::LengthConversionEngine engine(catalog);

    // When: convert 1 cubit to meter
    const double meters = engine.convert("cubit", 1.0, "meter");

    // Then: 0.4572 meter
    REQUIRE(meters == Approx(0.4572).margin(test_constants::kEpsilonRelaxed));
}

TEST_CASE("test_register_dynamic_increases_catalog_size", "[register][dynamic]") {
    // Given: 3 default units
    entity::UnitCatalog catalog = entity::UnitCatalog::withDefaultUnits();

    // When: register cubit
    catalog.registerUnit("cubit", 1.0 / 0.4572);

    // Then: size 4
    REQUIRE(catalog.size() == 4);
}

TEST_CASE("test_register_dynamic_convert_all_includes_cubit", "[register][dynamic]") {
    // Given: cubit registered (1 cubit = 0.4572 meter hub factor)
    entity::UnitCatalog catalog = entity::UnitCatalog::withDefaultUnits();
    catalog.registerUnit("cubit", 1.0 / 0.4572);
    const entity::LengthConversionEngine engine(catalog);

    // When: convertAll from meter 1.0
    const auto rows = engine.convertAll("meter", 1.0);

    // Then: 4 rows including cubit (1 m → 1/0.4572 cubit)
    REQUIRE(rows.size() == 4);
    bool foundCubit = false;
    for (const auto& row : rows) {
        if (row.first == "cubit") {
            foundCubit = true;
            REQUIRE(row.second == Approx(1.0 / 0.4572).margin(test_constants::kEpsilonRelaxed));
        }
    }
    REQUIRE(foundCubit);
}

TEST_CASE("test_register_exception_duplicate_unit_throws", "[register][dynamic]") {
    // Given: feet already registered (1 meter = 3.28084 feet)
    entity::UnitCatalog catalog = entity::UnitCatalog::withDefaultUnits();

    // When / Then: duplicate register
    REQUIRE_THROWS_AS(catalog.registerUnit("feet", 3.28084), std::invalid_argument);
}

TEST_CASE("test_register_exception_non_positive_factor_throws", "[register][dynamic]") {
    // Given: empty catalog
    entity::UnitCatalog catalog = entity::UnitCatalog::empty();

    // When / Then: invalid factor
    REQUIRE_THROWS_AS(catalog.registerUnit("bad", 0.0), std::invalid_argument);
    REQUIRE_THROWS_AS(catalog.registerUnit("worse", -1.0), std::invalid_argument);
}

TEST_CASE("test_register_dynamic_mile_then_convert_to_feet", "[register][dynamic]") {
    // Given: 1 mile = 1609.34 meter → hub factor 1609.34 (same as feet-per-meter style)
    entity::UnitCatalog catalog = entity::UnitCatalog::withDefaultUnits();
    catalog.registerUnit("mile", 1609.34);
    const entity::LengthConversionEngine engine(catalog);

    // When: 1609.34 mile-units to feet (equals 1 meter canonical)
    const double feet = engine.convert("mile", 1609.34, "feet");

    // Then: 1 meter = 3.28084 feet
    REQUIRE(feet == Approx(test_constants::kMeterToFeetFactor)
                             .margin(test_constants::kEpsilonRelaxed));
}

TEST_CASE("test_register_dynamic_has_unit_query", "[register][dynamic]") {
    // Given: cubit registered
    entity::UnitCatalog catalog = entity::UnitCatalog::withDefaultUnits();
    catalog.registerUnit("cubit", 1.0 / 0.4572);

    // When / Then
    REQUIRE(catalog.hasUnit("cubit"));
    REQUIRE_FALSE(catalog.hasUnit("parsec"));
}
