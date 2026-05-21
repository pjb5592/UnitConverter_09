#pragma once

#include <iosfwd>
#include <string>

namespace boundary {

class CliReader {
public:
    static constexpr const char* kConvertPrompt =
        "Insert value for converting (ex: meter:2.5): ";

    static bool readConvertLine(std::istream& in, std::ostream& promptOut, std::string& line);
};

}  // namespace boundary
