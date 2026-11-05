#include "ControllerReadScope.h"
#include "GameplayMenuPolicy.h"
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
    dg::gameplay_menu::Focus editor{true,true,false,false,true};
    Check(dg::gameplay_menu::Runtime(editor)&&!dg::gameplay_menu::Commands(editor),"open editor keeps gameplay rules active but owns new mod commands");
    {
        dg::controller_input::PhysicalRead navigation;
        Check(GamePoll(ForwardingProvider,pad)&&transforms==4&&pad.sThumbRX==300,"editor navigation reads physical axes, never camera-forward injection");
    }
    Check(GamePoll(ForwardingProvider,pad)&&transforms==5&&pad.sThumbRY==32767,"next game read keeps the TPS rule even while editor remains open");
    Check(dg::gameplay_menu::Hotkeys(editor,false),"configured function keys remain available while editor is displayed");
    Check(!dg::gameplay_menu::Hotkeys(editor,true)&&dg::gameplay_menu::Runtime(editor),"text/rebinding capture blocks commands without disabling gameplay rules");
    auto orbit=dg::gameplay_menu::Orbit(editor,2,3,5,7);
    Check(orbit.yaw==2&&orbit.pitch==3,"open editor preserves camera joystick but discards editing mouse deltas");
    auto gameplay=editor;gameplay.menu=false;orbit=dg::gameplay_menu::Orbit(gameplay,2,3,5,7);
    Check(orbit.yaw==7&&orbit.pitch==10,"closed editor retains combined native mouse/controller orbit");
    gameplay.foreground=false;orbit=dg::gameplay_menu::Orbit(gameplay,2,3,5,7);
    Check(orbit.yaw==0&&orbit.pitch==0,"Alt-Tab cannot rotate either camera input");
    editor.menu=false;Check(dg::gameplay_menu::Runtime(editor)&&dg::gameplay_menu::Commands(editor),"closing editor re-enables bindings without toggling gameplay rules");
    editor.foreground=false;Check(!dg::gameplay_menu::Hotkeys(editor,false),"background function keys stay inactive");Check(!dg::gameplay_menu::Runtime(editor)&&!dg::gameplay_menu::Commands(editor),"Alt-Tab blocks runtime interaction and new commands");
    editor.foreground=true;editor.minimized=true;Check(!dg::gameplay_menu::Runtime(editor),"minimized window remains protected");
    editor.minimized=false;editor.closing=true;Check(!dg::gameplay_menu::Runtime(editor),"shutdown remains protected");
    editor.closing=false;editor.window=false;Check(!dg::gameplay_menu::Runtime(editor),"missing game window never qualifies as gameplay");
}
}
int main(){try{Run();std::puts("Nested controller API reads: raw camera data and single game transform passed");return 0;}catch(const std::exception& e){std::puts(e.what());return 1;}}
