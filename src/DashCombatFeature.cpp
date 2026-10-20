#include "DashCombatFeature.h"
#include "LocalPlayerOwnership.h"
#include "NativeTpsFeature.h"
#include "DashCombatPolicy.h"
#include "DashRecoveryPolicy.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <MinHook.h>
#include <atomic>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace dg::dash_combat {
namespace {
using TickFn=void(*)(void*,float);
TickFn g_tick=nullptr;
uintptr_t g_base=0;
LogFn g_log=nullptr;
std::atomic_bool g_ready{false};
dash_combat::AimResume g_resume[2]{};
using NotifyFn=void(*)(void*,void*,void*);
NotifyFn g_notify=nullptr,g_sound=nullptr,g_particle=nullptr;
std::atomic_bool g_presentationReady{false};
std::atomic_uint32_t g_soundSkipped{0},g_particleSkipped{0};
std::atomic_bool g_combatReady{false};
std::atomic_uint32_t g_resumed{0},g_suppressed{0},g_unavailable{0};
using ActionQueryFn=unsigned char(*)(void*,unsigned char);
using StartAimFn=void(*)(void*);
ActionQueryFn g_actionQuery=nullptr;
StartAimFn g_startAim=nullptr;
thread_local bool g_resuming=false;
using MovePermissionFn=void(*)(void*,unsigned char,bool);
std::atomic_bool g_recoveryReady{false};
std::atomic_uint g_recoveryRequests{0};
MovePermissionFn g_allowMove=nullptr;
MoveRecovery g_recovery[2]{};
thread_local bool g_recovering=false;
bool Readable(const void* p,size_t bytes){
    MEMORY_BASIC_INFORMATION m{};
    if(!p||!VirtualQuery(p,&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_NOACCESS|PAGE_GUARD)))return false;
    const auto offset=reinterpret_cast<uintptr_t>(p)-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&bytes<=m.RegionSize-offset;
}
template<class T> bool Read(const void* object,size_t offset,T& value){
    if(!object||reinterpret_cast<uintptr_t>(object)>UINTPTR_MAX-offset)return false;
    const auto* p=static_cast<const unsigned char*>(object)+offset;
    if(!Readable(p,sizeof(value)))return false;
    std::memcpy(&value,p,sizeof(value));return true;
}
bool Typed(void* object,uintptr_t classSlot){
    void* expected=nullptr;void* actual=nullptr;void* array=nullptr;void* item=nullptr;
    int index=-1,last=-1;
    return Read(reinterpret_cast<void*>(g_base+classSlot),0,expected)&&expected&&
        Read(object,0x10,actual)&&Read(expected,0x38,index)&&index>=0&&index<=512&&
        Read(actual,0x38,last)&&last>=index&&last<=512&&Read(actual,0x30,array)&&array&&
        Read(array,size_t(index)*8,item)&&item==static_cast<unsigned char*>(expected)+0x30;
}
bool Alive(void* actor){
    void* resource=nullptr;float health=0;unsigned char role=0;
    return Read(actor,0xEA0,resource)&&Typed(resource,0x3810268)&&
        Read(resource,0x118,role)&&role==3&&Read(resource,0x37C,health)&&
        std::isfinite(health)&&health>0;
}
bool ActiveComponent(void* ability,void* actor){
    void* component=nullptr;void* instigator=nullptr;void* owner=nullptr;void* array=nullptr;
    int count=0,capacity=0;
    // Reflected Instigator is an ACTOR, also confirmed in the recorded dash
    // captures. It is not the actor's ability component at C70.
    if(!Read(actor,0xC70,component)||!component||!Read(ability,0x48,instigator)||instigator!=actor||
       !Read(component,0xE8,owner)||owner!=actor||!Read(component,0xF8,array)||!array||
       !Read(component,0x100,count)||!Read(component,0x104,capacity)||
       count<=0||count>capacity||capacity>64)return false;
    for(int i=0;i<count;++i){void* item=nullptr;if(!Read(array,size_t(i)*8,item))return false;if(item==ability)return true;}
    return false;
}
bool Owner(void* ability,local_players::Context& owner){
    void* actor=nullptr;void* table=nullptr;uintptr_t movementTick=0,cleanup=0;
    unsigned char state=0,role=0,type=0;void* helper=nullptr;void* movement=nullptr;
    unsigned char mode=0,custom=0;
    // The base dash class also has other subclasses. Only the observed
    // decelerating dash implementation and its complete native cleanup fit
    // the fields below. Chests, mounted/Ultimate and unknown variants pass.
    return Typed(ability,0x3810F90)&&Read(ability,0,table)&&
        Read(table,0x2E0,movementTick)&&movementTick==g_base+0x5B93F0&&
        Read(table,0x2A8,cleanup)&&cleanup==g_base+0x59C110&&
        Read(ability,0xD8,state)&&state==2&&Read(ability,0x1D8,helper)&&helper&&
        Read(ability,0x1B0,actor)&&actor&&local_players::ResolveActor(actor,owner)&&
        owner.index>=0&&owner.index<2&&owner.character==reinterpret_cast<uintptr_t>(actor)&&
        owner.kind==local_players::PoseKind::Foot&&
        Read(actor,0x118,role)&&role==3&&Read(actor,0xC18,type)&&(type==1||type==2)&&Alive(actor)&&
        Read(actor,0x388,movement)&&movement&&Read(movement,0x1B0,mode)&&mode==6&&
        Read(movement,0x1B1,custom)&&custom==5&&ActiveComponent(ability,actor);
}
template<size_t N> bool NameIs(void* object,const char (&expected)[N]){
    int index=-1,number=-1;uintptr_t names=0,chunk=0,entry=0;uint32_t flags=0;
    if(!Read(object,0x18,index)||!Read(object,0x1C,number)||number||index<0||index>=2000000||
       !Read(reinterpret_cast<void*>(g_base+0x3955198),0,names)||!names||
       !Read(reinterpret_cast<void*>(names),size_t(index/16384)*8,chunk)||!chunk||
       !Read(reinterpret_cast<void*>(chunk),size_t(index%16384)*8,entry)||!entry||
       !Read(reinterpret_cast<void*>(entry),8,flags))return false;
    if(flags&1){
        std::array<wchar_t,N> text{};if(!Read(reinterpret_cast<void*>(entry),0xC,text))return false;
        for(size_t i=0;i<N;++i)if(text[i]!=wchar_t(expected[i]))return false;
        return true;
    }
    std::array<char,N> text{};
    return Read(reinterpret_cast<void*>(entry),0xC,text)&&std::memcmp(text.data(),expected,N)==0;
}
template<size_t N> bool ClassNameIs(void* object,const char (&expected)[N]){
    void* cls=nullptr;return Read(object,0x10,cls)&&NameIs(cls,expected);
}
bool CombatOwner(void* actor,local_players::Context& owner){
    unsigned char type=0,role=0;
    return actor&&local_players::ResolveActor(actor,owner)&&owner.index>=0&&owner.index<2&&
        owner.character==reinterpret_cast<uintptr_t>(actor)&&owner.kind==local_players::PoseKind::Foot&&
        Read(actor,0xC18,type)&&type==1&&Read(actor,0x118,role)&&role==3&&Alive(actor)&&
        !native_tps::IsWorldPaused(actor)&&!native_tps::CinematicActive(unsigned(owner.index));
}
bool AbilityArray(void* actor,void*& array,int& count){
    void* component=nullptr;void* reciprocal=nullptr;int capacity=0;
    return Read(actor,0xC70,component)&&component&&Read(component,0xE8,reciprocal)&&reciprocal==actor&&
        Read(component,0xF8,array)&&Read(component,0x100,count)&&Read(component,0x104,capacity)&&
        count>=0&&count<=capacity&&capacity<=64&&(!count||array);
}
bool AimActions(void* actor,bool& aim,bool& primary,bool& secondary){
    void* table=nullptr;uintptr_t query=0;
    // Recorded Strife action interface; 669D30 -> 635100 reads the native
    // action-state array. No XInput mask, key, axis or synthetic release.
    if(!g_actionQuery||!Read(actor,0x8D0,table)||!Read(table,0x18,query)||
       query!=reinterpret_cast<uintptr_t>(g_actionQuery))return false;
    auto fn=g_actionQuery;
    void* iface=static_cast<unsigned char*>(actor)+0x8D0;
    aim=fn(iface,31)!=0;primary=fn(iface,18)!=0;secondary=fn(iface,20)!=0;return true;
}
bool GroundDashActive(void* actor){
    void* array=nullptr;int count=0;
    if(!AbilityArray(actor,array,count))return false;
    for(int i=0;i<count;++i){
        void* ability=nullptr;void* table=nullptr;void* instigator=nullptr;void* character=nullptr;
        void* helper=nullptr;uintptr_t physics=0,cleanup=0;unsigned char state=0;
        if(Read(array,size_t(i)*8,ability)&&Typed(ability,0x3810F90)&&Read(ability,0,table)&&
           Read(table,0x2E0,physics)&&physics==g_base+0x5B93F0&&
           Read(table,0x2A8,cleanup)&&cleanup==g_base+0x59C110&&
           Read(ability,0xD8,state)&&state==2&&Read(ability,0x1D8,helper)&&helper&&
           Read(ability,0x48,instigator)&&instigator==actor&&
           Read(ability,0x1B0,character)&&character==actor)return true;
    }
    return false;
}
void* MainPistols(void* actor){
    // The action-context ranged getter 66A7F0 returns null when its E08
    // context stack is empty. General AIM instead belongs to DualPistolsBP:
    // reflected Strife member 1038, also registered in native weapon slot 5.
    void* component=nullptr;void* reciprocal=nullptr;void* array=nullptr;
    void* weapon=nullptr;void* reflected=nullptr;void* aimClass=nullptr;
    int count=0,capacity=0;
    if(!Read(actor,0xC78,component)||!Read(component,0xE8,reciprocal)||reciprocal!=actor||
       !Read(component,0x108,array)||!array||!Read(component,0x110,count)||
       !Read(component,0x114,capacity)||count<6||count>capacity||capacity>32||
       !Read(array,5*8,weapon)||!Read(actor,0x1038,reflected)||weapon!=reflected)return nullptr;
    return Typed(weapon,0x3811D20)&&Read(weapon,0xE8,reciprocal)&&reciprocal==actor&&
        ClassNameIs(weapon,"MayhemWeapon_DualPistols_Blueprint_C")&&
        Read(weapon,0x618,aimClass)&&
        NameIs(aimClass,"MayhemAbility_Strife_DualPistols_Aim_Start_Ability_C")?weapon:nullptr;
}
dash_combat::Ammo EquippedAmmo(void* actor,unsigned slot){
    if(slot!=2&&slot!=3)return dash_combat::Ammo::Unknown;
    void* component=nullptr;void* array=nullptr;void* weapon=nullptr;void* reciprocal=nullptr;
    int count=0,capacity=0;
    if(!Read(actor,0xC78,component)||!Read(component,0x108,array)||!array||
       !Read(component,0x110,count)||!Read(component,0x114,capacity)||count<=int(slot)||
       count>capacity||capacity>32||!Read(array,size_t(slot)*8,weapon)||
       !Typed(weapon,0x3811D20)||!Read(weapon,0xE8,reciprocal)||reciprocal!=actor)
        return dash_combat::Ammo::Unknown;
    if(ClassNameIs(weapon,"MayhemStandardShot_Blueprint_C"))return dash_combat::Ammo::Standard;
    // The caller supplies only the native primary/secondary ranged slots.
    // A typed weapon reciprocally owned and currently registered there is a
    // valid equipped ammunition source. Do not enumerate special BP names:
    // that omitted Beam Shot and cannot cover other legitimate subclasses.
    return dash_combat::Ammo::Special;
}
bool SuppressDashStandard(void* notify,void* mesh,void* animation,local_players::Context& owner){
    if(!g_combatReady.load(std::memory_order_acquire)||!native_tps::OnGameThread())return false;
    void* actor=nullptr;void* outer=nullptr;bool aim=false,primary=false,secondary=false;
    if(!Read(notify,0x20,outer)||outer!=animation||!Read(mesh,0xE8,actor)||!CombatOwner(actor,owner))return false;
    const bool sequence=NameIs(animation,"Strife_Aim_Dash_Fire_F")||NameIs(animation,"Strife_Aim_Dash_Fire_B")||
        NameIs(animation,"Strife_Aim_Dash_Fire_L")||NameIs(animation,"Strife_Aim_Dash_Fire_R");
    return sequence&&GroundDashActive(actor)&&AimActions(actor,aim,primary,secondary)&&
        dash_combat::SuppressForcedStandard(true,true,primary,EquippedAmmo(actor,2),secondary,EquippedAmmo(actor,3));
}
void HookSpawnNotify(void* notify,void* mesh,void* animation){
    void* forced=nullptr;local_players::Context owner{};
    const bool suppress=g_combatReady.load(std::memory_order_acquire)&&
        Typed(notify,0x3807E38)&&Read(notify,0x50,forced)&&NameIs(forced,"MayhemProjectile_Standard_Blueprint_C")&&
        SuppressDashStandard(notify,mesh,animation,owner);
    if(suppress){const unsigned n=++g_suppressed;if(g_log&&(n<=8||n%1000==0)){
        char line[200]{};sprintf_s(line,"Dash combat V1.57 FORCED_STANDARD_SKIPPED player=%d n=%u selectedSpecialResumesNatively=1 gameplay=EXPERIMENTAL",owner.index+1,n);g_log(line);}}
    if(!suppress&&g_notify)g_notify(notify,mesh,animation);
}
bool SkipPresentation(void* notify,void* mesh,void* animation,bool sound){
    if(!g_presentationReady.load(std::memory_order_acquire)||!native_tps::OnGameThread())return false;
    // Serialized F/B/L/R sequences have separate gunfire, flash and casing
    // notifies. Keep footsteps, dash/effort sounds, movement and montage intact.
    void* asset=nullptr;local_players::Context owner{};
    const bool type=sound?ClassNameIs(notify,"AnimNotify_PlaySound"):
        ClassNameIs(notify,"AnimNotify_PlayParticleEffect");
    if(!type||!Read(notify,0x38,asset))return false;
    const bool known=sound?NameIs(asset,"char_strife_gunfire_Cue"):
        (NameIs(asset,"VFX_strife_muzzleFlash")||NameIs(asset,"VFX_strife_bulletCasings"));
    if(!known||!SuppressDashStandard(notify,mesh,animation,owner))return false;
    const unsigned n=sound?++g_soundSkipped:++g_particleSkipped;
    if(g_log&&n<=8){char line[200]{};sprintf_s(line,
        "Dash combat V1.57 %s_SKIPPED player=%d n=%u nativeDashLifecyclePreserved=1 gameplay=EXPERIMENTAL",
        sound?"STANDARD_SOUND":"STANDARD_EFFECT",owner.index+1,n);g_log(line);}
    return true;
}
void HookSoundNotify(void* notify,void* mesh,void* animation){
    if(!SkipPresentation(notify,mesh,animation,true)&&g_sound)g_sound(notify,mesh,animation);
}
void HookParticleNotify(void* notify,void* mesh,void* animation){
    if(!SkipPresentation(notify,mesh,animation,false)&&g_particle)g_particle(notify,mesh,animation);
}
void RecoverMovement(void* ability,const local_players::Context& owner,float elapsed,ULONGLONG now){
    auto& state=g_recovery[owner.index];
    if(!runtime::Get().dashMovementRecoveryEnabled.load()||!RecoveryReady()||g_recovering||
       native_tps::IsWorldPaused(reinterpret_cast<void*>(owner.character))||native_tps::CinematicActive(unsigned(owner.index))){state={};return;}
    void* table=nullptr;uintptr_t setter=0;void* ifaceTable=nullptr;uintptr_t query=0;
    void* actor=reinterpret_cast<void*>(owner.character);void* movement=nullptr;
    float remaining=0,duration=0,velocity[3]{};uint64_t interruptMask=0;
    if(!Read(ability,0,table)||!Read(table,0x2A0,setter)||setter!=reinterpret_cast<uintptr_t>(g_allowMove)||
       !Read(actor,0x8D0,ifaceTable)||!Read(ifaceTable,0x18,query)||query!=reinterpret_cast<uintptr_t>(g_actionQuery)||
       !Read(ability,0x224,remaining)||!Read(ability,0x228,duration)||!Read(ability,0xB0,interruptMask)||
       !Read(actor,0x388,movement)||!Read(movement,0x10C,velocity)){state={};return;}
    const float speed2=velocity[0]*velocity[0]+velocity[1]*velocity[1]+velocity[2]*velocity[2];
    const bool held=g_actionQuery(static_cast<unsigned char*>(actor)+0x8D0,29)!=0;
    if(!state.Sample(owner.localPlayer,owner.character,reinterpret_cast<uintptr_t>(ability),now,
        elapsed,remaining,duration,speed2,true,held,(interruptMask&2)!=0))return;
    struct Scope {Scope(){g_recovering=true;}~Scope(){g_recovering=false;}} scope;
    // Native virtual +2A0 adds the MOVE cause, sends its event and replays the
    // real queued/held action. It owns cleanup; no EndAbility/NONE call, field
    // patch, montage cancellation, movement-mode write or synthetic input.
    g_allowMove(ability,1,true);
    const unsigned n=++g_recoveryRequests;
    if(g_log&&(n<=12||n%1000==0)){char line[200]{};sprintf_s(line,
        "Dash recovery V1.71 MOVE_PERMISSION_REQUEST player=%d n=%u decelerationEnded=1 stopped=1 nativeMoveHeld=1 oncePerDash=1 EXPERIMENTAL",owner.index+1,n);g_log(line);}
}
void Tick(void* ability,float dt,ULONGLONG now){
    // Observe only after the complete native tick. The optional movement
    // permission below never edits a timer, movement flag or montage.
    if(g_tick)g_tick(ability,dt);
    if(!native_tps::OnGameThread()||!g_ready.load(std::memory_order_acquire)||
       !std::isfinite(dt)||dt<=0||dt>0.1f)return;
    local_players::Context owner{};
    if(!Owner(ability,owner))return;
    float elapsedDash=0;
    if(Read(ability,0xDC,elapsedDash)&&std::isfinite(elapsedDash)&&elapsedDash>=0){
        g_resume[owner.index].Observe(owner.localPlayer,owner.character,
            reinterpret_cast<uintptr_t>(ability),elapsedDash,now);
        RecoverMovement(ability,owner,elapsedDash,now);
    }
}
void HookTick(void* ability,float dt){Tick(ability,dt,GetTickCount64());}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x5B72B0,{0x40,0x53,0x48,0x83,0xEC,0x30,0x0F,0x29,0x74,0x24,0x20,0x48,0x8B,0xD9,0x0F,0x28}},
        {0x5B93F0,{0x48,0x8B,0xC4,0x48,0x89,0x58,0x10,0x48,0x89,0x78,0x18,0x55,0x48,0x8D,0x68,0xA1}},
        {0x59C110,{0x40,0x57,0x48,0x83,0xEC,0x60,0x48,0xC7,0x44,0x24,0x30,0xFE,0xFF,0xFF,0xFF,0x48}},
        {0x7BBFF0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x92,0x4F,0x05,0x03,0x48,0x85}}
    };
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
        if(log)log("Dash combat V1.58 signature mismatch; native dash retained");return;
    }
    const unsigned char permission[]={0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48};
    const unsigned char interrupt[]={0x48,0x8B,0xC4,0x41,0x56,0x48,0x83,0xEC,0x70,0x48,0xC7,0x40,0xA8,0xFE,0xFF,0xFF};
    if(!std::memcmp(reinterpret_cast<void*>(g_base+0x599310),permission,sizeof(permission))&&
       !std::memcmp(reinterpret_cast<void*>(g_base+0x5B2CE0),interrupt,sizeof(interrupt))){
        g_allowMove=reinterpret_cast<MovePermissionFn>(g_base+0x599310);g_recoveryReady=true;
    }
    const auto init=MH_Initialize();if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* target=reinterpret_cast<void*>(g_base+0x5B72B0);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookTick),reinterpret_cast<void**>(&g_tick))!=MH_OK)return;
    if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_tick=nullptr;return;}
    g_ready.store(true,std::memory_order_release);
    if(log)log("Dash combat V1.71 READY: validated AIM observation retained; optional stopped-dash MOVE permission starts OFF");
    const Guard combatGuards[]={
        {0x5D8130,{0x48,0x89,0x6C,0x24,0x18,0x57,0x48,0x83,0xEC,0x40,0x48,0x8B,0xBA,0xE8,0x00,0x00}},
        {0x69FBD0,{0x40,0x53,0x48,0x83,0xEC,0x50,0x48,0x83,0xB9,0x18,0x06,0x00,0x00,0x00,0x48,0x8B}},
        {0x7CE080,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x92,0x3C,0x04,0x03,0x48,0x85}},
        {0x669D30,{0x48,0x81,0xC1,0x40,0x05,0x00,0x00,0xE9,0xC4,0xB3,0xFC,0xFF,0xCC,0xCC,0xCC,0xCC}},
        {0x742030,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0xFA,0x5D,0x0C,0x03,0x48,0x85}}
    };
    for(const auto& guard:combatGuards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
        if(log)log("Dash combat V1.56 signature mismatch; native combat retained");return;
    }
    void* notify=reinterpret_cast<void*>(g_base+0x5D8130);
    if(MH_CreateHook(notify,reinterpret_cast<void*>(&HookSpawnNotify),reinterpret_cast<void**>(&g_notify))!=MH_OK)return;
    if(MH_EnableHook(notify)!=MH_OK){MH_RemoveHook(notify);g_notify=nullptr;return;}
    g_actionQuery=reinterpret_cast<ActionQueryFn>(g_base+0x669D30);
    g_startAim=reinterpret_cast<StartAimFn>(g_base+0x69FBD0);
    g_combatReady.store(true,std::memory_order_release);
    // Independent optional pair: failure retains the validated AIM/projectile
    // repair. Both native entrypoints/vtables are exact-retail signature gated.
    const unsigned char soundSig[]={0x48,0x8B,0xC4,0x55,0x57,0x41,0x56,0x48,0x8D,0x68,0xA1,0x48,0x81,0xEC,0xB0,0x00};
    const unsigned char particleSig[]={0x48,0x8B,0x01,0x48,0xFF,0xA0,0x60,0x02,0x00,0x00};
    uintptr_t soundEntry=0,particleEntry=0;
    const bool presentationLayout=Read(reinterpret_cast<void*>(g_base+0x2C939B0),0x240,soundEntry)&&soundEntry==g_base+0x16977D0&&
        Read(reinterpret_cast<void*>(g_base+0x2C935D8),0x240,particleEntry)&&particleEntry==g_base+0x16977C0&&
        !std::memcmp(reinterpret_cast<void*>(soundEntry),soundSig,sizeof(soundSig))&&
        !std::memcmp(reinterpret_cast<void*>(particleEntry),particleSig,sizeof(particleSig));
    void* sound=reinterpret_cast<void*>(soundEntry);void* particle=reinterpret_cast<void*>(particleEntry);
    bool soundCreated=false,particleCreated=false;
    if(presentationLayout){
        soundCreated=MH_CreateHook(sound,reinterpret_cast<void*>(&HookSoundNotify),reinterpret_cast<void**>(&g_sound))==MH_OK;
        if(soundCreated)particleCreated=MH_CreateHook(particle,reinterpret_cast<void*>(&HookParticleNotify),reinterpret_cast<void**>(&g_particle))==MH_OK;
        if(soundCreated&&particleCreated&&MH_EnableHook(sound)==MH_OK&&MH_EnableHook(particle)==MH_OK)
            g_presentationReady.store(true,std::memory_order_release);
    }
    if(!g_presentationReady.load()){
        if(soundCreated){MH_DisableHook(sound);MH_RemoveHook(sound);}
        if(particleCreated){MH_DisableHook(particle);MH_RemoveHook(particle);}
        g_sound=nullptr;g_particle=nullptr;
    }
    if(log)log(g_presentationReady.load()?"Dash presentation V1.57 READY: exact standard gunfire/flash/casings in owned F/B/L/R special-ammo dash only; EXPERIMENTAL":
        "Dash presentation V1.57 unavailable; native presentation retained, validated AIM repair kept");
    if(log)log("Dash combat V1.56 READY: live-player held-AIM resume via registered DualPistolsBP after idle ground dash; V1.55 forced-standard scope retained; TPS/Recovery independent; EXPERIMENTAL");
}
bool RecoveryReady(){return g_recoveryReady.load()&&g_combatReady.load()&&g_allowMove&&g_actionQuery;}
void Shutdown(){g_recoveryReady=false;g_presentationReady.store(false,std::memory_order_release);g_combatReady.store(false,std::memory_order_release);g_ready.store(false,std::memory_order_release);}
void OnLivePlayer(void* actor){
    if(!g_ready.load()||!g_combatReady.load()||!g_startAim||g_resuming||!native_tps::OnGameThread())return;
    const auto now=GetTickCount64();bool pending=false;
    for(auto& resume:g_resume){
        if(resume.seen&&(now<resume.seen||now-resume.seen>3000))resume={};
        pending=pending||(resume.seen&&!resume.requested);
    }
    if(!pending)return;
    local_players::Context owner{};
    if(!CombatOwner(actor,owner)){
        if(owner.index>=0&&owner.index<2&&g_resume[owner.index].actor==reinterpret_cast<uintptr_t>(actor))
            g_resume[owner.index]={};
        return;
    }
    auto& resume=g_resume[owner.index];
    if(!resume.seen||resume.requested)return;
    if(now<resume.seen||now-resume.seen>3000||resume.local!=owner.localPlayer||
       resume.actor!=owner.character){resume={};return;}
    void* array=nullptr;int count=0;void* movement=nullptr;unsigned char mode=0;
    bool aim=false,primary=false,secondary=false;
    if(!AbilityArray(actor,array,count)||count||!Read(actor,0x388,movement)||
       !Read(movement,0x1B0,mode)||(mode!=1&&mode!=2)||!AimActions(actor,aim,primary,secondary))return;
    if(!aim){resume={};return;}
    // The native player-camera callback runs even without an aim ability.
    // A nested callback cannot request a second ability.
    struct Scope {Scope(){g_resuming=true;}~Scope(){g_resuming=false;}} scope;
    void* pistols=MainPistols(actor);
    if(!pistols){
        resume={}; // Unknown contract: stop, never retry every frame.
        const unsigned n=++g_unavailable;
        if(g_log&&n<=8)g_log("Dash combat V1.56 AIM_WEAPON_UNAVAILABLE; native action retained, no retry for this dash");
        return;
    }
    if(!resume.Take(owner.localPlayer,owner.character,now,true,true,aim))return;
    g_startAim(pistols);
    const unsigned n=++g_resumed;
    if(g_log&&(n<=8||n%1000==0)){char line[190]{};
        sprintf_s(line,"Dash combat V1.56 AIM_RESTART_REQUEST player=%d n=%u nativeHeldAim=1 oncePerDash=1 gameplay=EXPERIMENTAL",owner.index+1,n);g_log(line);}
}
void Report(){if(g_log){char line[240]{};sprintf_s(line,"Dash combat totals aimRestartRequests=%u forcedStandardSkipped=%u soundSkipped=%u effectSkipped=%u; firing-during-dash issue remains open",g_resumed.load(),g_suppressed.load(),g_soundSkipped.load(),g_particleSkipped.load());g_log(line);}}
}
