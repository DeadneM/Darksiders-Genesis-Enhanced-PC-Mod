#pragma once

#include <windows.h>

namespace dg::skip_logos {

using LogFn = void(*)(const char*);

struct Telemetry {
    bool proxyAvailable = false;
    bool installed = false;
    bool enabled = true;
    LONG createFileCalls = 0;
    LONG mp4Calls = 0;
    LONG blocked = 0;
};

bool Initialize(LogFn logger);
bool Apply(bool enabled);
Telemetry GetTelemetry();
void Shutdown();

} // namespace dg::skip_logos
