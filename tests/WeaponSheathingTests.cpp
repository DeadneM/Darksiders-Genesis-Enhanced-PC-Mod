#include "../src/WeaponSheathingFeature.cpp"
#include "ConfigStore.h"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
unsigned char actor[0x1100]{},component[0x110]{},resource[0x400]{},items[48]{};void* chunks[1]{items};
dg::local_players::Context context{};bool gameThread=true,paused=false;unsigned clearCalls=0;
template<class T> void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
template<class T> T Get(void* p,size_t at){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+at,sizeof(value));return value;}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void ClearRanged(void* p){++clearCalls;Put<unsigned char>(p,0xD16,0);}
void ClearCombat(void* p){++clearCalls;Put<unsigned char>(p,0xD15,0);}
void Tick(){dg::sheathing::OnLivePlayer(context,7);}
void Run(){
    using namespace dg::sheathing;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x3959680,chunks+0);Put(image.data(),0x3959694,2);
    Put(items,24,reinterpret_cast<uintptr_t>(actor));Put(items,24+16,33);Put(actor,0xC,1);
    Put(actor,0,g_base+0x2707940);Put(image.data(),0x2707940+0xBB8,g_base+0x663AC0);
    Put(image.data(),0x2707940+0xBD0,g_base+0x663DC0);
    Put<unsigned char>(actor,0x118,3);Put<unsigned char>(actor,0xC18,1);
    Put(actor,0xEA0,static_cast<void*>(resource));Put(resource,0x37C,100.f);
    Put(actor,0xC70,static_cast<void*>(component));Put(component,0xE8,static_cast<void*>(actor));
    Put(actor,0xCD0,0.f);Put(actor,0xCD4,4.f);Put(actor,0xDB8,1.25f);Put(actor,0xDBC,2.5f);
    context.index=0;context.localPlayer=0x1000;context.character=reinterpret_cast<uintptr_t>(actor);
    g_ready=true;g_combat=ClearCombat;g_ranged=ClearRanged;
    auto& rt=dg::runtime::Get();rt.weaponSheathingEnabled=true;rt.sheathNeverEnabled=false;rt.sheathDelaySeconds=.5f;
    Tick();Check(Get<float>(actor,0xCD0)==.5f&&Get<float>(actor,0xCD4)==.5f,"both real native timer thresholds receive the delay");
    Check(Get<float>(actor,0xDB8)==1.25f&&Get<float>(actor,0xDBC)==2.5f,"native elapsed timers are never reset or forced");
    rt.sheathNeverEnabled=true;Tick();Check(Get<float>(actor,0xCD4)==std::numeric_limits<float>::max(),"Never prevents native threshold expiry without changing action flags");
    rt.weaponSheathingEnabled=false;Tick();Check(Get<float>(actor,0xCD0)==0&&Get<float>(actor,0xCD4)==4,"OFF restores each distinct native duration");
    Put<unsigned char>(actor,0xD15,1);Put<unsigned char>(actor,0xD16,1);
    Check(RequestSheathe(),"manual action works independently of automatic control");
    Put(component,0x100,1);Tick();Check(clearCalls==0,"active abilities are not cancelled");
    Put(component,0x100,0);Put<unsigned char>(actor,0xE10+31,1);Tick();Check(clearCalls==0,"held native AIM is respected even without an ability");
    Put<unsigned char>(actor,0xE10+31,0);Tick();Check(clearCalls==2&&!Get<unsigned char>(actor,0xD15)&&!Get<unsigned char>(actor,0xD16),"idle request invokes both native clear functions once");
    Tick();Check(clearCalls==2,"manual request cannot repeat on later callbacks");
    rt.weaponSheathingEnabled=true;rt.sheathNeverEnabled=false;rt.sheathDelaySeconds=2;Tick();
    context.kind=dg::local_players::PoseKind::Horse;Tick();Check(Get<float>(actor,0xCD4)==4,"leaving foot form restores the normal actor instead of touching the horse");
    context.kind=dg::local_players::PoseKind::Foot;Tick();Put(actor,0xCD0,8.f);Put(actor,0xCD4,9.f);Tick();
    rt.weaponSheathingEnabled=false;Tick();Check(Get<float>(actor,0xCD0)==8&&Get<float>(actor,0xCD4)==9,"native data changes are retained as the new baseline");
    wchar_t folder[MAX_PATH]{},path[MAX_PATH]{};Check(GetTempPathW(MAX_PATH,folder)&&GetTempFileNameW(folder,L"DGS",0,path),"temporary INI");
    dg::config::Store a;a.SetPath(path);a.weaponSheathingEnabled=true;a.sheathNeverEnabled=false;a.sheathDelaySeconds=.5f;a.hotkeys[6]=dg::config::Action::SheatheWeapons;
    Check(a.SaveNow(),"sheathing settings save");dg::config::Store b;b.SetPath(path);Check(b.Load(),"sheathing settings load");
    Check(b.weaponSheathingEnabled&&!b.sheathNeverEnabled&&b.sheathDelaySeconds==.5f&&b.hotkeys==a.hotkeys&&rt.sheathDelaySeconds.load()==.5f,"options, sub-unit seconds, appended hotkey and runtime round-trip");
    static_assert(int(dg::config::Action::SheatheWeapons)==48&&int(dg::config::Action::RefillAmmo)==47);
    b.ResetDefaults(false);Check(!b.weaponSheathingEnabled&&b.sheathNeverEnabled&&b.sheathDelaySeconds==10,"new automatic behavior remains opt-in");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return false;}}
namespace dg::local_players {bool Matches(unsigned i,uintptr_t a,uint32_t g){return i==0&&a==reinterpret_cast<uintptr_t>(actor)&&g==7;}}
int main(){try{Run();std::cout<<"Native sheathing thresholds, restoration, manual lifecycle and configuration passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
