#include "../src/ForeignWrathFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::foreign_wrath;
struct Fixture {
 unsigned char actor[0x1100]{},controller[0x400]{},mesh[0x800]{},anim[0x4D00]{},blend[0xE0]{},samples[72]{},idle[0x100]{},skeleton[0x80]{};
 unsigned char power[0x120]{},package[0x100]{},cdo[0x240]{},ability[0x240]{},montage[0x180]{},replacement[0x180]{},instance[0x50]{};
 unsigned char launch[0x80]{},launchPackage[0x100]{},asset[0x200]{},bone[12]{},classes[6][0x80]{},items[640*24]{};
} f;
unsigned char entries[24][192]{};uintptr_t names[24]{},nameChunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{};
dg::local_players::Context owner{};bool gameThread=true,local=true,buildOK=true,playing=true,helperReady=true,throwResolve=false;
unsigned plays=0,finishes=0,builds=0,getters=0,resolves=0;void* played=nullptr;float playedRate=0;
template<class T>void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
void Label(void* p,int id,const char* text){Put(p,0x18,id);strcpy_s(reinterpret_cast<char*>(entries[id]+12),180,text);}
void Register(int id,void* p,int serial){Put(p,0xC,id);Put(f.items,size_t(id)*24,reinterpret_cast<uintptr_t>(p));Put(f.items,size_t(id)*24+16,serial);}
uintptr_t* Getter(void*,uintptr_t* result,Name){++getters;*result=0xCAFE;return result;}
uintptr_t* Resolver(void* actor,uintptr_t* result,Name id,bool* unlocked,bool* affordable){
 ++resolves;HookGetter(actor,result,id);*unlocked=true;*affordable=false;
 if(throwResolve)throw std::runtime_error("native resolver");return result;
}
void PlayNative(void* ability,void* montage,float rate,Name){++plays;played=montage;playedRate=rate;Put(ability,0x1D8,reinterpret_cast<uintptr_t>(f.instance));}
void FinishNative(void* ability,unsigned char cause){Check(ability==f.ability&&cause==0,"only current cast finishes normally");++finishes;}
bool PlayingNative(void* instance){Check(instance==f.instance,"own montage instance");return playing;}
void Fresh(){
 f={};owner={};gameThread=local=buildOK=playing=helperReady=true;throwResolve=false;plays=finishes=builds=getters=resolves=0;played=nullptr;playedRate=0;
 g_ready=g_geyserReady=true;g_requested=g_geyserFailed=false;g_getter=Getter;g_play=PlayNative;g_finish=FinishNative;g_playing=PlayingNative;g_log=nullptr;g_override=nullptr;
 g_cursor=g_count=0;g_complete=false;g_scanTick=0;g_world=0;g_events=0;g_geyserActors[0]=g_geyserActors[1]={};
 const char* labels[]={"Strife_Blueprint_C","StrifeAnim_Blueprint_C","BlendSpace1D","AnimSequence","AnimMontage","BlueprintGeneratedClass"};
 for(int i=0;i<6;++i)Label(f.classes[i],i+1,labels[i]);
 Label(f.blend,7,"Strife_Locomotion");Label(f.idle,8,"Strife_Idle");strcpy_s(reinterpret_cast<char*>(entries[9]+12),180,"TPS");
 Label(f.power,10,"MayhemAbility_Wrath_BladeGeyser_Blueprint_C");Label(f.package,11,"/Game/Blueprints/Abilities/MayhemAbility_Wrath_BladeGeyser_Blueprint");
 Label(f.launch,12,"MayhemAbility_War_LaunchAttack_Blueprint_C");Label(f.launchPackage,13,"/Game/Blueprints/Abilities/WarMelee/MayhemAbility_War_LaunchAttack_Blueprint");
 strcpy_s(reinterpret_cast<char*>(entries[14]+12),180,"Bone_Strife_Root");
 owner.index=0;owner.localPlayer=123;owner.world=456;owner.controller=reinterpret_cast<uintptr_t>(f.controller);owner.character=owner.pawn=reinterpret_cast<uintptr_t>(f.actor);
 Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put<unsigned char>(f.actor,0x118,3);Put<unsigned char>(f.actor,0xC18,1);Put(f.actor,0x348,owner.controller);Put(f.controller,0x350,owner.pawn);
 Put(f.actor,0x380,reinterpret_cast<uintptr_t>(f.mesh));Put(f.mesh,0xE8,owner.pawn);Put(f.mesh,0x718,reinterpret_cast<uintptr_t>(f.anim));Put(f.mesh,0x5A0,reinterpret_cast<uintptr_t>(f.asset));
 Put(f.asset,0x190,Array{reinterpret_cast<uintptr_t>(f.bone),1,1});Put(f.bone,0,Name{14,0});
 Put(f.anim,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.anim,0x4C38,owner.pawn);Put(f.anim,0x49C8,g_base+0x2A76860);Put(f.anim,0x4A10,reinterpret_cast<uintptr_t>(f.blend));Put(f.anim,0x3740,Name{9,0});
 Put(f.blend,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.blend,0xC8,Array{reinterpret_cast<uintptr_t>(f.samples),3,3});Put(f.samples,24,reinterpret_cast<uintptr_t>(f.idle));Put(f.samples,44,1.0f);
 Put(f.idle,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put(f.idle,0x38,reinterpret_cast<uintptr_t>(f.skeleton));Put(f.idle,0x90,2.666667f);Put(f.idle,0x94,1.0f);Put(f.anim,0x28,reinterpret_cast<uintptr_t>(f.skeleton));
 Put(f.power,0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Put(f.power,0x20,reinterpret_cast<uintptr_t>(f.package));Put(f.power,0xF8,reinterpret_cast<uintptr_t>(f.cdo));Put(f.cdo,0x10,reinterpret_cast<uintptr_t>(f.power));
 Put(f.launch,0x20,reinterpret_cast<uintptr_t>(f.launchPackage));Put(f.ability,0x10,reinterpret_cast<uintptr_t>(f.power));Put(f.ability,0x48,owner.pawn);Put(f.ability,0x1B0,owner.pawn);Put<unsigned char>(f.ability,0xD8,2);
 Register(0,f.power,41);Register(1,f.actor,42);Register(2,f.ability,43);g_geyser={reinterpret_cast<uintptr_t>(f.power),0,41};
 objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(reinterpret_cast<void*>(g_base),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(reinterpret_cast<void*>(g_base),0x3959694,640);
}
void Cast(){HookPlay(f.ability,f.montage,.75f,Name{0,0});}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));for(unsigned i=0;i<24;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
 Fresh();uintptr_t cls=0;Check(Prepare(f.actor,cls,4)&&cls==reinterpret_cast<uintptr_t>(f.power),"ready Geyser retains own class");
 Check(!Prepare(f.actor,cls,6)&&Status(6)==0,"retired transfer cannot be requested or adapted");
 uintptr_t result=0;bool unlocked=false,affordable=true;Resolve(Resolver,f.actor,&result,{9,2},&unlocked,&affordable,cls);
 Check(result==cls&&unlocked&&!affordable&&resolves==1&&!getters&&!g_override,"scoped class override preserves native unlock/cost and one resolve");
 HookGetter(f.actor,&result,{9,2});Check(result==0xCAFE&&getters==1,"ordinary class lookup stays native");
 throwResolve=true;try{Resolve(Resolver,f.actor,&result,{9,2},&unlocked,&affordable,cls);}catch(const std::runtime_error&){}Check(!g_override,"exception restores scope");
 Fresh();Cast();Check(builds==1&&plays==1&&!finishes&&played==f.replacement&&playedRate==.75f,"validated Geyser builder/rate/lifecycle retained");
 Put(f.ability,0x10,reinterpret_cast<uintptr_t>(f.launch));Cast();Check(finishes==1&&!g_geyserActors[0],"unsupported companion consumes only its own recent Geyser marker");
 Cast();Check(plays==2&&finishes==1,"unrelated later launch remains native");
 Fresh();owner.index=1;Cast();Check(builds==1&&g_geyserActors[1].address==owner.pawn,"P2 owns its own marker");
 for(unsigned failure=0;failure<6;++failure){Fresh();if(failure==0)local=false;if(failure==1)gameThread=false;if(failure==2)owner.kind=dg::local_players::PoseKind::Horse;
  if(failure==3)Put<unsigned char>(f.actor,0x118,2);if(failure==4)Label(f.classes[0],1,"War_Blueprint_C");if(failure==5)Put<unsigned char>(f.ability,8,0x10);
  Cast();Check(!builds&&!finishes&&played==f.montage,"native/nonlocal/foreign-pose/default path passes through");}
 Fresh();buildOK=false;Cast();Check(g_geyserFailed&&finishes==1&&Status(4)==0,"failed compatible montage stops only Geyser");Cast();Check(builds==1,"failed constructor not retried");
 Fresh();g_geyser={};g_geyserReady=false;g_requested=true;std::memset(f.items,0,sizeof(f.items));Register(598,f.power,41);Observe(owner);Check(g_cursor==512&&!g_geyserReady,"scan stays bounded");g_scanTick=0;Observe(owner);Check(g_geyserReady,"later bounded slice finds loaded class");
 Fresh();Shutdown();Cast();Check(played==f.montage&&!builds,"shutdown restores native calls");
}
}
namespace dg::foreign_montage {
void Initialize(LogFn,bool){}bool Ready(){return helperReady;}
bool CanBuild(const Source& s){return helperReady&&s.sequence==reinterpret_cast<uintptr_t>(f.idle)&&s.skeleton==reinterpret_cast<uintptr_t>(f.skeleton)&&s.slotIndex==9;}
void* Geyser(uintptr_t m,const Source& s){++builds;Check(m==reinterpret_cast<uintptr_t>(f.montage)&&CanBuild(s)&&s.rootIndex==14,"own source/root reach Geyser builder");return buildOK?f.replacement:nullptr;}
void* Compatible(uintptr_t,const Source&){return nullptr;}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}}
namespace dg::local_players {bool ResolveActor(void*,Context& c){c=owner;return local;}}
int main(){try{Run();std::cout<<"Geyser isolation, retired transfer removal, scoped resolver and native passthrough passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
