#include "AirJumpFeature.h"
#include "NativeMemoryReader.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <intrin.h>
#include <atomic>
#include <cstdio>
#include <cmath>

namespace dg::air_jump {
namespace {
using JumpFn=bool(*)(void*,bool);
using CountFn=int(*)(void*);
using ActorFn=void(*)(void*);
JumpFn g_jump=nullptr;CountFn g_count=nullptr;ActorFn g_input=nullptr,g_stopGlide=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};unsigned g_rearms=0;
struct State {uintptr_t actor=0;uint32_t generation=0;int mode=0;bool gliding=false,credit=false;};
State g_players[2]{}; // synchronous game thread only
struct Scope {void* component=nullptr;bool allow=false,used=false;};
thread_local Scope g_scope{};
template<class T> bool Read(void* object,size_t offset,T& value){
    native_objects::MemoryReader reader;return reader.Read(reinterpret_cast<uintptr_t>(object),offset,value);
}
bool Eligible(void* component,local_players::Context& owner,State*& state,bool fromGlide=false){
    void* actor=nullptr;void* movement=nullptr;unsigned char mode=0,role=0;
    const int setting=runtime::Get().infiniteDoubleJumpMode.load();
    if(!g_ready.load()||setting<1||setting>2||!native_tps::OnGameThread()||
       !Read(component,0x850,actor)||!actor||!Read(actor,0x388,movement)||movement!=component||
       !Read(actor,0x118,role)||role!=3||!Read(component,0x1B0,mode)||mode!=(fromGlide?6:3)||
       !local_players::ResolveActor(actor,owner)||owner.kind!=local_players::PoseKind::Foot||
       owner.character!=reinterpret_cast<uintptr_t>(actor)||owner.index<0||owner.index>1||
       native_tps::IsWorldPaused(actor)||native_tps::CinematicActive(unsigned(owner.index)))return false;
    if(fromGlide){
        unsigned char custom=0,flags=0;
        if(!Read(component,0x1B1,custom)||custom!=6||!Read(actor,0xD14,flags)||!(flags&1))return false;
        // A short glide can begin and end between camera observations.
        // This live native jump request also observes its proven glide state.
        OnLivePlayer(owner,local_players::PublishedSlot(unsigned(owner.index)).generation);
    }
    // Always needs no camera-observer lease. Resolve the live actor each time.
    // The one-use glide credit still belongs to the observed actor/generation.
    if(setting==2)return true;
    auto& s=g_players[owner.index];
    if(s.actor!=owner.character||s.generation!=local_players::PublishedSlot(unsigned(owner.index)).generation||
       s.mode!=setting||!s.credit)return false;
    state=&s;return true;
}
bool Exhausted(int count){return count==2;}
int Count(void* component,uintptr_t caller){
    const int count=g_count?g_count(component):0;
    if(!Exhausted(count))return count;
    // The capture keeps ReplicatedJumpCount at two while UE's own count
    // reaches three. Leave admission, animation and replication reads native.
    if(caller==0x69FCB5&&g_scope.component==component&&g_scope.allow){g_scope.used=true;return 1;}
    return count; // animation, replication, glide and special restricted branch
}
int HookCount(void* component){return Count(component,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);}
bool HookJump(void* component,bool replayingMoves){
    if(!g_jump)return false;
    const Scope previous=g_scope;g_scope={};
    local_players::Context owner{};State* state=nullptr;
    const bool eligible=Eligible(component,owner,state);
    float originalDelay=0,elapsed=0,appliedDelay=0;bool extended=false;
    if(eligible){
        g_scope={component,true,false};
        // The native impulse also expires against MaxDoubleJumpDelaySec.
        // Extend this parameter only during an exhausted extra jump; native
        // code supplies the impulse and performs all state transitions.
        if(g_count&&Exhausted(g_count(component))&&Read(component,0x860,originalDelay)&&
           Read(component,0x9E8,elapsed)&&std::isfinite(originalDelay)&&originalDelay>=0&&
           std::isfinite(elapsed)&&elapsed>originalDelay&&elapsed<1e6f){
            appliedDelay=elapsed;
            std::memcpy(static_cast<unsigned char*>(component)+0x860,&appliedDelay,sizeof(appliedDelay));extended=true;
        }
    }
    const bool result=g_jump(component,replayingMoves);
    const bool rearmed=result&&g_scope.used;
    g_scope=previous;
    float current=0;
    if(extended&&Read(component,0x860,current)&&current==appliedDelay)
        std::memcpy(static_cast<unsigned char*>(component)+0x860,&originalDelay,sizeof(originalDelay));
    if(rearmed){
        if(state)state->credit=false;
        if(g_log&&++g_rearms<=12){char line[150]{};sprintf_s(line,"Air jump V1.65 REARM player=%d mode=%d nativeActionSucceeded=1 EXPERIMENTAL",owner.index+1,runtime::Get().infiniteDoubleJumpMode.load());g_log(line);}
    }
    return result;
}
void HookInput(void* actor){
    void* component=nullptr;local_players::Context owner{};State* state=nullptr;
    if(g_stopGlide&&Read(actor,0x388,component)&&Eligible(component,owner,state,true)&&
       owner.character==reinterpret_cast<uintptr_t>(actor)){
        // Native CanJump rejects D14.Gliding before DoJump is reached.
        // End this glide through the game's own function on a jump request;
        // then let the original input/FSM decide whether jumping is allowed.
        g_stopGlide(actor);
    }
    if(g_input)g_input(actor);
}
}
void OnLivePlayer(const local_players::Context& owner,uint32_t generation){
    if(!g_ready.load()||!native_tps::OnGameThread()||owner.index<0||owner.index>1)return;
    auto& state=g_players[owner.index];const int setting=runtime::Get().infiniteDoubleJumpMode.load();
    if(state.actor!=owner.character||state.generation!=generation||state.mode!=setting)
        state={owner.character,generation,setting,false,false};
    void* movement=nullptr;unsigned char mode=0,custom=0,flags=0;
    if(setting<1||setting>2||owner.kind!=local_players::PoseKind::Foot||
       !Read(reinterpret_cast<void*>(owner.character),0x388,movement)||
       !Read(movement,0x1B0,mode)||!Read(movement,0x1B1,custom)||
       (mode!=3&&!(mode==6&&custom==6))||
       !Read(reinterpret_cast<void*>(owner.character),0xD14,flags)){
        state.gliding=false;state.credit=false;return;
    }
    // StartGliding 6886A0 selects MOVE_Custom(6), subtype Gliding(6).
    // Treating every non-Falling mode as landing erased the credit here.
    const bool gliding=mode==6&&custom==6&&(flags&1)!=0;
    if(gliding&&!state.gliding)state.credit=true;
    state.gliding=gliding;
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]{
        {0x69FC70,{0x48,0x89,0x5C,0x24,0x18,0x55,0x57,0x41,0x56,0x48,0x8D,0x6C,0x24,0xA0,0x48,0x81}},
        {0x56D400,{0x48,0x8B,0x81,0x40,0x07,0x00,0x00,0x48,0x85,0xC0,0x74,0x07,0x8B,0x80,0x74,0x09}},
        {0x6A017A,{0xF3,0x0F,0x10,0x83,0x60,0x08,0x00,0x00,0x0F,0x2F,0x83,0xE8,0x09,0x00,0x00,0x72}},
        {0x583080,{0x48,0x8B,0x01,0x44,0x0F,0xB6,0xC2,0xBA,0x06,0x00,0x00,0x00,0x48,0xFF,0xA0,0x70}},
        {0x6886FF,{0xB2,0x06,0x48,0x8B,0xCF,0xE8,0x77,0xA9,0xEF,0xFF,0x48,0x8B,0x03,0x48,0x8B,0xCB}},
        {0x676910,{0x48,0x89,0x5C,0x24,0x10,0x57,0x48,0x83,0xEC,0x30,0x48,0x8B,0x01,0x48,0x8B,0xD9}},
        {0x689120,{0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8B,0x01,0x48,0x8B,0xD9,0xFF,0x90,0x80,0x06}},
        {0x6A0152,{0x83,0xFF,0x02,0x0F,0x8D,0xF0,0x01,0x00,0x00,0x48,0x8B,0x03,0x45,0x33,0xC0,0x48}}
    };
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16))return;
    const auto init=MH_Initialize();if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return;
    struct Hook {uintptr_t rva;void* detour;void** original;};
    const Hook hooks[]{
        {0x69FC70,reinterpret_cast<void*>(&HookJump),reinterpret_cast<void**>(&g_jump)},
        {0x56D400,reinterpret_cast<void*>(&HookCount),reinterpret_cast<void**>(&g_count)},
        {0x676910,reinterpret_cast<void*>(&HookInput),reinterpret_cast<void**>(&g_input)}
    };
    unsigned created=0;bool enabled=true;
    for(const auto& h:hooks){
        if(MH_CreateHook(reinterpret_cast<void*>(g_base+h.rva),h.detour,h.original)!=MH_OK){enabled=false;break;}
        ++created;
    }
    g_stopGlide=reinterpret_cast<ActorFn>(g_base+0x689120);
    if(enabled)for(const auto& h:hooks)if(MH_EnableHook(reinterpret_cast<void*>(g_base+h.rva))!=MH_OK){enabled=false;break;}
    if(enabled){g_ready.store(true,std::memory_order_release);
        if(log)log("Air jump V1.65 READY: native jump request ends owned glide, native extra-jump action/count and scoped deadline; Off/After gliding/Always; no counter/velocity/animation writes; EXPERIMENTAL");return;}
    for(unsigned i=0;i<created;++i){void* target=reinterpret_cast<void*>(g_base+hooks[i].rva);MH_DisableHook(target);MH_RemoveHook(target);}
    g_jump=nullptr;g_count=nullptr;g_input=nullptr;g_stopGlide=nullptr;
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
}
