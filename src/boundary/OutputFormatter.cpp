#include "boundary/OutputFormatter.hpp"

#include "entity/DisplayRounder.hpp"

#include <iomanip>
#include <sstream>

namespace boundary {

namespace {

constexpr std::size_t kMinFractionalDigitsForSixDigitDisplay = 5;
constexpr std::size_t kSixDigitDisplayFractionalWidth = 6;
constexpr int kTableTargetPrecision = 6;
constexpr int kJsonTargetPrecision = 6;

}  // namespace

std::string OutputFormatter::formatSourceValueToken(const std::string& valueToken) {
    const std::size_t dot = valueToken.find('.');
    if (dot == std::string::npos) {
        return valueToken;
    }
    std::string fractional = valueToken.substr(dot + 1);
    if (fractional.size() >= kMinFractionalDigitsForSixDigitDisplay) {
        while (fractional.size() < kSixDigitDisplayFractionalWidth) {
            fractional.push_back('0');
        }
        return valueToken.substr(0, dot + 1) + fractional;
    }
    return valueToken;
}

std::string OutputFormatter::formatTableLine(const std::string& sourceValueText,
                                             const std::string& sourceUnit,
                                             double targetValue,
                                             const std::string& targetUnit) {
    std::ostringstream line;
    line << sourceValueText << ' ' << sourceUnit << " = ";
    line << std::fixed << std::setprecision(kTableTargetPrecision) << targetValue << ' '
         << targetUnit;
    return line.str();
}

std::string OutputFormatter::formatDisplayTableLine(double sourceValue,
                                                    const std::string& sourceUnit,
                                                    double targetValue,
                                                    const std::string& targetUnit) {
    std::ostringstream line;
    line << entity::roundOneDecimal(sourceValue) << ' ' << sourceUnit << " = " << targetValue << ' '
         << targetUnit;
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
             << std::setprecision(kJsonTargetPrecision) << conversions[i].targetValue << '}';
    }
    json << "]}";
    return json.str();
}

}  // namespace boundary
