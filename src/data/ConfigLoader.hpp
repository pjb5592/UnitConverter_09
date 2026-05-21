#pragma once

#include <string>

#include "entity/UnitCatalog.hpp"

namespace data {

class ConfigLoadError : public std::runtime_error {
public:
    explicit ConfigLoadError(const std::string& message) : std::runtime_error(message) {}
};

class ConfigLoader {
public:
    static entity::UnitCatalog loadConfig(const std::string& path);

    static entity::UnitCatalog loadFromJson(const std::string& path);
    static entity::UnitCatalog loadFromYaml(const std::string& path);
    static entity::UnitCatalog loadWithDefaultsOnMissing(const std::string& path);
};

}  // namespace data
