#include "ShieldVisualFeature.h"
#include "EffectAppearanceFeature.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include "OwnedParticleTransform.h"
#include <array>
#include <atomic>
namespace dg::shield_visual {
namespace {
using Id=native_objects::Identity;using Vector=owned_particle::Vector;
using Spawn=void*(*)(void*,void*,uint64_t,const Vector*,const Vector*,const Vector*,unsigned char,bool,unsigned char);
using Destroy=void(*)(void*,void*);
struct Array {uintptr_t data=0;int count=0,capacity=0;};
struct Player {Id actor{},mesh{},world{},status{},original{},effect{},actorClass{},statusClass{};int componentAt=-1,listAt=-1;int choice=-1;uint64_t next=0,settled=0;unsigned attempts=0;bool hidden=false,requested=false,reported=false,zero=false;owned_particle::State transform{};};
std::array<Player,2> g_players{};uintptr_t g_base=0;VisibleFn g_visible=nullptr;Spawn g_spawn=nullptr;Destroy g_destroy=nullptr;owned_particle::Event g_event=nullptr;LogFn g_log=nullptr;
std::atomic_bool g_ready{false};thread_local bool g_ownSpawn=false;
template<class T>bool ReadShield(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
template<size_t N>bool Is(uintptr_t p,const char(&s)[N]){return native_objects::NameIs(g_base,p,s);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),id);}
bool Source(uintptr_t p){uintptr_t cls=0,package=0;return ReadShield(p,0x10,cls)&&Is(cls,"ParticleSystem")&&Is(p,"P_CreatureCore_ShieldDemon")&&ReadShield(p,0x20,package)&&Is(package,"/Game/VFX/Particles/Characters/Generic/P_CreatureCore_ShieldDemon");}
int Choice(){return effect_appearance::Normalize(unsigned(effect_appearance::Family::ShieldDemonCore),runtime::Get().effectAppearances[unsigned(effect_appearance::Family::ShieldDemonCore)].load());}
float Preset(int choice){return choice==2?.35f:choice==3?.25f:choice==4?.6f:choice==5?.35f:1.f;}
bool Particle(const Id& id,uintptr_t actor,bool original){uintptr_t cls=0,owner=0,asset=0,parent=0,parentOwner=0;
    return Alive(id)&&ReadShield(id.address,0x10,cls)&&Is(cls,"ParticleSystemComponent")&&ReadShield(id.address,0xE8,owner)&&owner==actor&&ReadShield(id.address,0x108,parent)&&ReadShield(parent,0xE8,parentOwner)&&parentOwner==actor&&ReadShield(id.address,0x578,asset)&&(!original||Source(asset));}
void Restore(Player& s){unsigned char flags=0;if(s.hidden&&Alive(s.actor)&&Particle(s.original,s.actor.address,true)&&ReadShield(s.original.address,0x1CC,flags)&&!(flags&0x20))g_visible(reinterpret_cast<void*>(s.original.address),true,1);s.hidden=false;}
void Remove(Player& s){if(Alive(s.actor)&&Particle(s.effect,s.actor.address,false)&&g_destroy)g_destroy(reinterpret_cast<void*>(s.actor.address),reinterpret_cast<void*>(s.effect.address));s.effect={};s.transform={};s.reported=false;s.zero=false;Restore(s);}
void Hide(Player& s){unsigned char flags=0;if(!s.hidden&&Particle(s.original,s.actor.address,true)&&ReadShield(s.original.address,0x1CC,flags)&&(flags&0x20)){g_visible(reinterpret_cast<void*>(s.original.address),false,1);s.hidden=true;}}
bool FindStatus(Player& state,uintptr_t actor,Id& status,Id& particle){
    uintptr_t cls=0,component=0,owner=0;Array list{};
    if(!ReadShield(actor,0x10,cls))return false;
    if(state.actorClass.address!=cls||!Alive(state.actorClass)){
        if(!native_objects::PropertyField(g_base,cls,"m_pStatusEffectComponent","ObjectProperty",8,state.componentAt)||!Capture(cls,state.actorClass))return false;
    }
    if(state.componentAt<0||!ReadShield(actor,size_t(state.componentAt),component)||!ReadShield(component,0xE8,owner)||owner!=actor||!ReadShield(component,0x10,cls))return false;
    if(state.statusClass.address!=cls||!Alive(state.statusClass)){
        if(!native_objects::PropertyField(g_base,cls,"m_statusEffects","ArrayProperty",16,state.listAt)||!Capture(cls,state.statusClass))return false;
    }
    if(state.listAt<0||!ReadShield(component,size_t(state.listAt),list)||list.count<0||list.count>list.capacity||list.capacity>128||(!list.data&&list.count))return false;
    uintptr_t found=0,psc=0;
    for(int i=0;i<list.count;++i){uintptr_t p=0,type=0,recipient=0,visual=0;
        if(!ReadShield(list.data,size_t(i)*8,p)||!ReadShield(p,0x10,type)||!(Is(type,"MayhemStatusEffect_Core_ShieldDemon_1_Blueprint_C")||Is(type,"MayhemStatusEffect_Core_ShieldDemon_2_Blueprint_C")||Is(type,"MayhemStatusEffect_Core_ShieldDemon_3_Blueprint_C"))||!ReadShield(p,0x68,recipient)||recipient!=actor||!ReadShield(p,0x78,visual)||!visual)continue;
        if(found)return false;found=p;psc=visual;
    }
    // The observed source at +0x58 is null, not a particle pointer. The actual
    // owned native PSC reference at +0x78 is the visual lifecycle authority.
    return found&&Capture(found,status)&&Capture(psc,particle)&&Particle(particle,actor,true);
}
bool SpawnVisual(Player& s,uintptr_t asset){
    uintptr_t parent=0,cls=0;int at=-1;Vector position{},rotation{},scale{};uint64_t socket=0;
    if(!Particle(s.original,s.actor.address,true)||!ReadShield(s.original.address,0x10,cls)||!ReadShield(s.original.address,0x108,parent)||
       !native_objects::PropertyField(g_base,cls,"RelativeLocation","StructProperty",12,at)||!ReadShield(s.original.address,size_t(at),position)||
       !native_objects::PropertyField(g_base,cls,"RelativeRotation","StructProperty",12,at)||!ReadShield(s.original.address,size_t(at),rotation)||
       !native_objects::PropertyField(g_base,cls,"RelativeScale3D","StructProperty",12,at)||!ReadShield(s.original.address,size_t(at),scale)||
       !native_objects::PropertyField(g_base,cls,"AttachSocketName","NameProperty",8,at)||!ReadShield(s.original.address,size_t(at),socket)||!owned_particle::Finite(position)||!owned_particle::Finite(rotation)||!owned_particle::Finite(scale))return false;
    g_ownSpawn=true;void* visual=g_spawn(reinterpret_cast<void*>(asset),reinterpret_cast<void*>(parent),socket,&position,&rotation,&scale,0,true,0);g_ownSpawn=false;
    if(!visual||!Capture(reinterpret_cast<uintptr_t>(visual),s.effect)||!Particle(s.effect,s.actor.address,false))return false;
    uintptr_t actual=0;return ReadShield(s.effect.address,0x578,actual)&&actual==asset;
}
}
void Initialize(uintptr_t base,VisibleFn visible,LogFn log){g_base=base;g_visible=visible;g_log=log;
    constexpr unsigned char spawn[]={0x40,0x55,0x53,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x57,0x48,0x8D,0x6C,0x24,0xF9};
    constexpr unsigned char destroy[]={0x4C,0x8B,0xC2,0x48,0x85,0xD2,0x74,0x18,0x48,0x39,0x8A,0xE8,0x00,0x00,0x00,0x75};
    g_ready=base&&visible&&!std::memcmp(reinterpret_cast<void*>(base+0x189A510),spawn,sizeof(spawn))&&!std::memcmp(reinterpret_cast<void*>(base+0x163EFA0),destroy,sizeof(destroy));
    if(g_log)g_log(g_ready?"Shield visual V1.147 READY: shared emitter checked before detour; native status untouched":"Shield visual V1.147 unavailable: native emitter signature mismatch");
    if(g_ready){g_spawn=reinterpret_cast<Spawn>(base+0x189A510);g_destroy=reinterpret_cast<Destroy>(base+0x163EFA0);g_event=reinterpret_cast<owned_particle::Event>(base+0xAC0630);}}
