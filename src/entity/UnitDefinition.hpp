#pragma once

#include <string>

namespace entity {

struct UnitDefinition {
    std::string name;
    double factorToMeter{1.0};
};

}  // namespace entity
