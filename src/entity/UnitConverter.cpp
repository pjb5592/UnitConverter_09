#include "entity/UnitConverter.hpp"

#include "entity/LengthConversionRatios.hpp"

#include <unordered_map>

namespace entity {

namespace {

std::unordered_map<std::string, double>& customRatioToMeter() {
    static std::unordered_map<std::string, double> ratios;
    return ratios;
}

}  // namespace

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
    if (fromUnit == "meter" && toUnit == "meter") {
        return value;
    }
    const auto it = customRatioToMeter().find(fromUnit);
    if (it != customRatioToMeter().end() && toUnit == "meter") {
        return value * it->second;
    }
    return 0.0;
}

std::vector<std::pair<std::string, double>> UnitConverter::convertAll(const std::string& fromUnit,
                                                                      double value) {
    if (fromUnit != "meter") {
        return {};
    }
    return {
        {"meter", value},
        {"feet", convert("meter", value, "feet")},
        {"yard", convert("meter", value, "yard")},
    };
}

void UnitConverter::registerUnit(const std::string& name, double ratioToMeter) {
    customRatioToMeter()[name] = ratioToMeter;
}

}  // namespace entity
