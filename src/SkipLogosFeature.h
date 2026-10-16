#pragma once

#include <windows.h>

namespace dg::skip_logos {

using LogFn = void(*)(const char*);

struct Telemetry {
    bool proxyAvailable = false;
    bool targetValid = false;
    bool patched = false;
    bool enabled = false;
};

bool Initialize(LogFn logger);
bool Apply(bool enabled);
Telemetry GetTelemetry();
void Shutdown();

} // namespace dg::skip_logos
