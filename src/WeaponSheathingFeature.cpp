#include "WeaponSheathingFeature.h"
#include "NativeMemoryReader.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include "RefillRequest.h"
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <limits>

namespace dg::sheathing {
namespace {
using ClearFn=void(*)(void*);
ClearFn g_combat=nullptr,g_ranged=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};
struct FieldBaseline {float original=0,applied=0;bool captured=false,modified=false;};
struct Baseline {FieldBaseline fields[2]{};};
native_objects::Baselines<Baseline,16> g_baselines;
refill::Request g_requests[2]{};SRWLOCK g_lock=SRWLOCK_INIT;
unsigned g_changes=0,g_manual=0;
template<class T> bool Read(const void* obj,size_t at,T& value){
    native_objects::MemoryReader reader;return reader.Read(reinterpret_cast<uintptr_t>(obj),at,value);
}
bool Owned(const local_players::Context& c,unsigned char& type){
    auto* actor=reinterpret_cast<void*>(c.character);uintptr_t vt=0,combat=0,ranged=0;unsigned char role=0;
    void* resource=nullptr;float health=0;
    return c.index>=0&&c.index<2&&c.character&&c.localPlayer&&
        Read(actor,0,vt)&&Read(reinterpret_cast<void*>(vt),0xBB8,combat)&&combat==g_base+0x663AC0&&
        Read(reinterpret_cast<void*>(vt),0xBD0,ranged)&&ranged==g_base+0x663DC0&&
        Read(actor,0x118,role)&&role==3&&Read(actor,0xC18,type)&&(type==1||type==2)&&
        Read(actor,0xEA0,resource)&&Read(resource,0x37C,health)&&std::isfinite(health)&&health>0;
}
void ApplyDuration(void* actor,bool apply,unsigned char type){
    float current[2]{};native_objects::Identity identity{};
    if(!Read(actor,0xCD0,current)||!std::isfinite(current[0])||!std::isfinite(current[1])||
       current[0]<0||current[1]<0||!native_objects::CaptureLive(g_base,actor,identity))return;
    native_objects::MemoryReader reader;auto* b=g_baselines.Acquire(reader,g_base,identity);if(!b)return;
    const auto& rt=runtime::Get();
    const float seconds[2]={type==1?rt.sheathStrifeMeleeDelaySeconds.load():rt.sheathWarDelaySeconds.load(),rt.sheathDelaySeconds.load()};
    const bool never[2]={type==1?rt.sheathStrifeMeleeNeverEnabled.load():rt.sheathWarNeverEnabled.load(),rt.sheathNeverEnabled.load()};
    // Combat: Strife blades / War sword. Ranged: Strife pistols only.
    // War's ranged equipment stays native; all manual Clear shortcuts remain.
    bool changed=false;
    for(unsigned i=0;i<2;++i){
        auto& field=b->fields[i];
        const bool ours=field.modified&&current[i]==field.applied;
        if(!field.captured||!ours){field.original=current[i];field.captured=true;field.modified=false;}
        const bool modify=apply&&rt.weaponSheathingEnabled.load()&&(type==1||i==0)&&
            std::isfinite(seconds[i])&&seconds[i]>=0&&seconds[i]<=300;
        const float target=modify?(never[i]?std::numeric_limits<float>::max():seconds[i]):field.original;
        if(current[i]!=target){
            std::memcpy(static_cast<unsigned char*>(actor)+0xCD0+i*sizeof(float),&target,sizeof(target));changed=true;
        }
        field.applied=target;field.modified=modify;
    }
    if(changed&&g_log&&++g_changes<=8)g_log("Weapon sheathing V1.64 duration: independent Strife blades / Strife pistols / War sword; EXPERIMENTAL");
}
bool Idle(void* actor){
    void* component=nullptr;void* owner=nullptr;int count=-1,capacity=0;
    unsigned char aim=0,primary=0,secondary=0;
    return Read(actor,0xC70,component)&&component&&Read(component,0xE8,owner)&&owner==actor&&
        Read(component,0x100,count)&&Read(component,0x104,capacity)&&count==0&&capacity>=0&&capacity<=64&&
        Read(actor,0xE10+31,aim)&&!aim&&Read(actor,0xE10+18,primary)&&!primary&&
        Read(actor,0xE10+20,secondary)&&!secondary;
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));if(!g_base)return;
    constexpr unsigned char clearSig[]={0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8B,0x01,0x48,0x8B,0xD9,0xFF,0x90,0x80,0x06};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x663AC0),clearSig,sizeof(clearSig))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x663DC0),clearSig,sizeof(clearSig)))return;
    unsigned ranged=0,combat=0;
    std::memcpy(&ranged,reinterpret_cast<void*>(g_base+0x362DBD8+36),4);
    std::memcpy(&combat,reinterpret_cast<void*>(g_base+0x362DC00+36),4);
    if(ranged!=0xCD4||combat!=0xCD0)return;
    g_combat=reinterpret_cast<ClearFn>(g_base+0x663AC0);g_ranged=reinterpret_cast<ClearFn>(g_base+0x663DC0);
    g_ready.store(true,std::memory_order_release);
    if(log)log("Weapon sheathing V1.64 READY: three independent native delays; all manual Clear shortcuts retained; no new sheathing hooks; EXPERIMENTAL");
}
void OnLivePlayer(const local_players::Context& c,uint32_t generation){
    unsigned char type=0;
    if(!g_ready.load(std::memory_order_acquire)||!native_tps::OnGameThread()||!Owned(c,type))return;
    auto* actor=reinterpret_cast<void*>(c.character);
    const bool foot=c.kind==local_players::PoseKind::Foot;
    const bool gameplay=foot&&!native_tps::IsWorldPaused(actor)&&!native_tps::CinematicActive(unsigned(c.index));
    // Leaving normal form restores the normal character's native thresholds.
    // No horse/Ultimate state or component is ever altered.
    if(gameplay||!foot)ApplyDuration(actor,gameplay,type);
    bool request=false;const auto now=GetTickCount64();
    AcquireSRWLockExclusive(&g_lock);
    auto& pending=g_requests[c.index];pending.Observe(c.character,generation,now,foot);
    request=pending.Take(c.character,generation,now,gameplay&&Idle(actor));
    ReleaseSRWLockExclusive(&g_lock);
    if(request){
        g_ranged(actor);g_combat(actor);
        unsigned char states[2]{1,1};const bool applied=Read(actor,0xD15,states)&&!states[0]&&!states[1];
        AcquireSRWLockExclusive(&g_lock);g_requests[c.index].Complete(applied);ReleaseSRWLockExclusive(&g_lock);
        if(g_log){char line[160]{};sprintf_s(line,"Weapon sheathing V1.59 MANUAL player=%d nativeIdleReadback=%d n=%u EXPERIMENTAL",c.index+1,applied?1:0,++g_manual);g_log(line);}
    }
}
bool RequestSheathe(int playerIndex){
    if(!Ready()||playerIndex< -1||playerIndex>1)return false;bool queued=false;const auto now=GetTickCount64();
    AcquireSRWLockExclusive(&g_lock);
    for(unsigned i=0;i<2;++i){if(playerIndex>=0&&unsigned(playerIndex)!=i)continue;auto& p=g_requests[i];if(local_players::Matches(i,p.actor,p.generation))queued=p.Queue(now)||queued;}
    ReleaseSRWLockExclusive(&g_lock);return queued;
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
void Shutdown(){g_ready.store(false,std::memory_order_release);}
}
