#include "control/ConversionUseCase.hpp"

#include "boundary/InputParser.hpp"

namespace control {

ConversionUseCase::ConversionUseCase(const entity::UnitCatalog& catalog) : catalog_(catalog) {}

std::vector<std::string> ConversionUseCase::convertTable(const std::string& line) const {
    boundary::InputParser::parse(line);
    (void)catalog_;
    return {};
}

std::string ConversionUseCase::convertJson(const std::string& line) const {
    (void)line;
    (void)catalog_;
    return "";
}

}  // namespace control
