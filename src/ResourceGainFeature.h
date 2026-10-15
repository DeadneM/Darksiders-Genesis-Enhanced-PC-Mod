#pragma once
#include <atomic>
#include <cstdint>
namespace dg::resource_gain {
using LogFn=void(*)(const char*);
struct Telemetry {bool ready[2]{};uint32_t observed[2]{},positive[2]{},boosted[2]{};};
void Initialize(LogFn logger,std::atomic<void*>* localPlayer,bool exactExecutable);
void OnLivePlayer(void* actor,uint32_t generation);
Telemetry GetTelemetry();
void Shutdown();
}
