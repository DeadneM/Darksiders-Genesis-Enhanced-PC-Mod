#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
namespace dg::recovery {
struct TailWindow {
    uintptr_t ability=0,instance=0;
    uint32_t generation=0;
    float lastElapsed=0,seconds=0;
};
inline bool TailReady(TailWindow& state,uintptr_t ability,uintptr_t instance,uint32_t generation,
    bool naturalBlend,float elapsed,float delta,float delayMs){
    if(!naturalBlend||!std::isfinite(elapsed)||elapsed<0||!std::isfinite(delta)||delta<0||delta>0.25f){
        state={};return false;
    }
    if(state.ability!=ability||state.instance!=instance||state.generation!=generation||elapsed<state.lastElapsed){
        state={ability,instance,generation,elapsed,0};
    }else if(elapsed>state.lastElapsed){
        state.seconds+=std::min(delta,elapsed-state.lastElapsed);
        state.lastElapsed=elapsed;
    }
    const float delay=std::isfinite(delayMs)?std::clamp(delayMs,0.0f,500.0f)*0.001f:0.5f;
    return state.seconds+0.000001f>=delay;
}
}
