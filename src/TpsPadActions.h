#pragma once
#include <algorithm>
#include <cstdint>

namespace dg::tps_input {
// Controller policies operate on the selected action binding and on a state
// supplied by its owner. P1 and P2 never share hold timers or release pulses.
template<class Pad> bool BindingDown(int mask,const Pad& pad){
    if(!mask)return false;
    if(mask==0x10000)return pad.bLeftTrigger>=30;
    if(mask==0x20000)return pad.bRightTrigger>=30;
    return (pad.wButtons&static_cast<unsigned short>(mask))!=0;
}
struct EquipmentHoldState {
    bool physicalDown=false;
    uint64_t releaseStart=0,releaseFinish=0;
    bool releasePulseDelivered=false;
    int selectedMask=0x80;
};
struct EquipmentEvents {bool rebound=false,down=false,up=false,stow=false;};
template<class Pad>
EquipmentEvents ApplyEquipmentHold(Pad& pad,int mask,uint64_t now,EquipmentHoldState& state){
    EquipmentEvents events{};
    if(!mask){state={};state.selectedMask=0;return events;}
    const bool physical=BindingDown(mask,pad);
    // The existing native equipment toggle consumes R3. Its configurable
    // source is preserved; this is the validated hold/stow policy, not a new
    // keyboard mapping or a synthetic action invocation.
    if(mask!=0x80)pad.wButtons&=static_cast<unsigned short>(~0x80);
    if(mask==0x10000)pad.bLeftTrigger=0;
    else if(mask==0x20000)pad.bRightTrigger=0;
    else pad.wButtons&=static_cast<unsigned short>(~static_cast<unsigned short>(mask));
    if(state.selectedMask!=mask){state={};state.selectedMask=mask;events.rebound=true;}
    if(physical!=state.physicalDown){
        state.physicalDown=physical;
        state.releasePulseDelivered=false;
        if(physical){state.releaseStart=state.releaseFinish=0;events.down=true;}
        else{state.releaseStart=now+100;state.releaseFinish=state.releaseStart+200;events.up=true;}
    }
    const bool pulse=!physical&&state.releaseStart&&now>=state.releaseStart&&now<state.releaseFinish;
    if(physical||pulse)pad.wButtons|=0x80;
    else pad.wButtons&=static_cast<unsigned short>(~0x80);
    if(pulse&&!state.releasePulseDelivered){state.releasePulseDelivered=true;events.stow=true;}
    return events;
}
struct CameraPressState {
    bool held=false,consumed=false,recenterDown=false;
    uint64_t started=0;
    int toggleMask=-1,recenterMask=-1;
};
struct CameraPressEvents {bool toggle=false,recenter=false;};
template<class Pad>
CameraPressEvents CameraPress(CameraPressState& s,const Pad* pad,bool focused,uint64_t now,
                              int toggleMask,int holdMs,int recenterMask,bool tpsEnabled){
    CameraPressEvents events{};
    if(s.toggleMask!=toggleMask||s.recenterMask!=recenterMask||!focused||!pad){
        s={};s.toggleMask=toggleMask;s.recenterMask=recenterMask;
        if(!focused||!pad)return events;
    }
    const bool down=BindingDown(toggleMask,*pad);
    if(down&&!s.held){s.held=true;s.consumed=false;s.started=now;}
    if(down&&s.held&&!s.consumed&&now>=s.started&&
       now-s.started>=static_cast<uint64_t>(std::clamp(holdMs,250,2000))){s.consumed=true;events.toggle=true;}
    if(!down&&s.held){
        if(!s.consumed&&tpsEnabled&&recenterMask==toggleMask)events.recenter=true;
        s.held=false;s.consumed=false;s.started=0;
    }
    const bool recenter=BindingDown(recenterMask,*pad);
    if(recenterMask!=toggleMask&&recenter&&!s.recenterDown&&tpsEnabled)events.recenter=true;
    s.recenterDown=recenter;
    return events;
}
template<class Pad> bool MapHorseChord(Pad& pad,int first,int second){
    const bool down=first&&second&&first!=second&&BindingDown(first,pad)&&BindingDown(second,pad);
    if(down)pad.wButtons|=0x100|0x200;
    return down;
}
}
