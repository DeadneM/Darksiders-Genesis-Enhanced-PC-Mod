#include "PistolVisualFeature.h"
#include "PistolAnimationPolicy.h"
#include "NativeMemoryReader.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include "DashCombatFeature.h"
#include "ForeignRampageFeature.h"
#include "GeyserStaticPose.h"
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
using EvaluateFn=void(*)(void*,PoseContext*);
using InitContextFn=void(*)(PoseContext*);
using SampleFn=void(*)(void*,const NativeArray*,CompactPose*,Curve*);
EvaluateFn g_evaluate=nullptr;InitContextFn g_initContext=nullptr;SampleFn g_sample=nullptr;
std::atomic_bool g_animationReady{false};
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};
SRWLOCK g_lock=SRWLOCK_INIT;thread_local bool g_settingHidden=false;
struct Snapshot {
    native_objects::Identity actor{},mesh{},skeleton{};
    uint32_t generation=0;uint64_t tick=0;
    float worldTime=0,speed=0,nativePhase=0;
    bool gameplay=false,lower=false;
};
struct AnimationSource {
    native_objects::Identity anim{},blend{},skeleton{};
    std::array<native_objects::Identity,3> sequences{};
    AnimationRig rig{};std::array<float,3> lengths{};
};
struct Player {
    Snapshot snapshot{};AnimationSource source{};AnimationClock clock{};
    ArmPose pose{};uint64_t sampleTick=0;
};
Player g_players[2]{};
struct Hidden {native_objects::Identity component{},actor{};bool nativeHidden=false;};
Hidden g_hidden[2][8]{};std::atomic_uint g_captures{0};
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
    if(index==14)return native_objects::NameValueIs(g_base,bone.name,0,"Bone_Strife_Spine2");
    if(index==78)return native_objects::NameValueIs(g_base,bone.name,0,"Bone_Strife_Holster_R");
    if(index==79)return native_objects::NameValueIs(g_base,bone.name,0,"Bone_Strife_Holster_L");
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
    if(!ArrayOf(skeleton+0x190,BoneCount,info)||!Bone(info.data,14,13)||!Bone(info.data,78,1)||!Bone(info.data,79,1)||
       !Bone(info.data,17,14)||!Bone(info.data,18,17)||
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
unsigned BladeMask(){const auto choice=runtime::Get().strifeBladeVisibility.load();return choice==1?2u:choice==2?1u:choice==3?3u:0u;}
bool Fresh(const Snapshot& s,uint64_t now){return s.gameplay&&now>=s.tick&&now-s.tick<=250;}
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
    for(const auto& p:g_players)if(mask&&p.snapshot.mesh.address==component&&Fresh(p.snapshot,now))tracked=true;
    ReleaseSRWLockShared(&g_lock);if(!tracked)return false;
    uintptr_t actor=0,skeleton=0,renderObject=0,vt=0,expectedData=0;
    if(!Read(component,0xE8,actor)||!Read(component,0x5A0,skeleton)||!Read(component,0x6C0,renderObject)||
       !Read(renderObject,0,vt)||vt!=g_base+0x2DC8930||!Read(renderObject,0x40,expectedData)||expectedData!=renderData)return false;
    Array current{},previous{};if(!Packet(data,current,previous))return false;
    Matrices matrices{},old{};
    if(!Read(current.data,0,matrices)||(previous.count&&!Read(previous.data,0,old)))return false;
    // A_i = inverse(bind_i) * componentPose_i. Using the spine matrix from
    // this same render packet cancels its bind matrix between capture/apply:
    // A_i_rest * inverse(A_spine_rest) * A_spine_now.
    // Never combine a submitted pose with another component-space buffer.
    const std::array<Matrix,2> anchors{matrices[PoseAnchor[0]],matrices[PoseAnchor[1]]};
    bool changed=false,handled=false;
    AcquireSRWLockExclusive(&g_lock);
    for(unsigned i=0;i<2;++i){
        auto& p=g_players[i];const auto& s=p.snapshot;
        // component is a LIVE native argument. Cached UObject addresses are
        // compared only after object-table identities are validated.
        if(s.mesh.address!=component||s.actor.address!=actor||s.skeleton.address!=skeleton||!Fresh(s,now)||
           !Live(s.mesh)||!Live(s.actor)||!Live(s.skeleton))continue;
        handled=true;
        const bool sampled=p.pose.ready&&p.sampleTick&&now>=p.sampleTick&&now-p.sampleTick<=100;
        if(s.lower&&sampled)changed=p.pose.Apply(mask,anchors,now,matrices,previous.count?&old:nullptr);
        else p.pose.previousMask=0;
        if(mask)changed=HideHolsters(mask,matrices,previous.count?&old:nullptr)||changed;
        break;
    }
    ReleaseSRWLockExclusive(&g_lock);
    if(changed){
        std::memcpy(reinterpret_cast<void*>(current.data),&matrices,sizeof(matrices));
        if(previous.count)std::memcpy(reinterpret_cast<void*>(previous.data),&old,sizeof(old));
    }
    return handled;
}
bool HookInit(void* data,void* component,void* renderData,int lod,void* morphs,void* weights,int previousMode){
    const bool result=g_init?g_init(data,component,renderData,lod,morphs,weights,previousMode):false;
    // Original initialization has completed; no render command has yet been
    // enqueued by 1BCD990. Never modify a packet after its submission.
    if(result)ProcessPacket(reinterpret_cast<uintptr_t>(data),reinterpret_cast<uintptr_t>(component),
        reinterpret_cast<uintptr_t>(renderData),lod,GetTickCount64());
    return result;
}
void HookHidden(void* component,bool hidden,unsigned char propagation){
    bool keepHidden=false;
    if(!g_settingHidden&&Ready()&&native_tps::OnGameThread()){
        const auto address=reinterpret_cast<uintptr_t>(component);const unsigned mask=ChoiceMask();
        for(auto& player:g_hidden)for(unsigned slot=0;slot<8;++slot){
            auto& saved=player[slot];uintptr_t owner=0;
            if(saved.component.address==address&&Live(saved.component)&&Live(saved.actor)&&
               Read(address,0xE8,owner)&&owner==saved.actor.address){
                saved.nativeHidden=hidden;keepHidden=((slot<6?mask:BladeMask())&(1u<<(slot%2)))!=0;
            }
        }
    }
    // Preserve the native call, propagation and child visibility first. Then
    // retain OFF on this exact component before any render packet is submitted.
    if(g_hiddenFn)g_hiddenFn(component,hidden,propagation);
    if(keepHidden&&!hidden&&g_hiddenFn){
        g_settingHidden=true;g_hiddenFn(component,true,1);g_settingHidden=false;
    }
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
void HideAt(unsigned player,unsigned slot,const Snapshot& snapshot,size_t offset){
    auto& saved=g_hidden[player][slot];uintptr_t component=0,owner=0,parent=0,vt=0;unsigned char flags=0;
    const bool blade=slot>=6;
    if(!Read(snapshot.actor.address,offset,component)||!Read(component,0,vt)||vt!=g_base+(blade?0x2724250:0x2A575C8)||
       !Read(component,0xE8,owner)||owner!=snapshot.actor.address||!Read(component,0x108,parent)||parent!=snapshot.mesh.address||
       !Read(component,0x1CC,flags)||
       (blade&&!(slot==6?NameIs(component,"Dagger_R"):NameIs(component,"Dagger_L")))){Restore(saved);return;}
    native_objects::Identity id{};
    if(!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(component),id))return;
    if(saved.component&&saved.component!=id)Restore(saved);
    if(!saved.component)saved={id,snapshot.actor,bool(flags&0x40)};
    if(!(flags&0x40))SetHidden(component,true);
}
void Hide(unsigned player,unsigned side,const Snapshot& snapshot){HideAt(player,side,snapshot,0x1040+side*8);}
bool Mounted(const local_players::Context& c,Snapshot& s){
    uintptr_t cls=0,controller=0,pawn=0,mesh=0,owner=0;unsigned char type=0,role=0;
    if(c.kind!=local_players::PoseKind::Horse||!c.localPlayer||!c.character||!c.pawn||c.character==c.pawn||
       !Read(c.character,0x10,cls)||!NameIs(cls,"Strife_Blueprint_C")||!Read(c.character,0xC18,type)||type!=1||
       !Read(c.pawn,0x10,cls)||!NameIs(cls,"Despair_Blueprint_C")||!Read(c.pawn,0x118,role)||role!=3||
       !Read(c.pawn,0x348,controller)||controller!=c.controller||!Read(controller,0x350,pawn)||pawn!=c.pawn||
       !Read(c.pawn,0x380,mesh)||!Read(mesh,0xE8,owner)||owner!=c.pawn)return false;
    return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(c.pawn),s.actor)&&
        native_objects::CaptureLive(g_base,reinterpret_cast<void*>(mesh),s.mesh);
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
    // Schema check is bounded to the verified anchors and holster leaves, and is cached by the
    // full skeleton identity on later frames.
    bool known=false;AcquireSRWLockShared(&g_lock);
    known=g_players[c.index].snapshot.skeleton==s.skeleton;ReleaseSRWLockShared(&g_lock);
    if(!known&&!Skeleton(pawn,resource))return false;
    return true;
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
// Build the immutable animation contract on the game thread only. UObject
// serial creation follows the existing native weak-reference assignment.
bool Source(const Snapshot& s,AnimationSource& source){
    uintptr_t anim=0,cls=0,actor=0,vt=0,blend=0,skeleton=0,overridePose=0;
    struct Weak {int index,serial;} master{};
    if(!g_animationReady||!Read(s.mesh.address,0,vt)||vt!=g_base+0x274DF98||
       !Read(s.mesh.address,0x718,anim)||!Read(anim,0x10,cls)||!NameIs(cls,"StrifeAnim_Blueprint_C")||
       !Read(anim,0x4C38,actor)||actor!=s.actor.address||!Read(anim,0x318,vt)||vt!=g_base+0x2A77628||
       !Read(anim,0x49C8,vt)||vt!=g_base+0x2A76860||!Read(anim,0x4A10,blend)||!NameIs(blend,"Strife_Locomotion")||
       !Read(blend,0x10,cls)||!NameIs(cls,"BlendSpace1D")||!Read(blend,0x38,skeleton)||
       !Read(s.mesh.address,0x5A8,master)||master.serial>0||!Read(s.mesh.address,0x630,overridePose)||overridePose)return false;
    // Identity checks precede every use of a cached UObject. The same contract
    // is reused until mesh/actor/animation changes, not reconstructed per bone.
    bool cached=source.anim.address==anim&&source.blend.address==blend&&source.skeleton.address==skeleton&&
        Live(source.anim)&&Live(source.blend)&&Live(source.skeleton);
    for(const auto& sequence:source.sequences)cached=cached&&Live(sequence);
    if(cached)return true;
    AnimationSource next{};Array info{},inverse{},samples{};int perBone=-1;
    if(!ArrayOf(s.skeleton.address+0x190,BoneCount,info)||!ArrayOf(s.skeleton.address+0x2C8,BoneCount,inverse)||
       !ArrayOf(blend+0xC8,3,samples)||!Read(blend,0xB8,perBone)||perBone!=0||
       !Read(inverse.data,0,next.rig.inverseBind)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(anim),next.anim)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(blend),next.blend)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(skeleton),next.skeleton))return false;
    for(unsigned i=0;i<BoneCount;++i){
        struct Info {int name,number,parent;} bone{};
        if(!Read(info.data,size_t(i)*12,bone)||bone.parent<(i?0:-1)||bone.parent>=int(i)||!Finite(next.rig.inverseBind[i]))return false;
        next.rig.parents[i]=bone.parent;
    }
    constexpr float positions[3]={1,0,1.0f/3};
    for(unsigned i=0;i<3;++i){
        struct Sample {uintptr_t sequence;float position[3],rate;} sample{};
        if(!Read(samples.data,size_t(i)*24,sample)||std::abs(sample.position[0]-positions[i])>0.001f||
           sample.position[1]!=0||sample.position[2]!=0||sample.rate!=1||
           !(i==0?NameIs(sample.sequence,"Strife_Run"):i==1?NameIs(sample.sequence,"Strife_Idle"):NameIs(sample.sequence,"Strife_Walk"))||
           !Read(sample.sequence,0x10,cls)||!NameIs(cls,"AnimSequence")||
           !Read(sample.sequence,0x90,next.lengths[i])||!std::isfinite(next.lengths[i])||next.lengths[i]<=0.01f||next.lengths[i]>60||
           !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(sample.sequence),next.sequences[i]))return false;
    }
    source=next;return true;
}
bool SampleAnimation(uintptr_t node,PoseContext* output,uint64_t now){
    if(!Ready()||!g_animationReady||!ChoiceMask()||!output||!g_initContext||!g_sample)return false;
    // One reusable private buffer per animation worker. No per-frame heap
    // allocation or large stack frame; recursive evaluation fails open.
    struct Scratch {AnimationSource source{};std::array<LocalTransform,BoneCount> local{};Matrices skin{};ArmPose pose{};bool busy=false;};
    thread_local Scratch scratch;
    if(scratch.busy)return false;
    struct Scope {bool& flag;explicit Scope(bool& b):flag(b){flag=true;}~Scope(){flag=false;}} scope(scratch.busy);
    Snapshot snapshot{};auto& source=scratch.source;AnimationClock clock{};unsigned index=2;
    AcquireSRWLockShared(&g_lock);
    for(unsigned i=0;i<2;++i){const auto& p=g_players[i];
        if(p.source.anim&&node==p.source.anim.address+0x318&&Fresh(p.snapshot,now)&&p.snapshot.lower){
            snapshot=p.snapshot;source=p.source;clock=p.clock;index=i;break;
        }
    }
    ReleaseSRWLockShared(&g_lock);if(index==2)return false;
    if(!Live(snapshot.actor)||!Live(snapshot.mesh)||!Live(snapshot.skeleton)||!Live(source.anim)||!Live(source.blend)||!Live(source.skeleton))return false;
    for(const auto& seq:source.sequences)if(!Live(seq))return false;
    uintptr_t proxy=0,reciprocal=0,mesh=0,skeleton=0,vt=0;PoseContext native{};Array required{},assetSamples{};int perBone=-1;
    if(!Read(node,0,vt)||vt!=g_base+0x2A77628||!Read(reinterpret_cast<uintptr_t>(output),0,native)||native.additive||
       !Read(source.anim.address,0x240,proxy)||!proxy||proxy!=native.proxy||
       !Read(proxy,0xA0,reciprocal)||reciprocal!=source.anim.address||!Read(proxy,0xB8,mesh)||mesh!=snapshot.mesh.address||
       !Read(proxy,0xB0,skeleton)||skeleton!=source.skeleton.address||native.pose.container!=proxy+0x450||
       !ArrayOf(proxy+0x450,BoneCount,required)||native.pose.transforms.count!=int(BoneCount)||
       !Read(source.blend.address,0xB8,perBone)||perBone!=0||!ArrayOf(source.blend.address+0xC8,3,assetSamples))return false;
    for(unsigned i=0;i<3;++i){uintptr_t sequence=0;
        if(!Read(assetSamples.data,size_t(i)*24,sequence)||sequence!=source.sequences[i].address)return false;
    }
    // This candidate supports the observed full 80-bone compact mapping.
    // Unknown LOD/retargeting layouts keep native presentation.
    std::array<uint16_t,BoneCount> indices{};
    if(!Read(required.data,0,indices))return false;
    for(unsigned i=0;i<BoneCount;++i)if(indices[i]!=i)return false;
    if(!clock.Step(snapshot.worldTime,snapshot.speed,snapshot.nativePhase,source.lengths))return false;
    std::array<BlendSample,2> samples{};const auto count=clock.Samples(source.lengths,samples);
    const NativeArray cache{reinterpret_cast<uintptr_t>(samples.data()),int(count),int(count)};
    PoseContext sampled{};sampled.proxy=proxy;
    // Exactly the same MemStack context initialization used by the native
    // layered-blend evaluator. Call only inside this live evaluation scope;
    // no MemStack pointer escapes and no animation update/notifies are run.
    g_initContext(&sampled);
    if(sampled.pose.container!=native.pose.container||sampled.pose.transforms.count!=int(BoneCount)||
       sampled.pose.transforms.capacity<int(BoneCount)||sampled.pose.transforms.capacity>1024||!sampled.pose.transforms.data||
       (sampled.pose.transforms.data&15u))return false;
    g_sample(reinterpret_cast<void*>(source.blend.address),&cache,&sampled.pose,&sampled.curve);
    auto& local=scratch.local;auto& skin=scratch.skin;auto& pose=scratch.pose;
    if(sampled.pose.transforms.count!=int(BoneCount)||!Read(sampled.pose.transforms.data,0,local)||
       !source.rig.Skin(local,skin)||!pose.Capture(skin,{skin[14],skin[14]}))return false;
    bool published=false;AcquireSRWLockExclusive(&g_lock);auto& p=g_players[index];
    if(p.snapshot.actor==snapshot.actor&&p.snapshot.mesh==snapshot.mesh&&p.snapshot.skeleton==snapshot.skeleton&&
       p.snapshot.generation==snapshot.generation&&p.source.anim==source.anim&&p.snapshot.lower&&Fresh(p.snapshot,now)){
        // Retain the previous *rendered* frame across changing animation data.
        p.pose.relative=pose.relative;p.pose.clavicle=pose.clavicle;p.pose.ready=true;
        p.clock=clock;p.sampleTick=now;published=true;
    }
    ReleaseSRWLockExclusive(&g_lock);
    if(published&&g_log&&++g_captures<=4)g_log("Pistol OFF: V1.81 native unarmed locomotion sampled into private render arms; melee/powers and sockets native; EXPERIMENTAL");
    return published;
}
void HookEvaluate(void* node,PoseContext* output){
    // All actors retain their original evaluation. Only the exact owned
    // Strife root may supply an additional private unarmed presentation pose.
    if(g_evaluate)g_evaluate(node,output);
    if(!foreign_rampage::Evaluate(node,output)&&!geyser_static::Evaluate(node,output))SampleAnimation(reinterpret_cast<uintptr_t>(node),output,GetTickCount64());
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
    // Optional animation boundary: failure never disables model hiding,
    // mounted presentation, or any already validated gameplay hook.
    constexpr unsigned char rootSig[]={0x48,0x83,0xC1,0x10,0xE9,0x67,0xA0,0x2C,0x00};
    constexpr unsigned char contextSig[]={0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x48,0x89,0x7C,0x24,0x18};
    constexpr unsigned char sampleSig[]={0x48,0x8B,0xC4,0x55,0x53,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57};
    if(!std::memcmp(reinterpret_cast<void*>(g_base+0x13C0F50),rootSig,sizeof(rootSig))&&
       !std::memcmp(reinterpret_cast<void*>(g_base+0x1694890),contextSig,sizeof(contextSig))&&
       !std::memcmp(reinterpret_cast<void*>(g_base+0x16B4C30),sampleSig,sizeof(sampleSig))){
        auto* root=reinterpret_cast<void*>(g_base+0x13C0F50);
        if(MH_CreateHook(root,reinterpret_cast<void*>(&HookEvaluate),reinterpret_cast<void**>(&g_evaluate))==MH_OK){
            g_initContext=reinterpret_cast<InitContextFn>(g_base+0x1694890);g_sample=reinterpret_cast<SampleFn>(g_base+0x16B4C30);
            if(MH_EnableHook(root)==MH_OK)g_animationReady=true;
            else {MH_RemoveHook(root);g_evaluate=nullptr;g_initContext=nullptr;g_sample=nullptr;}
        }
    }
    g_ready=true;
    if(log)log(g_animationReady?"Pistol OFF animated boundary READY (V1.81 experimental)":"Pistol OFF animated boundary unavailable: native arms retained");
    if(log)log("Pistol OFF model path READY; animated unarmed arms separately signature/owner gated; native melee/powers kept; EXPERIMENTAL");
}
void OnLivePlayer(const local_players::Context& owner,uint32_t generation,bool focused){
    if(!native_tps::OnGameThread()||owner.index<0||owner.index>1)return;
    const unsigned index=unsigned(owner.index);Snapshot next{};const auto now=GetTickCount64();
    if(Ready()&&local_players::Matches(index,owner.character,generation)&&Normal(owner,next)){
        next.generation=generation;next.tick=now;
        next.gameplay=focused&&!native_tps::CinematicActive(index);
        next.lower=next.gameplay&&!native_tps::IsWorldPaused(reinterpret_cast<void*>(owner.pawn))&&
            !native_tps::RadialActive(index)&&Ranged(owner.pawn);
    }
    AnimationSource source{};
    AcquireSRWLockShared(&g_lock);
    const auto& before=g_players[index].snapshot;
    const bool same=before.actor==next.actor&&before.mesh==next.mesh&&before.skeleton==next.skeleton&&before.generation==next.generation;
    if(same)source=g_players[index].source;
    ReleaseSRWLockShared(&g_lock);
    if(next.lower&&ChoiceMask()){
        float speed=0,maxSpeed=0;
        if(!Source(next,source)||!Read(source.anim.address,0x298,speed)||!Read(source.anim.address,0x294,maxSpeed)||
           !std::isfinite(speed)||!std::isfinite(maxSpeed)||speed<0||maxSpeed<1||
           !Read(source.anim.address,0x49E4,next.nativePhase)||!std::isfinite(next.nativePhase)||
           !Read(owner.world,0x528,next.worldTime)||!std::isfinite(next.worldTime))next.lower=false;
        else next.speed=speed/maxSpeed;
    }
    AcquireSRWLockExclusive(&g_lock);auto& p=g_players[index];
    if(!same||p.source.anim!=source.anim||!next.lower||!ChoiceMask()){
        p.pose={};p.clock={};p.sampleTick=0;
    }
    p.snapshot=next;p.source=source;
    const unsigned mask=next.gameplay?ChoiceMask():0;
    ReleaseSRWLockExclusive(&g_lock);
    Snapshot mounted{};
    const bool horse=Ready()&&focused&&local_players::Matches(index,owner.character,generation)&&Mounted(owner,mounted)&&
        !native_tps::CinematicActive(index);
    // Mounted hand/stowed components are separate exact owned meshes. Never
    // hide the horse/rider parent or alter mounted animation/action state.
    const unsigned mountedMask=horse?ChoiceMask():0;
    const unsigned bladeMask=next.gameplay?BladeMask():0;
    for(unsigned slot=0;slot<8;++slot){
        const unsigned side=slot%2;
        if(slot<2&&(mask&(1u<<side)))Hide(index,side,next);
        else if(slot>=2&&slot<6&&(mountedMask&(1u<<side)))HideAt(index,slot,mounted,(slot<4?0xBA0:0xBF8)+side*8);
        else if(slot>=6&&(bladeMask&(1u<<side)))HideAt(index,slot,next,0x1010+side*8);
        else Restore(g_hidden[index][slot]);
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
        if(s.mesh.address==component&&Fresh(s,now)&&Live(s.mesh)&&Live(s.actor)){
            skip=HideEffect(side,ChoiceMask());break;
        }
    }
    ReleaseSRWLockShared(&g_lock);return skip;
}
bool Ready(){return g_ready.load(std::memory_order_acquire)&&dash_combat::PresentationReady();}
bool PoseReady(unsigned index){
    if(index>=2||!Ready())return false;
    const auto now=GetTickCount64();AcquireSRWLockShared(&g_lock);const auto& p=g_players[index];
    const bool ready=p.snapshot.lower&&Fresh(p.snapshot,now)&&p.pose.ready&&p.sampleTick&&now>=p.sampleTick&&now-p.sampleTick<=100;
    ReleaseSRWLockShared(&g_lock);return ready;
}
void Shutdown(){g_ready=false;g_animationReady=false;}
}
