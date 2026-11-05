#include "../src/AirDashFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::air_dash;
struct Fixture {
    unsigned char actor[0x1100]{},controller[0x400]{},resource[0x400]{},component[0x120]{};
    unsigned char ability[0x240]{},airClass[0x100]{},finish[0x100]{},items[4*24]{};
    uintptr_t active[1]{},chunks[1]{};
} f;
bool thread=true,paused=false,cinema=false,radial=false,matches=true;
template<class T>void Put(void* p,size_t at,T v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
template<class T>T At(void* p,size_t at){T v{};std::memcpy(&v,static_cast<unsigned char*>(p)+at,sizeof(v));return v;}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
Id Live(void* object,int i){Put(object,0xC,i);Put(f.items,size_t(i)*24,reinterpret_cast<uintptr_t>(object));Put(f.items,size_t(i)*24+16,i+1);return {reinterpret_cast<uintptr_t>(object),i,i+1};}
void Fresh(std::vector<unsigned char>& image){
    f={};g_players={};thread=matches=true;paused=cinema=radial=false;g_ready=true;g_base=reinterpret_cast<uintptr_t>(image.data());
    f.chunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(f.chunks));Put(image.data(),0x3959694,4);
    auto& p=g_players[0];p.actor=Live(f.actor,0);g_air=Live(f.airClass,1);Live(f.ability,2);g_finish=Live(f.finish,3);
    p.allowed=p.strife=p.await=true;p.request=Gesture::Forward;p.until=GetTickCount64()+1000;p.generation=7;
    auto& c=p.context;c.index=0;c.localPlayer=1;c.world=2;c.character=c.pawn=reinterpret_cast<uintptr_t>(f.actor);c.controller=reinterpret_cast<uintptr_t>(f.controller);
    Put(f.actor,0x348,reinterpret_cast<uintptr_t>(f.controller));Put(f.controller,0x350,reinterpret_cast<uintptr_t>(f.actor));Put<unsigned char>(f.actor,0x118,3);
    Put(f.actor,0xEA0,reinterpret_cast<uintptr_t>(f.resource));Put(f.resource,0x37C,100.f);
    Put(f.actor,0xC70,reinterpret_cast<uintptr_t>(f.component));Put(f.component,0xE8,reinterpret_cast<uintptr_t>(f.actor));
    f.active[0]=reinterpret_cast<uintptr_t>(f.ability);Put(f.component,0xF8,reinterpret_cast<uintptr_t>(f.active));Put(f.component,0x100,1);Put(f.component,0x104,1);
    Put(f.ability,0x10,reinterpret_cast<uintptr_t>(f.airClass));Put(f.ability,0x48,reinterpret_cast<uintptr_t>(f.actor));Put(f.ability,0x1B0,reinterpret_cast<uintptr_t>(f.actor));
    Put<unsigned char>(f.ability,0xD8,2);Put(f.ability,0xDC,.05f);Put(f.ability,0x44,3.f);Put(f.ability,0x1F8,5000.f);Put(f.ability,0x1FC,0.f);
}
void NativeValues(){Check(At<float>(f.ability,0x44)==3&&At<float>(f.ability,0x1F8)==5000&&At<float>(f.ability,0x1FC)==0,"rejected instance retains all original native parameters");}
void Run(){
    std::vector<unsigned char> image(0x3960000);
    Fresh(image);unsigned char reference[16]{};
    Check(Reference(reference,{0,8,"None"})&&!Reference(reference,{0,8,"ARealObject"}),"a null cooked reference is allowed only where None is explicitly expected");
    Put(reference,0,reinterpret_cast<uintptr_t>(f.airClass));Check(!Reference(reference,{0,8,"None"}),"a non-null object cannot substitute for a declared null property");
    Fresh(image);BeforeTick(f.ability);
    Check(g_players[0].saved.ability&&!g_players[0].await,"only the queued owner's active AirDash is adopted once");
    Check(At<float>(f.ability,0x1F8)==0&&At<float>(f.ability,0x1FC)==3000&&std::abs(At<float>(f.ability,0x44)-.29f)<.00001f,"native forward physics has a finite deadline relative to native elapsed time");
    BeforeTick(f.ability);Put<unsigned char>(f.ability,0xD8,4);AfterTick(f.ability);NativeValues();Check(!g_players[0].saved.ability,"native completion restores original parameters");
    Fresh(image);BeforeTick(f.ability);Put<unsigned char>(f.ability,0xD8,5);const auto c=g_players[0].context;OnLivePlayer(c,7,true);NativeValues();
    Check(!g_players[0].saved.ability,"completion after the tick wrapper is restored by the live service too");
    Fresh(image);BeforeTick(f.ability);Put(f.ability,0x1FC,123.f);Put<unsigned char>(f.ability,0xD8,4);AfterTick(f.ability);
    Check(At<float>(f.ability,0x1FC)==123&&At<float>(f.ability,0x1F8)==5000,"restoration preserves a later external parameter change");
    for(auto gesture:{Gesture::Down,Gesture::Forward}){
        Fresh(image);g_players[0].request=gesture;BeforeTick(f.ability);
        if(gesture==Gesture::Down)NativeValues();
        Put<unsigned char>(f.ability,0xD8,5);AfterTick(f.ability);
        Check(bool(g_players[0].gestureAbility),"completion marker survives parameter restoration until the actual native callback");
        unsigned char original=38,copy=255;
        Check(FinishParameters(f.ability,f.finish,&original,copy)&&original==38&&copy==0,"touchdown uses a private cause copy to prevent an unwanted second ground dash");
        Check(!g_players[0].gestureAbility&&!FinishParameters(f.ability,f.finish,&original,copy),"one callback consumes its own gesture marker only once");NativeValues();
    }
    Fresh(image);BeforeTick(f.ability);Put<unsigned char>(f.ability,0xD8,5);unsigned char original=47,copy=255;
    Check(!FinishParameters(f.ability,f.finish,&original,copy)&&original==47&&copy==255&&!g_players[0].gestureAbility,"timeout and cancellation preserve their original callback cause");NativeValues();
    Fresh(image);BeforeTick(f.ability);original=38;
    Check(!FinishParameters(f.ability,f.finish,&original,copy)&&g_players[0].gestureAbility,"callback before native cleanup cannot replace the cause");
    Put<unsigned char>(f.ability,0xD8,5);
    Check(!FinishParameters(f.ability,f.airClass,&original,copy)&&g_players[0].gestureAbility,"another callback is never filtered");
    matches=false;Check(!FinishParameters(f.ability,f.finish,&original,copy)&&g_players[0].gestureAbility,"ownership change cannot rewrite a callback");
    for(int guard=0;guard<15;++guard){Fresh(image);auto& p=g_players[0];
        switch(guard){
        case 0:thread=false;break;case 1:p.await=false;break;case 2:p.until=0;break;
        case 3:paused=true;break;case 4:cinema=true;break;case 5:radial=true;break;
        case 6:p.context.kind=dg::local_players::PoseKind::Horse;break;
        case 7:Put(f.ability,0x48,uintptr_t(0));break;case 8:f.active[0]=0;break;
        case 9:Put(f.component,0x104,0);break;case 10:Put<unsigned char>(f.ability,0xD8,1);break;
        case 11:Put(f.ability,8,uint32_t(0x10));break;case 12:Put(f.ability,0x44,4.f);break;
        case 13:matches=false;break;case 14:Put(f.resource,0x37C,0.f);break;
        }
        const float timeout=At<float>(f.ability,0x44);BeforeTick(f.ability);
        Check(!p.saved.ability&&At<float>(f.ability,0x44)==timeout&&At<float>(f.ability,0x1F8)==5000&&At<float>(f.ability,0x1FC)==0,"unknown, inactive, foreign, dead, menu or unmatched instance is never edited");
    }
    Fresh(image);BeforeTick(f.ability);Put(f.items,2*24+16,99);Put<unsigned char>(f.ability,0xD8,4);AfterTick(f.ability);
    Check(At<float>(f.ability,0x1F8)==0&&!g_players[0].saved.ability,"object-address reuse is not restored through an expired identity");
    Fresh(image);g_players[1]=g_players[0];g_players[0]={};g_players[1].context.index=1;BeforeTick(f.ability);
    Check(g_players[1].saved.ability&&!g_players[0].saved.ability,"queued forward movement remains independent per player");
    Shutdown();NativeValues();
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return cinema;}bool RadialActive(unsigned){return radial;}}
namespace dg::local_players {bool Matches(unsigned,uintptr_t,uint32_t){return matches;}}
namespace dg::dash_combat {bool TickReady(){return true;}}
int main(){try{Run();std::cout<<"Air dash native-instance boundaries and restoration passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
