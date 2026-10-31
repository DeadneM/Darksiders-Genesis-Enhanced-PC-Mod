#include "../src/NativeTpsFeature.cpp"
#include <iostream>
#include <stdexcept>
namespace {
using namespace dg::native_tps;
int actor=0,controller=0,other=0;
bool enabled=true,throwQuery=false;
unsigned nativeCalls=0,fallbackCalls=0,outsideCalls=0;
Vec2 nativeResult{.8f,.6f};
void Check(bool good,const char* text){if(!good)throw std::runtime_error(text);}
bool Provider(void* owner,MouseRay& ray){Check(owner==&actor,"same native actor");ray.origin={-300,0,300};ray.direction={1,0,0};return enabled;}
bool Outside(void*,const Vec2*,Vec3*,Vec3*){++outsideCalls;return false;}
Vec2* Original(void* pc,Vec2* out,void* owner,const Vec3* reference,bool flag,void* extra){
    ++nativeCalls;Check(pc==&controller&&owner==&actor&&reference&&flag&&extra==&other,"all original native arguments forwarded");
    if(throwQuery)throw std::runtime_error("query unwind");
    if(g_scope){
        Vec3 origin{},direction{};Vec2 pixel{33,44};
        Check(ScopedDeproject(pc,&pixel,&origin,&direction,g_base+0x571A77),"native 2D initial deprojection replaced");
        Check(origin.x==-300&&direction.x==1&&pixel.x==33,"same TPS ray without writing native cursor coordinates");
        Check(ScopedDeproject(pc,&pixel,&origin,&direction,g_base+0x1AC95E6),"nested native under-cursor hit uses same TPS ray");
    }
    *out=nativeResult;return out;
}
Vec3* Fallback(void* pc,Vec3* out,void* owner,NativeTarget* cache,bool,void*){
    ++fallbackCalls;Check(pc==&controller&&owner==&actor,"3D fallback keeps original owner/controller");Vec3 origin{},direction{};
    Check(ScopedDeproject(pc,nullptr,&origin,&direction,g_base+0x571EB8),"accepted forward 3D fallback scope retained");
    *out={1000,0,300};*cache=NativeTarget{*out,1};return out;
}
void Run(){
    g_ready=true;g_base=0x100000;g_provider=Provider;g_aim2=Original;g_aim3=Fallback;g_deproject=Outside;
    Vec2 out{};const Vec3 reference{};
    auto invoke=[&](){return HookAim2(&controller,&out,&actor,&reference,true,&other);};
    Check(invoke()==&out&&out.x==.8f&&out.y==.6f&&nativeCalls==1&&!fallbackCalls,"native under-cursor result precedes the old projectile-only replacement");
    Check(!g_scope,"query leaves no scope active");
    nativeResult={0,0};invoke();Check(out.x==1&&out.y==0&&nativeCalls==2&&fallbackCalls==1,"horizontal-plane miss falls forward and never reruns original query");
    nativeResult={-1,0};invoke();Check(out.x==1&&fallbackCalls==2,"behind-plane result cannot reintroduce reversed fire");
    enabled=false;invoke();Check(out.x==-1&&fallbackCalls==2&&nativeCalls==4,"native view/provider rejection passes through without TPS mutation");enabled=true;
    const MouseRay ray{{-300,0,300},{1,0,0}};Vec3 origin{},direction{};
    {ScopedRay scope(&controller,ray,true);
        Check(!ScopedDeproject(&other,nullptr,&origin,&direction,g_base+0x571A77),"another controller cannot borrow this player's ray");
        Check(!ScopedDeproject(&controller,nullptr,&origin,&direction,g_base+0x123),"unrelated nested call stays native");}
    Check(outsideCalls==2,"unrelated deprojection calls original once each");
    {ScopedRay scope(&controller,ray);Check(!ScopedDeproject(&controller,nullptr,&origin,&direction,g_base+0x1AC95E6),"under-cursor site is scoped to original2D query only");}
    throwQuery=true;try{invoke();}catch(const std::runtime_error&){}throwQuery=false;
    Check(!g_scope,"query unwind restores previous scope");
}
}
namespace dg::local_players {
bool Ready(){return false;}
bool CameraCinematic(unsigned){return false;}
Slot PublishedSlot(unsigned){return {};}
bool ResolveActor(void*,Context&){return false;}
}
int main(){try{Run();std::cout<<"Original cursor targeting, scoped nested deprojection and forward fallback passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
