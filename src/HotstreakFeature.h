#pragma once
#include "RefillRequest.h"
namespace dg::hotstreak {
using LogFn=void(*)(const char*);
using AddJuice=void(*)(void*,float);
struct Telemetry {
    bool supported=false,available=false,pending=false;
    float current=0,maximum=0;
    refill::Result result=refill::Result::Idle;
    uint32_t modified=0,refills=0,topups=0;
};
void Initialize(LogFn,bool exactExecutable,AddJuice nativeAdd);
void OnLivePlayer(void*,uint32_t generation);
bool RequestRefill();
Telemetry GetTelemetry(unsigned index=0);
void Shutdown();
}
