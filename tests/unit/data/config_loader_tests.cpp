#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "data/ConfigLoader.hpp"
#include "fixtures/TestConstants.hpp"

using Catch::Approx;

namespace fs = std::filesystem;

namespace {

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

void writeTempFile(const fs::path& path, const std::string& content) {
    std::ofstream out(path);
    REQUIRE(out.is_open());
    out << content;
}

}  // namespace

TEST_CASE("test_config_json_valid_loads_three_units", "[config][json]") {
    // Given: config/units.json with meter/feet/yard factors
    const std::string path = configPath("units.json");

    // When: load JSON
    const entity::UnitCatalog catalog = data::ConfigLoader::loadFromJson(path);

    // Then: 1 meter = 3.28084 feet, 1 meter = 1.09361 yard
    REQUIRE(catalog.size() == 3);
    REQUIRE(catalog.factorOf("feet") ==
            Approx(test_constants::kMeterToFeetFactor).margin(test_constants::kEpsilonStrict));
    REQUIRE(catalog.factorOf("yard") ==
            Approx(test_constants::kMeterToYardFactor).margin(test_constants::kEpsilonStrict));
}

TEST_CASE("test_config_json_missing_path_throws", "[config][json]") {
    // Given: non-existent path
    const std::string path = configPath("does_not_exist.json");

    // When / Then: ConfigLoadError
    REQUIRE_THROWS_AS(data::ConfigLoader::loadFromJson(path), data::ConfigLoadError);
}

TEST_CASE("test_config_json_invalid_syntax_throws", "[config][json]") {
    // Given: broken JSON in temp file
    const fs::path temp = fs::temp_directory_path() / "units_invalid_test.json";
    writeTempFile(temp, "{ not valid json }}}");

    // When / Then
    REQUIRE_THROWS_AS(data::ConfigLoader::loadFromJson(temp.string()), data::ConfigLoadError);
    fs::remove(temp);
}

TEST_CASE("test_config_json_zero_factor_throws", "[config][json]") {
    // Given: invalid factor <= 0
    const fs::path temp = fs::temp_directory_path() / "units_zero_factor.json";
    writeTempFile(temp,
                  R"({"units":[{"name":"meter","factorToMeter":0.0},{"name":"feet","factorToMeter":3.28084}]})");

    // When / Then
    REQUIRE_THROWS_AS(data::ConfigLoader::loadFromJson(temp.string()), data::ConfigLoadError);
    fs::remove(temp);
}

TEST_CASE("test_config_yaml_valid_loads_factors", "[config][yaml]") {
    // Given: YAML config mirroring JSON factors (1 meter = 3.28084 feet)
    const fs::path temp = fs::temp_directory_path() / "units_valid_test.yaml";
    writeTempFile(temp,
                  "units:\n"
                  "  - name: meter\n"
                  "    factorToMeter: 1.0\n"
                  "  - name: feet\n"
                  "    factorToMeter: 3.28084\n"
                  "  - name: yard\n"
                  "    factorToMeter: 1.09361\n");

    // When
    const entity::UnitCatalog catalog = data::ConfigLoader::loadFromYaml(temp.string());

    // Then
    REQUIRE(catalog.size() == 3);
    REQUIRE(catalog.factorOf("feet") ==
            Approx(3.28084).margin(test_constants::kEpsilonStrict));
    fs::remove(temp);
}

TEST_CASE("test_config_yaml_missing_file_throws", "[config][yaml]") {
    // Given: missing yaml path
    const std::string path = configPath("missing_units.yaml");

    // When / Then
    REQUIRE_THROWS_AS(data::ConfigLoader::loadFromYaml(path), data::ConfigLoadError);
}

TEST_CASE("test_config_defaults_on_missing_keeps_328084_109361", "[config][json]") {
    // Given: missing path (README TC-B-07 style fallback)
    const std::string path = configPath("not_found.json");

    // When: load with defaults
    const entity::UnitCatalog catalog = data::ConfigLoader::loadWithDefaultsOnMissing(path);

    // Then: default factors preserved
    REQUIRE(catalog.factorOf("feet") ==
            Approx(test_constants::kMeterToFeetFactor).margin(test_constants::kEpsilonStrict));
    REQUIRE(catalog.factorOf("yard") ==
            Approx(test_constants::kMeterToYardFactor).margin(test_constants::kEpsilonStrict));
}

TEST_CASE("test_config_json_duplicate_names_last_wins_or_throws", "[config][json]") {
    // Given: duplicate unit name in JSON
    const fs::path temp = fs::temp_directory_path() / "units_dup.json";
    writeTempFile(temp,
                  R"({"units":[{"name":"meter","factorToMeter":1.0},{"name":"meter","factorToMeter":2.0}]})");

    // When / Then: duplicate registration rejected
    REQUIRE_THROWS_AS(data::ConfigLoader::loadFromJson(temp.string()), data::ConfigLoadError);
    fs::remove(temp);
}
