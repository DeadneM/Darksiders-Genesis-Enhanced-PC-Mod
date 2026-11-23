#pragma once
#include "PistolAppearanceSlots.h"
#include "MeleeEffectPolicy.h"
#include "RuntimeSettings.h"
#include <cmath>
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
struct alignas(16) Transform {
    float rotation[4];
    float translation[4]{};
    float scale[4]{1,1,1,0};
};
static_assert(sizeof(Transform)==48);
using SetTransform=void(*)(void*,const Transform*,bool,void*,unsigned char);
using SetDepth=void(*)(void*,bool);
using SetStencil=void(*)(void*,int);
using SetStencilMask=void(*)(void*,unsigned char);
inline uintptr_t base=0;
inline Hide hide=nullptr;
inline LogFn logger=nullptr;
inline Construct construct=nullptr;
inline Register registerComponent=nullptr;
inline SetMesh setMesh=nullptr;
inline Destroy destroy=nullptr;
inline Attach setupAttachment=nullptr;
inline Collision setCollision=nullptr;
inline SetTransform setRelativeTransform=nullptr;
inline SetDepth setDepth=nullptr;
inline SetStencil setStencil=nullptr;
inline SetStencilMask setStencilMask=nullptr;
inline bool mountedMontagesReady=false;

struct Array {uintptr_t data;int count,capacity;};
struct Copy {
    Id component{},asset{};
    bool visible=false,attempted=false;
};
struct Player {
    Id actor{},body{},skeleton{};
    uint64_t pelvis=0,hands[2]{};
    Copy stowed[2]{},held[2]{};
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

inline void Report(unsigned hand,const char* outcome,bool held){
    static unsigned reports[2]{};
    if(!logger||reports[held]>=16)return;
    ++reports[held];
    char line[128]{};
    sprintf_s(line,"%s hand=%s result=%s",held?"Mounted blades":"Stowed blades V1.182",hand?"left":"right",outcome);
    logger(line);
}

// Both meshes share these crossed holster offsets from the rider's pelvis.
// The faces lie against the hips, tips follow the holster axis and the cutting edges face down.
// Using the pelvis keeps the decorations independent of hidden gun holsters.
inline constexpr Transform stowedPose[]{
    {{-0.76540302f,-0.29184189f,-0.20434580f,-0.53593780f},{9.482513f,-7.064043f,20.547066f}},
    {{0.29183815f,-0.76540005f,-0.53594198f,0.20435133f},{9.482193f,-7.063946f,-20.547089f}},
};
// The rider meshes shrink the dagger bones to 0.000099. Attach to their
// animated parent hands and reuse the authored dagger-to-hand grip at unit scale.
inline constexpr Transform handGrip[]{
    {{0.0000115f,0.0002871f,0.9999826f,-0.0058919f},{-0.0079193f,-0.0017624f,0.0352688f}},
    {{0.0002350f,-0.0000311f,-0.0000808f,-0.9999999f},{-0.0000076f,0.0000229f,0.0000057f}},
};

// The native weapon on this rider supplies the settings used by the
// occlusion pass. Apply those to the new component before registration.
inline void CopyDepthSettings(uintptr_t donor,void* created){
    if(!setDepth||!setStencil||!setStencilMask)return;
    unsigned char depth=0,mask=0;
    int stencil=0;
    if(!Read(donor,0x275,depth)||!Read(donor,0x27B,mask)||!Read(donor,0x27C,stencil)||
       mask>9||stencil<0||stencil>255)return;
    setStencil(created,stencil);
    setStencilMask(created,mask);
    setDepth(created,(depth&0x80u)!=0);
}

// Make the new mesh renderable only after attachment and setup.
inline const char* Create(const Player& player,uintptr_t cls,const Id& asset,uint64_t socketName,
                          const Transform& transform,uintptr_t mountedDonor,Copy& copy){
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

    setupAttachment(created,reinterpret_cast<void*>(player.body.address),socketName);
    uintptr_t parent=0;
    uint64_t socket=0;
    if(!Read(component,0x108,parent)||parent!=player.body.address||
        !Read(component,0x110,socket)||socket!=socketName)return "blade attachment rejected";

    setRelativeTransform(created,&transform,false,nullptr,0);
    if(mountedDonor)CopyDepthSettings(mountedDonor,created);
    registerComponent(created);
    unsigned char flags=0;
    if(!Read(component,0xAC,flags)||!(flags&1u))return "component registration rejected";
    copy.asset=asset;
    return nullptr;
}

inline void FindSockets(Player& player,bool mounted){
    Array info{};
    if(!Read(player.skeleton.address,0x190,info)||!info.data||info.count<=0||info.count>512||
        info.capacity<info.count||info.capacity>1024)return;
    for(int i=0;i<info.count;++i){
        uint64_t value=0;
        if(!Read(info.data,size_t(i)*12,value))return;
        const int index=int(uint32_t(value)),number=int(value>>32);
        if(native_objects::NameValueEquals(base,index,number,"Bone_Strife_Pelvis"))player.pelvis=value;
        else if(mounted&&native_objects::NameValueEquals(base,index,number,"Bone_Strife_Hand_R"))player.hands[0]=value;
        else if(mounted&&native_objects::NameValueEquals(base,index,number,"Bone_Strife_Hand_L"))player.hands[1]=value;
        if(player.pelvis&&(!mounted||(player.hands[0]&&player.hands[1])))return;
    }
}

inline bool AnimInstanceClass(uintptr_t cls){
    for(unsigned depth=0;cls&&depth<16;++depth){
        if(Name(cls,"AnimInstance"))return true;
        if(!Read(cls,0x40,cls))return false;
    }
    return false;
}

// Both hands draw their enabled blades during either mounted attack.
inline bool MountedAttack(uintptr_t body){
    uintptr_t anim=0,outer=0,cls=0;
    uint32_t flags=0;
    Array instances{};
    if(!Read(body,0x718,anim)||!Read(anim,0x78,instances)||!instances.data||
       instances.count<=0||instances.count>16||instances.capacity<instances.count||instances.capacity>64||
       !Read(anim,8,flags)||(flags&0x18030u)||!Read(anim,0x20,outer)||outer!=body||
       !Read(anim,0x10,cls)||!AnimInstanceClass(cls))return false;
    for(int i=instances.count-1;i>=0;--i){
        uintptr_t instance=0,montage=0;
        float weight=0;
        if(!Read(instances.data,size_t(i)*sizeof(uintptr_t),instance))return false;
        if(!instance)continue;
        if(!Read(instance,0,montage)||!Read(instance,0x150,weight)||!std::isfinite(weight))return false;
        if(!montage||weight<=0)continue;
        uintptr_t package=0;
        if(!Read(montage,0x10,cls)||!Name(cls,"AnimMontage")||!Read(montage,0x20,package))continue;
        if(Name(montage,"Strife_Horse_Attack_R_Montage")&&
           Name(package,"/Game/Characters/Heroes/Hero_Strife/Animations/Horse/Strife_Horse_Attack_R_Montage"))return true;
        if(Name(montage,"Strife_Horse_Attack_L_Montage")&&
           Name(package,"/Game/Characters/Heroes/Hero_Strife/Animations/Horse/Strife_Horse_Attack_L_Montage"))return true;
    }
    return false;
}

inline bool PresentBlade(const Player& player,Copy& copy,uintptr_t character,uintptr_t sourceBody,
                         unsigned hand,uintptr_t mesh,uint64_t socket,bool held,bool mounted){
    if(!mesh||!socket){
        Visibility(player.actor,copy,false);
        return false;
    }
    if(copy.component&&(!Alive(copy.component)||copy.asset.address!=mesh))Discard(player.actor,copy);
    if(!copy.component&&!copy.attempted){
        uintptr_t donor=0,componentClass=0;
        unsigned char donorFlags=0;
        Id asset{};
        if(!pistol_appearance::Component(base,character,sourceBody,0,donor,donorFlags)||
           !Read(donor,0x10,componentClass)||!Name(componentClass,"StaticMeshComponent")||!Capture(mesh,asset))return false;
        uintptr_t mountedDonor=0;
        if(mounted){
            const unsigned slot=held?2+hand:4+(hand^1u);
            if(!pistol_appearance::Component(base,player.actor.address,player.body.address,slot,mountedDonor,donorFlags))mountedDonor=0;
        }
        const auto& transform=held?handGrip[hand]:stowedPose[hand];
        const char* failure=Create(player,componentClass,asset,socket,transform,mountedDonor,copy);
        if(failure)Discard(player.actor,copy);
        copy.attempted=true;
        Report(hand,failure?failure:"attached and registered",held);
    }
    Visibility(player.actor,copy,true);
    return copy.visible;
}

inline void Initialize(uintptr_t module,Hide setHidden,LogFn log,bool verifiedMeshSetter){
    base=module;
    logger=log;
    constexpr unsigned char constructSig[]={0x40,0x55,0x53,0x56,0x57,0x41,0x54,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x40};
    constexpr unsigned char registerSig[]={0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x8B,0x89,0xE8,0,0,0};
    constexpr unsigned char destroySig[]={0x4C,0x8B,0xC2,0x48,0x85,0xD2,0x74,0x18,0x48,0x39,0x8A,0xE8,0,0,0};
    constexpr unsigned char attachSig[]={0xF6,0x81,0xAC,0,0,0,0x01,0x4C,0x8B,0xCA,0x75,0x7A,0x48,0x3B,0xD1};
    constexpr unsigned char collisionSig[]={0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xEC,0x20};
    constexpr unsigned char transformSig[]={0x49,0x8B,0xC1,0x45,0x33,0xC9,0x45,0x84,0xC0,0x4C,0x0F,0x45,0xC8,0x0F,0xB6,0x44,0x24,0x28};
    // MountedHolsterFeature verified SetStaticMesh before installing its detour.
    if(!base||!setHidden||!verifiedMeshSetter||
        std::memcmp(reinterpret_cast<void*>(base+0xAC57A0),constructSig,sizeof(constructSig))||
        std::memcmp(reinterpret_cast<void*>(base+0x1750780),registerSig,sizeof(registerSig))||
        std::memcmp(reinterpret_cast<void*>(base+0x163EFA0),destroySig,sizeof(destroySig))||
        std::memcmp(reinterpret_cast<void*>(base+0x17B2430),attachSig,sizeof(attachSig))||
        std::memcmp(reinterpret_cast<void*>(base+0x1B10D20),collisionSig,sizeof(collisionSig))||
        std::memcmp(reinterpret_cast<void*>(base+0x17A77C0),transformSig,sizeof(transformSig))||
        *reinterpret_cast<const uintptr_t*>(base+0x2A575C8+0x5E0)!=base+0x1B10D20)return;
    hide=setHidden;
    construct=reinterpret_cast<Construct>(base+0xAC57A0);
    registerComponent=reinterpret_cast<Register>(base+0x1750780);
    destroy=reinterpret_cast<Destroy>(base+0x163EFA0);
    setupAttachment=reinterpret_cast<Attach>(base+0x17B2430);
    setCollision=reinterpret_cast<Collision>(base+0x1B10D20);
    setMesh=reinterpret_cast<SetMesh>(base+0x17DDBC0);
    setRelativeTransform=reinterpret_cast<SetTransform>(base+0x17A77C0);
    constexpr unsigned char depthSig[]={0x44,0x0F,0xB6,0xC2,0x0F,0xB6,0x91,0x75,0x02,0,0};
    constexpr unsigned char stencilSig[]={0x8B,0xC2,0x85,0xD2,0x79,0x04,0x33,0xD2,0xEB,0x0A,0xBA,0xFF,0,0,0,0x3B,0xC2,0x0F,0x4C,0xD0,0x39,0x91,0x7C,0x02,0,0};
    constexpr unsigned char maskSig[]={0x38,0x91,0x7B,0x02,0,0,0x74,0x0B,0x88,0x91,0x7B,0x02,0,0};
    if(!std::memcmp(reinterpret_cast<void*>(base+0x17842D0),depthSig,sizeof(depthSig))&&
       !std::memcmp(reinterpret_cast<void*>(base+0x1782F50),stencilSig,sizeof(stencilSig))&&
       !std::memcmp(reinterpret_cast<void*>(base+0x1782F90),maskSig,sizeof(maskSig))){
        setDepth=reinterpret_cast<SetDepth>(base+0x17842D0);
        setStencil=reinterpret_cast<SetStencil>(base+0x1782F50);
        setStencilMask=reinterpret_cast<SetStencilMask>(base+0x1782F90);
    }
    constexpr unsigned char activeArray[]={0x48,0x63,0x81,0x80,0,0,0,0x85,0xC0,0x7E,0x3D,0x48,0x8D,0x50,0xFF,0x0F,0x57,0xC0,0x48,0x8B,0x41,0x78};
    constexpr unsigned char activeWeight[]={0x0F,0x2E,0x80,0x50,0x01,0,0,0x75,0x0F};
    mountedMontagesReady=!std::memcmp(reinterpret_cast<void*>(base+0x166AE50),activeArray,sizeof(activeArray))&&
        !std::memcmp(reinterpret_cast<void*>(base+0x166AE83),activeWeight,sizeof(activeWeight));
}
inline bool Ready(){return construct&&setRelativeTransform;}

inline void Update(const local_players::Context& owner,const Id& mountedActor,const Id& mountedBody){
    if(!Ready()||owner.index<0||owner.index>1)return;
    auto& player=players[owner.index];
    const auto& rt=runtime::Get();
    const bool enabled=rt.showStrifeStowedBlades.load();
    const unsigned off=melee_effect::Mask(rt.strifeBladeVisibility.load());
    const bool mounted=mountedActor&&mountedBody;
    const bool attacking=mounted&&mountedMontagesReady&&off!=3&&MountedAttack(mountedBody.address);
    if(!enabled&&!attacking&&!player.actor)return;

    const bool foot=owner.kind==local_players::PoseKind::Foot&&owner.pawn==owner.character;
    uintptr_t cls=0,bodyOwner=0,skeleton=0;
    Id actorId=mountedActor,bodyId=mountedBody,skeletonId{};
    const bool ownedFoot=enabled&&foot&&owner.localPlayer&&
        Read(owner.character,0x10,cls)&&Name(cls,"Strife_Blueprint_C")&&
        pistol_appearance::Controlled(owner.pawn)&&Capture(owner.pawn,actorId);
    uintptr_t body=bodyId.address;
    const bool localBody=mounted||(ownedFoot&&Read(owner.pawn,0x380,body)&&
        Read(body,0xE8,bodyOwner)&&bodyOwner==owner.pawn&&Capture(body,bodyId));
    const bool valid=localBody&&(enabled||mountedMontagesReady)&&
        Read(body,0x5A0,skeleton)&&Capture(skeleton,skeletonId);
    if(!valid||player.actor!=actorId||player.body!=bodyId||player.skeleton!=skeletonId){
        for(auto& copy:player.stowed)Discard(player.actor,copy);
        for(auto& copy:player.held)Discard(player.actor,copy);
        player={};
        if(!valid)return;
        player.actor=actorId;
        player.body=bodyId;
        player.skeleton=skeletonId;
        FindSockets(player,mounted);
    }

    uintptr_t sourceBody=0;
    Read(owner.character,0x380,sourceBody);
    for(unsigned hand=0;hand<2;++hand){
        const bool inHand=attacking&&player.hands[hand];
        const bool stowed=enabled&&player.pelvis;
        uintptr_t weapon=0,mesh=0;
        unsigned char flags=0;
        const bool show=sourceBody&&!(off&(1u<<hand))&&(inHand||stowed)&&
            pistol_appearance::Component(base,owner.character,sourceBody,hand+6,weapon,flags)&&
            (mounted||(flags&0x40))&&Read(weapon,0x5B0,mesh)&&BladeAsset(mesh,hand);
        if(!show)mesh=0;
        const bool drawn=PresentBlade(player,player.held[hand],owner.character,sourceBody,hand,
            inHand?mesh:0,player.hands[hand],true,mounted);
        PresentBlade(player,player.stowed[hand],owner.character,sourceBody,hand,
            stowed&&!drawn?mesh:0,player.pelvis,false,mounted);
    }
}
}
