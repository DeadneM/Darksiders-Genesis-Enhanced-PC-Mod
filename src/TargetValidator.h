#pragma once
#include <cstdint>
#include <string>

namespace dg::target {

struct ValidationResult {
    bool exact = false;
    std::uint64_t fileSize = 0;
    std::string sha256;
    std::string reason;
};

ValidationResult ValidateCurrentExecutable();

} // namespace dg::target
