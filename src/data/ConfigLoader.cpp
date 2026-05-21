#include "data/ConfigLoader.hpp"

#include "entity/UnitCatalogJson.hpp"

namespace data {

entity::UnitCatalog ConfigLoader::loadFromJson(const std::string& path) {
    try {
        return entity::loadCatalogFromJsonFile(path);
    } catch (const entity::CatalogJsonError& ex) {
        throw ConfigLoadError(ex.what());
    }
}

entity::UnitCatalog ConfigLoader::loadFromYaml(const std::string& path) {
    try {
        return entity::loadCatalogFromYamlFile(path);
    } catch (const entity::CatalogJsonError& ex) {
        throw ConfigLoadError(ex.what());
    }
}

entity::UnitCatalog ConfigLoader::loadWithDefaultsOnMissing(const std::string& path) {
    try {
        return loadFromJson(path);
    } catch (const ConfigLoadError&) {
        return entity::UnitCatalog::withDefaultUnits();
    }
}

entity::UnitCatalog ConfigLoader::loadConfig(const std::string& path) {
    return loadWithDefaultsOnMissing(path);
}

}  // namespace data
