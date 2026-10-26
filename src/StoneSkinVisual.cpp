#include "StoneSkinVisual.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include <array>
#include <cstdio>

namespace dg::stone_visual {
namespace {
using Id=native_objects::Identity;
struct Array {uintptr_t data=0;int count=0,capacity=0;};
using Event=void(*)(void*,void*,void*);
Event g_event=nullptr;uintptr_t g_base=0;foreign_wrath::LogFn g_log=nullptr;unsigned g_events=0;
Id g_materials[2]{};
struct Slot {Id material{},original{};bool inherited=false;};
struct Visual {Id actor{},mesh{};uintptr_t world=0;int variant=-1;std::array<Slot,4> slots{};};
Visual g_visual[2]{};
template<class T>bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
template<size_t N>bool Is(uintptr_t p,const char(&s)[N]){return native_objects::NameIs(g_base,p,s);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),id);}
struct CachedField {Id cls{};const char* name=nullptr;const char* kind=nullptr;int size=0,offset=-1;};
std::array<CachedField,24> g_fields{};
bool Field(uintptr_t p,const char* name,const char* kind,int size,int& offset){
    uintptr_t cls=0;if(!Read(p,0x10,cls))return false;
    CachedField* free=nullptr;
    for(auto& f:g_fields){
        if(f.cls&&!Alive(f.cls))f={};
        if(!f.cls){if(!free)free=&f;continue;}
        if(f.cls.address==cls&&f.size==size&&!std::strcmp(f.name,name)&&!std::strcmp(f.kind,kind)){offset=f.offset;return true;}
    }
    if(!native_objects::PropertyField(g_base,cls,name,kind,size,offset))return false;
    Id identity{};if(free&&Capture(cls,identity))*free={identity,name,kind,size,offset};return true;
}
bool List(uintptr_t p,size_t at,Array& a,int limit){return Read(p,at,a)&&a.count>=0&&a.count<=a.capacity&&a.capacity<=limit&&(a.data||!a.count);}
uintptr_t Function(uintptr_t object,const char* name,uintptr_t entry){
    uintptr_t cls=0;if(!Read(object,0x10,cls))return 0;
    for(int n=0;cls&&n<16;++n){
        uintptr_t f=0;if(!Read(cls,0x48,f))return 0;
        for(int i=0;f&&i<512;++i){
            uintptr_t kind=0,fn=0;int index=0,number=0;
            if(!Read(f,0x10,kind)||!Read(f,0x18,index)||!Read(f,0x1C,number))return 0;
            if(native_objects::NameValueEquals(g_base,index,number,name))return Is(kind,"Function")&&Read(f,0xC0,fn)&&fn==g_base+entry?f:0;
            if(!Read(f,0x28,f))return 0;
        }
        if(f||!Read(cls,0x40,cls))return 0;
    }
    return 0;
}
bool Parameter(uintptr_t f,const char* name,const char* kind,int size,int offset){int at=-1;return native_objects::PropertyField(g_base,f,name,kind,size,at)&&at==offset;}
bool Set(uintptr_t mesh,int index,uintptr_t material){
    const auto f=Function(mesh,"SetMaterial",0x1DCCC80);
    if(!f||!Parameter(f,"ElementIndex","IntProperty",4,0)||!Parameter(f,"Material","ObjectProperty",8,8))return false;
    struct Args {int index=0,pad=0;uintptr_t material=0;} args{index,0,material};
    g_event(reinterpret_cast<void*>(mesh),reinterpret_cast<void*>(f),&args);return true;
}
bool Override(uintptr_t mesh,int index,uintptr_t& value){
    int at=-1;Array a{};value=0;
    return Field(mesh,"OverrideMaterials","ArrayProperty",16,at)&&List(mesh,at,a,32)&&
        (index>=a.count||Read(a.data,size_t(index)*8,value));
}
uintptr_t Create(uintptr_t mesh,int index,uintptr_t original){
    const auto f=Function(mesh,"CreateDynamicMaterialInstance",0x1DC92E0);
    if(!f||!Parameter(f,"ElementIndex","IntProperty",4,0)||!Parameter(f,"SourceMaterial","ObjectProperty",8,8)||
       !Parameter(f,"OptionalName","NameProperty",8,16)||!Parameter(f,"ReturnValue","ObjectProperty",8,24))return 0;
    struct Args {int index=0,pad=0;uintptr_t source=0;foreign_wrath::Name name{0,0};uintptr_t result=0;} args{index,0,original};
    static_assert(sizeof(Args)==32);
    g_event(reinterpret_cast<void*>(mesh),reinterpret_cast<void*>(f),&args);return args.result;
}
bool Copy(uintptr_t material,uintptr_t source){
    const auto f=Function(material,"K2_CopyMaterialInstanceParameters",0x1D96D80);
    if(!f||!Parameter(f,"Source","ObjectProperty",8,0)||!Parameter(f,"bQuickParametersOnly","BoolProperty",1,8))return false;
    struct Args {uintptr_t source=0;bool quick=true;unsigned char pad[7]{};} args{source};
    g_event(reinterpret_cast<void*>(material),reinterpret_cast<void*>(f),&args);return true;
}
void Restore(Visual& v){
    uintptr_t owner=0;
    if(g_event&&Alive(v.actor)&&Alive(v.mesh)&&Read(v.mesh.address,0xE8,owner)&&owner==v.actor.address){
        for(unsigned i=0;i<v.slots.size();++i){auto& s=v.slots[i];uintptr_t current=0;
            // Only our MID may be replaced. A native/user skin change wins.
            if(Alive(s.material)&&Override(v.mesh.address,int(i),current)&&current==s.material.address&&
               (s.inherited||Alive(s.original)))Set(v.mesh.address,int(i),s.inherited?0:s.original.address);
        }
    }
    v={};
}
int Status(uintptr_t actor){
    int at=-1;uintptr_t component=0,owner=0;Array statuses{};bool stone=false,molten=false;
    if(!Field(actor,"m_pStatusEffectComponent","ObjectProperty",8,at)||!Read(actor,at,component)||
       !Read(component,0xE8,owner)||owner!=actor||!Field(component,"m_statusEffects","ArrayProperty",16,at)||!List(component,at,statuses,128))return -1;
    for(int i=0;i<statuses.count;++i){
        uintptr_t status=0,cls=0,instigator=0;
        if(!Read(statuses.data,size_t(i)*8,status)||!status||!Read(status,0x10,cls)||
           !Field(status,"m_pInstigatorActor","ObjectProperty",8,at)||!Read(status,at,instigator)||instigator!=actor)continue;
        for(int n=0;cls&&n<12;++n){
            if(Is(cls,"MayhemStatusEffect_StoneSkin_Base_Blueprint_C"))stone=true;
            if(Is(cls,"MayhemStatusEffect_Core_FallenDog_Molten_1_Blueprint_C")||Is(cls,"MayhemStatusEffect_Core_FallenDog_Molten_2_Blueprint_C")||
               Is(cls,"MayhemStatusEffect_Core_FallenDog_Molten_3_Blueprint_C"))molten=true;
            if(!Read(cls,0x40,cls))break;
        }
    }
    return stone?(molten?1:0):-1;
}
}
void Initialize(foreign_wrath::LogFn log,bool exact){
    g_log=log;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    // Existing exact-executable ProcessEvent boundary, possibly already
    // detoured by the mod's UI observer. No additional hook or native ABI.
    g_event=reinterpret_cast<Event>(g_base+0xAC0630);
}
void ObserveObject(uintptr_t p){
    if(!g_event||!native_tps::OnGameThread())return;uintptr_t cls=0,package=0;
    if(!Read(p,0x10,cls)||!Is(cls,"MaterialInstanceConstant")||!Read(p,0x20,package))return;
    const int kind=Is(p,"War_Inst_StoneSkin_A")?0:(Is(p,"War_Inst_StoneSkin_Molten_A")?1:-1);
    if(kind>=0&&!Alive(g_materials[kind])&&
       (kind?Is(package,"/Game/Characters/Heroes/Hero_War/Textures/War_Inst_StoneSkin_Molten_A"):
             Is(package,"/Game/Characters/Heroes/Hero_War/Textures/War_Inst_StoneSkin_A")))Capture(p,g_materials[kind]);
}
bool AssetsReady(){return Alive(g_materials[0])&&Alive(g_materials[1]);}
bool InvalidateAssets(){
    bool changed=false;for(auto& id:g_materials)if(id&&!Alive(id)){id={};changed=true;}return changed;
}
void Observe(const local_players::Context& c){
    if(!g_event||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    auto& v=g_visual[c.index];uintptr_t cls=0,pc=0,pawn=0,mesh=0,owner=0;unsigned char role=0;
    const bool owned=c.localPlayer&&c.world&&c.kind==local_players::PoseKind::Foot&&c.character&&c.pawn==c.character&&
        Read(c.character,0x10,cls)&&Is(cls,"Strife_Blueprint_C")&&Read(c.character,0x118,role)&&role==3&&
        Read(c.character,0x348,pc)&&pc==c.controller&&Read(pc,0x350,pawn)&&pawn==c.character&&
        Read(c.character,0x380,mesh)&&Read(mesh,0xE8,owner)&&owner==c.character;
    const int variant=owned?Status(c.character):-1;
    if(v.actor&&(variant<0||variant!=v.variant||v.world!=c.world||v.actor.address!=c.character||v.mesh.address!=mesh||!Alive(v.actor)||!Alive(v.mesh)))Restore(v);
    if(!owned||variant<0||!Alive(g_materials[variant]))return;
    if(!v.actor){
        if(!Capture(c.character,v.actor)||!Capture(mesh,v.mesh)){v={};return;}
        v.world=c.world;v.variant=variant;
    }
    // Strife has four populated body/gear/cloth slots; the fifth is empty.
    // A MID's original parent holds the previous appearance strongly through
    // GC. Only native uniform parameters are copied; no shared material edit.
    for(unsigned i=0;i<v.slots.size();++i){
        auto& slot=v.slots[i];uintptr_t current=0;
        if(!Override(mesh,int(i),current))continue;
        if(Alive(slot.material)&&current==slot.material.address)continue;
        slot={};
        const auto f=Function(mesh,"GetMaterial",0x1DC9CB0);
        if(!f||!Parameter(f,"ElementIndex","IntProperty",4,0)||!Parameter(f,"ReturnValue","ObjectProperty",8,8))continue;
        struct Args {int index=0,pad=0;uintptr_t result=0;} args{int(i)};
        g_event(reinterpret_cast<void*>(mesh),reinterpret_cast<void*>(f),&args);
        Id original{};if(!args.result||!Capture(args.result,original))continue;
        const auto mid=Create(mesh,int(i),args.result);uintptr_t parent=0;int at=-1;Id identity{};
        if(!mid||mid==args.result||!Field(mid,"Parent","ObjectProperty",8,at)||!Read(mid,at,parent)||parent!=args.result||
           !Capture(mid,identity)||!Copy(mid,g_materials[variant].address)){
            // Factory installs the MID before copying. Roll back on failure.
            uintptr_t now=0;if(mid&&Override(mesh,int(i),now)&&now==mid)Set(mesh,int(i),current);
            continue;
        }
        slot={identity,original,current==0};
        if(g_log&&g_events++<8)g_log("Stoneskin V1.112 visual: private Strife MID, native base/molten parameters; status-owned lifetime; EXPERIMENTAL UV fit");
    }
}
void Shutdown(){
    if(native_tps::OnGameThread())for(auto& v:g_visual)Restore(v);
    g_event=nullptr;g_fields={};
}
}
