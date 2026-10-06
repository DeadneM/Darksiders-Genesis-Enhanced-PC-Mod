#pragma once

#include <cstdint>

namespace dg::startup_movies {

using LogFn = void(*)(const char*);

struct Telemetry {
    bool installed = false;
    bool enabled = false;
    std::uint32_t blockedAttributeChecks = 0;
    std::uint32_t blockedOpens = 0;
};

bool Install(bool enabled, LogFn logger);
void SetEnabled(bool enabled);
Telemetry GetTelemetry();

} // namespace dg::startup_movies
