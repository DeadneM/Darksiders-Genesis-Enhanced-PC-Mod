#include "TpsStickRoles.h"
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
    std::printf("TPS roles: %s (%u failures)\n",failures?"FAILED":"PASSED",failures);
    return failures?1:0;
}
