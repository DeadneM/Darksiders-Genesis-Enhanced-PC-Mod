#include "ForeignWrathFeature.h"
#include "ForeignMontage.h"
#include "StoneSkinVisual.h"
#include "GeyserStaticPose.h"
#include "ForeignCaltropsFeature.h"
#include "ForeignPowerFeature.h"
#include "ForeignBeamFeature.h"
#include "ForeignRampageFeature.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <atomic>
#include <algorithm>
#include <cmath>
#include <cstdio>

namespace dg::foreign_wrath {
namespace {
using Getter=uintptr_t*(*)(void*,uintptr_t*,Name);
using Play=void(*)(void*,void*,float,Name);
using Create=void*(*)(void*,Name,float,float,float,int,float,float);
using Finish=void(*)(void*,unsigned char);
using Playing=bool(*)(void*);
Getter g_getter=nullptr;Play g_play=nullptr;Finish g_finish=nullptr;Playing g_playing=nullptr;
Create g_create=nullptr;
uintptr_t g_base=0,g_world=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false},g_requested{false};
native_objects::Identity g_geyser{},g_geyserActors[2]{};
native_objects::Identity g_stone{};std::atomic_bool g_stoneFailed{false};unsigned g_stoneEvents=0;
std::atomic_bool g_geyserReady{false},g_geyserFailed{false};
native_objects::Identity g_sequenceClass{},g_nativeClass{},g_materialClass{};
int g_cursor=0,g_count=0;bool g_complete=false;uint64_t g_scanTick=0;unsigned g_events=0;
struct Array {uintptr_t data=0;int count=0,capacity=0;};
struct Override {void* actor=nullptr;uintptr_t abilityClass=0;Name id{};};
thread_local const Override* g_override=nullptr;
template<class T> bool Read(uintptr_t p,size_t offset,T& value){native_objects::MemoryReader r;return r.Read(p,offset,value);}
template<size_t N> bool Is(uintptr_t object,const char (&name)[N]){return native_objects::NameIs(g_base,object,name);}
native_objects::Identity* GeyserActor(int index){
    if(index==0)return &g_geyserActors[0];
    if(index==1)return &g_geyserActors[1];
    return nullptr;
}
bool Alive(const native_objects::Identity& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}

bool GeyserClass(uintptr_t object){
    uintptr_t kind=0,package=0;
    return Read(object,0x10,kind)&&Is(kind,"BlueprintGeneratedClass")&&Read(object,0x20,package)&&
        Is(object,"MayhemAbility_Wrath_BladeGeyser_Blueprint_C")&&Is(package,"/Game/Blueprints/Abilities/MayhemAbility_Wrath_BladeGeyser_Blueprint");
}
bool StoneClass(uintptr_t object){
    uintptr_t kind=0,package=0;
    return Read(object,0x10,kind)&&Is(kind,"BlueprintGeneratedClass")&&Read(object,0x20,package)&&
        Is(object,"MayhemAbility_StoneSkin_Blueprint_C")&&Is(package,"/Game/Blueprints/Abilities/Stoneskin/MayhemAbility_StoneSkin_Blueprint");
}
int Kind(uintptr_t cls){
    if(cls==g_geyser.address&&Alive(g_geyser)&&GeyserClass(cls))return 4;
    return cls==g_stone.address&&Alive(g_stone)&&StoneClass(cls)?6:0;
}
bool OwnedStrife(void* live,local_players::Context& c){
    const auto actor=reinterpret_cast<uintptr_t>(live);uintptr_t cls=0,controller=0,pawn=0;unsigned char role=0,type=0;
    return native_tps::OnGameThread()&&local_players::ResolveActor(live,c)&&c.index>=0&&c.index<2&&c.localPlayer&&c.world&&
        c.kind==local_players::PoseKind::Foot&&c.character==actor&&c.pawn==actor&&
        Read(actor,0x10,cls)&&Is(cls,"Strife_Blueprint_C")&&Read(actor,0x118,role)&&role==3&&
        Read(actor,0xC18,type)&&type==1&&Read(actor,0x348,controller)&&controller==c.controller&&
        Read(controller,0x350,pawn)&&pawn==actor;
}
struct Animation {uintptr_t sequence=0,skeleton=0;Name slot{};float length=0;};
bool Source(uintptr_t actor,Animation& out){
    uintptr_t mesh=0,owner=0,anim=0,cls=0,vt=0,blend=0;Array samples{};
    struct Sample {uintptr_t sequence;float position[3],rate;} idle{};
    float rate=0;
    return Read(actor,0x380,mesh)&&Read(mesh,0xE8,owner)&&owner==actor&&Read(mesh,0x718,anim)&&
        Read(anim,0x10,cls)&&Is(cls,"StrifeAnim_Blueprint_C")&&Read(anim,0x4C38,owner)&&owner==actor&&
        Read(anim,0x49C8,vt)&&vt==g_base+0x2A76860&&Read(anim,0x4A10,blend)&&Is(blend,"Strife_Locomotion")&&
        Read(blend,0x10,cls)&&Is(cls,"BlendSpace1D")&&
        Read(blend,0xC8,samples)&&samples.data&&samples.count==3&&samples.capacity>=3&&samples.capacity<=16&&
        Read(samples.data,24,idle)&&idle.sequence&&idle.position[0]==0&&idle.position[1]==0&&idle.position[2]==0&&idle.rate==1&&
        Is(idle.sequence,"Strife_Idle")&&Read(idle.sequence,0x10,cls)&&Is(cls,"AnimSequence")&&
        Read(idle.sequence,0x38,out.skeleton)&&out.skeleton&&Read(anim,0x28,owner)&&owner==out.skeleton&&
        Read(idle.sequence,0x90,out.length)&&std::isfinite(out.length)&&out.length>0.1f&&out.length<10&&
        Read(idle.sequence,0x94,rate)&&rate==1&&Read(anim,0x3740,out.slot)&&
        native_objects::NameValueIs(g_base,out.slot.index,out.slot.number,"TPS")&&(out.sequence=idle.sequence)!=0;
}
void Scan(uintptr_t generatedClass){
    const bool beamChanged=foreign_beam::InvalidateAssets(),rampageChanged=foreign_rampage::InvalidateAssets();
    const bool caltropsChanged=foreign_caltrops::InvalidateAssets(),cloneChanged=foreign_power::InvalidateAssets();
    if(geyser_static::InvalidateAssets()||stone_visual::InvalidateAssets()||beamChanged||rampageChanged||caltropsChanged||cloneChanged){g_cursor=0;g_complete=false;}
    if(g_geyser&&!Alive(g_geyser)){g_geyser={};g_geyserReady=false;g_cursor=0;g_complete=false;}
    if(g_stone&&!Alive(g_stone)){g_stone={};g_cursor=0;g_complete=false;}
    if(g_geyser){g_geyserReady=true;if(stone_visual::AssetsReady()&&geyser_static::AssetsReady()&&foreign_montage::GesturesReady()&&foreign_beam::AssetsReady()&&g_stone&&foreign_rampage::AssetsReady()&&foreign_caltrops::AssetsReady()&&foreign_power::AssetsReady())return;}
    uintptr_t chunks=0;int count=0;
    if(!Read(g_base,0x3959680,chunks)||!Read(g_base,0x3959694,count)||count<=0||count>2000000)return;
    if(count!=g_count){if(count<g_count)g_cursor=0;g_count=count;g_complete=false;}
    if(g_complete)return;
    // One shared discovery pass instead of three independent 512-object scans.
    // At most 256 objects, checking a 1 ms high-resolution budget every 16.
    LARGE_INTEGER started{},frequency{};
    const bool timed=QueryPerformanceFrequency(&frequency)&&frequency.QuadPart>=1000&&QueryPerformanceCounter(&started);
    if(g_sequenceClass&&!Alive(g_sequenceClass))g_sequenceClass={};
    if(g_nativeClass&&!Alive(g_nativeClass))g_nativeClass={};
    if(g_materialClass&&!Alive(g_materialClass))g_materialClass={};
    for(unsigned n=0;n<256&&g_cursor<count;++n,++g_cursor){
        if(n&&!(n%16)&&timed){LARGE_INTEGER now{};
            if(QueryPerformanceCounter(&now)&&now.QuadPart-started.QuadPart>=frequency.QuadPart/1000)break;}
        uintptr_t chunk=0,object=0,cls=0;uint32_t flags=0;
        if(!Read(chunks,size_t(g_cursor/65536)*8,chunk)||!Read(chunk,size_t(g_cursor%65536)*24,object)||!object||
           !Read(chunk,size_t(g_cursor%65536)*24+8,flags)||(flags&0x30000000u)||!Read(object,0x10,cls))continue;
        if(!g_sequenceClass&&Is(cls,"AnimSequence"))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(cls),g_sequenceClass);
        if(!g_nativeClass&&Is(cls,"Class"))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(cls),g_nativeClass);
        if(!g_materialClass&&Is(cls,"MaterialInstanceConstant"))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(cls),g_materialClass);
        const unsigned kind=cls==generatedClass?1u:cls==g_nativeClass.address?2u:cls==g_sequenceClass.address?3u:0u;
        if(cls==g_materialClass.address)stone_visual::ObserveObject(object);
        if(!kind)continue;
        foreign_caltrops::ObserveObject(object,kind);foreign_power::ObserveObject(object,kind);
        if(kind==1||kind==3){foreign_beam::ObserveObject(object);foreign_rampage::ObserveObject(object);}
        if(kind==3){foreign_montage::ObserveSequence(object);geyser_static::ObserveSequence(object);}
        if(kind!=1)continue;
        if(!g_geyser&&GeyserClass(object))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(object),g_geyser);
        if(!g_stone&&StoneClass(object))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(object),g_stone);
    }
    if(g_cursor>=count)g_complete=true;if(g_geyser)g_geyserReady=true;
}
uintptr_t* HookGetter(void* actor,uintptr_t* result,Name id){
    const auto* request=g_override;
    if(g_ready&&request&&request->actor==actor&&request->id==id&&result&&native_tps::OnGameThread()&&(Kind(request->abilityClass)||foreign_caltrops::IsClass(request->abilityClass)||foreign_power::IsClass(request->abilityClass)||foreign_beam::IsClass(request->abilityClass)||foreign_rampage::IsClass(request->abilityClass))){
        *result=request->abilityClass;return result;
    }
    return g_getter?g_getter(actor,result,id):result;
}
int ForeignCast(void* live,local_players::Context& c){
    const auto ability=reinterpret_cast<uintptr_t>(live);
    uintptr_t cls=0,actor=0,character=0;unsigned char state=0,role=0;
    uint32_t objectFlags=0;
    if(!(g_ready&&g_finish&&g_playing&&native_tps::OnGameThread()&&Read(ability,8,objectFlags)&&!(objectFlags&0x30u)&&
        Read(ability,0x10,cls)&&Read(ability,0x48,actor)&&Read(ability,0x1B0,character)&&character==actor&&
        Read(ability,0xD8,state)&&(state==1||state==2)&&Read(actor,0x118,role)&&role==3&&OwnedStrife(reinterpret_cast<void*>(actor),c)))return 0;
    return Kind(cls);
}
bool ForeignLaunch(void* live,local_players::Context& c){
    const auto ability=reinterpret_cast<uintptr_t>(live);uintptr_t cls=0,package=0,actor=0,character=0;unsigned char state=0;
    if(!g_ready||!g_finish||!native_tps::OnGameThread()||!Read(ability,0x10,cls)||!Is(cls,"MayhemAbility_War_LaunchAttack_Blueprint_C")||
       !Read(cls,0x20,package)||!Is(package,"/Game/Blueprints/Abilities/WarMelee/MayhemAbility_War_LaunchAttack_Blueprint")||
       !Read(ability,0x48,actor)||!Read(ability,0x1B0,character)||actor!=character||!Read(ability,0xD8,state)||(state!=1&&state!=2)||
       !OwnedStrife(reinterpret_cast<void*>(actor),c))return false;
    const auto* recent=GeyserActor(c.index);return recent&&recent->address==actor&&Alive(*recent);
}
void StonePlay(void* ability,void* montage,float rate,Name section,const local_players::Context& c){
    const auto original=reinterpret_cast<uintptr_t>(montage),p=reinterpret_cast<uintptr_t>(ability);
    uintptr_t cls=0,package=0,skeleton=0,owner=0;float duration=0,scale=0,length=0;Animation source{};
    native_objects::Identity abi{},pawn{};
    if(!native_objects::CaptureLive(g_base,ability,abi)||!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(c.character),pawn))return;
    bool played=false;
    if(g_create&&!g_stoneFailed&&std::isfinite(rate)&&rate>0&&rate<=10&&!section.index&&!section.number&&Source(c.character,source)&&
       Read(original,0x10,cls)&&Is(cls,"AnimMontage")&&Read(original,0x20,package)&&
       ((Is(original,"War_Atk_Stoneskin_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Atk_Stoneskin_Montage"))||
        (Is(original,"War_Atk_Stoneskin_Molten_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Atk_Stoneskin_Molten_Montage")))&&
       Read(original,0x90,duration)&&std::isfinite(duration)&&duration>.1f&&duration<10&&
       Read(original,0x94,scale)&&std::isfinite(scale)&&scale>.1f&&scale<10){
        duration/=scale;length=source.length;
        const auto sequence=foreign_montage::CastingSequence(source.sequence,source.skeleton,length);
        auto* replacement=g_create(reinterpret_cast<void*>(sequence),source.slot,.1f,.18f,1.f,1,-1.f,0.f);
        if(!Alive(abi)||!Alive(pawn)||!Read(p,0x48,owner)||owner!=c.character)return;
        const auto copy=reinterpret_cast<uintptr_t>(replacement);
        if(copy&&copy!=original&&Read(copy,0x10,cls)&&Is(cls,"AnimMontage")&&Read(copy,0x38,skeleton)&&skeleton==source.skeleton&&
           Read(copy,0x90,length)&&std::isfinite(length)&&length>.1f&&length<10){
            // Activation is short; the already-applied native status keeps its
            // own lifetime. Same Aether gesture as V1.112, no new return clip.
            const float activation=std::min(duration,1.f);
            const float adjusted=rate*length/activation;
            if(std::isfinite(adjusted)&&adjusted>.01f&&adjusted<100){
                // Native status/cost, own gesture; body coating is observed
                // independently from the status list, never from this clip.
                foreign_montage::CopyLaterSounds(original,replacement);
                g_play(ability,replacement,adjusted,section);
                if(!Alive(abi)||!Alive(pawn)||!Read(p,0x48,owner)||owner!=c.character)return;
                uintptr_t instance=0;played=Read(p,0x1D8,instance)&&instance&&g_playing(reinterpret_cast<void*>(instance));
            }
        }
    }
    // The native Blueprint applies the status and cost before PlayMontage.
    // War and Stinger stay native. A separate owned-material observer follows
    // the actual Strife status lifetime; no borrowed components or timer.
    if(!played){g_stoneFailed=true;g_finish(ability,0);}
    else foreign_montage::StartSounds(original,ability);
    if(g_log&&g_stoneEvents++<12)g_log(played?"Stoneskin V1.117 CAST: full one-second Aether activation; native status lifetime independent; EXPERIMENTAL":
        "Stoneskin V1.102 stopped: own cast contract unavailable; native finish, transfer disabled for session; select Native");
}

