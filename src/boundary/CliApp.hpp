#pragma once

#include <iosfwd>
#include <string>

namespace boundary {

class CliApp {
public:
    int run(std::istream& in, std::ostream& out, std::ostream& err) const;

    int run(std::istream& in, std::ostream& out, std::ostream& err,
            const std::string& configPath) const;
};

}  // namespace boundary
