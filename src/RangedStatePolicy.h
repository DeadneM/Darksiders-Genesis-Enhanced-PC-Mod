#pragma once
namespace dg::ranged_state {
// Native AIM stays valid with held aim/fire, an active ability, or hard lock.
// A normal native exit clears AimMode to zero. Do not clear ranged-idle or
// add trigger-specific exceptions: request at most once per orphan episode.
struct Policy {
    bool requested=false;
    bool Recover(unsigned char mode,bool held,bool active,bool allowed){
        if(mode!=1||held||active){requested=false;return false;}
        if(!allowed||requested)return false;
        requested=true;return true;
    }
};
}
