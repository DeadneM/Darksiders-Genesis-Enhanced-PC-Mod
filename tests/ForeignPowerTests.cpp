#include "../src/ForeignPowerFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::foreign_power;
struct Fixture {
 unsigned char actor[0x1100]{},controller[0x400]{},mesh[0x800]{},anim[0x80]{},movement[0x200]{},asset[0x200]{},bone[12]{},skeleton[0x80]{};
 unsigned char idle[2][0x100]{},power[2][0x120]{},packages[2][0x100]{},helpers[1][0x120]{},helperPackages[1][0x100]{},cdos[1][0x800]{},components[1][0x800]{};
 unsigned char classes[5][0x80]{},ability[0x240]{},montage[0x180]{},tracks[24]{},replacement[0x200]{},instance[0x40]{},items[64*24]{};
} f;
unsigned char entries[64][256]{};uintptr_t names[64]{},nameChunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{};
dg::local_players::Context owner{};bool local=true,gameThread=true,buildOK=true,registered=true,playOK=true,reassign=false;
unsigned constructors=0,registrations=0,builds=0,plays=0,finishes=0;void* lastPlayed=nullptr;int liveId=0;
template<class T>void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
template<class T>T At(const void* p,size_t at){T v{};std::memcpy(&v,static_cast<const unsigned char*>(p)+at,sizeof(v));return v;}
void Check(bool v,const char* why){if(!v)throw std::runtime_error(why);}
void Label(void* p,int id,const char* text){Put(p,0x18,id);strcpy_s(reinterpret_cast<char*>(entries[id]+12),244,text);}
dg::native_objects::Identity Live(void* p){const int id=liveId++;Put(p,0xC,id);Put(f.items,size_t(id)*24,reinterpret_cast<uintptr_t>(p));Put(f.items,size_t(id)*24+16,100+id);return {reinterpret_cast<uintptr_t>(p),id,100+id};}
void* ConstructNative(void* cls,void* outer,Name name,uint32_t flags,uint32_t internal,void* templ,bool copy,void* graph,bool assume){
 ++constructors;constexpr int i=0;
 Check(cls==f.helpers[i]&&outer==f.actor&&templ==f.cdos[i]&&!name.index&&!name.number&&flags==0x40&&!internal&&!copy&&!graph&&!assume,"helper uses exact own actor and CDO, full nine-argument ABI");
 std::memcpy(f.components[i],f.cdos[i],sizeof(f.components[i]));Put(f.components[i],8,flags);Put(f.components[i],0x20,owner.character);Live(f.components[i]);return f.components[i];
}
void RegisterNative(void* component,void* world){++registrations;Check(world==reinterpret_cast<void*>(owner.world),"component registers with its own world");if(registered){Put(component,0xE8,owner.character);Put<unsigned char>(component,0xAC,1);}}
void FinishNative(void* ability,unsigned char why){Check(ability==f.ability&&!why,"normal finish only exact owned active ability");++finishes;}
bool PlayingNative(void* p){Check(p==f.instance,"montage instance remains owned");return playOK;}
void PlayNative(void* ability,void* montage,float rate,Name){++plays;lastPlayed=montage;Check(ability==f.ability&&rate==.75f,"original rate and ability retained");Put(ability,0x1D8,reinterpret_cast<uintptr_t>(f.instance));}
void Fresh(bool war=true,int kind=0){
 f={};local=gameThread=buildOK=registered=playOK=true;reassign=false;constructors=registrations=builds=plays=finishes=0;lastPlayed=nullptr;liveId=0;
 g_ready=true;g_failed=0;g_construct=ConstructNative;g_register=RegisterNative;g_finish=FinishNative;g_playing=PlayingNative;g_handling=nullptr;g_log=nullptr;
 for(auto& x:g_owned)x={};
 const char* classes[]={"BlueprintGeneratedClass","Class","AnimSequence",war?"War_Blueprint_C":"Strife_Blueprint_C",war?"WarAnim_Blueprint_C":"StrifeAnim_Blueprint_C"};
 for(int i=0;i<5;++i)Label(f.classes[i],i+1,classes[i]);
 owner={};owner.index=0;owner.localPlayer=123;owner.world=456;owner.controller=reinterpret_cast<uintptr_t>(f.controller);owner.pawn=owner.character=reinterpret_cast<uintptr_t>(f.actor);
 Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put<unsigned char>(f.actor,0x118,3);Put<unsigned char>(f.actor,0xC18,war?2:1);Put(f.actor,0x348,owner.controller);Put(f.controller,0x350,owner.pawn);
 Put(f.actor,0x380,reinterpret_cast<uintptr_t>(f.mesh));Put(f.actor,0x388,reinterpret_cast<uintptr_t>(f.movement));Put(f.movement,0xE8,owner.character);Put<unsigned char>(f.movement,0x1B0,1);
 Put(f.mesh,0xE8,owner.character);Put(f.mesh,0x718,reinterpret_cast<uintptr_t>(f.anim));Put(f.mesh,0x5A0,reinterpret_cast<uintptr_t>(f.asset));Put(f.anim,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(f.anim,0x28,reinterpret_cast<uintptr_t>(f.skeleton));
 Put(f.asset,0x190,Array{reinterpret_cast<uintptr_t>(f.bone),1,1});strcpy_s(reinterpret_cast<char*>(entries[8]+12),244,war?"Bone_War_Root":"Bone_Strife_Root");Put(f.bone,0,Name{8,0});
 for(int i=0;i<2;++i){Label(f.idle[i],9+i,i?"War_Idle":"Strife_Idle");Put(f.idle[i],0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.idle[i],0x38,reinterpret_cast<uintptr_t>(f.skeleton));Put(f.idle[i],0x90,2.666667f);Put(f.idle[i],0x94,1.0f);if(i==1)g_idle=Live(f.idle[i]);}
 for(int i=0;i<2;++i){Label(f.power[i],11+i,i?"UnrelatedAbility_C":PowerName);Label(f.packages[i],18+i,i?"/Game/UnrelatedAbility":PowerPackage);Put(f.power[i],0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.power[i],0x20,reinterpret_cast<uintptr_t>(f.packages[i]));if(i==0)g_power=Live(f.power[i]);}
 Label(f.helpers[0],25,"ShadowCharacterComponent");Label(f.helperPackages[0],26,"/Script/DarksidersGenesis");
 Put(f.helpers[0],0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.helpers[0],0x20,reinterpret_cast<uintptr_t>(f.helperPackages[0]));
 Put(f.helpers[0],0xF8,reinterpret_cast<uintptr_t>(f.cdos[0]));Put(f.cdos[0],0x10,reinterpret_cast<uintptr_t>(f.helpers[0]));g_helper=Live(f.helpers[0]);
 Put(f.ability,0x10,reinterpret_cast<uintptr_t>(f.power[kind]));Put(f.ability,0x48,owner.character);Put(f.ability,0x1B0,owner.character);Put<unsigned char>(f.ability,0xD8,2);
 Put(f.montage,0x158,reinterpret_cast<uintptr_t>(f.tracks));Put(f.tracks,0,Name{33,0});Live(f.actor);Live(f.ability);
 objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(reinterpret_cast<void*>(g_base),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(reinterpret_cast<void*>(g_base),0x3959694,64);
}
void Cast(){Check(HandlePlay(PlayNative,f.ability,f.montage,.75f,Name{0,0}),"recognized cast handled");}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));for(unsigned i=0;i<64;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
 {Fresh();uintptr_t cls=0;constexpr int choice=1;
  Check(Prepare(f.actor,cls,choice)&&cls==At<uintptr_t>(f.ability,0x10)&&!constructors&&!registrations,"preparation selects exact loaded class without construction");
  Check(Status(6)==0&&!Prepare(f.actor,cls,6),"retired power unavailable");
  Cast();Check(builds==1&&plays==1&&!finishes&&lastPlayed==f.replacement,"compatible cast preserves native playback");
  Check(constructors==1&&registrations==1,"Clone creates and registers exactly its own helper");
  Cast();Check(constructors==1,"same owned helper reused across casts");
 }
 Fresh(false);uintptr_t nativeClass=123;Check(!Prepare(f.actor,nativeClass,1)&&nativeClass==123&&!HandlePlay(PlayNative,f.ability,f.montage,.75f,{})&&!constructors&&!plays&&!finishes,"native Strife Clone never enters the War adapter");
 for(int mismatch=0;mismatch<3;++mismatch){Fresh();if(mismatch==0)Put<unsigned char>(f.actor,0xC18,1);if(mismatch==1)Label(f.classes[4],5,"StrifeAnim_Blueprint_C");if(mismatch==2)strcpy_s(reinterpret_cast<char*>(entries[8]+12),244,"Bone_Strife_Root");uintptr_t cls=123;Check(!Prepare(f.actor,cls,1)&&cls==123&&!constructors&&!plays&&!finishes,"mixed actor, animation or bone ownership cannot prepare a foreign Clone");}
 Fresh();owner.index=1;Cast();Check(plays==1&&constructors==1,"P2 Clone retains its own component");
 for(int rejected:{2,5}){Fresh();uintptr_t cls=123;Check(!Status(rejected)&&!Prepare(f.actor,cls,rejected)&&cls==123&&!constructors,"Clone module never handles another transfer choice");}
 Fresh(true,1);Check(!HandlePlay(PlayNative,f.ability,f.montage,.75f,Name{0,0}),"unrelated native power remains untouched");
 for(int bad=0;bad<5;++bad){Fresh();if(bad==0)local=false;if(bad==1)gameThread=false;if(bad==2)owner.kind=dg::local_players::PoseKind::Horse;if(bad==3)Put<unsigned char>(f.actor,0x118,2);if(bad==4)Put<unsigned char>(f.ability,8,0x10);
  Check(!HandlePlay(PlayNative,f.ability,f.montage,.75f,{})&&!constructors&&!plays&&!finishes,"nonlocal, off-thread, transformed, remote and CDO paths untouched");}
 Fresh();registered=false;Cast();Check(finishes==1&&Status(1)==0&&!builds,"registration failure finishes exact cast and disables only that power");Check(Status(2)==0&&Status(5)==0,"rejected powers stay unavailable");
 Fresh();buildOK=false;Cast();Cast();Check(finishes==2&&builds==1&&constructors==1,"failed montage is not reconstructed repeatedly");
 Fresh();reassign=true;Cast();Check(!plays&&!finishes,"owner changed during construction is never played or finished through stale owner");
 Fresh();Shutdown();Check(!HandlePlay(PlayNative,f.ability,f.montage,.75f,{})&&!constructors,"shutdown restores native passthrough");
}
}
namespace dg::foreign_montage {
void StartSounds(uintptr_t,void*){}bool CopyLaterSounds(uintptr_t,void*){return true;}
bool Ready(){return true;}
void* Compatible(uintptr_t m,const Source& s){++builds;Check(m==reinterpret_cast<uintptr_t>(f.montage)&&s.sequence==reinterpret_cast<uintptr_t>(f.idle[At<unsigned char>(f.actor,0xC18)==2?1:0])&&s.slotIndex==33,"only own skeleton/pose and original slot reach builder");if(reassign)Put(f.ability,0x48,uintptr_t(0));return buildOK?f.replacement:nullptr;}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}}
namespace dg::local_players {bool ResolveActor(void*,Context& c){c=owner;return local;}}
int main(){try{Run();std::cout<<"Owned power components, native ABI, rejected transfer removal, failure isolation and passthrough passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
