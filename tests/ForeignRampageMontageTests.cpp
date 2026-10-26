#include "../src/ForeignMontage.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

namespace {
using namespace dg::foreign_montage;
struct Fixture {
 unsigned char montage[0x200]{},copy[0x200]{},sequences[2][0x240]{},idle[0x240]{},skeleton[0x200]{},warSkeleton[0x200]{};
 unsigned char classes[15][0x80]{},package[0x80]{},seqPackages[2][0x80]{},transient[0x80]{};
 unsigned char tracks[24]{},segments[64]{},sections[176]{},events[368]{},markers[24]{},states[4]{};
 unsigned char copyTracks[24]{},copySegments[64]{},copySections[176]{},copyEvents[12*184]{},copyMarkers[24]{},copyStates[4]{};
 unsigned char notify[0x100]{},notifyCopy[0x100]{},properties[12][0x80]{},blast[0x80]{};
 unsigned char chargeNotifies[9][0x110]{},chargeCopies[9][0x110]{},sequenceEvents[2][8*184]{},shield[0x80]{},shieldPackage[0x80]{};
 unsigned char skeletonClass[0x80]{},socketClass[0x80]{},socket[0x80]{},interrupts[2][0x80]{};uintptr_t sockets[1]{};
} f;
unsigned char entries[64][256]{};uintptr_t names[64]{},chunks[1]{reinterpret_cast<uintptr_t>(names)};
bool thread=true;int mode=0;unsigned constructions=0,skeletonCalls=0,chargeClones=0;Source source{};
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
 for(int i=0;i<9;++i)if(templ==f.chargeNotifies[i]){
  Check(outer==f.copy&&cls==reinterpret_cast<void*>(At<uintptr_t>(templ,0x10)),"charge effects cloned only under private montage with same native class");
  std::memcpy(f.chargeCopies[i],templ,0x110);Put(f.chargeCopies[i],8,flags);Put(f.chargeCopies[i],0x20,reinterpret_cast<uintptr_t>(outer));++chargeClones;return f.chargeCopies[i];
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
 if(mode==11)f.copyEvents[0x40]^=1; // constructor-normalized private notify data
 if(mode==12)Put(f.copySections,8,reinterpret_cast<uintptr_t>(f.copy));
 return f.copy;
}
void Fresh(int kind=0){
 std::memset(&f,0,sizeof(f));thread=true;mode=0;constructions=skeletonCalls=chargeClones=0;g_ready=true;g_construct=ConstructNative;g_package=PackageNative;g_skeleton=SkeletonNative;
 const char* classes[]={"AnimMontage","Class","AnimSequence","Package","AnimNotify_SpawnBlast","NameProperty","ClassProperty","AnimNotify_PlaySurfaceParticleEffect","AnimNotifyState_TimedParticleEffect","ObjectProperty","AnimNotify_CallAbilityFunction","ParticleSystem","ArrayProperty","StructProperty","AnimNotify_EnableAbilityInterrupt"};
 for(int i=0;i<15;++i)Label(f.classes[i],i<7?i+1:i<13?i+27:i+37,classes[i]);Put(f.classes[0],0x10,reinterpret_cast<uintptr_t>(f.classes[1]));
 constexpr const char* montageNames[]={"War_Atk_Rampage_Start_Montage","War_Atk_Rampage_Land_Montage","War_Atk_Rampage_Strike_Level1_Montage","War_Atk_Rampage_Strike_Level2_Montage","War_Atk_Rampage_Strike_Level3_Montage"};
 char path[200]{};sprintf_s(path,"/Game/Characters/Heroes/Hero_War/Animation/%s",montageNames[kind]);
 Label(f.montage,8,montageNames[kind]);Label(f.package,9,path);Put(f.montage,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.montage,0x20,reinterpret_cast<uintptr_t>(f.package));
 Label(f.skeleton,10,"Strife_Skeleton");Label(f.warSkeleton,11,"War_Mesh_Skeleton");Label(f.idle,12,"Strife_Idle");
 Put(f.idle,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.idle,0x38,reinterpret_cast<uintptr_t>(f.skeleton));Put(f.idle,0x90,2.666667f);Put(f.idle,0x94,1.f);
 Label(f.transient,13,"/Engine/Transient");Put(f.transient,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));
 strcpy_s(reinterpret_cast<char*>(entries[14]+12),244,"DefaultSlot");strcpy_s(reinterpret_cast<char*>(entries[15]+12),244,"Bone_Strife_Root");strcpy_s(reinterpret_cast<char*>(entries[16]+12),244,"TPS");
 source={reinterpret_cast<uintptr_t>(f.idle),reinterpret_cast<uintptr_t>(f.skeleton),14,0,15,0};
 const float length=kind==0?1.4f:(kind==1?1.3666667f:1.2666667f);Put(f.montage,0x38,reinterpret_cast<uintptr_t>(f.warSkeleton));Put(f.montage,0x90,length);Put(f.montage,0x94,kind==0?1.5f:1.f);
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
 if(kind==0){
  Put(f.montage,0x80,Array{});Put(f.montage,0x180,Array{});Put(f.montage,0x190,Array{});
  g_reserve=[](void* array,int n){Check(array==f.copy+0x80&&n>0&&n<=12,"native event-array allocator used only for private bounded motion cues");Put(array,0,Array{reinterpret_cast<uintptr_t>(f.copyEvents),0,12});};
  Label(f.properties[0],22,"SpawnSocket");Label(f.properties[1],23,"BlastClass");Put(f.classes[4],0x48,reinterpret_cast<uintptr_t>(f.properties[0]));
  Put(f.properties[0],0x28,reinterpret_cast<uintptr_t>(f.properties[1]));
  for(int i=0;i<2;++i){Put(f.properties[i],0x10,reinterpret_cast<uintptr_t>(f.classes[5+i]));Put(f.properties[i],0x34,8);Put(f.properties[i],0x44,i?0x40:0x38);}
  Label(f.properties[2],24,"SocketName");Put(f.properties[2],0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Put(f.properties[2],0x34,8);Put(f.properties[2],0x44,0x84);Put(f.classes[7],0x48,reinterpret_cast<uintptr_t>(f.properties[2]));
  Label(f.properties[3],25,"SocketName");Put(f.properties[3],0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Put(f.properties[3],0x34,8);Put(f.properties[3],0x44,0x38);
  Label(f.properties[4],26,"PSTemplate");Put(f.properties[4],0x10,reinterpret_cast<uintptr_t>(f.classes[9]));Put(f.properties[4],0x34,8);Put(f.properties[4],0x44,0x30);Put(f.properties[3],0x28,reinterpret_cast<uintptr_t>(f.properties[4]));Put(f.classes[8],0x48,reinterpret_cast<uintptr_t>(f.properties[3]));
  strcpy_s(reinterpret_cast<char*>(entries[27]+12),244,"Bone_War_Spine1");strcpy_s(reinterpret_cast<char*>(entries[28]+12),244,"Bone_War_Root");strcpy_s(reinterpret_cast<char*>(entries[29]+12),244,"VFX_Root");strcpy_s(reinterpret_cast<char*>(entries[30]+12),244,"EnableStopOnCollision");strcpy_s(reinterpret_cast<char*>(entries[31]+12),244,"Bone_Strife_Spine1");source.spineIndex=31;
  Label(f.shield,32,"P_Rampage_Shield");Label(f.shieldPackage,33,"/Game/VFX/Particles/Characters/War/Rampage/P_Rampage_Shield");Put(f.shield,0x20,reinterpret_cast<uintptr_t>(f.shieldPackage));Put(f.shield,0x10,reinterpret_cast<uintptr_t>(f.classes[11]));
  Label(f.skeletonClass,40,"Skeleton");Label(f.socketClass,41,"SkeletalMeshSocket");Put(f.warSkeleton,0x10,reinterpret_cast<uintptr_t>(f.skeletonClass));
  Put(f.skeletonClass,0x48,reinterpret_cast<uintptr_t>(f.properties[11]));Label(f.properties[11],42,"Sockets");Put(f.properties[11],0x10,reinterpret_cast<uintptr_t>(f.classes[12]));Put(f.properties[11],0x34,16);Put(f.properties[11],0x44,0x170);
  f.sockets[0]=reinterpret_cast<uintptr_t>(f.socket);Put(f.warSkeleton,0x170,Array{reinterpret_cast<uintptr_t>(f.sockets),1,1});Put(f.socket,0x10,reinterpret_cast<uintptr_t>(f.socketClass));Put(f.socket,0x20,reinterpret_cast<uintptr_t>(f.warSkeleton));Put(f.socket,0x28,Name{29,0});Put(f.socket,0x30,Name{28,0});Put(f.socket,0x48,-90.f);
  const char* labels[]={"RotationOffset","SocketName","BoneName","RelativeRotation","RelativeLocation","LocationOffset"};const int offsets[]={0x4C,0x28,0x30,0x44,0x38,0x40};
  for(int i=5;i<=10;++i){Label(f.properties[i],43+i-5,labels[i-5]);Put(f.properties[i],0x10,reinterpret_cast<uintptr_t>(f.classes[i==6||i==7?5:13]));Put(f.properties[i],0x34,i==6||i==7?8:12);Put(f.properties[i],0x44,offsets[i-5]);if(i<9)Put(f.properties[i],0x28,reinterpret_cast<uintptr_t>(f.properties[i+1]));}
  Put(f.properties[5],0x28,reinterpret_cast<uintptr_t>(f.properties[10]));Put(f.properties[4],0x28,reinterpret_cast<uintptr_t>(f.properties[5]));Put(f.socketClass,0x48,reinterpret_cast<uintptr_t>(f.properties[6]));
  constexpr float times[]={.213815f,.21528375f,.30003518f,0,0,.20045169f,.2290808f,.48798981f,.76002324f};
  for(int i=0;i<9;++i){
   const int seg=i<3?0:1,j=i<3?i:i-3;auto* n=f.chargeNotifies[i];auto* e=f.sequenceEvents[seg]+j*184;
   const int cls=i==0?4:i==3?8:i==5?10:7;Put(n,0x10,reinterpret_cast<uintptr_t>(f.classes[cls]));Put(n,0x20,reinterpret_cast<uintptr_t>(f.sequences[seg]));
   Put(e,i==3?0x50:0x48,reinterpret_cast<uintptr_t>(n));Put(e,0x24,times[i]);
   if(i==0){Put(n,0x38,Name{27,0});Put(n,0x40,reinterpret_cast<uintptr_t>(f.blast));}
   else if(i==3){Put(n,0x38,Name{29,0});Put(n,0x30,reinterpret_cast<uintptr_t>(f.shield));Put(e,0x58,1.0666667f);}
   else if(i==5)Put(n,0x38,Name{30,0});else Put(n,0x84,Name{28,0});
  }
  Put(f.sequences[0],0x80,Array{reinterpret_cast<uintptr_t>(f.sequenceEvents[0]),3,8});Put(f.sequences[1],0x80,Array{reinterpret_cast<uintptr_t>(f.sequenceEvents[1]),6,8});
 }

}
void* Build(){return RampageMotion(reinterpret_cast<uintptr_t>(f.montage),source);}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));for(int i=0;i<64;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
 for(int kind=0;kind<5;++kind){
  Fresh(kind);const auto before=std::make_unique<Fixture>(f);Check(Build()==f.copy&&skeletonCalls==1,"all five native phases build privately");
  Check(!std::memcmp(f.sequences,before->sequences,sizeof(f.sequences))&&!std::memcmp(f.montage,before->montage,sizeof(f.montage))&&!std::memcmp(f.sections,before->sections,sizeof(f.sections))&&!std::memcmp(f.events,before->events,sizeof(f.events))&&!std::memcmp(f.notify,before->notify,sizeof(f.notify)),"every source asset, motion clip and notify unchanged");
  Check(!std::memcmp(f.segments,f.copySegments,kind==0?64:32)&&(kind==0||(!std::memcmp(f.markers,f.copyMarkers,12)&&!std::memcmp(f.states,f.copyStates,4))),"native root-motion segments and branch/state caches byte-identical");
  Check(At<Name>(f.copyTracks,0).index==14&&At<uintptr_t>(f.copy,0x38)==source.skeleton&&At<uintptr_t>(f.copySections,8)==reinterpret_cast<uintptr_t>(f.copy)&&At<float>(f.copy,0x90)==At<float>(f.montage,0x90),"only private graph slot/skeleton/link changed, timing retained");
  if(kind==0)Check(At<int>(f.copySections,88+0x14)==1&&At<float>(f.copySections,88+0x24)==.33355409f,"native loop segment index and position retained");
  if(kind>=2)Check(At<Name>(f.notifyCopy,0x38).index==15&&At<uintptr_t>(f.notifyCopy,0x40)==At<uintptr_t>(f.notify,0x40)&&At<uintptr_t>(f.copyEvents,0x48)==reinterpret_cast<uintptr_t>(f.notifyCopy),"private slam origin moves to own root, exact native blast class preserved");
 }
 for(int failure=1;failure<=10;++failure){Fresh(4);mode=failure;const auto before=std::make_unique<Fixture>(f);Check(!Build()&&!skeletonCalls,"null/CDO/alias/changed cache rejected before changing skeleton");Check(!std::memcmp(f.montage,before->montage,sizeof(f.montage))&&!std::memcmp(f.segments,before->segments,sizeof(f.segments)),"rejected clone leaves sources untouched");}
 Fresh();const auto beforeCharge=std::make_unique<Fixture>(f);Check(Build()==f.copy&&At<int>(f.copy,0x88)==9&&chargeClones==8,"opening blast, six surface cues and timed shield cloned; collision callback preserved once");
 Check(!std::memcmp(f.sequenceEvents,beforeCharge->sequenceEvents,sizeof(f.sequenceEvents))&&!std::memcmp(f.chargeNotifies,beforeCharge->chargeNotifies,sizeof(f.chargeNotifies)),"source sequence events and their native objects remain byte-identical");
 Check(At<Name>(f.chargeCopies[0],0x38).index==31&&At<uintptr_t>(f.chargeCopies[0],0x40)==reinterpret_cast<uintptr_t>(f.blast),"charge uses receiver spine and exact native damage class");
 Check(At<uintptr_t>(f.copyEvents,3*184+0x50)==reinterpret_cast<uintptr_t>(f.chargeCopies[3])&&At<Name>(f.chargeCopies[3],0x38).index==15&&At<float>(f.copyEvents,3*184+0x58)==1.0666667f,"shield keeps native timed-state duration and receiver attachment");
 Check(At<int>(f.copyEvents,3*184+0x14)==1&&At<int>(f.copyEvents,3*184+0x74)==1&&std::fabs(At<float>(f.copyEvents,3*184+0x24)-.33333334f)<.00001f,"run begin/end links mapped to native segment without rate or loop changes");
 for(int fault=0;fault<3;++fault){Fresh();if(fault==0)Put(f.sequences[0],0x88,2);if(fault==1)Put(f.sequenceEvents[1],0x58,2.f);if(fault==2)source.spineIndex=0;Check(!Build()&&!skeletonCalls,"missing charge cue, invalid timed duration or unknown receiver spine refuses private playback");}
 Fresh();Check(Build()==f.copy&&At<float>(f.chargeCopies[3],0x50)==-90.f&&At<float>(f.chargeNotifies[3],0x50)==0,"private shield preserves actual native War socket yaw; source state untouched");
 Fresh();Put(f.socket,0x48,-45.f);Check(Build()==f.copy&&At<float>(f.chargeCopies[3],0x50)==-45.f,"shield uses native socket data, never a hard-coded camera yaw correction");
 Fresh();Put(f.socket,0x44,15.f);Check(!Build()&&!skeletonCalls,"unsupported non-yaw source socket refuses adaptation");
 Fresh(4);Put(f.sequences[0],0x80,Array{reinterpret_cast<uintptr_t>(f.sequenceEvents[0]),2,8});
 for(int i=0;i<2;++i){Put(f.interrupts[i],0x10,reinterpret_cast<uintptr_t>(f.classes[14]));Put(f.interrupts[i],0x20,reinterpret_cast<uintptr_t>(f.sequences[0]));auto* e=f.sequenceEvents[0]+i*184;const float time=i?.5021510124f:.3766137362f;Put(e,0x50,reinterpret_cast<uintptr_t>(f.interrupts[i]));Put(e,0x24,time);Put(e,0x58,1.2666666508f-time);}
 const auto finite=std::make_unique<Fixture>(f);Check(Build()==f.copy&&At<int>(f.copy,0x88)==3,"finite slam retains native impact and both cancellation windows");
 Check(At<uintptr_t>(f.copyEvents,2*184+0x50)==reinterpret_cast<uintptr_t>(f.interrupts[1])&&std::fabs(At<float>(f.copyEvents,2*184+0x60+0x24)-1.2666666508f)<.00001f,"native MOVE window and correct end link preserved without forced Finish");
 Check(!std::memcmp(f.interrupts,finite->interrupts,sizeof(f.interrupts))&&!std::memcmp(f.sequenceEvents,finite->sequenceEvents,sizeof(f.sequenceEvents)),"native interrupt source objects/events remain unchanged");
 Fresh(4);mode=11;Check(Build()==f.copy&&f.copyEvents[0x40]==f.events[0x40],"finite slam rebuilds exact source cues after native notify instancing");
 Fresh();mode=12;Check(Build()==f.copy,"native constructor self-relink accepted without weakening timing and loop checks");
 Fresh();Check(At<float>(f.montage,0x94)==1.5f&&Build()==f.copy&&At<float>(f.copy,0x94)==1.5f,"measured native start rate 1.5 preserved, no forced slowdown");
 Fresh();Put(f.montage,0x94,1.f);Check(!Build()&&!constructions,"old invented rate 1 rejected by measured start contract");
 Fresh(1);Put(f.montage,0x94,1.5f);Check(!Build()&&!constructions,"landing retains its own native rate 1");
 Fresh();source.slotIndex=16;Check(!Build()&&!constructions,"rendered TPS slot forbidden for foreign bone motion");
 Fresh();thread=false;Check(!Build()&&!constructions,"off-thread construction forbidden");
 Fresh();Put<unsigned char>(f.sequences[1],0x1A1,0);Check(!Build()&&!constructions,"missing source motion rejected");
 Fresh(2);Put(f.properties[0],0x34,4);Check(!Build()&&!skeletonCalls,"unverified reflected impact property prevents playback");
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
int main(){try{Run();std::cout<<"Private Rampage motion, loop/notify preservation, alias and reflected impact gates passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
