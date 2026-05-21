#pragma once

#include <string>
#include <vector>

#include "entity/UnitCatalog.hpp"

namespace control {

class ConversionUseCase {
public:
    explicit ConversionUseCase(const entity::UnitCatalog& catalog);

    std::vector<std::string> convertTable(const std::string& line) const;
    std::string convertJson(const std::string& line) const;

private:
    entity::UnitCatalog catalog_;
};

}  // namespace control
