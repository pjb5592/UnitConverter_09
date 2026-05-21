#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <string>

#include "entity/UnitCatalogJson.hpp"

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

TEST_CASE("T-04_valid_json_content_loads_units", "[catalog_json][T-04]") {
    const std::string content =
        R"({"units":[{"name":"meter","factorToMeter":1.0},{"name":"feet","factorToMeter":3.28084}]})";

    const entity::UnitCatalog catalog = entity::loadCatalogFromJsonContent(content);

    REQUIRE(catalog.size() == 2);
    REQUIRE(catalog.hasUnit("meter"));
    REQUIRE(catalog.hasUnit("feet"));
}
