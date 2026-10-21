#include "../src/HeavyLandingFeature.cpp"
#include "ConfigStore.h"
#include <vector>
#include <iostream>
#include <stdexcept>
namespace {
unsigned char actors[2][0x1100]{},controllers[2][0x400]{},components[2][0x180]{},movements[2][0x250]{},resources[2][0x400]{};
unsigned char abilities[2][0x240]{},classes[5][0x40]{},names[5][96]{},items[5*24]{};
uintptr_t nameEntries[5]{},nameChunks[1]{};void* chunks[1]{items};uintptr_t lists[2][1]{};
dg::local_players::Context owners[2]{};bool gameThread=true,paused=false,cinema[2]{},radial[2]{};uint64_t now=1000;
template<class T> void Put(void* p,size_t offset,T value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
template<class T> T Get(void* p,size_t offset){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+offset,sizeof(value));return value;}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
constexpr uint64_t OtherBits=(uint64_t(1)<<7)|(uint64_t(1)<<46);
void Tick(unsigned i=0,bool focused=true){dg::heavy_landing::Service(owners[i],7+i,focused,now);}
bool Released(unsigned i=0){return abilities[i][0x161]==1&&(Get<uint64_t>(abilities[i],0x80)&dg::heavy_landing::MoveBit)!=0;}
void Fresh(){
    using namespace dg::heavy_landing;
    gameThread=true;paused=false;now=1000;g_saved={};g_ground[0]={};g_ground[1]={};
    g_ready=true;dg::runtime::Get().heavyLandingRecoveryEnabled=true;
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
        Put(abilities[i],0x1D8,uintptr_t(0x12345678));
        Put(items,24*(i+3),reinterpret_cast<uintptr_t>(abilities[i]));Put(items,24*(i+3)+16,int(33+i));
    }
}
void Run(){
    using namespace dg::heavy_landing;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x3959680,chunks+0);Put(image.data(),0x3959694,5);
    nameChunks[0]=reinterpret_cast<uintptr_t>(nameEntries);Put(image.data(),0x3955198,nameChunks+0);
    const char* labels[]={"None","MayhemAbility_Strife_HeavyLand_C","MayhemAbility_War_HeavyLand_C","MayhemAbility_Strife_Ground_Dash_Level1_Blueprint_C","MayhemAbility_War_KnockdownLand_C"};
    for(int i=0;i<5;++i){std::memcpy(names[i]+12,labels[i],std::strlen(labels[i])+1);nameEntries[i]=reinterpret_cast<uintptr_t>(names[i]);Put(classes[i],0x18,i);}
    Fresh();const auto beforeActor=std::vector<unsigned char>(actors[0],actors[0]+sizeof(actors[0]));
    dg::runtime::Get().heavyLandingRecoveryEnabled=false;Tick();now+=100;Tick();Check(!Released(),"OFF preserves native heavy landing");
    dg::runtime::Get().heavyLandingRecoveryEnabled=true;Tick();now+=49;Tick();Check(!Released(),"contact must remain native Walking for at least 50 ms");
    now+=1;Tick();Check(Released(),"owned active HeavyLand releases both native movement gates after impact");
    Check(Get<uint64_t>(abilities[0],0x80)==(OtherBits|MoveBit),"other action flags are never changed");
    Check(abilities[0][0xD8]==2&&Get<float>(abilities[0],0xDC)==.3f&&Get<uintptr_t>(abilities[0],0x1D8)==0x12345678,
        "animation helper, ability lifecycle and elapsed time remain native");
    Check(std::memcmp(beforeActor.data(),actors[0],sizeof(actors[0]))==0,"no actor, input byte, movement vector or aim state is written");
    Tick();Check(Released(),"repeated callbacks retain the single baseline");
    dg::runtime::Get().heavyLandingRecoveryEnabled=false;Tick();Check(!Released()&&Get<uint64_t>(abilities[0],0x80)==OtherBits,"OFF restores precisely the previous bool and MOVE bit");
    Fresh();Put<unsigned char>(movements[0],0x1B0,3);Tick();now+=200;Tick();Check(!Released(),"never unlock while falling");
    Put<unsigned char>(movements[0],0x1B0,1);Tick();now+=60;Put(abilities[0],0xDC,.05f);Tick();Check(!Released(),"ground contact does not skip the first impact interval");
    Put(abilities[0],0xDC,.3f);Tick();Check(Released(),"movement becomes available after the impact interval");
    // Native MOVE stays false in all captured locked frames; recovery must not wait for it.
    Check(actors[0][0xE10+29]==0&&Released(),"a blocked MOVE action is not used as the prerequisite for releasing its own gates");
    Put(components[0],0x100,0);Tick();Check(!Released(),"leaving the active list restores the owned instance");
    for(int cls:{3,4}){Fresh();Put(abilities[0],0x10,reinterpret_cast<uintptr_t>(classes[cls]));Tick();now+=200;Tick();Check(!Released(),"dash/knockdown/special landing classes are excluded");}
    Fresh();Put(abilities[0],0x48,owners[1].pawn);Tick();now+=200;Tick();Check(!Released(),"foreign instigator cannot be modified");
    Fresh();Put(abilities[0],0x1C8,reinterpret_cast<uintptr_t>(movements[1]));Tick();now+=200;Tick();Check(!Released(),"foreign movement component is rejected");
    Fresh();Put(controllers[0],0x350,owners[1].pawn);Tick();now+=200;Tick();Check(!Released(),"possession must remain reciprocal");
    Fresh();gameThread=false;Tick();now+=200;Tick();Check(!Released(),"input/render threads never edit abilities");
    for(unsigned gate=0;gate<4;++gate){
        Fresh();Tick();now+=60;Tick();Check(Released(),"fixture acquired landing recovery");
        if(gate==0)paused=true;if(gate==1)cinema[0]=true;if(gate==2)radial[0]=true;
        Tick(0,gate!=3);Check(!Released(),"pause/cinematic/wheel/focus ownership restores native flags");
    }
    Fresh();Tick();Tick(1);now+=60;Tick();Tick(1);Check(Released()&&Released(1),"both characters have independent guarded landing instances");
    owners[0].kind=dg::local_players::PoseKind::Ultimate;Tick();Check(!Released()&&Released(1),"transformations restore only the relevant player's baseline");
    Fresh();abilities[0][0x161]=1;Put<uint64_t>(abilities[0],0x80,OtherBits|MoveBit);Tick();now+=60;Tick();
    dg::runtime::Get().heavyLandingRecoveryEnabled=false;Tick();Check(Released(),"pre-existing native movement permission is retained on OFF");
    Fresh();Tick();now+=60;Tick();Put(items,3*24+16,99);abilities[0][0x161]=1;
    dg::runtime::Get().heavyLandingRecoveryEnabled=false;Tick();Check(abilities[0][0x161]==1,"reused weak identity is discarded without touching the new object");
    Fresh();Tick();now+=60;Tick();Put<uint64_t>(abilities[0],0x80,OtherBits|MoveBit|(uint64_t(1)<<12));
    dg::runtime::Get().heavyLandingRecoveryEnabled=false;Tick();Check(Get<uint64_t>(abilities[0],0x80)==(OtherBits|(uint64_t(1)<<12)),"native updates to unrelated permissions survive restoration");
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
int main(){try{Run();std::cout<<"Heavy landing native gates, grounded scope, lifecycle restoration and profile preservation passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
