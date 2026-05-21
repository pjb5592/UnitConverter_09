#pragma once

#include <string>
#include <vector>

namespace boundary {

struct ConversionRow {
    std::string targetUnit;
    double targetValue{0.0};
};

class OutputFormatter {
public:
    static std::string formatTableLine(double sourceValue,
                                       const std::string& sourceUnit,
                                       double targetValue,
                                       const std::string& targetUnit);

    static std::string formatJson(const std::string& sourceUnit,
                                  double sourceValue,
                                  const std::vector<ConversionRow>& conversions);
};

}  // namespace boundary
