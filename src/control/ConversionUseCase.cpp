#include "control/ConversionUseCase.hpp"

#include "boundary/InputParser.hpp"
#include "boundary/OutputFormatter.hpp"
#include "entity/LengthConversionEngine.hpp"

namespace control {

ConversionUseCase::ConversionUseCase(const entity::UnitCatalog& catalog) : catalog_(catalog) {}

std::vector<boundary::ConversionRow> ConversionUseCase::buildConversionRows(
    const boundary::ParsedInput& parsed) const {
    const entity::LengthConversionEngine engine(catalog_);
    std::vector<boundary::ConversionRow> conversions;
    conversions.reserve(catalog_.size());
    for (const auto& unit : catalog_.units()) {
        conversions.push_back(
            {unit.name, engine.convert(parsed.unit, parsed.value, unit.name)});
    }
    return conversions;
}

std::vector<std::string> ConversionUseCase::convertTable(const std::string& line) const {
    const boundary::ParsedInput parsed = boundary::InputParser::parseConvertLine(line, catalog_);
    const std::string sourceValueText = boundary::OutputFormatter::formatSourceValueToken(
        boundary::InputParser::valueTokenFromLine(line));

    const std::vector<boundary::ConversionRow> conversions = buildConversionRows(parsed);
    std::vector<std::string> lines;
    lines.reserve(conversions.size());
    for (const auto& row : conversions) {
        lines.push_back(boundary::OutputFormatter::formatTableLine(sourceValueText, parsed.unit,
                                                                   row.targetValue, row.targetUnit));
    }
    return lines;
}

std::vector<std::string> ConversionUseCase::convertDisplayTable(const std::string& line) const {
    const boundary::ParsedInput parsed = boundary::InputParser::parseConvertLine(line, catalog_);
    const entity::LengthConversionEngine engine(catalog_);

    std::vector<std::string> lines;
    lines.reserve(catalog_.size());
    for (const auto& unit : catalog_.units()) {
        const double targetDisplay =
            engine.convertDisplay(parsed.unit, parsed.value, unit.name);
        lines.push_back(boundary::OutputFormatter::formatDisplayTableLine(
            parsed.value, parsed.unit, targetDisplay, unit.name));
    }
    return lines;
}

std::string ConversionUseCase::convertJson(const std::string& line) const {
    const boundary::ParsedInput parsed = boundary::InputParser::parseConvertLine(line, catalog_);
    return boundary::OutputFormatter::formatJson(parsed.unit, parsed.value,
                                                 buildConversionRows(parsed));
}

}  // namespace control
