#pragma once

#include <string>
#include <vector>

namespace boundary {

// R-01 dual contract: formatTableLine/formatJson use kTableTargetPrecision (6);
// formatDisplayTableLine uses entity::roundOneDecimal (1) for CLI/Golden Master.

struct ConversionRow {
    std::string targetUnit;
    double targetValue{0.0};
};

class OutputFormatter {
public:
    static std::string formatSourceValueToken(const std::string& valueToken);

    static std::string formatTableLine(const std::string& sourceValueText,
                                       const std::string& sourceUnit,
                                       double targetValue,
                                       const std::string& targetUnit);

    static std::string formatDisplayTableLine(double sourceValue,
                                              const std::string& sourceUnit,
                                              double targetValue,
                                              const std::string& targetUnit);

    static std::string formatJson(const std::string& sourceUnit,
                                  double sourceValue,
                                  const std::vector<ConversionRow>& conversions);
};

}  // namespace boundary
