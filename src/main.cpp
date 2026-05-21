#include <iostream>
#include <stdexcept>
#include <string>

#include "boundary/CliReader.hpp"
#include "boundary/ErrorCodes.hpp"
#include "boundary/ErrorPresenter.hpp"
#include "boundary/InputParser.hpp"
#include "control/ConversionUseCase.hpp"
#include "data/ConfigLoader.hpp"
#include "entity/UnitCatalogJson.hpp"

int main() {
    try {
        const entity::UnitCatalog catalog =
            data::ConfigLoader::loadFromJson(entity::resolveDefaultUnitsJsonPath());
        const control::ConversionUseCase useCase(catalog);

        std::string line;
        if (!boundary::CliReader::readConvertLine(std::cin, std::cout, line)) {
            return 1;
        }

        try {
            for (const std::string& outputLine : useCase.convertDisplayTable(line)) {
                std::cout << outputLine << '\n';
            }
            return 0;
        } catch (const std::invalid_argument& ex) {
            if (std::string(ex.what()) == boundary::ErrorCodes::kUnknownUnitParseMessage) {
                const boundary::ParsedInput parsed = boundary::InputParser::parse(line);
                boundary::ErrorPresenter::writeUnknownUnit(std::cerr, parsed.unit);
                return 1;
            }
            boundary::ErrorPresenter::writeException(std::cerr, ex);
            return 1;
        }
    } catch (const std::exception& ex) {
        boundary::ErrorPresenter::writeException(std::cerr, ex);
        return 1;
    }
}
