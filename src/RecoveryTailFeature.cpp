#include "RecoveryTailFeature.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <array>
#include <atomic>
#include <cmath>
#include <cstdio>

namespace dg::recovery_tail {
namespace {
constexpr uint64_t MoveBit=uint64_t(1)<<1;
using Interrupt=void(*)(void*,unsigned char,bool);
using Playing=bool(*)(void*);
Interrupt g_window=nullptr,g_interrupt=nullptr;Playing g_playing=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};unsigned g_events=0;
using Id=native_objects::Identity;
struct Pending {Id actor{},ability{},helper{},montage{};uintptr_t world=0;uint32_t generation=0;bool playing=false,owned=false;uint64_t stopped=0;};
std::array<Pending,2> g_pending{};
template<class T>bool Read(uintptr_t p,size_t at,T& value){native_objects::MemoryReader r;return r.Read(p,at,value);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),id);}
bool Named(uintptr_t p,const char* text){int index=0,number=0;return Read(p,0x18,index)&&Read(p,0x1C,number)&&native_objects::NameValueEquals(g_base,index,number,text);}
bool Top(uintptr_t actor,uintptr_t& ability){
    uintptr_t component=0,owner=0,list=0;int count=0,capacity=0;
    return Read(actor,0xC70,component)&&Read(component,0xE8,owner)&&owner==actor&&Read(component,0xF8,list)&&
        Read(component,0x100,count)&&Read(component,0x104,capacity)&&count>0&&count<=capacity&&capacity<=64&&list&&Read(list,size_t(count-1)*8,ability)&&ability;
}
unsigned Kind(uintptr_t ability,unsigned type){
    uintptr_t cls=0,outer=0,meta=0;if(!Read(ability,0x10,cls)||!Read(cls,0x10,meta)||!Named(meta,"BlueprintGeneratedClass")||!Read(cls,0x20,outer))return 0;
    const char* chest=type==1?"MayhemAbility_Strife_ChestInteract_C":"MayhemAbility_War_ChestInteract_C";
    const char* chestPackage=type==1?"/Game/Blueprints/Abilities/Interact/MayhemAbility_Strife_ChestInteract":"/Game/Blueprints/Abilities/Interact/MayhemAbility_War_ChestInteract";
    if(Named(cls,chest)&&Named(outer,chestPackage))return 2;
    constexpr const char* dash[]={"MayhemAbility_Strife_Ground_Dash_Level1_Blueprint_C","MayhemAbility_Strife_Ground_Dash_Level2_Blueprint_C","MayhemAbility_Strife_Ground_Dash_Level3_Blueprint_C",
        "MayhemAbility_Strife_Ground_Dash_Aim_Level1_Blueprint_C","MayhemAbility_Strife_Ground_Dash_Aim_Level2_Blueprint_C","MayhemAbility_Strife_Ground_Dash_Aim_Level3_Blueprint_C",
        "MayhemAbility_Strife_Ground_Dash_Fire_Level1_Blueprint_C","MayhemAbility_Strife_Ground_Dash_Fire_Level2_Blueprint_C","MayhemAbility_Strife_Ground_Dash_Fire_Level3_Blueprint_C","MayhemAbility_War_Ground_Dash_Blueprint_C"};
    for(unsigned i=0;i<std::size(dash);++i)if((type==1?i<9:i==9)&&Named(cls,dash[i])){
        char path[180]{};sprintf_s(path,"/Game/Blueprints/Abilities/Dash/%.*s",int(std::strlen(dash[i])-2),dash[i]);return Named(outer,path)?1u:0u;
    }
    return 0;
}
bool Montage(uintptr_t ability,unsigned type,unsigned kind,uintptr_t& helper,uintptr_t& montage){
    uintptr_t cls=0,outer=0;float rate=0;
    if(!Read(ability,0x1D8,helper)||!Read(helper,0x10,cls)||!Named(cls,"MayhemMontagePlayer")||!Read(helper,0x28,montage)||
       !Read(montage,0x10,cls)||!Named(cls,"AnimMontage")||!Read(montage,0x94,rate)||!std::isfinite(rate)||rate<=0||rate>10||!Read(montage,0x20,outer))return false;
    if(kind==2)return type==1?
        ((Named(montage,"Strife_IA_Chest_A_Open_Montage")&&Named(outer,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_IA_Chest_A_Open_Montage"))||
         (Named(montage,"Strife_IA_En_Chest_A_Open_Montage")&&Named(outer,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_IA_En_Chest_A_Open_Montage"))):
        ((Named(montage,"War_IA_Chest_A_Open_Montage")&&Named(outer,"/Game/Characters/Heroes/Hero_War/Animation/War_IA_Chest_A_Open_Montage"))||
         (Named(montage,"War_IA_En_Chest_A_Open_Montage")&&Named(outer,"/Game/Characters/Heroes/Hero_War/Animation/War_IA_En_Chest_A_Open_Montage")));
    // The authored direction references belong to this exact native dash.
    for(size_t at:{size_t(0x200),size_t(0x208),size_t(0x210),size_t(0x218)}){uintptr_t authored=0;if(Read(ability,at,authored)&&authored==montage)return true;}
    return false;
}
void Restore(Pending& saved){
    if(saved.owned&&Alive(saved.actor)&&Alive(saved.ability)&&Alive(saved.helper)&&Alive(saved.montage)){
        uintptr_t owner=0,character=0,top=0,helper=0,montage=0;unsigned char state=0,pending=0;local_players::Context current{};
        if(local_players::ResolveActor(reinterpret_cast<void*>(saved.actor.address),current)&&current.world==saved.world&&current.character==saved.actor.address&&current.pawn==current.character&&
           current.kind==local_players::PoseKind::Foot&&Read(saved.ability.address,0x48,owner)&&owner==saved.actor.address&&Read(saved.ability.address,0x1B0,character)&&character==owner&&
           Top(owner,top)&&top==saved.ability.address&&Read(top,0xD8,state)&&state==2&&Read(top,0x148,pending)&&!pending&&
           Read(top,0x1D8,helper)&&helper==saved.helper.address&&Read(helper,0x28,montage)&&montage==saved.montage.address){
            g_interrupt(reinterpret_cast<void*>(top),1,false);
            if(Alive(saved.ability)&&Alive(saved.actor)&&Read(top,0xD8,state)&&state==2&&Read(top,0x48,owner)&&owner==saved.actor.address&&
               Read(top,0x1B0,character)&&character==owner&&Read(top,0x1D8,helper)&&helper==saved.helper.address)g_window(reinterpret_cast<void*>(top),1,false);
        }
    }
    saved={};
}
void Service(const local_players::Context& c,uint32_t generation,bool focused,uint64_t now){
    if(!g_ready||!g_window||!g_interrupt||!g_playing||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    auto& saved=g_pending[unsigned(c.index)];auto& settings=runtime::Get();
    if((!settings.recoveryDashTailEnabled.load()&&!settings.recoveryChestTailEnabled.load())||!focused||!c.localPlayer||!c.world||c.kind!=local_players::PoseKind::Foot||
       c.pawn!=c.character||!local_players::Matches(unsigned(c.index),c.character,generation)||native_tps::IsWorldPaused(reinterpret_cast<void*>(c.pawn))||native_tps::CinematicActive(unsigned(c.index))||native_tps::RadialActive(unsigned(c.index))){Restore(saved);return;}
    uintptr_t pc=0,pawn=0,movement=0,owner=0,resources=0,ability=0,helper=0,montage=0;unsigned char role=0,type=0,state=0,pending=0,mode=0,custom=0;float health=0;
    uint32_t flags=0;uint64_t requested=0,enabled=0;
    if(!c.character||!Read(c.character,0x348,pc)||pc!=c.controller||!Read(pc,0x350,pawn)||pawn!=c.character||!Read(pawn,0x118,role)||role!=3||
       !Read(pawn,0xC18,type)||(type!=1&&type!=2)||!Read(pawn,0xEA0,resources)||!Read(resources,0x37C,health)||!std::isfinite(health)||health<=0||
       !Read(pawn,0x388,movement)||!Read(movement,0xE8,owner)||owner!=pawn||!Read(movement,0x1B0,mode)||!Read(movement,0x1B1,custom)||
       !Top(pawn,ability)||!Read(ability,8,flags)||(flags&0x30u)||!Read(ability,0x48,owner)||owner!=pawn||!Read(ability,0x1B0,owner)||owner!=pawn||
       !Read(ability,0xD8,state)||state!=2||!Read(ability,0x148,pending)||pending){Restore(saved);return;}
    const unsigned kind=Kind(ability,type);
    if(!kind||!(kind==1?settings.recoveryDashTailEnabled.load():settings.recoveryChestTailEnabled.load())||
       !Montage(ability,type,kind,helper,montage)||!Read(ability,0x120,requested)||!Read(ability,0xB0,enabled)){Restore(saved);return;}
    if(saved.ability.address!=ability||!Alive(saved.ability)||saved.actor.address!=pawn||saved.world!=c.world||saved.generation!=generation||
       saved.helper.address!=helper||saved.montage.address!=montage||!Alive(saved.helper)||!Alive(saved.montage)){
        Restore(saved);Pending next{};if(!Capture(pawn,next.actor)||!Capture(ability,next.ability)||!Capture(helper,next.helper)||!Capture(montage,next.montage))return;
        next.world=c.world;next.generation=generation;saved=next;
    }
    const bool playing=g_playing(reinterpret_cast<void*>(helper));
    if(!Alive(saved.ability)||!Alive(saved.actor)||!Alive(saved.helper)||!Alive(saved.montage)){saved={};return;}
    if(playing){if(saved.owned){Restore(saved);return;}saved.playing=true;saved.stopped=0;return;}
    if(!saved.playing||saved.owned)return; // Never treat an unstarted montage as a completed one.
    if(kind==1){
        float remaining=0,total=0;if(!((mode==1&&!custom)||(mode==6&&custom==5))||!Read(ability,0x224,remaining)||!Read(ability,0x228,total)||
           !std::isfinite(remaining)||!std::isfinite(total)||remaining>0||total<=0)return;
    }else if(mode!=1||custom)return;
    if(!saved.stopped){saved.stopped=now;return;}
    float delay=settings.recoveryTailDelaySeconds.load();if(!std::isfinite(delay)||delay<0||delay>1)delay=0;
    if(now<saved.stopped||now-saved.stopped<30+uint64_t(delay*1000)||(requested|enabled)&MoveBit)return;
    saved.owned=true; // Reentrant observations cannot issue the request twice.
    g_window(reinterpret_cast<void*>(ability),1,true);
    if(g_log&&g_events++<12){char line[180]{};sprintf_s(line,"Recovery tail V1.126: player=%d target=%s stopped montage, native MOVE window requested; EXPERIMENTAL",c.index+1,kind==1?"ground dash":"chest");g_log(line);}
}
}
void Initialize(LogFn log,bool exact){
    g_log=log;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char entry[]={0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF};
    constexpr unsigned char playing[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x83,0xC1};
    if(!g_base||std::memcmp(reinterpret_cast<void*>(g_base+0x5A9D60),entry,sizeof(entry))||std::memcmp(reinterpret_cast<void*>(g_base+0x599310),entry,sizeof(entry))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x676620),playing,sizeof(playing)))return;
    g_window=reinterpret_cast<Interrupt>(g_base+0x5A9D60);g_interrupt=reinterpret_cast<Interrupt>(g_base+0x599310);g_playing=reinterpret_cast<Playing>(g_base+0x676620);g_ready=true;
}
bool Ready(){return g_ready.load();}
void OnLivePlayer(const local_players::Context& c,uint32_t generation,bool focused){Service(c,generation,focused,GetTickCount64());}
void Shutdown(){g_ready=false;} // No cached-object access from the teardown thread.
}
