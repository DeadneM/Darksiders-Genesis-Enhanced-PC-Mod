#include "../src/MountedJumpFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <limits>
#include <utility>
namespace {
using dg::mounted_air::Floor;using dg::mounted_air::Vector;using dg::mounted_air::Hit;
bool gameThread=true,paused=false,cinema=false,radial=false,matched=true,rejectWalking=false,stealOnMode=false;
unsigned jumps=0,modeCalls=0,heightCalls=0,floorCalls=0;
std::vector<unsigned char> image(0x3B00000);
unsigned char objects[128][0xC20]{},nameData[128][128]{},items[128*24]{},element[0xF0]{};
uintptr_t names[128]{},nameChunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{reinterpret_cast<uintptr_t>(items)};
std::vector<std::string> labels;
enum Object {Horse,Controller,Movement,MoveClass,CharacterMove,ModeFunction,ModeParam,CustomParam,FunctionClass,ByteClass,FloatClass,
    StructClass,BoolClass,WeakClass,NameClass,IntClass,ScriptStructClass,Package,HorseClass,Input,InputClass,SettingsClass,Settings,
    Character,Capsule,CapsuleClass,HeightFunction,ReturnParam,FloorType,HitType};
unsigned nextObject=30,timeProperty=0;dg::local_players::Context owner{};dg::mounted_jump::Mapping mapping{};
Floor lastFloor{};float halfHeight=100;
template<class T>void Put(void* p,size_t at,T v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
template<class T>T Get(void* p,size_t at){T v{};std::memcpy(&v,static_cast<unsigned char*>(p)+at,sizeof(v));return v;}
uintptr_t P(unsigned i){return reinterpret_cast<uintptr_t>(objects[i]);}
void Check(bool b,const char* why){if(!b)throw std::runtime_error(why);}
int Label(const char* text){
    for(unsigned i=0;i<labels.size();++i)if(labels[i]==text)return int(i);
    const auto i=unsigned(labels.size());Check(i<128,"name table bounded");labels.emplace_back(text);names[i]=reinterpret_cast<uintptr_t>(nameData[i]);strcpy_s(reinterpret_cast<char*>(nameData[i]+12),116,text);return int(i);
}
void Named(unsigned object,const char* text){Put(objects[object],0x18,Label(text));}
unsigned Field(unsigned parent,const char* text,unsigned kind,int bytes,int at,unsigned structure=0,unsigned char mask=0){
    const auto i=nextObject++;Check(i<128,"field table bounded");Named(i,text);Put(objects[i],0x10,P(kind));Put(objects[i],0x34,bytes);Put(objects[i],0x44,at);
    Put(objects[i],0x28,Get<uintptr_t>(objects[parent],0x48));Put(objects[parent],0x48,P(i));
    if(structure)Put(objects[i],0x70,P(structure));
    if(mask){const unsigned char bits[]={4,0,mask,mask};std::memcpy(objects[i]+0x70,bits,4);}
    return i;
}
void OriginalFloor(void*,const Vector*,float,float,Floor* out,float,const Hit*){++floorCalls;*out={};}
void Query(const Vector& position){dg::mounted_jump::ComputePrivateFloor(objects[Movement],&position,50,50,&lastFloor,40,nullptr);}
void EngineEvent(void* p,void* function,void* args){
    Check(gameThread,"engine calls stay on game thread");
    if(function==objects[HeightFunction]){Check(p==objects[Capsule],"own capsule height");++heightCalls;*static_cast<float*>(args)=halfHeight;return;}
    Check(function==objects[ModeFunction]&&p==objects[Movement],"owned native mode adapter");
    ++modeCalls;const auto* bytes=static_cast<unsigned char*>(args);Check(!bytes[1]&&(bytes[0]==1||bytes[0]==3),"Walking/Falling only, never Flying");
    Put<unsigned char>(p,0x1B0,bytes[0]);Put<unsigned char>(p,0x1B1,0);
    if(stealOnMode)Put(objects[Horse],0x348,uintptr_t(42));
    if(bytes[0]==1){Query(Get<Vector>(objects[Capsule],0x1A0));if(rejectWalking||!dg::mounted_air::Walkable(lastFloor))Put<unsigned char>(p,0x1B0,3);else Put<float>(p,0x114,0);}
}
int* Find(void*,int* result,const void*){*result=0;return result;}
bool Jump(void* p,bool replay){Check(p==objects[Movement]&&!replay,"existing native jump reused");++jumps;Put<unsigned char>(p,0x1B0,3);Put<float>(p,0x114,500);return true;}
void Tick(bool commands=true,bool focused=true){dg::mounted_jump::g_players[0].tick=0;dg::mounted_jump::OnLivePlayer(owner,7,focused,commands);}
void Down(bool b){Put<unsigned char>(element,0x34,b?1:0);}
void Mode(unsigned char mode,float vertical=0){Put<unsigned char>(objects[Movement],0x1B0,mode);Put<float>(objects[Movement],0x114,vertical);}
void Press(){Down(false);Tick();Down(true);Tick();}
void Setup(){
    using namespace dg::mounted_jump;g_base=reinterpret_cast<uintptr_t>(image.data());g_event=EngineEvent;g_find=Find;g_jump=Jump;g_compute=OriginalFloor;g_ready=true;g_floorReady=true;
    Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(image.data(),0x3959694,128);
    for(unsigned i=0;i<128;++i){Put(objects[i],0xC,int(i));Put(items,i*24,P(i));Put(items,i*24+16,int(i+1));}
    for(const auto& pair:std::initializer_list<std::pair<unsigned,const char*>>{{MoveClass,"MayhemHorseCharacterMovementComponent"},{CharacterMove,"CharacterMovementComponent"},{ModeFunction,"SetMovementMode"},
        {ModeParam,"NewMovementMode"},{CustomParam,"NewCustomMode"},{FunctionClass,"Function"},{ByteClass,"ByteProperty"},{FloatClass,"FloatProperty"},{StructClass,"StructProperty"},
        {BoolClass,"BoolProperty"},{WeakClass,"WeakObjectProperty"},{NameClass,"NameProperty"},{IntClass,"IntProperty"},{ScriptStructClass,"ScriptStruct"},{Package,"/Script/Engine"},
        {HorseClass,"Despair_Blueprint_C"},{InputClass,"PlayerInput"},{SettingsClass,"InputSettings"},{CapsuleClass,"CapsuleComponent"},{HeightFunction,"GetScaledCapsuleHalfHeight"},
        {ReturnParam,"ReturnValue"},{FloorType,"FindFloorResult"},{HitType,"HitResult"}})Named(pair.first,pair.second);
    Put(objects[Movement],0,g_base+0x270ABF0);Put(image.data(),0x270ABF0+0x600,g_base+0x667510);
    Put(objects[Movement],0x10,P(MoveClass));Put(objects[MoveClass],0x40,P(CharacterMove));Put(objects[CharacterMove],0x20,P(Package));Put(objects[CapsuleClass],0x20,P(Package));
    Put(objects[CharacterMove],0x48,P(ModeFunction));Put(objects[CapsuleClass],0x48,P(HeightFunction));
    for(unsigned i:{unsigned(ModeFunction),unsigned(HeightFunction)}){Put(objects[i],0x10,P(FunctionClass));Put<uint32_t>(objects[i],0x98,0x4020400);}
    Put(objects[ModeFunction],0x20,P(CharacterMove));Put(objects[ModeFunction],0xC0,g_base+0x1CF1600);Put<uint16_t>(objects[ModeFunction],0x9E,2);
    Put(objects[ModeFunction],0x48,P(ModeParam));Put(objects[ModeParam],0x28,P(CustomParam));
    for(unsigned i:{unsigned(ModeParam),unsigned(CustomParam)}){Put(objects[i],0x10,P(ByteClass));Put<int>(objects[i],0x34,1);}Put<int>(objects[CustomParam],0x44,1);
    Put(objects[HeightFunction],0x20,P(CapsuleClass));Put(objects[HeightFunction],0xC0,g_base+0x1CED210);Put<uint16_t>(objects[HeightFunction],0x9E,4);Put(objects[HeightFunction],0x48,P(ReturnParam));Put(objects[ReturnParam],0x10,P(FloatClass));Put<int>(objects[ReturnParam],0x34,4);
    for(unsigned i:{unsigned(FloorType),unsigned(HitType)}){Put(objects[i],0x10,P(ScriptStructClass));Put(objects[i],0x20,P(Package));}
    Field(CharacterMove,"CurrentFloor",StructClass,148,0x350,FloorType);
    Field(FloorType,"HitResult",StructClass,136,12,HitType);Field(FloorType,"FloorDist",FloatClass,4,4);Field(FloorType,"LineDist",FloatClass,4,8);
    Field(FloorType,"bBlockingHit",BoolClass,4,0,0,1);Field(FloorType,"bWalkableFloor",BoolClass,4,0,0,2);Field(FloorType,"bLineTrace",BoolClass,4,0,0,4);
    Field(HitType,"bBlockingHit",BoolClass,1,0,0,1);Field(HitType,"bStartPenetrating",BoolClass,1,0,0,2);
    Field(HitType,"FaceIndex",IntClass,4,4);timeProperty=Field(HitType,"Time",FloatClass,4,8);Field(HitType,"Distance",FloatClass,4,12);
    const char* vectors[]={"Location","ImpactPoint","Normal","ImpactNormal","TraceStart","TraceEnd"};for(unsigned n=0;n<6;++n)Field(HitType,vectors[n],StructClass,12,int(16+n*12));
    Field(HitType,"PenetrationDepth",FloatClass,4,88);Field(HitType,"Item",IntClass,4,92);Field(HitType,"PhysMaterial",WeakClass,8,96);Field(HitType,"Actor",WeakClass,8,104);Field(HitType,"Component",WeakClass,8,112);Field(HitType,"BoneName",NameClass,8,120);Field(HitType,"MyBoneName",NameClass,8,128);
    Put(objects[Horse],0x10,P(HorseClass));Put<unsigned char>(objects[Horse],0x118,3);Put(objects[Horse],0x388,P(Movement));Put(objects[Horse],0x158,P(Capsule));
    Put(objects[Horse],0x348,P(Controller));Put(objects[Controller],0x350,P(Horse));Put(objects[Movement],0xE8,P(Horse));Put(objects[Movement],0xF8,P(Capsule));
    Put(objects[Capsule],0x10,P(CapsuleClass));Put(objects[Capsule],0xE8,P(Horse));Put(objects[Capsule],0x1A0,Vector{100,200,500});
    Put(objects[Controller],0x448,P(Input));Put(objects[Input],0x10,P(InputClass));Put(objects[Input],0x20,P(Controller));Put(objects[Input],0x260,Array{reinterpret_cast<uintptr_t>(element),1,1});
    mapping.action={Label("Jump"),0};mapping.key={Label("AnyBoundKey"),0};Put(objects[Settings],0x80,Array{reinterpret_cast<uintptr_t>(&mapping),1,1});Put(image.data(),0x3A5E1C0,P(SettingsClass));Put(objects[SettingsClass],0xF8,P(Settings));
    owner.index=0;owner.localPlayer=1;owner.world=100;owner.character=P(Character);owner.pawn=P(Horse);owner.controller=P(Controller);owner.kind=dg::local_players::PoseKind::Horse;
    dg::runtime::Get().mountedAirWalkEnabled=true;dg::runtime::Get().mountedJumpEnabled=true;
}
void Geometry(){
    using namespace dg::mounted_air;Plane p{400,100};Floor f{};
    Check(ApplyFloor(p,{100,200,502},30,50,40,f)&&Walkable(f)&&f.distance==2&&f.hit.impact.z==400&&f.hit.normal.z==1,"horizontal floor under capsule bottom");
    Check(f.hit.component.index==-1&&f.hit.actor.index==-1&&f.hit.material.index==-1,"no fake world actor/component/material");
    Floor higher{};higher.flags=3;higher.distance=1;higher.hit.flags=1;higher.hit.impact.z=401;higher.hit.component.index=55;
    Check(!ApplyFloor(p,{100,200,502},30,50,40,higher)&&higher.hit.component.index==55&&p.z==401,"higher real floor wins");
    Floor empty{};Check(!ApplyFloor(p,{0,0,800},30,50,40,empty)&&!Walkable(empty),"out-of-probe plane refused");
    Check(!ApplyFloor(p,{0,0,490},30,50,40,empty)&&!ApplyFloor(p,{0,0,502},0,0,40,empty),"deep penetration and empty sweep refused");
    Check(!ApplyFloor(p,{0,0,std::numeric_limits<float>::quiet_NaN()},30,50,40,empty),"nonfinite geometry refused");
}
unsigned footNotify=0,footMesh=0,footCue=0,footCalls=0,nativeFootCalls=0;
void FootstepScenarios(){
    using namespace dg::mounted_jump;
    gameThread=matched=true;paused=cinema=radial=false;g_footstepApi={};g_steps=0;g_log=nullptr;
    const unsigned notifyClass=nextObject++;footNotify=nextObject++;footMesh=nextObject++;
    const unsigned soundClass=nextObject++,soundPackage=nextObject++;footCue=nextObject++;
    Named(notifyClass,"AnimNotify_PlayFootstep");Named(soundClass,"SoundCue");Named(soundPackage,"/Game/Audio/Sfx/Character/Ruin/char_ruin_foot_base");Named(footCue,"char_ruin_foot_base");
    Put(objects[footNotify],0x10,P(notifyClass));Put(objects[footNotify],0,g_base+0x10000);Put(image.data(),0x10240,g_base+0x5D6E50);
    Field(notifyClass,"bPlaySounds",BoolClass,1,200);Field(notifyClass,"bPlayVFX",BoolClass,1,201);Field(notifyClass,"bPlayDecals",BoolClass,1,202);
    Put<unsigned char>(objects[footNotify],200,1);Put(objects[footCue],0x10,P(soundClass));Put(objects[footCue],0x20,P(soundPackage));
    Put(objects[Horse],0x380,P(footMesh));Put(objects[footMesh],0xE8,P(Horse));Put(objects[Horse],0x348,P(Controller));Put(objects[Controller],0x350,P(Horse));
    Put<unsigned char>(objects[Horse],0x118,3);Put<unsigned char>(objects[Horse],0xA66,0);Put(objects[Horse],0x158,P(Capsule));Put(objects[Movement],0xE8,P(Horse));Put(objects[Movement],0xF8,P(Capsule));Put(objects[Capsule],0xE8,P(Horse));
    Put(objects[Capsule],0x1A0,Vector{10,20,300});Put(objects[Movement],0x10C,Vector{120,0,0});Mode(1);
    g_players[0]={};g_players[1]={};auto& s=g_players[0];s.pawn=P(Horse);s.controller=P(Controller);s.character=P(Character);s.generation=7;s.tick=GetTickCount64();
    s.air.lease={{P(Horse),Horse,Horse+1},{P(Movement),Movement,Movement+1},{P(Controller),Controller,Controller+1},{P(Capsule),Capsule,Capsule+1},100,{200,100},true};
    Floor floor{};floor.flags=3;floor.hit.flags=1;floor.hit.impact.z=200;Put(objects[Movement],0x350,floor);
    g_sounds[3]={P(footCue),int(footCue),int(footCue+1)};dg::runtime::Get().mountedAirWalkEnabled=true;
    g_sound=+[](void* actor,void* cue,const dg::mounted_jump::Vector* p,const dg::mounted_jump::Vector*,float volume,float pitch,float start,void* attenuation,void* concurrency,void* source){
        Check(actor==objects[Horse]&&source==actor&&cue==objects[footCue]&&p->z==200&&volume==1&&pitch==1&&!start&&!attenuation&&!concurrency,"hoof cue uses native Sfx route at private floor height");++footCalls;};
    g_footstep=+[](void*,void*,void*){++nativeFootCalls;};
    const auto fire=[](){HookFootstep(objects[footNotify],objects[footMesh],nullptr);};
    for(const char* horse:{"Mayhem_Blueprint_C","Despair_Blueprint_C","Ruin_Blueprint_C","Rampage_Blueprint_C"}){Named(HorseClass,horse);s.tick=GetTickCount64();fire();}
    Check(footCalls==4&&!nativeFootCalls,"all four mounted skins use one native hoof sound per sound-only animation event");
    Put<unsigned char>(objects[footNotify],201,1);fire();Put<unsigned char>(objects[footNotify],201,0);Check(nativeFootCalls==1&&footCalls==4,"VFX notifications remain native, no duplicate sound");
    floor.hit.component={1,2};Put(objects[Movement],0x350,floor);fire();floor.hit.component={};Put(objects[Movement],0x350,floor);Check(nativeFootCalls==2,"real terrain keeps native material-specific footsteps");
    matched=false;fire();matched=true;Mode(3);fire();Mode(1);Check(nativeFootCalls==4,"foreign generation and airborne jump never use private-floor audio");
    s.tick=GetTickCount64()-251;fire();s.tick=GetTickCount64();Check(nativeFootCalls==5,"stale focus/menu lifecycle cannot emit a hoof sound");
    const auto cue=g_sounds[3];g_sounds[3]={};fire();g_sounds[3]=cue;Check(nativeFootCalls==6,"missing native cue fails open instead of consuming the notify");
    dg::runtime::Get().mountedAirWalkEnabled=false;fire();dg::runtime::Get().mountedAirWalkEnabled=true;Check(nativeFootCalls==7,"option OFF is entirely native");
    Put(objects[Movement],0x10C,Vector{});fire();Put(objects[Movement],0x10C,Vector{120,0,0});Check(nativeFootCalls==8,"stationary mount has no artificial footstep cadence");
    g_players[1]=s;g_players[0]={};g_players[1].tick=GetTickCount64();fire();Check(footCalls==5&&nativeFootCalls==8,"P2's own live lease emits independently");
}
void Run(){
    using namespace dg::mounted_air;Geometry();Setup();const auto base=dg::mounted_jump::g_base;auto& air=dg::mounted_jump::g_players[0].air;
    dg::mounted_jump::g_players[0].tick=GetTickCount64();
    Check(Methods(base,P(Movement))&&FloorSchema(base,P(MoveClass)),"exact reflected contracts accepted");
    api={};Put(objects[ModeFunction],0xC0,base+0x1CEFBD0);Check(!Methods(base,P(Movement)),"wrong native method refused");Put(objects[ModeFunction],0xC0,base+0x1CF1600);
    Put<int>(objects[CustomParam],0x44,2);Check(!Methods(base,P(Movement)),"wrong mode parameter refused");Put<int>(objects[CustomParam],0x44,1);
    api={};Put<int>(objects[timeProperty],0x44,12);Mode(3);Check(!AcquireFloor(base,EngineEvent,owner,P(Movement),air)&&!modeCalls,"wrong HitResult layout cannot write floor");Put<int>(objects[timeProperty],0x44,8);api={};
    gameThread=false;Check(!AcquireFloor(base,EngineEvent,owner,P(Movement),air)&&!modeCalls,"off-thread cannot acquire");gameThread=true;
    rejectWalking=true;Check(!AcquireFloor(base,EngineEvent,owner,P(Movement),air)&&!air.lease.active&&Get<unsigned char>(objects[Movement],0x1B0)==3,"rejected Walking removes support");rejectWalking=false;
    Mode(3);Check(AcquireFloor(base,EngineEvent,owner,P(Movement),air)&&air.lease.active&&Get<unsigned char>(objects[Movement],0x1B0)==1,"Walking sees floor during entry callback");
    const auto before=modeCalls;Release(base,EngineEvent,air,999);Check(modeCalls==before&&!air.lease.active,"world replacement avoids old objects");Query({100,200,500});Check(!Walkable(lastFloor),"discarded lease leaves no floor");
    Mode(3);Check(AcquireFloor(base,EngineEvent,owner,P(Movement),air),"reacquire");Put(objects[Horse],0x348,uintptr_t(42));const auto count=modeCalls;Release(base,EngineEvent,air,100);Check(modeCalls==count,"foreign possession wins");Put(objects[Horse],0x348,P(Controller));
    Mode(3);stealOnMode=true;Check(!AcquireFloor(base,EngineEvent,owner,P(Movement),air)&&!air.lease.active,"possession rechecked after callback");stealOnMode=false;Put(objects[Horse],0x348,P(Controller));
    Mode(3);Check(AcquireFloor(base,EngineEvent,owner,P(Movement),air),"serial setup");Put<int>(items,Movement*24+16,777);const auto calls=modeCalls;Release(base,EngineEvent,air,100);Check(modeCalls==calls,"recycled movement cannot be changed");Put<int>(items,Movement*24+16,Movement+1);
    dg::mounted_jump::g_players[0]={};Mode(1);Down(false);Tick();Press();Check(jumps==1,"ground jump retained");Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==3,"hold preserves upward jump");
    Mode(3,0);Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==1&&air.lease.active,"apex becomes Walking, never Flying");
    const auto traces=floorCalls;Query({600,700,502});Check(floorCalls==traces+1&&Walkable(lastFloor)&&lastFloor.distance==2&&lastFloor.hit.impact.x==600,"one original query plus own floor at new XY");
    Floor other{};const Vector pos{600,700,502};dg::mounted_jump::ComputePrivateFloor(objects[Character],&pos,50,50,&other,40,nullptr);Check(!Walkable(other),"other actor cannot use floor");
    const auto observed=dg::mounted_jump::g_players[0];dg::mounted_jump::g_players[1]=observed;dg::mounted_jump::g_players[0]={};Query(pos);Check(Walkable(lastFloor),"P2 lease supported independently");dg::mounted_jump::g_players[0]=observed;dg::mounted_jump::g_players[1]={};
    Down(false);Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==1,"release grace keeps support");Down(true);Tick();Check(jumps==2&&Get<unsigned char>(objects[Movement],0x1B0)==3&&!air.lease.active,"fresh press removes floor before native jump");
    Mode(3,0);Tick();Tick(false);Check(Get<unsigned char>(objects[Movement],0x1B0)==1,"editor preserves support");Tick();Check(jumps==2,"editor close held cannot jump");
    dg::runtime::Get().mountedAirWalkEnabled=false;Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==3&&!air.lease.active,"OFF requests Falling");Query(pos);Check(!Walkable(lastFloor),"OFF forwards original floor");
    Mode(1);Press();Check(jumps==3,"ground jump works with floor OFF");Mode(3,0);dg::runtime::Get().mountedAirWalkEnabled=true;Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==3,"held enable cannot catch old fall");
    Press();Check(Get<unsigned char>(objects[Movement],0x1B0)==1,"airborne action acquires floor");Tick(true,false);Check(Get<unsigned char>(objects[Movement],0x1B0)==3,"focus loss removes floor");Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==3,"focus restore held cannot acquire");
    Press();paused=true;Tick();paused=false;Check(Get<unsigned char>(objects[Movement],0x1B0)==3,"pause cancels support");
    Down(false);Tick();Press();owner.world=200;owner.kind=dg::local_players::PoseKind::Foot;const auto old=modeCalls;Tick();Check(modeCalls==old&&!air.lease.active,"world/form change discards plane");
    Policy p{};Check(Decide(p,false,3,-1,true,false,100,200)==Action::None,"held enable requires edge");Jumped(p);Check(Decide(p,false,3,2,true,false,100,200)==Action::None&&Decide(p,false,3,0,true,false,101,200)==Action::Support,"apex decision");
    p={};Check(Decide(p,true,1,0,false,false,100,200)==Action::None&&Decide(p,true,1,0,false,false,300,200)==Action::Fall,"Walking grace expires");p={};Check(Decide(p,true,1,0,false,false,100,0)==Action::Fall,"zero delay falls immediately");
    FootstepScenarios();
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return cinema;}bool RadialActive(unsigned){return radial;}}
namespace dg::local_players {bool Matches(unsigned,uintptr_t,uint32_t){return matched;}}
int main(){try{Run();std::cout<<"Mounted private floor / native Walking lifecycle checks passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
