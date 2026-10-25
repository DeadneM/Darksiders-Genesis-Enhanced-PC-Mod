#include "../src/ForeignRampageFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
using namespace dg::foreign_rampage;
struct Array {uintptr_t data=0;int count=0,capacity=0;};
struct Fixture {
 unsigned char actor[0x1100]{},controller[0x400]{},mesh[0x800]{},anim[0x800]{},movement[0x200]{},asset[0x240]{},bone[12]{},skeleton[0x200]{};
 unsigned char classes[8][0x80]{},powers[5][0x120]{},packages[5][0x100]{},idle[0x240]{},idlePackage[0x100]{};
 unsigned char properties[5][0x80]{},structure[0x80]{},slotProperty[0x80]{},launch[0x120]{},launchPackage[0x100]{};
 unsigned char ability[0x240]{},montage[0x200]{},replacement[0x200]{},instance[0x40]{},items[64*24]{};
} f;
unsigned char entries[48][256]{};uintptr_t names[48]{},chunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{};
dg::local_players::Context context{};bool local=true,thread=true,buildOK=true,playOK=true,reassign=false;int liveId=0;
unsigned builds=0,plays=0,finishes=0;unsigned char cause=255;
template<class T> void Put(void* p,size_t at,const T& v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
void Check(bool pass,const char* why){if(!pass)throw std::runtime_error(why);}
void Label(void* p,int id,const char* text){Put(p,0x18,id);strcpy_s(reinterpret_cast<char*>(entries[id]+12),244,text);}
dg::native_objects::Identity Live(void* p){const int id=liveId++;Put(p,0xC,id);Put(f.items,size_t(id)*24,reinterpret_cast<uintptr_t>(p));Put(f.items,size_t(id)*24+16,100+id);return {reinterpret_cast<uintptr_t>(p),id,100+id};}
void FinishNative(void* p,unsigned char why){Check(p==f.ability,"only own cast finishes");++finishes;cause=why;}
bool PlayingNative(void* p){Check(p==f.instance,"own native montage instance");return playOK;}
void PlayNative(void* p,void* montage,float rate,Name section){
 Check(p==f.ability&&montage==f.replacement&&rate==2&&section==Name{0,0},"native rate/section retained exactly");++plays;Put(p,0x1D8,reinterpret_cast<uintptr_t>(f.instance));
}
void Phase(int phase){Put(f.ability,0x10,reinterpret_cast<uintptr_t>(f.powers[phase]));}
void Fresh(){
 f={};context={};local=thread=buildOK=playOK=true;reassign=false;liveId=0;builds=plays=finishes=0;cause=255;
 g_ready=true;g_failed=false;g_log=nullptr;g_events=0;g_handling=nullptr;g_idle={};for(auto& id:g_phases)id={};for(auto& c:g_cast)c={};g_finish=FinishNative;g_playing=PlayingNative;
 const char* classes[]={"BlueprintGeneratedClass","AnimSequence","Strife_Blueprint_C","StrifeAnim_Blueprint_C","Skeleton","StructProperty","NameProperty","Class"};
 for(int i=0;i<8;++i)Label(f.classes[i],i+1,classes[i]);
 context.index=0;context.world=456;context.localPlayer=123;context.character=context.pawn=reinterpret_cast<uintptr_t>(f.actor);context.controller=reinterpret_cast<uintptr_t>(f.controller);
 Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put<unsigned char>(f.actor,0xC18,1);Put<unsigned char>(f.actor,0x118,3);Put(f.actor,0x348,context.controller);Put(f.controller,0x350,context.character);
 Put(f.actor,0x380,reinterpret_cast<uintptr_t>(f.mesh));Put(f.mesh,0xE8,context.character);Put(f.mesh,0x718,reinterpret_cast<uintptr_t>(f.anim));Put(f.mesh,0x5A0,reinterpret_cast<uintptr_t>(f.asset));
 Put(f.anim,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put(f.anim,0x28,reinterpret_cast<uintptr_t>(f.skeleton));Label(f.skeleton,9,"Strife_Skeleton");Put(f.skeleton,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));
 Put(f.asset,0x190,Array{reinterpret_cast<uintptr_t>(f.bone),1,1});
 Put(f.bone,0,Name{10,0});strcpy_s(reinterpret_cast<char*>(entries[10]+12),244,"Bone_Strife_Root");strcpy_s(reinterpret_cast<char*>(entries[11]+12),244,"DefaultSlot");Put(reinterpret_cast<void*>(g_base),0x3A32340,Name{11,0});
 Put(f.actor,0x388,reinterpret_cast<uintptr_t>(f.movement));Put(f.movement,0xE8,context.character);Put<unsigned char>(f.movement,0x1B0,1);
 for(int i=0;i<5;++i){char path[200]{};sprintf_s(path,"/Game/Blueprints/Abilities/Rampage/%.*s",int(std::strlen(Names[i])-2),Names[i]);Label(f.powers[i],12+i,Names[i]);Label(f.packages[i],17+i,path);Put(f.powers[i],0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.powers[i],0x20,reinterpret_cast<uintptr_t>(f.packages[i]));g_phases[i]=Live(f.powers[i]);}
 Label(f.idle,22,"Strife_Idle");Label(f.idlePackage,23,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Idle");Put(f.idle,0x20,reinterpret_cast<uintptr_t>(f.idlePackage));Put(f.idle,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.idle,0x38,reinterpret_cast<uintptr_t>(f.skeleton));g_idle=Live(f.idle);
 Label(f.structure,24,"AnimNode_Slot");Label(f.slotProperty,25,"SlotName");Put(f.structure,0x48,reinterpret_cast<uintptr_t>(f.slotProperty));Put(f.slotProperty,0x10,reinterpret_cast<uintptr_t>(f.classes[6]));Put(f.slotProperty,0x34,8);Put(f.slotProperty,0x44,0);
 Put(f.classes[3],0x48,reinterpret_cast<uintptr_t>(f.properties[0]));constexpr const char* slots[]={"UpperBody","TPS","Shoot_Left","Shoot_Right","Upperbody_Flinches"};
 for(int i=0;i<5;++i){Put(f.properties[i],0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Put(f.properties[i],0x70,reinterpret_cast<uintptr_t>(f.structure));Put(f.properties[i],0x34,16);Put(f.properties[i],0x44,0x100+i*16);if(i<4)Put(f.properties[i],0x28,reinterpret_cast<uintptr_t>(f.properties[i+1]));strcpy_s(reinterpret_cast<char*>(entries[26+i]+12),244,slots[i]);Put(f.anim,size_t(0x100+i*16),Name{26+i,0});}
 Label(f.launch,31,"MayhemAbility_War_LaunchAttack_Blueprint_C");Label(f.launchPackage,32,"/Game/Blueprints/Abilities/WarMelee/MayhemAbility_War_LaunchAttack_Blueprint");Put(f.launch,0x20,reinterpret_cast<uintptr_t>(f.launchPackage));
 Live(f.actor);Live(f.ability);Phase(0);Put(f.ability,0x48,context.character);Put(f.ability,0x1B0,context.character);Put<unsigned char>(f.ability,0xD8,2);
 objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(reinterpret_cast<void*>(g_base),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(reinterpret_cast<void*>(g_base),0x3959694,64);
}
bool Cast(){return HandlePlay(PlayNative,f.ability,f.montage,2,{0,0});}
void Run(){
 std::vector<unsigned char> image(0x3A33000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));for(int i=0;i<48;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
 Fresh();uintptr_t cls=0;Check(Status()==2&&Prepare(f.actor,cls)&&cls==g_phases[0].address&&!builds,"ready owned grounded Strife with exactly five known graph slots");
 Check(Cast()&&plays==1&&!finishes&&g_cast[0].actor.address==context.character,"owned start creates marker and plays exactly once");Phase(3);Check(Cast()&&plays==2,"associated native slam phase retained");Put(f.ability,0x10,reinterpret_cast<uintptr_t>(f.launch));Check(Cast()&&finishes==1&&cause==0&&!g_cast[0].actor,"only own final unsupported War companion consumed");Check(!Cast(),"unrelated later companion remains native");
 for(int phase=1;phase<5;++phase){Fresh();Check(Cast(),"start");Phase(phase);Check(Cast()&&plays==2,"all native finish levels supported");}
 Fresh();context.index=1;Check(Cast()&&g_cast[1].actor.address==context.character&&!g_cast[0].actor,"P2 has independent ownership");
 Fresh();buildOK=false;Check(Cast()&&finishes==1&&cause==28&&!plays&&Status()==0,"failed start uses abort28 so no additional phase launches");Cast();Check(builds==1&&finishes==2,"failed transfer not reconstructed repeatedly");
 Fresh();Cast();Phase(2);buildOK=false;Check(Cast()&&cause==2&&finishes==1&&!g_cast[0].actor,"failed slam uses abort2 so no War companion launches");
 Fresh();Phase(1);Check(Cast()&&finishes==1&&cause==2&&!builds,"orphan Strife finish refuses foreign bone playback");
 Fresh();playOK=false;Check(Cast()&&cause==28&&finishes==1,"failed native playback aborts without custom timeout");
 Fresh();reassign=true;Check(Cast()&&!plays&&!finishes,"owner replacement during builder never uses stale cast");
 for(int failure=0;failure<5;++failure){Fresh();if(failure==0)Put(f.anim,0x100,Name{11,0});if(failure==1)Put(f.slotProperty,0x34,4);if(failure==2)Put(f.properties[4],0x28,reinterpret_cast<uintptr_t>(f.properties[4]));if(failure==3)Put(f.anim,0x100,Name{27,0});if(failure==4)Put(f.properties[0],0x44,0x20000);Check(!Prepare(f.actor,cls)&&Cast()&&cause==28&&!builds,"rendered DefaultSlot/unknown graph/duplicate/invalid reflection all fail closed");}
 for(int failure=0;failure<8;++failure){Fresh();if(failure==0)local=false;if(failure==1)thread=false;if(failure==2)context.kind=dg::local_players::PoseKind::Horse;if(failure==3)Put<unsigned char>(f.actor,0x118,2);if(failure==4)Label(f.classes[2],3,"War_Blueprint_C");if(failure==5)Put<uint32_t>(f.ability,8,0x10);if(failure==6)context.index=-1;if(failure==7)context.index=2;Check(!Cast()&&!builds&&!finishes,"native War/remote/mount/default/nonlocal/off-thread/out-of-range players untouched");}
 Fresh();Put(f.items,size_t(g_idle.index)*24+16,999);Check(Status()==1&&InvalidateAssets()&&!InvalidateAssets(),"asset serial change requests exactly one bounded rescan");ObserveObject(reinterpret_cast<uintptr_t>(f.idle));Check(Status()==2,"recaptured loaded idle restores readiness");
 Fresh();Cast();Put<unsigned char>(f.ability,0xD8,0);Observe(context);Check(!g_cast[0].actor,"finished cast marker cleared by lifecycle observation");
 Fresh();Put<unsigned char>(f.movement,0x1B0,3);Check(!Prepare(f.actor,cls),"airborne request unavailable");
 Fresh();Shutdown();Check(!Cast()&&!builds,"shutdown restores native passthrough");
}
}
namespace dg::foreign_montage {
bool Ready(){return true;}
void* RampageMotion(uintptr_t montage,const Source& s){++builds;Check(montage==reinterpret_cast<uintptr_t>(f.montage)&&s.sequence==reinterpret_cast<uintptr_t>(f.idle)&&s.skeleton==reinterpret_cast<uintptr_t>(f.skeleton)&&s.slotIndex==11&&s.rootIndex==10,"only own skeleton/root/unused slot reaches motion builder");if(reassign)Put(f.ability,0x48,uintptr_t(0));return buildOK?f.replacement:nullptr;}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
namespace dg::local_players {bool ResolveActor(void*,Context& c){c=context;return local;}}
int main(){try{Run();std::cout<<"Rampage ownership, live graph guard, abort causes, companion and lifecycle passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
