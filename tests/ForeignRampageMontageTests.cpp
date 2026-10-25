#include "../src/ForeignMontage.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
using namespace dg::foreign_montage;
struct Fixture {
 unsigned char montage[0x200]{},copy[0x200]{},sequences[2][0x240]{},idle[0x240]{},skeleton[0x200]{},warSkeleton[0x200]{};
 unsigned char classes[7][0x80]{},package[0x80]{},seqPackages[2][0x80]{},transient[0x80]{};
 unsigned char tracks[24]{},segments[64]{},sections[176]{},events[368]{},markers[24]{},states[4]{};
 unsigned char copyTracks[24]{},copySegments[64]{},copySections[176]{},copyEvents[368]{},copyMarkers[24]{},copyStates[4]{};
 unsigned char notify[0x100]{},notifyCopy[0x100]{},properties[2][0x80]{},blast[0x80]{};
} f;
unsigned char entries[32][256]{};uintptr_t names[32]{},chunks[1]{reinterpret_cast<uintptr_t>(names)};
bool thread=true;int mode=0;unsigned constructions=0,skeletonCalls=0;Source source{};
template<class T> void Put(void* p,size_t at,const T& v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
template<class T> T At(const void* p,size_t at){T v{};std::memcpy(&v,static_cast<const unsigned char*>(p)+at,sizeof(v));return v;}
void Check(bool pass,const char* why){if(!pass)throw std::runtime_error(why);}
void Label(void* p,int id,const char* text){Put(p,0x18,id);strcpy_s(reinterpret_cast<char*>(entries[id]+12),244,text);}
void* PackageNative(){return f.transient;}
void SkeletonNative(void* p,void* s){++skeletonCalls;Put(p,0x38,reinterpret_cast<uintptr_t>(s));}
void* ConstructNative(void* cls,void* outer,Name n,uint32_t flags,uint32_t internal,void* templ,bool transients,void* graph,bool assume){
 ++constructions;Check(!n.index&&!n.number&&flags==0x40&&!internal&&!transients&&!graph&&!assume,"native construction ABI");
 if(templ==f.notify){
  Check(cls==f.classes[4]&&outer==f.copy,"only own private impact notify cloned");
  std::memcpy(f.notifyCopy,f.notify,sizeof(f.notify));Put(f.notifyCopy,8,flags);Put(f.notifyCopy,0x20,reinterpret_cast<uintptr_t>(outer));return f.notifyCopy;
 }
 Check(cls==f.classes[0]&&outer==f.transient&&templ==f.montage,"only native Rampage template in transient package");
 if(mode==1)return nullptr;if(mode==2)return f.montage;
 std::memcpy(f.copy,f.montage,sizeof(f.copy));Put(f.copy,8,mode==3?uint32_t(0x50):flags);Put(f.copy,0x20,reinterpret_cast<uintptr_t>(outer));
 auto clone=[&](void* header,size_t at,void* dest,const Array& a,size_t stride,bool alias){
  if(a.count)std::memcpy(dest,reinterpret_cast<void*>(a.data),size_t(a.count)*stride);
  Put(header,at,Array{alias?a.data:reinterpret_cast<uintptr_t>(dest),a.count,a.capacity});
 };
 clone(f.copy,0x158,f.copyTracks,At<Array>(f.montage,0x158),24,mode==4);
 clone(f.copyTracks,8,f.copySegments,At<Array>(f.tracks,8),32,mode==5);
 clone(f.copy,0x148,f.copySections,At<Array>(f.montage,0x148),88,mode==6);
 clone(f.copy,0x80,f.copyEvents,At<Array>(f.montage,0x80),184,mode==7);
 clone(f.copy,0x180,f.copyMarkers,At<Array>(f.montage,0x180),12,mode==8);
 clone(f.copy,0x190,f.copyStates,At<Array>(f.montage,0x190),4,mode==9);
 if(mode==10)f.copyMarkers[0]^=1;
 return f.copy;
}
void Fresh(int kind=0){
 f={};thread=true;mode=0;constructions=skeletonCalls=0;g_ready=true;g_construct=ConstructNative;g_package=PackageNative;g_skeleton=SkeletonNative;
 const char* classes[]={"AnimMontage","Class","AnimSequence","Package","AnimNotify_SpawnBlast","NameProperty","ClassProperty"};
 for(int i=0;i<7;++i)Label(f.classes[i],i+1,classes[i]);Put(f.classes[0],0x10,reinterpret_cast<uintptr_t>(f.classes[1]));
 constexpr const char* montageNames[]={"War_Atk_Rampage_Start_Montage","War_Atk_Rampage_Land_Montage","War_Atk_Rampage_Strike_Level1_Montage","War_Atk_Rampage_Strike_Level2_Montage","War_Atk_Rampage_Strike_Level3_Montage"};
 char path[200]{};sprintf_s(path,"/Game/Characters/Heroes/Hero_War/Animation/%s",montageNames[kind]);
 Label(f.montage,8,montageNames[kind]);Label(f.package,9,path);Put(f.montage,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.montage,0x20,reinterpret_cast<uintptr_t>(f.package));
 Label(f.skeleton,10,"Strife_Skeleton");Label(f.warSkeleton,11,"War_Mesh_Skeleton");Label(f.idle,12,"Strife_Idle");
 Put(f.idle,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.idle,0x38,reinterpret_cast<uintptr_t>(f.skeleton));Put(f.idle,0x90,2.666667f);Put(f.idle,0x94,1.f);
 Label(f.transient,13,"/Engine/Transient");Put(f.transient,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));
 strcpy_s(reinterpret_cast<char*>(entries[14]+12),244,"DefaultSlot");strcpy_s(reinterpret_cast<char*>(entries[15]+12),244,"Bone_Strife_Root");strcpy_s(reinterpret_cast<char*>(entries[16]+12),244,"TPS");
 source={reinterpret_cast<uintptr_t>(f.idle),reinterpret_cast<uintptr_t>(f.skeleton),14,0,15,0};
 const float length=kind==0?1.4f:(kind==1?1.3666667f:1.2666667f);Put(f.montage,0x38,reinterpret_cast<uintptr_t>(f.warSkeleton));Put(f.montage,0x90,length);Put(f.montage,0x94,1.f);
 Put(f.montage,0x158,Array{reinterpret_cast<uintptr_t>(f.tracks),1,1});Put(f.tracks,0,Name{16,0});Put(f.tracks,8,Array{reinterpret_cast<uintptr_t>(f.segments),kind==0?2:1,2});
 Put(f.montage,0x148,Array{reinterpret_cast<uintptr_t>(f.sections),kind==0?2:1,2});Put(f.montage,0x80,Array{reinterpret_cast<uintptr_t>(f.events),1,1});
 Put(f.montage,0x180,Array{reinterpret_cast<uintptr_t>(f.markers),1,1});Put(f.montage,0x190,Array{reinterpret_cast<uintptr_t>(f.states),1,1});
 Put(f.markers,0,73);Put(f.states,0,1);
 for(int i=0;i<(kind==0?2:1);++i){
  const char* name=kind==0?(i?"War_Atk_Rampage_Run":"War_Atk_Rampage_Start"):(kind==1?"War_Dash_F_Air_Land":"War_Atk_Rampage_Strike");
  sprintf_s(path,"/Game/Characters/Heroes/Hero_War/Animation/%s",name);Label(f.sequences[i],17+i,name);Label(f.seqPackages[i],19+i,path);
  Put(f.sequences[i],0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.sequences[i],0x20,reinterpret_cast<uintptr_t>(f.seqPackages[i]));Put(f.sequences[i],0x38,reinterpret_cast<uintptr_t>(f.warSkeleton));Put<unsigned char>(f.sequences[i],0x1A1,1);
  Segment s{};s.sequence=reinterpret_cast<uintptr_t>(f.sequences[i]);s.start=i?.33333334f:0;s.last=kind==0?(i?1.0666667f:.33333334f):length;s.rate=1;s.loops=1;s.valid=1;Put(f.segments,size_t(i)*32,s);
  const auto at=size_t(i)*88;Put(f.sections,at+8,reinterpret_cast<uintptr_t>(f.montage));Put(f.sections,at+0x14,i);Put(f.sections,at+0x24,i?.33355409f:0);Put(f.sections,at+0x28,s.sequence);
 }
 Put(f.events,8,reinterpret_cast<uintptr_t>(f.montage));Put(f.events,0x68,reinterpret_cast<uintptr_t>(f.montage));Put(f.events,0x24,.25f);
 if(kind>=2){
  Put(f.notify,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(f.notify,0x20,reinterpret_cast<uintptr_t>(f.montage));Put(f.notify,0x38,Name{21,0});Put(f.notify,0x40,reinterpret_cast<uintptr_t>(f.blast));
  strcpy_s(reinterpret_cast<char*>(entries[21]+12),244,"Bone_War_Hand_L");Put(f.events,0x48,reinterpret_cast<uintptr_t>(f.notify));
  Label(f.properties[0],22,"SpawnSocket");Label(f.properties[1],23,"BlastClass");
  Put(f.classes[4],0x48,reinterpret_cast<uintptr_t>(f.properties[0]));Put(f.properties[0],0x28,reinterpret_cast<uintptr_t>(f.properties[1]));
  for(int i=0;i<2;++i){Put(f.properties[i],0x10,reinterpret_cast<uintptr_t>(f.classes[5+i]));Put(f.properties[i],0x34,8);Put(f.properties[i],0x44,i?0x40:0x38);}
 }
}
void* Build(){return RampageMotion(reinterpret_cast<uintptr_t>(f.montage),source);}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));for(int i=0;i<32;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
 for(int kind=0;kind<5;++kind){
  Fresh(kind);const auto before=f;Check(Build()==f.copy&&skeletonCalls==1,"all five native phases build privately");
  Check(!std::memcmp(f.sequences,before.sequences,sizeof(f.sequences))&&!std::memcmp(f.montage,before.montage,sizeof(f.montage))&&!std::memcmp(f.sections,before.sections,sizeof(f.sections))&&!std::memcmp(f.events,before.events,sizeof(f.events))&&!std::memcmp(f.notify,before.notify,sizeof(f.notify)),"every source asset, motion clip and notify unchanged");
  Check(!std::memcmp(f.segments,f.copySegments,kind==0?64:32)&&!std::memcmp(f.markers,f.copyMarkers,12)&&!std::memcmp(f.states,f.copyStates,4),"native root-motion segments and branch/state caches byte-identical");
  Check(At<Name>(f.copyTracks,0).index==14&&At<uintptr_t>(f.copy,0x38)==source.skeleton&&At<uintptr_t>(f.copySections,8)==reinterpret_cast<uintptr_t>(f.copy)&&At<float>(f.copy,0x90)==At<float>(f.montage,0x90),"only private graph slot/skeleton/link changed, timing retained");
  if(kind==0)Check(At<int>(f.copySections,88+0x14)==1&&At<float>(f.copySections,88+0x24)==.33355409f,"native loop segment index and position retained");
  if(kind>=2)Check(At<Name>(f.notifyCopy,0x38).index==15&&At<uintptr_t>(f.notifyCopy,0x40)==At<uintptr_t>(f.notify,0x40)&&At<uintptr_t>(f.copyEvents,0x48)==reinterpret_cast<uintptr_t>(f.notifyCopy),"private slam origin moves to own root, exact native blast class preserved");
 }
 for(int failure=1;failure<=10;++failure){Fresh();mode=failure;const auto before=f;Check(!Build()&&!skeletonCalls,"null/CDO/alias/changed cache rejected before changing skeleton");Check(!std::memcmp(f.montage,before.montage,sizeof(f.montage))&&!std::memcmp(f.segments,before.segments,sizeof(f.segments)),"rejected clone leaves sources untouched");}
 Fresh();source.slotIndex=16;Check(!Build()&&!constructions,"rendered TPS slot forbidden for foreign bone motion");
 Fresh();thread=false;Check(!Build()&&!constructions,"off-thread construction forbidden");
 Fresh();Put<unsigned char>(f.sequences[1],0x1A1,0);Check(!Build()&&!constructions,"missing source motion rejected");
 Fresh(2);Put(f.properties[0],0x34,4);Check(!Build()&&!skeletonCalls,"unverified reflected impact property prevents playback");
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
int main(){try{Run();std::cout<<"Private Rampage motion, loop/notify preservation, alias and reflected impact gates passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
