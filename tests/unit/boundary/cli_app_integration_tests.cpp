#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "boundary/CliApp.hpp"

namespace fs = std::filesystem;

namespace {

std::string projectRoot() {
    const fs::path here = fs::current_path();
    if (fs::exists(here / "config" / "units.json")) {
        return here.string();
    }
    if (fs::exists(here / ".." / "config" / "units.json")) {
        return (here / "..").string();
    }
    if (fs::exists(here / ".." / ".." / "config" / "units.json")) {
        return (here / ".." / "..").string();
    }
    return here.string();
}

std::string configPath(const std::string& name) {
    return (fs::path(projectRoot()) / "config" / name).string();
}

}  // namespace

// T-02 — CliApp path E2E (same stack as main): meter:2.5 → 1-decimal display lines
TEST_CASE("T-02_cli_app_meter_25_prints_display_table_lines", "[cli][integration][T-02]") {
    std::istringstream in("meter:2.5\n");
    std::ostringstream out;
    std::ostringstream err;

    const int exitCode = boundary::CliApp().run(in, out, err, configPath("units.json"));

    REQUIRE(exitCode == 0);
    REQUIRE(out.str().find("2.5 meter = 8.2 feet") != std::string::npos);
    REQUIRE(err.str().empty());
}

// T-05 — unknown unit: parsec:1.0 → exit 1, MESSAGE-LOCK stderr, no conversion stdout
TEST_CASE("T-05_cli_unknown_parsec_exits_with_no_conversion_output", "[cli][integration][T-05]") {
    std::istringstream in("parsec:1.0\n");
    std::ostringstream out;
    std::ostringstream err;

    const int exitCode = boundary::CliApp().run(in, out, err, configPath("units.json"));

    REQUIRE(exitCode == 1);
    REQUIRE(err.str().find("Unknown unit: parsec") != std::string::npos);
    REQUIRE(out.str().find(" = ") == std::string::npos);
}

// T-06 — missing/invalid config → exit 1 before conversion
TEST_CASE("T-06_cli_missing_config_exits_nonzero", "[cli][integration][T-06]") {
    std::istringstream in("meter:2.5\n");
    std::ostringstream out;
    std::ostringstream err;

    const int exitCode =
        boundary::CliApp().run(in, out, err, configPath("does_not_exist_cli_test.json"));

    REQUIRE(exitCode == 1);
    REQUIRE_FALSE(err.str().empty());
}

TEST_CASE("T-06_cli_invalid_config_exits_nonzero", "[cli][integration][T-06]") {
    const fs::path temp = fs::temp_directory_path() / "units_invalid_cli_test.json";
    {
        std::ofstream file(temp);
        REQUIRE(file.is_open());
        file << "{ not valid json";
    }

    std::istringstream in("meter:2.5\n");
    std::ostringstream out;
    std::ostringstream err;

    const int exitCode = boundary::CliApp().run(in, out, err, temp.string());

    REQUIRE(exitCode == 1);
    REQUIRE_FALSE(err.str().empty());
    fs::remove(temp);
}

// T-07 — stdin EOF on first read → exit 1
TEST_CASE("T-07_cli_eof_on_read_exits_nonzero", "[cli][integration][T-07]") {
    std::istringstream in;
    std::ostringstream out;
    std::ostringstream err;

    const int exitCode = boundary::CliApp().run(in, out, err, configPath("units.json"));

    REQUIRE(exitCode == 1);
}
