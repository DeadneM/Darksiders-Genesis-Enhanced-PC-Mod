#include "../src/ShieldVisualFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>
namespace {
using namespace dg::shield_visual;
struct Fixture {
 unsigned char actor[2][0x1100]{},mesh[2][0x800]{},controller[2][0x400]{},component[2][0x200]{},status[2][0x100]{},particles[34][0x800]{};
 unsigned char classes[12][0x100]{},properties[6][0x100]{},functions[2][0x100]{},source[0x100]{},target[0x100]{},package[0x100]{},world[0x100]{},otherWorld[0x100]{};
 uintptr_t statuses[2][1]{},parents[1]{},names[64]{},nameChunk[1]{},objectChunk[1]{};unsigned char entries[64][512]{},items[96*24]{};
} f;
bool thread=true,paused=false,cinematic=false,missing=false,failLoad=false;unsigned visibleCalls=0,objectIndex=0,spawns=0,destroys=0,loads=0;dg::local_players::Context context[2]{};
template<class T>void Put(void* p,size_t at,const T& value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
void Check(bool value,const char* text){if(!value)throw std::runtime_error(text);}
void Label(void* object,int index,const char* name){Put(object,0x18,index);strcpy_s(reinterpret_cast<char*>(f.entries[index]+12),500,name);}
Id Live(void* object){const auto i=objectIndex++;Check(i<96,"fixture object bound");Put(object,0xC,int(i));Put(f.items,size_t(i)*24,reinterpret_cast<uintptr_t>(object));Put(f.items,size_t(i)*24+16,int(i+100));return {reinterpret_cast<uintptr_t>(object),int(i),int(i+100)};}
void Visible(void* p,bool show,unsigned char propagation){Check(propagation==1,"retained visibility ABI");unsigned char flags=0;std::memcpy(&flags,static_cast<unsigned char*>(p)+0x1CC,1);flags=show?static_cast<unsigned char>(flags|0x20):static_cast<unsigned char>(flags&~0x20);Put(p,0x1CC,flags);++visibleCalls;}
void Tick(int player=0){g_players[unsigned(player)].next=0;OnLivePlayer(context[player]);}
void Event(void* object,void* function,void* args){
 if(function==f.functions[0])Put(object,0x150,*static_cast<Vector*>(args));
 else {Check(function==f.functions[1],"verified cached location function");Put(object,0x140,*static_cast<Vector*>(args));}
}
void* SpawnNative(void* asset,void* parent,uint64_t,const Vector* location,const Vector*,const Vector* scale,unsigned char attach,bool destroy,unsigned char pool){
 Check(OwnSpawnActive()&&attach==0&&destroy&&pool==0,"private spawn does not inherit other appearance choices or pooling");
 const unsigned i=parent==f.mesh[1]?1:0;Check(parent==f.mesh[i],"copies the actual native attachment");auto* effect=f.particles[i+2];
 Put(effect,0x578,reinterpret_cast<uintptr_t>(asset));Put(effect,0x140,*location);Put(effect,0x150,*scale);Put<unsigned char>(effect,0x1CC,0x20);++spawns;
 // The shared transform suite separately exercises the full reflected ABI. This
 // fixture supplies a preverified cache to test status/ownership/lifecycle paths.
 auto& t=g_players[i].transform;t={};Capture(reinterpret_cast<uintptr_t>(effect),t.component);Capture(reinterpret_cast<uintptr_t>(f.classes[4]),t.cls);
 Capture(reinterpret_cast<uintptr_t>(f.functions[0]),t.scaleFunction);Capture(reinterpret_cast<uintptr_t>(f.functions[1]),t.locationFunction);t.scaleOffset=0x150;t.locationOffset=0x140;
 return effect;
}
void DestroyNative(void* actor,void* object){Check((actor==f.actor[0]&&object==f.particles[2])||(actor==f.actor[1]&&object==f.particles[3]),"destroy only the owned replacement, never native PSC");++destroys;}
void Fresh(std::vector<unsigned char>& image){
 thread=true;Shutdown();f={};g_players={};paused=cinematic=missing=failLoad=false;visibleCalls=objectIndex=spawns=destroys=loads=0;g_base=reinterpret_cast<uintptr_t>(image.data());
 const unsigned char stop[]={0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x8B,0x49,0x78,0x48,0x85,0xC9};const unsigned char damage[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x50,0x48,0x8B,0x79,0x68,0x48,0x8B};
 std::memcpy(image.data()+0x69FB90,stop,sizeof(stop));std::memcpy(image.data()+0x6D0860,damage,sizeof(damage));
 for(int i=0;i<64;++i)f.names[i]=reinterpret_cast<uintptr_t>(f.entries[i]);f.nameChunk[0]=reinterpret_cast<uintptr_t>(f.names);f.objectChunk[0]=reinterpret_cast<uintptr_t>(f.items);
 Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(f.nameChunk));Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(f.objectChunk));Put(image.data(),0x3959694,96);
 const char* kinds[]={"ActorClass","MeshClass","StatusComponent","MayhemStatusEffect_Core_ShieldDemon_3_Blueprint_C","ParticleSystemComponent","ParticleSystem","ObjectProperty","ArrayProperty","OtherStatus","World"};
 for(int i=0;i<10;++i){Label(f.classes[i],i+1,kinds[i]);Live(f.classes[i]);}
 f.parents[0]=reinterpret_cast<uintptr_t>(f.classes[3])+0x30;Put(f.classes[3],0x30,reinterpret_cast<uintptr_t>(f.parents));Put(f.classes[3],0x38,0);Put(image.data(),0x3812750,reinterpret_cast<uintptr_t>(f.classes[3]));
 Label(f.properties[0],11,"m_pStatusEffectComponent");Put(f.properties[0],0x10,reinterpret_cast<uintptr_t>(f.classes[6]));Put(f.properties[0],0x34,8);Put(f.properties[0],0x44,0x990);Put(f.classes[0],0x48,reinterpret_cast<uintptr_t>(f.properties[0]));
 Label(f.properties[1],12,"m_statusEffects");Put(f.properties[1],0x10,reinterpret_cast<uintptr_t>(f.classes[7]));Put(f.properties[1],0x34,16);Put(f.properties[1],0x44,0x130);Put(f.classes[2],0x48,reinterpret_cast<uintptr_t>(f.properties[1]));
 Label(f.source,13,"P_CreatureCore_ShieldDemon");Label(f.package,14,"/Game/VFX/Particles/Characters/Generic/P_CreatureCore_ShieldDemon");Label(f.target,15,"P_Borrower_Shield");Label(f.properties[0],11,"m_pStatusEffectComponent");strcpy_s(reinterpret_cast<char*>(f.entries[16]+12),500,"VFX_Aura");
 Put(f.source,0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Put(f.source,0x20,reinterpret_cast<uintptr_t>(f.package));Put(f.target,0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Live(f.source);Live(f.target);Live(f.package);
 Put(f.world,0x10,reinterpret_cast<uintptr_t>(f.classes[9]));Put(f.otherWorld,0x10,reinterpret_cast<uintptr_t>(f.classes[9]));Live(f.world);Live(f.otherWorld);
 for(int i=0;i<2;++i){
  Put(f.actor[i],0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put<unsigned char>(f.actor[i],0x118,3);Put(f.actor[i],0x380,reinterpret_cast<uintptr_t>(f.mesh[i]));Put(f.actor[i],0x990,reinterpret_cast<uintptr_t>(f.component[i]));Put(f.controller[i],0x350,reinterpret_cast<uintptr_t>(f.actor[i]));Put(f.mesh[i],0xE8,reinterpret_cast<uintptr_t>(f.actor[i]));
  Put(f.component[i],0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.component[i],0xE8,reinterpret_cast<uintptr_t>(f.actor[i]));f.statuses[i][0]=reinterpret_cast<uintptr_t>(f.status[i]);Put(f.component[i],0x130,Array{reinterpret_cast<uintptr_t>(f.statuses[i]),1,1});
  Put(f.status[i],0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put(f.status[i],0x58,uintptr_t(0));Put<unsigned char>(f.actor[i],0xC18,static_cast<unsigned char>(i+1));Put(f.status[i],0x68,reinterpret_cast<uintptr_t>(f.actor[i]));Put(f.status[i],0x78,reinterpret_cast<uintptr_t>(f.particles[i]));Put(f.status[i],0xAC,12.f);
  Live(f.actor[i]);Live(f.mesh[i]);Live(f.controller[i]);Live(f.component[i]);Live(f.status[i]);context[i]={};context[i].index=i;context[i].localPlayer=1;context[i].controller=reinterpret_cast<uintptr_t>(f.controller[i]);context[i].character=context[i].pawn=reinterpret_cast<uintptr_t>(f.actor[i]);context[i].world=reinterpret_cast<uintptr_t>(f.world);
 }
 for(unsigned i=0;i<34;++i){auto& particle=f.particles[i];Put(particle,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(particle,0x578,reinterpret_cast<uintptr_t>(i<2?f.source:f.target));Put(particle,0xE8,reinterpret_cast<uintptr_t>(f.actor[i==1||i==3?1:0]));Put(particle,0x108,reinterpret_cast<uintptr_t>(f.mesh[i==1||i==3?1:0]));Put<unsigned char>(particle,0x1CC,0x20);Live(particle);}
 Label(f.classes[10],20,"StructProperty");Label(f.classes[11],21,"NameProperty");Live(f.classes[10]);Live(f.classes[11]);Live(f.functions[0]);Live(f.functions[1]);
 const char* names[]={"RelativeLocation","RelativeRotation","RelativeScale3D","AttachSocketName"};const int offsets[]={0x140,0x160,0x150,0x170};
 for(unsigned i=0;i<4;++i){auto* prop=f.properties[i+2];Label(prop,int(i+24),names[i]);Put(prop,0x10,reinterpret_cast<uintptr_t>(f.classes[i==3?11:10]));Put(prop,0x34,i==3?8:12);Put(prop,0x44,offsets[i]);Put(prop,0x28,i?reinterpret_cast<uintptr_t>(f.properties[i+1]):uintptr_t(0));}
 Put(f.classes[4],0x48,reinterpret_cast<uintptr_t>(f.properties[5]));
 for(int i=0;i<2;++i){Put(f.particles[i],0x150,Vector{1,1,1});Put(f.particles[i],0x140,Vector{0,0,0});}
 dg::runtime::Snapshot settings{};settings.effectAppearances[unsigned(dg::effect_appearance::Family::ShieldDemonCore)]=6;dg::runtime::Publish(settings);
 g_ready=true;g_visible=Visible;g_spawn=SpawnNative;g_destroy=DestroyNative;g_event=Event;g_log=nullptr;
}
bool Shown(int i){unsigned char flags=0;std::memcpy(&flags,f.particles[i]+0x1CC,1);return (flags&0x20)!=0;}
void Active(int i=0){Tick(i);g_players[unsigned(i)].settled=GetTickCount64()-501;Tick(i);}
void Select(int choice){dg::runtime::Get().effectAppearances[unsigned(dg::effect_appearance::Family::ShieldDemonCore)].store(choice);}
void Run(){
 std::vector<unsigned char> image(0x3960000);Fresh(image);const auto status=std::vector<unsigned char>(f.status[0],f.status[0]+sizeof(f.status[0]));Active();
 Check(spawns==1&&!Shown(0)&&Shown(2),"real native source+58 zero binds via owned status+78 and presents stock");
 Check(!std::memcmp(status.data(),f.status[0],status.size()),"no status, damage, cost or recharge bytes changed");
 dg::runtime::Get().shieldTransforms[0].Store({2,10,20,30});Tick();Vector scale{},position{};ReadShield(reinterpret_cast<uintptr_t>(f.particles[2]),0x150,scale);ReadShield(reinterpret_cast<uintptr_t>(f.particles[2]),0x140,position);
 Check(dg::owned_particle::Same(scale,{2,2,2})&&dg::owned_particle::Same(position,{10,20,30})&&spawns==1,"live size/position update without recreating status or accumulating offsets");
 Tick();ReadShield(reinterpret_cast<uintptr_t>(f.particles[2]),0x150,scale);Check(dg::owned_particle::Same(scale,{2,2,2}),"no repeated multiplication");
 dg::runtime::Get().shieldTransforms[0].Store({0,0,0,0});Tick();Check(!Shown(2)&&!Shown(0),"size0 hides only the visual while keeping native protection");
 dg::runtime::Get().shieldTransforms[0].Store({1,0,0,0});Tick();Check(Shown(2),"size above zero restores our private effect");
 Select(4);Tick();Check(spawns==2&&destroys==1&&!Shown(0),"choice changes on existing shield, no native reactivation required");
 Put(f.status[0],0x78,uintptr_t(0));Tick();Check(destroys==2&&Shown(0),"native depletion removes private visual and relinquishes native visibility");
 Put(f.status[0],0x78,reinterpret_cast<uintptr_t>(f.particles[0]));Tick();Check(spawns==3&&!Shown(0),"native recharge restores the selected visual");
 Select(0);Tick();Check(Shown(0)&&destroys==3,"Native restores the untouched original and releases the override");
 Fresh(image);Select(1);Tick();const auto hides=visibleCalls;Tick();Check(!Shown(0)&&visibleCalls==hides&&!spawns,"Hidden has no per-frame visibility toggle or private spawn");Select(0);Tick();Check(Shown(0),"Native restores only our own hidden flag");
 Fresh(image);Select(5);missing=true;Active();Tick();Check(loads==1&&spawns==1,"missing selected asset requested once then used");
 Fresh(image);Select(5);missing=failLoad=true;Active();Tick();Check(loads==1&&!spawns&&Shown(0),"failed load leaves native visible with no retry loop");
 Fresh(image);paused=true;Active();Check(!spawns&&!loads&&Shown(0),"no creation/loading during pause");paused=false;cinematic=true;Tick();Check(!spawns,"no creation/loading during cutscene");
 Fresh(image);Active(1);Check(!Shown(1)&&Shown(0)&&g_players[1].actor.address==reinterpret_cast<uintptr_t>(f.actor[1]),"P2 binds and hides only its own native shield");
 Fresh(image);Put(f.status[0],0x68,reinterpret_cast<uintptr_t>(f.actor[1]));Active();Check(!spawns&&Shown(0),"foreign status recipient cannot control P1");
 Fresh(image);Put(f.status[0],0x10,reinterpret_cast<uintptr_t>(f.classes[8]));Active();Check(!spawns,"unrelated damage status excluded");
 Fresh(image);thread=false;Active();Check(!spawns,"worker thread performs no action");thread=true;
 std::cout<<"Owned shield: observed null source, native lifecycle, stock/live settings, size0, pause, failed load and P2 isolation passed\n";
}
}
namespace dg::effect_appearance {
bool ShieldLoaderReady(){return true;}
void* ShieldAsset(int choice,bool request){if(choice<2)return nullptr;if(request){++loads;if(failLoad)return nullptr;missing=false;}return missing?nullptr:f.target;}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return cinematic;}}
int main(){try{Run();return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
