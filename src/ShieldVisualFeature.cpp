#include "ShieldVisualFeature.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include "OwnedParticleTransform.h"
#include <array>
#include <atomic>
#include <cmath>
#include <cstdio>
namespace dg::shield_visual {
namespace {
using Id=native_objects::Identity;
struct Array {uintptr_t data=0;int count=0,capacity=0;};
struct Effect {Id component{},source{},target{},world{},status{},actor{};uint64_t born=0;bool hidden=false;float preset=1;owned_particle::State transform{};};
struct Player {Id actor{},mesh{},world{},actorClass{},componentClass{};int componentOffset=-1,statusesOffset=-1;uint64_t tick=0,next=0;};
std::array<Effect,32> g_effects{};std::array<Player,2> g_players{};
uintptr_t g_base=0;VisibleFn g_visible=nullptr;LogFn g_log=nullptr;
std::atomic_bool g_ready{false};unsigned g_count=0,g_binds=0,g_ends=0;uint64_t g_poll=0;
template<class T>bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
template<size_t N>bool Is(uintptr_t p,const char(&s)[N]){return native_objects::NameIs(g_base,p,s);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),id);}
bool Source(uintptr_t p){uintptr_t cls=0,package=0;return Read(p,0x10,cls)&&Is(cls,"ParticleSystem")&&Is(p,"P_CreatureCore_ShieldDemon")&&Read(p,0x20,package)&&Is(package,"/Game/VFX/Particles/Characters/Generic/P_CreatureCore_ShieldDemon");}
int Choice(){return effect_appearance::Normalize(unsigned(effect_appearance::Family::ShieldDemonCore),runtime::Get().effectAppearances[unsigned(effect_appearance::Family::ShieldDemonCore)].load());}
float Scale(int choice){return choice==2?.35f:choice==3?.25f:choice==4?.6f:choice==5?.35f:1.f;}
void Clear(Effect& e){if(e.component&&g_count)--g_count;e={};}
bool Template(const Effect& e){uintptr_t asset=0,cls=0;return Alive(e.component)&&Alive(e.target)&&Read(e.component.address,0x10,cls)&&Is(cls,"ParticleSystemComponent")&&Read(e.component.address,0x578,asset)&&asset==e.target.address;}
void Visibility(Effect& e,bool show){unsigned char flags=0;if(g_visible&&Template(e)&&Read(e.component.address,0x1CC,flags)&&bool(flags&0x20)!=show){g_visible(reinterpret_cast<void*>(e.component.address),show,1);e.hidden=!show;}}
bool DamageShield(uintptr_t status){
    uintptr_t expected=0,actual=0,parents=0,entry=0;int index=-1,last=-1;
    return Read(g_base,0x3812750,expected)&&expected&&Read(status,0x10,actual)&&Read(expected,0x38,index)&&index>=0&&index<=512&&
        Read(actual,0x38,last)&&last>=index&&last<=512&&Read(actual,0x30,parents)&&parents&&Read(parents,size_t(index)*8,entry)&&entry==expected+0x30;
}
bool List(uintptr_t p,size_t at,Array& a){return Read(p,at,a)&&a.count>=0&&a.count<=a.capacity&&a.capacity<=128&&(a.data||!a.count);}
bool Fields(Player& p,uintptr_t actor,uintptr_t& component){
    uintptr_t cls=0,owner=0;
    if(!Read(actor,0x10,cls))return false;
    if(p.actorClass.address!=cls||!Alive(p.actorClass)){
        p.actorClass={};p.componentOffset=-1;
        if(!native_objects::PropertyField(g_base,cls,"m_pStatusEffectComponent","ObjectProperty",8,p.componentOffset)||!Capture(cls,p.actorClass))return false;
    }
    if(p.componentOffset<0||!Read(actor,size_t(p.componentOffset),component)||!Read(component,0xE8,owner)||owner!=actor||!Read(component,0x10,cls))return false;
    if(p.componentClass.address!=cls||!Alive(p.componentClass)){
        p.componentClass={};p.statusesOffset=-1;
        if(!native_objects::PropertyField(g_base,cls,"m_statusEffects","ArrayProperty",16,p.statusesOffset)||!Capture(cls,p.componentClass))return false;
    }
    return p.statusesOffset>=0;
}
bool Publish(const local_players::Context& c,Player& p,uint64_t now){
    uintptr_t pawn=0,mesh=0,owner=0;unsigned char role=0;
    if(!c.localPlayer||!c.controller||!c.character||!c.pawn||!c.world||!Read(c.controller,0x350,pawn)||pawn!=c.pawn||
       !Read(c.character,0x118,role)||role!=3||!Read(c.character,0x380,mesh)||!Read(mesh,0xE8,owner)||owner!=c.character)return false;
    if(p.actor.address!=c.character||p.mesh.address!=mesh||p.world.address!=c.world||!Alive(p.actor)||!Alive(p.mesh)||!Alive(p.world)){
        Id actor{},body{},world{};if(!Capture(c.character,actor)||!Capture(mesh,body)||!Capture(c.world,world))return false;
        p.actor=actor;p.mesh=body;p.world=world;
    }
    p.tick=now;return true;
}
void Bind(Player& p,uint64_t now){
    bool pending=false;for(const auto& e:g_effects)pending=pending||(e.component&&!e.status&&e.world==p.world&&now>=e.born&&now-e.born<=2000);
    if(!pending)return;
    uintptr_t component=0;Array statuses{};if(!Fields(p,p.actor.address,component)||!List(component,size_t(p.statusesOffset),statuses))return;
    for(auto& e:g_effects)if(e.component&&!e.status&&e.world==p.world&&Template(e)){
        uintptr_t parent=0,parentOwner=0,candidate=0;unsigned candidates=0;bool direct=false;
        const bool attached=now>=e.born&&now-e.born>=100&&Read(e.component.address,0x108,parent)&&Read(parent,0xE8,parentOwner)&&parentOwner==p.actor.address;
        for(int i=0;i<statuses.count;++i){
            uintptr_t status=0,actor=0,source=0,particle=0;
            if(!Read(statuses.data,size_t(i)*8,status)||!DamageShield(status)||!Read(status,0x68,actor)||actor!=p.actor.address||
               !Read(status,0x58,source)||source!=e.source.address||!Read(status,0x78,particle))continue;
            if(particle==e.component.address){candidate=status;direct=true;break;}
            if(attached){candidate=status;++candidates;}
        }
        // A shield can be consumed in one hit before the first observation.
        // Its owned attachment and unique native core status still identify it.
        if(candidate&&(direct||candidates==1)){
            Id identity{};if(!Capture(candidate,identity))continue;e.status=identity;e.actor=p.actor;
            if(g_log&&g_binds++<8)g_log("Shield visual V1.145: replacement bound to the owned native DamageShield particle; protection unchanged");
        }
    }
}
void Poll(uint64_t now){
    for(auto& e:g_effects)if(e.component){
        if(!Template(e)){Clear(e);continue;}
        if(!e.status){if(now<e.born||now-e.born>2000)Clear(e);continue;}
        bool active=false,known=true;
        if(Alive(e.status)&&Alive(e.actor)){
            uintptr_t particle=0,actor=0,source=0;
            known=Read(e.status.address,0x78,particle)&&Read(e.status.address,0x68,actor)&&Read(e.status.address,0x58,source);
            active=known&&particle==e.component.address&&actor==e.actor.address&&source==e.source.address;
        }
        if(!known)continue;
        if(active){
            if(e.hidden)Visibility(e,true);
            int hero=0;uintptr_t parent=0,owner=0;
            if(Read(e.actor.address,0xC18,hero)&&hero>=1&&hero<=2&&
               Read(e.component.address,0x108,parent)&&Read(parent,0xE8,owner)&&owner==e.actor.address)
                owned_particle::Apply(g_base,e.component,runtime::Get().shieldTransforms[unsigned(hero-1)].Load(),e.preset,e.transform);
        }
        else {const bool before=e.hidden;Visibility(e,false);if(!before&&e.hidden&&g_log&&g_ends++<8)g_log("Shield visual V1.145: native shield particle reference cleared; private replacement hidden");}
    }
}
}
void Initialize(uintptr_t base,VisibleFn visible,LogFn log){
    g_base=base;g_visible=visible;g_log=log;
    constexpr unsigned char stop[]={0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x8B,0x49,0x78,0x48,0x85,0xC9};
    constexpr unsigned char damage[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x50,0x48,0x8B,0x79,0x68,0x48,0x8B};
    g_ready=base&&visible&&!std::memcmp(reinterpret_cast<void*>(base+0x69FB90),stop,sizeof(stop))&&!std::memcmp(reinterpret_cast<void*>(base+0x6D0860),damage,sizeof(damage));
}
bool CanTrack(){if(!g_ready)return false;for(const auto& e:g_effects)if(!e.component||!Alive(e.component))return true;return false;}
void ReleaseReused(void* component){
    if(!g_ready||!g_count||!component||!native_tps::OnGameThread())return;
    for(auto& e:g_effects)if(e.component.address==reinterpret_cast<uintptr_t>(component)){
        unsigned char flags=0;
        // A live factory result proves pool reuse. Restore our flag before
        // the new effect's own Hidden policy runs, including Stinger.
        if(e.hidden&&Alive(e.component)&&Read(e.component.address,0x1CC,flags)&&!(flags&0x20)&&g_visible)g_visible(component,true,1);
        Clear(e);break;
    }
}
void Track(void* component,void* original,void* rendered,void* world){
    if(!g_ready||!component||original==rendered||!native_tps::OnGameThread()||!Source(reinterpret_cast<uintptr_t>(original)))return;
    Effect next{};
    if(!Capture(reinterpret_cast<uintptr_t>(component),next.component)||!Capture(reinterpret_cast<uintptr_t>(original),next.source)||
       !Capture(reinterpret_cast<uintptr_t>(rendered),next.target)||!Capture(reinterpret_cast<uintptr_t>(world),next.world)||!Template(next))return;
    next.born=GetTickCount64();next.preset=Scale(Choice());
    for(auto& e:g_effects)if(!e.component||!Alive(e.component)){Clear(e);e=next;++g_count;return;}
}
void OnLivePlayer(const local_players::Context& c){
    if(!g_ready||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    auto& p=g_players[unsigned(c.index)];const auto now=GetTickCount64();if(now<p.next)return;p.next=now+100;
    if(Choice()<=1&&!g_count){p.tick=0;return;}
    if(Publish(c,p,now)){if(g_count)Bind(p,now);}else p.tick=0;
    if(now>=g_poll){g_poll=now+100;Poll(now);}
}
void Shutdown(){g_ready=false;if(native_tps::OnGameThread()){g_effects={};g_players={};g_count=0;}}
}
