#include "data/ConfigLoader.hpp"

#include "entity/UnitCatalogJson.hpp"

#include <fstream>
#include <regex>
#include <sstream>

namespace data {

namespace {

std::string readFile(const std::string& path) {
    std::ifstream input(path);
    if (!input.is_open()) {
        throw ConfigLoadError("Failed to load unit config: " + path);
    }
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

entity::UnitCatalog parseUnitsPayload(const std::string& content) {
    try {
        return entity::loadCatalogFromJsonContent(content);
    } catch (const entity::CatalogJsonError& ex) {
        throw ConfigLoadError(ex.what());
    }
}

}  // namespace

entity::UnitCatalog ConfigLoader::loadFromJson(const std::string& path) {
    return parseUnitsPayload(readFile(path));
}

entity::UnitCatalog ConfigLoader::loadFromYaml(const std::string& path) {
    const std::string content = readFile(path);
    static const std::regex yamlEntry(
        R"re(-\s*name:\s*([a-z][a-z0-9_]*)\s*[\r\n]+\s*factorToMeter:\s*([-+0-9.eE]+))re");
    const auto begin = std::sregex_iterator(content.begin(), content.end(), yamlEntry);
    const auto end = std::sregex_iterator();
    entity::UnitCatalog catalog = entity::UnitCatalog::empty();
    for (auto it = begin; it != end; ++it) {
        try {
            catalog.registerUnit((*it)[1].str(), std::stod((*it)[2].str()));
        } catch (const std::invalid_argument&) {
            throw ConfigLoadError("invalid unit entry in yaml config");
        }
    }
    if (catalog.size() == 0) {
        throw ConfigLoadError("no units found in yaml config");
    }
    return catalog;
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
