#include "../src/UltimateDurationFeature.cpp"
#include "ConfigStore.h"
#include <iostream>
#include <stdexcept>
#include <limits>
namespace {
unsigned char actor[0x1100]{},component[0x120]{},resource[0x500]{};
bool local=true,paused=false,cinematic=false,unlimited=false;int player=0;unsigned calls=0;float received=0;
template<class T> void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
float NativeConsume(void* p,float amount){Check(p==component,"native this argument preserved");++calls;received=amount;return unlimited?100.0f:91.25f;}
float Use(float amount=4,uintptr_t caller=0x68A4EC){return dg::ultimate_duration::Consume(component,amount,caller);}
void Run(){
    using namespace dg::ultimate_duration;g_ready=true;g_original=NativeConsume;
    Put(component,0xE8,static_cast<void*>(actor));Put(actor,0xCC0,static_cast<void*>(component));Put(actor,0xE78,static_cast<void*>(actor));
    Put(actor,0xEA0,static_cast<void*>(resource));Put<unsigned char>(actor,0x118,3);Put<unsigned char>(resource,0x118,3);
    Put(resource,0x3A8,80.f);Put(resource,0x3AC,100.f);
    auto& rt=dg::runtime::Get();rt.ultimateDecreaseEnabled=false;rt.ultimateDurationMultiplier=2;
    Check(Use()==91.25f&&received==4,"OFF preserves native amount and float return");
    rt.ultimateDecreaseEnabled=true;Check(Use()==91.25f&&received==2,"x2 duration halves only the computed native drain");
    rt.ultimateDurationMultiplier=.5f;Use();Check(received==8,"subunit duration shortens the form");
    rt.ultimateDurationMultiplier=10;Use();Check(std::fabs(received-.4f)<1e-6f,"x10 duration retains native rate bonuses");
    rt.ultimateDurationMultiplier=0;Use();Check(received==80,"zero consumes the remaining gauge rather than becoming Unlimited");
    Use(0);Check(received==0,"zero native drain from perks or other conditions is preserved");
    rt.ultimateDurationMultiplier=std::numeric_limits<float>::min();Use();Check(received==80&&std::isfinite(received),"tiny manually entered factor cannot overflow consumption");
    rt.ultimateDurationMultiplier=1;Use();Check(received==4,"x1 is native");
    rt.ultimateDurationMultiplier=2;
    Use(4,0x64B8C7);Check(received==4,"other costs and activation cannot be scaled");
    Use(-4);Check(received==-4,"refunds and gains stay native");
    local=false;Use();Check(received==4,"foreign actors stay native");local=true;
    paused=true;Use();Check(received==4,"pause stays native");paused=false;
    cinematic=true;Use();Check(received==4,"cinematic stays native");cinematic=false;
    Put<void*>(actor,0xE78,nullptr);Use();Check(received==4,"inactive form cannot be affected");Put(actor,0xE78,static_cast<void*>(actor));
    Put<void*>(actor,0xCC0,nullptr);Use();Check(received==4,"reciprocal component ownership is required");Put(actor,0xCC0,static_cast<void*>(component));
    player=1;Use();Check(received==2,"P2 receives the same duration setting");
    unlimited=true;Check(Use()==100,"native Unlimited handling retains priority and its return value");unlimited=false;
    rt.ultimateDurationMultiplier=std::numeric_limits<float>::quiet_NaN();Use();Check(received==4,"nonfinite input fails open");
    Check(calls==18,"every native call, including excluded paths, runs exactly once");

    wchar_t folder[MAX_PATH]{},path[MAX_PATH]{};Check(GetTempPathW(MAX_PATH,folder)&&GetTempFileNameW(folder,L"DGU",0,path),"temporary INI");
    dg::config::Store config;config.SetPath(path);config.Load();Check(!config.ultimateDecreaseEnabled&&config.ultimateDurationMultiplier==2,"duration option is opt-in with x2 parameter default");
    config.ultimateDecreaseEnabled=true;config.ultimateDurationMultiplier=.5f;config.hotkeys[10]=dg::config::Action::UltimateDecreaseRate;
    Check(config.SaveNow(),"duration saved");dg::config::Store loaded;loaded.SetPath(path);
    Check(loaded.Load()&&loaded.ultimateDecreaseEnabled&&loaded.ultimateDurationMultiplier==.5f&&
          loaded.hotkeys==config.hotkeys&&rt.ultimateDurationMultiplier.load()==.5f,"subunit duration, runtime and appended hotkey round-trip");
    static_assert(int(dg::config::Action::InfiniteDoubleJump)==51&&int(dg::config::Action::UltimateDecreaseRate)==52);
    loaded.ultimateDurationMultiplier=0;Check(loaded.SaveNow()&&loaded.Load()&&loaded.ultimateDurationMultiplier==0,"zero survives profile load");
    loaded.ResetDefaults(false);Check(!loaded.ultimateDecreaseEnabled&&loaded.ultimateDurationMultiplier==2,"reset preserves opt-in default");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
}
}
namespace dg::native_tps {bool OnGameThread(){return true;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return cinematic;}}
namespace dg::local_players {bool ResolveActor(void* p,Context& c){if(!local||p!=actor)return false;c={};c.index=player;c.character=reinterpret_cast<uintptr_t>(actor);c.kind=PoseKind::Ultimate;return true;}}
int main(){try{Run();std::cout<<"Native Ultimate decay, float return, exclusions, Unlimited and profile passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
