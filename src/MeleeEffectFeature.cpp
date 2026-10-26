#include "MeleeEffectFeature.h"
#include "MeleeEffectPolicy.h"
#include "LocalPlayerOwnership.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include "NativePropertyField.h"
#include <MinHook.h>
#include <atomic>
#include <cstdio>

namespace dg::melee_effect {
namespace {
using SpawnFn=void*(*)(void*,void*,const Transform*,bool,unsigned char);
using AttachFn=void*(*)(void*,void*,native_bone::Name,const native_bone::Point*,const native_bone::Point*,const native_bone::Point*,int,bool,unsigned char);
using RootFn=Transform*(*)(void*,Transform*,native_bone::Name,int);
SpawnFn g_spawn=nullptr;native_bone::LocationFn g_location=nullptr;
AttachFn g_attach=nullptr;RootFn g_root=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};unsigned g_changes=0;
struct Plan {Transform root{};uintptr_t world=0,asset=0,mesh=0;native_bone::Point right{},left{},offset{};native_bone::Name socket{};Side side=Side::None;unsigned mask=0,attached=0;};
thread_local const Plan* g_plan=nullptr;
struct Lava {uintptr_t asset=0,mesh=0,weapon=0;native_bone::Name from{},to{};int side=-1;};
thread_local const Lava* g_lava=nullptr;
template<class T> bool Read(uintptr_t p,size_t offset,T& value){native_objects::MemoryReader reader;return reader.Read(p,offset,value);}
template<size_t N> bool Is(uintptr_t p,const char (&text)[N]){return native_objects::NameIs(g_base,p,text);}
void Changed(const Plan& plan){
    if(g_log&&++g_changes<=12){char line[160]{};sprintf_s(line,"Strife bare blade FX V1.85: side=%u mask=%u attached=%u hand origin, scale=0.4; damage trace unchanged; EXPERIMENTAL",unsigned(plan.side),plan.mask,unsigned(plan.attached));g_log(line);}
}
void* HookSpawn(void* world,void* asset,const Transform* transform,bool autoDestroy,unsigned char pooling){
    Transform source{},out{};const auto* plan=g_plan;
    if(g_ready&&plan&&!plan->attached&&plan->world==reinterpret_cast<uintptr_t>(world)&&plan->asset==reinterpret_cast<uintptr_t>(asset)&&
       plan->mask==Mask(runtime::Get().strifeBladeVisibility.load())&&
       Read(reinterpret_cast<uintptr_t>(transform),0,source)&&Adjust(source,plan->side,plan->mask,plan->right,plan->left,out)){
        Changed(*plan);
        return g_spawn?g_spawn(world,asset,&out,autoDestroy,pooling):nullptr;
    }
    return g_spawn?g_spawn(world,asset,transform,autoDestroy,pooling):nullptr;
}
void* HookAttach(void* asset,void* mesh,native_bone::Name socket,const native_bone::Point* location,
                 const native_bone::Point* rotation,const native_bone::Point* scale,int attachType,bool autoDestroy,unsigned char pooling){
    if(const auto* lava=g_lava;g_ready&&lava&&lava->asset==reinterpret_cast<uintptr_t>(asset)&&lava->mesh==reinterpret_cast<uintptr_t>(mesh)&&
       socket.index==lava->from.index&&socket.number==lava->from.number&&attachType==0){
        // Only the flash uses the actual weapon muzzle. Projectiles, aim,
        // actor/socket transforms and the shared asset remain untouched.
        const native_bone::Point zero{};
        if((lava->side==0?runtime::Get().pistolRightModel.load():runtime::Get().pistolLeftModel.load())==3)return nullptr;
        return g_attach?g_attach(asset,reinterpret_cast<void*>(lava->weapon),lava->to,&zero,&zero,scale,attachType,autoDestroy,pooling):nullptr;
    }
    const auto* plan=g_plan;native_bone::Point position{},size{},outPosition{},outSize{};
    if(g_ready&&plan&&plan->attached&&plan->asset==reinterpret_cast<uintptr_t>(asset)&&plan->mesh==reinterpret_cast<uintptr_t>(mesh)&&
       plan->socket.index==socket.index&&plan->socket.number==socket.number&&attachType==0&&
       plan->mask==Mask(runtime::Get().strifeBladeVisibility.load())&&
       Read(reinterpret_cast<uintptr_t>(location),0,position)&&Read(reinterpret_cast<uintptr_t>(scale),0,size)&&
       Near(position.x,plan->offset.x)&&Near(position.y,plan->offset.y)&&Near(position.z,plan->offset.z)&&
       Attached(plan->root,position,size,plan->side,plan->mask,plan->right,plan->left,outPosition,outSize)){
        Changed(*plan);
        return g_attach?g_attach(asset,mesh,socket,&outPosition,rotation,&outSize,attachType,autoDestroy,pooling):nullptr;
    }
    return g_attach?g_attach(asset,mesh,socket,location,rotation,scale,attachType,autoDestroy,pooling):nullptr;
}
bool Prepare(void* notify,void* liveMesh,void* animation,Plan& plan){
    if(!g_ready||!native_tps::OnGameThread()||!Mask(runtime::Get().strifeBladeVisibility.load()))return false;
    const auto object=reinterpret_cast<uintptr_t>(notify),mesh=reinterpret_cast<uintptr_t>(liveMesh),anim=reinterpret_cast<uintptr_t>(animation);
    uintptr_t cls=0,outer=0,actor=0,package=0;native_bone::Point offset{};unsigned char attached=0;
    if(!Read(object,0x10,cls)||!Is(cls,"AnimNotify_PlayParticleEffect")||!Read(object,0x20,outer)||outer!=anim||
       !Read(object,0x38,plan.asset)||!Read(object,0x40,offset)||!Read(object,0x80,attached)||
       !Read(plan.asset,0x10,cls)||!Is(cls,"ParticleSystem")||!Read(plan.asset,0x20,package)||
       !((Is(plan.asset,"VFX_strife_meleeSlice")&&Is(package,"/Game/Particles/VFX_strife_meleeSlice"))||
         (Is(plan.asset,"VFX_strife_meleeSlice_strong")&&Is(package,"/Game/Particles/VFX_strife_meleeSlice_strong"))||
         (Is(plan.asset,"VFX_strife_stabSingle")&&Is(package,"/Game/Particles/VFX_strife_stabSingle")))||
       !Read(mesh,0xE8,actor)||!Read(actor,0x10,cls)||!Is(cls,"Strife_Blueprint_C"))return false;
    local_players::Context c{};uintptr_t current=0,controller=0,pawn=0;unsigned char role=0,type=0;
    if(!local_players::ResolveActor(reinterpret_cast<void*>(actor),c)||c.index<0||c.index>1||!c.localPlayer||
       c.kind!=local_players::PoseKind::Foot||c.pawn!=actor||c.character!=actor||!c.world||
       !Read(actor,0x380,current)||current!=mesh||!Read(actor,0x118,role)||role!=3||
       !Read(actor,0xC18,type)||type!=1||!Read(actor,0x348,controller)||controller!=c.controller||
       !Read(controller,0x350,pawn)||pawn!=actor)return false;
    unsigned sequence=0;
    if(Is(anim,"Strife_Atk_Melee_01"))sequence=1;
    else if(Is(anim,"Strife_Atk_Melee_02"))sequence=2;
    else if(Is(anim,"Strife_Atk_Melee_03")||Is(anim,"Strife_Atk_Melee_03_b"))sequence=3;
    else if(Is(anim,"Strife_Air_Atk_Melee_01"))sequence=4;
    else if(Is(anim,"Strife_Air_Atk_Melee_02"))sequence=5;
    else if(Is(anim,"Strife_Atk_Melee_Kick_Launch_Level1_Montage")||Is(anim,"Strife_Atk_Melee_Kick_Launch_Level2_Montage"))sequence=6;
    plan.side=Classify(sequence,offset);plan.mask=Mask(runtime::Get().strifeBladeVisibility.load());
    // The secondary launcher has one common authored arc, not two blade
    // emitters. With one blade OFF, its compact origin is that bare hand.
    if(sequence==6&&plan.side==Side::Shared&&plan.mask!=3)plan.side=plan.mask==1?Side::Right:Side::Left;
    if(!Selected(plan.side,plan.mask))return false;
    native_objects::MemoryReader reader;
    if(!native_bone::Location(reader,g_base,g_location,mesh,actor,43,"Bone_Strife_Hand_R",plan.right)||
       !native_bone::Location(reader,g_base,g_location,mesh,actor,20,"Bone_Strife_Hand_L",plan.left))return false;
    plan.world=c.world;plan.mesh=mesh;plan.offset=offset;plan.attached=(attached&1)!=0;
    if(plan.attached){
        uintptr_t vt=0,entry=0;
        if(!g_root||!Read(object,0x84,plan.socket)||
           !native_objects::NameValueIs(g_base,plan.socket.index,plan.socket.number,"Bone_Strife_Root")||
           !Read(mesh,0,vt)||!Read(vt,0x3F0,entry)||entry!=g_base+0x17A2590||
           g_root(liveMesh,&plan.root,plan.socket,0)!=&plan.root)return false;
    }
    return true;
}
bool PrepareLava(void* notify,void* liveMesh,void* animation,Lava& out){
    if(!g_ready||!native_tps::OnGameThread())return false;
    const auto n=reinterpret_cast<uintptr_t>(notify),mesh=reinterpret_cast<uintptr_t>(liveMesh),anim=reinterpret_cast<uintptr_t>(animation);
    uintptr_t cls=0,outer=0,actor=0,package=0,actual=0,controller=0,pawn=0;unsigned char attached=0,role=0;
    if(!Read(n,0x10,cls)||!Is(cls,"AnimNotify_PlayParticleEffect")||!Read(n,0x20,outer)||outer!=anim||
       !Is(anim,"Strife_LavaShot_Fire_Montage")||!Read(anim,0x20,package)||!Is(package,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_LavaShot_Fire_Montage")||
       !Read(n,0x38,out.asset)||!Is(out.asset,"P_LavaShot_MuzzleFlash")||!Read(n,0x80,attached)||!(attached&1)||
       !Read(n,0x84,out.from)||!native_objects::NameValueIs(g_base,out.from.index,out.from.number,"StrifeMuzzleFlash")||
       !Read(mesh,0xE8,actor)||!Read(actor,0x10,cls)||!Is(cls,"Strife_Blueprint_C"))return false;
    local_players::Context c{};
    if(!local_players::ResolveActor(reinterpret_cast<void*>(actor),c)||c.index<0||c.index>1||!c.localPlayer||!c.world||
       c.kind!=local_players::PoseKind::Foot||c.character!=actor||c.pawn!=actor||!Read(actor,0x380,actual)||actual!=mesh||
       !Read(actor,0x118,role)||role!=3||!Read(actor,0x348,controller)||controller!=c.controller||!Read(controller,0x350,pawn)||pawn!=actor)return false;
    // Pair each flash with the following native SpawnProjectile event. Its
    // WeaponMesh and MuzzleSocket are authoritative, including rebinds/skins.
    native_bone::Array events{};float flashTime=-1,best=.01f;int side=-1;
    if(!Read(anim,0x80,events)||!events.data||events.count<1||events.count>events.capacity||events.capacity>128)return false;
    for(int i=0;i<events.count;++i){uintptr_t eventNotify=0;const auto at=events.data+size_t(i)*184;
        if(Read(at,0x48,eventNotify)&&eventNotify==n&&!Read(at,0x24,flashTime))return false;}
    if(flashTime<0)return false;
    for(int i=0;i<events.count;++i){uintptr_t eventNotify=0;float time=0;const auto at=events.data+size_t(i)*184;
        if(!Read(at,0x48,eventNotify)||!Read(eventNotify,0x10,cls)||!Is(cls,"AnimNotify_SpawnProjectile")||!Read(at,0x24,time))continue;
        const float delta=time-flashTime;if(!std::isfinite(delta)||delta<0||delta>=best)continue;
        int weaponAt=0,socketAt=0;native_bone::Name weapon{},socket{};
        if(!native_objects::PropertyField(g_base,cls,"WeaponMesh","NameProperty",8,weaponAt)||!native_objects::PropertyField(g_base,cls,"MuzzleSocket","NameProperty",8,socketAt)||
           !Read(eventNotify,size_t(weaponAt),weapon)||!Read(eventNotify,size_t(socketAt),socket)||!native_objects::NameValueIs(g_base,socket.index,socket.number,"MuzzleFlash"))return false;
        if(native_objects::NameValueIs(g_base,weapon.index,weapon.number,"RightPistol"))side=0;
        else if(native_objects::NameValueIs(g_base,weapon.index,weapon.number,"LeftPistol"))side=1;else return false;
        best=delta;out.to=socket;
    }
    if(side<0||!Read(actor,0x1040+size_t(side)*8,out.weapon)||!Read(out.weapon,0xE8,actual)||actual!=actor||
       !Read(out.weapon,0x108,actual)||actual!=mesh||!Read(out.weapon,0x10,cls)||!Is(cls,"StaticMeshComponent"))return false;
    out.mesh=mesh;out.side=side;return true;
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char sig[]={0x48,0x8B,0xC4,0x48,0x89,0x58,0x10,0x48,0x89,0x68,0x18,0x48,0x89,0x70,0x20};
    constexpr unsigned char attachSig[]={0x40,0x55,0x53,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x57,0x48,0x8D,0x6C,0x24,0xF9};
    constexpr unsigned char rootSig[]={0x48,0x8B,0xC4,0x48,0x89,0x58,0x18,0x55,0x56,0x57,0x41,0x56,0x41,0x57};
    if(!native_bone::Boundary(g_base)||std::memcmp(reinterpret_cast<void*>(g_base+0x189A140),sig,sizeof(sig))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x189A510),attachSig,sizeof(attachSig))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x17A2590),rootSig,sizeof(rootSig)))return;
    auto* target=reinterpret_cast<void*>(g_base+0x189A140);
    auto* attachTarget=reinterpret_cast<void*>(g_base+0x189A510);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookSpawn),reinterpret_cast<void**>(&g_spawn))!=MH_OK)return;
    if(MH_CreateHook(attachTarget,reinterpret_cast<void*>(&HookAttach),reinterpret_cast<void**>(&g_attach))!=MH_OK){MH_RemoveHook(target);g_spawn=nullptr;return;}
    if(MH_EnableHook(target)!=MH_OK||MH_EnableHook(attachTarget)!=MH_OK){
        MH_DisableHook(target);MH_DisableHook(attachTarget);MH_RemoveHook(target);MH_RemoveHook(attachTarget);g_spawn=nullptr;g_attach=nullptr;return;
    }
    g_location=reinterpret_cast<native_bone::LocationFn>(g_base+0x17A2380);g_root=reinterpret_cast<RootFn>(g_base+0x17A2590);g_ready=true;
    if(log)log("Strife bare blade FX V1.85 READY: scoped attached/detached slash/stab only, native damage and notify retained; EXPERIMENTAL");
}
void Particle(void* notify,void* mesh,void* animation,NotifyFn original){
    if(!original)return;
    Plan plan{};Lava lava{};const bool muzzle=PrepareLava(notify,mesh,animation,lava),prepared=!muzzle&&Prepare(notify,mesh,animation,plan);
    struct Guard {const Plan* previous;const Lava* lava;~Guard(){g_plan=previous;g_lava=lava;}} guard{g_plan,g_lava};
    g_plan=prepared?&plan:nullptr;g_lava=muzzle?&lava:nullptr;original(notify,mesh,animation);
}
void Shutdown(){g_ready=false;}
}
