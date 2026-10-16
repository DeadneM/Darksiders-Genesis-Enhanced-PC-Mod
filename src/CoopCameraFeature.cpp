#include "CoopCameraFeature.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include "TpsStickRoles.h"
#include "TpsPadActions.h"
#include "CameraContinuity.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace dg::coop_camera {
namespace {
// P1 keeps the validated single-player implementation. P2 owns this separate
// value-only context; it cannot publish P1's mouse ray, orbit or aim telemetry.
struct State {
    camera_continuity::Identity identity{};
    uintptr_t local=0,character=0,camera=0,pressLocal=0;
    float yaw=0,pitch=0,baseYaw=0,arm=900,actorYaw=0,appliedYaw=0;
    bool baseValid=false,armValid=false,held=false,gameplay=false;
    bool paused=false,cinema=false,nativeForm=false;
    float heldView[7]{};
    ULONGLONG viewTick=0,inputTick=0,suspendStarted=0;
    bool focused=false,wheel=false,fire=false,stick=false,temporary=false;
    bool recenterPending=false,form=false,horseChordDown=false;
    int lastSuspend=-1;
    tps_input::CameraPressState cameraPress{};
    tps_input::EquipmentHoldState equipment{};
};
SRWLOCK g_lock=SRWLOCK_INIT;
State g_state;
LogFn g_log=nullptr;
unsigned g_ownerEvents=0;
bool Binding(int mask,const XINPUT_GAMEPAD& pad){
    if(!mask)return false;
    if(mask==0x10000)return pad.bLeftTrigger>=XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
    if(mask==0x20000)return pad.bRightTrigger>=XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
    return (pad.wButtons&static_cast<WORD>(mask))!=0;
}
bool Fire(const XINPUT_GAMEPAD& pad){
    const auto& rt=runtime::Get();
    return Binding(rt.tpsFirePrimaryMask.load(),pad)||Binding(rt.tpsFireSecondaryMask.load(),pad);
}
bool SameOwner(const State& state,const local_players::Slot& slot){
    return slot.localPlayer&&state.local==slot.localPlayer&&state.character==slot.character;
}
bool Active(const State& state,ULONGLONG now){
    return runtime::Get().thirdPersonEnabled.load()&&state.gameplay&&
        state.focused&&!state.temporary&&!state.wheel&&!state.nativeForm&&
        !(native_tps::CinematicActive(1)&&runtime::Get().tpsCutsceneAutoSuspend.load())&&
        state.viewTick&&now>=state.viewTick&&now-state.viewTick<300ull&&
        state.inputTick&&now>=state.inputTick&&now-state.inputTick<250ull;
}
void Unlock(){ReleaseSRWLockExclusive(&g_lock);}
}
void Initialize(LogFn log){g_log=log;}
void ApplyView(void* camera,void* outView,const local_players::Context& owner){
    if(owner.index!=1||!camera||!outView)return;
    const auto& rt=runtime::Get();
    local_players::Pose pose{};
    if(!local_players::ReadPose(owner,pose))return;
    const auto now=GetTickCount64();
    const bool paused=native_tps::IsWorldPaused(camera);
    const bool cinema=native_tps::CinematicActive(1)&&rt.tpsCutsceneAutoSuspend.load();
    auto* values=static_cast<unsigned char*>(outView);
    float native[7]{};std::memcpy(native,values,sizeof(native));
    for(float v:native)if(!std::isfinite(v))return;
    if(native[6]<1||native[6]>179)return;
    AcquireSRWLockExclusive(&g_lock);
    auto& s=g_state;
    unsigned char characterType=255;
    std::memcpy(&characterType,reinterpret_cast<unsigned char*>(owner.character)+0xC18,1);
    const camera_continuity::Identity identity{owner.localPlayer,owner.world,owner.character,characterType};
    const bool changed=s.local!=owner.localPlayer||s.character!=owner.character;
    if(changed){
        const bool swap=camera_continuity::CharacterSwap(s.identity,identity);
        const State previous=s;
        s=State{};s.local=owner.localPlayer;s.character=owner.character;
        if(swap){s.baseYaw=previous.baseYaw;s.baseValid=previous.baseValid;
            s.yaw=previous.yaw;s.pitch=previous.pitch;s.arm=previous.arm;s.armValid=previous.armValid;}
        if(g_log&&++g_ownerEvents<=40){
            char line[200]{};
            sprintf_s(line,"TPS V1.45 COOP_VIEW owner=P2 character=%p localPlayer=%p controllerId=%d; private orbit/hold/aim",
                reinterpret_cast<void*>(owner.character),reinterpret_cast<void*>(owner.localPlayer),owner.controllerId);
            g_log(line);
        }
    }
    s.identity=identity;
    s.actorYaw=pose.yaw;
    s.paused=paused;s.cinema=cinema;
    s.form=owner.kind==local_players::PoseKind::Ultimate;
    s.nativeForm=s.form&&characterType!=1;
    if(!rt.thirdPersonEnabled.load()||cinema||s.temporary){s.gameplay=false;Unlock();return;}
    if(paused){
        if(s.held&&s.camera==reinterpret_cast<uintptr_t>(camera))
            std::memcpy(values,s.heldView,sizeof(s.heldView));
        s.gameplay=false;Unlock();return;
    }
    if(!s.baseValid){s.baseYaw=std::remainder(pose.yaw,360.0f);s.baseValid=true;}
    if(s.recenterPending){s.yaw=std::remainder(pose.yaw-s.baseYaw,360.0f);s.pitch=0;s.recenterPending=false;}
    const bool horse=owner.kind==local_players::PoseKind::Horse;
    const bool ultimate=owner.kind==local_players::PoseKind::Ultimate;
    const float distanceMultiplier=ultimate?rt.tpsUltimateDistanceMultiplier.load():
        horse?rt.tpsHorseDistanceMultiplier.load():rt.thirdPersonDistanceMultiplier.load();
    const float height=ultimate?rt.tpsUltimateHeightOffset.load():
        horse?rt.tpsHorseHeightOffset.load():rt.thirdPersonHeightOffset.load();
    const float foot=rt.tpsFootAnchorOffset.load();
    const float basePitch=rt.thirdPersonPitchDegrees.load();
    const float orbitYaw=rt.cameraOrbitInputEnabled.load()?s.yaw:0;
    const float orbitPitch=rt.cameraOrbitInputEnabled.load()?s.pitch:0;
    if(!std::isfinite(distanceMultiplier)||distanceMultiplier<0||distanceMultiplier>3||
       !std::isfinite(height)||std::fabs(height)>1000||!std::isfinite(foot)||std::fabs(foot)>500||
       !std::isfinite(basePitch)){s.gameplay=false;Unlock();return;}
    constexpr float rad=0.01745329251994329577f;
    const float pitch=std::clamp(basePitch+orbitPitch,-75.0f,65.0f);
    const float yaw=std::remainder(s.baseYaw+orbitYaw,360.0f);
    const float distance=std::clamp(s.arm*distanceMultiplier,0.0f,12000.0f);
    float outgoing[7]={pose.x-std::cos(pitch*rad)*std::cos(yaw*rad)*distance,
        pose.y-std::cos(pitch*rad)*std::sin(yaw*rad)*distance,
        pose.z-foot-std::sin(pitch*rad)*distance+height+50.0f,pitch,yaw,native[5],native[6]};
    if(rt.thirdPersonFovEnabled.load()){
        const float fov=rt.thirdPersonCoopFovDegrees.load();
        if(std::isfinite(fov)&&fov>=40&&fov<=140)outgoing[6]=fov;
    }
    for(float v:outgoing)if(!std::isfinite(v)){s.gameplay=false;Unlock();return;}
    if(std::fabs(outgoing[0])>=1e7f||std::fabs(outgoing[1])>=1e7f||std::fabs(outgoing[2])>=1e7f){Unlock();return;}
    std::memcpy(values,outgoing,sizeof(outgoing));
    std::memcpy(s.heldView,outgoing,sizeof(outgoing));
    s.camera=reinterpret_cast<uintptr_t>(camera);s.held=true;
    s.appliedYaw=yaw;s.gameplay=true;s.viewTick=now;
    Unlock();
}
bool ObserveArm(void* arm,float nativeLength){
    // UActorComponent.Outer is the owning native actor; this is a live callback.
    if(!arm||!std::isfinite(nativeLength)||nativeLength<100||nativeLength>10000)return false;
    void* actor=nullptr;
    std::memcpy(&actor,static_cast<unsigned char*>(arm)+0x20,sizeof(actor));
    local_players::Context owner{};
    if(!local_players::ResolveActor(actor,owner)||owner.index!=1)return false;
    AcquireSRWLockExclusive(&g_lock);
    if(g_state.local==owner.localPlayer&&g_state.character==owner.character&&!g_state.armValid){
        g_state.arm=nativeLength;g_state.armValid=true;
    }
    Unlock();return true;
}
bool UpdateInput(const XINPUT_GAMEPAD* pad,bool focused,float dt,
                 int recenterMask,int suspendMask,int suspendDelayMs,int toggleMask,int toggleHoldMs){
    const auto slot=local_players::PublishedSlot(1);
    const auto now=GetTickCount64();const auto& rt=runtime::Get();
    AcquireSRWLockExclusive(&g_lock);auto& s=g_state;
    // A long press must also enable TPS before P2 has ever emitted a TPS
    // view. Only the published local identity is needed; no object reads.
    if(s.pressLocal!=slot.localPlayer){s.cameraPress={};s.pressLocal=slot.localPlayer;}
    const auto press=tps_input::CameraPress(s.cameraPress,pad,focused&&pad&&slot.localPlayer,now,
        toggleMask,toggleHoldMs,recenterMask,rt.thirdPersonEnabled.load());
    if(!SameOwner(s,slot)){s.focused=false;s.inputTick=0;s.equipment={};Unlock();return press.toggle;}
    s.focused=focused&&pad;s.inputTick=now;
    s.wheel=pad&&Binding(rt.tpsWheelPadMask.load(),*pad);
    s.fire=pad&&Fire(*pad);s.stick=false;
    if(!rt.thirdPersonEnabled.load()){
        s.yaw=0;s.pitch=0;s.baseValid=false;s.held=false;s.gameplay=false;
    }
    if(s.lastSuspend!=suspendMask){s.suspendStarted=0;s.lastSuspend=suspendMask;}
    const bool suspend=s.focused&&rt.thirdPersonEnabled.load()&&!s.wheel&&Binding(suspendMask,*pad);
    if(suspend&&!s.suspendStarted)s.suspendStarted=now;
    if(!suspend)s.suspendStarted=0;
    s.temporary=suspend&&now>=s.suspendStarted&&
        now-s.suspendStarted>=static_cast<ULONGLONG>(std::clamp(suspendDelayMs,0,3000));
    if(press.recenter)s.recenterPending=true;
    if(!s.focused||s.temporary||s.wheel||s.paused||s.cinema||!rt.thirdPersonEnabled.load()||
       !rt.cameraControllerOrbitEnabled.load()||rt.tpsCameraStickMode.load()==0||
       (rt.tpsMountedAimMode.load()==1&&s.fire)){Unlock();return press.toggle;}
    const bool left=rt.tpsCameraStickMode.load()==1;
    const float x=float(tps_input::CameraX(*pad,left))/32767.0f;
    const float y=float(tps_input::CameraY(*pad,left))/32767.0f;
    const float length=std::hypot(x,y);
    const float dead=float(left?XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE:XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)/32767.0f;
    s.stick=length>dead;
    const float speed=rt.cameraStickSpeed.load();
    if(s.stick&&std::isfinite(speed)&&std::isfinite(dt)&&dt>0&&dt<=0.1f){
        const float amount=std::min(1.0f,(length-dead)/(1.0f-dead));
        s.yaw=std::remainder(s.yaw+x/length*amount*speed*dt,360.0f);
        s.pitch=std::clamp(s.pitch+y/length*amount*speed*dt,-60.0f,60.0f);
    }
    Unlock();return press.toggle;
}
bool FilterPad(DWORD id,XINPUT_GAMEPAD& pad,bool focusedGameplay){
    const auto slot=local_players::PublishedSlot(1);
    if(!slot.localPlayer||slot.controllerId<0||DWORD(slot.controllerId)!=id)return false;
    const auto primary=local_players::PublishedSlot(0);
    if(primary.localPlayer&&primary.controllerId==slot.controllerId)return false;
    const auto& rt=runtime::Get();const auto now=GetTickCount64();
    AcquireSRWLockExclusive(&g_lock);
    auto& state=g_state;
    const bool owned=SameOwner(state,slot)&&focusedGameplay&&state.focused&&state.inputTick&&
        now>=state.inputTick&&now-state.inputTick<250;
    const bool active=owned&&Active(state,now);
    state.fire=Fire(pad); // physical firing state, before any equipment remap
    if(owned&&!state.paused&&!state.cinema&&!state.form){
        const bool horse=tps_input::MapHorseChord(pad,rt.horsePadButtonA.load(),rt.horsePadButtonB.load());
        if(horse&&!state.horseChordDown&&rt.horseEverywhereEnabled.load()){
            rt.horseSummonIntentTick.store(now,std::memory_order_release);
            rt.tpsLastHorseSummonInputTick.store(now,std::memory_order_release);
            if(g_log)g_log("TPS V1.47 P2 configured horse chord pre-armed; native summon only");
        }
        state.horseChordDown=horse;
    }else state.horseChordDown=false;
    // Preserve native wheel selection. Equipment hold/stow must run BEFORE
    // the temporary-view gate, just as on P1: that action can share its button.
    const bool wheel=Binding(rt.tpsWheelPadMask.load(),pad);
    if(!owned||!rt.thirdPersonEnabled.load()||state.paused||state.cinema||state.form)state.equipment={};
    else if(!wheel){
        const int mask=rt.tpsGrenadeButtonMask.load();
        const auto event=tps_input::ApplyEquipmentHold(pad,mask,now,state.equipment);
        static unsigned equipmentEvents=0;
        if(g_log&&(event.down||event.up||event.stow)&&++equipmentEvents<=24){
            char line[180]{};sprintf_s(line,"TPS V1.47 P2 EQUIPMENT action=%s physicalMask=0x%X privateHoldState=1",
                event.down?"HOLD":event.up?"RELEASE":"STOW",mask);g_log(line);
        }
    }
    Unlock();
    if(!active||wheel||!native_tps::NativeStickSwapKnown())return true;
    const int mode=rt.tpsCameraStickMode.load();
    const bool swap=native_tps::NativeStickSwap();
    if(mode!=0)tps_input::MapGameplayRoles(pad,mode==1,swap);
    const bool fire=Fire(pad),forward=rt.tpsMountedAimMode.load()==0;
    if(forward&&fire)tps_input::SetNativeLook(pad,swap,0,32767);
    else if(mode!=0&&(forward||!fire))tps_input::SetNativeLook(pad,swap,0,0);
    return true;
}
bool AdjustLook(void* actor,float& a,float& b,float& c,bool rotation){
    local_players::Context owner{};
    if(!local_players::ResolveActor(actor,owner)||owner.index!=1)return false;
    const auto& rt=runtime::Get();
    // War's ultimate melee controls remain native.
    if(owner.kind==local_players::PoseKind::Ultimate){
        unsigned char type=0;
        std::memcpy(&type,reinterpret_cast<unsigned char*>(owner.character)+0xC18,1);
        if(type!=1)return true;
    }
    AcquireSRWLockExclusive(&g_lock);const State s=g_state;Unlock();
    if(s.local!=owner.localPlayer||s.character!=owner.character||!Active(s,GetTickCount64()))return true;
    if(!std::isfinite(a)||!std::isfinite(b)||!std::isfinite(c))return true;
    if(rt.tpsMountedAimMode.load()==1&&s.fire)return true;
    if(s.fire){
        if(rotation){a=0;b=std::remainder(s.appliedYaw,360.0f);c=0;}
        else{constexpr float rad=0.01745329251994329577f;a=std::cos(s.appliedYaw*rad);b=std::sin(s.appliedYaw*rad);c=0;}
    }else if(rt.tpsCameraStickMode.load()!=0&&s.stick){
        a=0;b=rotation?std::remainder(s.actorYaw,360.0f):0;c=0;
    }
    return true;
}
bool ShotRotation(const local_players::Context& owner,float& pitch,float& yaw){
    const auto slot=local_players::PublishedSlot(1);
    AcquireSRWLockExclusive(&g_lock);const State s=g_state;Unlock();
    if(owner.index!=1||owner.character!=s.character||owner.localPlayer!=s.local||
       !SameOwner(s,slot)||!Active(s,GetTickCount64())||!s.fire||
       runtime::Get().tpsMountedAimMode.load()!=0)return false;
    pitch=s.heldView[3];yaw=s.appliedYaw;return std::isfinite(pitch)&&std::isfinite(yaw);
}
}
