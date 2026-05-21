#pragma once

#include <stdexcept>
#include <string>

#include "entity/UnitCatalog.hpp"

namespace entity {

class CatalogJsonError : public std::runtime_error {
public:
    explicit CatalogJsonError(const std::string& message) : std::runtime_error(message) {}
};

// Resolves config/units.json from cwd or parent dirs (ctest runs from build/).
std::string resolveDefaultUnitsJsonPath();

std::string readConfigFileContent(const std::string& path);

UnitCatalog loadCatalogFromJsonContent(const std::string& content);
UnitCatalog loadCatalogFromJsonFile(const std::string& path);

}  // namespace entity
