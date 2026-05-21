#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "entity/UnitDefinition.hpp"

namespace entity {

class UnitCatalog {
public:
    void registerUnit(const std::string& name, double factorToMeter);
    bool hasUnit(const std::string& name) const;
    double factorOf(const std::string& name) const;
    const std::vector<UnitDefinition>& units() const;
    std::size_t size() const;

    static UnitCatalog withDefaultUnits();
    static UnitCatalog empty();

private:
    std::vector<UnitDefinition> units_;
};

}  // namespace entity
