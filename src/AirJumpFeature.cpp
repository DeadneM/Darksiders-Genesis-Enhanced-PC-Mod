#include "AirJumpFeature.h"
#include "NativeMemoryReader.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <intrin.h>
#include <atomic>
#include <cstdio>

namespace dg::air_jump {
namespace {
using JumpFn=bool(*)(void*,bool);
using CountFn=int(*)(void*);
JumpFn g_jump=nullptr;CountFn g_count=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};unsigned g_rearms=0;
struct State {uintptr_t actor=0;uint32_t generation=0;int mode=0;bool gliding=false,credit=false;};
State g_players[2]{}; // synchronous game thread only
struct Scope {void* component=nullptr;bool allow=false,used=false;};
thread_local Scope g_scope{};
template<class T> bool Read(void* object,size_t offset,T& value){
    native_objects::MemoryReader reader;return reader.Read(reinterpret_cast<uintptr_t>(object),offset,value);
}
int Count(void* component,uintptr_t caller){
    const int count=g_count?g_count(component):0;
    // Only this read inside the native jump action. Animation, replication,
    // glide eligibility and every other consumer receive the real count.
    if(caller==0x69FCB5&&g_scope.component==component&&g_scope.allow&&count==2){g_scope.used=true;return 1;}
    return count;
}
int HookCount(void* component){return Count(component,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);}
bool HookJump(void* component,bool replayingMoves){
    if(!g_jump)return false;
    const Scope previous=g_scope;g_scope={};
    local_players::Context owner{};void* actor=nullptr;void* movement=nullptr;unsigned char mode=0,role=0;
    const int setting=runtime::Get().infiniteDoubleJumpMode.load();
    const bool eligible=g_ready.load()&&setting>=1&&setting<=2&&native_tps::OnGameThread()&&
        Read(component,0x850,actor)&&actor&&Read(actor,0x388,movement)&&movement==component&&
        Read(actor,0x118,role)&&role==3&&Read(component,0x1B0,mode)&&mode==3&&
        local_players::ResolveActor(actor,owner)&&owner.kind==local_players::PoseKind::Foot&&
        owner.character==reinterpret_cast<uintptr_t>(actor)&&
        !native_tps::IsWorldPaused(actor)&&!native_tps::CinematicActive(unsigned(owner.index));
    State* state=nullptr;
    if(eligible&&owner.index>=0&&owner.index<2){
        auto& s=g_players[owner.index];
        if(s.actor==owner.character&&s.generation==local_players::PublishedSlot(unsigned(owner.index)).generation&&
           s.mode==setting&&(setting==2||s.credit)){state=&s;g_scope={component,true,false};}
    }
    const bool result=g_jump(component,replayingMoves);
    const bool rearmed=result&&g_scope.used;
    g_scope=previous;
    if(rearmed){
        if(state&&setting==1)state->credit=false;
        if(g_log&&++g_rearms<=12){char line[150]{};sprintf_s(line,"Air jump V1.62 REARM player=%d mode=%d nativeActionSucceeded=1 EXPERIMENTAL",owner.index+1,setting);g_log(line);}
    }
    return result;
}
}
void OnLivePlayer(const local_players::Context& owner,uint32_t generation){
    if(!g_ready.load()||!native_tps::OnGameThread()||owner.index<0||owner.index>1)return;
    auto& state=g_players[owner.index];const int setting=runtime::Get().infiniteDoubleJumpMode.load();
    if(state.actor!=owner.character||state.generation!=generation||state.mode!=setting)
        state={owner.character,generation,setting,false,false};
    void* movement=nullptr;unsigned char mode=0,flags=0;
    if(setting<1||setting>2||owner.kind!=local_players::PoseKind::Foot||
       !Read(reinterpret_cast<void*>(owner.character),0x388,movement)||
       !Read(movement,0x1B0,mode)||mode!=3||
       !Read(reinterpret_cast<void*>(owner.character),0xD14,flags)){
        state.gliding=false;state.credit=false;return;
    }
    const bool gliding=(flags&1)!=0;
    if(gliding&&!state.gliding)state.credit=true;
    state.gliding=gliding;
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]{
        {0x69FC70,{0x48,0x89,0x5C,0x24,0x18,0x55,0x57,0x41,0x56,0x48,0x8D,0x6C,0x24,0xA0,0x48,0x81}},
        {0x56D400,{0x48,0x8B,0x81,0x40,0x07,0x00,0x00,0x48,0x85,0xC0,0x74,0x07,0x8B,0x80,0x74,0x09}},
        {0x6A0152,{0x83,0xFF,0x02,0x0F,0x8D,0xF0,0x01,0x00,0x00,0x48,0x8B,0x03,0x45,0x33,0xC0,0x48}}
    };
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16))return;
    const auto init=MH_Initialize();if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* a=reinterpret_cast<void*>(g_base+0x69FC70);void* b=reinterpret_cast<void*>(g_base+0x56D400);
    const bool first=MH_CreateHook(a,reinterpret_cast<void*>(&HookJump),reinterpret_cast<void**>(&g_jump))==MH_OK;
    const bool second=first&&MH_CreateHook(b,reinterpret_cast<void*>(&HookCount),reinterpret_cast<void**>(&g_count))==MH_OK;
    if(second&&MH_EnableHook(b)==MH_OK&&MH_EnableHook(a)==MH_OK){g_ready.store(true,std::memory_order_release);
        if(log)log("Air jump V1.62 READY: native jump action/count read, Off/After gliding/Always; no counter/velocity/animation writes; EXPERIMENTAL");return;}
    if(first){MH_DisableHook(a);MH_RemoveHook(a);}if(second){MH_DisableHook(b);MH_RemoveHook(b);}g_jump=nullptr;g_count=nullptr;
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
}
