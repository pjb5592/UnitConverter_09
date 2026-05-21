#include "entity/UnitConverter.hpp"

#include "entity/LengthConversionEngine.hpp"
#include "entity/UnitCatalog.hpp"

namespace entity {

namespace {

UnitCatalog& catalogState() {
    static UnitCatalog catalog = UnitCatalog::withDefaultUnits();
    return catalog;
}

}  // namespace

double UnitConverter::convert(const std::string& fromUnit, double value, const std::string& toUnit) {
    const LengthConversionEngine engine(catalogState());
    return engine.convert(fromUnit, value, toUnit);
}

std::vector<std::pair<std::string, double>> UnitConverter::convertAll(const std::string& fromUnit,
                                                                      double value) {
    const LengthConversionEngine engine(catalogState());
    return engine.convertAll(fromUnit, value);
}

void UnitConverter::registerUnit(const std::string& name, double ratioToMeter) {
    // ratioToMeter: meters represented by one unit of `name` (e.g. inch → 0.0254 m)
    catalogState().registerUnit(name, 1.0 / ratioToMeter);
}

}  // namespace entity
