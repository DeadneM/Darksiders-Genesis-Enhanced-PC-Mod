#include "EffectAppearanceFeature.h"
#include "EffectAppearanceData.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include "NativeCostProbe.h"
#include "ShieldVisualFeature.h"
#include "SelectedParticleLoader.h"
#include "SelectedAssetRecovery.h"
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
selected_particle::State& g_shieldLoader=selected_particle::sharedContext;
selected_recovery::Requests<Assets.size()> g_recovery{};
std::array<std::atomic_bool,FamilyCount> g_loadFailed{};std::atomic_int g_loadChoice{-1},g_loadResult{0};
Create g_create=nullptr;Visible g_visible=nullptr;uintptr_t g_base=0;LogFn g_log=nullptr;
std::atomic_bool g_ready{false};std::array<std::atomic_uint,FamilyCount> g_loaded{};
std::array<Id,Assets.size()> g_assets{};Id g_particleClass{};
struct World {Id id{};uint64_t tick=0;};std::array<World,2> g_worlds{};
struct HiddenEffect {Id component{},asset{};unsigned family=0;};
std::array<HiddenEffect,128> g_hidden{};std::atomic_uint g_hiddenCount{0};unsigned g_changes=0;
std::bitset<Assets.size()> g_available{};
int g_cursor=0,g_count=0;bool g_complete=false;unsigned g_generation=0;
uint64_t g_scanTick=0,g_checkTick=0,g_restoreTick=0,g_retryTick=0,g_recoveryTick=0;
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
    for(unsigned i=0;i<g_assets.size();++i)if(Alive(g_assets[i]))g_available.set(i);else if(g_assets[i]){g_assets[i]={};g_recovery.Forget(i);invalid=true;}
    if(g_particleClass&&!Alive(g_particleClass)){g_particleClass={};invalid=true;}
    if(invalid){g_cursor=0;g_complete=false;}
    for(unsigned family=0;family<FamilyCount;++family){
        unsigned mask=1;bool any=false;for(unsigned i=0;i<Assets.size();++i)if(!Assets[i].targetOnly&&Roles[Assets[i].role].family==family&&g_available[i]){any=true;break;}
        if(any)mask|=1u<<Hidden(family);
        for(int choice=1;choice<int(Families[family].count);++choice){if(choice==Hidden(family))continue;bool needed=false,complete=true;for(unsigned role=0;role<Roles.size();++role)if(Roles[role].family==family){const int target=Target(role,choice);if(target>=0){needed=true;complete=complete&&g_available[unsigned(target)];}}if(needed&&complete)mask|=1u<<choice;}
        g_loaded[family].store(mask,std::memory_order_relaxed);
    }
}
void Scan(uint64_t now){
    cost_probe::Scope measured(cost_probe::EffectScan);
    Availability(now,false);if(g_available.all()||(now>=g_scanTick&&now-g_scanTick<16))return;g_scanTick=now;
    uintptr_t chunks=0;int count=0;if(!Read(g_base,0x3959680,chunks)||!Read(g_base,0x3959694,count)||count<=0||count>2000000)return;
    if(count<g_count)g_cursor=0;if(count!=g_count){g_count=count;g_complete=false;}
    if(g_complete){
        bool missing=false;for(unsigned family=0;family<FamilyCount;++family){
            const int selected=Normalize(family,runtime::Get().effectAppearances[family].load());
            if(selected&&!(g_loaded[family].load()&(1u<<unsigned(selected))))missing=true;
        }
        if(!missing||(now>=g_retryTick&&now-g_retryTick<5000))return;
        g_cursor=0;g_complete=false;g_retryTick=now;
    }
    struct Item {uintptr_t pointer=0;unsigned flags=0,pad=0;int serial=0,pad1=0;};static_assert(sizeof(Item)==24);
    unsigned inspected=0;bool changed=false;
    while(inspected<512&&g_cursor<count){
        uintptr_t chunk=0;if(!Read(chunks,size_t(g_cursor/65536)*8,chunk))break;
        const int n=std::min({16,count-g_cursor,65536-g_cursor%65536});std::array<Item,16> items{};
        if(n==16){if(!Read(chunk,size_t(g_cursor%65536)*24,items))break;}
        else {bool valid=true;for(int i=0;i<n;++i)valid=Read(chunk,size_t(g_cursor%65536+i)*24,items[i])&&valid;if(!valid)break;}
        for(int i=0;i<n;++i){const auto& item=items[i];Object object{};
            if(!item.pointer||(item.flags&0x30000000u)||!Read(item.pointer,0,object))continue;
            if((object.flags&0x10)&&!selected_particle::Context(g_base,g_shieldLoader))selected_particle::Observe(g_base,item.pointer,g_shieldLoader);
            if(object.flags&0x30u)continue;
            const int kind=Classify(item.pointer,object);if(kind>=0&&!g_available[unsigned(kind)]&&Capture(item.pointer,g_assets[unsigned(kind)])){g_available.set(unsigned(kind));changed=true;}
        }
        g_cursor+=n;inspected+=unsigned(n);
    }
    if(g_cursor>=count){g_complete=true;g_retryTick=now;}if(changed)Availability(now,true);
}
void RecoverSelected(const local_players::Context& c,const Id& world,uint64_t now){
    const auto generation=runtime::Get().effectAppearanceGeneration.load();
    const selected_recovery::Scope scope{world.address,0,uint32_t(world.serial),generation};
    if(g_recovery.scoped&&g_recovery.scope==scope&&now>=g_recoveryTick&&now-g_recoveryTick<250)return;
    g_recoveryTick=now;
    g_recovery.Enter(scope,now);
    std::bitset<Assets.size()> needed;
    static_assert(FamilyCount<=64);
    std::array<uint64_t,Assets.size()> wantedBy{};
    for(unsigned family=0;family<FamilyCount;++family){
        g_loadFailed[family].store(false,std::memory_order_relaxed);
        if(family==unsigned(Family::ShieldDemonCore))continue; // validated status-owned loader
        const int choice=Normalize(family,runtime::Get().effectAppearances[family].load());
        if(!choice||choice==Hidden(family))continue;
        for(unsigned role=0;role<Roles.size();++role)if(Roles[role].family==family){
            const int target=Target(role,choice);if(target>=0){needed.set(unsigned(target));wantedBy[unsigned(target)]|=uint64_t(1)<<family;}
        }
    }
    if(needed.none())return;
    const bool context=selected_particle::Context(g_base,g_shieldLoader);
    const bool safe=selected_recovery::loadingAllowed&&context&&
        !native_tps::IsWorldPaused(reinterpret_cast<void*>(c.pawn))&&
        !native_tps::CinematicActive(unsigned(c.index))&&!native_tps::RadialActive(unsigned(c.index));
    bool attempted=false;
    for(unsigned i=0;i<Assets.size();++i){
        if(!needed[i]||g_available[i])continue;
        if(!attempted&&g_recovery.Begin(i,now,safe,context)){
            attempted=true;const auto& entry=Assets[i];
            auto* result=selected_particle::Load(g_base,entry.package,entry.name,g_shieldLoader);
            const auto p=reinterpret_cast<uintptr_t>(result);Object object{};
            const bool accepted=result&&Read(p,0,object)&&Classify(p,object)==int(i)&&Capture(p,g_assets[i]);
            if(accepted){g_available.set(i);Availability(now,true);}
            if(g_log){char message[224]{};sprintf_s(message,"Selected effect V1.150: asset=%s attempt=%u %s",entry.name,g_recovery.entries[i].count,accepted?"loaded and verified":"load unavailable; native retained");g_log(message);}
        }
        if(!g_available[i]&&g_recovery.Exhausted(i)){
            for(unsigned family=0;family<FamilyCount;++family)if(wantedBy[i]&(uint64_t(1)<<family))g_loadFailed[family].store(true,std::memory_order_relaxed);
        }
    }
}
const World* CurrentWorld(uintptr_t address,uint64_t now){
    for(const auto& world:g_worlds)if(world.id.address==address&&world.tick&&now>=world.tick&&now-world.tick<=500&&Alive(world.id))return &world;
    return nullptr;
}
struct Plan {void* asset=nullptr;int source=-1;unsigned family=0;bool hidden=false;};
Plan Resolve(void* asset,void* world){
    Plan plan{asset};if(shield_visual::OwnSpawnActive())return plan;if(!g_ready.load(std::memory_order_relaxed)||!runtime::Get().effectAppearanceMask.load(std::memory_order_relaxed)||!asset||!world||!native_tps::OnGameThread())return plan;
    int source=-1;for(unsigned i=0;i<g_assets.size();++i)if(g_assets[i].address==reinterpret_cast<uintptr_t>(asset)){if(Alive(g_assets[i]))source=int(i);break;}
    if(source<0)return plan;
    const auto sourceIndex=unsigned(source);if(sourceIndex>=Assets.size())return plan;
    const auto& entry=Assets[sourceIndex];if(entry.targetOnly||entry.role>=Roles.size())return plan;
    const auto family=Roles[entry.role].family;if(family>=FamilyCount||family==unsigned(Family::ShieldDemonCore))return plan;
    const int choice=Normalize(family,runtime::Get().effectAppearances[family].load(std::memory_order_relaxed));
    if(!choice||!CurrentWorld(reinterpret_cast<uintptr_t>(world),GetTickCount64()))return plan;
    plan.source=source;plan.family=family;plan.hidden=choice==Hidden(family);
    if(!plan.hidden){const int target=Target(entry.role,choice);if(target>=0&&Alive(g_assets[unsigned(target)]))plan.asset=reinterpret_cast<void*>(g_assets[unsigned(target)].address);}
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
    g_visible=reinterpret_cast<Visible>(g_base+0x17B1840);shield_visual::Initialize(g_base,g_visible,log);g_ready=true;
    if(log)log("Effect appearances V1.116 READY: shared template library, Native defaults, no startup scan or gameplay class changes; EXPERIMENTAL");
}
void OnLivePlayer(const local_players::Context& c){
    if(!g_ready||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    shield_visual::OnLivePlayer(c);
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
    w.tick=now;Scan(now);RecoverSelected(c,w.id,now);
}
bool SelectionLoadFailed(unsigned family){return family<FamilyCount&&g_loadFailed[family].load(std::memory_order_relaxed);}
bool ShieldLoadFailed(int choice){return g_loadChoice.load()==choice&&g_loadResult.load()<0;}
bool ShieldLoaderReady(){return selected_particle::Context(g_base,g_shieldLoader);}
void* ShieldAsset(int choice,bool requestLoad){
    if(!g_ready||!native_tps::OnGameThread())return nullptr;
    const int target=Target(93,choice);if(target<0||unsigned(target)>=Assets.size())return nullptr;const auto i=unsigned(target);
    if(Alive(g_assets[i]))return reinterpret_cast<void*>(g_assets[i].address);
    if(!requestLoad)return nullptr;
    g_loadChoice=choice;g_loadResult=0;
    const auto& asset=Assets[i];void* loaded=selected_particle::Load(g_base,asset.package,asset.name,g_shieldLoader);
    Object object{};const auto p=reinterpret_cast<uintptr_t>(loaded);
    if(!loaded||!Read(p,0,object)||Classify(p,object)!=target||!Capture(p,g_assets[i])){
        g_loadResult=-1;if(g_log)g_log("Shield asset V1.147: selected cosmetic load failed or contract unavailable; native retained");return nullptr;
    }
    g_loadResult=1;Availability(GetTickCount64(),true);if(g_log)g_log("Shield asset V1.147: selected ParticleSystem loaded and package/name/class verified");return loaded;
}
bool Ready(){return g_ready.load(std::memory_order_relaxed);}
unsigned LoadedMask(unsigned family){return family<FamilyCount?g_loaded[family].load(std::memory_order_relaxed):0;}
void Shutdown(){if(native_tps::OnGameThread())for(auto& saved:g_hidden)if(saved.component)Restore(saved);shield_visual::Shutdown();g_ready=false;}
}
