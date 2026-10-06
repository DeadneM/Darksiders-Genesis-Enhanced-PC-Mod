#pragma once

namespace dg::skip_logos {

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
