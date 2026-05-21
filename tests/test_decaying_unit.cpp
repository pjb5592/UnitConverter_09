#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

#include "DecayingUnit.hpp"
#include "fixtures/TestConstants.hpp"

using Catch::Approx;

namespace {

constexpr double kCubitToMeter = 0.4572;
constexpr double kMeterToFeet = 3.28084;
constexpr double kCubitToFeet = kCubitToMeter * kMeterToFeet;
constexpr double kMeterToCubit = 1.0 / kCubitToMeter;

}  // namespace

TEST_CASE("감쇠 단위 변환", "[bonus]") {
    const auto margin = test_constants::kEpsilonRelaxed;

    SECTION("BT-01 registerUnit cubit then cubit to meter") {
        DecayingUnit converter;
        converter.registerUnit("cubit", kCubitToMeter);

        const double result = converter.convert("cubit", 1.0, "meter");

        REQUIRE(result == Approx(kCubitToMeter).margin(margin));
    }

    SECTION("BT-02 registerUnit cubit then meter to cubit reverse") {
        DecayingUnit converter;
        converter.registerUnit("cubit", kCubitToMeter);

        const double result = converter.convert("meter", 1.0, "cubit");

        REQUIRE(result == Approx(kMeterToCubit).margin(margin));
    }

    SECTION("BT-03 registerUnit cubit then cubit to feet cross") {
        DecayingUnit converter;
        converter.registerUnit("cubit", kCubitToMeter);

        const double result = converter.convert("cubit", 1.0, "feet");

        REQUIRE(result == Approx(kCubitToFeet).margin(margin));
    }

    SECTION("BT-04 negative ratio registration throws invalid_argument") {
        DecayingUnit converter;

        REQUIRE_THROWS_AS(converter.registerUnit("bad", -0.4572), std::invalid_argument);
    }

    SECTION("BT-05 registerUnit cubit then convertAll returns all units") {
        DecayingUnit converter;
        converter.registerUnit("cubit", kCubitToMeter);

        const auto rows = converter.convertAll("cubit", 1.0);

        REQUIRE(rows.size() == 4);
        bool foundMeter = false;
        bool foundFeet = false;
        bool foundYard = false;
        bool foundCubit = false;
        for (const auto& row : rows) {
            if (row.first == "meter") {
                foundMeter = true;
                REQUIRE(row.second == Approx(kCubitToMeter).margin(margin));
            } else if (row.first == "feet") {
                foundFeet = true;
                REQUIRE(row.second == Approx(kCubitToFeet).margin(margin));
            } else if (row.first == "yard") {
                foundYard = true;
                REQUIRE(row.second ==
                        Approx(kCubitToMeter / (1.0 / 1.09361)).margin(margin));
            } else if (row.first == "cubit") {
                foundCubit = true;
                REQUIRE(row.second == Approx(1.0).margin(margin));
            }
        }
        REQUIRE(foundMeter);
        REQUIRE(foundFeet);
        REQUIRE(foundYard);
        REQUIRE(foundCubit);
    }

    SECTION("BT-06 meter to feet regression unchanged") {
        DecayingUnit converter;

        const double result = converter.convert("meter", 1.0, "feet");

        REQUIRE(result == Approx(kMeterToFeet).margin(margin));
    }
}
