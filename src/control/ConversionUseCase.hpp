#pragma once

#include <string>
#include <vector>

#include "boundary/InputParser.hpp"
#include "boundary/OutputFormatter.hpp"
#include "entity/UnitCatalog.hpp"

namespace control {

class ConversionUseCase {
public:
    explicit ConversionUseCase(const entity::UnitCatalog& catalog);

    std::vector<std::string> convertTable(const std::string& line) const;
    std::vector<std::string> convertDisplayTable(const std::string& line) const;
    std::string convertJson(const std::string& line) const;

private:
    std::vector<boundary::ConversionRow> buildConversionRows(
        const boundary::ParsedInput& parsed) const;

    std::vector<std::string> formatTableLines(const boundary::ParsedInput& parsed,
                                              const std::string& line) const;

    std::vector<std::string> formatDisplayTableLines(const boundary::ParsedInput& parsed) const;

    entity::UnitCatalog catalog_;
};

}  // namespace control
