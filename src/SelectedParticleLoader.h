#pragma once
#include "OwnedParticleTransform.h"
#include <array>
namespace dg::selected_particle {
// Only the selected cosmetic ParticleSystem is requested, once per choice/world.
// Observed native CDO is revalidated through its rooted class and UObject slot;
// CDO serials can legitimately remain zero. No guessed ClassDefaultObject offset.
struct State {native_objects::Identity cls{},make{},load{};uintptr_t context=0;int index=-1,serial=0;bool busy=false;};
inline bool Context(uintptr_t base,const State& s){
    native_objects::MemoryReader r;uintptr_t object=0,cls=0;int serial=0;uint32_t flags=0,ownFlags=0;
    return owned_particle::Alive(base,s.cls)&&native_objects::Item(r,base,s.index,object,serial,flags)&&object==s.context&&serial==s.serial&&
        r.Read(object,0x10,cls)&&cls==s.cls.address&&r.Read(object,8,ownFlags)&&(ownFlags&0x10)&&
        native_objects::NameIs(base,object,"Default__KismetSystemLibrary");
}
inline void Observe(uintptr_t base,uintptr_t object,State& s){
    if(Context(base,s))return;
    uintptr_t cls=0;unsigned flags=0;int index=-1,serial=0;uintptr_t entry=0;uint32_t itemFlags=0;native_objects::MemoryReader r;
    if(!r.Read(object,8,flags)||!(flags&0x10)||!native_objects::NameIs(base,object,"Default__KismetSystemLibrary")||
       !r.Read(object,0x10,cls)||!native_objects::NameIs(base,cls,"KismetSystemLibrary")||!r.Read(object,0xC,index)||
       !native_objects::Item(r,base,index,entry,serial,itemFlags)||entry!=object)return;
    native_objects::Identity id{};if(!native_objects::CaptureLive(base,reinterpret_cast<void*>(cls),id))return;
    s={};s.cls=id;s.context=object;s.index=index;s.serial=serial;
}
inline void* Load(uintptr_t base,const char* package,const char* name,State& s,owned_particle::Event event=nullptr){
    if(s.busy||!Context(base,s))return nullptr;
    using namespace owned_particle;
    if(!Alive(base,s.make)||!Alive(base,s.load)){
        const auto make=Function(base,s.cls.address,"MakeSoftObjectPath",0x1D79110),load=Function(base,s.cls.address,"LoadAsset_Blocking",0x1D78A90);
        if(!make||!load||!Parameter(base,make,"PathString","StrProperty",16,0)||!Parameter(base,make,"ReturnValue","StructProperty",24,16)||
           !Parameter(base,load,"Asset","SoftObjectProperty",40,0)||!Parameter(base,load,"ReturnValue","ObjectProperty",8,40)||
           !native_objects::CaptureLive(base,reinterpret_cast<void*>(make),s.make)||!native_objects::CaptureLive(base,reinterpret_cast<void*>(load),s.load))return nullptr;
    }
    std::array<wchar_t,512> path{};size_t n=0;
    for(const char* p=package;*p;++p){if(n>=path.size()-2||*p<32||*p>126)return nullptr;path[n++]=wchar_t(*p);}
    if(n<6||path[0]!='/'||path[1]!='G'||path[2]!='a'||path[3]!='m'||path[4]!='e'||path[5]!='/')return nullptr;
    path[n++]=L'.';for(const char* p=name;*p;++p){if(n>=path.size()-1||*p<32||*p>126)return nullptr;path[n++]=wchar_t(*p);}
    struct String {uintptr_t data=0;int num=0,capacity=0;};
    struct Path {uint64_t assetName=0;String sub{};};
    struct MakeArgs {String input{};Path result{};} make{{reinterpret_cast<uintptr_t>(path.data()),int(n+1),int(n+1)},{}};
    struct LoadArgs {int weakIndex=-1,weakSerial=0,tag=0,pad=0;Path path{};void* result=nullptr;} load;
    static_assert(sizeof(MakeArgs)==40&&sizeof(LoadArgs)==48);
    if(!event)event=reinterpret_cast<Event>(base+0xAC0630);
    s.busy=true;event(reinterpret_cast<void*>(s.context),reinterpret_cast<void*>(s.make.address),&make);
    // These catalog paths contain no subobject. Do not accept an unexpected
    // allocated subpath or feed a guessed string/reference layout to the loader.
    if(!make.result.assetName||make.result.sub.data||make.result.sub.num||make.result.sub.capacity){s.busy=false;return nullptr;}
    load.path=make.result;event(reinterpret_cast<void*>(s.context),reinterpret_cast<void*>(s.load.address),&load);s.busy=false;return load.result;
}
}
