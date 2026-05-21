#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

// Legacy-style dynamic unit registry (ratio = meters per one unit).
// RED: implementation in DecayingUnit.cpp is intentionally incomplete.
class DecayingUnit {
public:
    DecayingUnit();
    ~DecayingUnit();

    void registerUnit(const std::string& name, double ratioToMeter);

    double convert(const std::string& fromUnit, double value,
                   const std::string& toUnit) const;

    std::vector<std::pair<std::string, double>> convertAll(const std::string& fromUnit,
                                                           double value) const;

private:
    struct Item;
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
