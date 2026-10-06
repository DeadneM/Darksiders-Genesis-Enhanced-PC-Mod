#pragma once

#include <windows.h>

namespace dg::skip_logos {

using LogFn = void(*)(const char*);

struct Telemetry {
    bool installed = false;
    bool resolverMethodHooked = false;
    LONG resolverCreateCalls = 0;
    LONG urlCalls = 0;
    LONG blocked = 0;
};

bool Initialize(LogFn logger);
Telemetry GetTelemetry();
void Shutdown();

} // namespace dg::skip_logos
