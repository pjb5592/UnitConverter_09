#include "boundary/ErrorPresenter.hpp"

#include "boundary/ErrorCodes.hpp"

#include <iostream>

namespace boundary {

void ErrorPresenter::writeUnknownUnit(std::ostream& out, const std::string& unit) {
    out << ErrorCodes::kUnknownUnitPrefix << unit << '\n';
}

void ErrorPresenter::writeException(std::ostream& out, const std::exception& ex) {
    out << ex.what() << '\n';
}

}  // namespace boundary
