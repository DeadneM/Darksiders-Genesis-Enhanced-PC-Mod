#pragma once
#include <atomic>
#include <cstdint>
#include "RefillRequest.h"
namespace dg::ammo {
using LogFn=void(*)(const char*);
struct Telemetry { bool ready=false;uint32_t observed=0,spends=0,modified=0,topups=0; };
struct Reserve {
    bool available=false,pending=false;
    uint32_t finiteReserves=0,refills=0;
    refill::Result result=refill::Result::Idle;
};
void Initialize(LogFn logger,bool exactExecutable);
void OnLivePlayer(void* actor,uint32_t generation);
Telemetry GetTelemetry();
bool StandardReady();
bool RequestRefill();
Reserve GetReserve(unsigned index=0);
void Shutdown();
}
