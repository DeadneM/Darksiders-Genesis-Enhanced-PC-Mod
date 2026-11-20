#include "../src/GlideFeature.cpp"
#include "../src/HorseFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>
namespace {
alignas(16) unsigned char actor[0xE00]{},horseActor[0xA00]{},component[0xB00]{},movementClass[0x80]{};
alignas(16) unsigned char controller[0x400]{};
void* tree[1]{movementClass+0x30};
bool owned=true;
bool gameThread=true,paused=false,cinema=false;int ownerIndex=1;
unsigned calls=0;
template<class T> void Put(void* p,size_t o,T v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,sizeof(v));}
template<class T> T Get(void* p,size_t o){T v{};std::memcpy(&v,static_cast<unsigned char*>(p)+o,sizeof(v));return v;}
void Check(bool v,const char* msg){if(!v)throw std::runtime_error(msg);}
void NativeTick(void* c,float dt,int type,void* function){
    Check(c==component&&dt==0.25f&&type==2&&function==reinterpret_cast<void*>(0x1234),"all native tick arguments are forwarded unchanged");
    if(actor[0xD14]&1)Put(c,0x9F0,Get<float>(c,0x9F0)+dt);
    ++calls;
}
void Tick(){dg::glide::HookTick(component,0.25f,2,reinterpret_cast<void*>(0x1234));}
void Run(){
    std::vector<unsigned char> image(0x3900000);
    dg::glide::g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x380FFF0,static_cast<void*>(movementClass));Put(movementClass,0x30,tree+0);Put(movementClass,0x38,0);
    Put(component,0x10,static_cast<void*>(movementClass));Put(component,0x850,static_cast<void*>(actor));
    Put(component,0x9F0,1.0f);Put(component,0x86C,3.0f);actor[0xD14]=1;
    dg::glide::g_ready=true;dg::glide::g_original=NativeTick;
    auto& rt=dg::runtime::Get();rt.unlimitedGlideEnabled=true;
    for(unsigned i=0;i<100;++i)Tick();
    Check(Get<float>(component,0x9F0)==1&&Get<float>(component,0x86C)==3&&calls==100,"active glide budget stays fixed while native physics ticks continue and duration stays unchanged");
    rt.unlimitedGlideEnabled=false;Tick();
    Check(Get<float>(component,0x9F0)==1.25f,"OFF resumes consumption from the held native budget");
    rt.unlimitedGlideEnabled=true;owned=false;Tick();
    Check(Get<float>(component,0x9F0)==1.5f,"foreign movement cannot use Unlimited glide");owned=true;
    actor[0xD14]=0;Tick();Check(Get<float>(component,0x9F0)==1.5f,"release or landing leaves native non-glide state alone");
    dg::horse::HorseSlot slot{};slot.horse=horseActor;slot.staminaFieldsReady=true;slot.nativeStaminaSprintRate=10;
    Put(horseActor,0x9C0,80.0f);Put(horseActor,0x9C4,100.0f);
    dg::horse::g_sprintDurationEnabled=true;dg::horse::g_sprintDurationMultiplier=5;
    rt.unlimitedHorseSprintEnabled=false;dg::horse::ApplySlotLocked(slot);
    Check(Get<float>(horseActor,0x918)==2,"validated horse duration multiplier stays unchanged when Unlimited is OFF");
    rt.unlimitedHorseSprintEnabled=true;dg::horse::ApplySlotLocked(slot);
    Check(Get<float>(horseActor,0x918)==0,"Unlimited stops native sprint drain for an owned horse");
    owned=false;dg::horse::ApplySlotLocked(slot);
    Check(Get<float>(horseActor,0x918)==2,"foreign horse retains ordinary configured drain");owned=true;
    rt.unlimitedHorseSprintEnabled=false;dg::horse::ApplySlotLocked(slot);
    Check(Get<float>(horseActor,0x918)==2,"horse Unlimited OFF restores configured multiplier");

    dg::horse::g_summonFloorBase=reinterpret_cast<uintptr_t>(image.data());
    const auto caller=dg::horse::g_summonFloorBase+dg::horse::kSummonFloorReturnRva;
    Put(actor,0x348,reinterpret_cast<uintptr_t>(controller));Put(controller,0x350,reinterpret_cast<uintptr_t>(actor));
    Put(actor,0x388,reinterpret_cast<uintptr_t>(component));Put(component,0xE8,reinterpret_cast<uintptr_t>(actor));
    actor[0x118]=3;component[0x1B0]=3;component[0x1B1]=0;
    const dg::horse::SummonVector position{10,20,350};
    const dg::horse::SummonFloor original{{1,2,3},false,{4,5,6}};
    auto floor=original;rt.horseAirSummonEnabled=false;
    Check(!dg::horse::AirSummonFloor(caller,actor,&position,floor)&&!std::memcmp(&floor,&original,sizeof(floor)),"air summon OFF preserves the complete native result");
    rt.horseAirSummonEnabled=true;
    Check(dg::horse::AirSummonFloor(caller,actor,&position,floor)&&floor.valid&&floor.position.x==10&&floor.position.y==20&&floor.position.z+100==position.z&&floor.padding[2]==6,
        "local P2 falling summon compensates native rise without editing player physics or padding");
    ownerIndex=0;floor=original;
    Check(dg::horse::AirSummonFloor(caller,actor,&position,floor),"same native action supports P1 independently of camera and bindings");ownerIndex=1;
    floor=original;component[0x1B0]=1;
    Check(!dg::horse::AirSummonFloor(caller,actor,&position,floor)&&!std::memcmp(&floor,&original,sizeof(floor)),"ground summons keep native floor and failure result");
    component[0x1B0]=6;component[0x1B1]=6;actor[0xD14]=1;
    Check(dg::horse::AirSummonFloor(caller,actor,&position,floor),"verified native glide can summon in the air");
    actor[0xD14]=0;Check(!dg::horse::AirSummonFloor(caller,actor,&position,floor),"custom mode without live glide does not qualify");
    component[0x1B1]=2;actor[0xD14]=1;Check(!dg::horse::AirSummonFloor(caller,actor,&position,floor),"unrelated custom movement is not changed");
    component[0x1B0]=3;component[0x1B1]=0;
    Check(!dg::horse::AirSummonFloor(caller+1,actor,&position,floor),"other native users of the floor helper are never changed");
    owned=false;Check(!dg::horse::AirSummonFloor(caller,actor,&position,floor),"foreign actor is never authorized");owned=true;
    gameThread=false;Check(!dg::horse::AirSummonFloor(caller,actor,&position,floor),"off-thread callback keeps native result");gameThread=true;
    paused=true;Check(!dg::horse::AirSummonFloor(caller,actor,&position,floor),"paused world keeps native result");paused=false;
    cinema=true;Check(!dg::horse::AirSummonFloor(caller,actor,&position,floor),"cinematic keeps native result");cinema=false;
    Put(controller,0x350,uintptr_t(1));Check(!dg::horse::AirSummonFloor(caller,actor,&position,floor),"broken reciprocal possession cannot authorize a summon");
    Put(controller,0x350,reinterpret_cast<uintptr_t>(actor));actor[0x118]=2;
    Check(!dg::horse::AirSummonFloor(caller,actor,&position,floor),"non-authority player keeps native result");actor[0x118]=3;
    const dg::horse::SummonVector invalid{10,20,std::numeric_limits<float>::infinity()};
    Check(!dg::horse::AirSummonFloor(caller,actor,&invalid,floor),"invalid position cannot create a summon point");
    rt.horseAirSummonEnabled=false;
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return cinema;}}
namespace dg::local_players {
bool ResolveActor(void* a,Context& c){if(!owned||(a!=actor&&a!=horseActor))return false;c={};c.index=ownerIndex;c.controller=reinterpret_cast<uintptr_t>(controller);c.character=reinterpret_cast<uintptr_t>(actor);c.pawn=reinterpret_cast<uintptr_t>(a);c.kind=a==horseActor?PoseKind::Horse:PoseKind::Foot;return true;}
bool CoopActive(){return true;}
}
int main(){try{Run();std::cout<<"Actual glide tick and horse sprint budget/OFF/scope checks passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
