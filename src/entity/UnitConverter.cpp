#include "entity/UnitConverter.hpp"

#include "entity/LengthConversionEngine.hpp"
#include "entity/UnitCatalog.hpp"

namespace entity {

namespace {

UnitCatalog& sharedCatalog() {
    static UnitCatalog catalog = UnitCatalog::withDefaultUnits();
    return catalog;
}

double convertWithCatalog(UnitCatalog& catalog, const std::string& fromUnit, double value,
                          const std::string& toUnit) {
    const LengthConversionEngine engine(catalog);
    return engine.convert(fromUnit, value, toUnit);
}

std::vector<std::pair<std::string, double>> convertAllWithCatalog(UnitCatalog& catalog,
                                                                 const std::string& fromUnit,
                                                                 double value) {
    const LengthConversionEngine engine(catalog);
    return engine.convertAll(fromUnit, value);
}

void registerUnitWithCatalog(UnitCatalog& catalog, const std::string& name,
                             double ratioToMeter) {
    // ratioToMeter: meters represented by one unit of `name` (e.g. inch → 0.0254 m)
    catalog.registerUnit(name, 1.0 / ratioToMeter);
}

}  // namespace

double UnitConverter::convert(const std::string& fromUnit, double value, const std::string& toUnit) {
    return convertWithCatalog(sharedCatalog(), fromUnit, value, toUnit);
}

std::vector<std::pair<std::string, double>> UnitConverter::convertAll(const std::string& fromUnit,
                                                                      double value) {
    return convertAllWithCatalog(sharedCatalog(), fromUnit, value);
}

void UnitConverter::registerUnit(const std::string& name, double ratioToMeter) {
    registerUnitWithCatalog(sharedCatalog(), name, ratioToMeter);
}

}  // namespace entity
