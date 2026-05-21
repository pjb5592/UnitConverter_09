#pragma once

#include <string>
#include <utility>
#include <vector>

namespace entity {

// RED contract surface (GREEN 단계에서 구현 예정)
class UnitConverter {
public:
    static double convert(const std::string& fromUnit, double value, const std::string& toUnit);

    static std::vector<std::pair<std::string, double>> convertAll(const std::string& fromUnit,
                                                                  double value);

    static void registerUnit(const std::string& name, double ratioToMeter);
};

}  // namespace entity
