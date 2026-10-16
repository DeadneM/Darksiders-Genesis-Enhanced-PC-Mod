#pragma once
#include <atomic>
#include <cstdint>
namespace dg::ammo {
using LogFn=void(*)(const char*);
struct Telemetry { bool ready=false;uint32_t observed=0,spends=0,modified=0,topups=0; };
void Initialize(LogFn logger,bool exactExecutable);
void OnLivePlayer(void* actor,uint32_t generation);
Telemetry GetTelemetry();
void Shutdown();
}
