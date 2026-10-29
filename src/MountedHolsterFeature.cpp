#include "MountedHolsterFeature.h"
#include "PistolAppearanceSlots.h"
#include "PistolVisualFeature.h"
#include <MinHook.h>
#include "NativeMemoryReader.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <algorithm>
#include <array>
#include <atomic>
#include <cstdio>

namespace dg::mounted_holsters {
namespace {
using MeshFn=bool(*)(void*,void*);
MeshFn g_setMesh=nullptr;uintptr_t g_base=0;LogFn g_log=nullptr;
std::atomic_bool g_ready{false},g_boundaryReady{false};unsigned g_changes=0;std::atomic_uint32_t g_retained[2]{};
native_objects::Identity g_models[4]{}; // right/left hand, right/left stowed
native_objects::Identity g_meshClass{};
struct Saved {native_objects::Identity component{},original{},applied{};};
Saved g_saved[2][2]{}; // game thread only
Saved g_hands[2][4]{}; // foot R/L, mounted R/L; independent from holster leases
int g_cursor=0;ULONGLONG g_scanTick=0;
template<class T> bool Read(uintptr_t p,size_t offset,T& value){
    native_objects::MemoryReader r;return r.Read(p,offset,value);
}
template<size_t N> bool NameIs(uintptr_t p,const char (&expected)[N]){
    return native_objects::NameIs(g_base,p,expected);
}
int Model(uintptr_t object){
    uintptr_t cls=0,package=0;
    if(!Read(object,0x10,cls)||cls!=g_meshClass.address||!Read(object,0x20,package))return -1;
    if(NameIs(object,"Strife_Gun_Mesh_R")&&NameIs(package,"/Game/Characters/Heroes/Hero_Strife/Model/Strife_Gun_Mesh_R"))return 0;
    if(NameIs(object,"Strife_Gun_Mesh_L")&&NameIs(package,"/Game/Characters/Heroes/Hero_Strife/Model/Strife_Gun_Mesh_L"))return 1;
    if(NameIs(object,"Strife_Gun_R_Stowed")&&NameIs(package,"/Game/Characters/Heroes/Hero_Strife/Model/Strife_Gun_R_Stowed"))return 2;
    if(NameIs(object,"Strife_Gun_L_Stowed")&&NameIs(package,"/Game/Characters/Heroes/Hero_Strife/Model/Strife_Gun_L_Stowed"))return 3;
    return -1;
}
bool Models(){
    native_objects::MemoryReader reader;bool complete=true;
    for(auto& id:g_models)if(!native_objects::Alive(reader,g_base,id)){id={};complete=false;}
    if(complete)return true;
    const auto now=GetTickCount64();if(now-g_scanTick<16)return false;g_scanTick=now;
    int count=0;uintptr_t chunks=0;
    if(!Read(g_base+0x3959694,0,count)||count<=0||count>2000000||!Read(g_base+0x3959680,0,chunks))return false;
    if(g_cursor>=count)g_cursor=0;
    struct Item {uintptr_t object;uint32_t flags,pad;int serial,reserved;};
    static_assert(sizeof(Item)==24);
    // At most 512 records per 16 ms while missing assets. No loading, no
    // persistent per-frame object-table scan after all four are identified.
    for(unsigned batch=0;batch<4&&g_cursor<count;++batch){
        const int n=(std::min)({128,count-g_cursor,65536-g_cursor%65536});
        uintptr_t chunk=0;std::array<Item,128> items{};
        if(!Read(chunks,size_t(g_cursor/65536)*8,chunk))return false;
        // Read exactly n items; the fixed array must not cross a chunk boundary.
        for(int i=0;i<n;++i)if(!Read(chunk,size_t((g_cursor%65536)+i)*24,items[size_t(i)]))return false;
        g_cursor+=n;
        for(int i=0;i<n;++i){
            const auto& item=items[size_t(i)];if(!item.object||(item.flags&0x30000000u))continue;
            const int slot=Model(item.object);
            if(slot>=0&&slot<4&&!g_models[slot])native_objects::CaptureLive(g_base,reinterpret_cast<void*>(item.object),g_models[slot]);
        }
    }
    for(const auto& id:g_models)if(!native_objects::Alive(reader,g_base,id))return false;
    return true;
}
bool Component(uintptr_t horse,uintptr_t body,uintptr_t component){
    uintptr_t owner=0,parent=0,vt=0;
    return Read(component,0,vt)&&vt==g_base+0x2A575C8&&
        Read(component,0xE8,owner)&&owner==horse&&Read(component,0x108,parent)&&parent==body;
}
bool SourceComponent(const local_players::Context& owner,uintptr_t& component){
    size_t offset=0;unsigned char type=0;uintptr_t body=0;
    if(owner.kind==local_players::PoseKind::Horse)offset=0xBF8;
    else if(owner.kind==local_players::PoseKind::Foot&&owner.pawn==owner.character&&
            Read(owner.character,0xC18,type)&&type==1)offset=0x1040;
    else return false;
    return Read(owner.pawn,0x380,body)&&Read(owner.pawn,offset,component)&&Component(owner.pawn,body,component);
}
void Restore(Saved& saved){
    native_objects::MemoryReader reader;uintptr_t mesh=0;
    if(!saved.component)return;
    if(!native_objects::Alive(reader,g_base,saved.component)||
       !Read(saved.component.address,0x5B0,mesh)||mesh!=saved.applied.address){saved={};return;}
    if(native_objects::Alive(reader,g_base,saved.original)&&g_setMesh&&
       g_setMesh(reinterpret_cast<void*>(saved.component.address),reinterpret_cast<void*>(saved.original.address)))saved={};
}
int Choice(unsigned side){
    const auto& rt=runtime::Get();const int choice=side?rt.pistolLeftModel.load():rt.pistolRightModel.load();
    return choice>=1&&choice<=2?choice:0;
}
void SetModel(Saved& saved,uintptr_t component,int target){
    native_objects::Identity identity{};uintptr_t current=0;
    if(!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(component),identity)||!Read(component,0x5B0,current))return;
    if(saved.component&&saved.component!=identity)Restore(saved);
    if(saved.component&&saved.component!=identity)return;
    // A native/foreign mesh change supersedes our baseline. Unknown assets
    // are never replaced. Returning to Native restores only our last write.
    if(saved.component&&current!=saved.applied.address)saved={};
    if(target<0){Restore(saved);return;}
    int original=-1;
    for(int i=0;i<4;++i)if(current==g_models[i].address)original=i;
    if(original<0||current==g_models[target].address)return;
    const auto baseline=saved.component?saved.original:g_models[original];
    if(g_setMesh(reinterpret_cast<void*>(component),reinterpret_cast<void*>(g_models[target].address))){
        saved={identity,baseline,g_models[target]};
        pistol_visual::OnMeshAssigned(reinterpret_cast<void*>(component));
        if(g_log&&++g_changes<=12)g_log("Pistol models V1.67: owned component mesh changed; native socket, visibility, animation and projectile path preserved; EXPERIMENTAL");
    }
}
int CachedModel(uintptr_t model){
    native_objects::MemoryReader reader;
    for(int i=0;i<4;++i)if(g_models[i].address==model&&native_objects::Alive(reader,g_base,g_models[i]))return i;
    return -1;
}
void* NativeMeshChoice(const pistol_appearance::OwnedSlot& owned,void* requested,int& incoming,int& target){
    incoming=CachedModel(reinterpret_cast<uintptr_t>(requested));target=-1;
    const int choice=Choice(owned.slot%2);
    if(owned.slot<4)target=choice?choice-1:-1;
    else if(owned.slot<6)target=choice?choice+1:(runtime::Get().mountedHolstersEnabled.load()?int(owned.slot%2)+2:-1);
    native_objects::MemoryReader reader;
    return incoming>=0&&target>=0&&native_objects::Alive(reader,g_base,g_models[target])?
        reinterpret_cast<void*>(g_models[target].address):requested;
}
bool HookSetMesh(void* component,void* mesh){
    if(!g_setMesh)return false;
    pistol_appearance::OwnedSlot owned{};
    const bool tracked=g_ready.load()&&native_tps::OnGameThread()&&
        pistol_appearance::Resolve(g_base,reinterpret_cast<uintptr_t>(component),owned)&&owned.slot<6;
    const unsigned player=unsigned(owned.context.index);
    if(!tracked||player>1)return g_setMesh(component,mesh);
    int incoming=-1,target=-1;void* effective=NativeMeshChoice(owned,mesh,incoming,target);
    Saved& saved=owned.slot<4?g_hands[player][owned.slot]:g_saved[player][owned.slot-4];
    if(saved.component&&saved.component!=owned.component)Restore(saved);
    const bool result=g_setMesh(component,effective);
    uintptr_t actual=0;
    if(result&&Read(owned.component.address,0x5B0,actual)){
        if(effective!=mesh&&actual==reinterpret_cast<uintptr_t>(effective)){
            saved={owned.component,g_models[incoming],g_models[target]};
            if(g_log&&++g_retained[player]<=8)g_log("Pistol models V1.129: native assignment retained the selected owned model; menus and focus independent; EXPERIMENTAL");
        }else if(saved.component==owned.component&&
                 (actual!=saved.applied.address||(effective==mesh&&actual==reinterpret_cast<uintptr_t>(mesh))))saved={};
    }
    // OFF is enforced after the mesh assignment too: an engine refresh must
    // not expose a newly assigned pistol before its next visibility event.
    pistol_visual::OnMeshAssigned(component);
    return result;
}
void ApplyHands(unsigned player,uintptr_t actor,bool mounted){
    if(player>1)return;
    uintptr_t body=0,components[2]{};unsigned char role=0,type=0;
    if(!Read(actor,0x118,role)||role!=3||!Read(actor,0x380,body)||
       (!mounted&&(!Read(actor,0xC18,type)||type!=1)))return;
    for(unsigned side=0;side<2;++side){
        const size_t offset=(mounted?0xBA0:0x1040)+side*8;
        if(!Read(actor,offset,components[side])||!Component(actor,body,components[side]))continue;
        const int choice=Choice(side);
        SetModel(g_hands[player][(mounted?2:0)+side],components[side],choice?choice-1:-1);
    }
}
void Apply(unsigned player,uintptr_t horse){
    if(player>1)return;
    uintptr_t body=0,components[2]{},meshes[2]{};unsigned char role=0;
    if(!Read(horse,0x118,role)||role!=3||!Read(horse,0x380,body)||
       !Read(horse,0xBF8,components[0])||!Read(horse,0xC00,components[1]))return;
    for(unsigned i=0;i<2;++i)if(!Component(horse,body,components[i])||!Read(components[i],0x5B0,meshes[i])||
       (meshes[i]!=g_models[0].address&&meshes[i]!=g_models[1].address&&
        meshes[i]!=g_models[2].address&&meshes[i]!=g_models[3].address))return;
    for(unsigned i=0;i<2;++i){
        const int choice=Choice(i);
        const int target=choice?choice+1:(runtime::Get().mountedHolstersEnabled.load()?int(i)+2:-1);
        SetModel(g_saved[player][i],components[i],target);
    }
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    const unsigned char setter[]={0x48,0x8B,0xC4,0x55,0x57,0x41,0x56,0x48,0x8D,0x68,0x88,0x48,0x81,0xEC,0x60,0x01};
    uintptr_t target=0;
    if(!g_base||std::memcmp(reinterpret_cast<void*>(g_base+0x17DDBC0),setter,sizeof(setter))||
       !Read(g_base+0x2A575C8,0x868,target)||target!=g_base+0x17DDBC0)return;
    g_setMesh=reinterpret_cast<MeshFn>(target);void* original=nullptr;
    const auto mh=MH_Initialize();
    if((mh==MH_OK||mh==MH_ERROR_ALREADY_INITIALIZED)&&
       MH_CreateHook(reinterpret_cast<void*>(target),reinterpret_cast<void*>(&HookSetMesh),&original)==MH_OK){
        g_setMesh=reinterpret_cast<MeshFn>(original);
        if(MH_EnableHook(reinterpret_cast<void*>(target))==MH_OK)g_boundaryReady=true;
        else {MH_RemoveHook(reinterpret_cast<void*>(target));g_setMesh=reinterpret_cast<MeshFn>(target);}
    }
    g_ready=true;
    if(log)log(g_boundaryReady?"Pistol models V1.129 READY: owned native mesh assignments enforce selected models; shared visibility policy":"Pistol models V1.129: global mesh boundary unavailable; previous live reconciliation retained");
}
void OnLivePlayer(const local_players::Context& owner){
    if(!g_ready.load()||!native_tps::OnGameThread()||owner.index<0||owner.index>1)return;
    const unsigned player=unsigned(owner.index);const bool selected=Choice(0)||Choice(1);
    const bool enabled=runtime::Get().mountedHolstersEnabled.load()||selected;
    if(!enabled||owner.kind!=local_players::PoseKind::Horse)for(auto& saved:g_saved[player])Restore(saved);
    const bool foot=owner.kind==local_players::PoseKind::Foot&&owner.pawn==owner.character;
    for(unsigned i=0;i<4;++i)if(!selected||(i<2?!foot:owner.kind!=local_players::PoseKind::Horse))Restore(g_hands[player][i]);
    if(!enabled)return; // Appearance stays selected during pause, wheels, menus and cinematics.
    uintptr_t cls=0,component=0;
    if(owner.kind==local_players::PoseKind::Horse&&
       (!Read(owner.pawn,0x10,cls)||!NameIs(cls,"Despair_Blueprint_C")))return;
    if(!SourceComponent(owner,component))return;
    native_objects::MemoryReader reader;
    if(!native_objects::Alive(reader,g_base,g_meshClass)){
        uintptr_t mesh=0;
        if(!Read(component,0x5B0,mesh)||!Read(mesh,0x10,cls)||!NameIs(cls,"StaticMesh")||
           !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(cls),g_meshClass))return;
    }
    // Same bounded prewarm as V1.66. Only explicit per-hand choices may now
    // change foot models; Native/Native preserves the validated path.
    if(!Models())return;
    if(owner.kind==local_players::PoseKind::Horse){Apply(player,owner.pawn);ApplyHands(player,owner.pawn,true);}
    else if(foot)ApplyHands(player,owner.pawn,false);
}
bool Ready(){return g_ready.load();}
void Shutdown(){g_ready=false;}
}
