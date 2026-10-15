#pragma once
#include <cstdint>
namespace dg::skip_warning {
using LogFn=void(*)(const char*);
struct Telemetry { bool ready=false;uint32_t observed=0,advanced=0,rejected=0; };
void Initialize(LogFn logger,bool exactExecutable);
bool Apply(bool enabled);
Telemetry GetTelemetry();
void Shutdown();
}
