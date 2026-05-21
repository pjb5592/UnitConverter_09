#pragma once

#include <iosfwd>

namespace boundary {

class CliApp {
public:
    int run(std::istream& in, std::ostream& out, std::ostream& err) const;
};

}  // namespace boundary
