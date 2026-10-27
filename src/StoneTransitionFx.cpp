#include "StoneTransitionFx.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include <cstdio>
#include <cstddef>

namespace dg::stone_fx {
namespace {
using Id=native_objects::Identity;
uintptr_t g_base=0;Event g_event=nullptr;Id g_class{},g_assets[3]{};
template<class T>bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
template<size_t N>bool Is(uintptr_t p,const char(&s)[N]){return native_objects::NameIs(g_base,p,s);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),id);}
uintptr_t Function(){
    uintptr_t field=0;if(!Alive(g_class)||!Read(g_class.address,0x48,field))return 0;
    for(int i=0;field&&i<512;++i){uintptr_t cls=0,outer=0;uint32_t flags=0;uint16_t bytes=0;
        if(Is(field,"SpawnEmitterAttached"))return Read(field,0x10,cls)&&Is(cls,"Function")&&Read(field,0x20,outer)&&outer==g_class.address&&
            Read(field,0x98,flags)&&(flags&0x2400u)==0x2400u&&Read(field,0x9E,bytes)&&bytes==72?field:0;
        uintptr_t next=0;if(!Read(field,0x28,next)||next==field)return 0;field=next;
    }return 0;
}
bool Parameter(uintptr_t f,const char* name,const char* kind,int size,int offset){int at=-1;return native_objects::PropertyField(g_base,f,name,kind,size,at)&&at==offset;}
}
void Initialize(uintptr_t base,Event dispatch){g_base=base;g_event=dispatch;}
void ObserveObject(uintptr_t p){
    if(!g_event||!native_tps::OnGameThread())return;uintptr_t cls=0,package=0;
    if(!Read(p,0x10,cls)||!Read(p,0x20,package))return;
    if(Is(cls,"Class")&&Is(p,"GameplayStatics")&&Is(package,"/Script/Engine")){if(!Alive(g_class))Capture(p,g_class);return;}
    if(!Is(cls,"ParticleSystem"))return;
    constexpr const char* names[]={"P_WarStoneSkin_Base","P_WarStoneSkin_Molten","P_War_StoneSkin_Base_Crumble_Expiration"};
    int index=0,number=0,pindex=0,pnumber=0;
    if(!Read(p,0x18,index)||!Read(p,0x1C,number)||!Read(package,0x18,pindex)||!Read(package,0x1C,pnumber))return;
    for(int i=0;i<3;++i){char path[180]{};sprintf_s(path,"/Game/VFX/Particles/Characters/War/StoneSkin/%s",names[i]);
        if(native_objects::NameValueEquals(g_base,index,number,names[i])&&native_objects::NameValueEquals(g_base,pindex,pnumber,path))Capture(p,g_assets[i]);}
}
bool AssetsReady(){return Alive(g_class)&&Alive(g_assets[0])&&Alive(g_assets[1])&&Alive(g_assets[2]);}
bool Ready(int kind){return g_event&&kind>=0&&kind<3&&Alive(g_class)&&Alive(g_assets[kind]);}
bool InvalidateAssets(){bool changed=false;for(auto* id:{&g_class,&g_assets[0],&g_assets[1],&g_assets[2]})if(*id&&!Alive(*id)){*id={};changed=true;}return changed;}
bool Spawn(const local_players::Context& c,int kind){
    if(!g_event||!native_tps::OnGameThread()||kind<0||kind>2||!Alive(g_assets[kind])||c.index<0||c.index>1||
       !c.localPlayer||!c.world||c.kind!=local_players::PoseKind::Foot||c.character!=c.pawn)return false;
    uintptr_t cls=0,controller=0,pawn=0,mesh=0,owner=0;unsigned char role=0;
    if(!Read(c.character,0x10,cls)||!Is(cls,"Strife_Blueprint_C")||!Read(c.character,0x118,role)||role!=3||
       !Read(c.character,0x348,controller)||controller!=c.controller||!Read(controller,0x350,pawn)||pawn!=c.character||
       !Read(c.character,0x380,mesh)||!Read(mesh,0xE8,owner)||owner!=c.character)return false;
    const auto f=Function();uintptr_t cdo=0,actual=0;
    // The native class strongly owns its CDO; never clear template flags or
    // try to allocate a weak identity for a class-default object.
    if(!f||!Read(g_class.address,0xF8,cdo)||!Read(cdo,0x10,actual)||actual!=g_class.address||!Is(cdo,"Default__GameplayStatics")||
       !Parameter(f,"EmitterTemplate","ObjectProperty",8,0)||!Parameter(f,"AttachToComponent","ObjectProperty",8,8)||
       !Parameter(f,"AttachPointName","NameProperty",8,16)||!Parameter(f,"Location","StructProperty",12,24)||
       !Parameter(f,"Rotation","StructProperty",12,36)||!Parameter(f,"Scale","StructProperty",12,48)||
       !Parameter(f,"LocationType","ByteProperty",1,60)||!Parameter(f,"bAutoDestroy","BoolProperty",1,61)||
       !Parameter(f,"PoolingMethod","EnumProperty",1,62)||!Parameter(f,"ReturnValue","ObjectProperty",8,64))return false;
    struct Vec {float x=0,y=0,z=0;};
    struct Args {uintptr_t effect=0,mesh=0;foreign_wrath::Name socket{0,0};Vec position{},rotation{},scale{1,1,1};
        unsigned char locationType=0;bool autoDestroy=true;unsigned char pooling=0,pad=0;uintptr_t result=0;};
    static_assert(sizeof(Args)==72&&offsetof(Args,result)==64&&offsetof(Args,autoDestroy)==61);
    Args args{};args.effect=g_assets[kind].address;args.mesh=mesh;if(kind==2)args.position.z=180;
    // Explicitly authorized visual-only public API. The existing dispatcher
    // marshals the verified 72-byte parameters; no direct constructor call,
    // raw field writes, new native entry address or new hook is introduced.
    g_event(reinterpret_cast<void*>(cdo),reinterpret_cast<void*>(f),&args);return args.result!=0;
}
void Shutdown(){g_event=nullptr;g_class={};for(auto& id:g_assets)id={};}
}
