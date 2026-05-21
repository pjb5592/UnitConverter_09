#pragma once

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace golden_master {

inline constexpr const char* kPromptPrefix =
    "Insert value for converting (ex: meter:2.5): ";

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

inline std::string trimTrailingNewlines(std::string text) {
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
        text.pop_back();
    }
    return text;
}

inline std::string normalizeLineEndings(std::string text) {
    std::string normalized;
    normalized.reserve(text.size());
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\r') {
            if (i + 1 < text.size() && text[i + 1] == '\n') {
                normalized.push_back('\n');
                ++i;
            } else {
                normalized.push_back('\n');
            }
        } else {
            normalized.push_back(text[i]);
        }
    }
    return normalized;
}

inline std::string extractSection(const std::string& baselineDocument,
                                  const std::string& sectionHeader) {
    const std::string marker = '[' + sectionHeader + ']';
    const std::size_t start = baselineDocument.find(marker);
    if (start == std::string::npos) {
        return {};
    }

    std::size_t end = baselineDocument.find("\n---", start);
    if (end == std::string::npos) {
        end = baselineDocument.size();
    } else {
        ++end;  // skip leading '\n' of separator
    }

    return normalizeLineEndings(
        trimTrailingNewlines(baselineDocument.substr(start, end - start)));
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

inline std::string formatSection(const std::string& scenarioInput,
                                 const std::vector<std::string>& conversionLines) {
    std::ostringstream section;
    section << '[' << scenarioInput << "]\n";
    for (const std::string& line : conversionLines) {
        section << line << '\n';
    }
    return trimTrailingNewlines(section.str());
}

// stdout 리디렉션: UnitConverter < input.txt > actual.txt
inline bool captureStdoutToFile(const std::string& exePath,
                                const std::filesystem::path& inputPath,
                                const std::filesystem::path& actualPath) {
    std::string command = '"' + exePath + "\" < \"" + inputPath.string() + "\" > \"" +
                          actualPath.string() + "\" 2>&1";
#ifdef _WIN32
    command = "cmd /c " + command;
#endif
    return std::system(command.c_str()) == 0;
}

inline std::string captureScenarioSection(const std::string& exePath,
                                          const std::string& scenarioInput,
                                          const std::filesystem::path& workDir) {
    const std::filesystem::path inputPath = workDir / "input.txt";
    const std::filesystem::path actualPath = workDir / "actual.txt";

    if (!writeFile(inputPath, scenarioInput + "\n")) {
        return {};
    }
    if (!captureStdoutToFile(exePath, inputPath, actualPath)) {
        return {};
    }

    const std::string rawStdout = readFile(actualPath);
    return formatSection(scenarioInput, extractConversionLines(rawStdout));
}

inline std::string unifiedDiff(const std::string& expected, const std::string& actual) {
    const std::vector<std::string> expectedLines = splitLines(expected);
    const std::vector<std::string> actualLines = splitLines(actual);

    std::ostringstream out;
    out << "--- expected\n";
    out << "+++ actual\n";

    const std::size_t maxLines =
        expectedLines.size() > actualLines.size() ? expectedLines.size() : actualLines.size();
    if (maxLines > 0) {
        out << "@@ -1," << expectedLines.size() << " +1," << actualLines.size() << " @@\n";
    }

    for (std::size_t i = 0; i < maxLines; ++i) {
        const bool hasExp = i < expectedLines.size();
        const bool hasAct = i < actualLines.size();
        const std::string& exp = hasExp ? expectedLines[i] : std::string{};
        const std::string& act = hasAct ? actualLines[i] : std::string{};

        if (hasExp && hasAct && exp == act) {
            out << ' ' << exp << '\n';
        } else {
            if (hasExp) {
                out << '-' << exp << '\n';
            }
            if (hasAct) {
                out << '+' << act << '\n';
            }
        }
    }

    if (expectedLines.empty() && actualLines.empty() && expected != actual) {
        out << "@@ trailing whitespace or byte mismatch @@\n";
        out << "- (bytes=" << expected.size() << ")\n";
        out << "+ (bytes=" << actual.size() << ")\n";
    }

    return out.str();
}

inline void assertSectionEquals(const std::string& expected, const std::string& actual,
                                const std::string& testId) {
    const std::string expectedNorm = normalizeLineEndings(expected);
    const std::string actualNorm = normalizeLineEndings(actual);
    if (expectedNorm == actualNorm) {
        return;
    }
    std::cerr << "\n[" << testId << "] Golden Master mismatch\n";
    std::cerr << unifiedDiff(expectedNorm, actualNorm) << '\n';
}

}  // namespace golden_master
