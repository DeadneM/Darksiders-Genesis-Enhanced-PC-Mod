#pragma once
#include "NativePropertyField.h"
#include "PistolAppearanceSlots.h"
#include "RuntimeSettings.h"
// Decorative components only. Original weapon components, traces and sockets
// remain untouched. All calls are game-thread ProcessEvent with live contracts.
namespace dg::stowed_blades {
using Id=native_objects::Identity;
using Event=void(*)(void*,void*,void*);
using Hide=void(*)(uintptr_t,bool);
inline uintptr_t base=0;inline Event event=nullptr;inline Hide hide=nullptr;
struct Function {Id id{};};inline Function functions[5]{};
struct Copy {Id owner{},component{},asset{},parent{};uint64_t socket=0;bool visible=false;};
struct Player {Copy sides[2]{};Id actor{};unsigned attempts=0;bool enabled=false;};
inline Player players[2]{};inline Id prototype{};inline int cursor=0;inline bool complete=false;
template<class T>bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
inline bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,base,id);}
inline bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(base,reinterpret_cast<void*>(p),id);}
inline bool Name(uintptr_t p,std::string_view name){int index=0,number=0;return Read(p,0x18,index)&&Read(p,0x1C,number)&&native_objects::NameValueEquals(base,index,number,name);}
inline uintptr_t Fn(unsigned key,uintptr_t object,const char* label,uintptr_t native,unsigned bytes){
    auto& cached=functions[key];uintptr_t entry=0;uint16_t size=0;
    if(Alive(cached.id)&&Read(cached.id.address,0xC0,entry)&&entry==base+native&&Read(cached.id.address,0x9E,size)&&size==bytes)return cached.id.address;
    cached={};uintptr_t cls=0;if(!Read(object,0x10,cls))return 0;
    for(unsigned depth=0;cls&&depth<16;++depth){uintptr_t field=0;if(!Read(cls,0x48,field))return 0;
        for(unsigned n=0;field&&n<768;++n){
            if(Name(field,label)){uintptr_t kind=0;
                if(!Read(field,0x10,kind)||!Name(kind,"Function")||!Read(field,0xC0,entry)||entry!=base+native||
                   !Read(field,0x9E,size)||size!=bytes||!Capture(field,cached.id))return 0;
                return field;}
            if(!Read(field,0x28,field))return 0;
        }
        if(field||!Read(cls,0x40,cls))return 0;
    }return 0;
}
inline bool Param(uintptr_t f,const char* name,const char* type,int size,int expected){int at=-1;return native_objects::PropertyField(base,f,name,type,size,at)&&at==expected;}
inline void Call(uintptr_t obj,uintptr_t fn,void* args){event(reinterpret_cast<void*>(obj),reinterpret_cast<void*>(fn),args);}
inline bool Template(){
    if(Alive(prototype))return true;if(complete)return false;
    int count=0;uintptr_t chunks=0;if(!Read(base+0x3959694,0,count)||count<=0||count>2000000||!Read(base+0x3959680,0,chunks))return false;
    for(unsigned n=0;n<512&&cursor<count;++n,++cursor){uintptr_t chunk=0,obj=0,cls=0,outer=0;
        if(!Read(chunks,size_t(cursor/65536)*8,chunk)||!Read(chunk,size_t(cursor%65536)*24,obj))return false;
        if(obj&&Name(obj,"LeftPistol_GEN_VARIABLE")&&Read(obj,0x10,cls)&&Name(cls,"StaticMeshComponent")&&
           Read(obj,0x20,outer)&&Name(outer,"Strife_Blueprint_C")&&Capture(obj,prototype))return true;
    }
    complete=cursor>=count;return false;
}
inline void HideCopy(Copy& c){if(Alive(c.owner)&&Alive(c.component)&&c.visible){uintptr_t owner=0;if(Read(c.component.address,0xE8,owner)&&owner==c.owner.address)hide(c.component.address,true);}c.visible=false;}
inline void Release(Copy& c){
    HideCopy(c);if(Alive(c.owner)&&Alive(c.component)){uintptr_t owner=0;
        if(Read(c.component.address,0xE8,owner)&&owner==c.owner.address){
            const auto f=Fn(4,owner,"K2_DestroyComponent",0x1CB91D0,8);
            if(f&&Param(f,"Component","ObjectProperty",8,0)){uintptr_t arg=c.component.address;Call(owner,f,&arg);}
        }
    }c={};
}
inline bool Create(uintptr_t owner,uintptr_t context,uintptr_t asset,Copy& c){
    const auto f=Fn(0,owner,"AddComponent",0x1CB67F0,80);
    if(!f||!Param(f,"TemplateName","NameProperty",8,0)||!Param(f,"bManualAttachment","BoolProperty",1,8)||
       !Param(f,"RelativeTransform","StructProperty",48,16)||!Param(f,"ComponentTemplateContext","ObjectProperty",8,64)||
       !Param(f,"ReturnValue","ObjectProperty",8,72))return false;
    alignas(16) std::array<unsigned char,80> args{};uint64_t name=0;if(!Read(prototype.address,0x18,name))return false;
    std::memcpy(args.data(),&name,8);args[8]=1;const float identity[12]={0,0,0,1,0,0,0,0,1,1,1,0};
    std::memcpy(args.data()+16,identity,48);std::memcpy(args.data()+64,&context,8);Call(owner,f,args.data());
    uintptr_t comp=0,actual=0,vt=0;std::memcpy(&comp,args.data()+72,8);
    if(!comp||!Read(comp,0xE8,actual)||actual!=owner||!Read(comp,0,vt)||vt!=base+0x2A575C8||!Capture(owner,c.owner)||!Capture(comp,c.component))return false;
    hide(comp,true);
    const auto collision=Fn(1,comp,"SetCollisionEnabled",0x1DCC3C0,1);
    if(!collision||!Param(collision,"NewType","ByteProperty",1,0)){Release(c);return false;}
    unsigned char noCollision=0;Call(comp,collision,&noCollision);
    using Mesh=bool(*)(void*,void*);
    if(!Capture(asset,c.asset)||!reinterpret_cast<Mesh>(base+0x17DDBC0)(reinterpret_cast<void*>(comp),reinterpret_cast<void*>(asset))){Release(c);return false;}
    return true;
}
inline bool Attach(Copy& c,uintptr_t parent,uint64_t socket){
    Id parentId{};if(!Capture(parent,parentId))return false;
    if(c.parent==parentId&&c.socket==socket)return true;
    const auto f=Fn(2,c.component.address,"K2_AttachToComponent",0x1DD86B0,21);
    if(!f||!Param(f,"Parent","ObjectProperty",8,0)||!Param(f,"SocketName","NameProperty",8,8)||
       !Param(f,"LocationRule","EnumProperty",1,16)||!Param(f,"RotationRule","EnumProperty",1,17)||
       !Param(f,"ScaleRule","EnumProperty",1,18)||!Param(f,"bWeldSimulatedBodies","BoolProperty",1,19)||!Param(f,"ReturnValue","BoolProperty",1,20))return false;
    alignas(16) std::array<unsigned char,32> args{};std::memcpy(args.data(),&parent,8);std::memcpy(args.data()+8,&socket,8);
    args[16]=args[17]=args[18]=2;Call(c.component.address,f,args.data());if(!args[20])return false;
    c.parent=parentId;c.socket=socket;return true;
}
inline uint64_t HolsterSocket(uintptr_t parent,unsigned hand){
    uintptr_t mesh=0;struct Array {uintptr_t data;int count,capacity;} info{};
    if(!Read(parent,0x5A0,mesh)||!Read(mesh,0x190,info)||!info.data||info.count<=0||info.count>512||info.capacity<info.count||info.capacity>1024)return 0;
    for(int i=0;i<info.count;++i){uint64_t name=0;if(!Read(info.data,size_t(i)*12,name))return 0;
        if(native_objects::NameValueEquals(base,int(uint32_t(name)),int(name>>32),hand?"Bone_Strife_Holster_R":"Bone_Strife_Holster_L"))return name;
    }return 0;
}
inline void Update(const local_players::Context& c){
    if(!event||!hide||c.index<0||c.index>1)return;
    auto& p=players[c.index];const bool enabled=runtime::Get().showStrifeStowedBlades.load();
    Id actor{};unsigned char type=0;uintptr_t cls=0,body=0;
    const bool valid=enabled&&c.localPlayer&&c.character&&Read(c.character,0xC18,type)&&type==1&&Read(c.character,0x10,cls)&&Name(cls,"Strife_Blueprint_C")&&
        ((c.kind==local_players::PoseKind::Foot&&c.pawn==c.character)||(c.kind==local_players::PoseKind::Horse&&c.pawn!=c.character&&
        Read(c.pawn,0x10,cls)&&pistol_appearance::StrifeMountClass(base,cls)))&&Capture(c.pawn,actor)&&Read(c.pawn,0x380,body);
    if(!valid||p.actor!=actor){for(auto& s:p.sides)Release(s);p={};}
    if(!valid)return;
    if(!p.enabled){cursor=0;complete=false;}p.enabled=true;p.actor=actor;
    if(!Template())return;
    const int visibility=runtime::Get().strifeBladeVisibility.load();
    const unsigned off=visibility==1?2u:visibility==2?1u:visibility==3?3u:0u;
    for(unsigned hand=0;hand<2;++hand){auto& copy=p.sides[hand];uintptr_t original=0,asset=0,owner=0;unsigned char flags=0;
        // Only the native hidden (sheathed) state is decorated. No original
        // component visibility, transform, animation, collision or trace changes.
        const bool show=!(off&(1u<<hand))&&Read(c.character,hand?0x1018:0x1010,original)&&Read(original,0xE8,owner)&&owner==c.character&&
            Read(original,0x1CC,flags)&&(flags&0x40)&&Read(original,0x5B0,asset)&&asset;
        if(!show){HideCopy(copy);continue;}
        if(copy.component&&(!Alive(copy.component)||copy.asset.address!=asset)){Release(copy);}
        if(!copy.component){if(p.attempts>=4)continue;++p.attempts;if(!Create(c.pawn,c.character,asset,copy))continue;}
        const uint64_t socket=copy.parent.address==body?copy.socket:HolsterSocket(body,hand);
        if(!socket||!Attach(copy,body,socket)){HideCopy(copy);continue;}
        if(!copy.visible){hide(copy.component.address,false);copy.visible=true;}
    }
}
}
