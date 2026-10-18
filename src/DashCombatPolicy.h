#pragma once
#include <cstdint>
#include <cmath>

namespace dg::dash_combat {
// Values only: the saved ability address is never dereferenced after a Tick.
struct AimResume {
    uintptr_t local=0,actor=0,ability=0;
    uint64_t seen=0;
    float elapsed=0;
    bool requested=false;

    void Observe(uintptr_t player,uintptr_t character,uintptr_t dash,float time,uint64_t now){
        if(!player||!character||!dash||!now||!std::isfinite(time)||time<0){*this={};return;}
        const bool same=local==player&&actor==character&&ability==dash&&seen&&
            now>=seen&&now-seen<=250&&time>=elapsed;
        if(!same)*this={player,character,dash,now,time,false};
        seen=now;elapsed=time;
    }
    bool Take(uintptr_t player,uintptr_t character,uint64_t now,
              bool gameplay,bool idle,bool aimHeld){
        if(!seen||local!=player||actor!=character||now<seen||now-seen>3000||!gameplay){
            *this={};return false;
        }
        if(requested||!idle||!aimHeld)return false;
        requested=true;return true;
    }
};

enum class Ammo { Unknown, Standard, Special };
inline bool SuppressForcedStandard(bool dashFire,bool forcedStandard,
                                   bool primaryHeld,Ammo primary,
                                   bool secondaryHeld,Ammo secondary){
    // A mixed/unknown request stays native. The two animation notifies are
    // generic pistol sockets, not a proven mapping to primary/secondary ammo.
    return dashFire&&forcedStandard&&(primaryHeld||secondaryHeld)&&
        (!primaryHeld||primary==Ammo::Special)&&(!secondaryHeld||secondary==Ammo::Special);
}
}
