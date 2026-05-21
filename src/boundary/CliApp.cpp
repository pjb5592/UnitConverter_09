#include "boundary/CliApp.hpp"

#include "boundary/CliReader.hpp"
#include "boundary/ErrorCodes.hpp"
#include "boundary/ErrorPresenter.hpp"
#include "boundary/InputParser.hpp"
#include "control/ConversionUseCase.hpp"
#include "data/ConfigLoader.hpp"
#include "entity/UnitCatalogJson.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace boundary {

int CliApp::run(std::istream& in, std::ostream& out, std::ostream& err) const {
    try {
        const entity::UnitCatalog catalog =
            data::ConfigLoader::loadFromJson(entity::resolveDefaultUnitsJsonPath());
        const control::ConversionUseCase useCase(catalog);

        std::string line;
        if (!CliReader::readConvertLine(in, out, line)) {
            return 1;
        }

        try {
            for (const std::string& outputLine : useCase.convertDisplayTable(line)) {
                out << outputLine << '\n';
            }
            return 0;
        } catch (const std::invalid_argument& ex) {
            if (std::string(ex.what()) == ErrorCodes::kUnknownUnitParseMessage) {
                const ParsedInput parsed = InputParser::parse(line);
                ErrorPresenter::writeUnknownUnit(err, parsed.unit);
                return 1;
            }
            ErrorPresenter::writeException(err, ex);
            return 1;
        }
    } catch (const std::exception& ex) {
        ErrorPresenter::writeException(err, ex);
        return 1;
    }
}

}  // namespace boundary
