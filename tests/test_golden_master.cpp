// Golden Master (Approval) — unit_converter CLI stdout 회귀
// Catch2 TEST_CASE_METHOD ≈ Google Test TEST_F(GoldenMasterTest, ...)

#include <catch2/catch_test_macros.hpp>

#include <cstdlib>
#include <filesystem>
#include <string>

#include "golden_master/GoldenMasterHelper.hpp"

class GoldenMasterTest {
public:
    GoldenMasterTest() {
        repoRoot_ = std::filesystem::current_path();
        baselinePath_ = repoRoot_ / "tests" / "golden_master_expected.txt";
        workDir_ = repoRoot_ / "tests" / "golden_master" / "work";
        std::filesystem::create_directories(workDir_);

        baselineDocument_ = golden_master::readFile(baselinePath_);
        REQUIRE_FALSE(baselineDocument_.empty());

        exePath_ = converterExePath();
        INFO("UNIT_CONVERTER_EXE=" << exePath_);
        REQUIRE_FALSE(exePath_.empty());
        REQUIRE(std::filesystem::exists(exePath_));
    }

    static std::string workSubdirName(const std::string& scenarioInput) {
        std::string name = scenarioInput;
        for (char& ch : name) {
            if (ch == ':') {
                ch = '_';
            }
        }
        return name;
    }

    void runScenario(const char* testId, const std::string& scenarioInput) {
        const std::string expected =
            golden_master::extractSection(baselineDocument_, scenarioInput);
        REQUIRE_FALSE(expected.empty());

        const std::filesystem::path scenarioWork = workDir_ / workSubdirName(scenarioInput);
        std::filesystem::create_directories(scenarioWork);

        const std::string actual = golden_master::captureScenarioSection(
            exePath_.string(), scenarioInput, scenarioWork);
        REQUIRE_FALSE(actual.empty());

        if (expected != actual) {
            golden_master::assertSectionEquals(expected, actual, testId);
        }
        REQUIRE(expected == actual);
    }

private:
    static std::string converterExePath() {
#ifdef UNIT_CONVERTER_EXE_PATH
        return UNIT_CONVERTER_EXE_PATH;
#else
        const char* fromEnv = std::getenv("UNIT_CONVERTER_EXE");
        return fromEnv != nullptr ? std::string(fromEnv) : std::string{};
#endif
    }

    std::filesystem::path repoRoot_;
    std::filesystem::path baselinePath_;
    std::filesystem::path workDir_;
    std::filesystem::path exePath_;
    std::string baselineDocument_;
};

// GM-TC-01: "meter:2.5" == 기준 [meter:2.5] 섹션
TEST_CASE_METHOD(GoldenMasterTest, "UnitConverter_meter_2_5", "[GoldenMaster][GM-TC-01]") {
    runScenario("GM-TC-01", "meter:2.5");
}

// GM-TC-02: "feet:1.0" == 기준 [feet:1.0] 섹션
TEST_CASE_METHOD(GoldenMasterTest, "UnitConverter_feet_1_0", "[GoldenMaster][GM-TC-02]") {
    runScenario("GM-TC-02", "feet:1.0");
}

// GM-TC-03: "yard:1.0" == 기준 [yard:1.0] 섹션
TEST_CASE_METHOD(GoldenMasterTest, "UnitConverter_yard_1_0", "[GoldenMaster][GM-TC-03]") {
    runScenario("GM-TC-03", "yard:1.0");
}

// GM-TC-04: "meter:0.0" == 기준 [meter:0.0] 섹션
TEST_CASE_METHOD(GoldenMasterTest, "UnitConverter_meter_0_0", "[GoldenMaster][GM-TC-04]") {
    runScenario("GM-TC-04", "meter:0.0");
}
