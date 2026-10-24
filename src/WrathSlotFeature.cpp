#include "WrathSlotFeature.h"
#include "WrathSlotSettings.h"
#include "ForeignWrathFeature.h"
#include "LocalPlayerOwnership.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <atomic>
#include <cstdio>

namespace dg::wrath_slots {
namespace {
using Name=foreign_wrath::Name;
static_assert(sizeof(Name)==8);
// 663220 calls the Blueprint class lookup AND the selected ID's native unlock
// and Wrath-cost checks. Five arguments include the TSubclassOf sret destination.
// Hooking only 7AAC30 would leave the old ID's unlock check in this caller.
using ResolveFn=uintptr_t*(*)(void*,uintptr_t*,Name,bool*,bool*);
ResolveFn g_original=nullptr;uintptr_t g_base=0;LogFn g_log=nullptr;
std::atomic_bool g_ready{false};thread_local bool g_resolving=false;
unsigned g_changes=0;

template<class T> bool Read(uintptr_t p,size_t offset,T& value){
    native_objects::MemoryReader reader;return reader.Read(p,offset,value);
}
bool Enabled(){
    for(const auto& family:runtime::Get().wrathSlots)for(const auto& choice:family)
        if(choice.load(std::memory_order_relaxed))return true;
    return false;
}
bool OwnId(unsigned family,const Name& id){
    return id.number>=2&&id.number<=4&&(family==0?
        native_objects::NameValueIs(g_base,id.index,0,"Strife_Wrath"):
        native_objects::NameValueIs(g_base,id.index,0,"War_Wrath"));
}
bool ResourceType(uintptr_t resource){
    uintptr_t expected=0,actual=0,chain=0,entry=0;int index=-1,last=-1;
    return Read(g_base,0x3810268,expected)&&expected&&Read(resource,0x10,actual)&&actual&&
        Read(expected,0x38,index)&&index>=0&&index<=512&&Read(actual,0x38,last)&&last>=index&&
        Read(actual,0x30,chain)&&Read(chain,size_t(index)*8,entry)&&entry==expected+0x30;
}
bool Map(void* liveActor,const Name& original,Name& selected,uintptr_t& foreignClass,bool& blocked){
    if(!g_ready.load(std::memory_order_acquire)||g_resolving||!Enabled()||!native_tps::OnGameThread())return false;
    local_players::Context owner{};const auto actor=reinterpret_cast<uintptr_t>(liveActor);
    if(!local_players::ResolveActor(liveActor,owner)||owner.index<0||owner.index>1||!owner.localPlayer||
       owner.kind!=local_players::PoseKind::Foot||owner.character!=actor||owner.pawn!=actor)return false;
    uintptr_t cls=0,controller=0,pawn=0,resources=0;unsigned char type=0,role=0;
    if(!Read(actor,0x10,cls)||!Read(actor,0xC18,type)||(type!=1&&type!=2)||
       !Read(actor,0x118,role)||role!=3||!Read(actor,0x348,controller)||!controller||controller!=owner.controller||
       !Read(controller,0x350,pawn)||pawn!=actor||!Read(actor,0xEA0,resources)||!ResourceType(resources))return false;
    const unsigned family=unsigned(type-1);
    if(!(family==0?native_objects::NameIs(g_base,cls,"Strife_Blueprint_C"):
                  native_objects::NameIs(g_base,cls,"War_Blueprint_C"))||!OwnId(family,original))return false;
    // Resolve semantic slots from the live resource IDs, never physical buttons
    // or assumed ordering. An ambiguous/foreign layout remains fully native.
    std::array<Name,3> ids{};int slot=-1;
    for(unsigned i=0;i<3;++i){
        if(!Read(resources,0x3E4+i*8,ids[i])||!OwnId(family,ids[i]))return false;
        for(unsigned j=0;j<i;++j)if(ids[i]==ids[j])return false;
        if(ids[i]==original)slot=int(i);
    }
    if(slot<0)return false;
    const int choice=runtime::Get().wrathSlots[family][unsigned(slot)].load(std::memory_order_relaxed);
    if(!Supported(family,choice))return false;
    if(Foreign(family,choice)){
        // The original slot's own unlock remains required for a foreign
        // power. The selected class supplies its cost to the native caller.
        selected=original;blocked=!foreign_wrath::Prepare(liveActor,foreignClass,choice);return true;
    }
    const int number=NativeNumber(family,choice);
    if(!number||number==original.number)return false;
    selected={original.index,number};return true;
}
uintptr_t* HookResolve(void* actor,uintptr_t* result,Name id,bool* unlocked,bool* affordable){
    if(!g_original)return result;
    Name selected{};uintptr_t foreignClass=0;bool blocked=false;
    if(!result||!unlocked||!affordable||!Map(actor,id,selected,foreignClass,blocked))return g_original(actor,result,id,unlocked,affordable);
    if(blocked){*result=0;*unlocked=false;*affordable=false;return result;}
    struct Guard {Guard(){g_resolving=true;}~Guard(){g_resolving=false;}} guard;
    auto* returned=foreignClass?foreign_wrath::Resolve(g_original,actor,result,selected,unlocked,affordable,foreignClass):
        g_original(actor,result,selected,unlocked,affordable);
    if(g_log&&++g_changes<=12){char line[200]{};sprintf_s(line,
        "Wrath slots V1.83: native ID number %d -> %d foreign=%d unlock=%d costOK=%d; native lifecycle retained; EXPERIMENTAL",
        id.number,selected.number,foreignClass?1:0,*unlocked?1:0,*affordable?1:0);g_log(line);}
    return returned;
}
bool Boundary(){
    constexpr unsigned char entry[]={0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x6C,0x24,0x10,0x48,0x89,0x74,0x24,0x18};
    constexpr unsigned char getter[]={0xE8,0xD4,0x79,0x14,0x00};
    constexpr unsigned char unlock[]={0x4C,0x8B,0xC3,0x33,0xD2,0xE8,0x3C,0xC0,0x04,0x00};
    constexpr unsigned char cost[]={0x48,0x8B,0xCD,0xE8,0xDD,0xA2,0x17,0x00};
    return g_base&&!std::memcmp(reinterpret_cast<void*>(g_base+0x663220),entry,sizeof(entry))&&
        !std::memcmp(reinterpret_cast<void*>(g_base+0x663257),getter,sizeof(getter))&&
        !std::memcmp(reinterpret_cast<void*>(g_base+0x66331A),unlock,sizeof(unlock))&&
        !std::memcmp(reinterpret_cast<void*>(g_base+0x66332B),cost,sizeof(cost));
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if(!Boundary()){if(log)log("Wrath slots V1.77 unavailable: native lookup/unlock/cost signature mismatch");return;}
    auto* target=reinterpret_cast<void*>(g_base+0x663220);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookResolve),reinterpret_cast<void**>(&g_original))!=MH_OK)return;
    if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_original=nullptr;return;}
    g_ready.store(true,std::memory_order_release);
    foreign_wrath::Initialize(log,exactExecutable);
    if(log)log("Wrath slots V1.96 READY: batched transfers use the selected native class and cost; original slots/inputs retained; EXPERIMENTAL");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
int ForeignStatus(int choice){return foreign_wrath::Status(choice);}
void OnLivePlayer(const local_players::Context& owner){foreign_wrath::Observe(owner);}
void Shutdown(){g_ready.store(false,std::memory_order_release);foreign_wrath::Shutdown();}
}
