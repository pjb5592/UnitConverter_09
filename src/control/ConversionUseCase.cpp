#include "control/ConversionUseCase.hpp"

#include <stdexcept>

namespace control {

ConversionUseCase::ConversionUseCase(const entity::UnitCatalog& catalog) : catalog_(catalog) {}

std::vector<std::string> ConversionUseCase::convertTable(const std::string& line) const {
    if (line.find(':') == std::string::npos) {
        throw std::invalid_argument("invalid format");
    }
    (void)catalog_;
    return {};
}

std::string ConversionUseCase::convertJson(const std::string& line) const {
    (void)line;
    (void)catalog_;
    return "";
}

}  // namespace control
