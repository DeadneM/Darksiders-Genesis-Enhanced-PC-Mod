#include "ControllerReadScope.h"
#include "TpsStickRoles.h"
#include <cstdio>
#include <stdexcept>

namespace {
struct Pad {short sThumbLX=100,sThumbLY=200,sThumbRX=300,sThumbRY=400;};
unsigned transforms=0;
bool fail=false;
bool Provider(Pad& out){out=Pad{};return !fail;}
template<class Reader>bool GamePoll(Reader reader,Pad& out){
    dg::controller_input::GameRead scope;
    const bool ok=reader(out);
    if(ok&&scope.CanFilter()){
        dg::tps_input::MapGameplayRoles(out,true,false);
        dg::tps_input::SetNativeLook(out,false,0,32767);++transforms;
    }
    return ok;
}
bool ForwardingProvider(Pad& out){return GamePoll(Provider,out);}
void Check(bool ok,const char* label){if(!ok)throw std::runtime_error(label);}
void Run(){
    Pad pad{};
    Check(GamePoll(ForwardingProvider,pad)&&transforms==1,"forwarded XInput read transforms once, not twice");
    Check(pad.sThumbLX==300&&pad.sThumbLY==400&&pad.sThumbRX==0&&pad.sThumbRY==32767,"one role swap preserves movement and forward aim");
    {
        dg::controller_input::PhysicalRead raw;
        Check(GamePoll(ForwardingProvider,pad)&&transforms==1,"camera/shortcuts cannot receive any TPS transformation through nested exports");
        Check(pad.sThumbLX==100&&pad.sThumbRX==300&&pad.sThumbRY==400,"hardware camera sample contains actual sticks, never injected up");
        dg::controller_input::PhysicalRead second;
        Check(GamePoll(ForwardingProvider,pad)&&transforms==1,"nested raw scopes keep the outer bypass");
    }
    Check(GamePoll(ForwardingProvider,pad)&&transforms==2,"raw poll does not suppress the next game read");
    fail=true;Check(!GamePoll(ForwardingProvider,pad)&&transforms==2,"failed native read cannot mutate or consume game input");fail=false;
    Check(GamePoll(ForwardingProvider,pad)&&transforms==3,"failed read releases both nested scopes");
    try{dg::controller_input::PhysicalRead raw;throw 1;}catch(int){}
    Check(GamePoll(ForwardingProvider,pad)&&transforms==4,"exception cleanup cannot leave all sticks bypassed");
    Check(dg::controller_input::gameReadDepth==0&&dg::controller_input::physicalReadDepth==0,"scopes leave no persistent input state");
}
}
int main(){try{Run();std::puts("Nested controller API reads: raw camera data and single game transform passed");return 0;}catch(const std::exception& e){std::puts(e.what());return 1;}}
