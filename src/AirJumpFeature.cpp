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
using SetCountFn=void(*)(void*,int);
using ActorFn=void(*)(void*);
JumpFn g_jump=nullptr;CountFn g_count=nullptr;
SetCountFn g_setCount=nullptr,g_setAirCount=nullptr;
ActorFn g_input=nullptr,g_stopGlide=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};unsigned g_rearms=0;
struct State {uintptr_t actor=0;uint32_t generation=0;int mode=0;bool gliding=false,credit=false,armed=false;};
State g_players[2]{}; // synchronous game thread only
template<class T> bool Read(void* object,size_t offset,T& value){
    native_objects::MemoryReader reader;return reader.Read(reinterpret_cast<uintptr_t>(object),offset,value);
}
bool Owned(void* component,local_players::Context& owner){
    void* actor=nullptr;void* movement=nullptr;void* countOwner=nullptr;unsigned char role=0;
    return g_ready.load()&&native_tps::OnGameThread()&&
        Read(component,0x850,actor)&&actor&&Read(component,0x740,countOwner)&&countOwner==actor&&
        Read(actor,0x388,movement)&&movement==component&&Read(actor,0x118,role)&&role==3&&
        local_players::ResolveActor(actor,owner)&&owner.kind==local_players::PoseKind::Foot&&
        owner.character==reinterpret_cast<uintptr_t>(actor)&&owner.index>=0&&owner.index<2;
}
void Restore(void* component,State& state){
    unsigned char mode=0,air=0;
    // A later native jump, landing or other native value always wins.
    if(state.armed&&g_count&&g_setCount&&g_setAirCount&&g_count(component)==1&&
       Read(component,0x1B0,mode)&&mode==3&&Read(component,0xA00,air)&&air==1){
        g_setCount(component,2);g_setAirCount(component,2);
    }
    state.armed=false;
}
State& Current(void* component,const local_players::Context& owner){
    auto& state=g_players[owner.index];const auto generation=local_players::PublishedSlot(unsigned(owner.index)).generation;
    const int setting=runtime::Get().infiniteDoubleJumpMode.load();
    if(state.actor!=owner.character||state.generation!=generation||state.mode!=setting){
        if(state.actor==owner.character&&state.generation==generation)Restore(component,state);
        state={owner.character,generation,setting,false,false,false};
    }
    return state;
}
bool Eligible(void* component,const local_players::Context& owner,const State& state,bool glide=false){
    unsigned char mode=0,custom=0,flags=0;
    auto* actor=reinterpret_cast<void*>(owner.character);
    return state.mode>=1&&state.mode<=2&&(state.mode==2||state.credit)&&
        !native_tps::IsWorldPaused(actor)&&!native_tps::CinematicActive(unsigned(owner.index))&&
        Read(component,0x1B0,mode)&&Read(actor,0xD14,flags)&&!(flags&0x1E)&&
        (glide?(mode==6&&Read(component,0x1B1,custom)&&custom==6&&(flags&1)):(mode==3&&!(flags&1)));
}
bool Rearm(void* component,State& state){
    unsigned char air=0;
    if(state.armed||!g_count||!g_setCount||!g_setAirCount||g_count(component)!=2||
       !Read(component,0xA00,air)||air!=2)return false;
    // Use the native setters as a pair: the replicated count feeds animation;
    // A00 feeds automatic glide admission. Rearming only DoJump split these.
    g_setCount(component,1);g_setAirCount(component,1);
    if(g_count(component)==1&&Read(component,0xA00,air)&&air==1){state.armed=true;return true;}
    // One setter may decline a non-local owner. Undo only our exact value.
    if(g_count(component)==1)g_setCount(component,2);
    if(Read(component,0xA00,air)&&air==1)g_setAirCount(component,2);
    return false;
}
bool AnimationSawDoubleJump(const local_players::Context& owner){
    void* mesh=nullptr;void* anim=nullptr;int count=0;unsigned char jump=0,pressed=0;
    auto* actor=reinterpret_cast<void*>(owner.character);
    return Read(actor,0xE10+30,jump)&&!jump&&Read(actor,0x430,pressed)&&!(pressed&4)&&
        Read(actor,0x380,mesh)&&Read(mesh,0x718,anim)&&anim&&Read(anim,0x2E8,count)&&count==2;
}
int Count(void* component,uintptr_t caller){
    if(caller!=0x68D65B||!native_tps::OnGameThread()||
       (runtime::Get().infiniteDoubleJumpMode.load()==0&&!g_players[0].armed&&!g_players[1].armed))return g_count?g_count(component):0;
    local_players::Context owner{};
    if(Owned(component,owner)){
        auto& state=Current(component,owner);
        // 68D656 copies this real native getter into AnimInstance.JumpCount.
        // Publish the available jump here, after animation has seen count=2
        // and the native jump action is released. The SAME update sees 1;
        // the next successful native jump publishes 2 again. No fake getter
        // result, animation-node reset, UE jump-counter or velocity write.
        if(Eligible(component,owner,state)){
            if(AnimationSawDoubleJump(owner))Rearm(component,state);
        }else if(state.armed)Restore(component,state);
    }
    return g_count?g_count(component):0;
}
int HookCount(void* component){return Count(component,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);}
bool HookJump(void* component,bool replayingMoves){
    if(!g_jump)return false;
    local_players::Context owner{};State* state=nullptr;
    if(Owned(component,owner)){
        auto& current=Current(component,owner);
        if(Eligible(component,owner,current)&&current.armed&&g_count&&g_count(component)==1)state=&current;
        else if(current.armed)Restore(component,current);
    }
    float originalDelay=0,elapsed=0,appliedDelay=0;bool extended=false;
    if(state&&Read(component,0x860,originalDelay)&&Read(component,0x9E8,elapsed)&&
       std::isfinite(originalDelay)&&originalDelay>=0&&std::isfinite(elapsed)&&elapsed>originalDelay&&elapsed<1e6f){
        appliedDelay=elapsed;
        std::memcpy(static_cast<unsigned char*>(component)+0x860,&appliedDelay,sizeof(appliedDelay));extended=true;
    }
    const bool result=g_jump(component,replayingMoves);
    float currentDelay=0;
    if(extended&&Read(component,0x860,currentDelay)&&currentDelay==appliedDelay)
        std::memcpy(static_cast<unsigned char*>(component)+0x860,&originalDelay,sizeof(originalDelay));
    if(state&&result&&g_count(component)==2){
        state->armed=false;state->credit=false;
        if(g_log&&++g_rearms<=12){char line[150]{};sprintf_s(line,"Air jump V1.66 CONSUMED player=%d mode=%d realNativeCount=2 animationCycle=EXPERIMENTAL",owner.index+1,state->mode);g_log(line);}
    }
    return result;
}
void HookInput(void* actor){
    void* component=nullptr;local_players::Context owner{};
    if(g_stopGlide&&Read(actor,0x388,component)&&Owned(component,owner)){
        auto& state=Current(component,owner);
        // Observe the actual glide before ending it. This also catches glides
        // shorter than the local-player observation interval.
        unsigned char mode=0,custom=0,flags=0;
        if(Read(component,0x1B0,mode)&&mode==6&&Read(component,0x1B1,custom)&&custom==6&&
           Read(actor,0xD14,flags)&&(flags&1)){
            if(!state.gliding)state.credit=true;
            state.gliding=true;
            if(Eligible(component,owner,state,true)){
                g_stopGlide(actor);state.gliding=false;
                if(Eligible(component,owner,state))Rearm(component,state);
            }
        }
    }
    if(g_input)g_input(actor); // exactly one native request; all bindings native
}
}
void OnLivePlayer(const local_players::Context& owner,uint32_t generation){
    if(!g_ready.load()||!native_tps::OnGameThread()||owner.index<0||owner.index>1)return;
    void* movement=nullptr;auto* actor=reinterpret_cast<void*>(owner.character);
    if(owner.kind!=local_players::PoseKind::Foot||!Read(actor,0x388,movement)){
        g_players[owner.index]={};return;
    }
    local_players::Context live{};if(!Owned(movement,live))return;
    auto& state=Current(movement,live);
    if(state.generation!=generation)return;
    unsigned char mode=0,custom=0,flags=0;
    if(state.mode<1||state.mode>2||!Read(movement,0x1B0,mode)||!Read(movement,0x1B1,custom)||
       !Read(actor,0xD14,flags)||(mode!=3&&!(mode==6&&custom==6))){
        state.gliding=false;state.credit=false;state.armed=false;return;
    }
    const bool gliding=mode==6&&custom==6&&(flags&1)!=0;
    if(gliding&&!state.gliding)state.credit=true;
    state.gliding=gliding;
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]{
        {0x669D30,{0x48,0x81,0xC1,0x40,0x05,0x00,0x00,0xE9,0xC4,0xB3,0xFC,0xFF,0xCC,0xCC,0xCC,0xCC}},
        {0x635100,{0x0F,0xB6,0xC2,0x0F,0xB6,0x04,0x08,0xC3,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC}},
        {0x582FF0,{0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x8B,0xFA,0x48}},
        {0x6BA9F0,{0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x8B,0xFA,0x48}},
        {0x68D656,{0xE8,0xA5,0xFD,0xED,0xFF,0x89,0x86,0xE8,0x02,0x00,0x00,0x49,0x8B,0xCE,0x41,0x0F}},
        {0x678C90,{0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0xE8,0xF2,0x57,0xF9,0xFF,0x0F,0xB6}},
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
    g_setCount=reinterpret_cast<SetCountFn>(g_base+0x582FF0);
    g_setAirCount=reinterpret_cast<SetCountFn>(g_base+0x6BA9F0);
    if(enabled)for(const auto& h:hooks)if(MH_EnableHook(reinterpret_cast<void*>(g_base+h.rva))!=MH_OK){enabled=false;break;}
    if(enabled){g_ready.store(true,std::memory_order_release);
        if(log)log("Air jump V1.66 READY: paired native count rearm observed by animation; native jump action and scoped deadline; no getter substitution/UE counter/velocity/animation-node writes; EXPERIMENTAL");return;}
    for(unsigned i=0;i<created;++i){void* target=reinterpret_cast<void*>(g_base+hooks[i].rva);MH_DisableHook(target);MH_RemoveHook(target);}
    g_jump=nullptr;g_count=nullptr;g_input=nullptr;g_stopGlide=nullptr;g_setCount=nullptr;g_setAirCount=nullptr;
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
}
