#include "../src/GrappleRangeFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>
namespace {
unsigned char actor[0x1100]{},ability[0x280]{};unsigned calls=0;bool local=true,paused=false;
dg::local_players::PoseKind pose=dg::local_players::PoseKind::Foot;
float acquisition=0,relay=0,pull=0;
template<class T> void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
template<class T> T Get(void* p,size_t at){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+at,sizeof(value));return value;}
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
void Target(void* p){++calls;acquisition=Get<float>(p,0x1E0);relay=Get<float>(p,0x220);}
void Pull(void* p){++calls;pull=Get<float>(p,0x240);}
void Run(){
    using namespace dg::grapple;g_base=0x140000000;g_ready=true;g_target=Target;g_pull=Pull;
    Put<unsigned char>(actor,0x118,3);Put(ability,0x48,static_cast<void*>(actor));
    Put(ability,0,g_base+0x27A32B0);Put(ability,0x1E0,1800.f);Put(ability,0x220,1500.f);
    const std::vector<unsigned char> baseline(ability,ability+sizeof(ability));
    auto& rt=dg::runtime::Get();rt.grappleRangeEnabled=true;rt.grappleRangeMultiplier=3;
    HookTarget(ability);Check(acquisition==5400&&relay==4500,"both native finders receive the scaled radii together");
    Check(std::memcmp(ability,baseline.data(),sizeof(ability))==0,"initialization restores every ability byte afterwards");
    rt.grappleRangeEnabled=false;HookTarget(ability);Check(acquisition==1800&&relay==1500,"OFF passes native acquisition/relay unchanged");
    rt.grappleRangeEnabled=true;local=false;HookTarget(ability);Check(acquisition==1800,"foreign actors are excluded");local=true;
    paused=true;HookTarget(ability);Check(acquisition==1800,"pause cannot start a range override");paused=false;
    pose=dg::local_players::PoseKind::Horse;HookTarget(ability);Check(acquisition==1800,"transformed/mounted state stays native");pose=dg::local_players::PoseKind::Foot;
    Put<uint32_t>(ability,8,0x10);HookTarget(ability);Check(acquisition==1800,"class default objects are excluded");Put<uint32_t>(ability,8,0);
    Put(ability,0x220,-1.f);HookTarget(ability);Check(acquisition==1800&&relay==-1,"one invalid radius leaves the whole native initialization untouched");
    Put(ability,0x220,1500.f);Put(ability,0,g_base+0x27A2BA0);Put(ability,0x240,1200.f);
    HookPull(ability);Check(pull==3600&&Get<float>(ability,0x240)==1200,"pull endpoint receives same factor, original field is restored");
    rt.grappleRangeMultiplier=std::numeric_limits<float>::quiet_NaN();HookPull(ability);Check(pull==1200,"nonfinite factor falls back to native");
    Check(calls==9,"every native callback executes exactly once");
}
}
namespace dg::native_tps {bool OnGameThread(){return true;}bool CinematicActive(unsigned){return false;}bool IsWorldPaused(void*){return paused;}}
namespace dg::local_players {bool ResolveActor(void* p,Context& c){if(!local||p!=actor)return false;c={};c.index=1;c.character=reinterpret_cast<uintptr_t>(actor);c.kind=pose;return true;}}
int main(){try{Run();std::cout<<"Grapple native initialization, scope and byte restoration passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
