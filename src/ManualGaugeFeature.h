#pragma once
#include "GaugeRequestPolicy.h"
#include <atomic>
#include <cstdint>

namespace dg::manual_gauges {
using LogFn=void(*)(const char*);
enum class Result { Idle, Queued, Applied, Partial, Rejected, Cancelled, Expired };
struct GaugeValue { float current=0,maximum=0; bool available=false; };
struct Telemetry {
    GaugeValue values[3]{};
    unsigned readyMask=0,lastAppliedMask=0;
    bool supported=false,pending=false,formActive=false;
    Result result=Result::Idle;
    uintptr_t actor=0;
    uint32_t generation=0,requests=0,applied=0;
    uint64_t observedAt=0;
};
void Initialize(LogFn logger,bool exactExecutable);
// Live validated player callback on the camera's sampled game thread only.
void OnLivePlayer(void* player,uint32_t generation,void* livePlayerState=nullptr);
// Existing native setters call this with their live argument. Default OFF is
// an exact pass-through; cached identities are only compared, never followed.
float UnlimitedRequest(unsigned gauge,void* liveHolder,float requested);
bool RequestSet(unsigned mask,float percent);
void CancelRequest();
Telemetry GetTelemetry(unsigned index=0);
void Shutdown();
}
