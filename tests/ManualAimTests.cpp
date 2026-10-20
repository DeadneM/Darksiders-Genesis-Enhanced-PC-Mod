#include "../src/ManualAimFeature.cpp"
#include "ConfigStore.h"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
unsigned char actors[2][0x1100]{},controllers[2][0x400]{},items[72]{};
void* chunks[1]{items};
dg::local_players::Context owners[2]{};
bool gameThread=true,paused=false,cinema[2]{},radial[2]{},reject=false,reenter=false;
unsigned queries=0;uint64_t now=1;
struct Event {uintptr_t pawn;unsigned char action;};std::vector<Event> events;
template<class T> void Put(void* p,size_t offset,T value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
unsigned char Query(void* iface,unsigned char action){
    Check(action==31,"only the native AIM state can be queried");++queries;
    return *(static_cast<unsigned char*>(iface)-0x8D0+0xE10+31);
}
void Dispatch(void* iface,unsigned char action){
    const auto pawn=reinterpret_cast<uintptr_t>(iface)-0x8D0;
    events.push_back({pawn,action});
    if(!reject&&(action==31||action==32))*reinterpret_cast<unsigned char*>(pawn+0xE10+31)=action==31?1:0;
    if(reenter)dg::manual_aim::Service(owners[0],7,now);
}
void Input(unsigned i,bool held){dg::manual_aim::Publish(i,owners[i].character,7+i,owners[i].controllerId,held,now);}
void Tick(unsigned i=0){dg::manual_aim::Service(owners[i],7+i,now);}
bool Held(unsigned i=0){return actors[i][0xE10+31]!=0;}
void Fresh(){
    using namespace dg::manual_aim;
    gameThread=true;paused=false;reject=false;reenter=false;queries=0;events.clear();now=GetTickCount64();
    if(!now)now=1;
    for(unsigned i=0;i<2;++i){
        std::memset(actors[i],0,sizeof(actors[i]));std::memset(controllers[i],0,sizeof(controllers[i]));
        g_states[i]={};g_sources[i]={};cinema[i]=false;radial[i]=false;
        Put(actors[i],0xC,int(i+1));Put<unsigned char>(actors[i],0x118,3);
        Put(actors[i],0x348,reinterpret_cast<uintptr_t>(controllers[i]));
        Put(controllers[i],0x350,reinterpret_cast<uintptr_t>(actors[i]));
        Put(actors[i],0x8D0,g_base+0x2708780);
        Put(items,24*(i+1),reinterpret_cast<uintptr_t>(actors[i]));Put(items,24*(i+1)+16,int(33+i));
        auto& o=owners[i];o={};o.index=int(i);o.controllerId=int(i);o.localCount=2;
        o.localPlayer=0x1000+0x100*i;o.controller=reinterpret_cast<uintptr_t>(controllers[i]);
        o.character=o.pawn=reinterpret_cast<uintptr_t>(actors[i]);o.world=0x3000;
        Active(i,false);
    }
    g_ready=true;g_dispatch=&Dispatch;g_query=&Query;
}
void Run(){
    using namespace dg::manual_aim;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x3959680,chunks+0);Put(image.data(),0x3959694,3);
    Put(image.data(),0x2708780+0x10,g_base+0x681D50);Put(image.data(),0x2708780+0x18,g_base+0x669D30);
    Fresh();Input(0,false);Tick();Tick();
    Check(events.empty()&&!queries,"OFF never reads native AIM or dispatches anything");
    Input(0,true);Tick();Tick();
    Check(events.size()==1&&events[0].action==31&&Held()&&dg::runtime::Get().manualAimHeld[0].load(),"hold dispatches AIM once and exposes only a direction flag");
    Input(0,false);Check(!dg::runtime::Get().manualAimHeld[0].load(),"focus/input loss immediately disables the look override");Tick();Tick();
    Check(events.size()==2&&events.back().action==32&&!Held(),"release dispatches AIM_RELEASE once");
    Fresh();actors[0][0xE10+31]=1;Input(0,true);Tick();Input(0,false);Tick();
    Check(events.empty()&&Held(),"releasing the added source preserves an already-held native AIM source");
    HookDispatch(actors[0]+0x8D0,32);Check(!Held(),"the later native release remains functional");
    Fresh();Input(0,true);Tick();HookDispatch(actors[0]+0x8D0,31);Input(0,false);Tick();
    Check(Held()&&events.size()==2&&events.back().action==31,"a native press during added aim survives its release");
    HookDispatch(actors[0]+0x8D0,32);Check(!Held(),"native ownership ends on its own release");
    Fresh();Input(0,true);Tick();HookDispatch(actors[0]+0x8D0,31);HookDispatch(actors[0]+0x8D0,32);
    Check(Held()&&events.size()==2,"native release cannot cancel an independently-held added source");
    Input(0,false);Tick();Check(!Held()&&events.size()==3&&events.back().action==32,"last-source release leaves no stuck AIM");
    Fresh();Input(0,true);Tick();actors[0][0xE10+31]=0;Tick();Tick();
    Check(events.size()==1&&!g_states[0].owned,"native forced cancellation is never restarted each frame");
    Input(0,false);Tick();Input(0,true);Tick();Check(events.size()==2&&Held(),"release/repress rearms after native cancellation");
    Fresh();reject=true;Input(0,true);Tick();Tick();Check(events.size()==1&&!Held(),"native input gates can reject acquisition without a retry loop");
    reject=false;Input(0,false);Tick();Input(0,true);Tick();
    reject=true;Input(0,false);Tick();const auto releaseCount=events.size();Tick();
    Check(Held()&&g_states[0].pendingRelease&&events.size()==releaseCount,"rejected release remains pending with a bounded retry interval");
    now+=64;reject=false;Tick();Check(!Held()&&!g_states[0].pendingRelease,"pending release completes through the native gate when available");
    Fresh();Input(0,true);Tick();paused=true;Tick();
    Check(!g_states[0].owned&&g_states[0].pendingRelease&&!dg::runtime::Get().manualAimHeld[0].load(),"pause relinquishes own aim without native calls while UI owns input");
    paused=false;Tick();Check(!Held()&&events.size()==2,"resume releases own pending state without restarting a held chord");
    Tick();Check(events.size()==2,"no phantom hold after pause");
    for(unsigned mode=0;mode<2;++mode){
        Fresh();Input(0,true);if(mode==0)radial[0]=true;else cinema[0]=true;Tick();
        radial[0]=false;cinema[0]=false;Tick();Check(events.empty(),"wheel/cinematic entry consumes a held shortcut");
        Input(0,false);Tick();Input(0,true);Tick();Check(Held(),"release/repress works after UI returns");
    }
    Fresh();Input(0,true);Tick();now+=501;Tick();
    Check(!Held()&&!dg::runtime::Get().manualAimHeld[0].load(),"stale controller samples expire instead of holding AIM forever");
    Fresh();Input(0,true);gameThread=false;Tick();Check(events.empty()&&!queries,"render/input threads never call a native action");gameThread=true;
    Put(controllers[0],0x350,uintptr_t(0));Tick();Check(events.empty()&&!queries,"reciprocal controller ownership is mandatory");
    Fresh();Put(actors[0],0x8D0,g_base+0x2708788);Input(0,true);Tick();Check(events.empty()&&!queries,"unsupported input vtable fails open");
    Fresh();Input(0,true);Tick();const auto before=events.size();
    Put(items,24+16,99);Tick();Check(events.size()==before&&!g_states[0].owned,"weak serial replacement discards the old lease without calling it");
    Fresh();Input(0,true);Tick();owners[0].character+=16;Input(0,true);Tick();
    Check(events.size()==1&&!g_states[0].owned,"possession change never releases or restarts the old actor");
    Fresh();Input(0,true);Input(1,true);Tick();Tick(1);
    Check(Held(0)&&Held(1)&&events.size()==2,"both local players acquire independent native AIM states");
    Input(1,false);Tick(1);Check(Held(0)&&!Held(1)&&dg::runtime::Get().manualAimHeld[0].load(),"P2 release cannot affect P1");
    Input(0,false);Tick();Check(!Held(0),"P1 subsequently releases its own state");
    for(const auto& event:events)Check(event.action==31||event.action==32,"generated events never contain either fire action");
    Fresh();reenter=true;Input(0,true);Tick();Input(0,false);Tick();
    Check(events.size()==2&&!Held(),"nested native callbacks cannot recurse or repeat actions");reenter=false;
    Fresh();Input(0,true);Tick();Shutdown();Check(!Ready()&&!dg::runtime::Get().manualAimHeld[0].load(),"shutdown clears publication without an off-thread native call");
    wchar_t folder[MAX_PATH]{},path[MAX_PATH]{};
    Check(GetTempPathW(MAX_PATH,folder)&&GetTempFileNameW(folder,L"DGA",0,path),"temporary INI");
    dg::config::Store config;config.SetPath(path);Check(config.Load()&&!config.aimPadButtonA&&!config.aimPadButtonB,"old profiles default both added inputs to OFF");
    config.aimPadButtonA=0x0040;config.aimPadButtonB=0x0080;config.menuKey=VK_HOME;config.sheathKeyboardKey='K';
    config.hotkeys[2]=dg::config::Action::SheatheWeapons;WritePrivateProfileStringW(L"Personal",L"Keep",L"yes",path);
    Check(config.SaveNow(),"new bindings save");dg::config::Store loaded;loaded.SetPath(path);Check(loaded.Load(),"bindings reload");
    Check(loaded.aimPadButtonA==0x40&&loaded.aimPadButtonB==0x80&&loaded.menuKey==VK_HOME&&loaded.sheathKeyboardKey=='K'&&loaded.hotkeys==config.hotkeys,"two inputs persist without changing old bindings or Hotkey IDs");
    loaded.aimPadButtonA=0;loaded.aimPadButtonB=0x10000;Check(loaded.SaveNow()&&loaded.Load()&&!loaded.aimPadButtonA&&loaded.aimPadButtonB==0x10000,"single trigger binding survives round trip");
    wchar_t kept[8]{};GetPrivateProfileStringW(L"Personal",L"Keep",L"",kept,8,path);Check(std::wcscmp(kept,L"yes")==0,"unknown personal INI entries are retained");
    WritePrivateProfileStringW(L"Controls",L"AimWithoutFirePadButtonA",L"3",path);WritePrivateProfileStringW(L"Controls",L"AimWithoutFirePadButtonB",L"-1",path);
    Check(loaded.Load()&&!loaded.aimPadButtonA&&!loaded.aimPadButtonB,"invalid multi-bit and negative values fail to OFF");
    loaded.aimPadButtonA=0x80;loaded.ResetDefaults(false);Check(!loaded.aimPadButtonA&&!loaded.aimPadButtonB,"reset disables both new inputs");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
}
}
namespace dg::native_tps {
bool OnGameThread(){return gameThread;}bool IsWorldPaused(void*){return paused;}
bool CinematicActive(unsigned i){return i<2&&cinema[i];}bool RadialActive(unsigned i){return i<2&&radial[i];}
}
namespace dg::local_players {
bool Matches(unsigned i,uintptr_t character,uint32_t generation){return i<2&&character==owners[i].character&&generation==7+i;}
Slot PublishedSlot(unsigned i){if(i>1)return {};const auto& o=owners[i];return {o.localPlayer,o.character,o.controllerId,now,7+i};}
}
int main(){try{Run();std::cout<<"Native manual AIM lifecycle, source composition, player isolation and profile preservation passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
