#pragma once
#include <atomic>
#include <cstdint>
namespace dg::gauges {
using LogFn=void(*)(const char*);
struct Telemetry {
    bool wrathReady=false,chaosReady=false,synergyReady=false;
    uint32_t wrathBoostCalls=0,chaosBoostCalls=0,synergyBoostCalls=0;
    uint32_t wrathObserved=0,chaosObserved=0,synergyObserved=0;
};
void Initialize(LogFn logger,std::atomic<void*>* localPlayer,bool exactExecutable);
Telemetry GetTelemetry();
void Shutdown();
}
