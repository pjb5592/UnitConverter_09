#include "boundary/CliReader.hpp"

#include <iostream>

namespace boundary {

bool CliReader::readConvertLine(std::istream& in, std::ostream& promptOut, std::string& line) {
    promptOut << kConvertPrompt;
    return static_cast<bool>(std::getline(in, line));
}

}  // namespace boundary
