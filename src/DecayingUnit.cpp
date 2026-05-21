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
    if (ratioToMeter <= 0.0) {
        throw std::invalid_argument("ratio to meter must be positive");
    }
    impl_->items.push_back({name, ratioToMeter});
}

double DecayingUnit::convert(const std::string& fromUnit, double value,
                            const std::string& toUnit) const {
    double fromRatio = 0.0;
    double toRatio = 0.0;
    for (const auto& item : impl_->items) {
        if (item.name == fromUnit) {
            fromRatio = item.ratioToMeter;
        }
        if (item.name == toUnit) {
            toRatio = item.ratioToMeter;
        }
    }
    if (fromRatio <= 0.0 || toRatio <= 0.0) {
        throw std::invalid_argument("unknown unit");
    }
    const double meters = value * fromRatio;
    return meters / toRatio;
}

std::vector<std::pair<std::string, double>> DecayingUnit::convertAll(
    const std::string& fromUnit, double value) const {
    std::vector<std::pair<std::string, double>> rows;
    rows.reserve(impl_->items.size());
    for (const auto& item : impl_->items) {
        rows.emplace_back(item.name, convert(fromUnit, value, item.name));
    }
    return rows;
}
