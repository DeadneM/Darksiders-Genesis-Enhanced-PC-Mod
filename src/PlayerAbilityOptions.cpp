#include "PlayerAbilityOptions.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <array>
#include <cmath>

namespace dg::player_abilities {
namespace {
using Id=native_objects::Identity;
using Unlock=bool(*)(void*,unsigned char,uint64_t);
using Request=void(*)(void*,unsigned char);
using Synergy=void(*)(void*,unsigned char);
uintptr_t g_base=0;Unlock g_unlock=nullptr;Request g_request=nullptr;Synergy g_synergy=nullptr;
LogFn g_log=nullptr;std::atomic_bool g_counterReady{false},g_synergyReady{false};
struct Pending {Id actor{},controller{},ability{};uintptr_t world=0;uint32_t generation=0;uint64_t started=0;int mode=0;bool finished=false;};
std::array<Pending,2> g_pending{};std::atomic_bool g_pendingActive{false};unsigned g_counterEvents=0,g_synergyEvents=0;
void Clear(Pending& p){p={};g_pendingActive.store(bool(g_pending[0].actor)||bool(g_pending[1].actor),std::memory_order_relaxed);}
template<class T>bool Read(uintptr_t p,size_t at,T& out){native_objects::MemoryReader r;return r.Read(p,at,out);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),id);}
template<size_t N>bool Is(uintptr_t p,const char(&name)[N]){return native_objects::NameIs(g_base,p,name);}
bool Hero(const local_players::Context& c){
    uintptr_t controller=0,pawn=0,resource=0;unsigned char role=0;float health=0;
    return c.index>=0&&c.index<2&&c.kind==local_players::PoseKind::Foot&&c.pawn==c.character&&
        Read(c.character,0x348,controller)&&controller==c.controller&&Read(controller,0x350,pawn)&&pawn==c.character&&
        Read(c.character,0x118,role)&&role==3&&Read(c.character,0xEA0,resource)&&
        Read(resource,0x37C,health)&&std::isfinite(health)&&health>0;
}
bool Gameplay(const local_players::Context& c){
    return Hero(c)&&!native_tps::IsWorldPaused(reinterpret_cast<void*>(c.character))&&
        !native_tps::CinematicActive(unsigned(c.index))&&!native_tps::RadialActive(unsigned(c.index));
}
bool HookUnlock(void* actor,unsigned char type,uint64_t name){
    // The two exact skill IDs are tested by the native dash SpawnProxyActor.
    // Never edit purchases, stat tables, cores or other players' progression.
    if(!runtime::Get().strifeCounterEvadeEnabled.load()&&type==0&&native_tps::OnGameThread()&&
       (native_objects::NameValueIdentifierEquals(g_base,int(uint32_t(name)),int(name>>32),"Strife_Ability_Counter1")||
        native_objects::NameValueIdentifierEquals(g_base,int(uint32_t(name)),int(name>>32),"Strife_Ability_Counter2"))){
        local_players::Context c;uintptr_t cls=0;unsigned char hero=0;
        if(local_players::ResolveActor(actor,c)&&Hero(c)&&Read(c.character,0xC18,hero)&&hero==1&&
           Read(c.character,0x10,cls)&&Is(cls,"Strife_Blueprint_C")){
            if(g_log&&g_counterEvents++<4)g_log("Counter evade: native level I/II activation suppressed for local Strife; purchases retained");
            return false;
        }
    }
    return g_unlock?g_unlock(actor,type,name):false;
}
bool ActiveSynergy(const local_players::Context& c,Id& found,bool& valid){
    uintptr_t component=0,owner=0,array=0;int count=0,capacity=0;valid=false;
    if(!Read(c.character,0xC70,component)||!Read(component,0xE8,owner)||owner!=c.character||
       !Read(component,0xF8,array)||!Read(component,0x100,count)||!Read(component,0x104,capacity)||
       count<0||capacity<count||capacity>64||(count&&!array))return false;
    for(int i=0;i<count;++i){uintptr_t ability=0,cls=0,instigator=0;unsigned char state=0;
        if(!Read(array,size_t(i)*8,ability)||!ability||!Read(ability,0x10,cls)||!Read(ability,0xD8,state))return false;
        if(state!=2||(!Is(cls,"MayhemAbility_Strife_Synergy_Blueprint_C")&&!Is(cls,"MayhemAbility_War_Synergy_Blueprint_C")))continue;
        if(!Read(ability,0x48,instigator)||instigator!=c.character||!Capture(ability,found))return false;
        valid=true;return true;
    }
    valid=true;return false;
}
void HookRequest(void* controller,unsigned char spawnType){
    const int mode=runtime::Get().synergySwapMode.load();
    // Intercept the server's already-classified SYNERGY request. Native tap,
    // hold threshold, remappable input, unlock checks and co-op all stay native.
    if(mode<1||mode>2||spawnType!=3||!g_synergy||!native_tps::OnGameThread()){
        if(g_request)g_request(controller,spawnType);return;
    }
    uintptr_t pawn=0;local_players::Context c;
    if(!Read(reinterpret_cast<uintptr_t>(controller),0x350,pawn)||!local_players::ResolveActor(reinterpret_cast<void*>(pawn),c)||
       c.controller!=reinterpret_cast<uintptr_t>(controller)||c.localCount!=1||!Gameplay(c)){
        if(g_request)g_request(controller,spawnType);return;
    }
    auto& pending=g_pending[unsigned(c.index)];
    if(pending.actor&&pending.controller.address==c.controller&&pending.actor.address==c.character)return;
    bool valid=false;Id active{};
    if(ActiveSynergy(c,active,valid)||!valid){if(g_request)g_request(controller,spawnType);return;}
    Pending next{};const auto slot=local_players::PublishedSlot(unsigned(c.index));
    if(!Capture(c.character,next.actor)||!Capture(c.controller,next.controller)||
       !local_players::Matches(unsigned(c.index),c.character,slot.generation)){
        if(g_request)g_request(controller,spawnType);return;
    }
    next.world=c.world;next.generation=slot.generation;next.mode=mode;next.started=GetTickCount64();
    pending=next;g_pendingActive.store(true,std::memory_order_relaxed);
    // Retail co-op branch 0x581437 calls precisely this routine with 3. It
    // dispatches this hero's native DoSynergyAbility and consumes native gauge.
    g_synergy(reinterpret_cast<void*>(c.character),3);
    if(ActiveSynergy(c,active,valid))pending.ability=active;
    if(g_log&&g_synergyEvents++<8)g_log(mode==1?"Synergy: current hero first, native default swap queued after completion; EXPERIMENTAL":
        "Synergy: current hero, no swap; native activation and cost; EXPERIMENTAL");
}
template<size_t N>bool Guard(uintptr_t at,const unsigned char(&bytes)[N]){return !std::memcmp(reinterpret_cast<void*>(g_base+at),bytes,N);}
bool Install(uintptr_t at,void* hook,void** original){
    void* target=reinterpret_cast<void*>(g_base+at);
    if(MH_CreateHook(target,hook,original)!=MH_OK)return false;
    const auto result=MH_EnableHook(target);if(result==MH_OK||result==MH_ERROR_ENABLED)return true;
    MH_RemoveHook(target);*original=nullptr;return false;
}
}
void OnLivePlayer(const local_players::Context& c,uint32_t generation,bool commandsAllowed){
    if(!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    auto& pending=g_pending[unsigned(c.index)];if(!pending.actor)return;
    const auto now=GetTickCount64();
    if(!commandsAllowed||!Gameplay(c)||c.localCount!=1||c.character!=pending.actor.address||c.controller!=pending.controller.address||
       c.world!=pending.world||generation!=pending.generation||!local_players::Matches(unsigned(c.index),c.character,generation)||
       !Alive(pending.actor)||!Alive(pending.controller)||runtime::Get().synergySwapMode.load()!=pending.mode||
       now<pending.started||now-pending.started>15000){Clear(pending);return;}
    Id active{};bool valid=false;const bool running=ActiveSynergy(c,active,valid);
    if(!valid){Clear(pending);return;}
    if(running){if(!pending.ability)pending.ability=active;else if(active!=pending.ability)Clear(pending);return;}
    if(!pending.finished){if(now-pending.started>1000&&!pending.ability)Clear(pending);return;}
    const bool swap=pending.mode==1;Clear(pending);
    // ECharacterSpawnType::DEFAULT=2, not SYNERGY=3: no second power/cost.
    if(swap&&g_request)g_request(reinterpret_cast<void*>(c.controller),2);
}
void ObserveFinish(void* ability,void* function){
    if(!g_pendingActive.load(std::memory_order_relaxed)||!native_tps::OnGameThread())return;
    for(auto& p:g_pending)if(p.ability.address==reinterpret_cast<uintptr_t>(ability)&&Alive(p.ability)&&
        Is(reinterpret_cast<uintptr_t>(function),"OnFinish"))p.finished=true;
}
void Initialize(LogFn log,bool exactExecutable,bool finishBoundaryReady){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));if(!g_base)return;
    constexpr unsigned char unlock[]={0x48,0x8B,0x89,0xA0,0x0E,0,0,0x48,0x85,0xC9,0x0F,0x85,0x20,0x8C,0x03,0,0x32,0xC0,0xC3};
    if(Guard(0x676730,unlock))g_counterReady=Install(0x676730,reinterpret_cast<void*>(&HookUnlock),reinterpret_cast<void**>(&g_unlock));
    constexpr unsigned char request[]={0x48,0x8B,0xC4,0x55,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8B,0xEC,0x48,0x83,0xEC,0x50};
    constexpr unsigned char synergy[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x80,0xB9,0x18,0x01,0,0,0x03};
    if(finishBoundaryReady&&Guard(0x5812F0,request)&&Guard(0x65CD00,synergy)){
        g_synergy=reinterpret_cast<Synergy>(g_base+0x65CD00);
        g_synergyReady=Install(0x5812F0,reinterpret_cast<void*>(&HookRequest),reinterpret_cast<void**>(&g_request));
    }
    if(log)log("Player ability options: retail native boundaries checked; Vanilla defaults; saved unlocks and bindings retained; EXPERIMENTAL");
}
bool CounterReady(){return g_counterReady;}
bool SynergyReady(){return g_synergyReady;}
void Shutdown(){g_pending={};g_pendingActive.store(false,std::memory_order_relaxed);}
}
