#include "data/ConfigLoader.hpp"

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

void addUnit(entity::UnitCatalog& catalog, const std::string& name, double factor) {
    if (factor <= 0.0) {
        throw ConfigLoadError("invalid factor in config");
    }
    try {
        catalog.registerUnit(name, factor);
    } catch (const std::invalid_argument&) {
        throw ConfigLoadError("invalid unit entry in config");
    }
}

entity::UnitCatalog parseUnitsPayload(const std::string& content) {
    entity::UnitCatalog catalog = entity::UnitCatalog::empty();
    static const std::regex entryRegex(
        R"re("name"\s*:\s*"([a-z][a-z0-9_]*)"\s*,\s*"factorToMeter"\s*:\s*([-+0-9.eE]+))re");
    const auto begin = std::sregex_iterator(content.begin(), content.end(), entryRegex);
    const auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        addUnit(catalog, (*it)[1].str(), std::stod((*it)[2].str()));
    }
    if (catalog.size() == 0) {
        throw ConfigLoadError("no units found in config");
    }
    return catalog;
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
        addUnit(catalog, (*it)[1].str(), std::stod((*it)[2].str()));
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
