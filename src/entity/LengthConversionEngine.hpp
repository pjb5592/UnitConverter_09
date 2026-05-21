#pragma once

#include <string>
#include <utility>
#include <vector>

#include "entity/UnitCatalog.hpp"

namespace entity {

class LengthConversionEngine {
public:
    static constexpr double kEpsilon = 1e-9;

    explicit LengthConversionEngine(const UnitCatalog& catalog);

    double convert(const std::string& fromUnit, double value, const std::string& toUnit) const;
    double convertDisplay(const std::string& fromUnit, double value, const std::string& toUnit) const;
    std::vector<std::pair<std::string, double>> convertAll(const std::string& fromUnit,
                                                         double value) const;

private:
    double toCanonicalMeters(const std::string& unit, double value) const;
    double fromCanonicalMeters(const std::string& unit, double meters) const;

    UnitCatalog catalog_;
};

}  // namespace entity
