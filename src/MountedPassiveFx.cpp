#include "MountedPassiveFx.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include <array>
#include <cstdio>

namespace dg::mounted_fx {
namespace {
using Id=native_objects::Identity;
using Event=void(*)(void*,void*,void*);
uintptr_t g_base=0;Event g_event=nullptr;local_players::LogFn g_log=nullptr;unsigned g_repairs=0;
struct Array {uintptr_t data=0;int count=0,capacity=0;};
struct State {Id horse{};uintptr_t world=0;uint64_t start=0,next=0;bool checked=false;};
std::array<State,2> g_states{};
template<class T>bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
bool Is(uintptr_t p,const char* name){return native_objects::NameIs(g_base,p,name);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Field(uintptr_t object,const char* name,const char* kind,int size,int& at){
    uintptr_t cls=0;return Read(object,0x10,cls)&&native_objects::PropertyField(g_base,cls,name,kind,size,at);
}
uintptr_t Activate(uintptr_t component){
    uintptr_t cls=0;if(!Read(component,0x10,cls))return 0;
    for(unsigned depth=0;cls&&depth<16;++depth){
        uintptr_t f=0;if(!Read(cls,0x48,f))return 0;
        for(unsigned i=0;f&&i<512;++i){
            if(Is(f,"Activate")){
                uintptr_t type=0,entry=0;uint16_t size=0;int at=-1;
                return Read(f,0x10,type)&&Is(type,"Function")&&Read(f,0xC0,entry)&&entry==g_base+0x1CBE4D0&&
                    Read(f,0x9E,size)&&size==1&&native_objects::PropertyField(g_base,f,"bReset","BoolProperty",1,at)&&at==0?f:0;
            }
            uintptr_t next=0;if(!Read(f,0x28,next)||next==f)return 0;f=next;
        }
        if(f||!Read(cls,0x40,cls))return 0;
    }
    return 0;
}
bool Owned(const local_players::Context& c){
    uintptr_t cls=0,controller=0,pawn=0;unsigned char role=0,despawning=0;
    return c.kind==local_players::PoseKind::Horse&&c.localPlayer&&c.world&&c.character&&c.pawn&&c.pawn!=c.character&&
        Read(c.pawn,0x10,cls)&&(Is(cls,"Mayhem_Blueprint_C")||Is(cls,"Ruin_Blueprint_C")||Is(cls,"Despair_Blueprint_C")||Is(cls,"Rampage_Blueprint_C"))&&
        Read(c.pawn,0x118,role)&&role==3&&Read(c.pawn,0x348,controller)&&controller==c.controller&&
        Read(controller,0x350,pawn)&&pawn==c.pawn&&Read(c.pawn,0xA66,despawning)&&!despawning;
}
void ObserveAt(const local_players::Context& c,uint64_t now){
    if(!g_event||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    auto& s=g_states[unsigned(c.index)];
    if(c.kind!=local_players::PoseKind::Horse){s={};return;}
    if(s.horse.address==c.pawn&&s.world==c.world&&now<s.next)return;
    if(!Owned(c)){s={};return;}
    Id horse{};if(!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(c.pawn),horse)){s={};return;}
    if(horse!=s.horse||s.world!=c.world)s={horse,c.world,now};
    s.next=now+250;
    if(s.checked)return;
    if(native_tps::IsWorldPaused(reinterpret_cast<void*>(c.pawn))||native_tps::CinematicActive(unsigned(c.index))||native_tps::RadialActive(unsigned(c.index))){s.start=now;return;}
    if(now<s.start||now-s.start<2000)return;
    // Native OnLevelSpawn and OnHorseSpawn both activate PassiveFXArray.
    // Repair only a wholly inactive set on a stable, possessed mount once.
    // Never touch summon/despawn bursts, sprint or weather components.
    s.checked=true;int at=-1;Array list{};unsigned count=0;std::array<Id,16> components{};std::array<uintptr_t,16> functions{};
    if(!Field(c.pawn,"PassiveFXArray","ArrayProperty",16,at)||!Read(c.pawn,at,list)||!list.data||list.count<1||list.count>list.capacity||list.capacity>16)return;
    for(int i=0;i<list.count;++i){
        uintptr_t p=0,cls=0,owner=0,parent=0,parentOwner=0,asset=0;unsigned char active=0;int activeAt=-1;
        if(!Read(list.data,size_t(i)*8,p)||!Read(p,0x10,cls)||!Is(cls,"ParticleSystemComponent")||
           !Read(p,0xE8,owner)||owner!=c.pawn||!Read(p,0x108,parent)||!Read(parent,0xE8,parentOwner)||parentOwner!=c.pawn||!Read(p,0x578,asset))return;
        // Native base-class eyes/tail slots can legitimately have no template.
        if(!asset)continue;
        if(!Read(asset,0x10,cls)||!Is(cls,"ParticleSystem")||
           !Field(p,"bIsActive","BoolProperty",1,activeAt)||activeAt!=0xAE||!Read(p,activeAt,active)||(active&1)||
           !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),components[count])||!(functions[count]=Activate(p)))return;
        for(unsigned j=0;j<count;++j)if(components[j]==components[count])return;
        ++count;
    }
    for(unsigned i=0;i<count;++i){
        if(!Alive(horse)||!Owned(c)||!Alive(components[i]))return;
        unsigned char reset=0;g_event(reinterpret_cast<void*>(components[i].address),reinterpret_cast<void*>(functions[i]),&reset);
    }
    if(count&&g_log&&g_repairs++<8){char message[160]{};sprintf_s(message,"Mounted FX V1.156: one native passive activation request, player=%d count=%u; visual validation required",c.index,count);g_log(message);}
}
}
void Initialize(local_players::LogFn log,bool exact){
    g_log=log;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));g_event=reinterpret_cast<Event>(g_base+0xAC0630);
}
void OnLivePlayer(const local_players::Context& c){ObserveAt(c,GetTickCount64());}
void Shutdown(){g_event=nullptr;g_states={};g_repairs=0;}
}
