#include <catch2/catch_test_macros.hpp>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include "golden_master/GoldenMasterHelper.hpp"

namespace {

std::filesystem::path repoRoot() {
    return std::filesystem::current_path();
}

std::filesystem::path expectedPath() {
    return repoRoot() / "tests" / "golden_master_expected.txt";
}

std::string converterExePath() {
#ifdef UNIT_CONVERTER_EXE_PATH
    return UNIT_CONVERTER_EXE_PATH;
#else
    const char* fromEnv = std::getenv("UNIT_CONVERTER_EXE");
    return fromEnv != nullptr ? std::string(fromEnv) : std::string{};
#endif
}

}  // namespace

// Approval / Golden Master: unit_converter stdout 회귀 LOCK
// - 기준 파일 없음 → 현재 캡처로 tests/golden_master_expected.txt 자동 생성(approve)
// - 기준 파일 있음 → actual vs expected 문자열 비교, 불일치 시 diff 후 FAIL
TEST_CASE("GoldenMaster_cli_stdout_matches_expected", "[golden_master][integration]") {
    const std::string exePath = converterExePath();
    INFO("UNIT_CONVERTER_EXE=" << exePath);
    REQUIRE_FALSE(exePath.empty());
    REQUIRE(std::filesystem::exists(exePath));

    const std::filesystem::path workDir = repoRoot();
    const std::string actual = golden_master::captureAllScenarios(exePath, workDir);
    REQUIRE_FALSE(actual.empty());

    const std::filesystem::path expectedFile = expectedPath();
    if (!std::filesystem::exists(expectedFile)) {
        // Approve: 최초 실행 시 현재 출력을 기준으로 저장
        REQUIRE(golden_master::writeFile(expectedFile, actual));
        WARN("Created baseline: " << expectedFile.string()
                                 << " — git add 후 커밋하세요.");
        return;
    }

    const std::string expected = golden_master::readFile(expectedFile);
    if (expected == actual) {
        return;
    }

    const std::string diff = golden_master::diffText(expected, actual);
    std::cerr << "\n=== Golden Master diff ===\n" << diff;
    std::cerr << "\n--- expected ---\n" << expected;
    std::cerr << "\n--- actual ---\n" << actual;
    FAIL_CHECK("Golden Master mismatch. "
               "기준 갱신: scripts/generate_golden_master.ps1 또는 .sh 실행 후 git add.");
}
