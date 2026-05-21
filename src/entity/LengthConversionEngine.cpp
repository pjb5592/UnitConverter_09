#include "entity/LengthConversionEngine.hpp"

#include "entity/DisplayRounder.hpp"

#include <cmath>

namespace entity {

LengthConversionEngine::LengthConversionEngine(const UnitCatalog& catalog) : catalog_(catalog) {}

double LengthConversionEngine::toCanonicalMeters(const std::string& unit, double value) const {
    const double factor = catalog_.factorOf(unit);
    if (factor <= 0.0) {
        throw std::invalid_argument("invalid factor");
    }
    return value / factor;
}

double LengthConversionEngine::fromCanonicalMeters(const std::string& unit, double meters) const {
    const double factor = catalog_.factorOf(unit);
    return meters * factor;
}

double LengthConversionEngine::convert(const std::string& fromUnit, double value,
                                       const std::string& toUnit) const {
    const double meters = toCanonicalMeters(fromUnit, value);
    return fromCanonicalMeters(toUnit, meters);
}

double LengthConversionEngine::convertDisplay(const std::string& fromUnit, double value,
                                              const std::string& toUnit) const {
    return roundOneDecimal(convert(fromUnit, value, toUnit));
}

std::vector<std::pair<std::string, double>> LengthConversionEngine::convertAll(
    const std::string& fromUnit, double value) const {
    std::vector<std::pair<std::string, double>> results;
    results.reserve(catalog_.size());
    for (const auto& unit : catalog_.units()) {
        results.emplace_back(unit.name, convert(fromUnit, value, unit.name));
    }
    return results;
}

}  // namespace entity
