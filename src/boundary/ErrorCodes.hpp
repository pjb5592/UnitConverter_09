#pragma once

namespace boundary {

// MESSAGE-LOCK: PRD / GM / parser contract strings (do not reword without PRD bump)
struct ErrorCodes {
    static constexpr const char* kUnknownUnitPrefix = "Unknown unit: ";
    static constexpr const char* kUnknownUnitParseMessage = "unknown unit";
};

}  // namespace boundary
