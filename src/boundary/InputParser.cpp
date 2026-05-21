#include "boundary/InputParser.hpp"

#include "boundary/ErrorCodes.hpp"

#include <cctype>
#include <stdexcept>

namespace boundary {

namespace {

struct UnitValueTokens {
    std::string unit;
    std::string valueToken;
};

UnitValueTokens splitUnitValueLine(const std::string& line) {
    if (line.empty()) {
        throw std::invalid_argument("empty input");
    }

    const std::size_t colon = line.find(':');
    if (colon == std::string::npos) {
        throw std::invalid_argument("invalid format");
    }

    return {line.substr(0, colon), line.substr(colon + 1)};
}

double parseNonNegativeValue(const std::string& valueToken) {
    try {
        std::size_t consumed = 0;
        const double value = std::stod(valueToken, &consumed);
        if (consumed != valueToken.size()) {
            throw std::invalid_argument("invalid number");
        }
        if (value < 0.0) {
            throw std::invalid_argument("negative length");
        }
        return value;
    } catch (const std::exception&) {
        throw std::invalid_argument("invalid number");
    }
}

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

std::string InputParser::valueTokenFromLine(const std::string& line) {
    return splitUnitValueLine(line).valueToken;
}

ParsedInput InputParser::parse(const std::string& line) {
    const UnitValueTokens tokens = splitUnitValueLine(line);

    if (!isValidUnitName(tokens.unit)) {
        throw std::invalid_argument("invalid unit name");
    }

    ParsedInput result;
    result.unit = tokens.unit;
    result.value = parseNonNegativeValue(tokens.valueToken);
    return result;
}

ParsedInput InputParser::parseConvertLine(const std::string& line,
                                         const entity::UnitCatalog& catalog) {
    const ParsedInput parsed = parse(line);
    if (!catalog.hasUnit(parsed.unit)) {
        throw std::invalid_argument(ErrorCodes::kUnknownUnitParseMessage);
    }
    return parsed;
}

}  // namespace boundary
