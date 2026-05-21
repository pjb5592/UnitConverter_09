#pragma once

#include <exception>
#include <iosfwd>
#include <string>

namespace boundary {

class ErrorPresenter {
public:
    static void writeUnknownUnit(std::ostream& out, const std::string& unit);
    static void writeException(std::ostream& out, const std::exception& ex);
};

}  // namespace boundary
