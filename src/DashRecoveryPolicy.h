#pragma once
#include <cmath>
#include <cstdint>

namespace dg::dash_combat {
// One request after an observed countdown crosses zero. Values only; none of
// these saved addresses are used for subsequent native object dereferences.
struct MoveRecovery {
    uintptr_t local=0,actor=0,ability=0;
    uint64_t seen=0;
    float elapsed=0;
    bool decelerated=false,requested=false;
    unsigned stopped=0;
    bool Sample(uintptr_t player,uintptr_t character,uintptr_t dash,uint64_t now,
                float time,float remaining,float duration,float speed2,
                bool enabled,bool moveHeld,bool moveAllowed){
        if(!enabled||!player||!character||!dash||!now||!std::isfinite(time)||time<0||
           !std::isfinite(remaining)||!std::isfinite(duration)||duration<=0||duration>5||
           remaining>duration||!std::isfinite(speed2)||speed2<0){*this={};return false;}
        const bool same=local==player&&actor==character&&ability==dash&&seen&&
            now>=seen&&now-seen<=250&&time>=elapsed;
        if(!same)*this={player,character,dash,now,time,false,false,0};
        seen=now;elapsed=time;
        if(remaining>0){decelerated=true;stopped=0;return false;}
        // Do not cancel a moving dash, arm on an already-ended timer, or keep
        // replaying a request that native code may have refused. Native MOVE
        // must actually be held, and an already available MOVE stays native.
        if(speed2>1.0f){stopped=0;return false;}
        if(stopped<2)++stopped;
        if(!decelerated||stopped<2||requested||!moveHeld||moveAllowed)return false;
        requested=true;return true;
    }
};
}
