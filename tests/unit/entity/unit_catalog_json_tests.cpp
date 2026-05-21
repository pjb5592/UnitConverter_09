#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <stdexcept>
#include <string>

#include "entity/UnitCatalogJson.hpp"

namespace fs = std::filesystem;

// T-04 — UnitCatalogJson content validation (invalid JSON, factor<=0, bad numeric)
// Invariant: CatalogJsonError preserves config failure semantics at entity layer
TEST_CASE("T-04_invalid_json_content_throws_catalog_json_error", "[catalog_json][T-04]") {
    REQUIRE_THROWS_AS(entity::loadCatalogFromJsonContent("{ not valid json }}}"),
                      entity::CatalogJsonError);
}

TEST_CASE("T-04_zero_factor_throws_catalog_json_error", "[catalog_json][T-04]") {
    const std::string content =
        R"({"units":[{"name":"meter","factorToMeter":0.0},{"name":"feet","factorToMeter":3.28084}]})";

    REQUIRE_THROWS_AS(entity::loadCatalogFromJsonContent(content), entity::CatalogJsonError);
}

TEST_CASE("T-04_negative_factor_throws_catalog_json_error", "[catalog_json][T-04]") {
    const std::string content = R"({"units":[{"name":"meter","factorToMeter":-1.0}]})";

    REQUIRE_THROWS_AS(entity::loadCatalogFromJsonContent(content), entity::CatalogJsonError);
}

TEST_CASE("T-04_no_units_in_content_throws_catalog_json_error", "[catalog_json][T-04]") {
    REQUIRE_THROWS_AS(entity::loadCatalogFromJsonContent(R"({"units":[]})"),
                      entity::CatalogJsonError);
}

TEST_CASE("T-04_valid_yaml_content_loads_units", "[catalog_json][T-04][yaml]") {
    const std::string content =
        "units:\n"
        "  - name: meter\n"
        "    factorToMeter: 1.0\n"
        "  - name: feet\n"
        "    factorToMeter: 3.28084\n";

    const entity::UnitCatalog catalog = entity::loadCatalogFromYamlContent(content);

    REQUIRE(catalog.size() == 2);
    REQUIRE(catalog.hasUnit("meter"));
    REQUIRE(catalog.hasUnit("feet"));
}

TEST_CASE("T-04_invalid_yaml_content_throws_catalog_json_error", "[catalog_json][T-04][yaml]") {
    REQUIRE_THROWS_AS(entity::loadCatalogFromYamlContent("units:\n  - name: meter\n"),
                      entity::CatalogJsonError);
}

TEST_CASE("T-04_resolve_default_path_throws_when_config_missing", "[catalog_json][T-04]") {
    const fs::path emptyCwd = fs::temp_directory_path() / "uc09_no_config_cwd";
    fs::create_directories(emptyCwd);
    const fs::path previous = fs::current_path();
    fs::current_path(emptyCwd);

    REQUIRE_THROWS_AS(entity::resolveDefaultUnitsJsonPath(), entity::CatalogJsonError);

    fs::current_path(previous);
    fs::remove_all(emptyCwd);
}

TEST_CASE("T-04_load_json_file_missing_throws_catalog_json_error", "[catalog_json][T-04]") {
    REQUIRE_THROWS_AS(entity::loadCatalogFromJsonFile("not/a/real/units.json"),
                      entity::CatalogJsonError);
}

TEST_CASE("T-04_duplicate_unit_in_json_throws_catalog_json_error", "[catalog_json][T-04]") {
    const std::string content =
        R"({"units":[{"name":"meter","factorToMeter":1.0},{"name":"meter","factorToMeter":2.0}]})";

    REQUIRE_THROWS_AS(entity::loadCatalogFromJsonContent(content), entity::CatalogJsonError);
}

TEST_CASE("T-04_valid_json_content_loads_units", "[catalog_json][T-04]") {
    const std::string content =
        R"({"units":[{"name":"meter","factorToMeter":1.0},{"name":"feet","factorToMeter":3.28084}]})";

    const entity::UnitCatalog catalog = entity::loadCatalogFromJsonContent(content);

    REQUIRE(catalog.size() == 2);
    REQUIRE(catalog.hasUnit("meter"));
    REQUIRE(catalog.hasUnit("feet"));
}
