#include "../src/HeavyLandingFeature.cpp"
#include "ConfigStore.h"
#include <vector>
#include <iostream>
#include <stdexcept>
namespace {
unsigned char actors[2][0x1100]{},controllers[2][0x400]{},components[2][0x180]{},movements[2][0x250]{},resources[2][0x400]{};
unsigned char abilities[2][0x240]{},helpers[2][0x40]{},montages[2][0xA0]{},packages[2][0x40]{},classes[7][0x40]{},names[11][192]{},items[5*24]{};
uintptr_t nameEntries[11]{},nameChunks[1]{};void* chunks[1]{items};uintptr_t lists[2][1]{};
dg::local_players::Context owners[2]{};bool gameThread=true,paused=false,cinema[2]{},radial[2]{};uint64_t now=1000;
template<class T> void Put(void* p,size_t offset,T value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
template<class T> T Get(void* p,size_t offset){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+offset,sizeof(value));return value;}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
constexpr uint64_t OtherBits=(uint64_t(1)<<7)|(uint64_t(1)<<46);
unsigned opens=0,restores=0;bool nativeArgs=true;
void Window(void* p,unsigned char cause,bool yes){
    nativeArgs=nativeArgs&&cause==1;auto bits=Get<uint64_t>(p,0x120);
    Put(p,0x120,yes?bits|dg::heavy_landing::MoveBit:bits&~dg::heavy_landing::MoveBit);
    if(yes)++opens;else ++restores;
}
void Interrupt(void* p,unsigned char cause,bool yes){
    nativeArgs=nativeArgs&&cause==1;auto bits=Get<uint64_t>(p,0xB0);
    Put(p,0xB0,yes?bits|dg::heavy_landing::MoveBit:bits&~dg::heavy_landing::MoveBit);
}
void Tick(unsigned i=0,bool focused=true){dg::heavy_landing::Service(owners[i],7+i,focused,now);}
bool Released(unsigned i=0){return (Get<uint64_t>(abilities[i],0x120)&dg::heavy_landing::MoveBit)!=0;}
void Fresh(){
    using namespace dg::heavy_landing;
    gameThread=true;paused=false;now=1000;g_saved={};g_ground[0]={};g_ground[1]={};
    g_ready=true;g_window=Window;g_interrupt=Interrupt;opens=restores=0;nativeArgs=true;dg::runtime::Get().heavyLandingRecoveryEnabled=true;
    for(unsigned i=0;i<2;++i){
        std::memset(actors[i],0,sizeof(actors[i]));std::memset(abilities[i],0,sizeof(abilities[i]));
        cinema[i]=false;radial[i]=false;
        auto& o=owners[i];o={};o.index=int(i);o.controllerId=int(i);o.localCount=2;o.localPlayer=0x1000+i*0x100;
        o.character=o.pawn=reinterpret_cast<uintptr_t>(actors[i]);o.controller=reinterpret_cast<uintptr_t>(controllers[i]);o.world=0x3000;
        Put<unsigned char>(actors[i],0x118,3);Put<unsigned char>(actors[i],0xC18,static_cast<unsigned char>(i+1));
        Put(actors[i],0x348,o.controller);Put(controllers[i],0x350,o.pawn);
        Put(actors[i],0xC70,reinterpret_cast<uintptr_t>(components[i]));Put(components[i],0xE8,o.pawn);
        lists[i][0]=reinterpret_cast<uintptr_t>(abilities[i]);Put(components[i],0xF8,reinterpret_cast<uintptr_t>(lists[i]));
        Put(components[i],0x100,1);Put(components[i],0x104,1);
        Put(actors[i],0x388,reinterpret_cast<uintptr_t>(movements[i]));Put<unsigned char>(movements[i],0x1B0,1);Put<unsigned char>(movements[i],0x1B1,0);
        Put(actors[i],0xEA0,reinterpret_cast<uintptr_t>(resources[i]));Put(resources[i],0x37C,100.f);
        Put(abilities[i],0xC,int(i+3));Put(abilities[i],0x10,reinterpret_cast<uintptr_t>(classes[i+1]));
        Put(abilities[i],0x48,o.pawn);Put(abilities[i],0x1B0,o.pawn);Put(abilities[i],0x1C8,reinterpret_cast<uintptr_t>(movements[i]));
        Put<unsigned char>(abilities[i],0xD8,2);Put(abilities[i],0xDC,.3f);Put<uint64_t>(abilities[i],0x80,OtherBits);
        Put<uint64_t>(abilities[i],0xB0,OtherBits);Put<uint64_t>(abilities[i],0x120,OtherBits);
        Put(abilities[i],0x1D8,reinterpret_cast<uintptr_t>(helpers[i]));Put(helpers[i],0x10,reinterpret_cast<uintptr_t>(classes[5]));
        Put(helpers[i],0x28,reinterpret_cast<uintptr_t>(montages[i]));Put(montages[i],0x10,reinterpret_cast<uintptr_t>(classes[6]));
        Put(montages[i],0x18,int(7+i));Put(montages[i],0x20,reinterpret_cast<uintptr_t>(packages[i]));Put(montages[i],0x94,1.f);Put(packages[i],0x18,int(9+i));
        Put(items,24*(i+3),reinterpret_cast<uintptr_t>(abilities[i]));Put(items,24*(i+3)+16,int(33+i));
    }
}
void Run(){
    using namespace dg::heavy_landing;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x3959680,chunks+0);Put(image.data(),0x3959694,5);
    nameChunks[0]=reinterpret_cast<uintptr_t>(nameEntries);Put(image.data(),0x3955198,nameChunks+0);
    const char* labels[]={"None","MayhemAbility_Strife_HeavyLand_C","MayhemAbility_War_HeavyLand_C","MayhemAbility_Strife_Ground_Dash_Level1_Blueprint_C","MayhemAbility_War_KnockdownLand_C","MayhemMontagePlayer","AnimMontage",
        "Strife_Jump_Land_Heavy_Montage","War_Jump_Land_Heavy_Montage","/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Jump_Land_Heavy_Montage","/Game/Characters/Heroes/Hero_War/Animation/War_Jump_Land_Heavy_Montage"};
    for(int i=0;i<11;++i){std::memcpy(names[i]+12,labels[i],std::strlen(labels[i])+1);nameEntries[i]=reinterpret_cast<uintptr_t>(names[i]);if(i<7)Put(classes[i],0x18,i);}
    Fresh();const auto beforeActor=std::vector<unsigned char>(actors[0],actors[0]+sizeof(actors[0]));
    dg::runtime::Get().heavyLandingRecoveryEnabled=false;Tick();now+=100;Tick();Check(!Released()&&!opens,"OFF keeps native windows");
    dg::runtime::Get().heavyLandingRecoveryEnabled=true;Tick();now+=49;Tick();Check(!Released(),"requires 50 ms of continuous native ground contact");
    now+=1;Tick();Check(Released()&&opens==1&&nativeArgs,"uses native MOVE interrupt enum, not physical input/action enum");
    Check(Get<uint64_t>(abilities[0],0x80)==OtherBits&&abilities[0][0x161]==0,"old movement bool/action writes removed");
    Check(abilities[0][0xD8]==2&&Get<float>(abilities[0],0xDC)==.3f,"no forced Finish or animation time change");
    Check(std::memcmp(beforeActor.data(),actors[0],sizeof(actors[0]))==0,"no actor/input/movement/aim writes");Tick();Check(opens==1,"no repeated native call for open window");
    Put<uint64_t>(abilities[0],0xB0,OtherBits|MoveBit); // simulated engine reconciliation, not runtime validation
    dg::runtime::Get().heavyLandingRecoveryEnabled=false;Tick();Check(!Released()&&Get<uint64_t>(abilities[0],0xB0)==OtherBits&&restores==1,"OFF restores early request and effective interrupt");
    Fresh();Put<unsigned char>(movements[0],0x1B0,3);Tick();now+=200;Tick();Check(!Released(),"never opens in air");
    Put<unsigned char>(movements[0],0x1B0,1);Tick();now+=60;Put(abilities[0],0xDC,.05f);Tick();Check(!Released(),"initial impact retained");Put(abilities[0],0xDC,.15f);Tick();Check(Released(),"post-impact window does not depend on blocked MOVE action");
    for(int cls:{3,4}){Fresh();Put(abilities[0],0x10,reinterpret_cast<uintptr_t>(classes[cls]));Tick();now+=200;Tick();Check(!Released(),"dash/knockdown/special landings stay native");}
    Fresh();Put(abilities[0],0x48,owners[1].pawn);Tick();now+=200;Tick();Check(!Released(),"foreign instigator rejected");
    Fresh();Put(abilities[0],0x1C8,reinterpret_cast<uintptr_t>(movements[1]));Tick();now+=200;Tick();Check(!Released(),"foreign movement rejected");
    Fresh();Put(controllers[0],0x350,owners[1].pawn);Tick();now+=200;Tick();Check(!Released(),"requires reciprocal possession");
    Fresh();Put(montages[0],0x94,2.f);Tick();now+=200;Tick();Check(!Released(),"unexpected montage rate refused");
    Fresh();Put(montages[0],0x20,reinterpret_cast<uintptr_t>(packages[1]));Tick();now+=200;Tick();Check(!Released(),"wrong montage package refused");
    Fresh();abilities[0][0x148]=1;Tick();now+=200;Tick();Check(!Released(),"pending native completion not changed");
    Fresh();gameThread=false;Tick();now+=200;Tick();Check(!Released(),"no API call off game thread");
    for(unsigned gate=0;gate<4;++gate){
        Fresh();Tick();now+=60;Tick();Check(Released(),"fixture acquired window");
        if(gate==0)paused=true;if(gate==1)cinema[0]=true;if(gate==2)radial[0]=true;Tick(0,gate!=3);Check(!Released(),"pause/cinematic/wheel/focus restores early window");
    }
    Fresh();Tick();Tick(1);now+=60;Tick();Tick(1);Check(Released()&&Released(1),"independent local windows");
    owners[0].kind=dg::local_players::PoseKind::Ultimate;Tick();Check(!Released()&&Released(1),"transform restores only its player");
    Fresh();Put<uint64_t>(abilities[0],0x120,OtherBits|MoveBit);Tick();now+=60;Tick();dg::runtime::Get().heavyLandingRecoveryEnabled=false;Tick();Check(Released()&&!opens&&!restores,"pre-existing window never owned or closed");
    Fresh();Tick();now+=60;Tick();Put(items,3*24+16,99);dg::runtime::Get().heavyLandingRecoveryEnabled=false;Tick();Check(Released()&&!restores,"reused identity discarded without call on new object");
    Fresh();Tick();now+=60;Tick();Put(abilities[0],0xDC,.7f);dg::runtime::Get().heavyLandingRecoveryEnabled=false;Tick();Check(Released()&&!restores,"does not close due native MOVE notify");
    Fresh();Tick();now+=60;Tick();Put<unsigned char>(abilities[0],0xD8,4);Tick();Check(!restores,"completed ability not called");
    Fresh();Tick();now+=60;Tick();Put<uint64_t>(abilities[0],0x120,OtherBits|MoveBit|(uint64_t(1)<<12));dg::runtime::Get().heavyLandingRecoveryEnabled=false;Tick();Check(Get<uint64_t>(abilities[0],0x120)==(OtherBits|(uint64_t(1)<<12)),"other native windows preserved");
    wchar_t folder[MAX_PATH]{},path[MAX_PATH]{};Check(GetTempPathW(MAX_PATH,folder)&&GetTempFileNameW(folder,L"DGL",0,path),"temporary INI");
    dg::config::Store c;c.SetPath(path);Check(c.Load()&&!c.heavyLandingRecoveryEnabled,"legacy INI defaults to native landing");
    c.heavyLandingRecoveryEnabled=true;c.aimPadButtonA=0x40;c.sheathKeyboardKey='K';Check(c.SaveNow(),"landing option saves");
    dg::config::Store loaded;loaded.SetPath(path);Check(loaded.Load()&&loaded.heavyLandingRecoveryEnabled&&dg::runtime::Get().heavyLandingRecoveryEnabled.load(),"option reaches the runtime and survives reload");
    Check(loaded.aimPadButtonA==0x40&&loaded.sheathKeyboardKey=='K',"manual aim and previous shortcuts survive landing profile I/O");
    loaded.ResetDefaults(false);Check(!loaded.heavyLandingRecoveryEnabled,"reset returns the experimental recovery to OFF");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
    Shutdown();Check(!Ready(),"shutdown disables callbacks without off-thread restoration");
}
}
namespace dg::local_players {bool Matches(unsigned i,uintptr_t a,uint32_t g){return i<2&&a==owners[i].character&&g==7+i;}}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned i){return cinema[i];}bool RadialActive(unsigned i){return radial[i];}}
int main(){try{Run();std::cout<<"Heavy landing native window, grounded scope, lifecycle restoration and profile preservation passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
