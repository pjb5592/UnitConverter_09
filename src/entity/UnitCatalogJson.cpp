#include "entity/UnitCatalogJson.hpp"

#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>

namespace entity {

namespace {

double parseFactorToken(const std::string& token) {
    try {
        return std::stod(token);
    } catch (const std::exception&) {
        throw CatalogJsonError("invalid factor in config");
    }
}

void addUnit(UnitCatalog& catalog, const std::string& name, double factor) {
    if (factor <= 0.0) {
        throw CatalogJsonError("invalid factor in config");
    }
    try {
        catalog.registerUnit(name, factor);
    } catch (const std::invalid_argument&) {
        throw CatalogJsonError("invalid unit entry in config");
    }
}

}  // namespace

std::string readConfigFileContent(const std::string& path) {
    std::ifstream input(path);
    if (!input.is_open()) {
        throw CatalogJsonError("Failed to load unit config: " + path);
    }
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

std::string resolveDefaultUnitsJsonPath() {
    namespace fs = std::filesystem;
    const fs::path cwd = fs::current_path();
    const fs::path candidates[] = {
        cwd / "config" / "units.json",
        cwd / ".." / "config" / "units.json",
        cwd / ".." / ".." / "config" / "units.json",
    };
    for (const fs::path& candidate : candidates) {
        if (fs::exists(candidate)) {
            return candidate.string();
        }
    }
    throw CatalogJsonError("config/units.json not found from " + cwd.string());
}

UnitCatalog loadCatalogFromJsonContent(const std::string& content) {
    UnitCatalog catalog = UnitCatalog::empty();
    static const std::regex entryRegex(
        R"re("name"\s*:\s*"([a-z][a-z0-9_]*)"\s*,\s*"factorToMeter"\s*:\s*([-+0-9.eE]+))re");
    const auto begin = std::sregex_iterator(content.begin(), content.end(), entryRegex);
    const auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        addUnit(catalog, (*it)[1].str(), parseFactorToken((*it)[2].str()));
    }
    if (catalog.size() == 0) {
        throw CatalogJsonError("no units found in config");
    }
    return catalog;
}

UnitCatalog loadCatalogFromJsonFile(const std::string& path) {
    return loadCatalogFromJsonContent(readConfigFileContent(path));
}

void registerUnitFromFactorToken(UnitCatalog& catalog, const std::string& name,
                                 const std::string& factorToken) {
    addUnit(catalog, name, parseFactorToken(factorToken));
}

UnitCatalog loadCatalogFromYamlContent(const std::string& content) {
    UnitCatalog catalog = UnitCatalog::empty();
    static const std::regex yamlEntry(
        R"re(-\s*name:\s*([a-z][a-z0-9_]*)\s*[\r\n]+\s*factorToMeter:\s*([-+0-9.eE]+))re");
    const auto begin = std::sregex_iterator(content.begin(), content.end(), yamlEntry);
    const auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        registerUnitFromFactorToken(catalog, (*it)[1].str(), (*it)[2].str());
    }
    if (catalog.size() == 0) {
        throw CatalogJsonError("no units found in yaml config");
    }
    return catalog;
}

UnitCatalog loadCatalogFromYamlFile(const std::string& path) {
    return loadCatalogFromYamlContent(readConfigFileContent(path));
}

}  // namespace entity
