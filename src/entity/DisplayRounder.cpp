#include "entity/DisplayRounder.hpp"

#include <cmath>

namespace entity {

double roundOneDecimal(double value) {
    if (!std::isfinite(value)) {
        return value;
    }
    return std::round(value * 10.0) / 10.0;
}

}  // namespace entity
