#include <iostream>

#include "boundary/InputParser.hpp"
#include "control/ConversionUseCase.hpp"
#include "data/ConfigLoader.hpp"
#include "entity/UnitCatalogJson.hpp"

int main() {
    try {
        const entity::UnitCatalog catalog =
            data::ConfigLoader::loadFromJson(entity::resolveDefaultUnitsJsonPath());
        const control::ConversionUseCase useCase(catalog);

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

        for (const std::string& outputLine : useCase.convertDisplayTable(line)) {
            std::cout << outputLine << '\n';
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
