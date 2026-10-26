#include "../src/MeleeEffectFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::melee_effect;
namespace native_bone=dg::native_bone;
struct Fixture {
 unsigned char actor[0x1100]{},controller[0x400]{},mesh[0x800]{},weapons[2][0x400]{},notify[2][0x90]{},projectiles[2][0x90]{};
 unsigned char montage[0x200]{},package[0x80]{},asset[0x80]{},classes[5][0x80]{},properties[2][0x80]{},events[4*184]{};
} f;
unsigned char entries[24][256]{};uintptr_t names[24]{},chunks[1]{reinterpret_cast<uintptr_t>(names)};
dg::local_players::Context owner{};bool local=true,thread=true;unsigned calls=0,notifies=0;void* sentMesh=nullptr;native_bone::Name sentSocket{};native_bone::Point sentPosition{},sentRotation{};
template<class T>void Put(void* p,size_t at,T v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void Label(void* p,int id,const char* s){Put(p,0x18,id);strcpy_s(reinterpret_cast<char*>(entries[id]+12),244,s);}
void* Attach(void* asset,void* mesh,native_bone::Name socket,const native_bone::Point* p,const native_bone::Point* r,const native_bone::Point*,int type,bool destroy,unsigned char pool){
 Check(asset==f.asset&&!type&&destroy&&pool==7,"native effect, lifetime, attachment rule and pool retained");++calls;sentMesh=mesh;sentSocket=socket;sentPosition=*p;sentRotation=*r;return reinterpret_cast<void*>(123);
}
void Notify(void* n,void* mesh,void* animation){
 ++notifies;Check(mesh==f.mesh&&animation==f.montage,"original notify remains once on the original animation");native_bone::Point offset{0,-100,50},rotation{0,-90,0},scale{1,1,1};
 HookAttach(f.asset,mesh,{15,0},&offset,&rotation,&scale,0,true,7);Check(n==f.notify[0]||n==f.notify[1],"one of the actual Lava flash events");
}
void Fresh(){
 f={};local=thread=true;calls=notifies=0;g_ready=true;g_attach=Attach;g_plan=nullptr;g_lava=nullptr;
 dg::runtime::Get().pistolRightModel=dg::runtime::Get().pistolLeftModel=0;dg::runtime::Get().strifeBladeVisibility=0;
 const char* classes[]={"Strife_Blueprint_C","AnimNotify_PlayParticleEffect","AnimNotify_SpawnProjectile","NameProperty","StaticMeshComponent"};for(int i=0;i<5;++i)Label(f.classes[i],i+1,classes[i]);
 owner={};owner.index=0;owner.localPlayer=1;owner.world=2;owner.character=owner.pawn=reinterpret_cast<uintptr_t>(f.actor);owner.controller=reinterpret_cast<uintptr_t>(f.controller);
 Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put<unsigned char>(f.actor,0x118,3);Put(f.actor,0x348,owner.controller);Put(f.controller,0x350,owner.pawn);Put(f.actor,0x380,reinterpret_cast<uintptr_t>(f.mesh));Put(f.mesh,0xE8,owner.pawn);
 Label(f.montage,6,"Strife_LavaShot_Fire_Montage");Label(f.package,7,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_LavaShot_Fire_Montage");Put(f.montage,0x20,reinterpret_cast<uintptr_t>(f.package));Label(f.asset,8,"P_LavaShot_MuzzleFlash");
 Put(f.montage,0x80,native_bone::Array{reinterpret_cast<uintptr_t>(f.events),4,4});
 Label(f.properties[0],9,"WeaponMesh");Label(f.properties[1],10,"MuzzleSocket");Put(f.classes[2],0x48,reinterpret_cast<uintptr_t>(f.properties[0]));Put(f.properties[0],0x28,reinterpret_cast<uintptr_t>(f.properties[1]));
 for(int i=0;i<2;++i){
  Put(f.properties[i],0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put(f.properties[i],0x34,8);Put(f.properties[i],0x44,0x38+i*8);
  Put(f.notify[i],0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.notify[i],0x20,reinterpret_cast<uintptr_t>(f.montage));Put(f.notify[i],0x38,reinterpret_cast<uintptr_t>(f.asset));Put<unsigned char>(f.notify[i],0x80,1);Put(f.notify[i],0x84,native_bone::Name{15,0});
  Put(f.projectiles[i],0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.projectiles[i],0x38,native_bone::Name{11+i,0});Put(f.projectiles[i],0x40,native_bone::Name{13,0});
  const size_t at=size_t(i)*368;Put(f.events,at+0x48,reinterpret_cast<uintptr_t>(f.notify[i]));Put(f.events,at+0x24,i?.21792334f:.01877668f);Put(f.events,at+184+0x48,reinterpret_cast<uintptr_t>(f.projectiles[i]));Put(f.events,at+184+0x24,i?.21883f:.020172f);
  Put(f.actor,0x1040+size_t(i)*8,reinterpret_cast<uintptr_t>(f.weapons[i]));Put(f.weapons[i],0xE8,owner.pawn);Put(f.weapons[i],0x108,reinterpret_cast<uintptr_t>(f.mesh));Put(f.weapons[i],0x10,reinterpret_cast<uintptr_t>(f.classes[4]));
 }
 const char* values[]={"RightPistol","LeftPistol","MuzzleFlash","Unused","StrifeMuzzleFlash"};for(int i=0;i<5;++i)strcpy_s(reinterpret_cast<char*>(entries[11+i]+12),244,values[i]);
}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));for(int i=0;i<24;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
 for(int side=0;side<2;++side){Fresh();const auto original=f;Particle(f.notify[side],f.mesh,f.montage,Notify);
  Check(calls==1&&notifies==1&&sentMesh==f.weapons[side]&&sentSocket.index==13&&!sentPosition.x&&!sentPosition.y&&!sentPosition.z&&!sentRotation.x&&!sentRotation.y&&!sentRotation.z&&!g_lava,"each flash follows its actual native projectile's own muzzle once");
  Check(!std::memcmp(&original,&f,sizeof(f)),"flash adaptation does not modify actor, gun, projectile notify, shared asset or source animation");
  Fresh();(side?dg::runtime::Get().pistolLeftModel:dg::runtime::Get().pistolRightModel)=3;Particle(f.notify[side],f.mesh,f.montage,Notify);Check(notifies==1&&!calls,"OFF gun keeps native bullet event while its flash is omitted");
 }
 for(int bad=0;bad<6;++bad){Fresh();if(bad==0)local=false;if(bad==1)thread=false;if(bad==2)owner.kind=dg::local_players::PoseKind::Horse;if(bad==3)Put(f.weapons[0],0xE8,uintptr_t(0));if(bad==4)Put(f.properties[0],0x34,4);if(bad==5)Label(f.montage,6,"Strife_LavaShot_Fire_Juiced_Montage");
  Lava plan{};Check(!PrepareLava(f.notify[0],f.mesh,f.montage,plan),"remote, off-thread, mount, borrowed weapon, unknown ABI and other animations retain native presentation");
 }
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
namespace dg::local_players {bool ResolveActor(void*,Context& c){c=owner;return local;}}
int main(){try{Run();std::cout<<"Lava native flash/projectile pairing, gun ownership, OFF and source preservation passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