bool OwnSpawnActive(){return g_ownSpawn;}
bool Ready(){return g_ready.load();}
void OnLivePlayer(const local_players::Context& c){
    if(!g_ready||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    auto& s=g_players[unsigned(c.index)];const auto now=GetTickCount64();if(now<s.next)return;s.next=now+100;
    const int choice=Choice();uintptr_t pawn=0,mesh=0,owner=0;unsigned char role=0,hero=0;
    if(!choice||!c.localPlayer||!c.controller||!c.character||!c.world||!ReadShield(c.controller,0x350,pawn)||pawn!=c.pawn||!ReadShield(c.character,0x118,role)||role!=3||!ReadShield(c.character,0xC18,hero)||hero<1||hero>2||!ReadShield(c.character,0x380,mesh)||!ReadShield(mesh,0xE8,owner)||owner!=c.character){Remove(s);s={};return;}
    if(s.actor.address!=c.character||s.world.address!=c.world||s.mesh.address!=mesh||!Alive(s.actor)||!Alive(s.world)||!Alive(s.mesh)){Remove(s);s={};if(!Capture(c.character,s.actor)||!Capture(c.world,s.world)||!Capture(mesh,s.mesh))return;s.settled=now;}
    if(s.choice!=choice){Remove(s);s.choice=choice;s.attempts=0;s.requested=false;}
    if(native_tps::IsWorldPaused(reinterpret_cast<void*>(c.pawn))||native_tps::CinematicActive(unsigned(c.index)))return;
    Id status{},original{};if(!FindStatus(s,c.character,status,original)){Remove(s);s.status={};s.original={};return;}
    if(status!=s.status||original!=s.original){Remove(s);s.status=status;s.original=original;s.attempts=0;}
    if(choice==1){if(s.effect)Remove(s);Hide(s);return;}
    if(now<s.settled||now-s.settled<500)return;
    uintptr_t asset=0;if(choice==6)ReadShield(s.original.address,0x578,asset);
    else {asset=reinterpret_cast<uintptr_t>(effect_appearance::ShieldAsset(choice,false));if(!asset&&!s.requested){s.requested=effect_appearance::ShieldLoaderReady();asset=reinterpret_cast<uintptr_t>(effect_appearance::ShieldAsset(choice,s.requested));}}
    if(!asset){Remove(s);return;}
    uintptr_t still=0,recipient=0;
    if(!Alive(s.actor)||!Alive(s.world)||!Alive(s.status)||!ReadShield(s.status.address,0x78,still)||still!=s.original.address||!ReadShield(s.status.address,0x68,recipient)||recipient!=c.character){Remove(s);return;}
    if(s.effect&&!Alive(s.effect)){s.effect={};s.transform={};}
    if(!s.effect&&s.attempts<3){++s.attempts;if(!SpawnVisual(s,asset)){Remove(s);return;}}
    uintptr_t actual=0;if(!Particle(s.effect,c.character,false)||!ReadShield(s.effect.address,0x578,actual)||actual!=asset){Remove(s);return;}
    const bool applied=owned_particle::Apply(g_base,s.effect,runtime::Get().shieldTransforms[unsigned(hero-1)].Load(),Preset(choice),s.transform,g_event);
    if(!s.reported&&g_log)g_log(applied?"Shield visual V1.147: owned native status and live transform active":"Shield visual V1.147: transform contract refused; native visual retained");s.reported=true;
    if(applied){
        Hide(s);const bool zero=runtime::Get().shieldTransforms[unsigned(hero-1)].Load().size==0;unsigned char flags=0;
        if((zero||s.zero)&&ReadShield(s.effect.address,0x1CC,flags)&&bool(flags&0x20)==zero)g_visible(reinterpret_cast<void*>(s.effect.address),!zero,1);s.zero=zero;
    }else Remove(s);
}
void Shutdown(){if(native_tps::OnGameThread())for(auto& s:g_players){Remove(s);s={};}g_ready=false;}
}
