#include "../src/ForeignCaltropsFeature.cpp"
#include <array>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

namespace {
using namespace dg::foreign_caltrops;
struct Fixture {
    unsigned char actor[0x1100]{},controller[0x400]{},movement[0x300]{},mesh[0x800]{},anim[0x100]{},skeleton[0x200]{},meshAsset[0x300]{},bones[24]{};
    unsigned char idle[0x100]{},idlePackage[0x100]{},power[0x120]{},powerPackage[0x100]{},cdo[0x240]{},ability[0x240]{};
    unsigned char projectile[0x100]{},projectilePackage[0x100]{},montage[0x180]{},montagePackage[0x100]{},tracks[24]{},strifeSkeleton[0x80]{};
    unsigned char replacement[0x180]{},instance[0x50]{},classes[5][0x80]{},items[640*24]{};
} f;
unsigned char entries[24][192]{};uintptr_t names[24]{},nameChunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{};
dg::local_players::Context owner{};
bool gesture=false;bool gameThread=true,local=true,createOK=true,playing=true,reenter=false,expire=false,expireCreate=false,changeOwner=false,throwPlay=false;
unsigned creates=0,plays=0,spawns=0,finishes=0,playingCalls=0;float playedRate=0;void* playedMontage=nullptr;Name playedSection{};
struct Created {void* sequence=nullptr;Name slot{};float in=0,out=0,rate=0;int loops=0;float trigger=0,start=0;} created;
struct Spawned {void* ability=nullptr;void* projectile=nullptr;Name bone{};} spawned;
template<class T>void Put(void* p,size_t offset,T value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void Label(void* object,int index,const char* name){Put(object,0x18,index);Put(object,0x1C,0);strcpy_s(reinterpret_cast<char*>(entries[index]+12),180,name);}
void Register(int index,void* object,int serial){Put(object,0xC,index);Put(f.items,size_t(index)*24,reinterpret_cast<uintptr_t>(object));Put(f.items,size_t(index)*24+16,serial);}
void* CreateNative(void* sequence,Name slot,float in,float out,float rate,int loops,float trigger,float start){
    ++creates;created={sequence,slot,in,out,rate,loops,trigger,start};if(expireCreate)Put(f.items,4*24+16,51);if(gesture)Put(f.replacement,0x90,.5666666627f);return createOK?f.replacement:nullptr;
}
void PlayNative(void* ability,void* montage,float rate,Name section){
    ++plays;playedMontage=montage;playedRate=rate;playedSection=section;Put(ability,0x1D8,reinterpret_cast<uintptr_t>(f.instance));
    if(reenter){reenter=false;Check(HandlePlay(PlayNative,ability,f.montage,rate,section),"reentrant same owned cast stays handled");}
    if(expire)Put(f.items,4*24+16,51);
    if(changeOwner)Put(ability,0x1B0,reinterpret_cast<uintptr_t>(f.controller));
    if(throwPlay)throw std::runtime_error("native playback error");
}
bool PlayingNative(void* instance){++playingCalls;Check(instance==f.instance,"exact native instance supplied to IsPlaying");return playing;}
void FinishNative(void* ability,unsigned char cause){++finishes;Check(ability==f.ability&&cause==0,"only current cast receives normal native finish");}
void SpawnNative(void* ability,void* projectile,Name bone){++spawns;spawned={ability,projectile,bone};Check(playingCalls==1,"spawn only follows confirmed native playback");}
void Fresh(){
    f={};owner={};gesture=false;gameThread=local=createOK=playing=true;reenter=expire=expireCreate=changeOwner=throwPlay=false;
    creates=plays=spawns=finishes=playingCalls=0;playedRate=0;playedMontage=nullptr;playedSection={};created={};spawned={};
    g_ready=true;g_failed=false;g_handling=nullptr;g_log=nullptr;g_events=0;
    g_create=CreateNative;g_finish=FinishNative;g_playing=PlayingNative;g_spawn=SpawnNative;
    const char* labels[]={"BlueprintGeneratedClass","AnimSequence","AnimMontage","War_Blueprint_C","WarAnim_Blueprint_C"};
    for(int i=0;i<5;++i)Label(f.classes[i],i+1,labels[i]);
    Label(f.power,6,"MayhemAbility_SpawnCaltrops_Blueprint_C");Label(f.powerPackage,7,"/Game/Blueprints/Abilities/Caltrops/MayhemAbility_SpawnCaltrops_Blueprint");
    Label(f.idle,8,"War_Idle");Label(f.idlePackage,9,"/Game/Characters/Heroes/Hero_War/Animation/War_Idle");Label(f.skeleton,10,"War_Mesh_Skeleton");
    Label(f.projectile,11,"MayhemCaltropSpawnerProjectile_Base_Blueprint_C");Label(f.projectilePackage,12,"/Game/Blueprints/Projectiles/MayhemCaltropSpawnerProjectile_Base_Blueprint");
    Label(f.montage,13,"Strife_Caltrops_Ground_F_Montage");Label(f.montagePackage,14,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Caltrops_Ground_F_Montage");Label(f.strifeSkeleton,15,"Strife_Skeleton");
    strcpy_s(reinterpret_cast<char*>(entries[16]+12),180,"Bone_War_Pelvis");strcpy_s(reinterpret_cast<char*>(entries[17]+12),180,"TPS");
    owner.index=0;owner.localPlayer=123;owner.world=456;owner.controller=reinterpret_cast<uintptr_t>(f.controller);owner.character=owner.pawn=reinterpret_cast<uintptr_t>(f.actor);
    Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put<unsigned char>(f.actor,0x118,3);Put<unsigned char>(f.actor,0xC18,2);Put(f.actor,0x348,owner.controller);Put(f.controller,0x350,owner.pawn);
    Put(f.actor,0x388,reinterpret_cast<uintptr_t>(f.movement));Put(f.movement,0xE8,owner.pawn);Put<unsigned char>(f.movement,0x1B0,1);
    Put(f.actor,0x380,reinterpret_cast<uintptr_t>(f.mesh));Put(f.mesh,0xE8,owner.pawn);Put(f.mesh,0x718,reinterpret_cast<uintptr_t>(f.anim));
    Put(f.anim,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(f.anim,0x28,reinterpret_cast<uintptr_t>(f.skeleton));Put(f.mesh,0x5A0,reinterpret_cast<uintptr_t>(f.meshAsset));
    Put(f.meshAsset,0x190,Array{reinterpret_cast<uintptr_t>(f.bones),2,2});Put(f.bones,12,Name{16,0});Put(f.bones,20,0);
    Put(f.power,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.power,0x20,reinterpret_cast<uintptr_t>(f.powerPackage));Put(f.power,0xF8,reinterpret_cast<uintptr_t>(f.cdo));
    Put(f.cdo,0x10,reinterpret_cast<uintptr_t>(f.power));Put(f.cdo,0x1E0,reinterpret_cast<uintptr_t>(f.projectile));
    Put(f.ability,0x10,reinterpret_cast<uintptr_t>(f.power));Put(f.ability,0x48,owner.pawn);Put(f.ability,0x1B0,owner.pawn);Put<unsigned char>(f.ability,0xD8,1);Put(f.ability,0x1E0,reinterpret_cast<uintptr_t>(f.projectile));
    Put(f.projectile,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.projectile,0x20,reinterpret_cast<uintptr_t>(f.projectilePackage));
    Put(f.idle,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.idle,0x20,reinterpret_cast<uintptr_t>(f.idlePackage));Put(f.idle,0x38,reinterpret_cast<uintptr_t>(f.skeleton));Put(f.idle,0x90,5.3333334923f);Put(f.idle,0x94,1.0f);
    Put(f.montage,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.montage,0x20,reinterpret_cast<uintptr_t>(f.montagePackage));Put(f.montage,0x38,reinterpret_cast<uintptr_t>(f.strifeSkeleton));Put(f.montage,0x90,2.3333332539f);Put(f.montage,0x94,1.0f);
    Put(f.montage,0x158,Array{reinterpret_cast<uintptr_t>(f.tracks),1,1});Put(f.tracks,0,Name{17,0});Put(f.replacement,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.replacement,0x38,reinterpret_cast<uintptr_t>(f.skeleton));Put(f.replacement,0x90,5.3333334923f);
    objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(reinterpret_cast<void*>(g_base),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(reinterpret_cast<void*>(g_base),0x3959694,640);
    Register(1,f.power,10);Register(2,f.idle,20);Register(3,f.actor,30);Register(4,f.ability,50);
    g_power={reinterpret_cast<uintptr_t>(f.power),1,10};g_idle={reinterpret_cast<uintptr_t>(f.idle),2,20};
    for(auto& choice:dg::runtime::Get().wrathSlots[1])choice=0;
}
bool Cast(float rate=1,Name section={0,0}){return HandlePlay(PlayNative,f.ability,f.montage,rate,section);}
void Run(){
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));
    for(unsigned i=0;i<24;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
    Fresh();uintptr_t cls=0;const auto actor=std::to_array(f.actor);const auto cdo=std::to_array(f.cdo);const auto idle=std::to_array(f.idle);const auto original=std::to_array(f.montage);
    Check(Prepare(f.actor,cls)&&cls==reinterpret_cast<uintptr_t>(f.power)&&!creates&&!spawns,"preparation validates own class, idle and spawner without creating anything");
    reenter=true;Check(Cast(1.25f)&&creates==1&&plays==1&&spawns==1&&!finishes&&!g_handling,"one native creation/play/spawn, including reentrant native playback");
    Check(created.sequence==f.idle&&created.slot==Name{17,0}&&created.in==.1f&&created.out==.1f&&created.rate==1&&created.loops==1&&created.trigger==-1&&created.start==0,"all eight native dynamic-montage arguments match contract");
    Check(playedMontage==f.replacement&&playedSection==Name{0,0}&&std::fabs(5.3333334923f/playedRate-2.3333332539f/1.25f)<.0001f,"own War montage preserves effective native cast duration and requested rate");
    Check(spawned.ability==f.ability&&spawned.projectile==f.projectile&&spawned.bone==Name{16,0},"three-argument native spawn uses current ability, own default class and War pelvis FName");
    Check(!std::memcmp(f.actor,actor.data(),actor.size())&&!std::memcmp(f.cdo,cdo.data(),cdo.size())&&!std::memcmp(f.idle,idle.data(),idle.size())&&!std::memcmp(f.montage,original.data(),original.size()),"no actor, source, CDO, input or resource writes");
    Fresh();owner.index=1;owner.localPlayer=789;Check(Cast()&&spawns==1&&spawned.ability==f.ability,"same ownership contract permits P2 without borrowing P1; runtime still untested");
    Fresh();g_handling=f.controller;Check(Cast()&&spawns==1&&g_handling==f.controller,"another cast's scope cannot suppress this cast and is restored after nested handling");g_handling=nullptr;
    Fresh();Label(f.montage,13,"Strife_Caltrops_Ground_B_Montage");Label(f.montagePackage,14,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Caltrops_Ground_B_Montage");Check(Cast()&&spawns==1,"backward ground montage keeps the base spawner contract");
    for(unsigned fail=0;fail<11;++fail){
        Fresh();
        if(fail==0)g_ready=false;
        if(fail==1)gameThread=false;
        if(fail==2){Label(f.classes[3],4,"Strife_Blueprint_C");Put<unsigned char>(f.actor,0xC18,1);}
        if(fail==3)owner.kind=dg::local_players::PoseKind::Horse;
        if(fail==4)owner.kind=dg::local_players::PoseKind::Ultimate;
        if(fail==5)Put<unsigned char>(f.actor,0x118,2);
        if(fail==6)Put(f.controller,0x350,uintptr_t(0));
        if(fail==7)Put(f.ability,0x48,uintptr_t(0));
        if(fail==8)Put<unsigned char>(f.ability,0xD8,5);
        if(fail==9)Put<uint32_t>(f.ability,8,0x10);
        if(fail==10)local=false;
        Check(!Cast()&&!plays&&!creates&&!spawns&&!finishes,"native Strife, nonlocal, transformed, expired or unrelated paths are not intercepted");
    }
    Fresh();Put<unsigned char>(f.movement,0x1B0,3);cls=123;
    Check(!Prepare(f.actor,cls)&&cls==123&&!creates&&!spawns,"airborne War request is rejected before native resolve/spending; output preserved");
    for(unsigned fail=0;fail<8;++fail){
        Fresh();
        if(fail==0)createOK=false;
        if(fail==1)playing=false;
        if(fail==2)Put(f.replacement,0x38,uintptr_t(0));
        if(fail==3)Put(f.bones,12,Name{17,0});
        if(fail==4)Label(f.projectilePackage,12,"/Game/Other");
        if(fail==5)Label(f.montage,13,"Strife_Caltrops_Air_F_Montage");
        if(fail==6)Put(f.idle,0x80,Array{1,1,1});
        if(fail==7)Put(f.replacement,0x90,std::numeric_limits<float>::quiet_NaN());
        Check(Cast()&&finishes==1&&!spawns&&g_failed&&Status()==0&&!g_handling,"unknown montage/bone, failed factory/playback stops only owned transfer through native normal finish");
        cls=123;Check(!Prepare(f.actor,cls)&&cls==123,"stopped transfer blocks next request without cost or output mutation");
    }
    Fresh();Check(Cast(std::numeric_limits<float>::quiet_NaN())&&finishes==1&&!spawns,"invalid rate never reaches constructor or spawn");
    Fresh();Check(Cast(1,{17,0})&&finishes==1&&!spawns,"unknown section is not passed into another skeleton");
    Fresh();expire=true;Check(Cast()&&plays==1&&!spawns&&!finishes,"cast deletion during native playback causes no stale finish or spawn call");
    Fresh();expireCreate=true;Check(Cast()&&creates==1&&!plays&&!spawns&&!finishes,"cast deletion during factory causes no stale play, finish or spawn call");
    Fresh();changeOwner=true;Check(Cast()&&plays==1&&!spawns&&!finishes&&g_failed,"owner change during playback cannot spawn for or finish another player's cast");
    Fresh();throwPlay=true;bool threw=false;try{Cast();}catch(const std::runtime_error&){threw=true;}
    Check(threw&&!g_handling&&!spawns&&!finishes,"native C++ exception propagates while scope restores");
    Fresh();g_power={};g_idle={};
    ObserveObject(reinterpret_cast<uintptr_t>(f.power),3);Check(!g_power,"shared discovery rejects the wrong object kind");
    ObserveObject(reinterpret_cast<uintptr_t>(f.power),1);Check(g_power&&!g_idle&&Status()==1,"one typed discovery event captures only its matching class");
    ObserveObject(reinterpret_cast<uintptr_t>(f.idle),3);Check(AssetsReady()&&Status()==2,"typed sequence event completes discovery without another GUObject pass");
    Put(f.items,2*24+16,999);Check(InvalidateAssets()&&!InvalidateAssets(),"asset serial change invalidates once");
    Fresh();gesture=true;Check(Cast()&&creates==1&&plays==1&&spawns==1&&!finishes&&created.sequence==f.strifeSkeleton&&playedRate==1.f,"receiver gesture uses authored speed, one native spawn and successful lifecycle");
    Fresh();Shutdown();Check(Status()==0&&!Prepare(f.actor,cls)&&!Cast(),"shutdown returns all paths to unavailable/native forwarding");
}
}
namespace dg::foreign_montage {
void StartSounds(uintptr_t,void*){}bool CopyLaterSounds(uintptr_t,void*){return true;}uintptr_t CastingSequence(uintptr_t fallback,uintptr_t,float& length){if(gesture){length=.5666666627f;return reinterpret_cast<uintptr_t>(f.strifeSkeleton);}return fallback;}}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}}
namespace dg::local_players {bool ResolveActor(void* actor,Context& c){c=owner;return local&&reinterpret_cast<uintptr_t>(actor)==owner.character;}}
int main(){try{Run();std::cout<<"Caltrops ownership, native ABI, compatible montage, source isolation and failure lifecycle passed\n";return 0;}
catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
