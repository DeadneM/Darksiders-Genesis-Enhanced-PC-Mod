#pragma once
#include <windows.h>
#include <array>
#include <atomic>
#include <cstdint>
#include <cstdio>

namespace dg::cost_probe {
// Sample one call in 64 on each thread. No UObject reads or engine calls.
// Inclusive CPU elapsed times: nested stages must never be added together.
enum Stage : unsigned { Gameplay, PistolObserve, PistolSample, StoneObserve, WrathScan, EffectScan, NativeAssist, Count };
inline constexpr const char* Names[Count]={"gameplay-service","pistol-observe","pistol-animation","stoneskin-observe","wrath-discovery","appearance-discovery","native-assist-inclusive"};
struct Counter {std::atomic_uint64_t samples{0},ticks{0},maximum{0};};
inline std::array<Counter,Count> Totals{};
inline int64_t Frequency(){static const int64_t hz=[](){LARGE_INTEGER f{};return QueryPerformanceFrequency(&f)?f.QuadPart:0;}();return hz;}
class Scope {
    Stage stage_;int64_t start_=0;
public:
    explicit Scope(Stage stage):stage_(stage){
        thread_local std::array<unsigned,Count> calls{};
        if((++calls[stage]&63u)!=1u||Frequency()<=0)return;
        LARGE_INTEGER now{};if(QueryPerformanceCounter(&now))start_=now.QuadPart;
    }
    ~Scope(){
        LARGE_INTEGER now{};if(!start_||!QueryPerformanceCounter(&now)||now.QuadPart<start_)return;
        const auto elapsed=uint64_t(now.QuadPart-start_);auto& c=Totals[stage_];
        c.ticks.fetch_add(elapsed,std::memory_order_relaxed);c.samples.fetch_add(1,std::memory_order_relaxed);
        auto before=c.maximum.load(std::memory_order_relaxed);
        while(before<elapsed&&!c.maximum.compare_exchange_weak(before,elapsed,std::memory_order_relaxed)){}
    }
    Scope(const Scope&)=delete;Scope& operator=(const Scope&)=delete;
};
// Game-thread periodic report and final shutdown report. Thread-local cadence
// avoids sharing mutable scheduling state with the shutdown thread.
inline void Report(void(*log)(const char*),bool force=false){
    static thread_local unsigned calls=0;static thread_local uint64_t next=0;
    if(!force&&(++calls&63u))return;
    const auto now=GetTickCount64();
    if(!log)return;
    if(!next&&!force){next=now+60000;return;}
    if(!force&&now<next)return;next=now+60000;
    const auto hz=Frequency();if(hz<=0)return;
    for(unsigned i=0;i<Count;++i){const auto& c=Totals[i];const auto n=c.samples.load(std::memory_order_relaxed);if(!n)continue;
        const double us=1000000.0/double(hz);char line[256]{};
        sprintf_s(line,"PERF V1.140 stage=%s samples=%llu avgUs=%.2f maxUs=%.2f sampling=1/64 inclusive=1 fpsMeasured=0",Names[i],static_cast<unsigned long long>(n),double(c.ticks.load(std::memory_order_relaxed))*us/double(n),double(c.maximum.load(std::memory_order_relaxed))*us);log(line);
    }
}
}
