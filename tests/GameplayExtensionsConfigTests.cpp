#include "ConfigStore.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <iostream>
#include <stdexcept>
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
int main(){try{
    using namespace dg::config;
    static_assert(int(Action::SheatheWeapons)==48&&int(Action::FiniteStandardAmmo)==49&&int(Action::GrappleRange)==50&&int(Action::InfiniteDoubleJump)==51);
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};Check(GetTempPathW(MAX_PATH,directory)&&GetTempFileNameW(directory,L"DGX",0,path),"temporary profile created");
    Store a;a.SetPath(path);a.Load();Check(!a.finiteStandardAmmoEnabled&&a.standardAmmoCapacity==200&&!a.grappleRangeEnabled&&a.grappleRangeMultiplier==2&&a.infiniteDoubleJumpMode==0,"missing keys retain native gameplay");
    a.finiteStandardAmmoEnabled=true;a.standardAmmoCapacity=23;a.grappleRangeEnabled=true;a.grappleRangeMultiplier=3.5f;a.infiniteDoubleJumpMode=1;
    a.hotkeys[0]=Action::FiniteStandardAmmo;a.hotkeys[1]=Action::GrappleRange;a.hotkeys[2]=Action::InfiniteDoubleJump;Check(a.SaveNow(),"extension settings saved");
    Store b;b.SetPath(path);Check(b.Load()&&b.finiteStandardAmmoEnabled&&b.standardAmmoCapacity==23&&b.grappleRangeEnabled&&b.grappleRangeMultiplier==3.5f&&b.infiniteDoubleJumpMode==1,"settings round trip");
    auto& rt=dg::runtime::Get();Check(rt.finiteStandardAmmoEnabled&&rt.standardAmmoCapacity==23&&rt.grappleRangeEnabled&&rt.grappleRangeMultiplier==3.5f&&rt.infiniteDoubleJumpMode==1,"runtime receives all settings");
    Check(b.hotkeys[0]==Action::FiniteStandardAmmo&&b.hotkeys[1]==Action::GrappleRange&&b.hotkeys[2]==Action::InfiniteDoubleJump,"new hotkey tokens round trip");
    WritePrivateProfileStringW(L"Values",L"StandardAmmoCapacity",L"999999",path);WritePrivateProfileStringW(L"Values",L"GrappleRangeMultiplier",L"nan",path);WritePrivateProfileStringW(L"Values",L"InfiniteDoubleJumpMode",L"99",path);b.Load();
    Check(b.standardAmmoCapacity==10000&&b.grappleRangeMultiplier==2&&b.infiniteDoubleJumpMode==2,"bounds and nonfinite fallback");
    b.ResetDefaults(false);Check(!b.finiteStandardAmmoEnabled&&b.standardAmmoCapacity==200&&!b.grappleRangeEnabled&&b.infiniteDoubleJumpMode==0,"reset disables all new experiments");
    DeleteFileW(path);std::cout<<"Gameplay extensions configuration, migration and append-only actions passed\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
