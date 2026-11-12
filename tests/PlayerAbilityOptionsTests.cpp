#include "../src/PlayerAbilityOptions.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
namespace {
using namespace dg::player_abilities;
std::vector<unsigned char> image(0x3B00000);
unsigned char objects[14][0x1100]{},items[14*24]{},nameData[24][128]{};
uintptr_t names[24]{},nameChunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{reinterpret_cast<uintptr_t>(items)};
std::vector<std::string> labels;
enum Object{Actor,Controller,Resource,Component,Ability,StrifeClass,AbilityClass,Finish,FunctionClass,WarClass,WarAbilityClass,World,Instance};
bool gameThread=true,paused=false,cinema=false,radial=false,matched=true,resolves=true,startSucceeds=true;
unsigned queries=0,requests=0,casts=0;unsigned char lastRequest=255;
dg::local_players::Context context{};uintptr_t active[2]{};
template<class T>void Put(void* p,size_t offset,T value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
uintptr_t P(unsigned i){return reinterpret_cast<uintptr_t>(objects[i]);}
void Check(bool b,const char* why){if(!b)throw std::runtime_error(why);}
int Label(const char* text){for(unsigned i=0;i<labels.size();++i)if(labels[i]==text)return int(i);const auto i=unsigned(labels.size());Check(i<24,"names bounded");labels.emplace_back(text);names[i]=reinterpret_cast<uintptr_t>(nameData[i]);strcpy_s(reinterpret_cast<char*>(nameData[i]+12),116,text);return int(i);}
uint64_t Skill(const char* text){return uint64_t(Label(text));}
void Named(unsigned i,const char* text){Put(objects[i],0x18,Label(text));}
bool NativeUnlock(void*,unsigned char,uint64_t){++queries;return true;}
void NativeRequest(void* p,unsigned char type){Check(p==objects[Controller],"native request owns controller");++requests;lastRequest=type;}
void NativeSynergy(void* p,unsigned char type){Check(p==objects[Actor]&&type==3,"native own-hero SYNERGY route");++casts;if(startSucceeds){Put<unsigned char>(objects[Ability],0xD8,2);Put<int>(objects[Component],0x100,1);}}
void Reset(){
    g_base=reinterpret_cast<uintptr_t>(image.data());g_pending={};g_pendingActive=false;g_unlock=NativeUnlock;g_request=NativeRequest;g_synergy=NativeSynergy;
    gameThread=true;paused=cinema=radial=false;matched=resolves=startSucceeds=true;queries=requests=casts=0;lastRequest=255;
    auto& settings=dg::runtime::Get();settings.strifeCounterEvadeEnabled=true;settings.synergySwapMode=0;
    context={0,0,1,123,P(Controller),P(Actor),P(Actor),P(World),dg::local_players::PoseKind::Foot,789};
    Put(objects[World],0x160,P(Instance));Put<unsigned char>(objects[Instance],0x188,0);
    Put(objects[Actor],0x348,P(Controller));Put(objects[Controller],0x350,P(Actor));Put(objects[Actor],0xEA0,P(Resource));Put<float>(objects[Resource],0x37C,100);
    Put<unsigned char>(objects[Actor],0x118,3);Put<unsigned char>(objects[Actor],0xC18,1);Put(objects[Actor],0x10,P(StrifeClass));
    Put(objects[Actor],0xC70,P(Component));Put(objects[Component],0xE8,P(Actor));Put(objects[Component],0xF8,reinterpret_cast<uintptr_t>(active));Put<int>(objects[Component],0x100,0);Put<int>(objects[Component],0x104,2);
    Put(objects[Ability],0x10,P(AbilityClass));Put(objects[Ability],0x48,P(Actor));Put<unsigned char>(objects[Ability],0xD8,0);active[0]=P(Ability);
    Put(objects[Finish],0x10,P(FunctionClass));
}
void Tick(){OnLivePlayer(context,7,true);}
void End(){ObserveFinish(objects[Ability],objects[Finish]);Put<unsigned char>(objects[Ability],0xD8,3);Put<int>(objects[Component],0x100,0);Tick();}
void Run(){
    Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put<int>(image.data(),0x3959694,14);
    for(int i=0;i<14;++i){Put<int>(objects[i],0xC,i);Put(items,i*24,P(unsigned(i)));Put<int>(items,i*24+16,i+1);}
    Named(StrifeClass,"Strife_Blueprint_C");Named(WarClass,"War_Blueprint_C");Named(AbilityClass,"MayhemAbility_Strife_Synergy_Blueprint_C");Named(WarAbilityClass,"MayhemAbility_War_Synergy_Blueprint_C");Named(Finish,"OnFinish");Named(FunctionClass,"Function");
    Reset();const auto one=Skill("Strife_Ability_Counter1"),two=Skill("Strife_Ability_Counter2"),other=Skill("Strife_Ability_Evasion2");
    Check(HookUnlock(objects[Actor],0,one)&&queries==1,"Vanilla passes purchased level through unchanged");
    dg::runtime::Get().strifeCounterEvadeEnabled=false;Check(!HookUnlock(objects[Actor],0,one)&&!HookUnlock(objects[Actor],0,two)&&queries==1,"OFF gates both native counter levels only");
    Check(HookUnlock(objects[Actor],0,other)&&HookUnlock(objects[Actor],1,one)&&queries==3,"other skills and creature cores unchanged");
    resolves=false;Check(HookUnlock(objects[Actor],0,one),"foreign actors stay native");resolves=true;
    Put(objects[Actor],0x10,P(WarClass));Check(HookUnlock(objects[Actor],0,one),"War progression unchanged");Put(objects[Actor],0x10,P(StrifeClass));context.index=1;context.localCount=2;Check(!HookUnlock(objects[Actor],0,two),"P2 counter option uses its own local owner");
    Reset();HookRequest(objects[Controller],3);Check(requests==1&&!casts,"Vanilla request unchanged");
    for(int mode:{1,2}){Reset();dg::runtime::Get().synergySwapMode=mode;HookRequest(objects[Controller],2);Check(requests==1&&!casts,"ordinary tap uses native swap");requests=0;
        HookRequest(objects[Controller],3);Check(casts==1&&!requests&&g_pending[0].ability,"hold starts the current hero once");HookRequest(objects[Controller],3);Check(casts==1,"repeat while active cannot duplicate costs");
        Tick();Check(!requests,"no swap during synergy");End();Check(requests==unsigned(mode==1)&&(!requests||lastRequest==2)&&!g_pending[0].actor,"only Inverted swaps after native completion, without second synergy");Tick();Check(requests==unsigned(mode==1),"completion is consumed once");}
    Reset();dg::runtime::Get().synergySwapMode=1;Put(objects[Actor],0x10,P(WarClass));Put(objects[Ability],0x10,P(WarAbilityClass));HookRequest(objects[Controller],3);End();Check(casts==1&&requests==1,"War's own native synergy works on the same route");
    for(int gate=0;gate<7;++gate){Reset();dg::runtime::Get().synergySwapMode=1;HookRequest(objects[Controller],3);
        if(gate==0)paused=true;if(gate==1)cinema=true;if(gate==2)radial=true;if(gate==3)matched=false;if(gate==4)context.world=999;if(gate==5)dg::runtime::Get().synergySwapMode=0;if(gate==6)Put<float>(objects[Resource],0x37C,0);
        End();Check(!requests&&!g_pending[0].actor,"transition/death/OFF cancels deferred swap");}
    Reset();dg::runtime::Get().synergySwapMode=1;context.localCount=2;HookRequest(objects[Controller],3);Check(requests==1&&!casts,"co-op's existing no-swap path preserved");
    Reset();dg::runtime::Get().synergySwapMode=1;Put<unsigned char>(objects[Instance],0x188,1);HookRequest(objects[Controller],3);Check(requests==1&&!casts,"network co-op with one viewport also stays native");
    Reset();dg::runtime::Get().synergySwapMode=1;startSucceeds=false;HookRequest(objects[Controller],3);g_pending[0].started=GetTickCount64()-1001;Tick();Check(!requests&&!g_pending[0].actor,"failed native start cannot swap later");
    Reset();dg::runtime::Get().synergySwapMode=1;HookRequest(objects[Controller],3);Put<int>(items,Ability*24+16,999);End();Check(!requests,"recycled ability cannot complete a queued swap");Put<int>(items,Ability*24+16,Ability+1);
    Shutdown();Check(!g_pending[0].actor&&!g_pending[1].actor,"shutdown discards requests without touching actors");
}
}
namespace dg::native_tps{bool OnGameThread(){return gameThread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return cinema;}bool RadialActive(unsigned){return radial;}}
namespace dg::local_players{bool ResolveActor(void* p,Context& c){if(!resolves||p!=objects[Actor])return false;c=context;return true;}bool Matches(unsigned,uintptr_t,uint32_t g){return matched&&g==7;}Slot PublishedSlot(unsigned){Slot s;s.generation=7;return s;}}
int main(){try{Run();std::cout<<"Native passive gate and owned synergy lifecycle checks passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
