#include "TpsStickRoles.h"
#include "TpsPadActions.h"
#include "CameraContinuity.h"
#include <cstdio>
#include <initializer_list>

struct Pad {
    unsigned short wButtons=0x4321;
    unsigned char bLeftTrigger=91,bRightTrigger=183;
    short sThumbLX=-32768,sThumbLY=12345,sThumbRX=32767,sThumbRY=-23456;
};
int main(){
    unsigned failures=0;
    for(bool cameraLeft:{false,true})for(bool nativeSwap:{false,true}){
        const Pad physical{};
        Pad game=physical;
        dg::tps_input::MapGameplayRoles(game,cameraLeft,nativeSwap);
        const int mx=dg::tps_input::CameraX(physical,!cameraLeft);
        const int my=dg::tps_input::CameraY(physical,!cameraLeft);
        const auto check=[&](bool condition,const char* label){
            if(!condition){++failures;std::printf("FAIL %s cameraLeft=%d nativeSwap=%d\n",label,cameraLeft,nativeSwap);}
        };
        check(dg::tps_input::CameraX(game,!nativeSwap)==mx&&
              dg::tps_input::CameraY(game,!nativeSwap)==my,"native movement consumes opposite physical stick");
        check(dg::tps_input::CameraX(game,nativeSwap)==dg::tps_input::CameraX(physical,cameraLeft)&&
              dg::tps_input::CameraY(game,nativeSwap)==dg::tps_input::CameraY(physical,cameraLeft),"native look consumes selected physical stick");
        for(bool fire:{false,true}){
            Pad aiming=game;
            dg::tps_input::SetNativeLook(aiming,nativeSwap,0,fire?32767:0);
            check(dg::tps_input::CameraX(aiming,!nativeSwap)==mx&&
                  dg::tps_input::CameraY(aiming,!nativeSwap)==my,"forward/filter preserves movement");
            check(dg::tps_input::CameraX(aiming,nativeSwap)==0&&
                  dg::tps_input::CameraY(aiming,nativeSwap)==(fire?32767:0),"native aim axis follows game checkbox");
            check(aiming.wButtons==physical.wButtons&&aiming.bLeftTrigger==physical.bLeftTrigger&&
                  aiming.bRightTrigger==physical.bRightTrigger,"buttons and triggers unchanged");
        }
    }
    const auto require=[&](bool condition,const char* label){if(!condition){++failures;std::printf("FAIL %s\n",label);}};
    using dg::camera_continuity::Identity;
    using dg::camera_continuity::CharacterSwap;
    const Identity strife{1,2,3,1},war{1,2,4,2};
    require(CharacterSwap(strife,war)&&CharacterSwap(war,strife),"actual retail Strife=1 / War=2 swaps retain camera basis in both directions");
    require(!CharacterSwap(strife,{1,2,4,1}),"same-type respawn still resets its camera");
    require(!CharacterSwap(strife,{1,7,4,2})&&!CharacterSwap(strife,{9,2,4,2}),"new world or another local player cannot borrow camera continuity");
    require(!CharacterSwap(strife,{1,2,4,0})&&!CharacterSwap(strife,{1,2,4,255}),"unset and unknown native types cannot masquerade as War");
    dg::camera_continuity::ReferenceTracker reference{};
    using Change=dg::camera_continuity::ReferenceChange;
    require(reference.Observe(&strife,1,true)==Change::Reset,"initial owner acquires its own camera basis");
    for(unsigned frame=0;frame<50;++frame)
        require(reference.Observe(nullptr,2,true)==Change::Unchanged,"old unowned blend camera cannot consume the replacement generation");
    require(reference.Observe(&war,2,true)==Change::KeepCharacterSwap,"new owned camera preserves the pre-blend Strife angle on War");
    require(reference.Observe(&strife,3,true)==Change::KeepCharacterSwap,"reverse native swap retains the angle too");
    const Identity respawn{1,2,5,1};
    require(reference.Observe(&respawn,4,true)==Change::Reset,"same-character respawn remains a new camera attachment");
    dg::tps_input::EquipmentHoldState first{},second{};
    Pad a{},b{};a.wButtons=0x4000;b.wButtons=0;a.bLeftTrigger=a.bRightTrigger=b.bLeftTrigger=b.bRightTrigger=0;
    auto event=dg::tps_input::ApplyEquipmentHold(a,0x4000,1000,first);
    require(event.down&&(a.wButtons&0x80)&&!(a.wButtons&0x4000),"configured equipment binding forwards a continuous native hold");
    dg::tps_input::ApplyEquipmentHold(b,0x4000,1000,second);
    require(!second.physicalDown&&!second.releaseStart&&!(b.wButtons&0x80),"P1 hold cannot arm P2 equipment");
    a.wButtons=0;event=dg::tps_input::ApplyEquipmentHold(a,0x4000,1100,first);
    require(event.up&&!(a.wButtons&0x80),"physical release starts a real UP gap");
    event=dg::tps_input::ApplyEquipmentHold(a,0x4000,1199,first);require(!event.stow&&!(a.wButtons&0x80),"stow cannot fire before its 100ms gap");
    event=dg::tps_input::ApplyEquipmentHold(a,0x4000,1200,first);require(event.stow&&(a.wButtons&0x80),"second native edge starts at the validated stow boundary");
    a.wButtons=0;event=dg::tps_input::ApplyEquipmentHold(a,0x4000,1399,first);require(!event.stow&&(a.wButtons&0x80),"stow pulse stays down for 200ms without duplicating its edge");
    a.wButtons=0;dg::tps_input::ApplyEquipmentHold(a,0x4000,1400,first);require(!(a.wButtons&0x80),"stow pulse releases at its exact endpoint");
    a.wButtons=0;a.bLeftTrigger=100;event=dg::tps_input::ApplyEquipmentHold(a,0x10000,1500,first);
    require(event.rebound&&event.down&&a.bLeftTrigger==0&&(a.wButtons&0x80),"trigger rebinding uses the same equipment action policy");
    a.wButtons=0x1234;a.bLeftTrigger=50;const auto untouched=a;dg::tps_input::ApplyEquipmentHold(a,0,1600,first);
    require(a.wButtons==untouched.wButtons&&a.bLeftTrigger==untouched.bLeftTrigger,"unbound equipment is exactly native");
    dg::tps_input::CameraPressState press{};Pad neutral{};neutral.wButtons=0;neutral.bLeftTrigger=neutral.bRightTrigger=0;
    Pad down=neutral;down.wButtons=0x20;
    auto action=dg::tps_input::CameraPress(press,&down,true,2000,0x20,500,0x20,true);
    require(!action.recenter&&!action.toggle,"shared recenter/toggle press does not recenter prematurely");
    action=dg::tps_input::CameraPress(press,&neutral,true,2100,0x20,500,0x20,true);
    require(action.recenter&&!action.toggle,"short press recenters only on release");
    dg::tps_input::CameraPress(press,&down,true,3000,0x20,500,0x20,true);
    action=dg::tps_input::CameraPress(press,&down,true,3500,0x20,500,0x20,true);require(action.toggle&&!action.recenter,"long press requests one TPS toggle at its configured delay");
    action=dg::tps_input::CameraPress(press,&down,true,3600,0x20,500,0x20,true);require(!action.toggle,"held toggle is not repeated");
    action=dg::tps_input::CameraPress(press,&neutral,true,3700,0x20,500,0x20,false);require(!action.recenter,"release after long press cannot move the camera");
    dg::tps_input::CameraPress(press,&down,true,4000,0x20,500,0x20,false);
    dg::tps_input::CameraPress(press,nullptr,false,4600,0x20,500,0x20,false);
    action=dg::tps_input::CameraPress(press,&down,true,4700,0x20,500,0x20,false);require(!action.toggle,"focus loss resets the hold timer");
    Pad mount=neutral;mount.wButtons=0x1000;require(!dg::tps_input::MapHorseChord(mount,0x1000,0x4000),"horse chord requires both configurable actions simultaneously");
    mount.wButtons|=0x4000;require(dg::tps_input::MapHorseChord(mount,0x1000,0x4000)&&(mount.wButtons&0x300)==0x300,"shared horse chord produces the native summon chord without an extra command");
    std::printf("TPS roles: %s (%u failures)\n",failures?"FAILED":"PASSED",failures);
    return failures?1:0;
}
