#include "../src/MountedPassiveFx.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::mounted_fx;
struct Fixture {
 unsigned char horse[0xC00]{},controller[0x400]{},mesh[0x200]{},particles[5][0x600]{},asset[0x80]{};
 unsigned char classes[7][0x80]{},fields[3][0x80]{},function[0x110]{},items[32*24]{},entries[24][128]{};
 uintptr_t names[24]{},nameChunk[1]{},objectChunk[1]{},passive[4]{};
} f;
dg::local_players::Context c{};unsigned nextId=0,calls=0;bool thread=true,paused=false,cinematic=false,radial=false;
template<class T>void Put(void* p,size_t at,const T& v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void Label(void* p,int i,const char* name){Put(p,0x18,i);strcpy_s(reinterpret_cast<char*>(f.entries[i]+12),116,name);}
Id Live(void* p){const auto i=nextId++;Check(i<32,"object fixture bound");Put(p,0xC,int(i));Put(f.items,i*24,reinterpret_cast<uintptr_t>(p));Put(f.items,i*24+16,int(100+i));return {reinterpret_cast<uintptr_t>(p),int(i),int(100+i)};}
void Property(void* p,int label,const char* name,int kind,int size,int at,void* next=nullptr){Label(p,label,name);Put(p,0x10,reinterpret_cast<uintptr_t>(f.classes[kind]));Put(p,0x34,size);Put(p,0x44,at);Put(p,0x28,reinterpret_cast<uintptr_t>(next));}
void Event(void* p,void* fn,void* args){
 Check(fn==f.function&&*static_cast<unsigned char*>(args)==0,"exact native Activate(false) contract");
 Check(p==f.particles[0]||p==f.particles[1]||p==f.particles[2],"only assigned native passive slots, never a burst or empty template");
 Put<unsigned char>(p,0xAE,1);++calls;
}
void Fresh(std::vector<unsigned char>& image){
 f={};g_states={};g_repairs=0;nextId=calls=0;thread=true;paused=cinematic=radial=false;
 g_base=reinterpret_cast<uintptr_t>(image.data());g_event=Event;g_log=nullptr;
 for(int i=0;i<24;++i)f.names[i]=reinterpret_cast<uintptr_t>(f.entries[i]);f.nameChunk[0]=reinterpret_cast<uintptr_t>(f.names);f.objectChunk[0]=reinterpret_cast<uintptr_t>(f.items);
 Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(f.nameChunk));Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(f.objectChunk));Put(image.data(),0x3959694,32);
 const char* labels[]={"Despair_Blueprint_C","ParticleSystemComponent","ParticleSystem","Function","ArrayProperty","BoolProperty","SceneComponent"};
 for(int i=0;i<7;++i)Label(f.classes[i],i+1,labels[i]);
 Property(f.fields[0],8,"PassiveFXArray",4,16,0xB40);Put(f.classes[0],0x48,reinterpret_cast<uintptr_t>(f.fields[0]));
 Property(f.fields[1],9,"bIsActive",5,1,0xAE,f.function);Put(f.classes[1],0x48,reinterpret_cast<uintptr_t>(f.fields[1]));
 Label(f.function,10,"Activate");Put(f.function,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put(f.function,0xC0,g_base+0x1CBE4D0);Put<uint16_t>(f.function,0x9E,1);
 Property(f.fields[2],11,"bReset",5,1,0);Put(f.function,0x48,reinterpret_cast<uintptr_t>(f.fields[2]));
 Put(f.horse,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put<unsigned char>(f.horse,0x118,3);Put(f.horse,0x348,reinterpret_cast<uintptr_t>(f.controller));Put(f.controller,0x350,reinterpret_cast<uintptr_t>(f.horse));
 Put(f.mesh,0x10,reinterpret_cast<uintptr_t>(f.classes[6]));Put(f.mesh,0xE8,reinterpret_cast<uintptr_t>(f.horse));Put(f.asset,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));
 Live(f.horse);Live(f.asset);
 for(int i=0;i<5;++i){auto* p=f.particles[i];Put(p,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(p,0xE8,reinterpret_cast<uintptr_t>(f.horse));Put(p,0x108,reinterpret_cast<uintptr_t>(f.mesh));Put(p,0x578,i==3?uintptr_t(0):reinterpret_cast<uintptr_t>(f.asset));Live(p);if(i<4)f.passive[i]=reinterpret_cast<uintptr_t>(p);}
 Put(f.horse,0xB40,Array{reinterpret_cast<uintptr_t>(f.passive),4,4});
 c={};c.index=0;c.localPlayer=1;c.character=2;c.world=3;c.kind=dg::local_players::PoseKind::Horse;c.pawn=reinterpret_cast<uintptr_t>(f.horse);c.controller=reinterpret_cast<uintptr_t>(f.controller);
}
void Run(){
 std::vector<unsigned char> image(0x3960000);Fresh(image);
 ObserveAt(c,1000);ObserveAt(c,2999);Check(!calls,"native mount initialization gets two seconds");ObserveAt(c,3250);Check(calls==3,"wholly inactive assigned native set is requested once, empty slot ignored");
 Put<unsigned char>(f.particles[0],0xAE,0);for(int i=0;i<100;++i)ObserveAt(c,4000+i*250);Check(calls==3,"no permanent force-active loop after the single request");
 Fresh(image);c.kind=dg::local_players::PoseKind::Foot;c.character=c.pawn;ObserveAt(c,1000);ObserveAt(c,4000);Check(calls==3,"direct mounted-state alias uses verified horse class/possession, not a prior foot-character identity");
 Fresh(image);Put<unsigned char>(f.particles[0],0xAE,1);ObserveAt(c,1000);ObserveAt(c,4000);Check(calls==2,"an active hoof does not block missing passive flames; active entries are untouched");
 Fresh(image);Put(f.horse,0xB40,Array{});ObserveAt(c,1000);ObserveAt(c,4000);Check(!calls&&!g_states[0].checked,"loading with mount does not finalize an uninitialized passive array");
 Put(f.horse,0xB40,Array{reinterpret_cast<uintptr_t>(f.passive),4,27});ObserveAt(c,4500);Check(calls==3,"late native array with normal allocator slack is accepted without a resummon");
 Fresh(image);for(int i=0;i<4;++i)Put(f.particles[i],0x578,uintptr_t(0));ObserveAt(c,1000);ObserveAt(c,4000);Check(!calls&&!g_states[0].checked,"unassigned initial templates keep the readiness window open");
 Put(f.particles[0],0x578,reinterpret_cast<uintptr_t>(f.asset));ObserveAt(c,4500);Check(calls==1,"late first assigned template can be activated once");
 Fresh(image);Put(f.horse,0xB40,Array{});ObserveAt(c,1000);for(int i=0;i<40;++i)ObserveAt(c,4000+i*250);Check(!calls&&g_states[0].checked,"missing native readiness is bounded rather than polled indefinitely");
 for(int failure=1;failure<12;++failure){Fresh(image);
  if(failure==1)Put(f.particles[1],0xE8,uintptr_t(1));
  if(failure==2)Put(f.function,0xC0,uintptr_t(0));
  if(failure==3)Put<uint16_t>(f.function,0x9E,8);
  if(failure==4)Put(f.fields[2],0x44,1);
  if(failure==5)Put(f.horse,0xB40,Array{reinterpret_cast<uintptr_t>(f.passive),17,17});
  if(failure==6)f.passive[1]=f.passive[0];
  if(failure==7)Put(f.controller,0x350,uintptr_t(0));
  if(failure==8)Put<unsigned char>(f.horse,0xA66,1);
  if(failure==9){c.kind=dg::local_players::PoseKind::Foot;c.pawn=reinterpret_cast<uintptr_t>(f.mesh);}
  if(failure==10)thread=false;
  if(failure==11)c.index=2;
  ObserveAt(c,1000);ObserveAt(c,4000);Check(!calls,"active, foreign, invalid, off-thread and despawning states never activated");
 }
 for(int gate=0;gate<3;++gate){Fresh(image);ObserveAt(c,1000);if(gate==0)paused=true;if(gate==1)cinematic=true;if(gate==2)radial=true;ObserveAt(c,4000);Check(!calls,"UI/cinematic transitions postpone repair");paused=cinematic=radial=false;ObserveAt(c,5000);Check(!calls,"wait again after a transition");ObserveAt(c,6250);Check(calls==3,"stable mounted state can complete its one request");}
 Fresh(image);c.index=1;ObserveAt(c,1000);ObserveAt(c,4000);Check(calls==3&&g_states[1].checked&&!g_states[0].horse,"P2 has independent state");Shutdown();Check(!g_event&&!g_states[1].horse,"shutdown retains no horse references");
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return cinematic;}bool RadialActive(unsigned){return radial;}}
int main(){try{Run();std::cout<<"Mounted native passive lifecycle and API guards passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
