#include "MountedHolsterFeature.h"
#include "NativeMemoryReader.h"
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
std::atomic_bool g_ready{false};unsigned g_changes=0;
native_objects::Identity g_models[4]{}; // right/left hand, right/left stowed
native_objects::Identity g_meshClass{};
struct Saved {native_objects::Identity component{},original{},applied{};};
Saved g_saved[2][2]{}; // game thread only
int g_cursor=0;ULONGLONG g_scanTick=0;
template<class T> bool Read(uintptr_t p,size_t offset,T& value){
    native_objects::MemoryReader r;return r.Read(p,offset,value);
}
template<size_t N> bool NameIs(uintptr_t p,const char (&expected)[N]){
    int index=-1,number=-1;uintptr_t names=0,chunk=0,entry=0;uint32_t flags=0;
    if(!Read(p,0x18,index)||!Read(p,0x1C,number)||number||index<0||index>=2000000||
       !Read(g_base+0x3955198,0,names)||!Read(names,size_t(index/16384)*8,chunk)||
       !Read(chunk,size_t(index%16384)*8,entry)||!Read(entry,8,flags))return false;
    if(flags&1){
        std::array<wchar_t,N> text{};if(!Read(entry,0xC,text))return false;
        for(size_t i=0;i<N;++i)if(text[i]!=wchar_t(expected[i]))return false;
        return true;
    }
    std::array<char,N> text{};return Read(entry,0xC,text)&&std::memcmp(text.data(),expected,N)==0;
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
void Restore(Saved& saved){
    native_objects::MemoryReader reader;uintptr_t mesh=0;
    if(!saved.component)return;
    if(!native_objects::Alive(reader,g_base,saved.component)||
       !Read(saved.component.address,0x5B0,mesh)||mesh!=saved.applied.address){saved={};return;}
    if(native_objects::Alive(reader,g_base,saved.original)&&g_setMesh&&
       g_setMesh(reinterpret_cast<void*>(saved.component.address),reinterpret_cast<void*>(saved.original.address)))saved={};
}
void Apply(unsigned player,uintptr_t horse){
    if(player>1)return;
    uintptr_t body=0,components[2]{},meshes[2]{};unsigned char role=0;
    if(!Read(horse,0x118,role)||role!=3||!Read(horse,0x380,body)||
       !Read(horse,0xBF8,components[0])||!Read(horse,0xC00,components[1]))return;
    for(unsigned i=0;i<2;++i)if(!Component(horse,body,components[i])||!Read(components[i],0x5B0,meshes[i])||
       (meshes[i]!=g_models[i].address&&meshes[i]!=g_models[i+2].address))return;
    for(unsigned i=0;i<2;++i){
        auto& saved=g_saved[player][i];native_objects::Identity identity{};
        if(!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(components[i]),identity))continue;
        if(saved.component&&saved.component!=identity)Restore(saved);
        if(saved.component&&saved.component!=identity)continue;
        if(meshes[i]==g_models[i+2].address)continue;
        if(g_setMesh(reinterpret_cast<void*>(components[i]),reinterpret_cast<void*>(g_models[i+2].address))){
            saved={identity,g_models[i],g_models[i+2]};
            if(g_log&&++g_changes<=8){char line[130]{};sprintf_s(line,"Mounted holsters V1.65 APPLIED player=%u side=%u nativeStowedMesh=1 EXPERIMENTAL",player+1,i);g_log(line);}
        }
    }
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    const unsigned char setter[]={0x48,0x8B,0xC4,0x55,0x57,0x41,0x56,0x48,0x8D,0x68,0x88,0x48,0x81,0xEC,0x60,0x01};
    uintptr_t target=0;
    if(!g_base||std::memcmp(reinterpret_cast<void*>(g_base+0x17DDBC0),setter,sizeof(setter))||
       !Read(g_base+0x2A575C8,0x868,target)||target!=g_base+0x17DDBC0)return;
    g_setMesh=reinterpret_cast<MeshFn>(target);g_ready=true;
    if(log)log("Mounted holsters V1.65 READY: optional native stowed models, two mounted holster components only; no hooks; EXPERIMENTAL");
}
void OnLivePlayer(const local_players::Context& owner){
    if(!g_ready.load()||!native_tps::OnGameThread()||owner.index<0||owner.index>1)return;
    const unsigned player=unsigned(owner.index);const bool enabled=runtime::Get().mountedHolstersEnabled.load();
    if(!enabled||owner.kind!=local_players::PoseKind::Horse){for(auto& saved:g_saved[player])Restore(saved);return;}
    uintptr_t cls=0;
    if(!Read(owner.pawn,0x10,cls)||!NameIs(cls,"Despair_Blueprint_C")||
       native_tps::IsWorldPaused(reinterpret_cast<void*>(owner.pawn))||native_tps::CinematicActive(player))return;
    native_objects::MemoryReader reader;
    if(!native_objects::Alive(reader,g_base,g_meshClass)){
        uintptr_t body=0,component=0,mesh=0;
        if(!Read(owner.pawn,0x380,body)||!Read(owner.pawn,0xBF8,component)||!Component(owner.pawn,body,component)||
           !Read(component,0x5B0,mesh)||!Read(mesh,0x10,cls)||!NameIs(cls,"StaticMesh")||
           !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(cls),g_meshClass))return;
    }
    if(!Models())return;
    Apply(player,owner.pawn);
}
bool Ready(){return g_ready.load();}
void Shutdown(){g_ready=false;}
}
