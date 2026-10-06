#pragma once

namespace dg::skip_logos {

// V0.18D: single UE4 StartupMovies config path. No runtime media fallback.

using LogFn = void(*)(const char*);

struct Telemetry {
    bool initialized = false;
    bool overridePresent = false;
    bool lastApplyOk = false;
    bool restartRequired = false;
};

bool Initialize(LogFn logger);
bool Apply(bool enabled);
Telemetry GetTelemetry();
void Shutdown();

} // namespace dg::skip_logos
