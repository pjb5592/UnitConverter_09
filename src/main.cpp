#include <iostream>

#include "boundary/InputParser.hpp"
#include "data/ConfigLoader.hpp"
#include "entity/DisplayRounder.hpp"
#include "entity/LengthConversionEngine.hpp"

int main() {
    try {
        const entity::UnitCatalog catalog =
            data::ConfigLoader::loadFromJson("config/units.json");
        const entity::LengthConversionEngine engine(catalog);

        std::cout << "Insert value for converting (ex: meter:2.5): ";
        std::string line;
        if (!std::getline(std::cin, line)) {
            return 1;
        }

        const boundary::ParsedInput parsed = boundary::InputParser::parse(line);
        if (!catalog.hasUnit(parsed.unit)) {
            std::cerr << "Unknown unit: " << parsed.unit << std::endl;
            return 1;
        }

        const double sourceDisplay = entity::roundOneDecimal(parsed.value);
        for (const auto& unit : catalog.units()) {
            const double targetDisplay =
                engine.convertDisplay(parsed.unit, parsed.value, unit.name);
            std::cout << sourceDisplay << ' ' << parsed.unit << " = " << targetDisplay << ' '
                      << unit.name << '\n';
        }
        return 0;
    } catch (const data::ConfigLoadError& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}
