#pragma once

#include <windows.h>

namespace dg::skip_logos {

using LogFn = void(*)(const char*);

struct Telemetry {
    bool proxyAvailable = false;
    bool targetValid = false;
    bool installed = false;
    bool enabled = true;
    LONG setupCalls = 0;
    LONG skippedCalls = 0;
};

bool Initialize(LogFn logger);
bool Apply(bool enabled);
Telemetry GetTelemetry();
void Shutdown();

} // namespace dg::skip_logos
