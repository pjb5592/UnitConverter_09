#pragma once

#include <string>

#include "entity/UnitCatalog.hpp"

namespace boundary {

struct ParsedInput {
    std::string unit;
    double value{0.0};
};

class InputParser {
public:
    static ParsedInput parse(const std::string& line);
    static ParsedInput parseConvertLine(const std::string& line, const entity::UnitCatalog& catalog);
};

}  // namespace boundary
