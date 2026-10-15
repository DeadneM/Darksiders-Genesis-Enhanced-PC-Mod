#pragma once
#include <atomic>
#include <cstdint>
namespace dg::recovery {
using LogFn=void(*)(const char*);
struct Telemetry {bool ready=false;uint32_t ticks=0,local=0,tails=0,completed=0;};
bool Initialize(LogFn logger,std::atomic<void*>* localPlayer,bool exactExecutable);
Telemetry GetTelemetry();
void Shutdown();
}