void HookPlay(void* ability,void* montage,float rate,Name section){
    local_players::Context c{};
    if(!g_play)return;
    if(foreign_caltrops::HandlePlay(g_play,ability,montage,rate,section)||foreign_power::HandlePlay(g_play,ability,montage,rate,section)||
       foreign_beam::HandlePlay(g_play,ability,montage,rate,section)||foreign_rampage::HandlePlay(g_play,ability,montage,rate,section))return;
    if(ForeignLaunch(ability,c)){if(auto* recent=GeyserActor(c.index))*recent={};g_finish(ability,0);return;}
    const int choice=ForeignCast(ability,c);
    if(!choice){g_play(ability,montage,rate,section);return;}
    if(choice==6){StonePlay(ability,montage,rate,section,c);return;}

    auto* recent=GeyserActor(c.index);
    if(!recent){g_finish(ability,0);return;}
    Animation source{};native_objects::Identity actor{};void* replacement=nullptr;bool played=false;
    uintptr_t mesh=0,asset=0;Array bones{};Name root{};
    if(!g_geyserFailed&&std::isfinite(rate)&&rate>0&&rate<=10&&!section.index&&!section.number&&
       Source(c.character,source)&&native_objects::CaptureLive(g_base,reinterpret_cast<void*>(c.character),actor)&&
       Read(c.character,0x380,mesh)&&Read(mesh,0x5A0,asset)&&Read(asset,0x190,bones)&&bones.data&&
       bones.count>0&&bones.count<=bones.capacity&&bones.capacity<=1000&&Read(bones.data,0,root)&&
       native_objects::NameValueIs(g_base,root.index,root.number,"Bone_Strife_Root"))
        replacement=foreign_montage::Geyser(reinterpret_cast<uintptr_t>(montage),{source.sequence,source.skeleton,source.slot.index,source.slot.number,root.index,root.number});
    if(replacement){
        g_play(ability,replacement,rate,section);uintptr_t instance=0;
        played=Read(reinterpret_cast<uintptr_t>(ability),0x1D8,instance)&&instance&&g_playing(reinterpret_cast<void*>(instance));
    }
    if(played){*recent=actor;geyser_static::Begin(c,ability,replacement);foreign_montage::StartSounds(reinterpret_cast<uintptr_t>(montage),ability);}
    else {g_geyserFailed=true;g_finish(ability,0);}
    if(g_log&&++g_events<=12)g_log(played?"Foreign Wrath V1.98 Geyser: native callback and FX timeline retained; own casting gesture when loaded, idle fallback; animation EXPERIMENTAL":
        "Foreign Wrath V1.88 Geyser contract unavailable; cast finished and further Geyser requests stopped for this session");
    return;

}
bool Boundary(){
    constexpr unsigned char getter[]={0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xEC,0x30};
    constexpr unsigned char play[]={0x48,0x89,0x5C,0x24,0x10,0x57,0x48,0x83,0xEC,0x40,0x48,0x8B,0x81,0xB0,0x01,0x00};
    constexpr unsigned char finish[]={0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48};
    constexpr unsigned char playing[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x83,0xC1};
    return g_base&&!std::memcmp(reinterpret_cast<void*>(g_base+0x7AAC30),getter,sizeof(getter))&&
        !std::memcmp(reinterpret_cast<void*>(g_base+0x5AA190),play,sizeof(play))&&
        !std::memcmp(reinterpret_cast<void*>(g_base+0x59B080),finish,sizeof(finish))&&
        !std::memcmp(reinterpret_cast<void*>(g_base+0x676620),playing,sizeof(playing));
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    geyser_static::Initialize(log,exactExecutable);foreign_montage::Initialize(log,exactExecutable);stone_visual::Initialize(log,exactExecutable);
    if(!Boundary()){if(log)log("Foreign Wrath V1.83 unavailable: native lifecycle signature mismatch");return;}
    auto* getter=reinterpret_cast<void*>(g_base+0x7AAC30);auto* play=reinterpret_cast<void*>(g_base+0x5AA190);
    if(MH_CreateHook(getter,reinterpret_cast<void*>(&HookGetter),reinterpret_cast<void**>(&g_getter))!=MH_OK)return;
    if(MH_CreateHook(play,reinterpret_cast<void*>(&HookPlay),reinterpret_cast<void**>(&g_play))!=MH_OK){MH_RemoveHook(getter);g_getter=nullptr;return;}
    if(MH_EnableHook(getter)!=MH_OK||MH_EnableHook(play)!=MH_OK){MH_DisableHook(getter);MH_DisableHook(play);MH_RemoveHook(getter);MH_RemoveHook(play);g_getter=nullptr;g_play=nullptr;return;}
    g_finish=reinterpret_cast<Finish>(g_base+0x59B080);g_playing=reinterpret_cast<Playing>(g_base+0x676620);g_ready=true;
    constexpr unsigned char create[]={0x48,0x8B,0xC4,0x48,0x89,0x50,0x10,0x55,0x56,0x57,0x41,0x56,0x41,0x57,0x48,0x8D};
    if(!std::memcmp(reinterpret_cast<void*>(g_base+0x168A410),create,sizeof(create)))g_create=reinterpret_cast<Create>(g_base+0x168A410);
    foreign_caltrops::Initialize(log,exactExecutable);
    foreign_power::Initialize(log,exactExecutable);
    foreign_beam::Initialize(log,exactExecutable);foreign_rampage::Initialize(log,exactExecutable);
    if(log)log("Foreign Wrath V1.105 READY: accepted Geyser/Caltrops/Clone/War beam/Stoneskin preserved; Rampage Strife native-motion candidate");
}
int Status(int choice){
    if(choice>=1&&choice<=6)g_requested=true;
    if(choice==6){g_requested=true;return !g_ready||!g_create||g_stoneFailed?0:(Alive(g_stone)?2:1);}
    if(choice==5){g_requested=true;return foreign_rampage::Status();}
    if(choice>=1&&choice<=4)g_requested=true;
    if(choice==2)return foreign_beam::Status();
    if(choice==3)return foreign_caltrops::Status();
    if(choice==1)return foreign_power::Status(choice);
    if(choice!=4||!g_ready)return 0;g_requested=true;
    return foreign_montage::Ready()&&!g_geyserFailed?(g_geyserReady?2:1):0;
}
void Observe(const local_players::Context& owner){
    foreign_rampage::Observe(owner);geyser_static::Observe(owner);stone_visual::Observe(owner);
    if(!g_ready||!native_tps::OnGameThread())return;
    bool wanted=g_requested.load();for(const auto& slots:runtime::Get().wrathSlots)for(const auto& choice:slots)wanted=wanted||((choice.load()>=1&&choice.load()<=5)||choice.load()==6);
    if(!wanted||owner.index<0||owner.index>1||!owner.localPlayer||!owner.world||owner.kind!=local_players::PoseKind::Foot)return;
    if(g_world!=owner.world){g_world=owner.world;g_cursor=0;g_count=0;g_complete=false;}
    if(auto* recent=GeyserActor(owner.index);recent&&recent->address&&recent->address!=owner.character)*recent={};
    const auto now=GetTickCount64();if(now>=g_scanTick&&now-g_scanTick<15)return;g_scanTick=now;
    uintptr_t cls=0,kind=0;if(!Read(owner.character,0x10,cls)||!Read(cls,0x10,kind)||!Is(kind,"BlueprintGeneratedClass"))return;
    Scan(kind);
}
bool Prepare(void* actor,uintptr_t& abilityClass,int choice){
    if(choice==6){
        local_players::Context c{};Animation source{};uintptr_t cdo=0,cls=0;
        if(Status(6)!=2||!OwnedStrife(actor,c)||Kind(g_stone.address)!=6||!Source(c.character,source)||
           !Read(g_stone.address,0xF8,cdo)||!Read(cdo,0x10,cls)||cls!=g_stone.address)return false;
        abilityClass=g_stone.address;return true;
    }
    if(choice==5){g_requested=true;return foreign_rampage::Prepare(actor,abilityClass);}
    if(choice==3)return foreign_caltrops::Prepare(actor,abilityClass);
    if(choice==1)return foreign_power::Prepare(actor,abilityClass,choice);
    if(choice==2){g_requested=true;return foreign_beam::Prepare(actor,abilityClass);}
    if(choice!=4)return false;g_requested=true;
    local_players::Context c{};Animation animation{};uintptr_t cdo=0,cls=0;
    if(!g_ready||!g_geyserReady||!OwnedStrife(actor,c)||Kind(g_geyser.address)!=4||g_geyserFailed||
       !Read(g_geyser.address,0xF8,cdo)||!Read(cdo,0x10,cls)||cls!=g_geyser.address||!Source(c.pawn,animation)||
       !foreign_montage::CanBuild({animation.sequence,animation.skeleton,animation.slot.index,animation.slot.number}))return false;
    abilityClass=g_geyser.address;return true;
}
uintptr_t* Resolve(ResolveFn native,void* actor,uintptr_t* result,Name id,bool* unlocked,bool* affordable,uintptr_t abilityClass){
    if(!native)return result;
    Override request{actor,abilityClass,id};struct Scope {const Override* previous;~Scope(){g_override=previous;}} scope{g_override};
    g_override=&request;return native(actor,result,id,unlocked,affordable);
}

void Shutdown(){geyser_static::Shutdown();stone_visual::Shutdown();foreign_caltrops::Shutdown();foreign_power::Shutdown();foreign_beam::Shutdown();foreign_rampage::Shutdown();g_ready=false;g_geyserReady=false;}
}
