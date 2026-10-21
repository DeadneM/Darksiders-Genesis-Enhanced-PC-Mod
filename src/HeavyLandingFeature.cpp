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
constexpr uint64_t MoveBit=uint64_t(1)<<29;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};unsigned g_changes=0;
struct Saved {native_objects::Identity identity{};unsigned owner=0;unsigned char movement=0;bool moveAllowed=false,seen=false;};
std::array<Saved,8> g_saved{}; // only modified heavy-land instances, game thread
struct Ground {uintptr_t actor=0;uint32_t generation=0;uint64_t since=0,last=0;};
Ground g_ground[2]{};
template<class T> bool Read(uintptr_t object,size_t offset,T& value){native_objects::MemoryReader r;return r.Read(object,offset,value);}
bool Writable(uintptr_t pointer,size_t size){
    MEMORY_BASIC_INFORMATION m{};
    if(!VirtualQuery(reinterpret_cast<void*>(pointer),&m,sizeof(m))||m.State!=MEM_COMMIT||(m.Protect&(PAGE_GUARD|PAGE_NOACCESS)))return false;
    const auto access=m.Protect&0xFF;const auto offset=pointer-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return (access==PAGE_READWRITE||access==PAGE_WRITECOPY)&&offset<=m.RegionSize&&size<=m.RegionSize-offset;
}
bool Fields(uintptr_t ability,unsigned char& movement,uint64_t& mask){
    return Read(ability,0x161,movement)&&movement<=1&&Read(ability,0x80,mask)&&
        Writable(ability+0x161,1)&&Writable(ability+0x80,8);
}
void Restore(Saved& saved){
    if(!saved.identity)return;
    native_objects::MemoryReader reader;unsigned char movement=0;uint64_t mask=0;
    if(native_objects::Alive(reader,g_base,saved.identity)&&Fields(saved.identity.address,movement,mask)){
        // Restore only our bool and MOVE bit, never the other action permissions.
        if(movement==1)std::memcpy(reinterpret_cast<void*>(saved.identity.address+0x161),&saved.movement,1);
        if(!saved.moveAllowed&&(mask&MoveBit)){mask&=~MoveBit;std::memcpy(reinterpret_cast<void*>(saved.identity.address+0x80),&mask,8);}
    }
    saved={};
}
void RestorePlayer(unsigned index){for(auto& saved:g_saved)if(saved.identity&&saved.owner==index)Restore(saved);}
bool Heavy(uintptr_t ability,unsigned char type){
    uintptr_t cls=0;if(!Read(ability,0x10,cls))return false;
    return type==1?native_objects::NameIs(g_base,cls,"MayhemAbility_Strife_HeavyLand_C"):
        type==2&&native_objects::NameIs(g_base,cls,"MayhemAbility_War_HeavyLand_C");
}
void Service(const local_players::Context& owner,uint32_t generation,bool focused,uint64_t now){
    if(!g_ready.load(std::memory_order_acquire)||!native_tps::OnGameThread()||owner.index<0||owner.index>1)return;
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
    // MOVE is still false in all recorded locked HeavyLand frames even with
    // intended movement. Never wait for an action that these very gates block.
    if(now>=ground.since&&now-ground.since>=50){
        for(int n=0;n<count;++n){
            uintptr_t ability=0,instigator=0,character=0,ownMovement=0;unsigned char state=0,allow=0;
            uint64_t mask=0;float elapsed=0;
            if(!Read(array,size_t(n)*8,ability)||!Heavy(ability,type)||!Read(ability,0x48,instigator)||instigator!=pawn||
               !Read(ability,0x1B0,character)||character!=pawn||!Read(ability,0x1C8,ownMovement)||ownMovement!=movement||
               !Read(ability,0xD8,state)||state!=2||!Read(ability,0xDC,elapsed)||!std::isfinite(elapsed)||
               elapsed<0.15f||elapsed>5||!Fields(ability,allow,mask))continue;
            native_objects::Identity identity{};
            if(!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(ability),identity))continue;
            Saved* saved=nullptr;
            for(auto& entry:g_saved)if(entry.identity==identity){saved=&entry;break;}
            if(!saved){
                for(auto& entry:g_saved)if(!entry.identity){saved=&entry;break;}
                if(!saved)continue;
                *saved={identity,index,allow,(mask&MoveBit)!=0,true};
            }
            saved->seen=true;
            if(allow!=1||!(mask&MoveBit)){
                const unsigned char yes=1;mask|=MoveBit;
                std::memcpy(reinterpret_cast<void*>(ability+0x161),&yes,1);
                std::memcpy(reinterpret_cast<void*>(ability+0x80),&mask,8);
                if(g_log&&++g_changes<=8){char line[190]{};sprintf_s(line,
                    "Heavy landing V1.75: player=%u grounded heavy-land movement gates released after impact; animation retained; EXPERIMENTAL",index+1);g_log(line);}
            }
        }
    }
    for(auto& saved:g_saved)if(saved.identity&&saved.owner==index&&!saved.seen)Restore(saved);
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    // Native OnStart adds MOVE to AllowedActionsFlags only when AllowPlayerMovement is true.
    constexpr unsigned char start[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20};
    // Native player movement also independently tests this ability bool every update.
    constexpr unsigned char gate[]={0x80,0xBE,0x61,0x01,0x00,0x00,0x00};
    if(!g_base||std::memcmp(reinterpret_cast<void*>(g_base+0x5A3670),start,sizeof(start))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x67F105),gate,sizeof(gate)))return;
    g_ready=true;
    if(log)log("Heavy landing V1.75 READY: exact HeavyLand only after native ground contact; two native movement gates; no hook, Finish, timer or input edits; EXPERIMENTAL");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
void OnLivePlayer(const local_players::Context& owner,uint32_t generation,bool focusedGameplay){Service(owner,generation,focusedGameplay,GetTickCount64());}
void Shutdown(){g_ready=false;} // teardown does not dereference cached game objects from this thread
}
