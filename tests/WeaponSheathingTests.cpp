#include "../src/WeaponSheathingFeature.cpp"
#include "ConfigStore.h"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
unsigned char actor[0x1100]{},component[0x110]{},resource[0x400]{},items[72]{};void* chunks[1]{items};
unsigned char war[0x1100]{},warComponent[0x110]{},warResource[0x400]{};
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
    Put(image.data(),0x3959680,chunks+0);Put(image.data(),0x3959694,3);
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
    Tick();Check(Get<float>(actor,0xCD0)==0&&Get<float>(actor,0xCD4)==.5f,"Strife delay applies only to pistols; temporary melee blades retain native zero delay");
    Check(Get<float>(actor,0xDB8)==1.25f&&Get<float>(actor,0xDBC)==2.5f,"native elapsed timers are never reset or forced");
    rt.sheathNeverEnabled=true;Tick();Check(Get<float>(actor,0xCD0)==0&&Get<float>(actor,0xCD4)==std::numeric_limits<float>::max(),"Never retains Strife pistols without retaining his melee blades");
    rt.weaponSheathingEnabled=false;Tick();Check(Get<float>(actor,0xCD0)==0&&Get<float>(actor,0xCD4)==4,"OFF restores each distinct native duration");
    Put<unsigned char>(actor,0xD15,1);Put<unsigned char>(actor,0xD16,1);
    Check(RequestSheathe(),"manual action works independently of automatic control");
    Put(component,0x100,1);Tick();Check(clearCalls==0,"active abilities are not cancelled");
    Put(component,0x100,0);Put<unsigned char>(actor,0xE10+31,1);Tick();Check(clearCalls==0,"held native AIM is respected even without an ability");
    Put<unsigned char>(actor,0xE10+31,0);Tick();Check(clearCalls==2&&!Get<unsigned char>(actor,0xD15)&&!Get<unsigned char>(actor,0xD16),"Strife manual shortcut retains both native Clear functions for all weapons");
    Tick();Check(clearCalls==2,"manual request cannot repeat on later callbacks");
    rt.weaponSheathingEnabled=true;rt.sheathNeverEnabled=false;rt.sheathDelaySeconds=2;Tick();
    context.kind=dg::local_players::PoseKind::Horse;Tick();Check(Get<float>(actor,0xCD4)==4,"leaving foot form restores the normal actor instead of touching the horse");
    context.kind=dg::local_players::PoseKind::Foot;Tick();Put(actor,0xCD0,8.f);Put(actor,0xCD4,9.f);Tick();
    rt.weaponSheathingEnabled=false;Tick();Check(Get<float>(actor,0xCD0)==8&&Get<float>(actor,0xCD4)==9,"native data changes are retained as the new baseline");
    wchar_t folder[MAX_PATH]{},path[MAX_PATH]{};Check(GetTempPathW(MAX_PATH,folder)&&GetTempFileNameW(folder,L"DGS",0,path),"temporary INI");
    dg::config::Store a;a.SetPath(path);a.weaponSheathingEnabled=true;a.sheathNeverEnabled=false;a.sheathDelaySeconds=.5f;a.hotkeys[6]=dg::config::Action::SheatheWeapons;
    a.sheathKeyboardKey=VK_F7;a.sheathPadButtonA=0;a.sheathPadButtonB=0x20000;
    Check(a.SaveNow(),"sheathing settings save");dg::config::Store b;b.SetPath(path);Check(b.Load(),"sheathing settings load");
    Check(b.weaponSheathingEnabled&&!b.sheathNeverEnabled&&b.sheathDelaySeconds==.5f&&b.hotkeys==a.hotkeys&&rt.sheathDelaySeconds.load()==.5f,"options, sub-unit seconds, appended hotkey and runtime round-trip");
    for(const auto* key:{L"StrifeMeleeNeverAutoSheathe",L"WarNeverAutoSheathe"})WritePrivateProfileStringW(L"Features",key,nullptr,path);
    for(const auto* key:{L"StrifeMeleeSheathDelaySeconds",L"WarSheathDelaySeconds"})WritePrivateProfileStringW(L"Values",key,nullptr,path);
    Check(b.Load()&&!b.sheathStrifeMeleeNeverEnabled&&b.sheathStrifeMeleeDelaySeconds==0&&
          !b.sheathWarNeverEnabled&&b.sheathWarDelaySeconds==.5f,"legacy personal delay migrates to War and pistols while Strife blades return to native zero");
    b.sheathStrifeMeleeNeverEnabled=false;b.sheathStrifeMeleeDelaySeconds=1.25f;
    b.sheathWarNeverEnabled=true;b.sheathWarDelaySeconds=12.5f;
    Check(b.SaveNow()&&b.Load()&&b.sheathStrifeMeleeDelaySeconds==1.25f&&b.sheathWarDelaySeconds==12.5f&&
          b.sheathWarNeverEnabled&&!b.sheathNeverEnabled&&rt.sheathStrifeMeleeDelaySeconds.load()==1.25f,
          "three delays and separate Never choices survive save/reload without changing pistol values");
    Check(b.sheathKeyboardKey==VK_F7&&b.sheathPadButtonA==0&&b.sheathPadButtonB==0x20000,"dedicated keyboard duplicates F7 Hotkey and either controller slot can work alone");
    WritePrivateProfileStringW(L"Controls",L"SheatheKeyboardKey",nullptr,path);
    WritePrivateProfileStringW(L"Controls",L"SheathePadButtonA",L"3",path);
    WritePrivateProfileStringW(L"Controls",L"SheathePadButtonB",L"-1",path);
    Check(b.Load()&&b.sheathKeyboardKey==0&&b.sheathPadButtonA==0&&b.sheathPadButtonB==0,"missing keyboard and invalid pad masks remain OFF");
    Check(b.hotkeys==a.hotkeys&&b.weaponSheathingEnabled&&b.sheathDelaySeconds==.5f,"legacy hotkeys and validated sheathing options survive binding load");
    b.sheathKeyboardKey='K';b.sheathPadButtonA=0x0100;b.sheathPadButtonB=0x0200;
    Check(b.SaveNow()&&b.Load()&&b.sheathKeyboardKey=='K'&&b.sheathPadButtonA==0x0100&&b.sheathPadButtonB==0x0200,"two-button combination round-trip");
    static_assert(int(dg::config::Action::SheatheWeapons)==48&&int(dg::config::Action::RefillAmmo)==47);
    b.ResetDefaults(false);Check(!b.weaponSheathingEnabled&&b.sheathNeverEnabled&&b.sheathDelaySeconds==10,"new automatic behavior remains opt-in");
    Check(!b.sheathStrifeMeleeNeverEnabled&&b.sheathStrifeMeleeDelaySeconds==0&&b.sheathWarNeverEnabled&&b.sheathWarDelaySeconds==10,"reset has native Strife blade delay and separate War defaults");
    Check(!b.sheathKeyboardKey&&!b.sheathPadButtonA&&!b.sheathPadButtonB,"new shortcuts default to OFF");
    Tick(); // Refresh eligibility after profile I/O; never depend on disk timing.
    rt.weaponSheathingEnabled=true;rt.sheathStrifeMeleeNeverEnabled=false;rt.sheathStrifeMeleeDelaySeconds=1.25f;
    rt.sheathNeverEnabled=false;rt.sheathDelaySeconds=2;Tick();
    Check(Get<float>(actor,0xCD0)==1.25f&&Get<float>(actor,0xCD4)==2,"Strife's two real thresholds can use distinct configured delays");
    rt.sheathNeverEnabled=true;Tick();Check(Get<float>(actor,0xCD0)==1.25f&&Get<float>(actor,0xCD4)==std::numeric_limits<float>::max(),"pistol Never does not retain Strife blades");
    rt.sheathStrifeMeleeNeverEnabled=true;Tick();Check(Get<float>(actor,0xCD0)==std::numeric_limits<float>::max(),"Strife blades can explicitly opt into Never independently");
    rt.weaponSheathingEnabled=false;Tick();Check(Get<float>(actor,0xCD0)==8&&Get<float>(actor,0xCD4)==9,"OFF restores both distinct, updated native baselines");
    // Distinct P2 War identity: no character-type mutation of a cached actor.
    std::memcpy(war,actor,sizeof(war));Put(war,0xC,2);Put<unsigned char>(war,0xC18,2);
    Put(items,48,reinterpret_cast<uintptr_t>(war));Put(items,64,34);
    Put(war,0xEA0,static_cast<void*>(warResource));Put(warResource,0x37C,100.f);
    Put(war,0xC70,static_cast<void*>(warComponent));Put(warComponent,0xE8,static_cast<void*>(war));
    Put(war,0xCD0,3.f);Put(war,0xCD4,6.f);Put<unsigned char>(war,0xD15,1);Put<unsigned char>(war,0xD16,1);
    auto warContext=context;warContext.index=1;warContext.character=reinterpret_cast<uintptr_t>(war);
    rt.weaponSheathingEnabled=true;rt.sheathNeverEnabled=false;rt.sheathDelaySeconds=2;
    rt.sheathWarNeverEnabled=false;rt.sheathWarDelaySeconds=1.5f;
    OnLivePlayer(warContext,8);
    Check(Get<float>(war,0xCD0)==1.5f&&Get<float>(war,0xCD4)==6,"War uses its independent sword delay, not the pistol delay or ranged equipment");
    rt.sheathWarNeverEnabled=true;OnLivePlayer(warContext,8);
    Check(Get<float>(war,0xCD0)==std::numeric_limits<float>::max()&&Get<float>(war,0xCD4)==6,"War Never affects only his sword");
    rt.weaponSheathingEnabled=false;OnLivePlayer(warContext,8);
    Check(Get<float>(war,0xCD0)==3&&Get<float>(war,0xCD4)==6,"War OFF restores his own native sword threshold");
    Check(!RequestSheathe(2)&&!RequestSheathe(-2),"invalid player selectors cannot queue anything");
    Check(RequestSheathe(1)&&!g_requests[0].requestedActor&&g_requests[1].requestedActor,"P2 shortcut does not request P1");
    auto beforeWar=clearCalls;Tick();Check(clearCalls==beforeWar,"P2 request cannot act on P1");
    OnLivePlayer(warContext,8);Check(clearCalls==beforeWar+2&&!Get<unsigned char>(war,0xD15)&&!Get<unsigned char>(war,0xD16),"War shortcut still clears all native weapon states while preserving P1 isolation");
    Check(RequestSheathe(0)&&!RequestSheathe(0),"duplicate sources cannot add a second manual request");
    const auto callsBefore=clearCalls;Tick();Tick();
    Check(clearCalls==callsBefore+2,"merged Strife requests invoke both native Clear functions once");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return false;}}
namespace dg::local_players {bool Matches(unsigned i,uintptr_t a,uint32_t g){return (i==0&&a==reinterpret_cast<uintptr_t>(actor)&&g==7)||(i==1&&a==reinterpret_cast<uintptr_t>(war)&&g==8);}}
int main(){try{Run();std::cout<<"Native sheathing thresholds, restoration, manual lifecycle and configuration passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
