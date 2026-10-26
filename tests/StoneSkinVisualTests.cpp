#include "../src/StoneSkinVisual.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::stone_visual;
unsigned char entries[64][256]{};uintptr_t names[64]{},chunks[1]{reinterpret_cast<uintptr_t>(names)};
struct Fixture {
 unsigned char actor[0x1100]{},controller[0x400]{},mesh[0x800]{},component[0x180]{},status[0x100]{},molten[0x100]{};
 unsigned char classes[12][0x80]{},fields[6][0x80]{},functions[5][0x110]{},parameters[12][0x80]{};
 unsigned char original[4][0x100]{},mid[4][0x100]{},materials[2][0x100]{},items[32*24]{};
 uintptr_t overrides[4]{},statuses[2]{};
} f;
uintptr_t objectChunks[1]{};local_players::Context owner{};unsigned copies=0,creates=0,sets=0;int id=0;bool thread=true,failCopy=false;
template<class T>void Put(void* p,size_t at,const T& v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void Label(void* p,int i,const char* s){Put(p,0x18,i);strcpy_s(reinterpret_cast<char*>(entries[i]+12),244,s);}
Id Live(void* p){const int n=id++;Put(p,0xC,n);Put(f.items,size_t(n)*24,reinterpret_cast<uintptr_t>(p));Put(f.items,size_t(n)*24+16,n+100);return {reinterpret_cast<uintptr_t>(p),n,n+100};}
void Property(void* p,int label,const char* name,void* kind,int size,int at,void* next=nullptr){Label(p,label,name);Put(p,0x10,reinterpret_cast<uintptr_t>(kind));Put(p,0x34,size);Put(p,0x44,at);Put(p,0x28,reinterpret_cast<uintptr_t>(next));}
void Dispatch(void* object,void* function,void* args){
 if(function==f.functions[0]){Check(object==f.mesh,"only owned mesh GetMaterial");auto* raw=static_cast<unsigned char*>(args);int index=0;std::memcpy(&index,raw,4);Put(raw,8,f.overrides[index]?f.overrides[index]:reinterpret_cast<uintptr_t>(f.original[index]));return;}
 if(function==f.functions[1]){Check(object==f.mesh,"only owned mesh factory");int index=0;std::memcpy(&index,args,4);uintptr_t parent=0;std::memcpy(&parent,static_cast<unsigned char*>(args)+8,8);++creates;Put(f.mid[index],0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(f.mid[index],0x40,parent);Live(f.mid[index]);f.overrides[index]=reinterpret_cast<uintptr_t>(f.mid[index]);Put(args,24,f.overrides[index]);return;}
 if(function==f.functions[2]){Check(object==f.mesh,"only owned mesh SetMaterial");int index=0;uintptr_t value=0;std::memcpy(&index,args,4);std::memcpy(&value,static_cast<unsigned char*>(args)+8,8);f.overrides[index]=value;++sets;return;}
 Check(function==f.functions[3]&&object!=f.materials[0]&&object!=f.materials[1],"parameter copy only on private MID");++copies;
}
void Fresh(){
 f={};g_visual[0]=g_visual[1]={};g_fields={};g_materials[0]=g_materials[1]={};g_event=Dispatch;g_log=nullptr;g_events=0;owner={};id=0;copies=creates=sets=0;thread=true;
 const char* labels[]={"Strife_Blueprint_C","MeshClass","StatusComponent","MayhemStatusEffect_StoneSkin_Base_Blueprint_C","MaterialInstanceDynamic","Function","ObjectProperty","ArrayProperty","IntProperty","NameProperty","BoolProperty","MayhemStatusEffect_Core_FallenDog_Molten_2_Blueprint_C"};
 for(int i=0;i<12;++i)Label(f.classes[i],i+1,labels[i]);
 Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put<unsigned char>(f.actor,0x118,3);Put(f.actor,0x348,reinterpret_cast<uintptr_t>(f.controller));Put(f.controller,0x350,reinterpret_cast<uintptr_t>(f.actor));Put(f.actor,0x380,reinterpret_cast<uintptr_t>(f.mesh));Put(f.actor,0x990,reinterpret_cast<uintptr_t>(f.component));
 Put(f.mesh,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.mesh,0xE8,reinterpret_cast<uintptr_t>(f.actor));Put(f.mesh,0x100,Array{reinterpret_cast<uintptr_t>(f.overrides),4,4});
 Put(f.component,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.component,0xE8,reinterpret_cast<uintptr_t>(f.actor));Put(f.component,0x130,Array{reinterpret_cast<uintptr_t>(f.statuses),1,2});
 Put(f.status,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put(f.status,0x60,reinterpret_cast<uintptr_t>(f.actor));f.statuses[0]=reinterpret_cast<uintptr_t>(f.status);
 Put(f.molten,0x10,reinterpret_cast<uintptr_t>(f.classes[11]));Put(f.molten,0x60,reinterpret_cast<uintptr_t>(f.actor));f.statuses[1]=reinterpret_cast<uintptr_t>(f.molten);
 Property(f.fields[0],14,"m_pStatusEffectComponent",f.classes[6],8,0x990);Put(f.classes[0],0x48,reinterpret_cast<uintptr_t>(f.fields[0]));
 Property(f.fields[1],15,"OverrideMaterials",f.classes[7],16,0x100,f.functions[0]);Put(f.classes[1],0x48,reinterpret_cast<uintptr_t>(f.fields[1]));
 Property(f.fields[2],16,"m_statusEffects",f.classes[7],16,0x130);Put(f.classes[2],0x48,reinterpret_cast<uintptr_t>(f.fields[2]));
 Property(f.fields[3],17,"m_pInstigatorActor",f.classes[6],8,0x60);Put(f.classes[3],0x48,reinterpret_cast<uintptr_t>(f.fields[3]));Put(f.classes[11],0x48,reinterpret_cast<uintptr_t>(f.fields[3]));
 Property(f.fields[4],18,"Parent",f.classes[6],8,0x40,f.functions[3]);Put(f.classes[4],0x48,reinterpret_cast<uintptr_t>(f.fields[4]));
 const char* fn[]={"GetMaterial","CreateDynamicMaterialInstance","SetMaterial","K2_CopyMaterialInstanceParameters"};const uintptr_t rv[]={0x1DC9CB0,0x1DC92E0,0x1DCCC80,0x1D96D80};
 for(int i=0;i<4;++i){Label(f.functions[i],20+i,fn[i]);Put(f.functions[i],0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Put(f.functions[i],0xC0,g_base+rv[i]);if(i<2)Put(f.functions[i],0x28,reinterpret_cast<uintptr_t>(f.functions[i+1]));}
 int n=0;auto parameter=[&](int label,const char* name,int kind,int bytes,int at){auto* p=f.parameters[n++];Property(p,label,name,f.classes[kind],bytes,at);return p;};
 auto chain=[&](int fn,std::initializer_list<unsigned char*> list){unsigned char* prev=nullptr;for(auto* p:list){if(prev)Put(prev,0x28,reinterpret_cast<uintptr_t>(p));else Put(f.functions[fn],0x48,reinterpret_cast<uintptr_t>(p));prev=p;}};
 chain(0,{parameter(25,"ElementIndex",8,4,0),parameter(26,"ReturnValue",6,8,8)});
 chain(1,{parameter(25,"ElementIndex",8,4,0),parameter(27,"SourceMaterial",6,8,8),parameter(28,"OptionalName",9,8,16),parameter(26,"ReturnValue",6,8,24)});
 chain(2,{parameter(25,"ElementIndex",8,4,0),parameter(29,"Material",6,8,8)});
 chain(3,{parameter(30,"Source",6,8,0),parameter(31,"bQuickParametersOnly",10,1,8)});
 Live(f.actor);Live(f.mesh);for(auto& m:f.original)Live(m);for(int i=0;i<2;++i)g_materials[i]=Live(f.materials[i]);
 owner.index=0;owner.world=1;owner.localPlayer=2;owner.character=owner.pawn=reinterpret_cast<uintptr_t>(f.actor);owner.controller=reinterpret_cast<uintptr_t>(f.controller);
}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));for(int i=0;i<64;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(image.data(),0x3959694,32);
 Fresh();Observe(owner);Check(creates==4&&copies==4&&g_visual[0].variant==0,"four private base body materials");Observe(owner);Check(creates==4&&copies==4,"no repeated creation/copy while unchanged");
 Put(f.component,0x138,0);Observe(owner);Check(sets==4&&!g_visual[0].actor,"status removal restores inherited slots");for(auto p:f.overrides)Check(!p,"original inheritance restored");
 Fresh();f.overrides[1]=reinterpret_cast<uintptr_t>(f.original[1]);Observe(owner);f.overrides[2]=reinterpret_cast<uintptr_t>(f.original[2]);Put(f.component,0x138,0);Observe(owner);Check(f.overrides[1]==reinterpret_cast<uintptr_t>(f.original[1])&&f.overrides[2]==reinterpret_cast<uintptr_t>(f.original[2])&&sets==3,"explicit material restored; native replacement wins");
 Fresh();Put(f.component,0x138,2);Observe(owner);Check(g_visual[0].variant==1,"active Molten core chooses native variant");
 owner.kind=local_players::PoseKind::Horse;Observe(owner);Check(!g_visual[0].actor&&sets==4,"transformation restores old owned body only");
 for(int bad=0;bad<5;++bad){Fresh();if(bad==0)thread=false;if(bad==1)owner.index=2;if(bad==2)owner.localPlayer=0;if(bad==3)Put<unsigned char>(f.actor,0x118,2);if(bad==4)Label(f.classes[0],1,"War_Blueprint_C");Observe(owner);Check(!creates&&!copies&&!sets,"War, remote, off-thread and invalid ownership untouched");}
 Fresh();Put(f.functions[3],0xC0,uintptr_t(0));Observe(owner);Check(creates==4&&!copies&&sets==4&&!g_visual[0].slots[0].material,"unknown parameter API rolls back factory-installed MIDs");
 Fresh();Observe(owner);Put(f.items,size_t(g_visual[0].mesh.index)*24+16,999);Put(f.component,0x138,0);Observe(owner);Check(!sets&&!g_visual[0].actor,"expired mesh identity never receives restore");
 Fresh();owner.index=1;Observe(owner);Check(g_visual[1].actor&&!g_visual[0].actor,"P2 has independent visual state in fixture");Shutdown();Check(sets==4&&!g_event,"game-thread shutdown restores owned materials");
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
int main(){try{Run();std::cout<<"Stoneskin owned material lifecycle, restoration, API guards and isolation passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
