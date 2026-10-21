#include "PistolVisualFeature.h"
#include "PistolVisualPolicy.h"
#include "NativeMemoryReader.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include "DashCombatFeature.h"
#include <MinHook.h>
#include <atomic>
#include <cstdio>

namespace dg::pistol_visual {
namespace {
// 1BCD990 passes exactly seven arguments to 1BBFD50, including this.
// Its final tail call 1BCDEC0 returns bool. All arguments/return are retained.
using InitFn=bool(*)(void*,void*,void*,int,void*,void*,int);
using HiddenFn=void(*)(void*,bool,unsigned char);
InitFn g_init=nullptr;HiddenFn g_hiddenFn=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};
SRWLOCK g_lock=SRWLOCK_INIT;thread_local bool g_settingHidden=false;
struct Snapshot {
    native_objects::Identity actor{},mesh{},skeleton{};
    uint32_t generation=0;uint64_t tick=0,idleSince=0;
    bool gameplay=false,capture=false,lower=false;
};
struct Player {Snapshot snapshot{};ArmPose pose{};uint64_t renderTick=0;};
Player g_players[2]{};std::atomic_bool g_poseReady[2]{};
struct Hidden {native_objects::Identity component{},actor{};bool nativeHidden=false;};
Hidden g_hidden[2][2]{};std::atomic_uint g_captures{0};
template<class T> bool Read(uintptr_t p,size_t offset,T& value){native_objects::MemoryReader r;return r.Read(p,offset,value);}
template<size_t N> bool NameIs(uintptr_t p,const char (&text)[N]){return native_objects::NameIs(g_base,p,text);}
template<size_t N> bool NameValueIs(int index,int number,const char (&text)[N]){return native_objects::NameValueIs(g_base,index,number,text);}
bool Live(const native_objects::Identity& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
struct Array {uintptr_t data=0;int count=0,capacity=0;};
bool ArrayOf(uintptr_t at,int count,Array& out){return Read(at,0,out)&&out.data&&out.count==count&&out.capacity>=count&&out.capacity<=1024;}
bool Writable(uintptr_t address,size_t bytes){
    MEMORY_BASIC_INFORMATION m{};
    if(!address||!VirtualQuery(reinterpret_cast<void*>(address),&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_GUARD|PAGE_NOACCESS)))return false;
    const auto access=m.Protect&0xFF;
    if(access!=PAGE_READWRITE&&access!=PAGE_WRITECOPY&&access!=PAGE_EXECUTE_READWRITE&&access!=PAGE_EXECUTE_WRITECOPY)return false;
    const auto offset=address-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&bytes<=m.RegionSize-offset;
}
bool Bone(uintptr_t info,unsigned index,int parent){
    struct Info {int name,number,parent;} bone{};
    if(!Read(info,size_t(index)*12,bone)||bone.number||bone.parent!=parent)return false;
    // Fixed supported hierarchy. Do not interpret a renamed/modded skeleton
    // by numeric bone indices alone.
    if(index==17)return native_objects::NameValueIs(g_base,bone.name,0,"Bone_Strife_Clavicle_L");
    if(index==18)return native_objects::NameValueIs(g_base,bone.name,0,"Bone_Strife_UpperArm_L");
    if(index==20)return native_objects::NameValueIs(g_base,bone.name,0,"Bone_Strife_Hand_L");
    if(index==40)return native_objects::NameValueIs(g_base,bone.name,0,"Bone_Strife_Clavicle_R");
    if(index==41)return native_objects::NameValueIs(g_base,bone.name,0,"Bone_Strife_UpperArm_R");
    if(index==43)return native_objects::NameValueIs(g_base,bone.name,0,"Bone_Strife_Hand_R");
    return false;
}
bool Skeleton(uintptr_t mesh,uintptr_t skeleton){
    Array info{};uintptr_t current=0;
    if(!ArrayOf(skeleton+0x190,BoneCount,info)||!Bone(info.data,17,14)||!Bone(info.data,18,17)||
       !Bone(info.data,20,19)||!Bone(info.data,40,14)||!Bone(info.data,41,40)||!Bone(info.data,43,42))return false;
    constexpr int parents[ArmBones]={-1,0,1,2,3,4,2,6,7,2,9,10,2,12,13,2,15,16,2,2,1,0};
    for(unsigned side=0;side<2;++side)for(unsigned j=0;j<ArmBones;++j){
        struct Info {int name,number,parent;} bone{};
        if(!Read(info.data,size_t(Upper[side]+j)*12,bone)||bone.parent!=
           (j?int(Upper[side])+parents[j]:int(Clavicle[side])))return false;
    }
    return Read(mesh,0x5A0,current)&&current==skeleton;
}
unsigned ChoiceMask(){const auto& rt=runtime::Get();return (rt.pistolRightModel.load()==3?1u:0u)|(rt.pistolLeftModel.load()==3?2u:0u);}
bool Fresh(const Snapshot& s,uint64_t now){return s.gameplay&&now>=s.tick&&now-s.tick<=250;}
bool Clavicles(uintptr_t mesh,std::array<Matrix,2>& matrices){
    int selector=-1;Array cs{};struct Weak {int index,serial;} master{};
    if(!Read(mesh,0x5A8,master)||(master.index>=0&&master.serial>0))return false;
    if(!Read(mesh,0x5F4,selector)||(selector!=0&&selector!=1)||!ArrayOf(mesh+0x5B0+size_t(selector)*16,BoneCount,cs))return false;
    // The native socket path uses exactly this same transform buffer. It is
    // read only. The dynamic render packet receives the changed matrices.
    for(unsigned side=0;side<2;++side){Transform t{};if(!Read(cs.data,size_t(Clavicle[side])*48,t)||!ToMatrix(t,matrices[side]))return false;}
    int after=-1;Array check{};
    return Read(mesh,0x5F4,after)&&after==selector&&Read(mesh+0x5B0+size_t(selector)*16,0,check)&&
        check.data==cs.data&&check.count==cs.count&&check.capacity==cs.capacity;
}
bool Packet(uintptr_t data,Array& current,Array& previous){
    uintptr_t vt=0;
    if(!Read(data,0,vt)||vt!=g_base+0x2DC88A8||!ArrayOf(data+0x10,BoneCount,current)||!Read(data,0x20,previous)||
       previous.count<0||previous.count>previous.capacity||previous.capacity>1024||
       (previous.count!=0&&previous.count!=int(BoneCount)))return false;
    if(previous.count&&(!previous.data||previous.data==current.data))return false;
    return Writable(current.data,sizeof(Matrices))&&(!previous.count||Writable(previous.data,sizeof(Matrices)));
}
bool ProcessPacket(uintptr_t data,uintptr_t component,uintptr_t renderData,int lod,uint64_t now){
    if(!Ready()||lod<0||lod>8)return false;
    bool tracked=false;const auto mask=ChoiceMask();AcquireSRWLockShared(&g_lock);
    for(const auto& p:g_players)if(p.snapshot.mesh.address==component&&Fresh(p.snapshot,now)&&(!p.pose.ready||mask))tracked=true;
    ReleaseSRWLockShared(&g_lock);if(!tracked)return false;
    uintptr_t actor=0,skeleton=0,renderObject=0,vt=0,expectedData=0;
    if(!Read(component,0xE8,actor)||!Read(component,0x5A0,skeleton)||!Read(component,0x6C0,renderObject)||
       !Read(renderObject,0,vt)||vt!=g_base+0x2DC8930||!Read(renderObject,0x40,expectedData)||expectedData!=renderData)return false;
    Array current{},previous{};if(!Packet(data,current,previous))return false;
    Matrices matrices{},old{};std::array<Matrix,2> clavicles{};
    if(!Read(current.data,0,matrices)||(previous.count&&!Read(previous.data,0,old))||!Clavicles(component,clavicles))return false;
    bool changed=false,captured=false,handled=false;
    AcquireSRWLockExclusive(&g_lock);
    for(unsigned i=0;i<2;++i){
        auto& p=g_players[i];const auto& s=p.snapshot;
        // component is a LIVE native argument. Cached UObject addresses are
        // compared only after object-table identities are validated.
        if(s.mesh.address!=component||s.actor.address!=actor||s.skeleton.address!=skeleton||!Fresh(s,now)||
           !Live(s.mesh)||!Live(s.actor)||!Live(s.skeleton))continue;
        handled=true;p.renderTick=now;
        if(s.capture&&!p.pose.ready&&s.idleSince&&now>=s.idleSince&&now-s.idleSince>=750){
            captured=p.pose.Capture(matrices,clavicles);g_poseReady[i]=p.pose.ready;
        }else if(s.lower){
            changed=p.pose.Apply(ChoiceMask(),clavicles,now,matrices,previous.count?&old:nullptr);
        }else p.pose.previousMask=0;
        break;
    }
    ReleaseSRWLockExclusive(&g_lock);
    if(changed){
        std::memcpy(reinterpret_cast<void*>(current.data),&matrices,sizeof(matrices));
        if(previous.count)std::memcpy(reinterpret_cast<void*>(previous.data),&old,sizeof(old));
    }
    if(captured&&g_log&&++g_captures<=4)g_log("Pistol OFF: native sheathed arm render pose captured; component-space socket transforms remain untouched; EXPERIMENTAL");
    return handled;
}
bool HookInit(void* data,void* component,void* renderData,int lod,void* morphs,void* weights,int previousMode){
    const bool result=g_init?g_init(data,component,renderData,lod,morphs,weights,previousMode):false;
    // Original initialization has completed; no render command has yet been
    // enqueued by 1BCD990. Never modify a packet after its submission.
    const bool handled=result&&ProcessPacket(reinterpret_cast<uintptr_t>(data),reinterpret_cast<uintptr_t>(component),
        reinterpret_cast<uintptr_t>(renderData),lod,GetTickCount64());
    if(!handled){
        AcquireSRWLockExclusive(&g_lock);
        for(auto& p:g_players)if(p.snapshot.mesh.address==reinterpret_cast<uintptr_t>(component))p.renderTick=0;
        ReleaseSRWLockExclusive(&g_lock);
    }
    return result;
}
void HookHidden(void* component,bool hidden,unsigned char propagation){
    if(!g_settingHidden&&native_tps::OnGameThread()){
        const auto address=reinterpret_cast<uintptr_t>(component);
        for(auto& player:g_hidden)for(auto& saved:player)
            if(saved.component.address==address&&Live(saved.component))saved.nativeHidden=hidden;
    }
    if(g_hiddenFn)g_hiddenFn(component,hidden,propagation);
}
void SetHidden(uintptr_t component,bool hidden){
    if(!g_hiddenFn)return;
    g_settingHidden=true;g_hiddenFn(reinterpret_cast<void*>(component),hidden,1);g_settingHidden=false;
}
void Restore(Hidden& saved){
    uintptr_t owner=0;unsigned char flags=0;
    if(saved.component&&Live(saved.component)&&Live(saved.actor)&&Read(saved.component.address,0xE8,owner)&&owner==saved.actor.address&&
       Read(saved.component.address,0x1CC,flags)&&bool(flags&0x40)!=saved.nativeHidden)
        SetHidden(saved.component.address,saved.nativeHidden);
    saved={};
}
void Hide(unsigned player,unsigned side,const Snapshot& snapshot){
    auto& saved=g_hidden[player][side];uintptr_t component=0,owner=0,parent=0,vt=0;unsigned char flags=0;
    if(!Read(snapshot.actor.address,0x1040+side*8,component)||!Read(component,0,vt)||vt!=g_base+0x2A575C8||
       !Read(component,0xE8,owner)||owner!=snapshot.actor.address||!Read(component,0x108,parent)||parent!=snapshot.mesh.address||
       !Read(component,0x1CC,flags)){Restore(saved);return;}
    native_objects::Identity id{};
    if(!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(component),id))return;
    if(saved.component&&saved.component!=id)Restore(saved);
    if(!saved.component)saved={id,snapshot.actor,bool(flags&0x40)};
    if(!(flags&0x40))SetHidden(component,true);
}
bool Normal(const local_players::Context& c,Snapshot& s){
    uintptr_t cls=0,controller=0,pawn=0,resource=0;unsigned char type=0,role=0;float health=0;
    if(c.index<0||c.index>1||!c.localPlayer||!c.character||c.character!=c.pawn||c.kind!=local_players::PoseKind::Foot||
       !Read(c.character,0x10,cls)||!NameIs(cls,"Strife_Blueprint_C")||!Read(c.character,0xC18,type)||type!=1||
       !Read(c.character,0x118,role)||role!=3||!Read(c.character,0x348,controller)||controller!=c.controller||
       !Read(controller,0x350,pawn)||pawn!=c.pawn||!Read(c.character,0xEA0,resource)||!Read(resource,0x37C,health)||
       !std::isfinite(health)||health<=0||!Read(c.character,0x380,pawn)||!Read(pawn,0xE8,controller)||controller!=c.character||
       !Read(pawn,0x5A0,resource))return false;
    if(!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(c.character),s.actor)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(pawn),s.mesh)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(resource),s.skeleton))return false;
    // Schema check is bounded to the six anchor bones, and is cached by the
    // full skeleton identity on later frames.
    bool known=false;AcquireSRWLockShared(&g_lock);
    known=g_players[c.index].snapshot.skeleton==s.skeleton;ReleaseSRWLockShared(&g_lock);
    if(!known&&!Skeleton(pawn,resource))return false;
    return true;
}
bool Idle(uintptr_t actor){
    unsigned char states[2]{},mode=0;uintptr_t component=0,movement=0,reciprocal=0;
    int count=-1,capacity=0;std::array<float,3> velocity{};
    return Read(actor,0xD15,states)&&!states[0]&&!states[1]&&Read(actor,0xC70,component)&&
        Read(component,0xE8,reciprocal)&&reciprocal==actor&&Read(component,0x100,count)&&Read(component,0x104,capacity)&&
        count==0&&capacity>=0&&capacity<=64&&Read(actor,0x388,movement)&&Read(movement,0x1B0,mode)&&mode==1&&
        Read(movement,0xD8,velocity)&&std::isfinite(velocity[0])&&std::isfinite(velocity[1])&&std::isfinite(velocity[2])&&
        std::abs(velocity[0])+std::abs(velocity[1])+std::abs(velocity[2])<1;
}
bool Ranged(uintptr_t actor){
    unsigned char states[2]{};uintptr_t component=0,owner=0;Array active{};
    if(!Read(actor,0xD15,states)||states[0]||states[1]!=1||!Read(actor,0xC70,component)||
       !Read(component,0xE8,owner)||owner!=actor||!Read(component,0xF8,active)||active.count<0||
       active.count>active.capacity||active.capacity>64||(!active.data&&active.count))return false;
    if(!active.count)return true;
    uintptr_t registry=0;Array weapons{};
    if(!Read(actor,0xC78,registry)||!Read(registry,0xE8,owner)||owner!=actor||!Read(registry,0x108,weapons)||
       !weapons.data||weapons.count<6||weapons.count>weapons.capacity||weapons.capacity>32)return false;
    uintptr_t slots[3]{};
    if(!Read(weapons.data,2*8,slots[0])||!Read(weapons.data,3*8,slots[1])||!Read(weapons.data,5*8,slots[2]))return false;
    // Only the player's native two ammo weapons and general pistol AIM may
    // replace arm presentation. Dash, chests, gear and every unknown ability
    // keep their own animations even if ranged-idle is still set.
    for(int i=0;i<active.count;++i){
        uintptr_t ability=0,instigator=0,character=0,weapon=0;
        if(!Read(active.data,size_t(i)*8,ability)||!Read(ability,0x48,instigator)||instigator!=actor||
           !Read(ability,0x1B0,character)||character!=actor||!Read(ability,0x50,weapon)||!weapon||
           (weapon!=slots[0]&&weapon!=slots[1]&&weapon!=slots[2])||!Read(weapon,0xE8,owner)||owner!=actor)return false;
    }
    return true;
}
EffectSide Side(int index,int number){
    if(NameValueIs(index,number,"VFX_Weapon_Gun_R")||NameValueIs(index,number,"Gun_R_Socket")||
       NameValueIs(index,number,"Bone_Strife_Fing_Index2_R"))return EffectSide::Right;
    if(NameValueIs(index,number,"VFX_Weapon_Gun_L")||NameValueIs(index,number,"Gun_L_Socket")||
       NameValueIs(index,number,"Bone_Strife_Fing_Index2_L"))return EffectSide::Left;
    if(NameValueIs(index,number,"VFX_Root_Beam")||NameValueIs(index,number,"StrifeMuzzleFlash"))return EffectSide::Shared;
    return EffectSide::Unknown;
}
bool Muzzle(uintptr_t asset){
    return NameIs(asset,"VFX_strife_muzzleFlash")||NameIs(asset,"VFX_strife_bulletCasings")||
        NameIs(asset,"P_Strife_Charge_Muzzle_Lvl1")||NameIs(asset,"P_Strife_GravShot_Muzzle_Juiced_01")||
        NameIs(asset,"P_Strife_GravShot_Muzzle_Lvl1")||NameIs(asset,"P_LavaShot_MuzzleFlash")||
        NameIs(asset,"P_LavaShot_Juiced_Flamethrower_BeamSpawn")||NameIs(asset,"P_NatureShotPellet_Firing")||
        NameIs(asset,"P_NatureShotPellet_FiringJuiced");
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if(!g_base)return;
    constexpr unsigned char initSig[]={0x48,0x89,0x5C,0x24,0x18,0x48,0x89,0x6C,0x24,0x20,0x48,0x89,0x54,0x24,0x10,0x56};
    constexpr unsigned char hiddenSig[]={0x40,0x55,0x56,0x57,0x41,0x56,0x41,0x57,0x48,0x8D,0x6C,0x24,0xC9,0x48,0x81,0xEC};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x1BBFD50),initSig,sizeof(initSig))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x17AE540),hiddenSig,sizeof(hiddenSig)))return;
    void* init=reinterpret_cast<void*>(g_base+0x1BBFD50);void* hidden=reinterpret_cast<void*>(g_base+0x17AE540);
    bool createdInit=MH_CreateHook(init,reinterpret_cast<void*>(&HookInit),reinterpret_cast<void**>(&g_init))==MH_OK;
    bool createdHidden=createdInit&&MH_CreateHook(hidden,reinterpret_cast<void*>(&HookHidden),reinterpret_cast<void**>(&g_hiddenFn))==MH_OK;
    if(!createdHidden||MH_EnableHook(init)!=MH_OK||MH_EnableHook(hidden)!=MH_OK){
        if(createdInit){MH_DisableHook(init);MH_RemoveHook(init);}if(createdHidden){MH_DisableHook(hidden);MH_RemoveHook(hidden);}
        g_init=nullptr;g_hiddenFn=nullptr;return;
    }
    g_ready=true;
    if(log)log("Pistol OFF candidate READY: owned normal Strife, render-packet arm pose, native hidden flag and bounded muzzle notifications; shared effects kept with one pistol; EXPERIMENTAL");
}
void OnLivePlayer(const local_players::Context& owner,uint32_t generation,bool focused){
    if(!native_tps::OnGameThread()||owner.index<0||owner.index>1)return;
    const unsigned index=unsigned(owner.index);Snapshot next{};const auto now=GetTickCount64();
    if(Ready()&&local_players::Matches(index,owner.character,generation)&&Normal(owner,next)){
        next.generation=generation;next.tick=now;
        next.gameplay=focused&&!native_tps::IsWorldPaused(reinterpret_cast<void*>(owner.pawn))&&!native_tps::CinematicActive(index)&&!native_tps::RadialActive(index);
        next.capture=next.gameplay&&Idle(owner.pawn);next.lower=next.gameplay&&Ranged(owner.pawn);
    }
    AcquireSRWLockExclusive(&g_lock);auto& p=g_players[index];
    const auto& before=p.snapshot;
    const bool same=before.actor==next.actor&&before.mesh==next.mesh&&before.skeleton==next.skeleton&&before.generation==next.generation;
    if(!same){p.pose={};p.renderTick=0;g_poseReady[index]=false;}
    next.idleSince=next.capture?(same&&before.capture&&before.idleSince?before.idleSince:now):0;
    p.snapshot=next;const unsigned mask=next.gameplay&&p.pose.ready&&p.renderTick&&now>=p.renderTick&&now-p.renderTick<=250?ChoiceMask():0;
    ReleaseSRWLockExclusive(&g_lock);
    for(unsigned side=0;side<2;++side){
        if(mask&(1u<<side))Hide(index,side,next);else Restore(g_hidden[index][side]);
    }
}
bool SkipParticle(void* notify,void* mesh){
    if(!Ready()||!ChoiceMask()||!native_tps::OnGameThread())return false;
    uintptr_t cls=0,asset=0;struct Name {int index,number;} socket{};
    const auto object=reinterpret_cast<uintptr_t>(notify),component=reinterpret_cast<uintptr_t>(mesh);
    if(!Read(object,0x10,cls)||!NameIs(cls,"AnimNotify_PlayParticleEffect")||!Read(object,0x38,asset)||
       !Muzzle(asset)||!Read(object,0x84,socket))return false;
    const auto side=Side(socket.index,socket.number);if(side==EffectSide::Unknown)return false;
    bool skip=false;const auto now=GetTickCount64();AcquireSRWLockShared(&g_lock);
    for(const auto& p:g_players){
        const auto& s=p.snapshot;
        if(s.mesh.address==component&&p.pose.ready&&p.renderTick&&now>=p.renderTick&&now-p.renderTick<=250&&Fresh(s,now)&&Live(s.mesh)&&Live(s.actor)){
            skip=HideEffect(side,ChoiceMask());break;
        }
    }
    ReleaseSRWLockShared(&g_lock);return skip;
}
bool Ready(){return g_ready.load(std::memory_order_acquire)&&dash_combat::PresentationReady();}
bool PoseReady(unsigned index){return index<2&&Ready()&&g_poseReady[index].load();}
void Shutdown(){g_ready=false;for(auto& ready:g_poseReady)ready=false;}
}
