#include "HeavyLandingFeature.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <array>
#include <atomic>
#include <cmath>
#include <cstdio>

namespace dg::heavy_landing {
namespace {
// EAbilityInterrupt::MOVE, not ECharacterAction::MOVE (29).
constexpr uint64_t MoveBit=uint64_t(1)<<1;
using Interrupt=void(*)(void*,unsigned char,bool);
Interrupt g_window=nullptr,g_interrupt=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};unsigned g_changes=0;
struct Saved {native_objects::Identity identity{};unsigned owner=0;float nativeWindow=0;bool seen=false;};
std::array<Saved,8> g_saved{}; // only early native MOVE windows, game thread
struct Ground {uintptr_t actor=0;uint32_t generation=0;uint64_t since=0,last=0;};
Ground g_ground[2]{};
template<class T> bool Read(uintptr_t object,size_t offset,T& value){native_objects::MemoryReader r;return r.Read(object,offset,value);}
void Restore(Saved& saved){
    if(!saved.identity)return;
    native_objects::MemoryReader reader;unsigned char state=0;float elapsed=0;
    const auto p=saved.identity.address;
    if(g_window&&g_interrupt&&native_objects::Alive(reader,g_base,saved.identity)&&Read(p,0xD8,state)&&state==2&&
       Read(p,0xDC,elapsed)&&std::isfinite(elapsed)&&elapsed>=0&&elapsed<saved.nativeWindow){
        // Undo only our early window. The original notify owns the later one.
        // Native APIs also reconcile the input/action maps.
        g_interrupt(reinterpret_cast<void*>(p),1,false);
        if(native_objects::Alive(reader,g_base,saved.identity))g_window(reinterpret_cast<void*>(p),1,false);
    }
    saved={};
}
void RestorePlayer(unsigned index){for(auto& saved:g_saved)if(saved.identity&&saved.owner==index)Restore(saved);}
bool Heavy(uintptr_t ability,unsigned char type){
    uintptr_t cls=0;if(!Read(ability,0x10,cls))return false;
    return type==1?native_objects::NameIs(g_base,cls,"MayhemAbility_Strife_HeavyLand_C"):
        type==2&&native_objects::NameIs(g_base,cls,"MayhemAbility_War_HeavyLand_C");
}
bool Montage(uintptr_t ability,unsigned char type){
    uintptr_t helper=0,cls=0,montage=0,package=0;float rate=0;
    if(!Read(ability,0x1D8,helper)||!Read(helper,0x10,cls)||!native_objects::NameIs(g_base,cls,"MayhemMontagePlayer")||
       !Read(helper,0x28,montage)||!Read(montage,0x10,cls)||!native_objects::NameIs(g_base,cls,"AnimMontage")||
       !Read(montage,0x94,rate)||rate!=1||!Read(montage,0x20,package))return false;
    return type==1?(native_objects::NameIs(g_base,montage,"Strife_Jump_Land_Heavy_Montage")&&
        native_objects::NameIs(g_base,package,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Jump_Land_Heavy_Montage")):
        (native_objects::NameIs(g_base,montage,"War_Jump_Land_Heavy_Montage")&&
        native_objects::NameIs(g_base,package,"/Game/Characters/Heroes/Hero_War/Animation/War_Jump_Land_Heavy_Montage"));
}
void Service(const local_players::Context& owner,uint32_t generation,bool focused,uint64_t now){
    if(!g_ready.load(std::memory_order_acquire)||!g_window||!g_interrupt||!native_tps::OnGameThread()||owner.index<0||owner.index>1)return;
    const unsigned index=unsigned(owner.index);auto& ground=g_ground[index];
    if(!runtime::Get().heavyLandingRecoveryEnabled.load()||!focused||owner.kind!=local_players::PoseKind::Foot||
       !owner.localPlayer||!local_players::Matches(index,owner.character,generation)||owner.pawn!=owner.character||
       native_tps::IsWorldPaused(reinterpret_cast<void*>(owner.pawn))||native_tps::CinematicActive(index)||native_tps::RadialActive(index)){
        RestorePlayer(index);ground={};return;
    }
    uintptr_t controller=0,pawn=0,component=0,actor=0,movement=0,resources=0,array=0;
    int count=0,capacity=0;unsigned char role=0,type=0,mode=0,custom=0;float health=0;
    const bool valid=owner.controller&&Read(owner.character,0x348,controller)&&controller==owner.controller&&
        Read(controller,0x350,pawn)&&pawn==owner.character&&Read(pawn,0x118,role)&&role==3&&
        Read(pawn,0xC18,type)&&(type==1||type==2)&&Read(pawn,0xC70,component)&&
        Read(component,0xE8,actor)&&actor==pawn&&Read(component,0xF8,array)&&Read(component,0x100,count)&&
        Read(component,0x104,capacity)&&count>=0&&count<=capacity&&capacity<=64&&(!count||array)&&
        Read(pawn,0x388,movement)&&Read(movement,0x1B0,mode)&&Read(movement,0x1B1,custom)&&
        Read(pawn,0xEA0,resources)&&Read(resources,0x37C,health)&&std::isfinite(health)&&health>0;
    if(!valid||mode!=1||custom){RestorePlayer(index);ground={};return;}
    const bool same=ground.actor==pawn&&ground.generation==generation&&ground.last&&now>=ground.last&&now-ground.last<=250;
    if(!same)ground={pawn,generation,now,now};
    else ground.last=now;
    for(auto& saved:g_saved)if(saved.owner==index)saved.seen=false;
    // Same native window as AnimNotify_EnableAbilityInterrupt. The engine
    // handles held/rebound movement, cancellation and montage cleanup.
    if(now>=ground.since&&now-ground.since>=50){
        for(int n=0;n<count;++n){
            uintptr_t ability=0,instigator=0,character=0,ownMovement=0;unsigned char state=0,pending=0;
            uint64_t requested=0,enabled=0;float elapsed=0;
            const float nativeWindow=type==1?.6684650183f:.4024589956f;
            if(!Read(array,size_t(n)*8,ability)||!Heavy(ability,type)||!Read(ability,0x48,instigator)||instigator!=pawn||
               !Read(ability,0x1B0,character)||character!=pawn||!Read(ability,0x1C8,ownMovement)||ownMovement!=movement||
               !Read(ability,0xD8,state)||state!=2||!Read(ability,0x148,pending)||pending||!Read(ability,0xDC,elapsed)||!std::isfinite(elapsed)||
               elapsed<0.15f||elapsed>=nativeWindow||!Montage(ability,type)||
               !Read(ability,0x120,requested)||!Read(ability,0xB0,enabled))continue;
            native_objects::Identity identity{};
            if(!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(ability),identity))continue;
            Saved* saved=nullptr;
            for(auto& entry:g_saved)if(entry.identity==identity){saved=&entry;break;}
            if(!saved){
                if((requested|enabled)&MoveBit)continue; // pre-existing native window
                for(auto& entry:g_saved)if(!entry.identity){saved=&entry;break;}
                if(!saved)continue;
                *saved={identity,index,nativeWindow,true};
            }
            saved->seen=true;
            if(!(requested&MoveBit)){
                g_window(reinterpret_cast<void*>(ability),1,true);
                if(g_log&&++g_changes<=8){char line[190]{};sprintf_s(line,
                    "Heavy landing V1.103: player=%u native MOVE interruption window opened after impact; recovery/cancellation remain native; EXPERIMENTAL",index+1);g_log(line);}
            }
        }
    }
    for(auto& saved:g_saved)if(saved.identity&&saved.owner==index&&!saved.seen)Restore(saved);
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char entry[]={0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF};
    constexpr unsigned char notify[]={0xE8,0xB2,0x17,0x00,0x00}; // NotifyBegin -> 5A9D60
    constexpr unsigned char window[]={0x48,0x8B,0x81,0x20,0x01,0x00,0x00,0x48,0x0F,0xAB,0xD0};
    if(!g_base||std::memcmp(reinterpret_cast<void*>(g_base+0x5A9D60),entry,sizeof(entry))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x599310),entry,sizeof(entry))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x5A85A9),notify,sizeof(notify))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x5A9DF8),window,sizeof(window)))return;
    g_window=reinterpret_cast<Interrupt>(g_base+0x5A9D60);
    g_interrupt=reinterpret_cast<Interrupt>(g_base+0x599310);
    g_ready=true;
    if(log)log("Heavy landing V1.103 READY: exact HeavyLand, grounded post-impact native MOVE window; no hook, forced Finish, input or source-animation edits; EXPERIMENTAL");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
void OnLivePlayer(const local_players::Context& owner,uint32_t generation,bool focusedGameplay){Service(owner,generation,focusedGameplay,GetTickCount64());}
void Shutdown(){g_ready=false;} // teardown does not dereference cached game objects from this thread
}
