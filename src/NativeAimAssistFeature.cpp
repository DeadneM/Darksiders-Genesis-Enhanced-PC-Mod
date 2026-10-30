#include "NativeAimAssistFeature.h"
#include "NativeMemoryReader.h"
#include "NativeObjectName.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <atomic>
#include <cstdio>
#include <cstring>
namespace dg::native_assist {
namespace {
using Vec=native_tps::Vec3;
using Select=void*(*)(void*,void*,const Vec*,void*);
using AimPoint=Vec*(*)(Vec*,void*,uint64_t);
Select g_select=nullptr;AimPoint g_point=nullptr;
uintptr_t g_base=0;projectile_aim::LogFn g_log=nullptr;projectile_aim::RayProvider g_ray=nullptr;
std::atomic_bool g_ready{false};thread_local bool g_nested=false;
std::atomic_uint32_t g_queries[2]{},g_scaled[2]{},g_points[2]{},g_unmapped{0};
struct Choice {uintptr_t local=0,character=0,pawn=0,weapon=0;uint32_t generation=0;uint64_t time=0,bone=0;float percent=100;native_objects::Identity target{};};
Choice g_choices[2][aim_assist::Count+1]{};
struct Observation {bool known=false;float percent=100;unsigned changes=0;};
Observation g_observed[2][aim_assist::Count+1]{};
template<class T> bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
bool Name(uintptr_t p,const char* text){int n=-1,number=-1;return Read(p,0x18,n)&&Read(p,0x1C,number)&&native_objects::NameValueEquals(g_base,n,number,text);}
int Kind(uintptr_t weapon){
    uintptr_t cls=0,package=0;uint32_t flags=0;
    if(!Read(weapon,8,flags)||(flags&0x18030u)||!Read(weapon,0x10,cls)||!Read(cls,0x20,package))return -1;
    for(unsigned i=0;i<aim_assist::Count;++i)if(Name(cls,aim_assist::Classes[i])&&Name(package,aim_assist::Packages[i]))return int(i);
    return -1;
}
float Percent(int kind){const auto& r=runtime::Get();if(!r.tpsAimAssistEnabled.load())return 100;
    if(r.tpsAimAssistMode.load()==1)return kind>=0?aim_assist::Strength(r.tpsAimAssistAmmo[unsigned(kind)].load()):100;
    return aim_assist::Strength(r.tpsAimAssistGlobal.load());}
bool Scope(uintptr_t finder,uintptr_t actor,local_players::Context& owner,uintptr_t& weapon,int& kind){
    uintptr_t cls=0,ability=0,other=0,table=0,getter=0,registry=0,list=0;uint32_t flags=0;int n=0,cap=0;unsigned char type=0;
    if(!native_tps::OnGameThread()||!local_players::ResolveActor(reinterpret_cast<void*>(actor),owner)||owner.index<0||owner.index>1||
       owner.kind!=local_players::PoseKind::Foot||owner.character!=actor||owner.pawn!=actor||!Read(actor,0xC18,type)||type!=1||
       native_tps::CinematicActive(unsigned(owner.index))||native_tps::IsWorldPaused(reinterpret_cast<void*>(actor))||native_tps::RadialActive(unsigned(owner.index))||
       !Read(finder,8,flags)||(flags&0x18030u)||!Read(finder,0x10,cls)||!Name(cls,"MayhemTargetFinder")||
       !Read(finder,0x58,other)||other!=actor||!Read(finder,0x20,ability)||!Read(ability,8,flags)||(flags&0x18030u)||
       !Read(ability,0,table)||!Read(table,0x2F0,getter)||getter!=g_base+0x5A1620||
       !Read(ability,0x218,other)||other!=finder||!Read(ability,0x48,other)||other!=actor||
       !Read(ability,0x1B0,other)||other!=actor||!Read(ability,0x1D8,weapon)||!Read(weapon,0xE8,other)||other!=actor||
       !Read(actor,0xC78,registry)||!Read(registry,0xE8,other)||other!=actor||!Read(registry,0x108,list)||!list||
       !Read(registry,0x110,n)||!Read(registry,0x114,cap)||n<4||n>cap||cap>32)return false;
    bool registered=false;for(int i=0;i<n;++i)if(Read(list,size_t(i)*8,other)&&other==weapon){registered=true;break;}
    if(!registered)return false;kind=Kind(weapon);
    if(kind<0){if(!Read(weapon,0x10,cls)||!Name(cls,"MayhemWeapon_DualPistols_Blueprint_C"))return false;}
    return true;
}
bool Writable(uintptr_t p){MEMORY_BASIC_INFORMATION m{};if(!VirtualQuery(reinterpret_cast<void*>(p),&m,sizeof(m))||m.State!=MEM_COMMIT||(m.Protect&(PAGE_GUARD|PAGE_NOACCESS)))return false;
    const auto access=m.Protect&255;const auto at=p-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return (access==PAGE_READWRITE||access==PAGE_WRITECOPY||access==PAGE_EXECUTE_READWRITE||access==PAGE_EXECUTE_WRITECOPY)&&at<=m.RegionSize&&8<=m.RegionSize-at;
}
struct Angles {
    float* pointer=nullptr;float original[2]{};
    bool Set(uintptr_t finder,float percent){
        if(percent==100)return true;
        float v=0,h=0,values[2]{};if(!Read(finder,0x2C,v)||!Read(finder,0x30,h)||!aim_assist::Angles(v,h,percent,values[0],values[1])||!Writable(finder+0x2C))return false;
        pointer=reinterpret_cast<float*>(finder+0x2C);original[0]=v;original[1]=h;std::memcpy(pointer,values,sizeof(values));return true;
    }
    ~Angles(){if(pointer)std::memcpy(pointer,original,sizeof(original));}
};
void* SelectTarget(void* finder,void* actor,const Vec* direction,void* predicate){
    if(!g_select)return nullptr;
    local_players::Context owner{};uintptr_t weapon=0;int kind=-1;native_tps::MouseRay ray{};
    if(!g_ready||g_nested||!Scope(reinterpret_cast<uintptr_t>(finder),reinterpret_cast<uintptr_t>(actor),owner,weapon,kind)||!g_ray||!g_ray(owner,ray)){
        ++g_unmapped;return g_select(finder,actor,direction,predicate);
    }
    const auto p=unsigned(owner.index);
    if(p>=2||kind< -1||kind>=int(aim_assist::Count))return g_select(finder,actor,direction,predicate);
    const auto slot=kind<0?aim_assist::Count:unsigned(kind);
    if(slot>aim_assist::Count)return g_select(finder,actor,direction,predicate);
    const float percent=Percent(kind);
    auto& choice=g_choices[p][slot];choice={};void* selected=nullptr;bool scaled=false,safe=false;
    struct Nested {Nested(){g_nested=true;}~Nested(){g_nested=false;}} nested;
    {Angles angles;safe=angles.Set(reinterpret_cast<uintptr_t>(finder),percent);
        scaled=safe&&percent!=100;
        // Same original selector, callback, owner and cache. TPS Camera Forward
        // supplies its direction; only two native angular limits are borrowed.
        selected=g_select(finder,actor,&ray.direction,predicate);
    } // Restore native angles BEFORE recording or calling any point helper.
    ++g_queries[p];if(scaled)++g_scaled[p];
    const auto generation=local_players::PublishedSlot(p).generation;
    choice.local=owner.localPlayer;choice.character=owner.character;choice.pawn=owner.pawn;choice.weapon=weapon;
    choice.generation=generation;choice.time=GetTickCount64();choice.percent=safe?percent:100;
    if(selected){native_objects::MemoryReader reader;
        native_objects::Capture(reader,g_base,reinterpret_cast<uintptr_t>(selected),choice.target);
        Read(reinterpret_cast<uintptr_t>(finder),0x3C,choice.bone);
    }
    auto& observed=g_observed[p][slot];const bool changed=!observed.known||observed.percent!=choice.percent;
    observed.known=true;observed.percent=choice.percent;
    if(g_log&&(g_queries[p].load()<=8||(changed&&observed.changes++<16))){float radius=0,v=0,h=0;Read(reinterpret_cast<uintptr_t>(finder),0x28,radius);Read(reinterpret_cast<uintptr_t>(finder),0x2C,v);Read(reinterpret_cast<uintptr_t>(finder),0x30,h);
        char line[300]{};sprintf_s(line,"TPS V1.135 NATIVE_ASSIST player=%u ammo=%d percent=%.1f scaled=%d nativeRadius=%.1f nativeAngles=%.1f/%.1f restored=1 selected=%d gameplayValidation=USER_REQUIRED",p+1,kind,percent,scaled?1:0,radius,v,h,selected?1:0);g_log(line);}
    return selected;
}
}
int AmmoKind(void* weapon){return Kind(reinterpret_cast<uintptr_t>(weapon));}
bool Target(const local_players::Context& owner,void* weapon,const native_tps::MouseRay& ray,const Vec& origin,Vec& point){
    if(!g_ready||!g_point||!native_tps::OnGameThread()||owner.index<0||owner.index>1||!weapon)return false;
    const int kind=Kind(reinterpret_cast<uintptr_t>(weapon));if(kind<0)return false;
    const auto p=unsigned(owner.index);const auto& rt=runtime::Get();const auto generation=local_players::PublishedSlot(p).generation;
    auto eligible=[&](const Choice& c,bool common){return c.local==owner.localPlayer&&c.character==owner.character&&c.pawn==owner.pawn&&c.generation==generation&&
        (common||c.weapon==reinterpret_cast<uintptr_t>(weapon))&&GetTickCount64()-c.time<=250&&c.percent==Percent(common?-1:kind);};
    const Choice* c=&g_choices[p][unsigned(kind)];
    if(!eligible(*c,false)){
        if(rt.tpsAimAssistEnabled.load()&&rt.tpsAimAssistMode.load()==1)return false;
        c=&g_choices[p][aim_assist::Count];if(!eligible(*c,true))return false;
    }
    native_objects::MemoryReader reader;uint32_t flags=0;if(!native_objects::Alive(reader,g_base,c->target)||!Read(c->target.address,8,flags)||(flags&0x18030u))return false;
    Vec candidate{};g_point(&candidate,reinterpret_cast<void*>(c->target.address),c->bone);
    const float ahead=(candidate.x-origin.x)*ray.direction.x+(candidate.y-origin.y)*ray.direction.y+(candidate.z-origin.z)*ray.direction.z;
    if(!std::isfinite(candidate.x)||!std::isfinite(candidate.y)||!std::isfinite(candidate.z)||!std::isfinite(ahead)||ahead<=.01f)return false;
    point=candidate;++g_points[p];return true;
}
void Initialize(projectile_aim::LogFn log,bool exact,projectile_aim::RayProvider provider){
    g_log=log;g_ray=provider;if(!exact)return;
    const auto base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char selector[]={0x48,0x8B,0xC4,0x4C,0x89,0x48,0x20,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56};
    constexpr unsigned char point[]={0x40,0x56,0x57,0x41,0x56,0x48,0x81,0xEC,0x90,0x00,0x00,0x00,0x48,0xC7,0x44,0x24};
    constexpr unsigned char getter[]={0x48,0x8B,0x81,0x18,0x02,0x00,0x00,0xC3};
    // Getter bytes are verified alongside the live ABI before delivery.
    if(std::memcmp(reinterpret_cast<void*>(base+0x6C3520),selector,sizeof(selector))||std::memcmp(reinterpret_cast<void*>(base+0x66A620),point,sizeof(point))||
       std::memcmp(reinterpret_cast<void*>(base+0x5A1620),getter,sizeof(getter))){if(log)log("TPS V1.135 NATIVE_ASSIST signatures unavailable; native selector retained");return;}
    g_base=base;void* original=nullptr;void* target=reinterpret_cast<void*>(base+0x6C3520);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&SelectTarget),&original)!=MH_OK)return;
    g_select=reinterpret_cast<Select>(original);g_point=reinterpret_cast<AimPoint>(base+0x66A620);
    if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_select=nullptr;g_point=nullptr;return;}
    g_ready=true;if(log)log("TPS V1.135 NATIVE_ASSIST READY: native target selection and chains; optional angular scale;100%=native; EXPERIMENTAL");
}
void Report(){if(!g_log)return;char line[260]{};sprintf_s(line,"TPS V1.135 NATIVE_ASSIST totals queries=%u/%u scaled=%u/%u nativePoints=%u/%u passthrough=%u gameplayValidation=USER_REQUIRED",g_queries[0].load(),g_queries[1].load(),g_scaled[0].load(),g_scaled[1].load(),g_points[0].load(),g_points[1].load(),g_unmapped.load());g_log(line);}
}
