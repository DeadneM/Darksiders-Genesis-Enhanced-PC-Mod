#pragma once
#include <cstdint>
namespace dg::health {
using LogFn=void(*)(const char*);
struct Telemetry {bool supported=false,available=false,pending=false;float current=0,maximum=0;unsigned refills=0;};
void Initialize(LogFn log,bool exactExecutable);
void OnLivePlayer(void* actor,uint32_t generation);
bool RequestRefill();
Telemetry GetTelemetry(unsigned index=0);
void Shutdown();
}
