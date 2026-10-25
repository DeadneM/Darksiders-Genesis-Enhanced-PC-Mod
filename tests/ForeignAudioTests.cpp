#include "../src/ForeignMontage.cpp"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::foreign_montage;
struct Fixture {
 unsigned char original[0x200]{},copy[0x200]{},sequence[0x200]{},playback[0x200]{},package[0x80]{},transient[0x80]{};
 unsigned char classes[6][0x80]{},notifies[2][0x100]{},vtable[0x280]{},sounds[2][0x80]{};
 unsigned char tracks[24]{},segments[32]{},events[184]{},sequenceEvents[184]{};
 unsigned char copyTracks[24]{},copySegments[32]{},copyEvents[4*184]{};
 unsigned char actor[0xC20]{},ability[0x220]{},controller[0x700]{},mesh[0x800]{},items[4*24]{};
} f;
unsigned char entries[32][256]{};uintptr_t names[32]{},chunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{};
bool thread=true,expire=false;unsigned calls=0,reserves=0;
template<class T>void Put(void* p,size_t at,T v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
template<class T>T At(const void* p,size_t at){T v{};std::memcpy(&v,static_cast<const unsigned char*>(p)+at,sizeof(v));return v;}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void Label(void* p,int id,const char* s){Put(p,0x18,id);strcpy_s(reinterpret_cast<char*>(entries[id]+12),244,s);}
void Sound(void* notify,void* mesh,void* asset){
 Check((notify==f.notifies[0]||notify==f.notifies[1])&&mesh==f.mesh&&(asset==f.original||asset==f.sequence),"native notify uses receiver mesh and original owning animation");
 ++calls;if(expire)Put(f.items,24+16,44);
}
void ReserveNative(void* header,int n){
 Check(header==f.copy+0x80&&n==1,"only private event header reaches native allocator");++reserves;
 Put(header,0,Array{reinterpret_cast<uintptr_t>(f.copyEvents),0,n});
}
void Fresh(){
 f={};std::memset(entries,0,sizeof(entries));calls=reserves=0;thread=true;expire=false;
 const char* cls[]={"AnimMontage","AnimSequence","AnimNotify_PlaySound","SoundCue","Strife_Blueprint_C","Package"};
 for(int i=0;i<6;++i)Label(f.classes[i],i+1,cls[i]);
 Label(f.original,7,"War_Atk_Stoneskin_Montage");Label(f.package,8,"/Game/Characters/Heroes/Hero_War/Animation/War_Atk_Stoneskin_Montage");Label(f.transient,9,"/Engine/Transient");
 Put(f.original,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.original,0x20,reinterpret_cast<uintptr_t>(f.package));Put(f.original,0x90,2.f);
 Put(f.original,0x158,Array{reinterpret_cast<uintptr_t>(f.tracks),1,1});Put(f.tracks,8,Array{reinterpret_cast<uintptr_t>(f.segments),1,1});
 Put(f.segments,0,Segment{reinterpret_cast<uintptr_t>(f.sequence),0,0,2,1,1,1});Put(f.sequence,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));
 Put(f.original,0x80,Array{reinterpret_cast<uintptr_t>(f.events),1,1});Put(f.sequence,0x80,Array{reinterpret_cast<uintptr_t>(f.sequenceEvents),1,1});
 Put(f.vtable,0x240,reinterpret_cast<uintptr_t>(&Sound));
 for(int i=0;i<2;++i){
  Put(f.notifies[i],0,reinterpret_cast<uintptr_t>(f.vtable));Put(f.notifies[i],0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.notifies[i],0x38,reinterpret_cast<uintptr_t>(f.sounds[i]));
  Put(f.notifies[i],0x40,1.f);Put(f.notifies[i],0x44,i?1.3f:1.f);Put<unsigned char>(f.notifies[i],0x48,1);Put(f.sounds[i],0x10,reinterpret_cast<uintptr_t>(f.classes[3]));
 }
 Put(f.notifies[0],0x20,reinterpret_cast<uintptr_t>(f.original));Put(f.notifies[1],0x20,reinterpret_cast<uintptr_t>(f.sequence));
 Put(f.events,0x48,reinterpret_cast<uintptr_t>(f.notifies[0]));Put(f.sequenceEvents,0x48,reinterpret_cast<uintptr_t>(f.notifies[1]));Put(f.sequenceEvents,0x24,1.f);Put(f.sequenceEvents,0x34,.0001f);
 Put(f.copy,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.copy,8,uint32_t(0x40));Put(f.copy,0x20,reinterpret_cast<uintptr_t>(f.transient));Put(f.copy,0x90,1.f);
 Put(f.copy,0x158,Array{reinterpret_cast<uintptr_t>(f.copyTracks),1,1});Put(f.copyTracks,8,Array{reinterpret_cast<uintptr_t>(f.copySegments),1,1});Put(f.copySegments,0,Segment{reinterpret_cast<uintptr_t>(f.playback),0,0,1,1,1,1});
 Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put<unsigned char>(f.actor,0x118,3);Put(f.actor,0x348,reinterpret_cast<uintptr_t>(f.controller));Put(f.actor,0x380,reinterpret_cast<uintptr_t>(f.mesh));
 Put(f.controller,0x350,reinterpret_cast<uintptr_t>(f.actor));Put<unsigned char>(f.controller,0x654,1);Put(f.mesh,0xE8,reinterpret_cast<uintptr_t>(f.actor));
 Put(f.ability,0x48,reinterpret_cast<uintptr_t>(f.actor));Put(f.ability,0x1B0,reinterpret_cast<uintptr_t>(f.actor));Put<unsigned char>(f.ability,0xD8,1);
 Put(f.actor,0xC,1);Put(f.ability,0xC,2);Put(f.items,24,reinterpret_cast<uintptr_t>(f.actor));Put(f.items,24+16,11);Put(f.items,48,reinterpret_cast<uintptr_t>(f.ability));Put(f.items,48+16,12);
 objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(reinterpret_cast<void*>(g_base),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(reinterpret_cast<void*>(g_base),0x3959694,4);
 g_ready=true;g_soundNotify=&Sound;g_reserve=&ReserveNative;g_log=nullptr;
}
void Start(){StartSounds(reinterpret_cast<uintptr_t>(f.original),f.ability);}
bool Copy(){return CopyLaterSounds(reinterpret_cast<uintptr_t>(f.original),f.copy);}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));for(int i=0;i<32;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
 Fresh();const auto before=f;Start();Check(calls==1,"opening cue invoked once, later voice excluded");Check(!std::memcmp(f.original,before.original,sizeof(f.original))&&!std::memcmp(f.notifies,before.notifies,sizeof(f.notifies)),"native montage and audio payload untouched");
 Check(Copy()&&reserves==1&&At<int>(f.copy,0x88)==1,"later cue installed on private gesture only");
 Check(At<float>(f.copyEvents,0x24)==.5f&&At<float>(f.copyEvents,0x34)==.00005f&&At<uintptr_t>(f.copyEvents,0x48)==reinterpret_cast<uintptr_t>(f.notifies[1]),"real-time timing, native volume/pitch/cue preserved");
 Check(At<uintptr_t>(f.copyEvents,8)==reinterpret_cast<uintptr_t>(f.copy)&&At<uintptr_t>(f.copyEvents,0x28)==reinterpret_cast<uintptr_t>(f.playback),"audio links target only private gesture");
 Check(!Copy()&&reserves==1,"existing private timeline never overwritten or duplicated");
 Fresh();Check(OpeningSound(reinterpret_cast<uintptr_t>(f.notifies[0]),0)&&!OpeningSound(reinterpret_cast<uintptr_t>(f.notifies[0]),.1f),"only supported boundary cue bypasses blend-sensitive queue");
 Presentation merged{};Check(Gather(reinterpret_cast<uintptr_t>(f.original),At<Array>(f.original,0x80),0,2,0,1,merged)&&!merged.count,"opening cue excluded from private Clone/beam queue to avoid duplicate playback");
 Check(Gather(reinterpret_cast<uintptr_t>(f.sequence),At<Array>(f.sequence,0x80),0,2,0,1,merged)&&merged.count==1,"later source cue retained in existing native timeline");
 Label(f.sounds[0],12,"char_war_wrath_bladegeyser");Presentation geyser{};Check(Collect(reinterpret_cast<uintptr_t>(f.original),At<Array>(f.original,0x80),geyser)&&geyser.sounds==1&&!geyser.count,"Geyser sound contract still counted while opening cue uses native direct path");
 Fresh();Put<unsigned char>(f.controller,0x654,0);Start();Check(!calls,"remote actor never starts local transfer audio");
 Fresh();Put(f.mesh,0xE8,uintptr_t(0));Start();Check(!calls,"borrowed mesh rejected");
 Fresh();Put<unsigned char>(f.ability,0xD8,0);Start();Check(!calls,"inactive ability rejected");
 Fresh();Put(f.notifies[0],0x4C,Name{10,0});Start();Check(!calls&&!Copy(),"unverified donor socket is not passed to foreign mesh");
 Fresh();Put(f.notifies[0],0x44,std::numeric_limits<float>::quiet_NaN());Start();Check(!calls&&!Copy(),"invalid pitch fails audio alone");
 Fresh();Put(f.vtable,0x240,uintptr_t(0));Start();Check(!calls&&!Copy(),"unverified notify entry never called");
 Fresh();Put(f.sequenceEvents,0x24,0.f);expire=true;Start();Check(calls==1,"actor serial invalidation between native calls prevents stale second cue");
 Fresh();thread=false;Start();Check(!calls&&!Copy(),"off-thread audio/native allocation rejected");
 Fresh();g_soundNotify=nullptr;Start();Check(!calls&&!Copy()&&!OpeningSound(reinterpret_cast<uintptr_t>(f.notifies[0]),0),"signature failure preserves original notify fallback");
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
int main(){try{Run();std::cout<<"Foreign audio source timeline, receiver ownership, native ABI and private later cues passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
