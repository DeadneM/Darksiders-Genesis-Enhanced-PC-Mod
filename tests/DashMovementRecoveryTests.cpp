#include "DashRecoveryPolicy.h"
#include "ConfigStore.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace {
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void Run(){
    using dg::dash_combat::MoveRecovery;
    MoveRecovery r{};
    auto sample=[&](float time,float remaining,float speed2,bool enabled=true,bool held=true,bool allowed=false){
        return r.Sample(1,2,3,100+uint64_t(time*1000),time,remaining,.3f,speed2,enabled,held,allowed,0,0,0);};
    Check(!sample(.1f,.2f,10000,false)&&!r.seen,"OFF has no armed intervention");
    Check(!sample(.2f,-.1f,0)&&!sample(.22f,-.12f,0),"starting mid-tail cannot invent an observed dash countdown");
    r={};Check(!sample(.01f,.29f,10000)&&!sample(.25f,.05f,100),"moving dash stays native, even near its end");
    Check(!sample(.31f,-.01f,4),"expired timer alone cannot interrupt a still moving character");
    Check(!sample(.32f,-.02f,0),"one stationary sample is insufficient");
    Check(!sample(.33f,-.03f,0,true,false),"released native MOVE cannot be synthesized");
    Check(sample(.34f,-.04f,0),"observed countdown end, stationary character and held native MOVE permit one request");
    Check(!sample(.35f,-.05f,0)&&!sample(.4f,-.1f,0),"native rejection cannot cause repeated requests in this dash");
    Check(!sample(.01f,.29f,1000)&&!r.requested,"new activation on the same native object rearms without leaking the old request");
    sample(.2f,.1f,1000);
    Check(!sample(.32f,-.02f,0,true,true,true)&&!sample(.33f,-.03f,0,true,true,true),"already available native movement needs no intervention");
    r={};sample(.1f,.2f,1000);
    Check(!r.Sample(9,2,3,220,.12f,-.01f,.3f,0,true,true,false,0,0,0)&&!r.decelerated,"another local player cannot consume this dash");
    r={};sample(.1f,.2f,1000);
    Check(!r.Sample(1,9,3,220,.12f,-.01f,.3f,0,true,true,false,0,0,0)&&!r.decelerated,"character changes expire observed motion");
    r={};sample(.1f,.2f,1000);
    Check(!r.Sample(1,2,3,700,.6f,-.3f,.3f,0,true,true,false,0,0,0)&&!r.decelerated,"paused/stale samples cannot unlock an old dash");
    r={};sample(.1f,.2f,1000);
    Check(!r.Sample(1,2,3,420,.32f,-.02f,.3f,0,true,true,false,10,0,0)&&
          !r.Sample(1,2,3,430,.33f,-.03f,.3f,0,true,true,false,20,0,0),
        "root/animation displacement still blocks recovery when the movement velocity is already zero");
    for(float invalid:{std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::infinity()}){
        r={};Check(!sample(.1f,.2f,invalid)&&!r.seen,"invalid velocity cannot authorize a native request");
    }
    wchar_t dir[MAX_PATH]{},path[MAX_PATH]{};
    Check(GetTempPathW(MAX_PATH,dir)&&GetTempFileNameW(dir,L"DGM",0,path),"temporary INI");
    dg::config::Store a;a.SetPath(path);Check(a.Load()&&!a.dashMovementRecoveryEnabled,"missing settings preserve validated native dash timing");
    a.dashMovementRecoveryEnabled=true;a.hotkeys[10]=dg::config::Action::DashMovementRecovery;
    Check(a.SaveNow(),"new candidate setting saves");
    dg::config::Store b;b.SetPath(path);Check(b.Load()&&b.dashMovementRecoveryEnabled&&dg::runtime::Get().dashMovementRecoveryEnabled.load()&&b.hotkeys==a.hotkeys,
        "explicit experiment and hotkey survive reload and runtime publication");
    Check(int(dg::config::Action::ActionRecovery)==3&&int(dg::config::Action::InfiniteUltimateDuration)==53&&int(dg::config::Action::DashMovementRecovery)==54,
        "new action is appended; retired recovery and all validated hotkey IDs stay unchanged");
    b.ResetDefaults(false);Check(!b.dashMovementRecoveryEnabled&&!dg::runtime::Get().dashMovementRecoveryEnabled.load(),"global reset disables the new candidate");
    DeleteFileW(path);
}
}
int main(){try{Run();std::cout<<"Dash movement recovery boundaries and configuration passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
