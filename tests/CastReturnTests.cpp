#include "../src/ForeignMontage.cpp"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::foreign_montage;
struct Fixture {
 unsigned char start[0x240]{},finish[0x240]{},montage[0x200]{},skeleton[0x180]{},classes[3][0x80]{},package[0x80]{},transient[0x80]{},notifies[2][0x100]{};
 unsigned char events[368]{},callbacks[184]{},tracks[24]{},segments[64]{},sections[88]{},items[8*24]{};int bone=0;
} f;
unsigned char entries[16][256]{};uintptr_t names[16]{},chunks[1]{},objectChunks[1]{};
bool gameThread=true,failReserve=false;unsigned reserves=0;
template<class T>void Put(void* p,size_t at,const T& value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
template<class T>T At(const void* p,size_t at){T value{};std::memcpy(&value,static_cast<const unsigned char*>(p)+at,sizeof(value));return value;}
void Check(bool yes,const char* message){if(!yes)throw std::runtime_error(message);}
void Label(void* p,int id,const char* text){Put(p,0x18,id);strcpy_s(reinterpret_cast<char*>(entries[id]+12),244,text);}
void* PackageNative(){return f.transient;}
dg::native_objects::Identity Live(void* p,int i){Put(p,0xC,i);Put(f.items,size_t(i)*24,reinterpret_cast<uintptr_t>(p));Put(f.items,size_t(i)*24+16,i+100);return {reinterpret_cast<uintptr_t>(p),i,i+100};}
void ReserveNative(void* array,int count){Check(array==f.tracks+8&&count==2,"reserve affects only the private montage segment array");++reserves;if(!failReserve)Put(array,0,Array{reinterpret_cast<uintptr_t>(f.segments),1,2});}
void Fresh(){
 f={};gameThread=true;failReserve=false;reserves=0;g_ready=true;g_package=PackageNative;g_reserveSegments=ReserveNative;g_log=nullptr;
 Label(f.classes[0],1,"AnimSequence");Label(f.classes[1],2,"AnimMontage");Label(f.classes[2],3,"AnimNotify_PlayFootstep");
 Label(f.skeleton,4,"War_Mesh_Skeleton");Label(f.finish,5,"War_Channel_End");Label(f.package,6,"/Game/Characters/Heroes/Hero_War/Animation/War_Channel_End");
 strcpy_s(reinterpret_cast<char*>(entries[7]+12),244,"TPS");Label(f.transient,8,"/Engine/Transient");
 g_gestures[1]=Live(f.start,0);g_warReturn=Live(f.finish,1);
 Put(f.finish,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.finish,0x20,reinterpret_cast<uintptr_t>(f.package));Put(f.finish,0x38,reinterpret_cast<uintptr_t>(f.skeleton));
 Put(f.finish,0x90,.6666666865f);Put(f.finish,0x94,1.f);Put<unsigned char>(f.finish,0x1A1,1);Put(f.finish,0xB0,Array{reinterpret_cast<uintptr_t>(&f.bone),1,1});
 Put(f.finish,0x80,Array{reinterpret_cast<uintptr_t>(f.events),2,2});
 const float times[]={.2398081571f,.5440219045f};
 for(int i=0;i<2;++i){Put(f.notifies[i],0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.notifies[i],0x20,reinterpret_cast<uintptr_t>(f.finish));Put(f.events,size_t(i)*184+0x48,reinterpret_cast<uintptr_t>(f.notifies[i]));Put(f.events,size_t(i)*184+0x24,times[i]);}
 Put(f.montage,8,uint32_t(0x40));Put(f.montage,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.montage,0x20,reinterpret_cast<uintptr_t>(f.transient));Put(f.montage,0x38,reinterpret_cast<uintptr_t>(f.skeleton));Put(f.montage,0x90,.5666666627f);
 Put(f.montage,0x158,Array{reinterpret_cast<uintptr_t>(f.tracks),1,1});Put(f.tracks,0,Name{7,0});Put(f.tracks,8,Array{reinterpret_cast<uintptr_t>(f.segments),1,1});
 Put(f.segments,0,Segment{reinterpret_cast<uintptr_t>(f.start),0,0,.5666666627f,1,1,1,{}});
 Put(f.montage,0x148,Array{reinterpret_cast<uintptr_t>(f.sections),1,1});Put(f.montage,0x80,Array{reinterpret_cast<uintptr_t>(f.callbacks),1,1});f.callbacks[0]=91;
}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());for(int i=0;i<16;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);chunks[0]=reinterpret_cast<uintptr_t>(names);objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);
 Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(image.data(),0x3959694,8);
 Fresh();float span=5;const auto original=f;
 Check(CaltropsGesture(reinterpret_cast<uintptr_t>(f.skeleton),span)==reinterpret_cast<uintptr_t>(f.finish)&&std::fabs(span-.6666666865f)<.00001f&&
       !reserves&&!std::memcmp(&f,&original,sizeof(f)),"Caltrops selects complete native return without construction, source changes or another segment");
 for(int bad=0;bad<6;++bad){Fresh();if(bad==0)gameThread=false;if(bad==1)g_warReturn={};if(bad==2)Put(f.items,1*24+16,999);
  if(bad==3)Put(f.events,0x24,.5f);if(bad==4)Label(f.skeleton,4,"Strife_Skeleton");span=5;
  Check(!CaltropsGesture(reinterpret_cast<uintptr_t>(bad==5?f.start:f.skeleton),span)&&span==5&&!reserves,"wrong receiver, stale/missing/invalid asset or thread keeps previous Caltrops fallback");}
 for(float seconds:{.30f,.55f}){
  Fresh();if(seconds==.30f)Put(f.montage,8,uint32_t(0));const auto before=f;Check(CompleteGroundCast(f.montage,reinterpret_cast<uintptr_t>(f.skeleton),seconds),"known private cast appends the authored grounded return, including native dynamic montages with flags zero");
  const auto finish=At<Segment>(f.segments,32);Check(reserves==1&&At<int>(f.tracks,0x10)==2&&std::fabs(At<float>(f.montage,0x90)-(.5666666627f+seconds))<.00001f&&
    finish.sequence==reinterpret_cast<uintptr_t>(f.finish)&&finish.start==.5666666627f&&finish.first==0&&finish.last==.6666666865f&&std::fabs(finish.last/finish.rate-seconds)<.00001f&&finish.loops==1&&finish.valid==1,"one nonlooping return has the requested complete duration");
  Check(!std::memcmp(f.start,before.start,sizeof(f.start))&&!std::memcmp(f.finish,before.finish,sizeof(f.finish))&&!std::memcmp(f.events,before.events,sizeof(f.events))&&
    !std::memcmp(f.notifies,before.notifies,sizeof(f.notifies))&&!std::memcmp(f.callbacks,before.callbacks,sizeof(f.callbacks))&&!std::memcmp(f.sections,before.sections,sizeof(f.sections))&&!std::memcmp(f.segments,before.segments,32),"source assets, first segment, existing callbacks and section data remain byte-identical");
  Check(!CompleteGroundCast(f.montage,reinterpret_cast<uintptr_t>(f.skeleton),seconds)&&reserves==1,"return cannot be appended twice");
 }
 for(int bad=0;bad<12;++bad){
  Fresh();if(bad==0)gameThread=false;if(bad==1)g_warReturn={};if(bad==2)Put(f.montage,8,uint32_t(0x50));if(bad==3)Put(f.montage,0x20,reinterpret_cast<uintptr_t>(f.package));
  if(bad==4)Put(f.montage,0x90,1.f);if(bad==5)Put(f.events,0x24,.5f);if(bad==6)Put(f.notifies[0],0x20,reinterpret_cast<uintptr_t>(f.start));
  if(bad==7)Put(f.finish,0x90,std::numeric_limits<float>::quiet_NaN());if(bad==8)Put(f.events,0x24,std::numeric_limits<float>::quiet_NaN());if(bad==9)g_reserveSegments=nullptr;
  if(bad==10)Put(f.items,1*24+16,999);if(bad==11)failReserve=true;const auto before=f;
  Check(!CompleteGroundCast(f.montage,reinterpret_cast<uintptr_t>(f.skeleton),.3f)&&!std::memcmp(&f,&before,sizeof(f)),"invalid/expired/private storage contract retains all original data");
 }
 Fresh();for(float bad:{0.f,2.f,std::numeric_limits<float>::quiet_NaN()})Check(!CompleteGroundCast(f.montage,reinterpret_cast<uintptr_t>(f.skeleton),bad)&&!reserves,"invalid duration refuses before reserve");
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}}
int main(){try{Run();std::cout<<"Ground cast source preservation, return timing, callbacks, ownership and refusal contracts passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
