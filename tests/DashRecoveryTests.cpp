#include "../src/ActionRecoveryFeature.cpp"
#include <array>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
namespace {
struct Player {
    alignas(16) unsigned char actor[0x1000]{},ability[0x240]{},component[0x110]{},movement[0x200]{},resource[0x400]{};
    void* active[1]{};
};
Player players[2];
unsigned char cls[0x80]{};
void* tree[1]{cls+0x30};
uintptr_t table[0x300/8]{};
uintptr_t actionTable[4]{};uint64_t nativeActions[2]{};
dg::local_players::Context owners[2]{};
bool gameThread=true,paused=false,cinema=false,endDuringTick=false;
unsigned nativeCalls=0,finishCalls[2]{};float actualDt=0;std::vector<int> order;
template<class T> void Put(void* p,size_t o,T value){std::memcpy(static_cast<unsigned char*>(p)+o,&value,sizeof(value));}
template<class T> T Get(void* p,size_t o){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+o,sizeof(value));return value;}
void Check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
int Index(void* a){return a==players[1].ability?1:0;}
unsigned char ActionState(void* p,unsigned char action){
    const auto actor=static_cast<unsigned char*>(p)-0x8D0;
    return (nativeActions[actor==players[1].actor?1:0]&(uint64_t(1)<<action))?1:0;
}
void NativeTick(void* a,float dt){
    ++nativeCalls;actualDt=dt;order.push_back(1);
    Put(a,0xDC,Get<float>(a,0xDC)+dt);
    Put(a,0x224,Get<float>(a,0x224)-dt);
    if(endDuringTick)Put<unsigned char>(a,0xD8,5);
}
void NativeFinish(void* a,bool interrupted){
    Check(!interrupted,"recovery uses normal native finish, not interruption");
    ++finishCalls[Index(a)];order.push_back(2);Put<unsigned char>(a,0xD8,5);
}
void Fresh(){
    for(auto& p:players)p={};
    owners[0]={};owners[1]={};order.clear();nativeCalls=0;finishCalls[0]=finishCalls[1]=0;
    gameThread=true;paused=cinema=endDuringTick=false;
    auto& rt=dg::runtime::Get();rt.actionRecoveryEnabled=true;rt.actionRecoveryDelayMs=0;
    dg::recovery::g_ready=true;dg::recovery::g_tick=NativeTick;dg::recovery::g_finish=NativeFinish;
    dg::recovery::g_phase[0]={};dg::recovery::g_phase[1]={};
    nativeActions[0]=nativeActions[1]=0;actionTable[3]=reinterpret_cast<uintptr_t>(&ActionState);
    Put(cls,0x30,tree+0);Put(cls,0x38,0);
    table[0x2E0/8]=dg::recovery::g_base+0x5B93F0;table[0x2A8/8]=dg::recovery::g_base+0x59C110;
    for(int i=0;i<2;++i){
        auto& p=players[i];p.active[0]=p.ability;
        Put(p.actor,0xC70,static_cast<void*>(p.component));Put<unsigned char>(p.actor,0x118,3);
        Put(p.actor,0x8D0,actionTable+0);
        Put<unsigned char>(p.actor,0xC18,static_cast<unsigned char>(i+1));Put(p.actor,0x388,static_cast<void*>(p.movement));
        Put(p.actor,0xEA0,static_cast<void*>(p.resource));Put(p.resource,0x10,static_cast<void*>(cls));
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
void Tick(int i,ULONGLONG now,float dt=0.016f){dg::recovery::Tick(players[i].ability,dt,now);}
void Cross(int i,ULONGLONG now){Put(players[i].ability,0x224,0.005f);Tick(i,now);}
void Unchanged(const std::function<void()>& alter,const char* message){
    Fresh();alter();Tick(0,100);Cross(0,116);Check(!finishCalls[0],message);
}
void Run(){
    std::vector<unsigned char> image(0x3900000);
    dg::recovery::g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x3810F90,static_cast<void*>(cls));
    Put(image.data(),0x3810268,static_cast<void*>(cls));
    Fresh();const auto actorBefore=std::to_array(players[0].actor);
    Tick(0,100);Check(!finishCalls[0],"positive deceleration keeps full native movement");
    Cross(0,116);Check(finishCalls[0]==1&&nativeCalls==2&&actualDt==0.016f,"first completed deceleration requests native finish after exact original dt");
    Check(order==std::vector<int>({1,1,2}),"native movement runs before recovery cleanup");
    Check(std::memcmp(players[0].actor,actorBefore.data(),actorBefore.size())==0,"module does not patch actor aim, movement flags, speed or transform");
    Tick(0,132);Check(finishCalls[0]==1,"finished action cannot request cleanup twice");
    Fresh();Put(players[0].ability,0x228,0.0f);Tick(0,100);Tick(0,116);Check(!finishCalls[0],"unarmed zero constructor clock is not a recovery phase");
    Fresh();Put(players[0].ability,0x224,-0.1f);Tick(0,100);Tick(0,116);Check(!finishCalls[0],"old negative timer without a positive observation fails open");
    Fresh();dg::runtime::Get().actionRecoveryDelayMs=50;Tick(0,100);Cross(0,116);
    Check(!finishCalls[0],"configured delay preserves the beginning of the tail");
    Tick(0,132);Tick(0,148);Tick(0,164);Check(finishCalls[0]==1,"delay is measured from exhausted native deceleration");
    Fresh();Tick(0,100);Tick(1,100);Cross(0,116);Check(finishCalls[0]==1&&!finishCalls[1],"P1 finish cannot consume P2 phase");
    Cross(1,116);Check(finishCalls[1]==1,"P2 uses its own live owner and native finish");
    Fresh();Tick(0,100);dg::runtime::Get().actionRecoveryEnabled=false;Cross(0,116);
    dg::runtime::Get().actionRecoveryEnabled=true;Tick(0,132);Check(!finishCalls[0],"OFF clears the phase and ON cannot reuse a stale arm");
    Fresh();Tick(0,100);Cross(0,500);Check(!finishCalls[0],"expired observations fail open");
    Fresh();Tick(0,100);Put(players[0].ability,0xDC,0.0f);Cross(0,116);Check(!finishCalls[0],"reused ability activation must arm again");
    Fresh();Tick(0,100);owners[0].localPlayer=0xDEAD;Cross(0,116);Check(!finishCalls[0],"owner replacement cannot reuse the old phase");
    Unchanged([]{gameThread=false;},"other threads cannot finish actions");
    for(unsigned action:{18u,20u,31u}){
        Fresh();Tick(0,100);nativeActions[0]=uint64_t(1)<<action;Cross(0,116);
        Check(!finishCalls[0],"native aim/fire protects dash animation, equipped ammunition and queued action lifecycle");
        nativeActions[0]=0;Tick(0,132);Check(!finishCalls[0],"release cannot reuse a combat-cancelled recovery arm");
    }
    Unchanged([]{Put(players[0].actor,0x8D0,static_cast<void*>(nullptr));},"unknown action interface keeps the full native dash");
    Unchanged([]{Put(players[0].ability,0x48,static_cast<void*>(players[0].component));},"an ability component cannot masquerade as the reflected actor Instigator");
    Unchanged([]{paused=true;},"pause keeps native timing");
    Unchanged([]{cinema=true;},"cinematic keeps native timing");
    Unchanged([]{endDuringTick=true;},"native completion during original Tick wins");
    Unchanged([]{owners[0].kind=dg::local_players::PoseKind::Horse;},"horse actions stay native");
    Unchanged([]{owners[0].kind=dg::local_players::PoseKind::Ultimate;},"Ultimate actions stay native");
    Unchanged([]{owners[0].index=2;},"unknown player slot stays native");
    Unchanged([]{Put<unsigned char>(players[0].actor,0x118,2);},"non-authority actor stays native");
    Unchanged([]{Put(players[0].resource,0x37C,0.0f);},"dead actor stays native");
    Unchanged([]{table[0x2E0/8]=0;},"unproved dash physics variant stays native");
    Unchanged([]{table[0x2A8/8]=0;},"unproved cleanup variant stays native");
    Unchanged([]{players[0].active[0]=nullptr;},"ability outside current component list stays native");
    Unchanged([]{Put(players[0].component,0xE8,static_cast<void*>(players[1].actor));},"nonreciprocal component stays native");
    Unchanged([]{Put(players[0].component,0x104,100);},"invalid ability array stays native");
    Unchanged([]{dg::runtime::Get().actionRecoveryDelayMs=std::numeric_limits<float>::quiet_NaN();},"nonfinite setting stays native");
    Fresh();Tick(0,100,-0.016f);Check(nativeCalls==1&&actualDt==-0.016f&&!finishCalls[0],"invalid dt is forwarded but never used for recovery");
}
}
namespace dg::native_tps {
bool OnGameThread(){return gameThread;}
bool IsWorldPaused(void*){return paused;}
bool CinematicActive(unsigned){return cinema;}
}
namespace dg::local_players {
bool ResolveActor(void* a,Context& result){for(int i=0;i<2;++i)if(a==players[i].actor){result=owners[i];return true;}return false;}
}
int main(){try{Run();std::cout<<"Native dash completion: phase, owners, cleanup order and fail-open cases passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
