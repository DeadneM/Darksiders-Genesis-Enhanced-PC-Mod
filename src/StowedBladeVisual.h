#pragma once
#include "PistolAppearanceSlots.h"
#include "RuntimeSettings.h"
#include <cstdio>

namespace dg::stowed_blades {
using Id=native_objects::Identity;
using LogFn=void(*)(const char*);
using Hide=void(*)(uintptr_t,bool);
using Construct=void*(*)(void*,void*,uint64_t,uint32_t,uint32_t,void*,bool,void*,bool);
using Register=void(*)(void*);
using SetMesh=bool(*)(void*,void*);
using Destroy=void(*)(void*,void*);
using Attach=void(*)(void*,void*,uint64_t);
using Collision=void(*)(void*,unsigned char);
inline uintptr_t base=0;
inline Hide hide=nullptr;
inline LogFn logger=nullptr;
inline Construct construct=nullptr;
inline Register registerComponent=nullptr;
inline SetMesh setMesh=nullptr;
inline Destroy destroy=nullptr;
inline Attach setupAttachment=nullptr;
inline Collision setCollision=nullptr;

struct Copy {
    Id component{},asset{};
    bool visible=false;
};
struct Player {
    Id actor{},body{},skeleton{};
    uint64_t sockets[2]{};
    Copy sides[2]{};
    bool attempted[2]{};
};
inline Player players[2]{};

template<class T>bool Read(uintptr_t p,size_t offset,T& value){
    native_objects::MemoryReader reader;
    return reader.Read(p,offset,value);
}
inline bool Alive(const Id& id){
    native_objects::MemoryReader reader;
    return native_objects::Alive(reader,base,id);
}
inline bool Capture(uintptr_t p,Id& id){
    return native_objects::CaptureLive(base,reinterpret_cast<void*>(p),id);
}
inline bool Name(uintptr_t p,std::string_view expected){
    int index=0,number=0;
    return Read(p,0x18,index)&&Read(p,0x1C,number)&&native_objects::NameValueEquals(base,index,number,expected);
}
inline bool Owned(uintptr_t component,uintptr_t actor){
    uintptr_t owner=0,vtable=0;
    return Read(component,0xE8,owner)&&owner==actor&&Read(component,0,vtable)&&vtable==base+0x2A575C8;
}
inline void Discard(const Id& actor,Copy& copy){
    if(Alive(actor)&&Alive(copy.component)&&Owned(copy.component.address,actor.address))
        destroy(reinterpret_cast<void*>(actor.address),reinterpret_cast<void*>(copy.component.address));
    copy={};
}
inline void Visibility(const Id& actor,Copy& copy,bool visible){
    if(copy.visible==visible||!Alive(copy.component)||!Owned(copy.component.address,actor.address))return;
    hide(copy.component.address,!visible);
    copy.visible=visible;
}
inline bool BladeAsset(uintptr_t asset,unsigned hand){
    uintptr_t cls=0;
    return Read(asset,0x10,cls)&&Name(cls,"StaticMesh")&&
        Name(asset,hand?"Strife_Weapon_Dagger_L":"Strife_Weapon_Dagger_R");
}

inline void Report(unsigned hand,const char* outcome){
    static unsigned reports=0;
    if(!logger||reports>=16)return;
    ++reports;
    char line[128]{};
    sprintf_s(line,"Stowed blades V1.182 hand=%s result=%s",hand?"left":"right",outcome);
    logger(line);
}

// Construct a meshless StaticMeshComponent of the exact native pistol
// component class. It becomes renderable only after attachment and setup.
inline const char* Create(const Player& player, uintptr_t cls, const Id& asset, unsigned hand, Copy& copy){
    auto* actor=reinterpret_cast<void*>(player.actor.address);
    auto* created=construct(reinterpret_cast<void*>(cls),actor,0,0x40u,0,nullptr,false,nullptr,false);
    const auto component=reinterpret_cast<uintptr_t>(created);
    if(!Owned(component,player.actor.address))return "component creation failed";
    if(!Capture(component,copy.component)){
        destroy(actor,created);
        return "component identity unavailable";
    }
    hide(component,true);
    setCollision(created,0);
    if(!setMesh(created,reinterpret_cast<void*>(asset.address)))return "mesh rejected";

    setupAttachment(created,reinterpret_cast<void*>(player.body.address),player.sockets[hand]);
    uintptr_t parent=0;
    uint64_t socket=0;
    if(!Read(component,0x108,parent)||parent!=player.body.address||
        !Read(component,0x110,socket)||socket!=player.sockets[hand])return "holster attachment rejected";

    registerComponent(created);
    unsigned char flags=0;
    if(!Read(component,0xAC,flags)||!(flags&1u))return "component registration rejected";
    copy.asset=asset;
    return nullptr;
}

inline void FindHolsters(uintptr_t skeleton,uint64_t (&sockets)[2]){
    struct Array {uintptr_t data;int count,capacity;} info{};
    if(!Read(skeleton,0x190,info)||!info.data||info.count<=0||info.count>512||
        info.capacity<info.count||info.capacity>1024)return;
    for(int i=0;i<info.count&&(!sockets[0]||!sockets[1]);++i){
        uint64_t value=0;
        if(!Read(info.data,size_t(i)*12,value))return;
        const int index=int(uint32_t(value)),number=int(value>>32);
        if(native_objects::NameValueEquals(base,index,number,"Bone_Strife_Holster_L"))sockets[0]=value;
        if(native_objects::NameValueEquals(base,index,number,"Bone_Strife_Holster_R"))sockets[1]=value;
    }
}

inline void Initialize(uintptr_t module,Hide setHidden,LogFn log,bool verifiedMeshSetter){
    base=module;
    logger=log;
    constexpr unsigned char constructSig[]={0x40,0x55,0x53,0x56,0x57,0x41,0x54,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x40};
    constexpr unsigned char registerSig[]={0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x8B,0x89,0xE8,0,0,0};
    constexpr unsigned char destroySig[]={0x4C,0x8B,0xC2,0x48,0x85,0xD2,0x74,0x18,0x48,0x39,0x8A,0xE8,0,0,0};
    constexpr unsigned char attachSig[]={0xF6,0x81,0xAC,0,0,0,0x01,0x4C,0x8B,0xCA,0x75,0x7A,0x48,0x3B,0xD1};
    constexpr unsigned char collisionSig[]={0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xEC,0x20};
    // MountedHolsterFeature verified SetStaticMesh before installing its detour.
    if(!base||!setHidden||!verifiedMeshSetter||
        std::memcmp(reinterpret_cast<void*>(base+0xAC57A0),constructSig,sizeof(constructSig))||
        std::memcmp(reinterpret_cast<void*>(base+0x1750780),registerSig,sizeof(registerSig))||
        std::memcmp(reinterpret_cast<void*>(base+0x163EFA0),destroySig,sizeof(destroySig))||
        std::memcmp(reinterpret_cast<void*>(base+0x17B2430),attachSig,sizeof(attachSig))||
        std::memcmp(reinterpret_cast<void*>(base+0x1B10D20),collisionSig,sizeof(collisionSig))||
        *reinterpret_cast<const uintptr_t*>(base+0x2A575C8+0x5E0)!=base+0x1B10D20)return;
    hide=setHidden;
    construct=reinterpret_cast<Construct>(base+0xAC57A0);
    registerComponent=reinterpret_cast<Register>(base+0x1750780);
    destroy=reinterpret_cast<Destroy>(base+0x163EFA0);
    setupAttachment=reinterpret_cast<Attach>(base+0x17B2430);
    setCollision=reinterpret_cast<Collision>(base+0x1B10D20);
    setMesh=reinterpret_cast<SetMesh>(base+0x17DDBC0);
}
inline bool Ready(){return construct!=nullptr;}

inline void Update(const local_players::Context& owner){
    if(!Ready()||owner.index<0||owner.index>1)return;
    auto& player=players[owner.index];
    const auto& rt=runtime::Get();
    const bool enabled=rt.showStrifeStowedBlades.load();
    if(!enabled&&!player.actor)return;

    const bool foot=owner.kind==local_players::PoseKind::Foot&&owner.pawn==owner.character;
    const bool mounted=owner.kind==local_players::PoseKind::Horse&&owner.pawn!=owner.character;
    uintptr_t cls=0,body=0,bodyOwner=0,skeleton=0;
    Id actorId{},bodyId{},skeletonId{};
    const bool valid=enabled&&owner.localPlayer&&(foot||mounted)&&
        Read(owner.character,0x10,cls)&&Name(cls,"Strife_Blueprint_C")&&
        (!mounted||(Read(owner.pawn,0x10,cls)&&pistol_appearance::StrifeMountClass(base,cls)))&&
        pistol_appearance::Controlled(owner.pawn)&&Capture(owner.pawn,actorId)&&
        Read(owner.pawn,0x380,body)&&Read(body,0xE8,bodyOwner)&&bodyOwner==owner.pawn&&Capture(body,bodyId)&&
        Read(body,0x5A0,skeleton)&&Capture(skeleton,skeletonId);
    if(!valid||player.actor!=actorId||player.body!=bodyId||player.skeleton!=skeletonId){
        for(auto& side:player.sides)Discard(player.actor,side);
        player={};
        if(!valid)return;
        player.actor=actorId;
        player.body=bodyId;
        player.skeleton=skeletonId;
        FindHolsters(skeleton,player.sockets);
    }

    uintptr_t sourceBody=0;
    if(!Read(owner.character,0x380,sourceBody))return;
    const int choice=rt.strifeBladeVisibility.load();
    const unsigned off=choice==1?2u:choice==2?1u:choice==3?3u:0u;
    for(unsigned hand=0;hand<2;++hand){
        auto& copy=player.sides[hand];
        uintptr_t weapon=0,mesh=0;
        unsigned char flags=0;
        const bool show=!(off&(1u<<hand))&&player.sockets[hand]&&
            pistol_appearance::Component(base,owner.character,sourceBody,hand+6,weapon,flags)&&
            (mounted||(flags&0x40))&&Read(weapon,0x5B0,mesh)&&BladeAsset(mesh,hand);
        if(!show){
            Visibility(actorId,copy,false);
            continue;
        }
        if(copy.component&&(!Alive(copy.component)||copy.asset.address!=mesh)){
            Discard(actorId,copy);
            player.attempted[hand]=false;
        }
        if(!copy.component&&!player.attempted[hand]){
            uintptr_t donor=0,componentClass=0;
            unsigned char donorFlags=0;
            Id asset{};
            if(!pistol_appearance::Component(base,owner.character,sourceBody,0,donor,donorFlags)||
                !Read(donor,0x10,componentClass)||!Name(componentClass,"StaticMeshComponent")||!Capture(mesh,asset))continue;
            player.attempted[hand]=true;
            const char* failure=Create(player,componentClass,asset,hand,copy);
            if(failure){
                Discard(actorId,copy);
                Report(hand,failure);
            }else Report(hand,"attached and registered");
        }
        Visibility(actorId,copy,true);
    }
}
}
