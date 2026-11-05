#include "StoneSkinVisual.h"
#include "StoneTransitionFx.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include "NativeCostProbe.h"
#include "SelectedParticleLoader.h"
#include "SelectedAssetRecovery.h"
#include <array>
#include <cstdio>

namespace dg::stone_visual {
namespace {
using Id=native_objects::Identity;
struct Array {uintptr_t data=0;int count=0,capacity=0;};
using Event=void(*)(void*,void*,void*);
Event g_event=nullptr;uintptr_t g_base=0;foreign_wrath::LogFn g_log=nullptr;unsigned g_events=0;
unsigned g_deferredEvents=0;
std::array<selected_recovery::Requests<6>,2> g_resourceRequests{};
std::array<Id,2> g_resourceActor{},g_resourceMesh{};
enum MaterialSource : unsigned {BaseBody,BaseCloth,MoltenCloth};
std::array<Id,3> g_materials{};
struct Slot {Id material{},original{};bool inherited=false,failed=false;uintptr_t observed=0;uint64_t retryAt=0;};
struct Visual {Id actor{},mesh{},asset{};uintptr_t world=0;int variant=-1;std::array<Slot,4> slots{};};
Visual g_visual[2]{};
struct Transition {Id actor{};uintptr_t world=0;bool pending=false;};
Transition g_transitions[2]{};
bool g_missing[2]{};
struct Observation {local_players::Context owner{};uint64_t next=0;bool entered=false,force=false;};
std::array<Observation,2> g_observations{};
uint64_t g_observed=0,g_skipped=0;
struct StatusClass {Id identity{};unsigned kind=0;};
std::array<StatusClass,128> g_statusClasses{};
size_t g_statusClassNext=0;uint64_t g_statusClassHits=0,g_statusClassMisses=0;
template<class T>bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
template<size_t N>bool Is(uintptr_t p,const char(&s)[N]){return native_objects::NameIs(g_base,p,s);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),id);}
struct CachedField {Id cls{};const char* name=nullptr;const char* kind=nullptr;int size=0,offset=-1;};
std::array<CachedField,24> g_fields{};
bool Field(uintptr_t p,const char* name,const char* kind,int size,int& offset){
    uintptr_t cls=0;if(!Read(p,0x10,cls))return false;
    CachedField* free=nullptr;
    // Validate only the matching cached contract on a hit. Other classes
    // cannot supply this field and are reclaimed only on a cache miss.
    for(auto& f:g_fields){
        if(f.cls.address==cls&&f.size==size&&f.name&&f.kind&&!std::strcmp(f.name,name)&&!std::strcmp(f.kind,kind)){
            if(Alive(f.cls)){offset=f.offset;return true;}f={};
        }
    }
    for(auto& f:g_fields)if(!f.cls||!Alive(f.cls)){free=&f;f={};break;}
    if(!native_objects::PropertyField(g_base,cls,name,kind,size,offset))return false;
    Id identity{};if(free&&Capture(cls,identity))*free={identity,name,kind,size,offset};return true;
}
bool List(uintptr_t p,size_t at,Array& a,int limit){return Read(p,at,a)&&a.count>=0&&a.count<=a.capacity&&a.capacity<=limit&&(a.data||!a.count);}
struct CachedFunction {Id cls{},function{};uintptr_t entry=0;const char* name=nullptr;};
struct CachedParameter {Id function{};const char* name=nullptr;const char* kind=nullptr;int size=0,offset=0;};
std::array<CachedFunction,8> g_functions{};
std::array<CachedParameter,16> g_parameters{};
size_t g_nextFunction=0,g_nextParameter=0;
uint64_t g_functionLookups=0,g_parameterLookups=0;
uintptr_t FindFunction(uintptr_t cls,const char* name,uintptr_t entry){
    ++g_functionLookups;
    for(int n=0;cls&&n<16;++n){
        uintptr_t f=0;if(!Read(cls,0x48,f))return 0;
        for(int i=0;f&&i<512;++i){
            uintptr_t kind=0,fn=0;int index=0,number=0;
            if(!Read(f,0x10,kind)||!Read(f,0x18,index)||!Read(f,0x1C,number))return 0;
            if(native_objects::NameValueIdentifierEquals(g_base,index,number,name))return Is(kind,"Function")&&Read(f,0xC0,fn)&&fn==g_base+entry?f:0;
            if(!Read(f,0x28,f))return 0;
        }
        if(f||!Read(cls,0x40,cls))return 0;
    }
    return 0;
}
uintptr_t Function(uintptr_t object,const char* name,uintptr_t entry){
    uintptr_t cls=0;if(!Read(object,0x10,cls))return 0;
    for(auto& cached:g_functions){
        if(cached.cls.address!=cls||cached.entry!=entry||!cached.name||std::strcmp(cached.name,name))continue;
        uintptr_t actual=0;
        if(Alive(cached.cls)&&Alive(cached.function)&&Read(cached.function.address,0xC0,actual)&&actual==g_base+entry)
            return cached.function.address;
        cached={};
    }
    const auto function=FindFunction(cls,name,entry);
    Id classId{},functionId{};
    if(function&&Capture(cls,classId)&&Capture(function,functionId)){
        g_functions[g_nextFunction]={classId,functionId,entry,name};
        g_nextFunction=(g_nextFunction+1)%g_functions.size();
    }
    return function;
}
bool Parameter(uintptr_t function,const char* name,const char* kind,int size,int offset){
    for(auto& cached:g_parameters){
        if(cached.function.address!=function||cached.size!=size||cached.offset!=offset||
           !cached.name||!cached.kind||std::strcmp(cached.name,name)||std::strcmp(cached.kind,kind))continue;
        if(Alive(cached.function))return true;
        cached={};
    }
    ++g_parameterLookups;
    int actual=-1;
    if(!native_objects::PropertyField(g_base,function,name,kind,size,actual)||actual!=offset)return false;
    Id identity{};
    if(Capture(function,identity)){
        g_parameters[g_nextParameter]={identity,name,kind,size,offset};
        g_nextParameter=(g_nextParameter+1)%g_parameters.size();
    }
    return true;
}
bool Set(uintptr_t mesh,int index,uintptr_t material){
    const auto f=Function(mesh,"SetMaterial",0x1DCCC80);
    if(!f||!Parameter(f,"ElementIndex","IntProperty",4,0)||!Parameter(f,"Material","ObjectProperty",8,8))return false;
    struct Args {int index=0,pad=0;uintptr_t material=0;} args{index,0,material};
    g_event(reinterpret_cast<void*>(mesh),reinterpret_cast<void*>(f),&args);return true;
}
bool Override(uintptr_t mesh,int index,uintptr_t& value){
    int at=-1;Array a{};value=0;
    return Field(mesh,"OverrideMaterials","ArrayProperty",16,at)&&List(mesh,at,a,32)&&
        (index>=a.count||Read(a.data,size_t(index)*8,value));
}
uintptr_t Create(uintptr_t mesh,int index,uintptr_t original){
    const auto f=Function(mesh,"CreateDynamicMaterialInstance",0x1DC92E0);
    if(!f||!Parameter(f,"ElementIndex","IntProperty",4,0)||!Parameter(f,"SourceMaterial","ObjectProperty",8,8)||
       !Parameter(f,"OptionalName","NameProperty",8,16)||!Parameter(f,"ReturnValue","ObjectProperty",8,24))return 0;
    struct Args {int index=0,pad=0;uintptr_t source=0;foreign_wrath::Name name{0,0};uintptr_t result=0;} args{index,0,original};
    static_assert(sizeof(Args)==32);
    g_event(reinterpret_cast<void*>(mesh),reinterpret_cast<void*>(f),&args);return args.result;
}
bool NativeBaseline(uintptr_t actor,uintptr_t mesh,uintptr_t current,uintptr_t assetMaterial){
    // Native character effects can wrap the equipped material in actor-owned
    // MIDs later in the same level. Accept that lineage, preserving the exact
    // current MID for restoration. Never accept a different material root,
    // foreign owner, unknown class, cycle or unbounded chain.
    if(current==assetMaterial&&current)return true;
    int at=-1;Array native{};bool retained=false;
    // Outer alone is not a strong GC reference. The observed native UPROPERTY
    // m_dynMaterials array must retain this exact baseline after replacement.
    if(!Field(actor,"m_dynMaterials","ArrayProperty",16,at)||!List(actor,at,native,32))return false;
    for(int i=0;i<native.count;++i){uintptr_t item=0;if(!Read(native.data,size_t(i)*8,item))return false;retained=retained||item==current;}
    if(!retained)return false;
    std::array<uintptr_t,8> seen{};
    for(unsigned depth=0;current&&depth<seen.size();++depth){
        if(current==assetMaterial)return true;
        uintptr_t cls=0,outer=0,parent=0;at=-1;
        for(unsigned i=0;i<depth;++i)if(seen[i]==current)return false;
        seen[depth]=current;
        if(!Read(current,0x10,cls)||!Is(cls,"MaterialInstanceDynamic")||!Read(current,0x20,outer)||
           (outer!=actor&&outer!=mesh)||!Field(current,"Parent","ObjectProperty",8,at)||!Read(current,at,parent))return false;
        current=parent;
    }
    return current==assetMaterial&&current!=0;
}
int Surface(uintptr_t asset,unsigned index,uintptr_t& original){
    int at=-1;Array slots{};uintptr_t cls=0;foreign_wrath::Name slot{};original=0;
    if(!Read(asset,0x10,cls)||!Is(cls,"SkeletalMesh")||!Field(asset,"Materials","ArrayProperty",16,at)||!List(asset,at,slots,32)||
       index>=unsigned(slots.count)||!Read(slots.data,size_t(index)*40,original)||!original||!Read(slots.data,size_t(index)*40+8,slot))return -1;
    if(native_objects::NameValueIdentifierEquals(g_base,slot.index,slot.number,"Strife_Body")||native_objects::NameValueIdentifierEquals(g_base,slot.index,slot.number,"Strife_Gear")||
       native_objects::NameValueIdentifierEquals(g_base,slot.index,slot.number,"Strife_Abyssal_Body"))return 0;
    if(native_objects::NameValueIdentifierEquals(g_base,slot.index,slot.number,"Strife_Cloth")||
       native_objects::NameValueIdentifierEquals(g_base,slot.index,slot.number,"Strife_Abyssal_Cloth")||
       (slot.number==2&&native_objects::NameValueIdentifierEquals(g_base,slot.index,0,"Strife_Cloth_ncl1"))||
       native_objects::NameValueIdentifierEquals(g_base,slot.index,slot.number,"Strife_Cloth_ncl1_1"))return 1;
    return -1;
}
bool MineralBody(uintptr_t material,uintptr_t source){
    const auto f=Function(material,"SetScalarParameterValue",0x1D97290);int at=-1;Array values{};unsigned found=0;
    if(!f||!Parameter(f,"ParameterName","NameProperty",8,0)||!Parameter(f,"Value","FloatProperty",4,8)||
       !Field(source,"ScalarParameterValues","ArrayProperty",16,at)||!List(source,at,values,64))return false;
    for(int i=0;i<values.count;++i){foreign_wrath::Name name{};int association=-1,index=0;const auto p=values.data+size_t(i)*36;
        if(!Read(p,0,name)||!Read(p,8,association)||!Read(p,12,index)||association!=2||index!=-1)continue;
        const unsigned bit=native_objects::NameValueIs(g_base,name.index,name.number,"Emissive_Power_Base")?1u:
            (native_objects::NameValueIs(g_base,name.index,name.number,"Emissive_Power_Scroll")?2u:0u);
        if(!bit)continue;
        struct Args {foreign_wrath::Name name{};float value=0;int pad=0;} args{name};
        g_event(reinterpret_cast<void*>(material),reinterpret_cast<void*>(f),&args);found|=bit;
    }
    return found==3;
}
void Restore(Visual& v){
    uintptr_t owner=0,asset=0;int at=-1;
    if(g_event&&Alive(v.actor)&&Alive(v.mesh)&&Read(v.mesh.address,0xE8,owner)&&owner==v.actor.address){
        const bool sameAsset=Alive(v.asset)&&Field(v.mesh.address,"SkeletalMesh","ObjectProperty",8,at)&&Read(v.mesh.address,at,asset)&&asset==v.asset.address;
        for(unsigned i=0;i<v.slots.size();++i){auto& s=v.slots[i];uintptr_t current=0;
            // Only our MID may be replaced. A native/user skin change wins.
            if(Alive(s.material)&&Override(v.mesh.address,int(i),current)&&current==s.material.address&&
               (s.inherited||!sameAsset||Alive(s.original)))Set(v.mesh.address,int(i),s.inherited||!sameAsset?0:s.original.address);
        }
    }
    v={};
}
unsigned StatusKind(uintptr_t cls,bool& complete){
    if(!cls){complete=false;return 0;}
    for(auto& cached:g_statusClasses)if(cached.identity.address==cls){
        if(Alive(cached.identity)){++g_statusClassHits;complete=true;return cached.kind;}
        cached={};
    }
    ++g_statusClassMisses;const auto original=cls;unsigned kind=0;complete=true;
    for(unsigned depth=0;cls&&depth<12;++depth){
        if(Is(cls,"MayhemStatusEffect_StoneSkin_Base_Blueprint_C"))kind|=1;
        if(Is(cls,"MayhemStatusEffect_Core_FallenDog_Molten_1_Blueprint_C")||Is(cls,"MayhemStatusEffect_Core_FallenDog_Molten_2_Blueprint_C")||
           Is(cls,"MayhemStatusEffect_Core_FallenDog_Molten_3_Blueprint_C"))kind|=2;
        if(!Read(cls,0x40,cls)){complete=false;break;}
    }
    if(cls)complete=false;
    Id identity{};
    if(complete&&Capture(original,identity)){
        g_statusClasses[g_statusClassNext]={identity,kind};
        g_statusClassNext=(g_statusClassNext+1)%g_statusClasses.size();
    }
    return kind;
}
int Status(uintptr_t actor,bool* known=nullptr){
    int at=-1;uintptr_t component=0,owner=0;Array statuses{};bool stone=false,molten=false,complete=true;
    if(!Field(actor,"m_pStatusEffectComponent","ObjectProperty",8,at)||!Read(actor,at,component)||
       !Read(component,0xE8,owner)||owner!=actor||!Field(component,"m_statusEffects","ArrayProperty",16,at)||!List(component,at,statuses,128))return -1;
    for(int i=0;i<statuses.count;++i){
        uintptr_t status=0,cls=0,instigator=0;
        if(!Read(statuses.data,size_t(i)*8,status)){complete=false;continue;}
        if(!status)continue;
        if(!Read(status,0x10,cls)){complete=false;continue;}
        bool classKnown=false;const unsigned kind=StatusKind(cls,classKnown);
        complete=complete&&classKnown;
        // Irrelevant status classes cannot provide this coating. Their native
        // instigator need not be reflected on every movement/camera callback.
        if(!kind)continue;
        if(!Field(status,"m_pInstigatorActor","ObjectProperty",8,at)||!Read(status,at,instigator)){complete=false;continue;}
        if(instigator!=actor)continue;
        stone=stone||((kind&1u)!=0);molten=molten||((kind&2u)!=0);
    }
    if(known)*known=complete||(stone&&molten);
    return stone?(molten?1:0):-1;
}
}
void Initialize(foreign_wrath::LogFn log,bool exact){
    g_log=log;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    // Existing exact-executable ProcessEvent boundary, possibly already
    // detoured by the mod's UI observer. No additional hook or native ABI.
    g_event=reinterpret_cast<Event>(g_base+0xAC0630);
    stone_fx::Initialize(g_base,g_event);
}
bool NeedsLoaderContext(){return g_base&&!selected_particle::Context(g_base,selected_particle::sharedContext);}
void ObserveObject(uintptr_t p){
    if(g_event&&native_tps::OnGameThread()&&NeedsLoaderContext())selected_particle::Observe(g_base,p,selected_particle::sharedContext);
    stone_fx::ObserveObject(p);
    if(!g_event||!native_tps::OnGameThread())return;uintptr_t cls=0,package=0;
    if(!Read(p,0x10,cls)||!Is(cls,"MaterialInstanceConstant")||!Read(p,0x20,package))return;
    constexpr const char* names[]={"War_Inst_StoneSkin_A","War_Inst_StoneSkin_B","War_Inst_StoneSkin_Molten_B"};
    int nameIndex=0,number=0,packageIndex=0,packageNumber=0;
    if(!Read(p,0x18,nameIndex)||!Read(p,0x1C,number)||!Read(package,0x18,packageIndex)||!Read(package,0x1C,packageNumber))return;
    for(unsigned i=0;i<g_materials.size();++i){char path[180]{};sprintf_s(path,"/Game/Characters/Heroes/Hero_War/Textures/%s",names[i]);
        if(!Alive(g_materials[i])&&native_objects::NameValueEquals(g_base,nameIndex,number,names[i])&&
           native_objects::NameValueEquals(g_base,packageIndex,packageNumber,path))Capture(p,g_materials[i]);
    }
}
bool AssetsReady(){for(const auto& id:g_materials)if(!Alive(id))return false;return stone_fx::AssetsReady();}
bool NeedsAssetScan(const local_players::Context& c){return c.index>=0&&c.index<2&&g_missing[c.index];}
bool InvalidateAssets(){bool changed=stone_fx::InvalidateAssets();for(auto& id:g_materials)if(id&&!Alive(id)){id={};changed=true;}if(changed)for(auto& request:g_resourceRequests)request.entries={};return changed;}
void Cast(const local_players::Context& c){
    if(!g_event||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    Transition next{};if(Capture(c.character,next.actor)){
        next.world=c.world;next.pending=true;g_transitions[c.index]=next;g_resourceRequests[c.index].Reset();
        g_observations[c.index].force=true;
        for(auto& slot:g_visual[c.index].slots)if(slot.failed){slot.failed=false;slot.retryAt=0;}
    }
}

void RecoverResources(const local_players::Context& c,uintptr_t meshAsset,int variant){
    const auto now=GetTickCount64();
    auto& requests=g_resourceRequests[c.index];auto& actor=g_resourceActor[c.index];auto& mesh=g_resourceMesh[c.index];
    if(actor.address!=c.character||mesh.address!=meshAsset||!Alive(actor)||!Alive(mesh)){
        if(!Capture(c.character,actor)||!Capture(meshAsset,mesh))return;
        requests.Reset();
    }
    const uint64_t selection=(uint64_t(uint32_t(mesh.serial))<<32)|unsigned(variant);
    requests.Enter({c.world,c.character,uint32_t(actor.serial),selection},now);
    auto& loader=selected_particle::sharedContext;const bool context=selected_particle::Context(g_base,loader);
    const bool safe=selected_recovery::loadingAllowed&&context&&
        !native_tps::IsWorldPaused(reinterpret_cast<void*>(c.pawn))&&
        !native_tps::CinematicActive(unsigned(c.index))&&!native_tps::RadialActive(unsigned(c.index));
    constexpr const char* materials[]={"War_Inst_StoneSkin_A","War_Inst_StoneSkin_B","War_Inst_StoneSkin_Molten_B"};
    constexpr const char* effects[]={"P_WarStoneSkin_Base","P_WarStoneSkin_Molten","P_War_StoneSkin_Base_Crumble_Expiration"};
    const unsigned wanted[]={0u,variant?2u:1u,3u+unsigned(variant),5u};
    for(const auto index:wanted){
        const bool material=index<3;
        const bool ready=material?Alive(g_materials[index]):stone_fx::TemplateReady(int(index-3));
        if(ready||!requests.Begin(index,now,safe,context))continue;
        const char* name=material?materials[index]:effects[index-3];char package[220]{};
        sprintf_s(package,material?"/Game/Characters/Heroes/Hero_War/Textures/%s":"/Game/VFX/Particles/Characters/War/StoneSkin/%s",name);
        auto* result=selected_particle::Load(g_base,package,name,loader,g_event);
        if(result)ObserveObject(reinterpret_cast<uintptr_t>(result));
        const bool accepted=material?Alive(g_materials[index]):stone_fx::TemplateReady(int(index-3));
        if(g_log){char message[224]{};sprintf_s(message,"Stoneskin resources V1.150: asset=%s attempt=%u %s",name,requests.entries[index].count,accepted?"loaded and verified":"unavailable; native materials retained");g_log(message);}
        break; // shared budget: at most one requested resource per service call
    }
}
void ObserveCurrent(const local_players::Context& c,uint64_t now){
    const bool wasMissing=g_missing[c.index];g_missing[c.index]=false;
    auto& v=g_visual[c.index];uintptr_t cls=0,pc=0,pawn=0,mesh=0,owner=0,asset=0;unsigned char role=0;int at=-1;
    const bool owned=c.localPlayer&&c.world&&c.kind==local_players::PoseKind::Foot&&c.character&&c.pawn==c.character&&
        Read(c.character,0x10,cls)&&Is(cls,"Strife_Blueprint_C")&&Read(c.character,0x118,role)&&role==3&&
        Read(c.character,0x348,pc)&&pc==c.controller&&Read(pc,0x350,pawn)&&pawn==c.character&&
        Read(c.character,0x380,mesh)&&Read(mesh,0xE8,owner)&&owner==c.character&&Field(mesh,"SkeletalMesh","ObjectProperty",8,at)&&Read(mesh,at,asset)&&asset;
    bool statusKnown=false;const int variant=owned?Status(c.character,&statusKnown):-1;
    auto& transition=g_transitions[c.index];
    const bool same=owned&&v.actor.address==c.character&&v.world==c.world&&v.mesh.address==mesh&&v.asset.address==asset&&
        Alive(v.actor)&&Alive(v.mesh)&&Alive(v.asset);
    if(v.actor&&!same)Restore(v);
    if(owned&&!statusKnown){
        g_missing[c.index]=wasMissing;
        return; // An inconclusive status read cannot remove an existing coating.
    }
    if(same&&statusKnown&&variant<0){const bool ok=stone_fx::Spawn(c,2);
        if(g_log&&g_events++<16)g_log(ok?"Stoneskin V1.123 expiry: native crumble at complete coating removal":"Stoneskin V1.123 expiry effect unavailable; native material restoration retained");}
    if(v.actor&&(variant<0||variant!=v.variant||v.world!=c.world||v.actor.address!=c.character||v.mesh.address!=mesh||v.asset.address!=asset||!Alive(v.actor)||!Alive(v.mesh)||!Alive(v.asset)))Restore(v);
    if(!owned||variant<0){if(!owned||statusKnown)transition={};return;}
    // Body always uses the base mineral material, even with Molten cloth.
    // Wait for these actual sources before invoking/caching a factory result.
    bool sourcesReady=Alive(g_materials[BaseBody])&&Alive(g_materials[variant?MoltenCloth:BaseCloth]);
    if(!sourcesReady||!stone_fx::Ready(variant)||!stone_fx::Ready(2)){
        RecoverResources(c,asset,variant);
        sourcesReady=Alive(g_materials[BaseBody])&&Alive(g_materials[variant?MoltenCloth:BaseCloth]);
    }
    // Missing unused variants must not make the active coating unavailable.
    g_missing[c.index]=!sourcesReady||!stone_fx::Ready(variant)||!stone_fx::Ready(2);
    if(!sourcesReady&&!wasMissing&&g_log&&g_deferredEvents++<12){char line[180]{};
        sprintf_s(line,"Stoneskin V1.137 deferred: player=%d variant=%d baseBody=%d cloth=%d; bounded rediscovery requested",c.index+1,variant,Alive(g_materials[BaseBody])?1:0,Alive(g_materials[variant?MoltenCloth:BaseCloth])?1:0);g_log(line);}
    if(!sourcesReady)return;
    if(!v.actor){
        if(!Capture(c.character,v.actor)||!Capture(mesh,v.mesh)||!Capture(asset,v.asset)){v={};return;}
        v.world=c.world;v.variant=variant;
    }
    // Preserve the native material's full shader, including its static fire
    // switches. Original constants stay on the equipped asset; a native MID
    // must remain in the actor's strong material cache during replacement.
    for(unsigned i=0;i<v.slots.size();++i){
        auto& slot=v.slots[i];uintptr_t current=0;
        if(!Override(mesh,int(i),current))continue;
        if(Alive(slot.material)&&current==slot.material.address)continue;
        if(slot.failed&&slot.observed==current&&now<slot.retryAt)continue;
        slot={};
        uintptr_t nativeOriginal=0;const int surface=Surface(asset,i,nativeOriginal);
        if(surface<0){slot.failed=true;slot.observed=current;slot.retryAt=now+1000;continue;}
        const auto f=Function(mesh,"GetMaterial",0x1DC9CB0);
        if(!f||!Parameter(f,"ElementIndex","IntProperty",4,0)||!Parameter(f,"ReturnValue","ObjectProperty",8,8)){
            slot.failed=true;slot.observed=current;slot.retryAt=now+1000;continue;
        }
        struct Args {int index=0,pad=0;uintptr_t result=0;} args{int(i)};
        g_event(reinterpret_cast<void*>(mesh),reinterpret_cast<void*>(f),&args);
        Id original{};if(!args.result||!Capture(args.result,original)){
            slot.failed=true;slot.observed=current;slot.retryAt=now+1000;continue;
        }
        if(!NativeBaseline(c.character,mesh,args.result,nativeOriginal)){slot.failed=true;slot.observed=current;slot.retryAt=now+1000;
            if(g_log&&g_deferredEvents++<12)g_log("Stoneskin V1.152 deferred slot: unrelated material root or foreign wrapper retained");
            continue;}
        // War's UV emissive masks illuminate the wrong regions on Strife.
        // Body/gear use the mineral base shader without its private glow;
        // cloth retains the native base/Molten shader and its fire.
        const auto source=g_materials[surface==0?BaseBody:(variant?MoltenCloth:BaseCloth)].address;
        const auto mid=Create(mesh,int(i),source);uintptr_t parent=0;int parentAt=-1;Id identity{};
        if(!mid||mid==source||!Field(mid,"Parent","ObjectProperty",8,parentAt)||!Read(mid,parentAt,parent)||parent!=source||!Capture(mid,identity)||
           (surface==0&&!MineralBody(mid,source))){
            // Factory may install its source or MID before validation.
            uintptr_t installed=0;
            if(Override(mesh,int(i),installed)&&((mid&&installed==mid)||installed==source))Set(mesh,int(i),current);
            slot.failed=true;slot.observed=current;slot.retryAt=now+1000;
            continue;
        }
        slot={identity,original,current==0};
        if(g_log&&g_events++<8)g_log("Stoneskin V1.117 visual: mineral body/gear, native fire on cloth only; private MIDs and status-owned restoration; EXPERIMENTAL UV fit");
    }
    if(transition.pending&&transition.world==c.world&&transition.actor.address==c.character&&Alive(transition.actor)){
        bool coated=false;for(const auto& slot:v.slots)coated=coated||Alive(slot.material);
        if(coated&&stone_fx::Ready(variant)){transition.pending=false;const bool ok=stone_fx::Spawn(c,variant);
            if(g_log&&g_events++<16)g_log(ok?"Stoneskin V1.123 launch: native explosion at coating appearance":"Stoneskin V1.123 launch effect unavailable; accepted gesture/materials retained");}
    }
}
void ObserveAt(const local_players::Context& c,uint64_t now){
    if(!g_event||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    cost_probe::Scope measured(cost_probe::StoneObserve);
    auto& sample=g_observations[c.index];
    const auto& previous=sample.owner;
    const bool same=sample.entered&&previous.world==c.world&&previous.character==c.character&&
        previous.pawn==c.pawn&&previous.controller==c.controller&&previous.localPlayer==c.localPlayer&&previous.kind==c.kind;
    if(same&&!sample.force&&now<sample.next&&sample.next-now<=250){++g_skipped;return;}
    sample.owner=c;sample.entered=true;sample.force=false;
    sample.next=now+(g_visual[c.index].actor||g_missing[c.index]?50:250);
    ++g_observed;
    ObserveCurrent(c,now);
    sample.next=now+(g_visual[c.index].actor||g_missing[c.index]?50:250);
}
void Observe(const local_players::Context& c){ObserveAt(c,GetTickCount64());}
void Shutdown(){
    if(native_tps::OnGameThread())for(auto& v:g_visual)Restore(v);
    if(g_log){char line[180]{};sprintf_s(line,"Stoneskin status-class cache hits=%llu misses=%llu capacity=128; identity checked on every hit",static_cast<unsigned long long>(g_statusClassHits),static_cast<unsigned long long>(g_statusClassMisses));g_log(line);}
    g_statusClasses={};g_statusClassNext=0;g_statusClassHits=g_statusClassMisses=0;
    g_resourceRequests={};g_resourceActor={};g_resourceMesh={};
    if(g_log){char line[224]{};sprintf_s(line,"Stoneskin V1.153 work sweeps=%llu skipped=%llu functionSearches=%llu parameterChecks=%llu; gameplay performance requires measurement",
        static_cast<unsigned long long>(g_observed),static_cast<unsigned long long>(g_skipped),
        static_cast<unsigned long long>(g_functionLookups),static_cast<unsigned long long>(g_parameterLookups));g_log(line);}
    g_observations={};g_observed=g_skipped=0;
    g_functions={};g_parameters={};g_nextFunction=g_nextParameter=0;g_functionLookups=g_parameterLookups=0;
    g_event=nullptr;g_fields={};g_transitions[0]=g_transitions[1]={};g_missing[0]=g_missing[1]=false;stone_fx::Shutdown();
}
}
