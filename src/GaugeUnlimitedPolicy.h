#pragma once
#include "GaugeRequestPolicy.h"
namespace dg::manual_gauges {
struct UnlimitedContext {
    uintptr_t actor=0,holders[3]{};
    uint32_t generation=0;
    uint64_t observedAt=0;
    unsigned readyMask=0;
};
inline bool UnlimitedContextMatches(const UnlimitedContext& c,unsigned gauge,
    uintptr_t holder,uintptr_t localActor,uint32_t generation,uint64_t now){
    const unsigned i=gauge==Wrath?0:gauge==Ultimate?1:gauge==Synergy?2:3;
    return i<3&&holder&&c.actor&&c.actor==localActor&&c.generation==generation&&
        (c.readyMask&gauge)&&c.holders[i]==holder&&c.observedAt&&
        now>=c.observedAt&&now-c.observedAt<500;
}
inline float UnlimitedValue(bool enabled,float requested,float maximum){
    float full=0;
    return enabled&&std::isfinite(requested)&&TargetValue(maximum,100,full)?full:requested;
}
}
