#include "../src/MountedJumpFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>
namespace {
bool gameThread=true,paused=false,cinema=false,radial=false,matched=true;
unsigned jumps=0,modes=0,stops=0;bool rejectStop=false,stealOnMode=false;
std::vector<unsigned char> image(0x3B00000);
unsigned char objects[24][0xC20]{},nameData[32][128]{},items[24*24]{},element[0xF0]{};
uintptr_t names[32]{},nameChunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{reinterpret_cast<uintptr_t>(items)};
enum Object {Horse,Controller,Movement,MoveClass,CharacterMove,BaseMove,ModeFunction,StopFunction,
    ModeParam,CustomParam,Velocity,FunctionClass,ByteClass,StructClass,Package,HorseClass,Input,InputClass,SettingsClass,Settings,Character};
dg::local_players::Context owner{};dg::mounted_jump::Mapping mapping{};
template<class T>void Put(void* p,size_t at,T v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
template<class T>T Get(void* p,size_t at){T v{};std::memcpy(&v,static_cast<unsigned char*>(p)+at,sizeof(v));return v;}
uintptr_t P(unsigned i){return reinterpret_cast<uintptr_t>(objects[i]);}
void Check(bool b,const char* why){if(!b)throw std::runtime_error(why);}
void EngineEvent(void* p,void* function,void* args){
    Check(gameThread&&p==objects[Movement],"engine callbacks stay on the owned movement and game thread");
    if(function==objects[ModeFunction]){
        ++modes;const auto* bytes=static_cast<unsigned char*>(args);
        Check(bytes[1]==0&&(bytes[0]==3||bytes[0]==5),"only native Falling/Flying; no custom physics");
        Put<unsigned char>(p,0x1B0,bytes[0]);Put<unsigned char>(p,0x1B1,0);
        if(stealOnMode)Put(objects[Horse],0x348,uintptr_t(42));
    }else if(function==objects[StopFunction]){
        ++stops;if(!rejectStop){Put<float>(p,0x10C,0);Put<float>(p,0x110,0);Put<float>(p,0x114,0);}
    }else Check(false,"unexpected reflected engine function");
}
int* Find(void*,int* result,const void*){*result=0;return result;}
bool Jump(void* p,bool replay){Check(p==objects[Movement]&&!replay,"existing native jump adapter");++jumps;Put<unsigned char>(p,0x1B0,3);Put<float>(p,0x114,500);return true;}
void Tick(bool commands=true,bool focused=true){dg::mounted_jump::g_players[0].tick=0;dg::mounted_jump::OnLivePlayer(owner,7,focused,commands);}
void Down(bool b){Put<unsigned char>(element,0x34,b?1:0);}
void Mode(unsigned char mode,float vertical=0){Put<unsigned char>(objects[Movement],0x1B0,mode);Put<float>(objects[Movement],0x114,vertical);}
void Press(){Down(false);Tick();Down(true);Tick();}
void Setup(){
    using namespace dg::mounted_jump;g_base=reinterpret_cast<uintptr_t>(image.data());g_event=EngineEvent;g_find=Find;g_jump=Jump;g_ready=true;
    const char* labels[]={"horse","controller","movement","MayhemMountMovementComponent","CharacterMovementComponent","MovementComponent",
        "SetMovementMode","StopMovementImmediately","NewMovementMode","NewCustomMode","Velocity","Function","ByteProperty","StructProperty",
        "/Script/Engine","Despair_Blueprint_C","input","PlayerInput","InputSettings","settings","character","Jump","AnyBoundKey"};
    Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(image.data(),0x3959694,24);
    for(unsigned i=0;i<23;++i){names[i]=reinterpret_cast<uintptr_t>(nameData[i]);strcpy_s(reinterpret_cast<char*>(nameData[i]+12),116,labels[i]);}
    for(unsigned i=0;i<24;++i){Put(objects[i],0xC,int(i));Put(objects[i],0x18,int(i));Put(items,i*24,P(i));Put(items,i*24+16,int(i+1));}
    Put(objects[Movement],0,g_base+0x270ABF0);Put(image.data(),0x270ABF0+0x600,g_base+0x667510);
    Put(objects[Movement],0x10,P(MoveClass));Put(objects[MoveClass],0x40,P(CharacterMove));Put(objects[CharacterMove],0x40,P(BaseMove));
    Put(objects[CharacterMove],0x20,P(Package));Put(objects[BaseMove],0x20,P(Package));
    Put(objects[CharacterMove],0x48,P(ModeFunction));Put(objects[BaseMove],0x48,P(StopFunction));Put(objects[BaseMove],0x40,uintptr_t(0));
    for(unsigned i:{unsigned(ModeFunction),unsigned(StopFunction)}){Put(objects[i],0x10,P(FunctionClass));Put<uint32_t>(objects[i],0x98,0x4020400);}
    Put(objects[ModeFunction],0x20,P(CharacterMove));Put(objects[ModeFunction],0xC0,g_base+0x1CF1600);Put<uint16_t>(objects[ModeFunction],0x9E,2);
    Put(objects[StopFunction],0x20,P(BaseMove));Put(objects[StopFunction],0xC0,g_base+0x752220);
    Put(objects[ModeFunction],0x48,P(ModeParam));Put(objects[ModeParam],0x28,P(CustomParam));
    for(unsigned i:{unsigned(ModeParam),unsigned(CustomParam)}){Put(objects[i],0x10,P(ByteClass));Put<int>(objects[i],0x34,1);}
    Put<int>(objects[CustomParam],0x44,1);Put(objects[ModeFunction],0x28,P(Velocity));
    Put(objects[Velocity],0x10,P(StructClass));Put<int>(objects[Velocity],0x34,12);Put<int>(objects[Velocity],0x44,0x10C);
    Put(objects[Horse],0x10,P(HorseClass));Put<unsigned char>(objects[Horse],0x118,3);Put(objects[Horse],0x388,P(Movement));
    Put(objects[Horse],0x348,P(Controller));Put(objects[Controller],0x350,P(Horse));Put(objects[Movement],0xE8,P(Horse));
    Put(objects[Controller],0x448,P(Input));Put(objects[Input],0x10,P(InputClass));Put(objects[Input],0x20,P(Controller));
    Put(objects[Input],0x260,Array{reinterpret_cast<uintptr_t>(element),1,1});
    mapping.action={21,0};mapping.key={22,0};Put(objects[Settings],0x80,Array{reinterpret_cast<uintptr_t>(&mapping),1,1});
    Put(image.data(),0x3A5E1C0,P(SettingsClass));Put(objects[SettingsClass],0xF8,P(Settings));
    owner.index=0;owner.localPlayer=1;owner.world=100;owner.character=P(Character);owner.pawn=P(Horse);owner.controller=P(Controller);owner.kind=dg::local_players::PoseKind::Horse;
    dg::runtime::Get().mountedAirWalkEnabled=true;dg::runtime::Get().mountedJumpEnabled=true;
}
void Run(){
    using namespace dg::mounted_air;Setup();const auto base=dg::mounted_jump::g_base;
    Check(Methods(base,P(Movement)),"real inherited reflection contracts accepted");
    api={};Put(objects[ModeFunction],0xC0,base+0x1CEFBD0);Check(!Methods(base,P(Movement)),"same-named foreign entry rejected");Put(objects[ModeFunction],0xC0,base+0x1CF1600);
    Put<int>(objects[CustomParam],0x44,2);Check(!Methods(base,P(Movement)),"wrong parameter layout rejected");Put<int>(objects[CustomParam],0x44,1);
    State s{};Mode(3,-20);gameThread=false;Check(!Hover(base,EngineEvent,owner,P(Movement),s)&&!modes,"no engine calls off game thread");gameThread=true;
    rejectStop=true;Check(!Hover(base,EngineEvent,owner,P(Movement),s)&&Get<unsigned char>(objects[Movement],0x1B0)==3&&!s.lease.active,"failed hover rolls back to Falling");rejectStop=false;
    Mode(3,-20);Check(Hover(base,EngineEvent,owner,P(Movement),s)&&s.lease.active&&stops==2,"one stop request on successful native hover");
    const auto before=modes;Release(base,EngineEvent,s,999);Check(modes==before&&!s.lease.active,"world replacement does not call old objects");
    Mode(3);Check(Hover(base,EngineEvent,owner,P(Movement),s),"reacquire");Put(objects[Horse],0x348,uintptr_t(42));const auto count=modes;
    Release(base,EngineEvent,s,100);Check(modes==count,"another controller owning the horse wins");Put(objects[Horse],0x348,P(Controller));
    Mode(3);stealOnMode=true;const auto stopCount=stops;Check(!Hover(base,EngineEvent,owner,P(Movement),s)&&stops==stopCount,"possession rechecked after engine callback");stealOnMode=false;Put(objects[Horse],0x348,P(Controller));
    Mode(3);Check(Hover(base,EngineEvent,owner,P(Movement),s),"reacquire for serial guard");Put<int>(items,Movement*24+16,777);const auto calls=modes;
    Release(base,EngineEvent,s,100);Check(modes==calls,"recycled movement serial cannot be written");Put<int>(items,Movement*24+16,Movement+1);
    // Real integration: native action -> old ground jump -> apex -> hover ->
    // grace-period re-jump. The option OFF preserves the original jump path.
    dg::mounted_jump::g_players[0]={};Mode(1);Down(false);Tick();Press();Check(jumps==1,"ground jump retained");
    Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==3,"holding does not cancel upward leap");
    Mode(3,0);Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==5,"holding at apex enters air walk");
    Down(false);Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==5,"release grace retains height");Down(true);Tick();Check(jumps==2&&Get<unsigned char>(objects[Movement],0x1B0)==3,"fresh press within grace reuses native DoJump");
    Mode(3,0);Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==5,"holding after re-jump can hover again");
    Tick(false);Check(Get<unsigned char>(objects[Movement],0x1B0)==5,"mod editor preserves owned hover");Tick();Check(jumps==2,"closing editor while held creates no jump");
    dg::runtime::Get().mountedAirWalkEnabled=false;Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==3,"turning air walk OFF restores Falling");
    Mode(1);Press();Check(jumps==3,"old ground jump works with air walk OFF");Mode(3,-1);Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==3,"OFF cannot acquire hover");
    dg::runtime::Get().mountedAirWalkEnabled=true;Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==3,"enabling while held cannot catch a preexisting fall");
    Press();Check(Get<unsigned char>(objects[Movement],0x1B0)==5,"fresh airborne press enters air walk");Tick(true,false);Check(Get<unsigned char>(objects[Movement],0x1B0)==3,"Alt-Tab restores Falling");
    Tick();Check(Get<unsigned char>(objects[Movement],0x1B0)==3,"focus restore with held action does not hover");Press();paused=true;Tick();paused=false;Check(Get<unsigned char>(objects[Movement],0x1B0)==3,"pause cancels owned air physics");
    Down(false);Tick();Press();owner.world=200;owner.kind=dg::local_players::PoseKind::Foot;const auto old=modes;Tick();Check(modes==old,"changed world never modifies the old horse");
    dg::runtime::Snapshot config{};config.mountedAirWalkReleaseDelay=std::numeric_limits<float>::quiet_NaN();dg::runtime::Publish(config);
    Check(!dg::runtime::Get().mountedAirWalkEnabled.load()&&dg::runtime::Get().mountedAirWalkReleaseDelay.load()==.2f,"default OFF and nonfinite delay normalized");
    Policy p{};Check(Decide(p,false,3,-1,true,false,100,200)==Action::None,"held enable requires release/new press");Jumped(p);
    Check(Decide(p,false,3,2,true,false,100,200)==Action::None&&Decide(p,false,3,0,true,false,101,200)==Action::Hover,"apex decision");
    Jumped(p);Decide(p,false,3,2,false,false,100,200);Check(Decide(p,false,3,-2,true,false,101,200)==Action::None,"release before hover means normal falling");
    p={};Check(Decide(p,true,5,0,false,false,100,200)==Action::None&&Decide(p,true,5,0,false,false,300,200)==Action::Fall,"grace expires");
    p={};Check(Decide(p,true,5,0,false,false,100,0)==Action::Fall,"zero delay falls immediately");
    p={};Decide(p,true,5,0,false,false,100,200);Check(Decide(p,true,5,0,false,false,99,200)==Action::Fall,"clock rollback exits safely");
    p={};p.blocked=true;Check(Decide(p,false,3,-2,true,true,100,200)==Action::None,"failed contract does not retry every frame");Decide(p,false,3,0,false,false,101,200);Check(Decide(p,false,3,-2,true,true,102,200)==Action::Hover,"release rearms failed contract");
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return cinema;}bool RadialActive(unsigned){return radial;}}
namespace dg::local_players {bool Matches(unsigned,uintptr_t,uint32_t){return matched;}}
int main(){try{Run();std::cout<<"Mounted air walk contracts and lifecycle checks passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
