#include "../src/StoneTransitionFx.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::stone_fx;
struct Fixture {
 unsigned char classes[12][0x120]{},statics[0x120]{},cdo[0x80]{},package[0x80]{},function[0x100]{},params[10][0x80]{};
 unsigned char particles[3][0x80]{},packages[3][0x80]{},actor[0x1100]{},controller[0x400]{},mesh[0x800]{},items[16*24]{};
} f;
unsigned char entries[64][256]{};uintptr_t names[64]{},chunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{};
dg::local_players::Context owner{};bool thread=true,spawnOk=true;int nextId=0;unsigned calls=0;float height=0;
uintptr_t lastEmitter=0;
template<class T>void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
template<class T>T At(const void* p,size_t at){T v{};std::memcpy(&v,static_cast<const unsigned char*>(p)+at,sizeof(v));return v;}
void Check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
void Label(void* p,int id,const char* text){Put(p,0x18,id);strcpy_s(reinterpret_cast<char*>(entries[id]+12),244,text);}
void Live(void* p){const int i=nextId++;Put(p,0xC,i);Put(f.items,size_t(i)*24,reinterpret_cast<uintptr_t>(p));Put(f.items,size_t(i)*24+16,i+100);}
void Dispatch(void* object,void* function,void* args){
 Check(object==f.cdo&&function==f.function,"only exact native GameplayStatics API is called");++calls;
 Check(At<uintptr_t>(args,8)==reinterpret_cast<uintptr_t>(f.mesh)&&At<uint64_t>(args,16)==0,"only current owned mesh, no donor socket");
 Check(At<float>(args,48)==1&&At<float>(args,52)==1&&At<float>(args,56)==1&&At<bool>(args,61)&&At<unsigned char>(args,62)==0,"native scale, auto-destruction and no pooling");
 height=At<float>(args,32);if(spawnOk)Put(args,64,uintptr_t(0x1234));
 lastEmitter=At<uintptr_t>(args,0);
}
void Fresh(){
 f={};thread=true;spawnOk=true;nextId=0;calls=0;height=-1;g_class={};for(auto& a:g_assets)a={};owner={};Initialize(g_base,Dispatch);
 const char* cls[]={"Class","Function","ObjectProperty","NameProperty","StructProperty","ByteProperty","BoolProperty","EnumProperty","ParticleSystem","Strife_Blueprint_C"};
 for(int i=0;i<10;++i)Label(f.classes[i],i+1,cls[i]);
 Label(f.statics,12,"GameplayStatics");Label(f.package,13,"/Script/Engine");Label(f.cdo,14,"Default__GameplayStatics");Label(f.function,15,"SpawnEmitterAttached");
 Put(f.statics,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.statics,0x20,reinterpret_cast<uintptr_t>(f.package));Put(f.statics,0xF8,reinterpret_cast<uintptr_t>(f.cdo));
 Put(f.cdo,8,uint32_t(0x10));Put(f.cdo,0x10,reinterpret_cast<uintptr_t>(f.statics));Live(f.statics);
 Put(f.statics,0x48,reinterpret_cast<uintptr_t>(f.function));Put(f.function,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.function,0x20,reinterpret_cast<uintptr_t>(f.statics));
 Put(f.function,0x98,uint32_t(0x4822401));Put(f.function,0x9E,uint16_t(72));Put(f.function,0x48,reinterpret_cast<uintptr_t>(f.params[0]));
 const char* param[]={"EmitterTemplate","AttachToComponent","AttachPointName","Location","Rotation","Scale","LocationType","bAutoDestroy","PoolingMethod","ReturnValue"};
 const int type[]={2,2,3,4,4,4,5,6,7,2},size[]={8,8,8,12,12,12,1,1,1,8},offset[]={0,8,16,24,36,48,60,61,62,64};
 for(int i=0;i<10;++i){Label(f.params[i],16+i,param[i]);Put(f.params[i],0x10,reinterpret_cast<uintptr_t>(f.classes[type[i]]));Put(f.params[i],0x34,size[i]);Put(f.params[i],0x44,offset[i]);Put(f.params[i],0x28,i<9?reinterpret_cast<uintptr_t>(f.params[i+1]):uintptr_t(0));}
 const char* ps[]={"P_WarStoneSkin_Base","P_WarStoneSkin_Molten","P_War_StoneSkin_Base_Crumble_Expiration"};
 for(int i=0;i<3;++i){Label(f.particles[i],26+i,ps[i]);char path[180]{};sprintf_s(path,"/Game/VFX/Particles/Characters/War/StoneSkin/%s",ps[i]);Label(f.packages[i],29+i,path);
  Put(f.particles[i],0x10,reinterpret_cast<uintptr_t>(f.classes[8]));Put(f.particles[i],0x20,reinterpret_cast<uintptr_t>(f.packages[i]));Live(f.particles[i]);ObserveObject(reinterpret_cast<uintptr_t>(f.particles[i]));}
 owner.index=0;owner.localPlayer=123;owner.world=456;owner.character=owner.pawn=reinterpret_cast<uintptr_t>(f.actor);owner.controller=reinterpret_cast<uintptr_t>(f.controller);
 Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.classes[9]));Put<unsigned char>(f.actor,0x118,3);Put(f.actor,0x348,owner.controller);Put(f.controller,0x350,owner.character);
 Put(f.actor,0x380,reinterpret_cast<uintptr_t>(f.mesh));Put(f.mesh,0xE8,owner.character);ObserveObject(reinterpret_cast<uintptr_t>(f.statics));
}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));
 objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(image.data(),0x3959694,16);for(int i=0;i<64;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
 for(int kind=0;kind<3;++kind){Fresh();const auto before=f;Check(AssetsReady()&&Spawn(owner,kind)&&calls==1&&height==(kind==2?180.f:0.f)&&lastEmitter==reinterpret_cast<uintptr_t>(f.particles[kind]),"base/molten launch and complete expiration use verified public API and correct template");
  Check(!std::memcmp(&f,&before,sizeof(f)),"API arguments only on local stack; source assets/CDO/actor/mesh untouched by adapter");}
 for(int bad=0;bad<13;++bad){Fresh();
  if(bad==0)thread=false;if(bad==1)owner.localPlayer=0;if(bad==2)owner.kind=dg::local_players::PoseKind::Horse;
  if(bad==3)Label(f.classes[9],10,"War_Blueprint_C");if(bad==4)Put(f.mesh,0xE8,uintptr_t(0));if(bad==5)Put(f.controller,0x350,uintptr_t(0));
  if(bad==6)Put(f.function,0x98,uint32_t(0));if(bad==7)Put(f.function,0x9E,uint16_t(80));if(bad==8)Put(f.params[8],0x44,63);
  if(bad==9)Put(f.items,16,999);if(bad==10)Put(f.cdo,0x10,reinterpret_cast<uintptr_t>(f.classes[9]));if(bad==11)Put<unsigned char>(f.actor,0x118,2);
  if(bad==12)Put(f.function,0x20,uintptr_t(0));const auto before=f;
  Check(!Spawn(owner,0)&&!calls&&!std::memcmp(&f,&before,sizeof(f)),"unsupported ownership/class/parameter/layout/function/identity/thread refuses without a call or mutation");}
 Fresh();owner.index=1;Check(Spawn(owner,2)&&calls==1,"P2 reciprocal context is independent of P1");
 Fresh();spawnOk=false;Check(!Spawn(owner,0)&&calls==1,"null component result is unavailable, not successful rendering");
 Fresh();Put(f.items,24+16,999);Check(InvalidateAssets()&&!AssetsReady()&&!Spawn(owner,0),"expired particle invalidates without dereferencing stale asset");
 Fresh();Shutdown();Check(!Spawn(owner,0)&&!calls,"shutdown does not emit effects");
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
int main(){try{Run();std::cout<<"Stoneskin public FX API, exact parameters, lifetime, scope and rejection contracts passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
