#include "../src/WrathSlotFeature.cpp"
#include "ConfigStore.h"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
using dg::wrath_slots::Name;
unsigned char actors[2][0x1100]{},controllers[2][0x400]{},resources[2][0x480]{},classes[3][0x80]{},entries[4][64]{};
uintptr_t names[4]{},chunks[1]{reinterpret_cast<uintptr_t>(names)},resourceChain[1]{};
dg::local_players::Context owners[2]{};
bool gameThread=true,resolve=true,nested=false;unsigned resolves=0;
bool foreignReady=false;unsigned foreignResolves=0;
struct Call {void* actor;uintptr_t* result;Name id;bool* unlocked;bool* affordable;};
std::vector<Call> calls;
template<class T> void Put(void* p,size_t offset,const T& value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
Name Id(unsigned family,int number){return {int(family),number};}
uintptr_t* Native(void* actor,uintptr_t* result,Name id,bool* unlocked,bool* affordable){
    calls.push_back({actor,result,id,unlocked,affordable});
    if(nested){
        nested=false;uintptr_t other=0;bool a=false,b=false;
        dg::wrath_slots::HookResolve(actors[1],&other,Id(1,2),&a,&b);
    }
    if(result)*result=uintptr_t(0x1000+id.index*0x100+id.number);
    if(unlocked)*unlocked=id.number!=4;
    if(affordable)*affordable=id.number!=3;
    return result;
}
void Fresh(){
    using namespace dg::wrath_slots;
    gameThread=resolve=true;nested=false;resolves=0;foreignReady=false;foreignResolves=0;calls.clear();g_ready=true;g_original=Native;g_resolving=false;g_log=nullptr;
    std::memset(actors,0,sizeof(actors));std::memset(controllers,0,sizeof(controllers));std::memset(resources,0,sizeof(resources));
    std::memset(classes,0,sizeof(classes));resourceChain[0]=reinterpret_cast<uintptr_t>(classes[2])+0x30;
    Put(classes[2],0x30,reinterpret_cast<uintptr_t>(resourceChain));Put(classes[2],0x38,0);
    for(unsigned family=0;family<2;++family){
        Put(classes[family],0x18,int(family+2));Put(actors[family],0x10,reinterpret_cast<uintptr_t>(classes[family]));
        Put<unsigned char>(actors[family],0x118,3);Put<unsigned char>(actors[family],0xC18,static_cast<unsigned char>(family+1));
        Put(actors[family],0x348,reinterpret_cast<uintptr_t>(controllers[family]));Put(controllers[family],0x350,reinterpret_cast<uintptr_t>(actors[family]));
        Put(actors[family],0xEA0,reinterpret_cast<uintptr_t>(resources[family]));Put(resources[family],0x10,reinterpret_cast<uintptr_t>(classes[2]));
        owners[family]={};auto& owner=owners[family];owner.index=int(family);owner.localCount=2;owner.localPlayer=0x5000+family;
        owner.controller=reinterpret_cast<uintptr_t>(controllers[family]);owner.character=owner.pawn=reinterpret_cast<uintptr_t>(actors[family]);
        for(unsigned slot=0;slot<3;++slot){
            Put(resources[family],0x3E4+slot*8,Id(family,int(slot+2)));
            dg::runtime::Get().wrathSlots[family][slot]=0;
        }
    }
}
Name Invoke(unsigned family,int number){
    uintptr_t result=0;bool unlocked=false,affordable=false;
    const auto first=calls.size();auto* out=dg::wrath_slots::HookResolve(actors[family],&result,Id(family,number),&unlocked,&affordable);
    const auto selected=calls[first].id;
    Check(out==&result&&result==uintptr_t(0x1000+selected.index*0x100+selected.number),"native return destination and class result retained");
    Check(unlocked==(selected.number!=4)&&affordable==(selected.number!=3),"both native target unlock and affordability results retained");
    Check(calls[first].actor==actors[family]&&calls[first].result==&result&&calls[first].unlocked==&unlocked&&calls[first].affordable==&affordable,
        "actor and all four other ABI arguments forwarded unchanged");
    return selected;
}
void Run(){
    using namespace dg::wrath_slots;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    const char* texts[]={"Strife_Wrath","War_Wrath","Strife_Blueprint_C","War_Blueprint_C"};
    for(unsigned i=0;i<4;++i){names[i]=reinterpret_cast<uintptr_t>(entries[i]);strcpy_s(reinterpret_cast<char*>(entries[i]+12),52,texts[i]);}
    Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));Put(image.data(),0x3810268,reinterpret_cast<uintptr_t>(classes[2]));
    Fresh();Check(Invoke(0,2)==Id(0,2)&&calls.size()==1&&resolves==0,"all Native has no ownership discovery or extra native call");
    for(unsigned family=0;family<2;++family)for(unsigned slot=0;slot<3;++slot)for(int power=1;power<=3;++power){
        Fresh();dg::runtime::Get().wrathSlots[family][slot]=int(family)*3+power;
        unsigned char beforeActor[sizeof(actors)]{},beforeResources[sizeof(resources)]{};
        std::memcpy(beforeActor,actors,sizeof(actors));std::memcpy(beforeResources,resources,sizeof(resources));
        Check(Invoke(family,int(slot+2))==Id(family,power+1)&&calls.size()==1,"all eighteen native slot/power combinations resolve exactly once");
        Check(!std::memcmp(beforeActor,actors,sizeof(actors))&&!std::memcmp(beforeResources,resources,sizeof(resources)),"no actor, input, action or resource-ID bytes written");
        for(unsigned other=0;other<3;++other)if(other!=slot)Check(Invoke(family,int(other+2))==Id(family,int(other+2)),"other slots unaffected");
    }
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;dg::runtime::Get().wrathSlots[1][0]=5;
    Check(Invoke(0,2)==Id(0,3)&&Invoke(1,2)==Id(1,3),"both local characters retain their independent settings");
    owners[0].index=1;owners[1].index=0;
    Check(Invoke(0,2)==Id(0,3)&&Invoke(1,2)==Id(1,3),"settings follow the character, not a hardcoded P1/P2 assignment");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;
    Put(resources[0],0x3E4,Id(0,4));Put(resources[0],0x3F4,Id(0,2));
    Check(Invoke(0,4)==Id(0,3)&&Invoke(0,2)==Id(0,2),"semantic slots use live native IDs after reordering");
    const auto failOpen=[](const char* why){Check(Invoke(0,2)==Id(0,2),why);};
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;Put(resources[0],0x3EC,Id(0,2));failOpen("duplicate native IDs reject the complete mapping");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;Put(resources[0],0x3F4,Id(1,4));failOpen("foreign resource family rejects mapping");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=5;failOpen("rejected Rampage transfer falls back to Native even in stale runtime config");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=4;foreignReady=true;
    Check(Invoke(0,2)==Id(0,2)&&foreignResolves==1,"Geyser selects its foreign class while keeping the native semantic slot");
    for(unsigned slot=0;slot<3;++slot)for(int choice:{2,3}){
        Fresh();dg::runtime::Get().wrathSlots[1][slot]=choice;foreignReady=true;
        Check(Invoke(1,int(slot+2))==Id(1,int(slot+2))&&foreignResolves==1&&calls.size()==1,"Caltrops on War retains each original slot unlock/cost ABI and one native resolve");
        Check(Invoke(0,2)==Id(0,2)&&foreignResolves==1,"War transfer cannot remap Strife's own Caltrops or other slots");
    }
    Fresh();dg::runtime::Get().wrathSlots[0][0]=4;
    {uintptr_t out=123;bool a=true,b=true;Check(HookResolve(actors[0],&out,Id(0,2),&a,&b)==&out&&out&&a&&!b&&calls.size()==1,
        "unavailable foreign power retains native icon/class and unlock, blocks cast affordability only");}
    Fresh();dg::runtime::Get().wrathSlots[0][0]=4;foreignReady=true;
    Check(Invoke(0,2)==Id(0,2)&&foreignResolves==1&&calls.size()==1,"foreign adapter retains original semantic slot unlock ID and complete native resolve ABI");
    Check(Invoke(0,3)==Id(0,3)&&foreignResolves==1,"other slots stay native while foreign slot selected");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;gameThread=false;failOpen("non-game-thread query remains native");Check(resolves==0,"no owner discovery off game thread");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;resolve=false;failOpen("unknown/nonlocal ownership remains native");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;owners[0].kind=dg::local_players::PoseKind::Horse;failOpen("horse uses native powers");
    owners[0].kind=dg::local_players::PoseKind::Ultimate;failOpen("ultimate uses native powers");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;Put<unsigned char>(actors[0],0x118,2);failOpen("remote role remains native");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;Put(controllers[0],0x350,reinterpret_cast<uintptr_t>(actors[1]));failOpen("controller replacement rejected");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;Put(classes[0],0x18,3);failOpen("unknown or foreign actor Blueprint rejected");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;Put(resources[0],0x10,uintptr_t(0));failOpen("unexpected resource type rejected");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;Check(Invoke(0,5)==Id(0,5),"synergy/unknown ID never remapped");
    Fresh();dg::runtime::Get().wrathSlots[0][0]=2;dg::runtime::Get().wrathSlots[1][0]=5;nested=true;
    Check(Invoke(0,2)==Id(0,3)&&calls.size()==2&&calls[1].id==Id(1,2),"native nested queries are not remapped recursively");
    Shutdown();calls.clear();failOpen("shutdown immediately restores original lookup");Check(resolves==1,"shutdown does not discover or dereference a cached actor");
    const unsigned char prologue[]={0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x6C,0x24,0x10,0x48,0x89,0x74,0x24,0x18};
    const unsigned char getter[]={0xE8,0xD4,0x79,0x14,0x00},unlock[]={0x4C,0x8B,0xC3,0x33,0xD2,0xE8,0x3C,0xC0,0x04,0x00},cost[]={0x48,0x8B,0xCD,0xE8,0xDD,0xA2,0x17,0x00};
    std::memcpy(image.data()+0x663220,prologue,sizeof(prologue));std::memcpy(image.data()+0x663257,getter,sizeof(getter));
    std::memcpy(image.data()+0x66331A,unlock,sizeof(unlock));std::memcpy(image.data()+0x66332B,cost,sizeof(cost));
    Check(Boundary(),"verified native lookup and both checks match");image[0x66332F]^=1;Check(!Boundary(),"changed cost call disables hook");
}
void Config(){
    using namespace dg::config;using namespace dg::wrath_slots;
    static_assert(int(Action::Count)==55);
    static_assert(Supported(0,6)&&Foreign(0,6)&&Supported(1,6)&&!Foreign(1,6)&&NativeNumber(1,6)==4&&Supported(0,4)&&Foreign(0,4)&&Supported(1,1)&&Foreign(1,1));
    static_assert(Supported(1,3)&&Foreign(1,3)&&NativeNumber(1,3)==0&&!Supported(0,5)&&Supported(1,2)&&Foreign(1,2));
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    Check(GetTempPathW(MAX_PATH,directory)&&GetTempFileNameW(directory,L"DGW",0,path),"temporary config created");
    Store a;a.SetPath(path);Check(a.Load()&&a.wrathSlots==Choices{},"missing six keys preserve native mapping");
    a.wrathSlots={{{3,2,1},{1,5,4}}};a.menuKey=0xC0;a.aimPadButtonA=0x20;a.aimPadButtonB=0x100;a.hotkeys[0]=Action::RefillWrath;
    Check(a.SaveNow(),"choices saved synchronously");Store b;b.SetPath(path);
    Check(b.Load()&&a.wrathSlots==b.wrathSlots&&b.menuKey==0xC0&&b.aimPadButtonA==0x20&&b.aimPadButtonB==0x100&&b.hotkeys[0]==Action::RefillWrath,
        "all six choices round-trip without replacing custom bindings");
    for(unsigned f=0;f<2;++f)for(unsigned s=0;s<3;++s)Check(dg::runtime::Get().wrathSlots[f][s].load()==b.wrathSlots[f][s],"six runtime choices published");
    WritePrivateProfileStringW(L"Values",Keys[0][0],L"4",path);WritePrivateProfileStringW(L"Values",Keys[1][2],L"1",path);
    WritePrivateProfileStringW(L"Values",Keys[0][1],L"-1",path);WritePrivateProfileStringW(L"Values",Keys[1][1],L"999",path);
    Check(b.Load()&&b.wrathSlots[0][0]==4&&b.wrathSlots[0][1]==0&&b.wrathSlots[1][1]==0&&b.wrathSlots[1][2]==1,"supported Geyser loads while unsupported, negative and unknown choices fall back to Native");
    WritePrivateProfileStringW(L"Values",Keys[0][0],L"6",path);Check(b.Load()&&b.wrathSlots[0][0]==6&&dg::runtime::Get().wrathSlots[0][0].load()==6,"Stoneskin uses its reserved ID without changing keys");
    WritePrivateProfileStringW(L"Values",Keys[1][0],L"3",path);Check(b.Load()&&b.wrathSlots[1][0]==3&&dg::runtime::Get().wrathSlots[1][0].load()==3&&b.menuKey==0xC0,"War Caltrops choice loads using existing key while custom menu binding remains intact");
    WritePrivateProfileStringW(L"Values",Keys[0][0],L"5",path);WritePrivateProfileStringW(L"Values",Keys[1][0],L"2",path);
    Check(b.Load()&&b.wrathSlots[0][0]==0&&b.wrathSlots[1][0]==2,"rejected Strife Rampage restores Native; accepted World Ender preserved");
    b.ResetDefaults(false);Check(b.wrathSlots==Choices{},"reset restores all native powers");DeleteFileW(path);
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}}
namespace dg::foreign_wrath {
void Initialize(LogFn,bool){}
void Shutdown(){foreignReady=false;}
int Status(int){return foreignReady?2:1;}
void Observe(const local_players::Context&){}
bool Prepare(void*,uintptr_t& cls,int choice){cls=foreignReady?(choice==4?0xB1ADE:0xFACE):0;return foreignReady;}
uintptr_t* Resolve(ResolveFn native,void* actor,uintptr_t* result,Name id,bool* unlocked,bool* affordable,uintptr_t cls){
    Check(cls==0xFACE||cls==0xB1ADE,"selected foreign class reaches scoped adapter");++foreignResolves;return native(actor,result,id,unlocked,affordable);
}
}
namespace dg::local_players {
bool ResolveActor(void* actor,Context& owner){
    ++resolves;if(!resolve)return false;
    for(unsigned i=0;i<2;++i)if(actor==actors[i]){owner=owners[i];return true;}
    return false;
}
}
int main(){try{Run();Config();std::cout<<"Native Wrath slot ABI, ownership, unlock/cost and profile checks passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
