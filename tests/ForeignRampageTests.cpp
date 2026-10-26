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
 unsigned char ability[0x240]{},nextAbility[0x240]{},montage[0x200]{},replacement[0x200]{},instance[0x40]{},items[64*24]{};
 unsigned char proxy[0x600]{},poseSequence[0x240]{};uint16_t compact[80]{};
 std::array<dg::pistol_visual::LocalTransform,80> pose{};
} f;
unsigned char entries[48][256]{};uintptr_t names[48]{},chunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{};
dg::local_players::Context context{};bool local=true,thread=true,buildOK=true,playOK=true,reassign=false;int liveId=0;
unsigned builds=0,plays=0,finishes=0;unsigned char cause=255;
template<class T> void Put(void* p,size_t at,const T& v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
void Check(bool pass,const char* why){if(!pass)throw std::runtime_error(why);}
void Label(void* p,int id,const char* text){Put(p,0x18,id);strcpy_s(reinterpret_cast<char*>(entries[id]+12),244,text);}
dg::native_objects::Identity Live(void* p){const int id=liveId++;Put(p,0xC,id);Put(f.items,size_t(id)*24,reinterpret_cast<uintptr_t>(p));Put(f.items,size_t(id)*24+16,100+id);return {reinterpret_cast<uintptr_t>(p),id,100+id};}
void FinishNative(void* p,unsigned char why){Check(p==f.ability||p==f.nextAbility,"only own cast finishes");++finishes;cause=why;}
bool PlayingNative(void* p){Check(p==f.instance,"own native montage instance");return playOK;}
void PlayNative(void* p,void* montage,float rate,Name section){
 Check((p==f.ability||p==f.nextAbility)&&montage==f.replacement&&rate==2&&section==Name{0,0},"native rate/section retained exactly");++plays;Put(p,0x1D8,reinterpret_cast<uintptr_t>(f.instance));
}
void Phase(int phase){Put(f.ability,0x10,reinterpret_cast<uintptr_t>(f.powers[phase]));}
unsigned samples=0;
void SampleNative(void* sequence,dg::pistol_visual::CompactPose* pose,dg::pistol_visual::Curve*,const Extract* extract){
 Check(sequence==f.poseSequence&&pose->transforms.data==reinterpret_cast<uintptr_t>(f.pose.data())&&extract->root&&std::fabs(extract->time-.65f)<.00001f,"pure native pose uses own sequence, exact extraction ABI and finite mapped phase");
 ++samples;f.pose[0].t[0]=999;f.pose[1].t[0]=321;
}
void Fresh(){
 f={};context={};local=thread=buildOK=playOK=true;reassign=false;liveId=0;builds=plays=finishes=0;cause=255;
 g_ready=true;g_failed=false;g_log=nullptr;g_events=0;g_handling=nullptr;g_sample=nullptr;g_position=nullptr;for(auto& v:g_visual)v={};g_idle={};for(auto& id:g_phases)id={};for(auto& c:g_cast)c={};g_finish=FinishNative;g_playing=PlayingNative;
 const char* classes[]={"BlueprintGeneratedClass","AnimSequence","Strife_Blueprint_C","StrifeAnim_Blueprint_C","Skeleton","StructProperty","NameProperty","Class"};
 for(int i=0;i<8;++i)Label(f.classes[i],i+1,classes[i]);
 context.index=0;context.world=456;context.localPlayer=123;context.character=context.pawn=reinterpret_cast<uintptr_t>(f.actor);context.controller=reinterpret_cast<uintptr_t>(f.controller);
 Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put<unsigned char>(f.actor,0xC18,1);Put<unsigned char>(f.actor,0x118,3);Put(f.actor,0x348,context.controller);Put(f.controller,0x350,context.character);
 Put(f.actor,0x380,reinterpret_cast<uintptr_t>(f.mesh));Put(f.mesh,0xE8,context.character);Put(f.mesh,0x718,reinterpret_cast<uintptr_t>(f.anim));Put(f.mesh,0x5A0,reinterpret_cast<uintptr_t>(f.asset));
 Put<unsigned char>(f.anim,0x30,3);Put(f.anim,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put(f.anim,0x28,reinterpret_cast<uintptr_t>(f.skeleton));Label(f.skeleton,9,"Strife_Skeleton");Put(f.skeleton,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));
 Put(f.asset,0x190,Array{reinterpret_cast<uintptr_t>(f.bone),1,1});
 Put(f.bone,0,Name{10,0});strcpy_s(reinterpret_cast<char*>(entries[10]+12),244,"Bone_Strife_Root");strcpy_s(reinterpret_cast<char*>(entries[11]+12),244,"DefaultSlot");Put(reinterpret_cast<void*>(g_base),0x3A32340,Name{11,0});
 Put(f.actor,0x388,reinterpret_cast<uintptr_t>(f.movement));Put(f.movement,0xE8,context.character);Put<unsigned char>(f.movement,0x1B0,1);
 for(int i=0;i<5;++i){char path[200]{};sprintf_s(path,"/Game/Blueprints/Abilities/Rampage/%.*s",int(std::strlen(Names[i])-2),Names[i]);Label(f.powers[i],12+i,Names[i]);Label(f.packages[i],17+i,path);Put(f.powers[i],0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.powers[i],0x20,reinterpret_cast<uintptr_t>(f.packages[i]));g_phases[i]=Live(f.powers[i]);}
 Label(f.idle,22,"Strife_Idle");Label(f.idlePackage,23,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Idle");Put(f.idle,0x20,reinterpret_cast<uintptr_t>(f.idlePackage));Put(f.idle,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.idle,0x38,reinterpret_cast<uintptr_t>(f.skeleton));g_idle=Live(f.idle);g_poses[0]=g_poses[1]=g_idle;
 Label(f.structure,24,"AnimNode_Slot");Label(f.slotProperty,25,"SlotName");Put(f.structure,0x48,reinterpret_cast<uintptr_t>(f.slotProperty));Put(f.slotProperty,0x10,reinterpret_cast<uintptr_t>(f.classes[6]));Put(f.slotProperty,0x34,8);Put(f.slotProperty,0x44,0);
 Put(f.classes[3],0x48,reinterpret_cast<uintptr_t>(f.properties[0]));constexpr const char* slots[]={"UpperBody","TPS","Shoot_Left","Shoot_Right","UpperBody_Flinches"};
 for(int i=0;i<5;++i){Put(f.properties[i],0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Put(f.properties[i],0x70,reinterpret_cast<uintptr_t>(f.structure));Put(f.properties[i],0x34,16);Put(f.properties[i],0x44,0x100+i*16);if(i<4)Put(f.properties[i],0x28,reinterpret_cast<uintptr_t>(f.properties[i+1]));strcpy_s(reinterpret_cast<char*>(entries[26+i]+12),244,slots[i]);Put(f.anim,size_t(0x100+i*16),Name{26+i,0});}
 Label(f.launch,31,"MayhemAbility_War_LaunchAttack_Blueprint_C");Label(f.launchPackage,32,"/Game/Blueprints/Abilities/WarMelee/MayhemAbility_War_LaunchAttack_Blueprint");Put(f.launch,0x20,reinterpret_cast<uintptr_t>(f.launchPackage));
 Live(f.actor);Live(f.ability);Phase(0);Put(f.ability,0x48,context.character);Put(f.ability,0x1B0,context.character);Put<unsigned char>(f.ability,0xD8,2);
 objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(reinterpret_cast<void*>(g_base),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(reinterpret_cast<void*>(g_base),0x3959694,64);
}
bool Cast(){return HandlePlay(PlayNative,f.ability,f.montage,2,{0,0});}
void Run(){
 std::vector<unsigned char> image(0x3A33000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));for(int i=0;i<48;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
 Fresh();uintptr_t cls=0;Check(Status()==2&&Prepare(f.actor,cls)&&cls==g_phases[0].address&&!builds,"ready owned grounded Strife with exactly five known graph slots");
 // Regression: same native FName identity, different display spelling.
 strcpy_s(reinterpret_cast<char*>(entries[30]+12),244,"Upperbody_Flinches");Check(Prepare(f.actor,cls),"legacy display casing also accepts identical FName identities");
 strcpy_s(reinterpret_cast<char*>(entries[30]+12),244,"UpperBody_Flinches");
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
 Fresh();Put(f.items,size_t(g_idle.index)*24+16,999);Check(Status()==1&&InvalidateAssets()&&!InvalidateAssets(),"asset serial change requests exactly one bounded rescan");ObserveObject(reinterpret_cast<uintptr_t>(f.idle));Check(Status()==1,"own Dash/Kick weak identities must also be rediscovered");g_poses[0]=g_poses[1]=g_idle;Check(Status()==2,"all required weak asset identities restore readiness");
 // Regression from the runtime log: Observe runs after Start finishes,
 // before a DISTINCT native Slam ability calls PlayMontage.
 Fresh();Cast();Put<unsigned char>(f.ability,0xD8,0);Observe(context);
 Check(g_cast[0].actor&&g_cast[0].phase==0,"ordinary Start end preserves pending native handoff");
 Live(f.nextAbility);Put(f.nextAbility,0x10,g_phases[4].address);Put(f.nextAbility,0x48,context.character);Put(f.nextAbility,0x1B0,context.character);Put<unsigned char>(f.nextAbility,0xD8,1);
 Check(HandlePlay(PlayNative,f.nextAbility,f.montage,2,{0,0})&&plays==2&&!finishes&&g_cast[0].phase==4,"distinct native Slam3 accepted after inactive Start");
 Put<unsigned char>(f.nextAbility,0xD8,0);Observe(context);Check(bool(g_cast[0].actor),"ordinary Slam end preserves final companion association");
 Put(f.nextAbility,0x10,reinterpret_cast<uintptr_t>(f.launch));Put<unsigned char>(f.nextAbility,0xD8,1);
 Check(HandlePlay(PlayNative,f.nextAbility,f.montage,2,{0,0})&&finishes==1&&cause==0&&!g_cast[0].actor,"final native companion consumes token after parent completion");
 Check(!HandlePlay(PlayNative,f.nextAbility,f.montage,2,{0,0}),"consumed token never catches unrelated later companion");
 Fresh();Cast();Put(f.ability,0x10,reinterpret_cast<uintptr_t>(f.launch));Check(Cast()&&finishes==1&&cause==0&&g_cast[0].phase==0,"early own companion finishes without stealing pending Start handoff or forwarding foreign bones");
 Phase(4);Check(Cast()&&plays==2&&g_cast[0].phase==4,"early companion does not orphan subsequent native Slam");
 for(int boundary=0;boundary<4;++boundary){
  Fresh();Cast();Put<unsigned char>(f.ability,0xD8,0);
  if(boundary==0)++context.world;if(boundary==1)context.kind=dg::local_players::PoseKind::Horse;
  if(boundary==2)++context.pawn;if(boundary==3)Put(f.items,size_t(g_cast[0].actor.index)*24+16,999);
  Observe(context);Check(!g_cast[0].actor,"world, mount, pawn or actor serial invalidation clears handoff token");
 }
 Fresh();Cast();Phase(2);Cast();Phase(3);Check(Cast()&&finishes==1&&cause==2&&plays==2,"a second finisher cannot reuse a consumed Start phase");
 Fresh();Put<unsigned char>(f.movement,0x1B0,3);Check(!Prepare(f.actor,cls),"airborne request unavailable");
 for(unsigned char mode:{0,1,2}){Fresh();Put(f.anim,0x30,mode);Check(!Prepare(f.actor,cls)&&Cast()&&cause==28&&!builds,"non-montage root-motion mode rejected without changing the mode");Check(f.anim[0x30]==mode,"never changes root-motion policy");}
 Fresh();Put(f.anim,0x100,Name{26,1});Check(!Prepare(f.actor,cls),"instanced slot name rejected");
 Fresh();Put(f.items,size_t(g_phases[0].index)*24+16,0);Check(Status()==1&&!Prepare(f.actor,cls),"unidentified class is unavailable without a null icon override");
 Fresh();const int abilityIndex=liveId-1;Put(f.items,size_t(abilityIndex)*24+16,0);Check(Cast()&&finishes==1&&cause==28&&!plays,"uncapturable live callback aborts rather than leaving the cast frozen");
 Fresh();Shutdown();Check(!Cast()&&!builds,"shutdown restores native passthrough");
 // The visual sampler is independent of the chain authorization token and
 // never advances animation, generates notifies or applies root motion.
 Fresh();samples=0;g_sample=SampleNative;Visual v{};v.ability=Live(f.ability);v.actor=Live(f.actor);v.mesh=Live(f.mesh);v.anim=Live(f.anim);v.skeleton=Live(f.skeleton);v.montage=Live(f.replacement);v.sequence=Live(f.poseSequence);v.position=.6f;v.length=1.2f;v.tick=GetTickCount64();g_visual[0]=v;
 Put(f.poseSequence,0x38,v.skeleton.address);Put(f.poseSequence,0x90,1.3f);Put(f.anim,0x240,reinterpret_cast<uintptr_t>(f.proxy));Put(f.anim,0x318,g_base+0x2A77628);
 Put(f.proxy,0xA0,v.anim.address);Put(f.proxy,0xB8,v.mesh.address);Put(f.proxy,0xB0,v.skeleton.address);for(unsigned i=0;i<80;++i)f.compact[i]=uint16_t(i);
 Put(f.proxy,0x450,Array{reinterpret_cast<uintptr_t>(f.compact),80,80});f.pose[0].t[0]=123;
 dg::pistol_visual::PoseContext output{};output.proxy=reinterpret_cast<uintptr_t>(f.proxy);output.pose.container=output.proxy+0x450;output.pose.transforms={reinterpret_cast<uintptr_t>(f.pose.data()),80,80};
 Check(Evaluate(f.anim+0x318,&output)&&samples==1&&f.pose[0].t[0]==123&&f.pose[1].t[0]==321&&!plays&&!finishes,"own visual pose sampled once, native root kept and no gameplay callback");
 f.compact[1]=2;Check(!Evaluate(f.anim+0x318,&output)&&samples==1,"unknown compact mapping preserves native evaluation");f.compact[1]=1;
 Put<unsigned char>(f.ability,0xD8,0);Check(!Evaluate(f.anim+0x318,&output)&&samples==1,"finished cast cannot leave a borrowed pose");Put<unsigned char>(f.ability,0xD8,2);
 Put(f.items,size_t(v.sequence.index)*24+16,999);Check(!Evaluate(f.anim+0x318,&output)&&samples==1,"stale animation identity never reaches native sampler");
}
}
namespace dg::foreign_montage {
void StartSounds(uintptr_t,void*){}bool CopyLaterSounds(uintptr_t,void*){return true;}
bool Ready(){return true;}
uintptr_t RampageNotify(uintptr_t n,uintptr_t,const Source&){return n;}
void* RampageMotion(uintptr_t montage,const Source& s){++builds;Check(montage==reinterpret_cast<uintptr_t>(f.montage)&&s.sequence==reinterpret_cast<uintptr_t>(f.idle)&&s.skeleton==reinterpret_cast<uintptr_t>(f.skeleton)&&s.slotIndex==11&&s.rootIndex==10,"only own skeleton/root/unused slot reaches motion builder");if(reassign)Put(f.ability,0x48,uintptr_t(0));return buildOK?f.replacement:nullptr;}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
namespace dg::local_players {bool ResolveActor(void*,Context& c){c=context;return local;}}
int main(){try{Run();std::cout<<"Rampage ownership, live graph guard, abort causes, companion and lifecycle passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
