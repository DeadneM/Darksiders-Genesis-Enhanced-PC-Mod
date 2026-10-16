#pragma once
#include <atomic>
#include <cstdint>
namespace dg::potions {
using LogFn=void(*)(const char*);
enum class Result { Idle,Queued,Applied,Rejected,Cancelled };
struct Telemetry {
    bool supported=false,available=false,pending=false;
    int current=0,maximum=0;
    float overflow=0,nativeThreshold=0;
    Result result=Result::Idle;
    uint32_t refills=0,holds=0,rateCalls=0;
};
void Initialize(LogFn,std::atomic<void*>* localPlayer,bool exactExecutable);
void OnLivePlayer(void*,uint32_t generation);
bool RequestRefill();
void CancelRequest();
Telemetry GetTelemetry();
void Shutdown();
}
