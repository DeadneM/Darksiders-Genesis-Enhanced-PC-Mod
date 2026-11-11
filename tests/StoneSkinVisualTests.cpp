#include "../src/StoneSkinVisual.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::stone_visual;
unsigned char entries[64][256]{};uintptr_t names[64]{},chunks[1]{reinterpret_cast<uintptr_t>(names)};
struct Fixture {
 unsigned char actor[0x1100]{},controller[0x400]{},mesh[0x800]{},component[0x180]{},status[0x100]{},molten[0x100]{};
 unsigned char classes[14][0x80]{},fields[11][0x80]{},functions[5][0x110]{},parameters[16][0x80]{};
 unsigned char original[4][0x100]{},mid[4][0x100]{},materials[2][0x100]{},items[128*24]{};
 unsigned char nativeMids[4][0x100]{};
 unsigned char asset[0x140]{},skinSlots[160]{},scalars[72]{};
 uintptr_t overrides[4]{},statuses[2]{},retainedMaterials[4]{};
} f;
uintptr_t objectChunks[1]{};dg::local_players::Context owner{};unsigned copies=0,creates=0,sets=0,scalarCalls=0;int id=0;bool thread=true,failFactory=false;
unsigned fx[3]{},launchAttempts=0,launchFailures=0;bool fxReady=true,paused=false,cinema=false,radial=false,recastInSpawn=false;bool missingFx[3]{};
template<class T>void Put(void* p,size_t at,const T& v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void Label(void* p,int i,const char* s){Put(p,0x18,i);strcpy_s(reinterpret_cast<char*>(entries[i]+12),244,s);}
Id Live(void* p){const int n=id++;Put(p,0xC,n);Put(f.items,size_t(n)*24,reinterpret_cast<uintptr_t>(p));Put(f.items,size_t(n)*24+16,n+100);return {reinterpret_cast<uintptr_t>(p),n,n+100};}
void Property(void* p,int label,const char* name,void* kind,int size,int at,void* next=nullptr){Label(p,label,name);Put(p,0x10,reinterpret_cast<uintptr_t>(kind));Put(p,0x34,size);Put(p,0x44,at);Put(p,0x28,reinterpret_cast<uintptr_t>(next));}
void Dispatch(void* object,void* function,void* args){
 if(function==f.functions[0]){Check(object==f.mesh,"only owned mesh GetMaterial");auto* raw=static_cast<unsigned char*>(args);int index=0;std::memcpy(&index,raw,4);Put(raw,8,f.overrides[index]?f.overrides[index]:reinterpret_cast<uintptr_t>(f.original[index]));return;}
 if(function==f.functions[1]){Check(object==f.mesh,"only owned mesh factory");int index=0;std::memcpy(&index,args,4);uintptr_t parent=0;std::memcpy(&parent,static_cast<unsigned char*>(args)+8,8);++creates;if(failFactory){f.overrides[index]=parent;return;}Put(f.mid[index],0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(f.mid[index],0x40,parent);Live(f.mid[index]);f.overrides[index]=reinterpret_cast<uintptr_t>(f.mid[index]);Put(args,24,f.overrides[index]);return;}
 if(function==f.functions[2]){Check(object==f.mesh,"only owned mesh SetMaterial");int index=0;uintptr_t value=0;std::memcpy(&index,args,4);std::memcpy(&value,static_cast<unsigned char*>(args)+8,8);f.overrides[index]=value;++sets;return;}
 if(function==f.functions[4]){bool own=false;for(auto& m:f.mid)own=own||object==m;Check(own,"body emission only on private MID");float value=1;std::memcpy(&value,static_cast<unsigned char*>(args)+8,4);Check(value==0,"body emission disabled without changing the source texture");++scalarCalls;return;}
 Check(function==f.functions[3]&&object!=f.materials[0]&&object!=f.materials[1],"parameter copy only on private MID");++copies;
}
uint64_t observationTime=1000;
void Step(const dg::local_players::Context& c){observationTime+=50;ObserveAt(c,observationTime);}
void Fresh(){
 launchAttempts=launchFailures=0;paused=cinema=radial=recastInSpawn=false;
 g_resourceRequests={};g_resourceActor={};g_resourceMesh={};dg::selected_particle::sharedContext={};for(auto& missing:missingFx)missing=false;
 g_statusClasses={};g_statusClassNext=0;g_statusClassHits=g_statusClassMisses=0;
 g_observations={};g_observed=g_skipped=0;g_functions={};g_parameters={};g_nextFunction=g_nextParameter=0;g_functionLookups=g_parameterLookups=0;observationTime=1000;
 f={};g_visual[0]=g_visual[1]={};g_transitions[0]=g_transitions[1]={};g_missing[0]=g_missing[1]=false;g_fields={};g_materials={};g_event=Dispatch;g_log=nullptr;g_events=g_deferredEvents=0;owner={};id=0;copies=creates=sets=scalarCalls=0;thread=true;failFactory=false;for(auto& n:fx)n=0;fxReady=true;
 const char* labels[]={"Strife_Blueprint_C","MeshClass","StatusComponent","MayhemStatusEffect_StoneSkin_Base_Blueprint_C","MaterialInstanceDynamic","Function","ObjectProperty","ArrayProperty","IntProperty","NameProperty","BoolProperty","MayhemStatusEffect_Core_FallenDog_Molten_2_Blueprint_C"};
 for(int i=0;i<12;++i)Label(f.classes[i],i+1,labels[i]);
 Label(f.classes[12],44,"FloatProperty");Label(f.classes[13],45,"SkeletalMesh");Label(f.asset,40,"Strife");Put(f.asset,0x10,reinterpret_cast<uintptr_t>(f.classes[13]));Put(f.mesh,0x5A0,reinterpret_cast<uintptr_t>(f.asset));
 Label(f.fields[8],41,"Strife_Body");Label(f.fields[9],42,"Strife_Gear");Label(f.functions[4],43,"Strife_Cloth");
 for(int i=0;i<4;++i){dg::foreign_wrath::Name n{i==0?41:i==1?42:43,0};Put(f.skinSlots,size_t(i)*40,reinterpret_cast<uintptr_t>(f.original[i]));Put(f.skinSlots,size_t(i)*40+8,n);}
 Put(f.asset,0xA0,Array{reinterpret_cast<uintptr_t>(f.skinSlots),4,4});
 Property(f.fields[5],46,"SkeletalMesh",f.classes[6],8,0x5A0,f.fields[1]);Property(f.fields[6],47,"Materials",f.classes[7],16,0xA0);Property(f.fields[7],50,"ScalarParameterValues",f.classes[7],16,0x60,f.functions[3]);
 Put(f.classes[13],0x48,reinterpret_cast<uintptr_t>(f.fields[6]));
 Label(f.fields[8],48,"Emissive_Power_Base");Label(f.fields[9],49,"Emissive_Power_Scroll");
 for(int i=0;i<2;++i){Put(f.scalars,size_t(i)*36,dg::foreign_wrath::Name{48+i,0});Put(f.scalars,size_t(i)*36+8,2);Put(f.scalars,size_t(i)*36+12,-1);Put(f.scalars,size_t(i)*36+16,10.f);Put(f.materials[i],0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(f.materials[i],0x60,Array{reinterpret_cast<uintptr_t>(f.scalars),2,2});}
 Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put<unsigned char>(f.actor,0x118,3);Put(f.actor,0x348,reinterpret_cast<uintptr_t>(f.controller));Put(f.controller,0x350,reinterpret_cast<uintptr_t>(f.actor));Put(f.actor,0x380,reinterpret_cast<uintptr_t>(f.mesh));Put(f.actor,0x990,reinterpret_cast<uintptr_t>(f.component));
 Put(f.mesh,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.mesh,0xE8,reinterpret_cast<uintptr_t>(f.actor));Put(f.mesh,0x100,Array{reinterpret_cast<uintptr_t>(f.overrides),4,4});
 Put(f.component,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.component,0xE8,reinterpret_cast<uintptr_t>(f.actor));Put(f.component,0x130,Array{reinterpret_cast<uintptr_t>(f.statuses),1,2});
 Put(f.status,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put(f.status,0x60,reinterpret_cast<uintptr_t>(f.actor));f.statuses[0]=reinterpret_cast<uintptr_t>(f.status);
 Put(f.molten,0x10,reinterpret_cast<uintptr_t>(f.classes[11]));Put(f.molten,0x60,reinterpret_cast<uintptr_t>(f.actor));f.statuses[1]=reinterpret_cast<uintptr_t>(f.molten);
 Property(f.fields[0],14,"m_pStatusEffectComponent",f.classes[6],8,0x990);Put(f.classes[0],0x48,reinterpret_cast<uintptr_t>(f.fields[0]));
 Property(f.fields[10],55,"m_dynMaterials",f.classes[7],16,0xA10);Put(f.fields[0],0x28,reinterpret_cast<uintptr_t>(f.fields[10]));Put(f.actor,0xA10,Array{reinterpret_cast<uintptr_t>(f.retainedMaterials),4,4});
 Property(f.fields[1],15,"OverrideMaterials",f.classes[7],16,0x100,f.functions[0]);Put(f.classes[1],0x48,reinterpret_cast<uintptr_t>(f.fields[5]));
 Property(f.fields[2],16,"m_statusEffects",f.classes[7],16,0x130);Put(f.classes[2],0x48,reinterpret_cast<uintptr_t>(f.fields[2]));
 Property(f.fields[3],17,"m_pInstigatorActor",f.classes[6],8,0x60);Put(f.classes[3],0x48,reinterpret_cast<uintptr_t>(f.fields[3]));Put(f.classes[11],0x48,reinterpret_cast<uintptr_t>(f.fields[3]));
 Property(f.fields[4],18,"Parent",f.classes[6],8,0x40,f.fields[7]);Put(f.classes[4],0x48,reinterpret_cast<uintptr_t>(f.fields[4]));
 const char* fn[]={"GetMaterial","CreateDynamicMaterialInstance","SetMaterial","K2_CopyMaterialInstanceParameters","SetScalarParameterValue"};const uintptr_t rv[]={0x1DC9CB0,0x1DC92E0,0x1DCCC80,0x1D96D80,0x1D97290};
 for(int i=0;i<5;++i){Label(f.functions[i],20+i,fn[i]);Put(f.functions[i],0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Put(f.functions[i],0xC0,g_base+rv[i]);if(i<2||i==3)Put(f.functions[i],0x28,reinterpret_cast<uintptr_t>(f.functions[i+1]));}
 int n=0;auto parameter=[&](int label,const char* name,int kind,int bytes,int at){auto* p=f.parameters[n++];Property(p,label,name,f.classes[kind],bytes,at);return p;};
 auto chain=[&](int fn,std::initializer_list<unsigned char*> list){unsigned char* prev=nullptr;for(auto* p:list){if(prev)Put(prev,0x28,reinterpret_cast<uintptr_t>(p));else Put(f.functions[fn],0x48,reinterpret_cast<uintptr_t>(p));prev=p;}};
 chain(0,{parameter(25,"ElementIndex",8,4,0),parameter(26,"ReturnValue",6,8,8)});
 chain(1,{parameter(25,"ElementIndex",8,4,0),parameter(27,"SourceMaterial",6,8,8),parameter(28,"OptionalName",9,8,16),parameter(26,"ReturnValue",6,8,24)});
 chain(2,{parameter(25,"ElementIndex",8,4,0),parameter(29,"Material",6,8,8)});
 chain(3,{parameter(30,"Source",6,8,0),parameter(31,"bQuickParametersOnly",10,1,8)});
 chain(4,{parameter(51,"ParameterName",9,8,0),parameter(52,"Value",12,4,8)});
 Live(f.actor);Live(f.mesh);Live(f.asset);for(auto& m:f.original)Live(m);g_materials[BaseBody]=Live(f.materials[0]);g_materials[MoltenCloth]=Live(f.materials[1]);
 owner.index=0;owner.world=1;owner.localPlayer=2;owner.character=owner.pawn=reinterpret_cast<uintptr_t>(f.actor);owner.controller=reinterpret_cast<uintptr_t>(f.controller);
}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));for(int i=0;i<64;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(image.data(),0x3959694,128);
 Fresh();const auto meshClass=Live(f.classes[1]);const auto getFunction=Live(f.functions[0]);
 for(int i=0;i<100;++i){
  Check(Function(reinterpret_cast<uintptr_t>(f.mesh),"GetMaterial",0x1DC9CB0)==reinterpret_cast<uintptr_t>(f.functions[0]),"verified native function is reused");
  Check(Parameter(reinterpret_cast<uintptr_t>(f.functions[0]),"ElementIndex","IntProperty",4,0),"verified parameter schema is reused");
 }
 Check(g_functionLookups==1&&g_parameterLookups==1,"one hundred calls require one function search and one parameter lookup");
 Check(!Function(reinterpret_cast<uintptr_t>(f.mesh),"WrongName",0x1DC9CB0),"same native address cannot bypass the requested function name");
 Put(f.functions[0],0xC0,uintptr_t(0));Check(!Function(reinterpret_cast<uintptr_t>(f.mesh),"GetMaterial",0x1DC9CB0),"cached function still checks its live native entry");
 Put(f.items,size_t(getFunction.index)*24+16,getFunction.serial+1);Put(f.parameters[0],0x44,4);
 Check(!Parameter(reinterpret_cast<uintptr_t>(f.functions[0]),"ElementIndex","IntProperty",4,0),"reused function identity cannot reuse an old parameter schema");
 Check(bool(meshClass),"cache fixture class has a live identity");
 Fresh();for(int i=0;i<1000;++i)ObserveAt(owner,1000);
 Check(g_observed==1&&g_skipped==999&&creates==4,"repeated camera callbacks share one material/status sweep");
 ObserveAt(owner,1049);Check(g_observed==1,"active visual checks wait fifty milliseconds");
 ObserveAt(owner,1050);Check(g_observed==2&&creates==4,"active visual check does not recreate its materials");
 Put(f.component,0x138,0);ObserveAt(owner,1100);Check(sets==4,"known expiry restores immediately at the next active check");
 ObserveAt(owner,1349);Check(g_observed==3,"inactive visual checks use a slower cadence");
 ObserveAt(owner,1350);Check(g_observed==4,"inactive visual still detects native status changes");
 owner.world=99;ObserveAt(owner,1351);Check(g_observed==5,"world change bypasses the cadence");
 Fresh();ObserveAt(owner,1000);Put(f.component,0x138,129);ObserveAt(owner,1050);
 Check(!sets&&bool(g_visual[0].actor)&&!fx[2],"incomplete status data preserves an existing coating without fake expiry");
 Put(f.component,0x138,0);ObserveAt(owner,1100);Check(sets==4&&fx[2]==1,"confirmed expiry remains authoritative after an inconclusive read");
 Fresh();failFactory=true;ObserveAt(owner,1000);failFactory=false;ObserveAt(owner,1050);
 Check(creates==4,"temporary factory failure is not retried every callback");
 ObserveAt(owner,2050);Check(creates==8&&bool(g_visual[0].slots[0].material),"same unchanged material can recover after the bounded retry interval");
 Fresh();failFactory=true;ObserveAt(owner,1000);failFactory=false;Cast(owner);ObserveAt(owner,1001);
 Check(creates==8,"a new cast retries a previous failure immediately");
 Fresh();auto* delayed=f.nativeMids[0];
 Put(delayed,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(delayed,0x20,reinterpret_cast<uintptr_t>(f.actor));
 Put(delayed,0x40,reinterpret_cast<uintptr_t>(f.original[0]));Live(delayed);f.overrides[0]=reinterpret_cast<uintptr_t>(delayed);
 ObserveAt(owner,1000);Check(creates==3,"unretained native wrapper remains untouched");
 f.retainedMaterials[0]=reinterpret_cast<uintptr_t>(delayed);ObserveAt(owner,2050);
 Check(creates==4&&bool(g_visual[0].slots[0].material),"a later native strong reference unblocks the same wrapper without a pointer change");
 Put(f.component,0x138,0);ObserveAt(owner,2100);Check(f.overrides[0]==reinterpret_cast<uintptr_t>(delayed),"recovered wrapper is restored exactly on expiry");
 Fresh();Put(f.component,0x138,2);Step(owner);
 Label(f.classes[11],12,"UnresolvedStatus");Put(f.classes[11],0x40,reinterpret_cast<uintptr_t>(f.classes[11]));Step(owner);
 Check(g_visual[0].variant==1&&!sets,"incomplete variant classification cannot silently replace Molten by base");
 Fresh();Label(f.fields[8],41,"strife_body");Label(f.functions[4],43,"STRIFE_CLOTH");Step(owner);
 Check(creates==4,"material slot identifiers follow native case-insensitive semantics");
 Fresh();const auto statusClass=Live(f.classes[3]);bool classKnown=false;
 Check(StatusKind(statusClass.address,classKnown)==1&&classKnown&&g_statusClassMisses==1,"Stoneskin class classified once");
 Check(StatusKind(statusClass.address,classKnown)==1&&classKnown&&g_statusClassMisses==1&&g_statusClassHits==1,"repeated class hierarchy work avoided");
 Label(f.classes[3],54,"UnrelatedStatusClass");Put(f.items,size_t(statusClass.index)*24+16,statusClass.serial+1);
 Check(StatusKind(statusClass.address,classKnown)==0&&classKnown&&g_statusClassMisses==2,"reused class address invalidates cached kind");
 for(unsigned i=0;i<g_statusClasses.size();++i)g_statusClasses[i]={statusClass,1};g_statusClassNext=g_statusClasses.size()-1;
 const auto moltenClass=Live(f.classes[11]);
 Check(StatusKind(moltenClass.address,classKnown)==2&&classKnown&&g_statusClassNext==0&&g_statusClasses.back().identity==moltenClass,"full bounded cache replaces an entry rather than permanently missing");
 Fresh();Put(f.classes[3],0x40,reinterpret_cast<uintptr_t>(f.classes[3]));
 StatusKind(reinterpret_cast<uintptr_t>(f.classes[3]),classKnown);Check(!classKnown&&!g_statusClasses[0].identity,"truncated/cyclic unknown hierarchy is never cached");
 Fresh();const auto classId=Live(f.classes[0]);int cachedOffset=-1;
 Check(Field(owner.character,"m_pStatusEffectComponent","ObjectProperty",8,cachedOffset)&&cachedOffset==0x990&&g_fields[0].cls==classId,"known reflected field caches complete class identity");
 Put(f.fields[0],0x44,0x998);Check(Field(owner.character,"m_pStatusEffectComponent","ObjectProperty",8,cachedOffset)&&cachedOffset==0x990,"live matching class reuses its verified immutable schema");
 Put(f.items,size_t(classId.index)*24+16,classId.serial+1);
 Check(Field(owner.character,"m_pStatusEffectComponent","ObjectProperty",8,cachedOffset)&&cachedOffset==0x998&&g_fields[0].cls.serial==classId.serial+1,"reused class address cannot reuse an old cached offset");
 Fresh();Label(f.fields[8],53,"Strife_Abyssal_Body");Label(f.fields[9],54,"Strife_Abyssal_Cloth");Label(f.asset,40,"Strife_Abyssal");
 Put(f.skinSlots,8,dg::foreign_wrath::Name{53,0});Put(f.skinSlots,48,dg::foreign_wrath::Name{54,0});
 Put(f.asset,0xA0,Array{reinterpret_cast<uintptr_t>(f.skinSlots),2,2});Cast(owner);Step(owner);
 uintptr_t abyssBody=0,abyssCloth=0;std::memcpy(&abyssBody,f.mid[0]+0x40,8);std::memcpy(&abyssCloth,f.mid[1]+0x40,8);
 Check(creates==2&&abyssBody==reinterpret_cast<uintptr_t>(f.materials[0])&&abyssCloth==reinterpret_cast<uintptr_t>(f.materials[0]),"Abyssal body and cloth keep independent private MIDs on the shared opaque base, with emission suppressed on body only");
 Put(f.component,0x138,0);Step(owner);Check(sets==2&&!f.overrides[0]&&!f.overrides[1]&&fx[2]==1,"Abyssal expiration restores armor inheritance without changing unknown slots");
 auto parents=[](int variant){for(int i=0;i<4;++i){uintptr_t parent=0;std::memcpy(&parent,f.mid[i]+0x40,8);Check(parent==reinterpret_cast<uintptr_t>(f.materials[(i<2||variant==0?0:1)]),"body/gear mineral A and opaque A cloth retain native fire switches; no texture setter");}};
 Fresh();const auto materials=std::vector<unsigned char>(reinterpret_cast<unsigned char*>(f.materials),reinterpret_cast<unsigned char*>(f.materials)+sizeof(f.materials));Step(owner);Check(creates==4&&!copies&&scalarCalls==4,"private mineral body/gear suppress only their two emissive parameters; cloth retains its native shader");parents(0);Check(!std::memcmp(materials.data(),f.materials,sizeof(f.materials)),"source shader/material parameters remain byte-identical");Step(owner);Check(creates==4,"no per-frame recreation");Put(f.component,0x138,0);Step(owner);Check(sets==4&&!g_visual[0].actor,"accepted status expiration and native inheritance restoration retained");for(auto x:f.overrides)Check(!x,"native materials restored");
 Fresh();Put(f.component,0x138,2);Step(owner);parents(1);Check(g_visual[0].variant==1&&scalarCalls==4&&!copies,"Molten cloth remains fiery while body and gear retain mineral parents");
 Fresh();Put(f.component,0x138,2);Check(AssetsReady(),"only the three used material sources are discovery dependencies");Step(owner);parents(1);Check(creates==4&&!NeedsAssetScan(owner),"opaque Molten cloth and base mineral body are the only material dependencies");
 Fresh();missingFx[1]=true;Step(owner);Check(creates==4&&!NeedsAssetScan(owner),"unused Molten explosion does not block or request loading for a base coating");
 Fresh();Put(f.component,0x138,2);missingFx[0]=true;Step(owner);Check(creates==4&&!NeedsAssetScan(owner),"unused base explosion does not request loading for a Molten coating");
 Fresh();Put(f.component,0x138,2);const auto body=g_materials[BaseBody];g_materials[BaseBody]={};Step(owner);
 Check(!creates&&NeedsAssetScan(owner)&&!g_visual[0].actor,"missing actual base body defers the factory and requests rediscovery, even when Molten body exists");
 auto other=owner;other.index=1;Check(!NeedsAssetScan(other),"missing P1 coating never drives retries for P2 or a departed slot");
 g_materials[BaseBody]=body;Step(owner);parents(1);Check(creates==4&&!NeedsAssetScan(owner),"late base material applies during the same status without a cached factory refusal");
 Fresh();g_materials[BaseBody]={};Step(owner);Check(!creates&&NeedsAssetScan(owner),"missing shared opaque base material also requests bounded rediscovery");owner.kind=dg::local_players::PoseKind::Horse;Step(owner);Check(!NeedsAssetScan(owner),"transformation cancels missing-coating demand");
 Fresh();f.overrides[1]=reinterpret_cast<uintptr_t>(f.original[1]);Step(owner);f.overrides[0]=reinterpret_cast<uintptr_t>(f.original[0]);Put(f.component,0x138,0);Step(owner);Check(sets==3&&f.overrides[1]==reinterpret_cast<uintptr_t>(f.original[1]),"explicit original restored and native replacement wins");
 Fresh();f.overrides[0]=reinterpret_cast<uintptr_t>(f.original[2]);Step(owner);Check(creates==3&&f.overrides[0]==reinterpret_cast<uintptr_t>(f.original[2]),"unsupported overrides never lose their last strong reference");
    Fresh();
    for(int i=0;i<4;++i){auto* native=f.nativeMids[i];Put(native,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(native,0x20,reinterpret_cast<uintptr_t>(f.actor));Put(native,0x40,reinterpret_cast<uintptr_t>(f.original[i]));Live(native);f.overrides[i]=f.retainedMaterials[i]=reinterpret_cast<uintptr_t>(native);}
    Step(owner);Check(creates==4,"native actor-owned MID wrappers of the equipped skin accept coating");
    Put(f.component,0x138,0);Step(owner);
    for(int i=0;i<4;++i)Check(f.overrides[i]==reinterpret_cast<uintptr_t>(f.nativeMids[i]),"expiration restores exact native wrappers and their parameters, not asset defaults");
    Fresh();auto* native=f.nativeMids[0];Put(native,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(native,0x20,reinterpret_cast<uintptr_t>(f.controller));Put(native,0x40,reinterpret_cast<uintptr_t>(f.original[0]));Live(native);f.overrides[0]=f.retainedMaterials[0]=reinterpret_cast<uintptr_t>(native);
    Step(owner);Check(creates==3&&f.overrides[0]==reinterpret_cast<uintptr_t>(native),"same material root with a foreign owner remains untouched");
    Fresh();native=f.nativeMids[0];Put(native,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(native,0x20,reinterpret_cast<uintptr_t>(f.actor));Put(native,0x40,reinterpret_cast<uintptr_t>(f.original[0]));Live(native);f.overrides[0]=reinterpret_cast<uintptr_t>(native);
    Step(owner);Check(creates==3&&f.overrides[0]==reinterpret_cast<uintptr_t>(native),"Outer alone never substitutes for a proven strong native baseline reference");
    for(int invalid=0;invalid<3;++invalid){
        Fresh();native=f.nativeMids[0];Put(native,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(native,0x20,reinterpret_cast<uintptr_t>(f.actor));Live(native);
        f.overrides[0]=f.retainedMaterials[0]=reinterpret_cast<uintptr_t>(native);
        const uintptr_t parent=invalid==0?reinterpret_cast<uintptr_t>(native):invalid==1?reinterpret_cast<uintptr_t>(f.original[1]):0;
        Put(native,0x40,parent);Step(owner);
        Check(creates==3&&f.overrides[0]==reinterpret_cast<uintptr_t>(native),"retained wrappers with a cycle, another skin-slot root or a missing parent remain untouched");
    }
 Fresh();Put(f.functions[1],0xC0,uintptr_t(0));Step(owner);Check(!creates&&!sets,"unrecognized material factory fails closed");Step(owner);Check(!creates&&!sets,"failed factory contract is not retried per frame");
 Fresh();failFactory=true;Step(owner);Check(creates==4&&sets==4,"failed dynamic factory source assignments are rolled back");for(auto x:f.overrides)Check(!x,"failure preserves original native inheritance");Step(owner);Check(creates==4&&sets==4,"failed creation is cached after rollback");
 Fresh();Put(f.skinSlots,8,dg::foreign_wrath::Name{43,0});Step(owner);uintptr_t parent=0;std::memcpy(&parent,f.mid[0]+0x40,8);Check(parent==reinterpret_cast<uintptr_t>(f.materials[0])&&scalarCalls==2,"cloth role follows material slot name and retains native emission, not mineral body suppression");
 Fresh();Step(owner);owner.kind=dg::local_players::PoseKind::Horse;Step(owner);Check(sets==4&&!g_visual[0].actor,"transformation restores only own visual state");
 for(int bad=0;bad<5;++bad){Fresh();if(bad==0)thread=false;if(bad==1)owner.index=2;if(bad==2)owner.localPlayer=0;if(bad==3)Put<unsigned char>(f.actor,0x118,2);if(bad==4)Label(f.classes[0],1,"War_Blueprint_C");Step(owner);Check(!creates&&!sets,"War, off-thread, remote and invalid owners untouched");}
 Fresh();owner.index=1;Step(owner);Check(g_visual[1].actor&&!g_visual[0].actor,"independent per-player observation");Shutdown();Check(sets==4&&!g_event,"game-thread shutdown restores originals");
 Fresh();Cast(owner);Step(owner);Step(owner);Check(fx[0]==1&&!fx[1]&&!fx[2]&&creates==4,"one explosion exactly when accepted coating appears, no per-frame burst or material recreation");
 Cast(owner);Step(owner);Check(fx[0]==2&&creates==4,"recast can emit one explosion without recreating accepted material state");
 Put(f.component,0x138,0);Step(owner);Step(owner);Check(fx[2]==1&&sets==4,"one crumble at complete status expiry, not cast animation end");
 Fresh();Put(f.component,0x138,2);Cast(owner);Step(owner);Check(fx[1]==1&&!fx[0],"Molten status chooses Molten launch");Put(f.component,0x138,1);Step(owner);Check(!fx[2],"Molten-to-base transition is not full expiration");
 Fresh();fxReady=false;Cast(owner);Step(owner);Check(!fx[0]&&g_transitions[0].pending&&NeedsAssetScan(owner),"late FX request rediscovery and defer explosion without replaying cast or altering materials");fxReady=true;Step(owner);Check(fx[0]==1&&!g_transitions[0].pending&&!NeedsAssetScan(owner),"bounded shared discovery completing emits deferred launch once and stops retries");
 Fresh();launchFailures=1;Cast(owner);Step(owner);Step(owner);Step(owner);
 Check(launchAttempts==1&&!fx[0]&&g_transitions[0].pending,"refused cosmetic creation waits for its retry interval");
 Step(owner);Step(owner);Check(launchAttempts==2&&fx[0]==1&&!g_transitions[0].pending&&creates==4,"one later successful explosion without recreating coating");
 Fresh();launchFailures=10;Cast(owner);for(int i=0;i<30;++i)Step(owner);
 Check(launchAttempts==3&&!fx[0]&&!g_transitions[0].pending&&creates==4,"three refused requests exhaust the budget without touching native damage or materials");
 Fresh();launchFailures=1;Cast(owner);Step(owner);Put(f.component,0x138,0);for(int i=0;i<6;++i)Step(owner);
 Check(launchAttempts==1&&!fx[0]&&!g_transitions[0].pending,"expiry cancels a refused burst instead of replaying it after removal");
 Fresh();launchFailures=1;Cast(owner);Step(owner);ObserveAt(owner,observationTime+1000);
 Check(launchAttempts==1&&!fx[0]&&!g_transitions[0].pending,"long service gap expires a pending retry");
 for(int gate=0;gate<3;++gate){Fresh();launchFailures=1;Cast(owner);Step(owner);
  if(gate==0)paused=true;if(gate==1)cinema=true;if(gate==2)radial=true;
  for(int i=0;i<4;++i)Step(owner);Check(launchAttempts==1,"native pause, cinema and radial gates suspend cosmetic retries");
  paused=cinema=radial=false;Step(owner);Check(launchAttempts==2&&fx[0]==1,"release inside the short window permits one successful retry");}
 for(int change=0;change<2;++change){Fresh();launchFailures=1;Cast(owner);Step(owner);
  if(change==0)owner.kind=dg::local_players::PoseKind::Horse;else owner.world=99;
  for(int i=0;i<5;++i)Step(owner);Check(launchAttempts==1,"form or world loss cannot retry an old cast");}
 Fresh();recastInSpawn=true;Cast(owner);Step(owner);Step(owner);
 Check(launchAttempts==2&&fx[0]==1&&!g_transitions[0].pending,"reentrant recast keeps its own ticket, not the previous refusal");
 for(int change=0;change<4;++change){Fresh();Cast(owner);Step(owner);
  if(change==0)owner.kind=dg::local_players::PoseKind::Horse;if(change==1)owner.world=99;if(change==2)Put(f.component,0x138,129);if(change==3){Shutdown();Check(!fx[2],"shutdown never emits expiry");continue;}
  Step(owner);Check(!fx[2],"transformation/world loss/unknown status read does not fake expiration");}
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return cinema;}bool RadialActive(unsigned){return radial;}}
namespace dg::stone_fx {
void Initialize(uintptr_t,Event){}void ObserveObject(uintptr_t){}bool AssetsReady(){return fxReady&&!missingFx[0]&&!missingFx[1]&&!missingFx[2];}bool Ready(int kind){return fxReady&&!missingFx[kind];}bool TemplateReady(int kind){return Ready(kind);}bool InvalidateAssets(){return false;}void Shutdown(){}
bool Spawn(const local_players::Context& c,int kind){Check(c.character==owner.character&&kind>=0&&kind<3,"cosmetic edge uses current owner and exact known effect");
 if(kind<2){++launchAttempts;if(recastInSpawn){recastInSpawn=false;dg::stone_visual::Cast(c);return false;}if(launchFailures){--launchFailures;return false;}}
 ++fx[kind];return fxReady;}
}
int main(){try{Run();std::cout<<"Stoneskin owned material lifecycle, restoration, API guards and isolation passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
