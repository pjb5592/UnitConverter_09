#pragma once

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace golden_master {

inline constexpr const char* kPromptPrefix =
    "Insert value for converting (ex: meter:2.5): ";

inline const std::vector<std::string>& scenarios() {
    static const std::vector<std::string> items = {"meter:2.5", "feet:1.0", "yard:1.0",
                                                   "meter:0.0"};
    return items;
}

inline std::string readFile(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return {};
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

inline bool writeFile(const std::filesystem::path& path, const std::string& content) {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        return false;
    }
    out << content;
    return static_cast<bool>(out);
}

inline std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    return lines;
}

inline std::vector<std::string> extractConversionLines(const std::string& rawStdout) {
    std::vector<std::string> conversionLines;
    for (const std::string& line : splitLines(rawStdout)) {
        const std::size_t promptPos = line.find(kPromptPrefix);
        if (promptPos != std::string::npos) {
            const std::string remainder = line.substr(promptPos + std::strlen(kPromptPrefix));
            if (!remainder.empty()) {
                conversionLines.push_back(remainder);
            }
            continue;
        }
        if (line.find(" = ") != std::string::npos) {
            conversionLines.push_back(line);
        }
    }
    return conversionLines;
}

inline std::string formatScenarioBlock(const std::string& scenarioInput,
                                       const std::vector<std::string>& conversionLines) {
    std::ostringstream block;
    block << '[' << scenarioInput << "]\n";
    for (const std::string& line : conversionLines) {
        block << line << '\n';
    }
    return block.str();
}

inline std::string buildGoldenDocument(
    const std::vector<std::pair<std::string, std::vector<std::string>>>& blocks) {
    std::ostringstream document;
    for (std::size_t i = 0; i < blocks.size(); ++i) {
        document << formatScenarioBlock(blocks[i].first, blocks[i].second);
        if (i + 1 < blocks.size()) {
            document << "---\n";
        }
    }
    return document.str();
}

inline std::string runConverterCapture(const std::string& exePath,
                                       const std::string& scenarioInput,
                                       const std::filesystem::path& workDir) {
    const std::filesystem::path inputPath =
        workDir / ("golden_input_" + scenarioInput + ".txt");
    const std::filesystem::path outputPath =
        workDir / ("golden_output_" + scenarioInput + ".txt");

    if (!writeFile(inputPath, scenarioInput + "\n")) {
        return {};
    }

    std::string command = '"' + exePath + "\" < \"" + inputPath.string() + "\" > \"" +
                          outputPath.string() + "\" 2>&1";
#ifdef _WIN32
    command = "cmd /c " + command;
#endif
    const int exitCode = std::system(command.c_str());
    if (exitCode != 0) {
        return {};
    }
    return readFile(outputPath);
}

inline std::string captureAllScenarios(const std::string& exePath,
                                       const std::filesystem::path& workDir) {
    std::vector<std::pair<std::string, std::vector<std::string>>> blocks;
    blocks.reserve(scenarios().size());
    for (const std::string& scenario : scenarios()) {
        const std::string rawStdout = runConverterCapture(exePath, scenario, workDir);
        blocks.emplace_back(scenario, extractConversionLines(rawStdout));
    }
    return buildGoldenDocument(blocks);
}

inline std::string diffText(const std::string& expected, const std::string& actual) {
    const std::vector<std::string> expectedLines = splitLines(expected);
    const std::vector<std::string> actualLines = splitLines(actual);
    const std::size_t maxLines =
        expectedLines.size() > actualLines.size() ? expectedLines.size() : actualLines.size();

    std::ostringstream diff;
    bool hasDiff = false;
    for (std::size_t i = 0; i < maxLines; ++i) {
        const std::string exp = i < expectedLines.size() ? expectedLines[i] : "<missing>";
        const std::string act = i < actualLines.size() ? actualLines[i] : "<missing>";
        if (exp != act) {
            hasDiff = true;
            diff << "L" << (i + 1) << " expected: " << exp << '\n';
            diff << "L" << (i + 1) << "   actual: " << act << '\n';
        }
    }
    if (!hasDiff && expected != actual) {
        diff << "Trailing newline or whitespace mismatch.\n";
        diff << "expected bytes: " << expected.size() << ", actual bytes: " << actual.size()
             << '\n';
    }
    return diff.str();
}

}  // namespace golden_master
