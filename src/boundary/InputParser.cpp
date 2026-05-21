#include "boundary/InputParser.hpp"

#include <cctype>
#include <stdexcept>

namespace boundary {

namespace {

bool isValidUnitName(const std::string& unit) {
    if (unit.empty()) {
        return false;
    }
    if (!std::islower(static_cast<unsigned char>(unit.front()))) {
        return false;
    }
    for (char ch : unit) {
        if (!std::islower(static_cast<unsigned char>(ch)) && ch != '_' &&
            !std::isdigit(static_cast<unsigned char>(ch))) {
            return false;
        }
    }
    return true;
}

}  // namespace

ParsedInput InputParser::parse(const std::string& line) {
    if (line.empty()) {
        throw std::invalid_argument("empty input");
    }

    const std::size_t colon = line.find(':');
    if (colon == std::string::npos) {
        throw std::invalid_argument("invalid format");
    }

    ParsedInput result;
    result.unit = line.substr(0, colon);
    const std::string valueToken = line.substr(colon + 1);

    if (!isValidUnitName(result.unit)) {
        throw std::invalid_argument("invalid unit name");
    }

    try {
        std::size_t consumed = 0;
        result.value = std::stod(valueToken, &consumed);
        if (consumed != valueToken.size()) {
            throw std::invalid_argument("invalid number");
        }
    } catch (const std::exception&) {
        throw std::invalid_argument("invalid number");
    }

    if (result.value < 0.0) {
        throw std::invalid_argument("negative length");
    }

    return result;
}

ParsedInput InputParser::parseConvertLine(const std::string& line,
                                         const entity::UnitCatalog& catalog) {
    const ParsedInput parsed = parse(line);
    if (!catalog.hasUnit(parsed.unit)) {
        throw std::invalid_argument("unknown unit");
    }
    return parsed;
}

}  // namespace boundary
