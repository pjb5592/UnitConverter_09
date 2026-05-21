#include "DecayingUnit.hpp"

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Item mirrors legacy UnitConverter catalog entries (do not modify UnitConverter.cpp).
struct DecayingUnit::Item {
    std::string name;
    double ratioToMeter;  // meters per 1 unit (e.g. 1 cubit = 0.4572 m)
};

struct DecayingUnit::Impl {
    std::vector<Item> items{
        {"meter", 1.0},
        {"feet", 1.0 / 3.28084},
        {"yard", 1.0 / 1.09361},
    };
};

DecayingUnit::DecayingUnit() : impl_(std::make_unique<Impl>()) {}

DecayingUnit::~DecayingUnit() = default;

void DecayingUnit::registerUnit(const std::string& name, double ratioToMeter) {
    (void)name;
    (void)ratioToMeter;
    // RED stub: no catalog update, no validation
}

double DecayingUnit::convert(const std::string& fromUnit, double value,
                            const std::string& toUnit) const {
    (void)fromUnit;
    (void)value;
    (void)toUnit;
    return 0.0;  // RED stub
}

std::vector<std::pair<std::string, double>> DecayingUnit::convertAll(
    const std::string& fromUnit, double value) const {
    (void)fromUnit;
    (void)value;
    return {};  // RED stub
}
