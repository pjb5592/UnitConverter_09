#include "boundary/OutputFormatter.hpp"

#include <iomanip>
#include <sstream>

namespace boundary {

std::string OutputFormatter::formatTableLine(const std::string& sourceValueText,
                                             const std::string& sourceUnit,
                                             double targetValue,
                                             const std::string& targetUnit) {
    std::ostringstream line;
    line << sourceValueText << ' ' << sourceUnit << " = ";
    line << std::fixed << std::setprecision(6) << targetValue << ' ' << targetUnit;
    return line.str();
}

std::string OutputFormatter::formatJson(const std::string& sourceUnit,
                                      double sourceValue,
                                      const std::vector<ConversionRow>& conversions) {
    std::ostringstream json;
    json << R"({"source":{"unit":")" << sourceUnit << R"(","value":)" << sourceValue
         << R"(},"conversions":[)";
    for (std::size_t i = 0; i < conversions.size(); ++i) {
        if (i > 0) {
            json << ',';
        }
        json << R"({"unit":")" << conversions[i].targetUnit << R"(","value":)" << std::fixed
             << std::setprecision(6) << conversions[i].targetValue << '}';
    }
    json << "]}";
    return json.str();
}

}  // namespace boundary
