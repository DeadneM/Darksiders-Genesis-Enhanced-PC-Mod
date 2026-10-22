#include "MountedJumpFeature.h"
#include "LocalPlayerOwnership.h"
#include "NativeMemoryReader.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <array>
#include <atomic>
#include <cstdio>
#include <cmath>

namespace dg::mounted_jump {
namespace {
using FindKey=int*(*)(void*,int*,const void*);
using Jump=bool(*)(void*,bool);
FindKey g_find=nullptr;Jump g_jump=nullptr;
struct Vector {float x=0,y=0,z=0;};
// execPlaySoundAtLocation 1D16FA0 -> 18911C0. Ten arguments, vector and
// rotation by const reference. No borrowed FString, FKey or shared ownership.
using PlaySound=void(*)(void*,void*,const Vector*,const Vector*,float,float,float,void*,void*,void*);
PlaySound g_sound=nullptr;
native_objects::Identity g_sounds[3]{};std::atomic_uint g_soundMask{0};
int g_soundCursor=0,g_soundCount=0;bool g_soundComplete=false;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};unsigned g_jumps=0;
struct Name {int index=0,number=0;};
struct Mapping {Name action{};unsigned char modifiers=0,padding[7]{};Name key{};uintptr_t details=0,reference=0;};
struct Array {uintptr_t data=0;int count=0,capacity=0;};
static_assert(sizeof(Mapping)==40&&offsetof(Mapping,key)==16);
struct Bindings {uintptr_t settings=0,data=0;int count=0;uint64_t refresh=0;std::array<int,8> indices{};unsigned size=0;};
Bindings g_bindings{};
struct Landing {uint64_t tick=0;bool airborne=false;};
struct State {uintptr_t pawn=0,controller=0;uint32_t generation=0;uint64_t signature=0,tick=0;bool seen=false,down=false;Landing landing{};};
State g_players[2]{}; // synchronous native game-thread observations only
template<class T> bool Read(uintptr_t object,size_t offset,T& value){
    native_objects::MemoryReader reader;return reader.Read(object,offset,value);
}
int SoundKind(uintptr_t object){
    uintptr_t cls=0,package=0;
    if(!Read(object,0x10,cls)||!native_objects::NameIs(g_base,cls,"SoundCue")||!Read(object,0x20,package))return -1;
    if(native_objects::NameIs(g_base,object,"char_ruin_jump")&&native_objects::NameIs(g_base,package,"/Game/Audio/Sfx/Character/Ruin/char_ruin_jump"))return 0;
    if(native_objects::NameIs(g_base,object,"char_ruin_jump_land")&&native_objects::NameIs(g_base,package,"/Game/Audio/Sfx/Character/Ruin/char_ruin_jump_land"))return 1;
    if(native_objects::NameIs(g_base,object,"char_ruin_snort_low_vo")&&native_objects::NameIs(g_base,package,"/Game/Audio/Sfx/Character/Ruin/char_ruin_snort_low_vo"))return 2;
    return -1;
}
void Sounds(){
    if(!g_sound)return;
    native_objects::MemoryReader reader;unsigned mask=0;bool expired=false;
    for(unsigned i=0;i<3;++i){
        if(native_objects::Alive(reader,g_base,g_sounds[i]))mask|=1u<<i;
        else if(g_sounds[i]){g_sounds[i]={};expired=true;}
    }
    if(expired){g_soundCursor=0;g_soundComplete=false;}
    int count=0;uintptr_t chunks=0;
    if(mask!=7&&Read(g_base,0x3959694,count)&&count>0&&count<=2000000&&Read(g_base,0x3959680,chunks)){
        if(count!=g_soundCount){g_soundCount=count;g_soundComplete=false;}
        // One bounded pass over loaded assets, with new appended objects read
        // if the object table grows. Never load packages or spin on a miss.
        if(!g_soundComplete)for(unsigned n=0;n<512&&g_soundCursor<count;++n,++g_soundCursor){
            uintptr_t chunk=0,object=0;uint32_t flags=0;
            if(!Read(chunks,size_t(g_soundCursor/65536)*8,chunk)||!Read(chunk,size_t(g_soundCursor%65536)*24,object)||
               !object||!Read(chunk,size_t(g_soundCursor%65536)*24+8,flags)||(flags&0x30000000u))continue;
            const int kind=SoundKind(object);
            if(kind>=0&&native_objects::CaptureLive(g_base,reinterpret_cast<void*>(object),g_sounds[kind]))mask|=1u<<kind;
        }
        if(g_soundCursor>=count)g_soundComplete=true;
    }
    g_soundMask.store(mask,std::memory_order_release);
}
void Sound(const local_players::Context& owner,bool landing){
    const int choice=runtime::Get().mountedJumpSound.load();
    if(!g_sound||choice<0||choice>=2)return;
    const unsigned kind=landing?1u:choice==1?2u:0u;
    native_objects::MemoryReader reader;uintptr_t root=0,actor=0;Vector position{},rotation{};
    if(!native_objects::Alive(reader,g_base,g_sounds[kind])||SoundKind(g_sounds[kind].address)!=int(kind)||
       !Read(owner.pawn,0x158,root)||!Read(root,0xE8,actor)||actor!=owner.pawn||!Read(root,0x1A0,position)||
       !std::isfinite(position.x)||!std::isfinite(position.y)||!std::isfinite(position.z))return;
    // Native SoundCue supplies Sfx volume, attenuation, mixer and random
    // samples. A local successful leap owns these two one-shot requests.
    g_sound(reinterpret_cast<void*>(owner.pawn),reinterpret_cast<void*>(g_sounds[kind].address),&position,&rotation,
        landing?0.9f:1.0f,1.0f,0.0f,nullptr,nullptr,reinterpret_cast<void*>(owner.pawn));
}
bool Owned(const local_players::Context& owner,uintptr_t& movement){
    const auto actor=owner.pawn;
    uintptr_t cls=0,controller=0,pawn=0,table=0,entry=0,reciprocal=0;unsigned char role=0,despawn=0;
    return owner.kind==local_players::PoseKind::Horse&&owner.localPlayer&&owner.character&&actor&&actor!=owner.character&&
        Read(actor,0x10,cls)&&(native_objects::NameIs(g_base,cls,"Despair_Blueprint_C")||native_objects::NameIs(g_base,cls,"Ruin_Blueprint_C"))&&
        Read(actor,0x118,role)&&role==3&&Read(actor,0x348,controller)&&controller==owner.controller&&
        Read(controller,0x350,pawn)&&pawn==actor&&Read(actor,0xA66,despawn)&&!despawn&&
        Read(actor,0x388,movement)&&Read(movement,0xE8,reciprocal)&&reciprocal==actor&&Read(movement,0,table)&&
        table==g_base+0x270ABF0&&Read(table,0x600,entry)&&entry==g_base+0x667510;
}
bool Keys(std::array<Mapping,8>& keys,unsigned& count,uint64_t now){
    uintptr_t cls=0,settings=0;Array mappings{};
    if(!Read(g_base,0x3A5E1C0,cls)||!native_objects::NameIs(g_base,cls,"InputSettings")||!Read(cls,0xF8,settings)||
       !Read(settings,0x80,mappings)||!mappings.data||mappings.count<1||mappings.count>mappings.capacity||mappings.capacity>2048)return false;
    auto& cache=g_bindings;
    if(cache.settings!=settings||cache.data!=mappings.data||cache.count!=mappings.count||now<cache.refresh||now-cache.refresh>=1000){
        cache={settings,mappings.data,mappings.count,now};
        for(int i=0;i<mappings.count;++i){
            Name action{};if(!Read(mappings.data,size_t(i)*40,action))return false;
            if(!native_objects::NameValueIs(g_base,action.index,action.number,"Jump"))continue;
            if(cache.size==cache.indices.size()){cache={};return false;}
            cache.indices[cache.size++]=i;
        }
    }
    count=cache.size;
    for(unsigned i=0;i<count;++i){
        if(!Read(mappings.data,size_t(cache.indices[i])*40,keys[i])||
           !native_objects::NameValueIs(g_base,keys[i].action.index,keys[i].action.number,"Jump")){cache={};return false;}
    }
    return count!=0;
}
bool Held(const local_players::Context& owner,bool& down,uint64_t& signature,uint64_t now){
    uintptr_t input=0,cls=0,outer=0;Array elements{};
    if(!Read(owner.controller,0x448,input)||!Read(input,0x10,cls)||!native_objects::NameIs(g_base,cls,"PlayerInput")||
       !Read(input,0x20,outer)||outer!=owner.controller||!Read(input,0x260,elements)||
       elements.count<0||elements.count>elements.capacity||elements.capacity>4096)return false;
    std::array<Mapping,8> keys{};unsigned count=0;if(!Keys(keys,count,now))return false;
    down=false;signature=14695981039346656037ull;
    for(unsigned i=0;i<count;++i){
        const auto& key=keys[i];signature=(signature^uint32_t(key.key.index))*1099511628211ull;
        signature=(signature^uint32_t(key.key.number))*1099511628211ull;
        signature=(signature^key.modifiers)*1099511628211ull;
        // The native game remapping page uses plain actions (all 120 observed
        // records have no modifier). Never turn a modified chord into one key.
        if(key.modifiers&15)continue;
        int index=-1;
        // Same read-only TMap lookup used inside UPlayerInput::IsPressed.
        // It reads only FName, writes our local index, and never owns/copies
        // FKey's shared pointer. Calling the by-value IsPressed ABI directly
        // with a borrowed key would incorrectly release its shared reference.
        g_find(reinterpret_cast<void*>(input+0x260),&index,&key.key);
        if(index==-1)continue;
        unsigned char flags=0;
        if(index<0||index>=elements.count||!elements.data||!Read(elements.data,size_t(index)*0xF0+0x34,flags))return false;
        down=down||(flags&1)!=0;
    }
    return true;
}
}
void OnLivePlayer(const local_players::Context& owner,uint32_t generation,bool focused){
    if(!native_tps::OnGameThread()||owner.index<0||owner.index>1)return;
    auto& state=g_players[owner.index];uintptr_t movement=0;
    if(!Ready()||!runtime::Get().mountedJumpEnabled.load()||!focused||
       !local_players::Matches(unsigned(owner.index),owner.character,generation)||!Owned(owner,movement)||
       native_tps::IsWorldPaused(reinterpret_cast<void*>(owner.pawn))||native_tps::CinematicActive(unsigned(owner.index))||
       native_tps::RadialActive(unsigned(owner.index))){state={};return;}
    const auto now=GetTickCount64();
    if(state.seen&&state.pawn==owner.pawn&&state.controller==owner.controller&&state.generation==generation&&now>=state.tick&&now-state.tick<8)return;
    bool down=false;uint64_t signature=0;
    if(!Held(owner,down,signature,now)){state={};return;}
    const bool same=state.seen&&state.pawn==owner.pawn&&state.controller==owner.controller&&state.generation==generation&&state.signature==signature;
    const bool edge=same&&!state.down&&down;const auto landing=same?state.landing:Landing{};
    state={owner.pawn,owner.controller,generation,signature,now,true,down,landing};
    if(!same&&!g_soundMask.load()){g_soundCursor=0;g_soundComplete=false;}
    Sounds();unsigned char mode=0;if(!Read(movement,0x1B0,mode))return;
    if(state.landing.tick){
        if(mode==3)state.landing.airborne=true;
        else if((mode==1||mode==2)&&state.landing.airborne){Sound(owner,true);state.landing={};}
        if(now<state.landing.tick||now-state.landing.tick>30000)state.landing={};
    }
    if(edge&&(mode==1||mode==2)&&g_jump(reinterpret_cast<void*>(movement),false)){
        Sound(owner,false);unsigned char after=0;
        state.landing={now,Read(movement,0x1B0,after)&&after==3};
        if(g_log&&++g_jumps<=8){char line[180]{};sprintf_s(line,
            "Mounted jump V1.80: player=%d native Jump -> horse DoJump; sound assets=%u; no imposed key; jump user-confirmed, sound EXPERIMENTAL",
            owner.index+1,g_soundMask.load());g_log(line);}
    }
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));if(!g_base)return;
    constexpr unsigned char find[]={0x48,0x89,0x5C,0x24,0x08,0x8B,0x41,0x08,0x4C,0x8B,0xDA,0x48,0x8B,0xD1};
    constexpr unsigned char jump[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x50,0x48,0x8B,0xB9,0xE8,0x00,0x00};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x1C75E60),find,sizeof(find))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x667510),jump,sizeof(jump)))return;
    g_find=reinterpret_cast<FindKey>(g_base+0x1C75E60);g_jump=reinterpret_cast<Jump>(g_base+0x667510);
    constexpr unsigned char sound[]={0x48,0x85,0xD2,0x0F,0x84,0xFE,0x00,0x00,0x00,0x48,0x89,0x5C,0x24,0x08,0x48,0x89};
    if(!std::memcmp(reinterpret_cast<void*>(g_base+0x18911C0),sound,sizeof(sound)))g_sound=reinterpret_cast<PlaySound>(g_base+0x18911C0);
    g_ready=true;if(log)log("Mounted jump V1.80 READY: rebindable Jump and native horse DoJump retained; loaded native varied sounds; no new hook; OFF by default; sounds EXPERIMENTAL");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
unsigned SoundAssetsReady(){return g_soundMask.load(std::memory_order_acquire);}
}
