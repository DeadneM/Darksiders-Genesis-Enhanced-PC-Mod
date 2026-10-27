#include "../src/ForeignMontage.cpp"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::foreign_montage;
struct Fixture {
 unsigned char original[0x200]{},copy[0x200]{},sequence[0x200]{},playback[0x200]{};
 unsigned char package[0x80]{},sequencePackage[0x80]{},transient[0x80]{},skeletons[2][0x80]{};
 unsigned char classes[8][0x80]{},notifies[4][0x100]{},effects[2][0x80]{};
 unsigned char tracks[24]{},segments[32]{},copyTracks[24]{},copySegments[32]{};
 unsigned char events[4*184]{},copyEvents[6*184]{};
} f;
unsigned char entries[32][256]{};uintptr_t names[32]{},chunks[1]{reinterpret_cast<uintptr_t>(names)};
bool thread=true;unsigned reserves=0;
template<class T>void Put(void* p,size_t at,T v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
template<class T>T At(const void* p,size_t at){T v{};std::memcpy(&v,static_cast<const unsigned char*>(p)+at,sizeof(v));return v;}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void Label(void* p,int id,const char* s){Put(p,0x18,id);strcpy_s(reinterpret_cast<char*>(entries[id]+12),244,s);}
void* PackageNative(){return f.transient;}
void ReserveNative(void* header,int n){
 Check(header==f.copy+0x80&&n==3,"native reserve only touches the owned montage array");++reserves;
 const int count=At<int>(header,8);Put(header,0,Array{reinterpret_cast<uintptr_t>(f.copyEvents),count,6});
}
void Fresh(bool molten=false){
 f={};std::memset(entries,0,sizeof(entries));thread=true;reserves=0;
 const char* cls[]={"AnimMontage","AnimSequence","AnimNotify_PlayParticleEffect","ParticleSystem","AnimNotify_PlaySound","AnimNotify_SpawnActor","Skeleton","Package"};
 for(int i=0;i<8;++i)Label(f.classes[i],i+1,cls[i]);
 const char* montage=molten?"War_Atk_Stoneskin_Molten_Montage":"War_Atk_Stoneskin_Montage";
 const char* sequence=molten?"War_Atk_Stoneskin_Molten":"War_Atk_Stoneskin";
 char path[200]{};sprintf_s(path,"/Game/Characters/Heroes/Hero_War/Animation/%s",montage);
 Label(f.original,9,montage);Label(f.package,10,path);sprintf_s(path,"/Game/Characters/Heroes/Hero_War/Animation/%s",sequence);
 Label(f.sequence,11,sequence);Label(f.sequencePackage,12,path);Label(f.transient,13,"/Engine/Transient");
 Label(f.skeletons[0],14,"War_Mesh_Skeleton");Label(f.skeletons[1],15,"Strife_Skeleton");
 Label(f.effects[0],16,molten?"P_WarStoneSkin_Molten":"P_WarStoneSkin_Base");Label(f.effects[1],17,molten?"P_WarStoneSkin_Molten_NoDust":"P_WarStoneSkin_Base_NoDust");
 Put(f.original,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.original,0x20,reinterpret_cast<uintptr_t>(f.package));Put(f.original,0x90,1.8333333731f);
 Put(f.original,0x158,Array{reinterpret_cast<uintptr_t>(f.tracks),1,1});Put(f.tracks,8,Array{reinterpret_cast<uintptr_t>(f.segments),1,1});
 Put(f.segments,0,Segment{reinterpret_cast<uintptr_t>(f.sequence),0,0,1.8333333731f,1,1,1});
 Put(f.sequence,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.sequence,0x20,reinterpret_cast<uintptr_t>(f.sequencePackage));Put(f.sequence,0x38,reinterpret_cast<uintptr_t>(f.skeletons[0]));
 Put(f.sequence,0x80,Array{reinterpret_cast<uintptr_t>(f.events),4,4});
 for(int i=0;i<2;++i){
  auto* n=f.notifies[i];auto* e=f.events+i*184;
  Put(n,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(n,0x20,reinterpret_cast<uintptr_t>(f.sequence));Put(n,0x38,reinterpret_cast<uintptr_t>(f.effects[i]));
  Put(f.effects[i],0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put(e,0x48,reinterpret_cast<uintptr_t>(n));Put(e,0x24,i?.741106f:.740429f);
  Put(e,0x34,.0001f);Put(e,0x98,17+i);Put<unsigned char>(e,0x9C,static_cast<unsigned char>(3+i));
 }
 // Native gameplay notifies must not cross into the private visual montage.
 for(int i=2;i<4;++i){auto* n=f.notifies[i];Put(n,0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Put(f.events+i*184,0x48,reinterpret_cast<uintptr_t>(n));Put(f.events+i*184,0x24,.748f);}
 Put(f.copy,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.copy,0x20,reinterpret_cast<uintptr_t>(f.transient));Put(f.copy,0x38,reinterpret_cast<uintptr_t>(f.skeletons[1]));Put(f.copy,0x90,1.f);
 Put(f.copy,0x158,Array{reinterpret_cast<uintptr_t>(f.copyTracks),1,1});Put(f.copyTracks,8,Array{reinterpret_cast<uintptr_t>(f.copySegments),1,1});
 Put(f.copySegments,0,Segment{reinterpret_cast<uintptr_t>(f.playback),0,0,1,1,1,1});Put(f.playback,0x38,reinterpret_cast<uintptr_t>(f.skeletons[1]));
 Put(f.copy,0x80,Array{reinterpret_cast<uintptr_t>(f.copyEvents),1,1});
 Put(f.notifies[2],0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(f.copyEvents,0x48,reinterpret_cast<uintptr_t>(f.notifies[2]));Put(f.copyEvents,0x24,.9f);
 g_ready=true;g_package=&PackageNative;g_reserve=&ReserveNative;g_log=nullptr;
}
bool Copy(){return CopyStoneskinLaunchEffects(reinterpret_cast<uintptr_t>(f.original),f.copy);}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));for(int i=0;i<32;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
 for(bool molten:{false,true}){
  Fresh(molten);const auto before=f;
  Check(Copy()&&reserves==1&&At<int>(f.copy,0x88)==3,"two launch particles join the existing audio event on flags-zero native dynamic montage");
  for(int i=0;i<2;++i){const auto* e=f.copyEvents+i*184;
   Check(At<uintptr_t>(e,0x48)==reinterpret_cast<uintptr_t>(f.notifies[i])&&std::fabs(At<float>(e,0x24)-(i?.741106f:.740429f)/1.8333333731f)<.00001f,"native base/molten launch phase mapped to accepted activation clip");
   Check(At<int>(e,0x98)==17+i&&At<unsigned char>(e,0x9C)==3+i,"LOD/filter payload retained, rather than manually firing both variants");
   Check(At<uintptr_t>(e,8)==reinterpret_cast<uintptr_t>(f.copy)&&At<uintptr_t>(e,0x28)==reinterpret_cast<uintptr_t>(f.playback),"both links reference private owned montage and receiver gesture");
  }
  Check(!std::memcmp(f.copyEvents+368,before.copyEvents,184),"preexisting sound metadata and links preserved byte for byte");
  Check(!std::memcmp(f.original,before.original,sizeof(f.original))&&!std::memcmp(f.sequence,before.sequence,sizeof(f.sequence))&&!std::memcmp(f.events,before.events,sizeof(f.events))&&!std::memcmp(f.notifies,before.notifies,sizeof(f.notifies)),"source, gameplay cues, particle payload and source filters untouched");
  Check(Copy()&&reserves==1&&At<int>(f.copy,0x88)==3,"repeated preparation does not duplicate launch cues");
 }
 Fresh();Put(f.copy,8,uint32_t(0x40));Check(Copy(),"private templated montage also accepted by exact transient ownership");
 for(int bad=0;bad<17;++bad){Fresh();
  if(bad==0)Label(f.package,10,"/Unrelated/War_Atk_Stoneskin_Montage");
  if(bad==1)Put(f.original,0x90,std::numeric_limits<float>::quiet_NaN());
  if(bad==2)Put(f.notifies[0],0x20,uintptr_t(0));
  if(bad==3)Put<unsigned char>(f.notifies[0],0x8C,1);
  if(bad==4)Put(f.notifies[0],0x84,Name{1,0});
  if(bad==5)Put(f.events,0x24,.2f);
  if(bad==6)Put(f.events,0x58,.1f);
  if(bad==7)Put(f.events,0x50,reinterpret_cast<uintptr_t>(f.notifies[3]));
  if(bad==8)Put(f.copy,8,uint32_t(0x10));
  if(bad==9)Put(f.copy,0x20,reinterpret_cast<uintptr_t>(f.package));
  if(bad==10)Label(f.skeletons[1],15,"War_Mesh_Skeleton");
  if(bad==11)Put(f.copyTracks,8,Array{reinterpret_cast<uintptr_t>(f.copySegments),2,2});
  if(bad==12)Put(f.copyEvents,0x48,reinterpret_cast<uintptr_t>(f.notifies[3]));
  if(bad==13)thread=false;
  if(bad==14)Put(f.sequence,0x80,Array{reinterpret_cast<uintptr_t>(f.events),1,4});
  if(bad==15)Put(f.events,184+0x48,reinterpret_cast<uintptr_t>(f.notifies[0]));
  if(bad==16)Put(f.copy,0x80,Array{reinterpret_cast<uintptr_t>(f.events),1,4});
  const auto before=f;Check(!Copy()&&!reserves&&!std::memcmp(&f,&before,sizeof(f)),"bad source/ownership/socket/state/filter/shape/thread contract fails without changes");
 }
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
int main(){try{Run();std::cout<<"Stoneskin launch-only native particles, private timing, filter/source preservation and rejection guards passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
