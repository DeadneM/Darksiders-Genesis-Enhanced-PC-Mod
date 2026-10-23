#include "ForeignWrathFeature.h"
#include "ForeignMontage.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <atomic>
#include <cmath>
#include <cstdio>

namespace dg::foreign_wrath {
namespace {
using Getter=uintptr_t*(*)(void*,uintptr_t*,Name);
using Play=void(*)(void*,void*,float,Name);
using Create=void*(*)(void*,Name,float,float,float,int,float,float);
using Finish=void(*)(void*,unsigned char);
using Playing=bool(*)(void*);
Getter g_getter=nullptr;Play g_play=nullptr;Create g_create=nullptr;Finish g_finish=nullptr;Playing g_playing=nullptr;
uintptr_t g_base=0,g_world=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false},g_requested{false},g_assetsReady{false};
native_objects::Identity g_stone{},g_geyser{},g_geyserActors[2]{};
std::atomic_bool g_requestGeyser{false},g_geyserReady{false},g_geyserFailed{false};bool g_searchedGeyser=false;
int g_cursor=0,g_count=0;bool g_complete=false;uint64_t g_scanTick=0;unsigned g_events=0,g_failures=0;
struct Array {uintptr_t data=0;int count=0,capacity=0;};
struct Override {void* actor=nullptr;uintptr_t abilityClass=0;Name id{};};
thread_local const Override* g_override=nullptr;
template<class T> bool Read(uintptr_t p,size_t offset,T& value){native_objects::MemoryReader r;return r.Read(p,offset,value);}
template<size_t N> bool Is(uintptr_t object,const char (&name)[N]){return native_objects::NameIs(g_base,object,name);}
bool Alive(const native_objects::Identity& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool StoneClass(uintptr_t object){
    uintptr_t kind=0,package=0;
    if(!Read(object,0x10,kind)||!Is(kind,"BlueprintGeneratedClass")||!Read(object,0x20,package))return false;
    return Is(object,"MayhemAbility_StoneSkin_Blueprint_C")&&Is(package,"/Game/Blueprints/Abilities/Stoneskin/MayhemAbility_StoneSkin_Blueprint");
}
bool GeyserClass(uintptr_t object){
    uintptr_t kind=0,package=0;
    return Read(object,0x10,kind)&&Is(kind,"BlueprintGeneratedClass")&&Read(object,0x20,package)&&
        Is(object,"MayhemAbility_Wrath_BladeGeyser_Blueprint_C")&&Is(package,"/Game/Blueprints/Abilities/MayhemAbility_Wrath_BladeGeyser_Blueprint");
}
int Kind(uintptr_t cls){
    if(cls==g_stone.address&&Alive(g_stone)&&StoneClass(cls))return 6;
    if(cls==g_geyser.address&&Alive(g_geyser)&&GeyserClass(cls))return 4;
    return 0;
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
    bool expired=false;
    if(g_stone&&!Alive(g_stone)){g_stone={};expired=true;}
    if(g_geyser&&!Alive(g_geyser)){g_geyser={};g_geyserReady=false;expired=true;}
    if(expired){g_cursor=0;g_complete=false;g_assetsReady=false;}
    const bool geyser=g_requestGeyser.load();
    if(g_stone){g_assetsReady=true;if(!geyser||g_geyser)return;}
    uintptr_t chunks=0;int count=0;
    if(!Read(g_base,0x3959680,chunks)||!Read(g_base,0x3959694,count)||count<=0||count>2000000)return;
    if(count!=g_count){if(count<g_count)g_cursor=0;g_count=count;g_complete=false;}
    if(g_complete)return;
    for(unsigned n=0;n<512&&g_cursor<count;++n,++g_cursor){
        uintptr_t chunk=0,object=0,cls=0;uint32_t flags=0;
        if(!Read(chunks,size_t(g_cursor/65536)*8,chunk)||!Read(chunk,size_t(g_cursor%65536)*24,object)||!object||
           !Read(chunk,size_t(g_cursor%65536)*24+8,flags)||(flags&0x30000000u)||!Read(object,0x10,cls)||cls!=generatedClass)continue;
        if(!g_stone&&StoneClass(object)&&native_objects::CaptureLive(g_base,reinterpret_cast<void*>(object),g_stone)&&g_log)
            g_log("Foreign Wrath V1.83 Stoneskin assets ready; no package load or cross-player component borrowing");
        if(geyser&&!g_geyser&&GeyserClass(object)&&native_objects::CaptureLive(g_base,reinterpret_cast<void*>(object),g_geyser)&&g_log)
            g_log("Foreign Wrath V1.88 Geyser class ready; private native montage contract checked on cast; EXPERIMENTAL");
        if(g_stone&&(!geyser||g_geyser))break;
    }
    if(g_cursor>=count)g_complete=true;
    if(g_stone)g_assetsReady=true;
    if(g_geyser)g_geyserReady=true;
}
uintptr_t* HookGetter(void* actor,uintptr_t* result,Name id){
    const auto* request=g_override;
    if(g_ready&&request&&request->actor==actor&&request->id==id&&result&&native_tps::OnGameThread()&&Kind(request->abilityClass)){
        *result=request->abilityClass;return result;
    }
    return g_getter?g_getter(actor,result,id):result;
}
int ForeignCast(void* live,local_players::Context& c){
    const auto ability=reinterpret_cast<uintptr_t>(live);
    uintptr_t cls=0,actor=0,character=0;unsigned char state=0,role=0;
    uint32_t objectFlags=0;
    if(!(g_ready&&g_create&&g_finish&&g_playing&&native_tps::OnGameThread()&&Read(ability,8,objectFlags)&&!(objectFlags&0x30u)&&
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
    const auto& recent=g_geyserActors[c.index];return recent.address==actor&&Alive(recent);
}
bool Duration(void* nativeMontage,float& duration){
    const auto montage=reinterpret_cast<uintptr_t>(nativeMontage);uintptr_t cls=0,package=0;float length=0,scale=0;
    if(!(Read(montage,0x10,cls)&&Is(cls,"AnimMontage")&&Read(montage,0x20,package)&&
        ((Is(montage,"War_Atk_Stoneskin_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Atk_Stoneskin_Montage"))||
         (Is(montage,"War_Atk_Stoneskin_Molten_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Atk_Stoneskin_Molten_Montage")))&&
        Read(montage,0x90,length)&&std::isfinite(length)&&length>.1f&&length<10&&
        Read(montage,0x94,scale)&&std::isfinite(scale)&&scale>.1f&&scale<10))return false;
    duration=length/scale;return true;
}
void HookPlay(void* ability,void* montage,float rate,Name section){
    local_players::Context c{};float duration=0;
    if(!g_play)return;
    // Geyser can request War's rising-slash companion on interruption. That
    // foreign melee chain has no Strife adapter: finish its unplayed cast safely.
    // War's own rising slash and every unrelated actor remain native.
    if(ForeignLaunch(ability,c)){g_geyserActors[c.index]={};g_finish(ability,0);return;}
    const int choice=ForeignCast(ability,c);
    if(!choice){g_play(ability,montage,rate,section);return;}
    if(choice==4){
        const auto player=static_cast<unsigned>(c.index);
        if(player>=2){g_finish(ability,0);return;}
        Animation source{};native_objects::Identity actor{};void* replacement=nullptr;bool played=false;
        if(!g_geyserFailed&&std::isfinite(rate)&&rate>0&&rate<=10&&!section.index&&!section.number&&
           Source(c.character,source)&&native_objects::CaptureLive(g_base,reinterpret_cast<void*>(c.character),actor))
            replacement=foreign_montage::Geyser(reinterpret_cast<uintptr_t>(montage),{source.sequence,source.skeleton,source.slot.index,source.slot.number});
        if(replacement){
            g_play(ability,replacement,rate,section);uintptr_t instance=0;
            played=Read(reinterpret_cast<uintptr_t>(ability),0x1D8,instance)&&instance&&g_playing(reinterpret_cast<void*>(instance));
        }
        if(played)g_geyserActors[player]=actor;
        else {g_geyserFailed=true;g_finish(ability,0);}
        if(g_log&&++g_events<=12)g_log(played?"Foreign Wrath V1.88 Geyser: native SpawnBlast notify on private compatible montage; native cost/timing/finish retained; EXPERIMENTAL":
            "Foreign Wrath V1.88 Geyser contract unavailable; cast finished and further Geyser requests stopped for this session");
        return;
    }
    Animation source{};void* replacement=nullptr;float length=0;uintptr_t skeleton=0,cls=0;bool played=false;
    // A transient compatible montage contains no foreign animation notifies.
    // Pass it through the ability's original play/finish delegate lifecycle.
    if(OwnedStrife(reinterpret_cast<void*>(c.character),c)&&Duration(montage,duration)&&
       std::isfinite(rate)&&rate>0&&rate<=10&&section.index==0&&section.number==0&&Source(c.pawn,source))
        replacement=g_create(reinterpret_cast<void*>(source.sequence),source.slot,.1f,.1f,1.0f,1,-1.0f,0.0f);
    if(replacement&&Read(reinterpret_cast<uintptr_t>(replacement),0x10,cls)&&Is(cls,"AnimMontage")&&
       Read(reinterpret_cast<uintptr_t>(replacement),0x38,skeleton)&&skeleton==source.skeleton&&
       Read(reinterpret_cast<uintptr_t>(replacement),0x90,length)&&std::isfinite(length)&&length>.1f&&length<10){
        const float adjusted=rate*length/duration;
        if(std::isfinite(adjusted)&&adjusted>.01f&&adjusted<100){
            g_play(ability,replacement,adjusted,section);uintptr_t instance=0;
            played=Read(reinterpret_cast<uintptr_t>(ability),0x1D8,instance)&&instance&&g_playing(reinterpret_cast<void*>(instance));
        }
    }
    // 5A76D0 uses Finish(cause=0) after native montage completion. During
    // OnStart, 59B080 defers that same request safely until start returns.
    if(!played)g_finish(ability,0);
    if(g_log&&++g_events<=12){char text[200]{};sprintf_s(text,
        "Foreign Wrath V1.83 Stoneskin player=%d compatibleMontage=%d nativeDuration=%.3f; native status/cost retained; EXPERIMENTAL",c.index+1,played?1:0,duration);g_log(text);}
}
bool Boundary(){
    constexpr unsigned char getter[]={0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xEC,0x30};
    constexpr unsigned char play[]={0x48,0x89,0x5C,0x24,0x10,0x57,0x48,0x83,0xEC,0x40,0x48,0x8B,0x81,0xB0,0x01,0x00};
    constexpr unsigned char create[]={0x48,0x8B,0xC4,0x48,0x89,0x50,0x10,0x55,0x56,0x57,0x41,0x56,0x41,0x57,0x48,0x8D};
    constexpr unsigned char finish[]={0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48};
    constexpr unsigned char playing[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x83,0xC1};
    return g_base&&!std::memcmp(reinterpret_cast<void*>(g_base+0x7AAC30),getter,sizeof(getter))&&
        !std::memcmp(reinterpret_cast<void*>(g_base+0x5AA190),play,sizeof(play))&&
        !std::memcmp(reinterpret_cast<void*>(g_base+0x168A410),create,sizeof(create))&&
        !std::memcmp(reinterpret_cast<void*>(g_base+0x59B080),finish,sizeof(finish))&&
        !std::memcmp(reinterpret_cast<void*>(g_base+0x676620),playing,sizeof(playing));
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    foreign_montage::Initialize(log,exactExecutable);
    if(!Boundary()){if(log)log("Foreign Wrath V1.83 unavailable: native lifecycle signature mismatch");return;}
    auto* getter=reinterpret_cast<void*>(g_base+0x7AAC30);auto* play=reinterpret_cast<void*>(g_base+0x5AA190);
    if(MH_CreateHook(getter,reinterpret_cast<void*>(&HookGetter),reinterpret_cast<void**>(&g_getter))!=MH_OK)return;
    if(MH_CreateHook(play,reinterpret_cast<void*>(&HookPlay),reinterpret_cast<void**>(&g_play))!=MH_OK){MH_RemoveHook(getter);g_getter=nullptr;return;}
    if(MH_EnableHook(getter)!=MH_OK||MH_EnableHook(play)!=MH_OK){MH_DisableHook(getter);MH_DisableHook(play);MH_RemoveHook(getter);MH_RemoveHook(play);g_getter=nullptr;g_play=nullptr;return;}
    g_create=reinterpret_cast<Create>(g_base+0x168A410);g_finish=reinterpret_cast<Finish>(g_base+0x59B080);g_playing=reinterpret_cast<Playing>(g_base+0x676620);g_ready=true;
    if(log)log("Foreign Wrath V1.88 READY: Stoneskin and Geyser on owned Strife only, native unlock/cost/lifecycle; compatible private montage; EXPERIMENTAL");
}
int Status(int choice){
    if(!g_ready)return 0;g_requested=true;
    if(choice==4){g_requestGeyser=true;return foreign_montage::Ready()&&!g_geyserFailed?(g_geyserReady?2:1):0;}
    return choice==6?(g_assetsReady?2:1):0;
}
void Observe(const local_players::Context& owner){
    if(!g_ready||!native_tps::OnGameThread())return;
    bool wanted=g_requested.load();for(const auto& choice:runtime::Get().wrathSlots[0]){wanted=wanted||choice.load()==6||choice.load()==4;if(choice.load()==4)g_requestGeyser=true;}
    if(!wanted||owner.index<0||owner.index>1||!owner.localPlayer||!owner.world||owner.kind!=local_players::PoseKind::Foot)return;
    if(g_world!=owner.world){g_world=owner.world;g_cursor=0;g_count=0;g_complete=false;}
    if(g_requestGeyser&&!g_searchedGeyser){g_cursor=0;g_complete=false;g_searchedGeyser=true;}
    if(g_geyserActors[owner.index].address&&g_geyserActors[owner.index].address!=owner.character)g_geyserActors[owner.index]={};
    const auto now=GetTickCount64();if(now>=g_scanTick&&now-g_scanTick<15)return;g_scanTick=now;
    uintptr_t cls=0,kind=0;if(!Read(owner.character,0x10,cls)||!Read(cls,0x10,kind)||!Is(kind,"BlueprintGeneratedClass"))return;
    Scan(kind);
}
bool Prepare(void* actor,uintptr_t& abilityClass,int choice){
    g_requested=true;local_players::Context c{};Animation animation{};uintptr_t cdo=0,cls=0;
    if(choice!=4&&choice!=6)return false;
    if(choice==4)g_requestGeyser=true;
    const auto& selected=choice==4?g_geyser:g_stone;
    if(!g_ready||!(choice==4?g_geyserReady.load():g_assetsReady.load())||!OwnedStrife(actor,c)||Kind(selected.address)!=choice)return false;
    if(!Read(selected.address,0xF8,cdo)||!Read(cdo,0x10,cls)||cls!=selected.address||!Source(c.pawn,animation)){
        if(g_log&&++g_failures<=6)g_log("Foreign Wrath waiting: native class default or own compatible animation unavailable; selected slot inactive, no cost");
        return false;
    }
    if(choice==4&&(g_geyserFailed||!foreign_montage::CanBuild({animation.sequence,animation.skeleton,animation.slot.index,animation.slot.number})))return false;
    abilityClass=selected.address;return true;
}
uintptr_t* Resolve(ResolveFn native,void* actor,uintptr_t* result,Name id,bool* unlocked,bool* affordable,uintptr_t abilityClass){
    if(!native)return result;
    Override request{actor,abilityClass,id};struct Scope {const Override* previous;~Scope(){g_override=previous;}} scope{g_override};
    g_override=&request;return native(actor,result,id,unlocked,affordable);
}
void Shutdown(){g_ready=false;g_assetsReady=false;g_geyserReady=false;}
}
