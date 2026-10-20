#include "LeviathanVisualFeature.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <array>
#include <algorithm>
#include <atomic>

namespace dg::leviathan_visual {
namespace {
struct Vector {float x=0,y=0,z=0;};
static_assert(sizeof(Vector)==12);
// Native execSpawnEmitterAttached 1D19B50 -> 189A510: three vectors by
// const reference, packed FName, attach enum, auto-destroy, pool enum.
using SpawnFn=void*(*)(void*,void*,uint64_t,const Vector*,const Vector*,const Vector*,unsigned char,bool,unsigned char);
using DestroyFn=void(*)(void*,void*); // 163EFA0: owner actor, owned component
using VisibleFn=void(*)(void*,bool,unsigned char);
SpawnFn g_spawn=nullptr;DestroyFn g_destroy=nullptr;VisibleFn g_visible=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};
std::atomic_uint g_loaded{0};
native_objects::Identity g_templates[3]{};
struct Hidden {native_objects::Identity component{},owner{};};
struct Player {
    native_objects::Identity actor{},effect{};uintptr_t parent=0;
    int selection=-2;unsigned attempts=0;ULONGLONG tick=0;
    std::array<Hidden,8> hidden{};
};
Player g_players[2]{};
int g_cursor=0;bool g_scanComplete=false;ULONGLONG g_scanTick=0;
template<class T> bool Read(uintptr_t p,size_t at,T& value){native_objects::MemoryReader r;return r.Read(p,at,value);}
template<size_t N> bool Name(uintptr_t p,const char(&s)[N]){return native_objects::NameIs(g_base,p,s);}
bool Particle(uintptr_t p){uintptr_t cls=0;return Read(p,0x10,cls)&&Name(cls,"ParticleSystemComponent");}
int Template(uintptr_t p){
    uintptr_t cls=0,package=0;
    if(!Read(p,0x10,cls)||!Name(cls,"ParticleSystem")||!Read(p,0x20,package))return -1;
    if(Name(p,"P_Dagon_CreatureCore_Status_Lvl1")&&Name(package,"/Game/VFX/Particles/Characters/Dagon/P_Dagon_CreatureCore_Status_Lvl1"))return 0;
    if(Name(p,"P_Dagon_CreatureCore_Status_Lvl2")&&Name(package,"/Game/VFX/Particles/Characters/Dagon/P_Dagon_CreatureCore_Status_Lvl2"))return 1;
    if(Name(p,"P_Dagon_CreatureCore_Status_Lvl3")&&Name(package,"/Game/VFX/Particles/Characters/Dagon/P_Dagon_CreatureCore_Status_Lvl3"))return 2;
    return -1;
}
void Scan(ULONGLONG now){
    native_objects::MemoryReader reader;bool complete=true;unsigned mask=0;
    for(unsigned i=0;i<3;++i){
        auto& id=g_templates[i];if(!native_objects::Alive(reader,g_base,id)){id={};complete=false;}
        else mask|=1u<<i;
    }
    g_loaded.store(mask);
    if(complete||g_scanComplete||now-g_scanTick<16)return;
    g_scanTick=now;int count=0;uintptr_t chunks=0;
    if(!Read(g_base+0x3959694,0,count)||count<=0||count>2000000||!Read(g_base+0x3959680,0,chunks))return;
    // One bounded pass over loaded objects per selection/owner change. Never
    // load an asset, retry indefinitely, or scan after all three are cached.
    for(unsigned n=0;n<512&&g_cursor<count;++n,++g_cursor){
        uintptr_t chunk=0,p=0;uint32_t flags=0;
        if(!Read(chunks,size_t(g_cursor/65536)*8,chunk)||!Read(chunk,size_t(g_cursor%65536)*24,p)||
           !Read(chunk,size_t(g_cursor%65536)*24+8,flags)||!p||(flags&0x30000000u))continue;
        const int kind=Template(p);
        if(kind>=0)native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_templates[kind]);
    }
    if(g_cursor>=count)g_scanComplete=true;
}
void Restore(Hidden& saved){
    if(!saved.component)return;
    native_objects::MemoryReader reader;uintptr_t owner=0;unsigned char flags=0;
    if(native_objects::Alive(reader,g_base,saved.component)&&native_objects::Alive(reader,g_base,saved.owner)&&
       Read(saved.component.address,0xE8,owner)&&owner==saved.owner.address&&
       Read(saved.component.address,0x1CC,flags)&&!(flags&0x20)&&g_visible)
        g_visible(reinterpret_cast<void*>(saved.component.address),true,1);
    saved={};
}
void Remove(Player& state){
    native_objects::MemoryReader reader;uintptr_t owner=0;
    if(state.effect&&native_objects::Alive(reader,g_base,state.effect)&&
       native_objects::Alive(reader,g_base,state.actor)&&Read(state.effect.address,0xE8,owner)&&owner==state.actor.address&&g_destroy)
        g_destroy(reinterpret_cast<void*>(owner),reinterpret_cast<void*>(state.effect.address));
    state.effect={};state.parent=0;
}
void Clear(Player& state){Remove(state);for(auto& hidden:state.hidden)Restore(hidden);}
void Hide(Player& state,uintptr_t component){
    if(component==state.effect.address||!Particle(component))return;
    uintptr_t owner=0,asset=0;unsigned char flags=0;
    if(!Read(component,0xE8,owner)||owner!=state.actor.address||!Read(component,0x578,asset)||Template(asset)<0||
       !Read(component,0x1CC,flags)||!(flags&0x20))return;
    native_objects::Identity id{};if(!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(component),id))return;
    native_objects::MemoryReader reader;
    for(auto& entry:state.hidden){
        if(entry.component&&!native_objects::Alive(reader,g_base,entry.component))entry={};
        if(!entry.component||entry.component==id){entry={id,state.actor};g_visible(reinterpret_cast<void*>(component),false,1);return;}
    }
}
void HideChildren(Player& state,uintptr_t root){
    std::array<uintptr_t,64> pending{};pending[0]=root;unsigned count=1;
    for(unsigned i=0;i<count;++i){
        uintptr_t children=0;int n=0,cap=0;
        if(!Read(pending[i],0x118,children)||!Read(pending[i],0x120,n)||!Read(pending[i],0x124,cap)||
           n<0||n>cap||cap>128||(!children&&n))continue;
        for(int j=0;j<n;++j){
            uintptr_t child=0,parent=0;
            if(!Read(children,size_t(j)*8,child)||!child||!Read(child,0x108,parent)||parent!=pending[i])continue;
            Hide(state,child);
            if(count<pending.size()&&std::find(pending.begin(),pending.begin()+count,child)==pending.begin()+count)pending[count++]=child;
        }
    }
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));if(!g_base)return;
    struct Guard{uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x189A510,{0x40,0x55,0x53,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x57,0x48,0x8D,0x6C,0x24,0xF9}},
        {0x163EFA0,{0x4C,0x8B,0xC2,0x48,0x85,0xD2,0x74,0x18,0x48,0x39,0x8A,0xE8,0x00,0x00,0x00,0x75}},
        {0x17B1840,{0x40,0x55,0x56,0x57,0x41,0x56,0x41,0x57,0x48,0x8D,0x6C,0x24,0xC9,0x48,0x81,0xEC}},
        {0x19F3EC0,{0x48,0x8B,0xC4,0x56,0x57,0x41,0x56,0x48,0x81,0xEC,0x80,0x00,0x00,0x00,0x48,0xC7}},
        {0x6BC230,{0x48,0x8B,0xC4,0x53,0x48,0x81,0xEC,0x90,0x00,0x00,0x00,0x48,0x8B,0xD9,0x48,0x8B}}
    };
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
        if(log)log("Leviathan visual V1.69 unavailable: native signature mismatch");return;
    }
    g_spawn=reinterpret_cast<SpawnFn>(g_base+0x189A510);g_destroy=reinterpret_cast<DestroyFn>(g_base+0x163EFA0);
    g_visible=reinterpret_cast<VisibleFn>(g_base+0x17B1840);g_ready=true;
    if(log)log("Leviathan visual V1.69 READY: native particle only, no core/status/stats granted; loaded assets only; EXPERIMENTAL");
}
void OnLivePlayer(const local_players::Context& owner){
    if(!Ready()||!native_tps::OnGameThread()||owner.index<0||owner.index>1)return;
    auto& state=g_players[owner.index];const auto& rt=runtime::Get();
    const int choice=rt.leviathanVisualMode.load();const bool enabled=rt.leviathanVisualEnabled.load()&&choice>=0&&choice<=3;
    native_objects::Identity actor{};uintptr_t root=0,reciprocal=0;unsigned char role=0;
    const bool owned=owner.pawn&&Read(owner.pawn,0x118,role)&&role==3&&Read(owner.pawn,0x158,root)&&root&&
        Read(root,0xE8,reciprocal)&&reciprocal==owner.pawn&&
        native_objects::CaptureLive(g_base,reinterpret_cast<void*>(owner.pawn),actor);
    if(!enabled||!owned){Clear(state);state={};return;}
    if(state.actor!=actor||state.selection!=choice){
        Clear(state);state={};state.actor=actor;state.selection=choice;
        g_cursor=0;g_scanComplete=false;
    }
    const auto now=GetTickCount64();
    if(native_tps::IsWorldPaused(reinterpret_cast<void*>(owner.pawn))||native_tps::CinematicActive(unsigned(owner.index)))return;
    if(choice)Scan(now);
    if(now-state.tick<100)return;state.tick=now;
    native_objects::MemoryReader reader;
    // Missing assets fail open: never hide native lightning without a replacement.
    if(choice&&!native_objects::Alive(reader,g_base,g_templates[choice-1])){
        Remove(state);for(auto& hidden:state.hidden)Restore(hidden);return;
    }
    for(auto& hidden:state.hidden){
        uintptr_t asset=0;
        if(hidden.component&&(!Read(hidden.component.address,0x578,asset)||Template(asset)<0))Restore(hidden);
    }
    if(choice){
        if(state.effect&&!native_objects::Alive(reader,g_base,state.effect))state.effect={};
        if(state.effect&&state.parent!=root)Remove(state);
        if(!state.effect&&state.attempts<3){
            ++state.attempts;const Vector zero{},one{1,1,1};
            // Native status particle creation (6BC230)
            // uses actor+158 when BoneName is None. The three Leviathan
            // defaults use None. VM_READ confirmed the native Lvl3 emitter
            // on CollisionCylinder; Strife's mesh is 97 units below it.
            void* component=g_spawn(reinterpret_cast<void*>(g_templates[choice-1].address),reinterpret_cast<void*>(root),0,&zero,&zero,&one,0,true,0);
            uintptr_t parent=0,componentOwner=0;
            if(component&&Particle(reinterpret_cast<uintptr_t>(component))&&Read(reinterpret_cast<uintptr_t>(component),0x108,parent)&&parent==root&&
               Read(reinterpret_cast<uintptr_t>(component),0xE8,componentOwner)&&componentOwner==owner.pawn&&
               native_objects::CaptureLive(g_base,component,state.effect))state.parent=root;
            else if(component&&Read(reinterpret_cast<uintptr_t>(component),0xE8,componentOwner)&&componentOwner==owner.pawn)
                g_destroy(reinterpret_cast<void*>(owner.pawn),component);
        }
        if(!state.effect){
            for(auto& hidden:state.hidden)Restore(hidden);
            if(g_log&&state.attempts==3){g_log("Leviathan visual V1.69 stopped after three unsuccessful spawns; native effect retained");++state.attempts;}
            return;
        }
    }
    HideChildren(state,root);
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
unsigned LoadedMask(){return g_loaded.load();}
void Shutdown(){g_ready=false;}
}
