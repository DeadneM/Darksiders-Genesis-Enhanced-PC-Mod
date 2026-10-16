#pragma once
#include <atomic>
#include <cstdint>
namespace dg::gauges {
using LogFn=void(*)(const char*);
struct Telemetry {
    bool synergyReady=false;
    uint32_t synergyBoostCalls=0,synergyObserved=0;
};
void Initialize(LogFn logger,std::atomic<void*>* localPlayer,bool exactExecutable);
Telemetry GetTelemetry();
void Shutdown();
}
