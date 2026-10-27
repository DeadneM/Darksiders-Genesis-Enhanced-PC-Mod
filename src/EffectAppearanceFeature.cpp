#include "EffectAppearanceFeature.h"
#include "EffectAppearanceData.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <algorithm>
#include <atomic>
#include <bitset>
#include <cstdio>

namespace dg::effect_appearance {
namespace {
using Id=native_objects::Identity;
using Create=void*(*)(void*,void*,void*,bool,unsigned char);
using Visible=void(*)(void*,bool,unsigned char);
Create g_create=nullptr;Visible g_visible=nullptr;uintptr_t g_base=0;LogFn g_log=nullptr;
std::atomic_bool g_ready{false};std::array<std::atomic_uint,FamilyCount> g_loaded{};
std::array<Id,Assets.size()> g_assets{};Id g_particleClass{};
struct World {Id id{};uint64_t tick=0;};std::array<World,2> g_worlds{};
struct HiddenEffect {Id component{},asset{};unsigned family=0;};
std::array<HiddenEffect,128> g_hidden{};std::atomic_uint g_hiddenCount{0};unsigned g_changes=0;
std::bitset<Assets.size()> g_available{};
int g_cursor=0,g_count=0;bool g_complete=false;unsigned g_generation=0;
uint64_t g_scanTick=0,g_checkTick=0,g_restoreTick=0;
template<class T>bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),id);}
int Compare(const char* a,const char* b){
    for(;;++a,++b){const unsigned x=unsigned(*a>='A'&&*a<='Z'?*a+32:*a),y=unsigned(*b>='A'&&*b<='Z'?*b+32:*b);if(x!=y)return x<y?-1:1;if(!x)return 0;}
}
bool ObjectName(uintptr_t p,std::array<char,256>& out){
    int index=0,number=0;uintptr_t names=0,chunk=0,entry=0;unsigned flags=0;
    if(!Read(p,0x18,index)||!Read(p,0x1C,number)||number||index<0||index>=2000000||!Read(g_base,0x3955198,names)||
       !Read(names,size_t(index/16384)*8,chunk)||!Read(chunk,size_t(index%16384)*8,entry)||!Read(entry,8,flags))return false;
    out={};if(flags&1){std::array<wchar_t,256> text{};if(!Read(entry,12,text))return false;for(unsigned i=0;i<text.size();++i){if(text[i]>127)return false;out[i]=char(text[i]);if(!text[i])return true;}return false;}
    return Read(entry,12,out)&&std::find(out.begin(),out.end(),char(0))!=out.end();
}
struct Object {uintptr_t vt=0;unsigned flags=0;int index=0;uintptr_t cls=0;int name=0,number=0;uintptr_t outer=0;};
static_assert(sizeof(Object)==40);
int Classify(uintptr_t p,const Object& object){
    if(g_particleClass&&object.cls!=g_particleClass.address)return -1;
    if(!Alive(g_particleClass)){
        if(!native_objects::NameIs(g_base,object.cls,"ParticleSystem")||!Capture(object.cls,g_particleClass))return -1;
    }
    if(object.cls!=g_particleClass.address)return -1;
    std::array<char,256> name{},package{};if(!ObjectName(p,name))return -1;
    const auto found=std::lower_bound(Assets.begin(),Assets.end(),name.data(),[](const AssetInfo& a,const char* text){return Compare(a.name,text)<0;});
    if(found==Assets.end()||Compare(found->name,name.data())||!ObjectName(object.outer,package)||Compare(found->package,package.data()))return -1;
    return int(found-Assets.begin());
}
void Availability(uint64_t now,bool force){
    if(!force&&now>=g_checkTick&&now-g_checkTick<200)return;g_checkTick=now;bool invalid=false;g_available.reset();
    for(unsigned i=0;i<g_assets.size();++i)if(Alive(g_assets[i]))g_available.set(i);else if(g_assets[i]){g_assets[i]={};invalid=true;}
    if(g_particleClass&&!Alive(g_particleClass))g_particleClass={};
    if(invalid){g_cursor=0;g_complete=false;}
    for(unsigned family=0;family<FamilyCount;++family){
        unsigned mask=1;bool any=false;for(unsigned i=0;i<Assets.size();++i)if(Roles[Assets[i].role].family==family&&g_available[i]){any=true;break;}
        if(any)mask|=1u<<Hidden(family);
        for(int choice=1;choice<Hidden(family);++choice){bool needed=false,complete=true;for(unsigned role=0;role<Roles.size();++role)if(Roles[role].family==family){const int target=Target(role,choice);if(target>=0){needed=true;complete=complete&&g_available[unsigned(target)];}}if(needed&&complete)mask|=1u<<choice;}
        g_loaded[family].store(mask,std::memory_order_relaxed);
    }
}
void Scan(uint64_t now){
    Availability(now,false);if(g_available.all()||(now>=g_scanTick&&now-g_scanTick<16))return;g_scanTick=now;
    uintptr_t chunks=0;int count=0;if(!Read(g_base,0x3959680,chunks)||!Read(g_base,0x3959694,count)||count<=0||count>2000000)return;
    if(count<g_count)g_cursor=0;if(count!=g_count){g_count=count;g_complete=false;}if(g_complete)return;
    struct Item {uintptr_t pointer=0;unsigned flags=0,pad=0;int serial=0,pad1=0;};static_assert(sizeof(Item)==24);
    unsigned inspected=0;bool changed=false;
    while(inspected<512&&g_cursor<count){
        uintptr_t chunk=0;if(!Read(chunks,size_t(g_cursor/65536)*8,chunk))break;
        const int n=std::min({16,count-g_cursor,65536-g_cursor%65536});std::array<Item,16> items{};
        if(n==16){if(!Read(chunk,size_t(g_cursor%65536)*24,items))break;}
        else {bool valid=true;for(int i=0;i<n;++i)valid=Read(chunk,size_t(g_cursor%65536+i)*24,items[i])&&valid;if(!valid)break;}
        for(int i=0;i<n;++i){const auto& item=items[i];Object object{};
            if(!item.pointer||(item.flags&0x30000000u)||!Read(item.pointer,0,object)||(object.flags&0x30u))continue;
            const int kind=Classify(item.pointer,object);if(kind>=0&&!g_available[unsigned(kind)]&&Capture(item.pointer,g_assets[unsigned(kind)])){g_available.set(unsigned(kind));changed=true;}
        }
        g_cursor+=n;inspected+=unsigned(n);
    }
    if(g_cursor>=count)g_complete=true;if(changed)Availability(now,true);
}
const World* CurrentWorld(uintptr_t address,uint64_t now){
    for(const auto& world:g_worlds)if(world.id.address==address&&world.tick&&now>=world.tick&&now-world.tick<=500&&Alive(world.id))return &world;
    return nullptr;
}
struct Plan {void* asset=nullptr;int source=-1;unsigned family=0;bool hidden=false;};
Plan Resolve(void* asset,void* world){
    Plan plan{asset};if(!g_ready.load(std::memory_order_relaxed)||!runtime::Get().effectAppearanceMask.load(std::memory_order_relaxed)||!asset||!world||!native_tps::OnGameThread())return plan;
    int source=-1;for(unsigned i=0;i<g_assets.size();++i)if(g_assets[i].address==reinterpret_cast<uintptr_t>(asset)){if(Alive(g_assets[i]))source=int(i);break;}
    if(source<0)return plan;const auto role=Assets[unsigned(source)].role,family=Roles[role].family;
    const int choice=Normalize(family,runtime::Get().effectAppearances[family].load(std::memory_order_relaxed));
    if(!choice||!CurrentWorld(reinterpret_cast<uintptr_t>(world),GetTickCount64()))return plan;
    plan.source=source;plan.family=family;plan.hidden=choice==Hidden(family);
    if(!plan.hidden){const int target=Target(role,choice);if(target>=0&&Alive(g_assets[unsigned(target)]))plan.asset=reinterpret_cast<void*>(g_assets[unsigned(target)].address);}
    return plan;
}
void Restore(HiddenEffect& saved){
    unsigned char flags=0;if(Alive(saved.component)&&Read(saved.component.address,0x1CC,flags)&&!(flags&0x20)&&g_visible)g_visible(reinterpret_cast<void*>(saved.component.address),true,1);
    saved={};if(g_hiddenCount)--g_hiddenCount;
}
void Created(void* component,const Plan& plan){
    if(!component||!native_tps::OnGameThread())return;
    if(g_hiddenCount)for(auto& saved:g_hidden)if(saved.component.address==reinterpret_cast<uintptr_t>(component)){Restore(saved);break;}
    if(!plan.hidden||plan.source<0||!g_visible)return;uintptr_t cls=0,asset=0;unsigned char flags=0;
    if(!Read(reinterpret_cast<uintptr_t>(component),0x10,cls)||!native_objects::NameIs(g_base,cls,"ParticleSystemComponent")||
       !Read(reinterpret_cast<uintptr_t>(component),0x578,asset)||asset!=reinterpret_cast<uintptr_t>(plan.asset)||!Read(reinterpret_cast<uintptr_t>(component),0x1CC,flags)||!(flags&0x20))return;
    for(auto& saved:g_hidden){if(saved.component&&!Alive(saved.component)){saved={};if(g_hiddenCount)--g_hiddenCount;}if(saved.component)continue;
        Id id{};if(!Capture(reinterpret_cast<uintptr_t>(component),id))return;saved={id,g_assets[unsigned(plan.source)],plan.family};++g_hiddenCount;g_visible(component,false,1);return;
    }
}
void* HookCreate(void* asset,void* world,void* outer,bool autoDestroy,unsigned char pooling){
    const auto plan=Resolve(asset,world);void* result=g_create?g_create(plan.asset,world,outer,autoDestroy,pooling):nullptr;
    if(plan.hidden||g_hiddenCount)Created(result,plan);
    if(g_log&&(plan.hidden||plan.asset!=asset)&&g_changes++<16){char line[160]{};sprintf_s(line,"Effect appearance V1.116: family=%u role=%d hidden=%u; native factory/lifecycle retained; EXPERIMENTAL",plan.family,plan.source,unsigned(plan.hidden));g_log(line);}
    return result;
}
}
void Initialize(LogFn log,bool exact){
    g_log=log;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char factory[]={0x48,0x89,0x5C,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xEC,0x60,0x41};
    constexpr unsigned char visible[]={0x40,0x55,0x56,0x57,0x41,0x56,0x41,0x57,0x48,0x8D,0x6C,0x24,0xC9,0x48,0x81,0xEC};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x187E600),factory,sizeof(factory))||std::memcmp(reinterpret_cast<void*>(g_base+0x17B1840),visible,sizeof(visible)))return;
    auto* entry=reinterpret_cast<void*>(g_base+0x187E600);if(MH_CreateHook(entry,reinterpret_cast<void*>(&HookCreate),reinterpret_cast<void**>(&g_create))!=MH_OK)return;
    if(MH_EnableHook(entry)!=MH_OK){MH_RemoveHook(entry);g_create=nullptr;return;}
    g_visible=reinterpret_cast<Visible>(g_base+0x17B1840);g_ready=true;
    if(log)log("Effect appearances V1.116 READY: shared template library, Native defaults, no startup scan or gameplay class changes; EXPERIMENTAL");
}
void OnLivePlayer(const local_players::Context& c){
    if(!g_ready||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    const auto now=GetTickCount64();if(now-g_restoreTick>=100){g_restoreTick=now;for(auto& saved:g_hidden)if(saved.component){uintptr_t asset=0;
        if(!Alive(saved.component)){saved={};if(g_hiddenCount)--g_hiddenCount;continue;}
        if(Normalize(saved.family,runtime::Get().effectAppearances[saved.family].load())!=Hidden(saved.family)||!Read(saved.component.address,0x578,asset)||asset!=saved.asset.address)Restore(saved);
    }}
    if(!runtime::Get().effectAppearanceMask.load()){g_worlds={};return;}auto& w=g_worlds[unsigned(c.index)];uintptr_t cls=0;
    if(!c.localPlayer||!c.controller||!c.pawn||!c.world||!Read(c.world,0x10,cls)||!native_objects::NameIs(g_base,cls,"World")){w={};return;}
    const auto generation=runtime::Get().effectAppearanceGeneration.load();
    if(g_generation!=generation||w.id.address!=c.world||!Alive(w.id)){
        if(!Capture(c.world,w.id)){w={};return;}g_cursor=0;g_count=0;g_complete=false;g_generation=generation;
    }
    w.tick=now;Scan(now);
}
bool Ready(){return g_ready.load(std::memory_order_relaxed);}
unsigned LoadedMask(unsigned family){return family<FamilyCount?g_loaded[family].load(std::memory_order_relaxed):0;}
void Shutdown(){if(native_tps::OnGameThread())for(auto& saved:g_hidden)if(saved.component)Restore(saved);g_ready=false;}
}
