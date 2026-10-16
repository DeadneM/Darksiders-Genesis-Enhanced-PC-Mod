#include "../src/GlideFeature.cpp"
#include "../src/HorseFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
alignas(16) unsigned char actor[0xE00]{},horse[0xA00]{},component[0xB00]{},movementClass[0x80]{};
void* tree[1]{movementClass+0x30};
bool owned=true;
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
    dg::horse::HorseSlot slot{};slot.horse=horse;slot.staminaFieldsReady=true;slot.nativeStaminaSprintRate=10;
    Put(horse,0x9C0,80.0f);Put(horse,0x9C4,100.0f);
    dg::horse::g_sprintDurationEnabled=true;dg::horse::g_sprintDurationMultiplier=5;
    rt.unlimitedHorseSprintEnabled=false;dg::horse::ApplySlotLocked(slot);
    Check(Get<float>(horse,0x918)==2,"validated horse duration multiplier stays unchanged when Unlimited is OFF");
    rt.unlimitedHorseSprintEnabled=true;dg::horse::ApplySlotLocked(slot);
    Check(Get<float>(horse,0x918)==0,"Unlimited stops native sprint drain for an owned horse");
    owned=false;dg::horse::ApplySlotLocked(slot);
    Check(Get<float>(horse,0x918)==2,"foreign horse retains ordinary configured drain");owned=true;
    rt.unlimitedHorseSprintEnabled=false;dg::horse::ApplySlotLocked(slot);
    Check(Get<float>(horse,0x918)==2,"horse Unlimited OFF restores configured multiplier");
}
}
namespace dg::native_tps {bool OnGameThread(){return true;}}
namespace dg::local_players {
bool ResolveActor(void* a,Context& c){if(!owned||(a!=actor&&a!=horse))return false;c={};c.index=1;c.character=reinterpret_cast<uintptr_t>(actor);c.pawn=reinterpret_cast<uintptr_t>(a);c.kind=a==horse?PoseKind::Horse:PoseKind::Foot;return true;}
bool CoopActive(){return true;}
}
int main(){try{Run();std::cout<<"Actual glide tick and horse sprint budget/OFF/scope checks passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
