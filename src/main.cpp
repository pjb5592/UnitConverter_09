#include <iostream>

#include "boundary/CliApp.hpp"

int main() {
    return boundary::CliApp().run(std::cin, std::cout, std::cerr);
}
