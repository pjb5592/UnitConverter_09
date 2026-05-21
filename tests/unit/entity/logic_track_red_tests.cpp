#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <string>

#include "data/ConfigLoader.hpp"
#include "entity/LengthConversionEngine.hpp"
#include "entity/UnitCatalog.hpp"
#include "fixtures/TestConstants.hpp"

using Catch::Approx;

namespace fs = std::filesystem;

namespace {

entity::UnitCatalog defaultCatalog() {
    return entity::UnitCatalog::withDefaultUnits();
}

entity::LengthConversionEngine defaultEngine() {
    return entity::LengthConversionEngine(defaultCatalog());
}

std::string projectRoot() {
    const fs::path here = fs::current_path();
    if (fs::exists(here / "config" / "units.json")) {
        return here.string();
    }
    if (fs::exists(here / ".." / "config" / "units.json")) {
        return (here / "..").string();
    }
    if (fs::exists(here / ".." / ".." / "config" / "units.json")) {
        return (here / ".." / "..").string();
    }
    return here.string();
}

std::string configPath(const std::string& name) {
    return (fs::path(projectRoot()) / "config" / name).string();
}

}  // namespace

// TC-B-01 — convert(fromUnit, value, toUnit) meter→feet ε≤1e-5
// Scenario: convert("meter", 2.5, "feet") == 8.20210
// Invariant: INV-D01 1 meter = 3.28084 feet; 허브 환산 정확도 1e-5
TEST_CASE("TC-B-01_convert_meter_to_feet_within_1e5", "[red][track-b][TC-B-01]") {
    const entity::LengthConversionEngine engine = defaultEngine();

    const double result = engine.convert("meter", 2.5, "feet");

    REQUIRE(result == Approx(8.20210).margin(test_constants::kEpsilonRelaxed));
}

// TC-B-02 — convert meter→yard ε≤1e-5
// Scenario: convert("meter", 2.5, "yard") == 2.734025
// Invariant: INV-D01 1 meter = 1.09361 yard
TEST_CASE("TC-B-02_convert_meter_to_yard_within_1e5", "[red][track-b][TC-B-02]") {
    const entity::LengthConversionEngine engine = defaultEngine();

    const double result = engine.convert("meter", 2.5, "yard");

    REQUIRE(result == Approx(2.734025).margin(test_constants::kEpsilonRelaxed));
}

// TC-B-03 — convertAll(fromUnit, value) → 모든 등록 단위
// Scenario: convertAll("meter", 1.0) → 3 entries meter/feet/yard
// Invariant: INV-D02 |convertAll| = 등록 단위 수; 각 target 1회
TEST_CASE("TC-B-03_convert_all_returns_all_registered_units", "[red][track-b][TC-B-03]") {
    const entity::LengthConversionEngine engine = defaultEngine();

    const auto rows = engine.convertAll("meter", 1.0);

    REQUIRE(rows.size() == 3);
    REQUIRE(rows[0].first == "meter");
    REQUIRE(rows[1].first == "feet");
    REQUIRE(rows[2].first == "yard");
    REQUIRE(rows[0].second == Approx(1.0).margin(test_constants::kEpsilonStrict));
    REQUIRE(rows[1].second == Approx(test_constants::kMeterToFeetFactor)
                                        .margin(test_constants::kEpsilonRelaxed));
    REQUIRE(rows[2].second == Approx(test_constants::kMeterToYardFactor)
                                        .margin(test_constants::kEpsilonRelaxed));
}

// T-03 — registerUnit 후 convertAll이 동일 catalog 사용 (R-07)
// Scenario: register fathom on catalog → convertAll includes fathom
// Invariant: INV-D02 등록 단위가 convertAll 행에 포함
TEST_CASE("T-03_register_unit_then_convert_all_uses_same_catalog", "[red][track-b][T-03]") {
    entity::UnitCatalog catalog = defaultCatalog();
    catalog.registerUnit("fathom", 1.0 / 1.8288);
    const entity::LengthConversionEngine engine(catalog);

    const auto rows = engine.convertAll("fathom", 1.0);

    bool foundFathom = false;
    for (const auto& row : rows) {
        if (row.first == "fathom") {
            foundFathom = true;
            REQUIRE(row.second == Approx(1.0).margin(test_constants::kEpsilonRelaxed));
        }
    }
    REQUIRE(foundFathom);
}

// TC-B-04 — registerUnit(name, ratio_to_meter) 후 변환
// Scenario: 1 inch = 0.0254 meter → convert("inch",1,"meter")==0.0254
// Invariant: INV-D03 양수 ratio; 등록 직후 허브 환산
TEST_CASE("TC-B-04_register_unit_then_convert", "[red][track-b][TC-B-04]") {
    entity::UnitCatalog catalog = defaultCatalog();
    catalog.registerUnit("inch", 1.0 / 0.0254);
    const entity::LengthConversionEngine engine(catalog);

    const double meters = engine.convert("inch", 1.0, "meter");

    REQUIRE(meters == Approx(0.0254).margin(test_constants::kEpsilonRelaxed));
}

// TC-B-05 — loadConfig(valid path) JSON 비율 적용
// Scenario: loadConfig("config/units.json") → feet 3.28084, yard 1.09361
// Invariant: INV-D04 설정 factorToMeter가 카탈로그에 반영
TEST_CASE("TC-B-05_load_config_valid_json_applies_ratios", "[red][track-b][TC-B-05]") {
    const std::string path = configPath("units.json");

    const entity::UnitCatalog catalog = data::ConfigLoader::loadConfig(path);

    REQUIRE(catalog.factorOf("feet") ==
            Approx(test_constants::kMeterToFeetFactor).margin(test_constants::kEpsilonStrict));
    REQUIRE(catalog.factorOf("yard") ==
            Approx(test_constants::kMeterToYardFactor).margin(test_constants::kEpsilonStrict));
}

// TC-B-06 — loadConfig(invalid path) 기본값 유지
// Scenario: loadConfig("not_found.json") → feet 3.28084, yard 1.09361
// Invariant: INV-D05 파일 없음 시 throw 없이 기본 3단위 비율 유지
TEST_CASE("TC-B-06_load_config_missing_path_keeps_defaults", "[red][track-b][TC-B-06]") {
    const std::string path = configPath("not_found.json");

    const entity::UnitCatalog catalog = data::ConfigLoader::loadConfig(path);

    REQUIRE(catalog.factorOf("feet") ==
            Approx(test_constants::kMeterToFeetFactor).margin(test_constants::kEpsilonStrict));
    REQUIRE(catalog.factorOf("yard") ==
            Approx(test_constants::kMeterToYardFactor).margin(test_constants::kEpsilonStrict));
}
