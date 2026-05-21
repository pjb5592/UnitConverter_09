#include "entity/UnitCatalog.hpp"

#include "entity/UnitCatalogJson.hpp"

#include <algorithm>

namespace entity {

void UnitCatalog::registerUnit(const std::string& name, double factorToMeter) {
    if (factorToMeter <= 0.0) {
        throw std::invalid_argument("factor must be positive");
    }
    const auto found = std::find_if(units_.begin(), units_.end(),
                                    [&](const UnitDefinition& u) { return u.name == name; });
    if (found != units_.end()) {
        throw std::invalid_argument("duplicate unit");
    }
    units_.push_back(UnitDefinition{name, factorToMeter});
}

bool UnitCatalog::hasUnit(const std::string& name) const {
    return std::any_of(units_.begin(), units_.end(),
                       [&](const UnitDefinition& u) { return u.name == name; });
}

double UnitCatalog::factorOf(const std::string& name) const {
    const auto found = std::find_if(units_.begin(), units_.end(),
                                    [&](const UnitDefinition& u) { return u.name == name; });
    if (found == units_.end()) {
        throw std::invalid_argument("unknown unit: " + name);
    }
    return found->factorToMeter;
}

const std::vector<UnitDefinition>& UnitCatalog::units() const {
    return units_;
}

std::size_t UnitCatalog::size() const {
    return units_.size();
}

UnitCatalog UnitCatalog::withDefaultUnits() {
    return loadCatalogFromJsonFile(resolveDefaultUnitsJsonPath());
}

UnitCatalog UnitCatalog::empty() {
    return UnitCatalog{};
}

}  // namespace entity
