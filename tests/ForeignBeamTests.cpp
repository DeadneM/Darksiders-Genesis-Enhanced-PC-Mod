#include "../src/ForeignBeamFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {
using namespace dg::foreign_beam;
struct Fixture {
 unsigned char actor[0x1100]{},controller[0x400]{},mesh[0x800]{},root[0x200]{},anim[0x80]{},movement[0x200]{},asset[0x240]{},bone[12]{},skeleton[0x200]{};
 unsigned char classes[10][0x80]{},power[0x120]{},powerPackage[0x100]{},helper[0x120]{},helperPackage[0x100]{},cdo[0x800]{},component[0x800]{};
 unsigned char idle[0x200]{},idlePackage[0x100]{},ability[0x240]{},montage[0x180]{},tracks[24]{},replacement[0x200]{},instance[0x40]{},items[32*24]{};
} f;
unsigned char entries[40][256]{};uintptr_t names[40]{},nameChunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{};
dg::local_players::Context context{};
bool local=true,thread=true,buildOK=true,registerOK=true,attachOK=true,playOK=true,reassign=false;int liveId=0;
unsigned constructors=0,registrations=0,attachments=0,destructions=0,builds=0,plays=0,finishes=0;
template<class T>void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
template<class T>T At(const void* p,size_t at){T v{};std::memcpy(&v,static_cast<const unsigned char*>(p)+at,sizeof(v));return v;}
void Check(bool v,const char* why){if(!v)throw std::runtime_error(why);}
void Label(void* p,int id,const char* text){Put(p,0x18,id);strcpy_s(reinterpret_cast<char*>(entries[id]+12),244,text);}
dg::native_objects::Identity Live(void* p){const int id=liveId++;Put(p,0xC,id);Put(f.items,size_t(id)*24,reinterpret_cast<uintptr_t>(p));Put(f.items,size_t(id)*24+16,100+id);return {reinterpret_cast<uintptr_t>(p),id,100+id};}
void* ConstructNative(void* cls,void* outer,Name name,uint32_t flags,uint32_t internal,void* templ,bool copy,void* graph,bool assume){
 ++constructors;Check(cls==f.helper&&outer==f.actor&&templ==f.cdo&&name==Name{0,0}&&flags==0x40&&!internal&&!copy&&!graph&&!assume,"exact nine-argument native construction, own actor and CDO");
 std::memcpy(f.component,f.cdo,sizeof(f.component));Put(f.component,8,flags);Put(f.component,0x20,context.character);Label(f.component,28,"MayhemWorldEnder_Blueprint_C");Put(f.component,0x1C,1);Live(f.component);return f.component;
}
void RegisterNative(void* p,void* world){
 ++registrations;Check(p==f.component&&world==reinterpret_cast<void*>(context.world),"register only in own world");
 Put(p,0xE8,context.character);if(registerOK)Put<unsigned char>(p,0xAC,1);
}
bool AttachNative(void* p,void* parent,Name name,unsigned char a,unsigned char b,unsigned char c,bool weld){
 ++attachments;Check(p==f.component&&parent==f.root&&name==Name{0,0}&&a==2&&b==2&&c==2&&!weld,"exact seven-argument scene attachment ABI, own capsule, snap rules, no weld");
 if(attachOK)Put(p,0x108,reinterpret_cast<uintptr_t>(parent));return attachOK;
}
void DestroyNative(void* actor,void* component){++destructions;Check(actor==f.actor&&component==f.component,"only newly owned component is destroyed");Put<unsigned char>(component,0xAC,0);}
void FinishNative(void* p,unsigned char why){++finishes;Check(p==f.ability&&!why,"finish exact owned cast normally");}
bool PlayingNative(void* p){Check(p==f.instance,"own native montage instance");return playOK;}
void PlayNative(void* p,void* montage,float rate,Name section){
 ++plays;Check(p==f.ability&&montage==f.replacement&&rate==.75f&&section==Name{0,0},"native ability, playback rate and section retained");
 Check(At<Name>(p,0x1F8)==Name{0,0}&&At<Name>(p,0x200)==Name{0,0},"ability source fields remain native None; private component supplies actual mesh and socket");Put(p,0x1D8,reinterpret_cast<uintptr_t>(f.instance));
}
void Fresh(bool war=true){
 f={};context={};local=thread=buildOK=registerOK=attachOK=playOK=true;reassign=false;liveId=0;
 constructors=registrations=attachments=destructions=builds=plays=finishes=0;
 g_ready=true;g_failed=false;g_log=nullptr;g_handling=nullptr;g_power=g_component=g_idle={};for(auto& x:g_owned)x={};
 g_construct=ConstructNative;g_register=RegisterNative;g_attach=AttachNative;g_destroy=DestroyNative;g_finish=FinishNative;g_playing=PlayingNative;
 const char* cls[]={"BlueprintGeneratedClass","AnimSequence",war?"War_Blueprint_C":"Strife_Blueprint_C",war?"WarAnim_Blueprint_C":"StrifeAnim_Blueprint_C","SkeletalMesh","Skeleton","SkeletalMeshSocket","ArrayProperty","NameProperty","Class"};
 for(int i=0;i<10;++i)Label(f.classes[i],i+1,cls[i]);
 context.index=0;context.localPlayer=123;context.world=456;context.character=context.pawn=reinterpret_cast<uintptr_t>(f.actor);context.controller=reinterpret_cast<uintptr_t>(f.controller);
 Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put<unsigned char>(f.actor,0x118,3);Put<unsigned char>(f.actor,0xC18,war?2:1);
 Put(f.actor,0x348,context.controller);Put(f.controller,0x350,context.pawn);Put(f.actor,0x380,reinterpret_cast<uintptr_t>(f.mesh));Put(f.actor,0x158,reinterpret_cast<uintptr_t>(f.root));
 Put(f.actor,0x388,reinterpret_cast<uintptr_t>(f.movement));Put(f.movement,0xE8,context.character);Put<unsigned char>(f.movement,0x1B0,1);Put(f.root,0xE8,context.character);
 Put(f.mesh,0xE8,context.character);Put(f.mesh,0x718,reinterpret_cast<uintptr_t>(f.anim));Put(f.mesh,0x5A0,reinterpret_cast<uintptr_t>(f.asset));Label(f.mesh,25,"CharacterMesh0");
 Put(f.anim,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put(f.anim,0x28,reinterpret_cast<uintptr_t>(f.skeleton));Label(f.skeleton,22,"War_Mesh_Skeleton");Put(f.skeleton,0x10,reinterpret_cast<uintptr_t>(f.classes[5]));
 Put(f.asset,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(f.asset,0x190,Array{reinterpret_cast<uintptr_t>(f.bone),1,1});Put(f.bone,0,Name{24,0});strcpy_s(reinterpret_cast<char*>(entries[24]+12),244,"Bone_War_Root");
 Label(f.power,11,"MayhemAbility_WorldEnder_Finish_Blueprint_C");Label(f.powerPackage,12,"/Game/Blueprints/Abilities/WorldEnder/MayhemAbility_WorldEnder_Finish_Blueprint");
 Label(f.helper,13,"MayhemWorldEnder_Blueprint_C");Label(f.helperPackage,14,"/Game/Blueprints/Player/MayhemWorldEnder_Blueprint");
 for(auto pair:{std::pair{f.power,f.powerPackage},std::pair{f.helper,f.helperPackage}}){Put(pair.first,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(pair.first,0x20,reinterpret_cast<uintptr_t>(pair.second));}
 Put(f.helper,0xF8,reinterpret_cast<uintptr_t>(f.cdo));Put(f.cdo,0x10,reinterpret_cast<uintptr_t>(f.helper));
 Label(f.idle,15,"War_Idle");Label(f.idlePackage,16,"/Game/Characters/Heroes/Hero_War/Animation/War_Idle");Put(f.idle,0x20,reinterpret_cast<uintptr_t>(f.idlePackage));
 Put(f.idle,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.idle,0x38,reinterpret_cast<uintptr_t>(f.skeleton));Put(f.idle,0x90,5.3333335f);Put(f.idle,0x94,1.f);
 g_power=Live(f.power);g_component=Live(f.helper);g_idle=Live(f.idle);Live(f.actor);Live(f.ability);
 Put(f.ability,0x10,reinterpret_cast<uintptr_t>(f.power));Put(f.ability,0x48,context.character);Put(f.ability,0x1B0,context.character);Put<unsigned char>(f.ability,0xD8,2);
 Put(f.montage,0x158,reinterpret_cast<uintptr_t>(f.tracks));Put(f.tracks,0,Name{27,0});strcpy_s(reinterpret_cast<char*>(entries[27]+12),244,"TPS");
 objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(reinterpret_cast<void*>(g_base),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(reinterpret_cast<void*>(g_base),0x3959694,32);
}
void Cast(){Check(HandlePlay(PlayNative,f.ability,f.montage,.75f,Name{0,0}),"owned War cast handled");}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));for(unsigned i=0;i<40;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
 Fresh();uintptr_t cls=0;Check(Prepare(f.actor,cls)&&cls==reinterpret_cast<uintptr_t>(f.power)&&!constructors,"preparation uses real War mesh and firing frame without constructing");
 const auto beforeCdo=std::vector<unsigned char>(f.cdo,f.cdo+sizeof(f.cdo));Cast();Cast();
 Check(plays==2&&builds==2&&constructors==1&&registrations==1&&attachments==1&&!finishes&&!destructions,"own registered and attached helper reused; native playback runs once per cast");
 Check(At<Name>(f.component,0x698)==Name{28,1}&&At<Name>(f.component,0x690)==Name{0,0}&&!std::memcmp(f.cdo,beforeCdo.data(),sizeof(f.cdo)),"private firing mesh uses its engine-assigned name including number, no artistic socket; CDO unchanged");
 Fresh();context.index=1;Cast();Check(plays==1&&g_owned[0].actor.address==context.character,"ownership also resolves P2 without borrowing P1");
 Fresh();buildOK=false;Cast();Cast();Check(finishes==2&&builds==1&&!constructors&&Status()==0,"bad montage stops only this transfer and leaves no unused helper");
 Fresh();registerOK=false;Cast();Check(finishes==1&&destructions==1&&!attachments&&!plays&&!g_owned[0].component,"failed registration cleans exact newly owned helper before attaching");
 Fresh();attachOK=false;Cast();Check(finishes==1&&destructions==1&&!plays&&!g_owned[0].component,"failed attachment cleans component and finishes native cast");
 Fresh();reassign=true;Cast();Check(!plays&&!constructors&&!finishes,"owner change during builder never reaches helper or stale finish");
 Fresh();Put(f.ability,0x1F8,Name{25,0});Cast();Check(finishes==1&&!builds&&!constructors,"unexpected non-native ability source is refused, not overwritten");
 Fresh();Put(f.items,size_t(g_component.index)*24+16,999);Check(Status()==1&&!Prepare(f.actor,cls),"expired class identity not reused by pointer");
 Check(InvalidateAssets()&&!g_component&&!InvalidateAssets(),"one expired asset requests one new bounded scan, not continuous full rescans");
 ObserveObject(reinterpret_cast<uintptr_t>(f.helper));Check(AssetsReady(),"newly captured class serial restores readiness without loading an asset");
 Fresh();Put<unsigned char>(f.movement,0x1B0,3);Check(!Prepare(f.actor,cls),"airborne requests stay unavailable");
 for(int bad=0;bad<6;++bad){Fresh(bad!=0);if(bad==1)local=false;if(bad==2)thread=false;if(bad==3)context.kind=dg::local_players::PoseKind::Horse;if(bad==4)Put<unsigned char>(f.actor,0x118,2);if(bad==5)Put<unsigned char>(f.ability,8,0x10);
  Check(!HandlePlay(PlayNative,f.ability,f.montage,.75f,{})&&!builds&&!constructors&&!finishes,"native Strife/nonlocal/off-thread/mount/remote/CDO paths untouched");}
 Fresh();Shutdown();Check(!HandlePlay(PlayNative,f.ability,f.montage,.75f,{})&&!builds,"shutdown returns native passthrough");
}
}
namespace dg::foreign_montage {
void StartSounds(uintptr_t,void*){}bool CopyLaterSounds(uintptr_t,void*){return true;}
bool Ready(){return true;}
void* WorldEnder(uintptr_t m,const Source& s){++builds;Check(m==reinterpret_cast<uintptr_t>(f.montage)&&s.sequence==reinterpret_cast<uintptr_t>(f.idle)&&s.skeleton==reinterpret_cast<uintptr_t>(f.skeleton)&&s.rootIndex==24&&s.slotIndex==27,"only own War skeleton/pose/root and source slot reach beam builder");if(reassign)Put(f.ability,0x48,uintptr_t(0));return buildOK?f.replacement:nullptr;}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
namespace dg::local_players {bool ResolveActor(void*,Context& c){c=context;return local;}}
int main(){try{Run();std::cout<<"War beam ownership, own firing frame, native ABI, cleanup and failure isolation passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
