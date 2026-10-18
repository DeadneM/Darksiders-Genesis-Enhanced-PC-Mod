#include "../src/InteractionHudFeature.cpp"
#include "ConfigStore.h"
#include <array>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
namespace {
struct Player {unsigned char actor[0x1100]{},hud[0x700]{},widget[0x40]{};};
Player players[2];dg::local_players::Context owners[2]{};
unsigned char items[3*24]{};void* chunks[1]{items};
bool gameThread=true;unsigned facingCalls=0,ownerCalls=0,scaleCalls=0;void* facingOwner[2]{};
template<class T> void Put(void* p,size_t at,T v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
template<class T> T Get(void* p,size_t at){T v{};std::memcpy(&v,static_cast<unsigned char*>(p)+at,sizeof(v));return v;}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
int Index(void* p){return p==players[1].hud?1:0;}
void Facing(void* p,float dt){Check(dt==.016f,"original dt retained");++facingCalls;facingOwner[Index(p)]=Get<void*>(p,0x5E0);}
void Owner(void* p,void* local){++ownerCalls;Put(p,0x5E0,local);}
void SetScale(void* p,const dg::interaction_hud::Scale* value){++scaleCalls;Put(p,0x17C,*value);}
void Fresh(){
    using namespace dg::interaction_hud;
    for(auto& p:players)p={};g_baselines={};
    gameThread=true;facingCalls=ownerCalls=scaleCalls=0;facingOwner[0]=facingOwner[1]=nullptr;
    g_ready=true;g_facing=Facing;g_owner=Owner;g_scale=SetScale;
    dg::runtime::Get().interactionHudScaleEnabled=true;dg::runtime::Get().interactionHudScale=.5f;
    for(int i=0;i<2;++i){
        auto& p=players[i];owners[i]={};owners[i].index=i;owners[i].localPlayer=0x1000+uintptr_t(i);
        owners[i].character=reinterpret_cast<uintptr_t>(p.actor);
        Put(p.actor,0xEF8,static_cast<void*>(p.hud));Put(p.hud,0,g_base+0x275B7D0);
        Put(p.hud,0xE8,static_cast<void*>(p.actor));Put(p.hud,0x610,static_cast<void*>(p.widget));
        Put(p.hud,0x17C,dg::interaction_hud::Scale{});Put(p.hud,0xC,i+1);
        Put(items,size_t(i+1)*24,reinterpret_cast<uintptr_t>(p.hud));Put(items,size_t(i+1)*24+16,17+i);
    }
}
void Tick(int i){dg::interaction_hud::HookFacing(players[i].hud,.016f);}
void Run(){
    std::vector<unsigned char> image(0x3960000);dg::interaction_hud::g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x3959680,chunks+0);Put(image.data(),0x3959694,3);
    Fresh();Tick(0);Tick(1);
    Check(facingCalls==2&&ownerCalls==2&&facingOwner[0]==reinterpret_cast<void*>(0x1000)&&
        facingOwner[1]==reinterpret_cast<void*>(0x1001),"each native facing call sees the reciprocal local owner, including the null-owner fallback bug");
    Check(Get<dg::interaction_hud::Scale>(players[0].hud,0x17C).x==.5f&&
        Get<dg::interaction_hud::Scale>(players[1].hud,0x17C).x==.5f,"P1/P2 prompts receive their own baseline scale");
    for(int i=0;i<30;++i){Tick(0);Tick(1);}
    Check(ownerCalls==2&&scaleCalls==2,"unchanged owner/scale produce no repeated native setter calls or compounding");
    owners[0].index=1;owners[0].localPlayer=0x1001;Tick(0);
    Check(Get<dg::interaction_hud::Scale>(players[0].hud,0x17C).x==.5f&&scaleCalls==2,
        "controller reassignment cannot compound an already scaled live component");
    dg::runtime::Get().interactionHudScaleEnabled=false;Tick(0);Tick(1);
    Check(Get<dg::interaction_hud::Scale>(players[0].hud,0x17C).x==1&&scaleCalls==4,"OFF restores native scale while the P2 owner fix remains");
    Fresh();Tick(0);Put(players[0].hud,0x17C,dg::interaction_hud::Scale{2,3,4});Tick(0);
    auto size=Get<dg::interaction_hud::Scale>(players[0].hud,0x17C);
    Check(size.x==1&&size.y==1.5f&&size.z==2,"native animation changes establish a fresh baseline without compounding");
    dg::runtime::Get().interactionHudScaleEnabled=false;Tick(0);
    size=Get<dg::interaction_hud::Scale>(players[0].hud,0x17C);Check(size.x==2&&size.y==3&&size.z==4,"OFF restores the updated native baseline");
    Fresh();Tick(0);Put(players[0].hud,0x17C,dg::interaction_hud::Scale{});Put(items,24+16,99);Tick(0);
    Check(Get<dg::interaction_hud::Scale>(players[0].hud,0x17C).x==.5f,"new UObject serial cannot reuse an old applied scale");
    Fresh();Put(players[0].actor,0xEF8,static_cast<void*>(players[1].hud));Tick(0);
    Check(facingCalls==1&&ownerCalls==0&&scaleCalls==0,"shared world/foreign widgets keep the complete native path");
    Fresh();gameThread=false;Tick(0);Check(facingCalls==1&&ownerCalls==0&&scaleCalls==0,"other threads are untouched");
    Fresh();dg::interaction_hud::g_ready=false;Tick(0);Check(facingCalls==1&&ownerCalls==0&&scaleCalls==0,"unavailable module remains native");
    Fresh();owners[0].index=2;Tick(0);Check(facingCalls==1&&ownerCalls==0&&scaleCalls==0,"unknown local slot remains native");
    Fresh();Put(players[0].hud,0x17C,dg::interaction_hud::Scale{-1,1,1});Tick(0);Check(scaleCalls==0,"unknown mirrored geometry is not scaled");
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    Check(GetTempPathW(MAX_PATH,directory)&&GetTempFileNameW(directory,L"DGI",0,path),"temporary INI available");
    dg::config::Store a;a.SetPath(path);a.interactionHudScaleEnabled=false;a.interactionHudScale=.5f;
    Check(a.SaveNow(),"HUD settings save");dg::config::Store b;b.SetPath(path);
    Check(b.Load()&&!b.interactionHudScaleEnabled&&b.interactionHudScale==.5f&&
        dg::runtime::Get().interactionHudScale.load()==.5f&&b.hotkeys==a.hotkeys,"native-size toggle and sub-unit scale survive INI/runtime without changing bindings");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}}
namespace dg::local_players {bool ResolveActor(void* p,Context& c){for(int i=0;i<2;++i)if(p==players[i].actor){c=owners[i];return true;}return false;}}
int main(){try{Run();std::cout<<"Owned interaction HUD: camera context, reversible scale, identities and configuration passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
