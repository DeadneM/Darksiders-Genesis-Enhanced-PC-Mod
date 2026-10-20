#include "../src/DashCombatFeature.cpp"
#include "RuntimeSettings.h"
#include <array>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
namespace {
struct Player {
    alignas(16) unsigned char actor[0x1100]{},ability[0x240]{},component[0x110]{},movement[0x200]{},resource[0x400]{};
    unsigned char pistols[0x6C0]{},weapons[0x120]{};
    void* equipped[6]{};
    void* active[1]{};
};
Player players[2];
unsigned char cls[0x80]{};
void* tree[1]{cls+0x30};
uintptr_t table[0x300/8]{};
uintptr_t actionTable[4]{};uint64_t nativeActions[2]{};
dg::local_players::Context owners[2]{};
dg::local_players::Pose poses[2]{};
unsigned moveRequests=0;void* moveAbility=nullptr;unsigned char moveCause=255;bool moveAllowed=false;
bool gameThread=true,paused=false,cinema=false,endDuringTick=false;
unsigned nativeCalls=0;float actualDt=0;
unsigned aimStarts=0;void* aimOwner=nullptr;
unsigned char aimClass[0x80]{},nameEntries[32][100]{};
void* namePointers[32]{};void* nameChunks[1]{namePointers};
template<class T> void Put(void* p,size_t o,T value){std::memcpy(static_cast<unsigned char*>(p)+o,&value,sizeof(value));}
template<class T> T Get(void* p,size_t o){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+o,sizeof(value));return value;}
void Check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
void StartAim(void* weapon){++aimStarts;aimOwner=Get<void*>(weapon,0xE8);}
void AllowMove(void* ability,unsigned char cause,bool allowed){++moveRequests;moveAbility=ability;moveCause=cause;moveAllowed=allowed;}
unsigned char ActionState(void* p,unsigned char action){
    const auto actor=static_cast<unsigned char*>(p)-0x8D0;
    return (nativeActions[actor==players[1].actor?1:0]&(uint64_t(1)<<action))?1:0;
}
void NativeTick(void* a,float dt){
    ++nativeCalls;actualDt=dt;
    Put(a,0xDC,Get<float>(a,0xDC)+dt);
    Put(a,0x224,Get<float>(a,0x224)-dt);
    if(endDuringTick)Put<unsigned char>(a,0xD8,5);
}
void Fresh(){
    for(auto& p:players)p={};
    owners[0]={};owners[1]={};nativeCalls=0;
    gameThread=true;paused=cinema=endDuringTick=false;
    dg::dash_combat::g_ready=true;dg::dash_combat::g_tick=NativeTick;
    dg::dash_combat::g_resume[0]={};dg::dash_combat::g_resume[1]={};
    dg::dash_combat::g_recovery[0]={};dg::dash_combat::g_recovery[1]={};
    dg::dash_combat::g_recoveryWorld[0]=dg::dash_combat::g_recoveryWorld[1]=0;
    dg::dash_combat::g_recoveryReady=false;dg::runtime::Get().dashMovementRecoveryEnabled=false;
    moveRequests=0;moveAbility=nullptr;moveCause=255;moveAllowed=false;poses[0]={};poses[1]={};
    dg::dash_combat::g_combatReady=false;aimStarts=0;aimOwner=nullptr;
    nativeActions[0]=nativeActions[1]=0;actionTable[3]=reinterpret_cast<uintptr_t>(&ActionState);
    Put(cls,0x30,tree+0);Put(cls,0x38,0);
    table[0x2E0/8]=dg::dash_combat::g_base+0x5B93F0;table[0x2A8/8]=dg::dash_combat::g_base+0x59C110;
    for(int i=0;i<2;++i){
        auto& p=players[i];p.active[0]=p.ability;
        Put(p.actor,0xC70,static_cast<void*>(p.component));Put<unsigned char>(p.actor,0x118,3);
        Put(p.actor,0x8D0,actionTable+0);
        Put<unsigned char>(p.actor,0xC18,static_cast<unsigned char>(i+1));Put(p.actor,0x388,static_cast<void*>(p.movement));
        Put(p.actor,0xEA0,static_cast<void*>(p.resource));Put(p.resource,0x10,static_cast<void*>(cls));
        Put(p.actor,0xC78,static_cast<void*>(p.weapons));Put(p.actor,0x1038,static_cast<void*>(p.pistols));
        Put(p.weapons,0xE8,static_cast<void*>(p.actor));Put(p.weapons,0x108,p.equipped+0);
        Put(p.weapons,0x110,6);Put(p.weapons,0x114,6);p.equipped[5]=p.pistols;
        Put(p.pistols,0x10,static_cast<void*>(cls));Put(p.pistols,0xE8,static_cast<void*>(p.actor));
        Put(p.pistols,0x618,static_cast<void*>(aimClass));
        Put<unsigned char>(p.resource,0x118,3);Put(p.resource,0x37C,100.0f);
        Put<unsigned char>(p.movement,0x1B0,6);Put<unsigned char>(p.movement,0x1B1,5);
        Put(p.component,0xE8,static_cast<void*>(p.actor));Put(p.component,0xF8,p.active+0);
        Put(p.component,0x100,1);Put(p.component,0x104,1);
        Put(p.ability,0,table+0);Put(p.ability,0x10,static_cast<void*>(cls));Put(p.ability,0x48,static_cast<void*>(p.actor));
        Put(p.ability,0x1B0,static_cast<void*>(p.actor));Put(p.ability,0x1D8,reinterpret_cast<void*>(0x1234));
        Put<unsigned char>(p.ability,0xD8,2);Put(p.ability,0xDC,0.4f);Put(p.ability,0x224,0.08f);Put(p.ability,0x228,0.1f);
        owners[i].index=i;owners[i].localPlayer=0x1000+uintptr_t(i);owners[i].character=reinterpret_cast<uintptr_t>(p.actor);
    }
}
void Tick(int i,ULONGLONG now,float dt=0.016f){dg::dash_combat::Tick(players[i].ability,dt,now);}
void Run(){
    std::vector<unsigned char> image(0x3960000);
    dg::dash_combat::g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x3810F90,static_cast<void*>(cls));
    Put(image.data(),0x3810268,static_cast<void*>(cls));
    Put(image.data(),0x3811D20,static_cast<void*>(cls));
    for(int i=0;i<32;++i)namePointers[i]=nameEntries[i];
    Put(image.data(),0x3807E38,static_cast<void*>(cls));
    Put(image.data(),0x3955198,nameChunks+0);Put(aimClass,0x18,1);
    const char weaponName[]="MayhemWeapon_DualPistols_Blueprint_C";
    const char aimName[]="MayhemAbility_Strife_DualPistols_Aim_Start_Ability_C";
    std::memcpy(nameEntries[0]+0xC,weaponName,sizeof(weaponName));
    std::memcpy(nameEntries[1]+0xC,aimName,sizeof(aimName));
    Fresh();const auto actorBefore=std::to_array(players[0].actor);
    Tick(0,100);Check(nativeCalls==1&&actualDt==.016f,"complete native tick runs once before observation");
    Tick(0,116);Check(nativeCalls==2&&Get<unsigned char>(players[0].ability,0xD8)==2,
        "elapsed deceleration never triggers early native completion");
    Check(std::memcmp(players[0].actor,actorBefore.data(),actorBefore.size())==0,"observer writes no actor movement or aim flags");
    endDuringTick=true;Tick(0,132);Check(Get<unsigned char>(players[0].ability,0xD8)==5,"native completion wins without a mod finish call");
    // Exercise the actual live callback with the captured missing-ability
    // state. No look getter is invoked anywhere in this sequence.
    for(bool tps:{false,true}){
        Fresh();dg::runtime::Get().thirdPersonEnabled=tps;
        dg::dash_combat::g_combatReady=true;dg::dash_combat::g_actionQuery=ActionState;
        dg::dash_combat::g_startAim=StartAim;
        nativeActions[0]=uint64_t(1)<<31;
        Tick(0,GetTickCount64());dg::dash_combat::OnLivePlayer(players[0].actor);
        Check(!aimStarts,"active dash wins over held aim");
        Put(players[0].component,0x100,0);Put<unsigned char>(players[0].movement,0x1B0,1);
        dg::dash_combat::OnLivePlayer(players[0].actor);
        Check(aimStarts==1&&aimOwner==players[0].actor,"live callback requests general native AIM with an empty ability/action-context stack and no look getter, native/TPS views");
        dg::dash_combat::OnLivePlayer(players[0].actor);Check(aimStarts==1,"native request/rejection cannot start abilities repeatedly");
    }
    Fresh();dg::dash_combat::g_combatReady=true;dg::dash_combat::g_actionQuery=ActionState;
    dg::dash_combat::g_startAim=StartAim;
    nativeActions[0]=uint64_t(1)<<31;Tick(0,GetTickCount64());
    Put(players[0].component,0x100,0);Put<unsigned char>(players[0].movement,0x1B0,1);nativeActions[0]=0;
    dg::dash_combat::OnLivePlayer(players[0].actor);
    Check(!aimStarts&&!dg::dash_combat::g_resume[0].seen,"release discards old intent before a later unrelated aim press");
    Fresh();dg::dash_combat::g_combatReady=true;dg::dash_combat::g_actionQuery=ActionState;dg::dash_combat::g_startAim=StartAim;
    nativeActions[0]=uint64_t(1)<<31;Tick(0,GetTickCount64());
    Put(players[0].component,0x100,0);Put<unsigned char>(players[0].movement,0x1B0,1);
    Put(players[0].actor,0x1038,static_cast<void*>(nullptr));dg::dash_combat::OnLivePlayer(players[0].actor);
    Check(!aimStarts&&!dg::dash_combat::g_resume[0].seen,"unrecognized or mismatching main pistol stops the repair for this dash");
    Fresh();dg::dash_combat::g_combatReady=true;dg::dash_combat::g_actionQuery=ActionState;dg::dash_combat::g_startAim=StartAim;
    Put<unsigned char>(players[1].actor,0xC18,1);nativeActions[1]=uint64_t(1)<<31;
    Tick(1,GetTickCount64());Put(players[1].component,0x100,0);Put<unsigned char>(players[1].movement,0x1B0,1);
    dg::dash_combat::OnLivePlayer(players[0].actor);Check(!aimStarts,"P1 service cannot consume P2's missing aim");
    dg::dash_combat::OnLivePlayer(players[1].actor);Check(aimStarts==1&&aimOwner==players[1].actor,"P2 restores only its own registered pistols");
    // Real hooks/predicates with synthetic native objects. No audio or native
    // engine functions are called; only the forwarding boundary is mocked.
    unsigned char notify[0x90]{},notifyClass[0x80]{},mesh[0x100]{},sequence[0x80]{},asset[0x80]{};
    unsigned char special[0x6C0]{},specialClass[0x80]{},forced[0x80]{};
    auto label=[](void* object,int index,const char* value){
        Put(object,0x18,index);Put(object,0x1C,0);std::memset(nameEntries[index],0,100);
        std::memcpy(nameEntries[index]+0xC,value,std::strlen(value)+1);
    };
    auto setup=[&](int player,const char* seq="Strife_Aim_Dash_Fire_F"){
        Fresh();dg::dash_combat::g_combatReady=true;dg::dash_combat::g_presentationReady=true;
        dg::dash_combat::g_actionQuery=ActionState;
        Put<unsigned char>(players[player].actor,0xC18,1);
        label(notifyClass,2,"AnimNotify_PlaySound");label(sequence,3,seq);
        label(specialClass,4,"MayhemStaticShot_Blueprint_C");label(asset,5,"char_strife_gunfire_Cue");
        label(forced,6,"MayhemProjectile_Standard_Blueprint_C");
        Put(specialClass,0x30,tree+0);Put(specialClass,0x38,0);
        Put(notifyClass,0x30,tree+0);Put(notifyClass,0x38,0);
        Put(special,0x10,static_cast<void*>(specialClass));Put(special,0xE8,static_cast<void*>(players[player].actor));
        players[player].equipped[2]=special;players[player].equipped[3]=special;
        Put(notify,0x10,static_cast<void*>(notifyClass));Put(notify,0x20,static_cast<void*>(sequence));
        Put(notify,0x38,static_cast<void*>(asset));Put(notify,0x50,static_cast<void*>(forced));
        Put(mesh,0xE8,static_cast<void*>(players[player].actor));nativeActions[player]=uint64_t(1)<<18;
    };
    for(int player:{0,1})for(bool tps:{false,true})
    for(const char* seq:{"Strife_Aim_Dash_Fire_F","Strife_Aim_Dash_Fire_B","Strife_Aim_Dash_Fire_L","Strife_Aim_Dash_Fire_R"}){
        setup(player,seq);dg::runtime::Get().thirdPersonEnabled=tps;
        const auto actorBefore=std::to_array(players[player].actor);
        const auto abilityBefore=std::to_array(players[player].ability);
        Check(dg::dash_combat::SkipPresentation(notify,mesh,sequence,true),"only special-ammo standard gunfire is skipped for each owned view/direction");
        label(notifyClass,2,"AnimNotify_PlayParticleEffect");label(asset,5,"VFX_strife_muzzleFlash");
        Check(dg::dash_combat::SkipPresentation(notify,mesh,sequence,false),"standard muzzle flash is skipped under the same predicate");
        label(asset,5,"VFX_strife_bulletCasings");
        Check(dg::dash_combat::SkipPresentation(notify,mesh,sequence,false),"standard casings share the exact scope");
        nativeActions[player]=uint64_t(1)<<20;
        Check(dg::dash_combat::SkipPresentation(notify,mesh,sequence,false),"secondary action uses the same presentation rule");
        Check(std::memcmp(players[player].actor,actorBefore.data(),actorBefore.size())==0&&
            std::memcmp(players[player].ability,abilityBefore.data(),abilityBefore.size())==0,
            "presentation filter never changes actor/ability lifecycle, aim or montage");
    }
    for(const char* sound:{"char_strife_dash","char_strife_vo_effort_dash","Footstep","UnknownSound"}){
        setup(0);label(asset,5,sound);Check(!dg::dash_combat::SkipPresentation(notify,mesh,sequence,true),"dash/effort/other sounds stay native");
    }
    setup(0);label(specialClass,4,"MayhemBeamShot_Blueprint_C");
    Check(dg::dash_combat::SkipPresentation(notify,mesh,sequence,true),
        "live V1.58 reproduction: primary Beam Shot is special ammo, not an unknown request");
    dg::dash_combat::g_notify=+[](void*,void*,void*){++nativeCalls;};
    Put(notify,0x10,static_cast<void*>(cls));Put(notify,0x50,static_cast<void*>(forced));
    dg::dash_combat::HookSpawnNotify(notify,mesh,sequence);
    Check(nativeCalls==0,"same equipped Beam Shot also suppresses the forced Standard projectile consumer");
    setup(0);label(specialClass,4,"AnotherRegisteredRangedSubclass");
    Check(dg::dash_combat::SkipPresentation(notify,mesh,sequence,true),
        "valid native ranged-slot subclasses are handled without another weapon-name exception");
    Check(dg::dash_combat::EquippedAmmo(players[0].actor,5)==dg::dash_combat::Ammo::Unknown,
        "general classification cannot extend to melee, gear or the general AIM slot");
    for(const char* effect:{"DashSmoke","AbilityEffect","UnknownParticle"}){
        setup(0);label(notifyClass,2,"AnimNotify_PlayParticleEffect");label(asset,5,effect);
        Check(!dg::dash_combat::SkipPresentation(notify,mesh,sequence,false),"unrelated effects stay native");
    }
    auto preserved=[&](const std::function<void()>& alter,const char* why){
        setup(0);alter();Check(!dg::dash_combat::SkipPresentation(notify,mesh,sequence,true),why);
    };
    preserved([&]{label(specialClass,4,"MayhemStandardShot_Blueprint_C");},"normal ammo and its Hotstreak keep gunfire");
    preserved([&]{Put(specialClass,0x38,-1);},"an untyped object in a ranged slot stays native");
    preserved([&]{nativeActions[0]=0;},"no held native fire action stays native");
    preserved([&]{label(sequence,3,"OtherFireAnimation");},"ordinary shooting and other dashes stay native");
    preserved([&]{Put(notify,0x20,static_cast<void*>(asset));},"notify must belong to that exact animation");
    preserved([&]{Put(mesh,0xE8,static_cast<void*>(asset));},"foreign/nonlocal mesh stays native");
    preserved([&]{players[0].active[0]=nullptr;},"ended dash stays native");
    preserved([&]{owners[0].kind=dg::local_players::PoseKind::Horse;},"mounted presentation stays native");
    preserved([&]{owners[0].kind=dg::local_players::PoseKind::Ultimate;},"Ultimate presentation stays native");
    preserved([&]{paused=true;},"pause stays native");preserved([&]{cinema=true;},"cinematic stays native");
    preserved([&]{gameThread=false;},"other threads stay native");
    preserved([&]{dg::dash_combat::g_presentationReady=false;},"unavailable hooks leave native presentation");
    preserved([&]{Put(notify,0x10,static_cast<void*>(specialClass));},"unknown notify class stays native");
    setup(0);dg::dash_combat::g_sound=+[](void*,void*,void*){++nativeCalls;};
    dg::dash_combat::HookSoundNotify(notify,mesh,sequence);Check(nativeCalls==0,"sound hook actually skips its native consumer");
    label(asset,5,"char_strife_dash");dg::dash_combat::HookSoundNotify(notify,mesh,sequence);
    Check(nativeCalls==1,"sound hook forwards an unrelated sound exactly once");
    setup(0);dg::dash_combat::g_particle=+[](void*,void*,void*){++nativeCalls;};
    label(notifyClass,2,"AnimNotify_PlayParticleEffect");label(asset,5,"VFX_strife_muzzleFlash");
    dg::dash_combat::HookParticleNotify(notify,mesh,sequence);Check(nativeCalls==0,"effect hook actually skips its native consumer");
    label(asset,5,"DashSmoke");dg::dash_combat::HookParticleNotify(notify,mesh,sequence);
    Check(nativeCalls==1,"effect hook forwards an unrelated effect exactly once");
    setup(0);dg::dash_combat::g_notify=+[](void*,void*,void*){++nativeCalls;};
    dg::dash_combat::HookSpawnNotify(notify,mesh,sequence);Check(nativeCalls==0,"shared predicate preserves forced-standard projectile filtering");
    label(specialClass,4,"MayhemStandardShot_Blueprint_C");dg::dash_combat::HookSpawnNotify(notify,mesh,sequence);
    Check(nativeCalls==1,"standard projectile notify still forwards exactly once");
    // Exercise the actual optional wrapper, including P1 Strife/P2 War and
    // both camera modes. The native permission call is a recorder; it cannot
    // hide a forced finish, actor write or repeated request in this fixture.
    auto enableRecovery=[](){
        dg::dash_combat::g_recoveryReady=true;dg::dash_combat::g_combatReady=true;
        dg::dash_combat::g_allowMove=AllowMove;dg::dash_combat::g_actionQuery=ActionState;
        dg::runtime::Get().dashMovementRecoveryEnabled=true;
        table[0x2A0/8]=reinterpret_cast<uintptr_t>(&AllowMove);
        table[0x280/8]=dg::dash_combat::g_base+0x5B2CE0;
        nativeActions[0]=nativeActions[1]=uint64_t(1)<<29;
    };
    for(bool tps:{false,true})for(int player=0;player<2;++player){
        Fresh();enableRecovery();dg::runtime::Get().thirdPersonEnabled=tps;
        const auto beforeActor=std::to_array(players[player].actor),beforeMovement=std::to_array(players[player].movement);
        for(unsigned i=0;i<12;++i)Tick(player,100+i*16);
        Check(moveRequests==1&&moveAbility==players[player].ability&&moveCause==1&&moveAllowed,
            "native/TPS and either local owner request native MOVE, exactly once after observed movement end");
        Check(nativeCalls==12&&Get<unsigned char>(players[player].ability,0xD8)==2&&Get<float>(players[player].ability,0x228)==.1f,
            "full native tick, active lifecycle and duration remain intact; no forced finish hidden in the wrapper");
        Check(std::memcmp(players[player].actor,beforeActor.data(),beforeActor.size())==0&&
              std::memcmp(players[player].movement,beforeMovement.data(),beforeMovement.size())==0,
            "new candidate never writes actor movement, facing, aim, input, velocity or movement flags");
    }
    Fresh();enableRecovery();
    for(unsigned i=0;i<12;++i){poses[0].x=float(i);Tick(0,100+i*16);}
    Check(!moveRequests,"continued root displacement prevents native interruption even with zero reported velocity");
    Fresh();enableRecovery();paused=true;
    for(unsigned i=0;i<12;++i)Tick(0,100+i*16);
    Check(!moveRequests,"paused native world cannot receive a recovery request");
    Fresh();enableRecovery();table[0x280/8]=0;
    for(unsigned i=0;i<12;++i)Tick(0,100+i*16);
    Check(!moveRequests,"unknown native interruption implementation fails open");
}
}
namespace dg::native_tps {
bool OnGameThread(){return gameThread;}
bool IsWorldPaused(void*){return paused;}
bool CinematicActive(unsigned){return cinema;}
}
namespace dg::local_players {
bool ResolveActor(void* a,Context& result){for(int i=0;i<2;++i)if(a==players[i].actor){result=owners[i];return true;}return false;}
bool ReadPose(const Context& owner,Pose& result){for(int i=0;i<2;++i)if(owner.character==reinterpret_cast<uintptr_t>(players[i].actor)){result=poses[i];return true;}return false;}
}
int main(){try{Run();std::cout<<"Native dash observation, held-aim recovery and presentation boundaries passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
