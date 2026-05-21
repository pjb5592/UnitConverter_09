#include "entity/UnitConverter.hpp"

#include "entity/LengthConversionRatios.hpp"

namespace entity {

double UnitConverter::convert(const std::string& fromUnit, double value, const std::string& toUnit) {
    if (fromUnit == "meter" && toUnit == "feet") {
        return value * length_conversion_ratios::kFeetPerMeter;
    }
    if (fromUnit == "meter" && toUnit == "yard") {
        return value * length_conversion_ratios::kYardPerMeter;
    }
    if (fromUnit == "feet" && toUnit == "meter") {
        return value / length_conversion_ratios::kFeetPerMeter;
    }
    return 0.0;
}

std::vector<std::pair<std::string, double>> UnitConverter::convertAll(const std::string& fromUnit,
                                                                      double value) {
    (void)fromUnit;
    (void)value;
    return {};
}

void UnitConverter::registerUnit(const std::string& name, double ratioToMeter) {
    (void)name;
    (void)ratioToMeter;
}

}  // namespace entity
