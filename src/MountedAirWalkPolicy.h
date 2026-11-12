#pragma once
#include <cstdint>
namespace dg::mounted_air {
enum class Action {None,Support,Jump,Fall};
struct Policy {bool leap=false,blocked=false;uint64_t released=0;};
inline void Jumped(Policy& s){s={};s.leap=true;}
inline Action Decide(Policy& s,bool supported,unsigned char mode,float vertical,bool down,bool edge,uint64_t now,unsigned delay){
    if(supported&&mode==1){
        if(down){s.released=0;return edge?Action::Jump:Action::None;}
        if(!s.released)s.released=now;
        return now<s.released||now-s.released>=delay?Action::Fall:Action::None;
    }
    s.released=0;
    if(mode!=3){s={};return Action::None;}
    if(!down){s.leap=false;s.blocked=false;return Action::None;}
    return !s.blocked&&(edge||(s.leap&&vertical<=0))?Action::Support:Action::None;
}
}
