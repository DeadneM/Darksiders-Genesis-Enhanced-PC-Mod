#include "ConfigStore.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <cstdio>
#include <initializer_list>
int main(){
    using namespace dg::config;
    static_assert(int(Action::UnlimitedAmmo)==36&&int(Action::RefillPotions)==37&&int(Action::PotionRecharge)==38&&int(Action::UnlimitedPotions)==39);
    unsigned failed=0;auto check=[&](bool x,const char* s){if(!x){++failed;std::printf("FAIL %s\n",s);}};
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    if(!GetTempPathW(MAX_PATH,directory)||!GetTempFileNameW(directory,L"DGH",0,path))return 1;
    Store a;a.SetPath(path);
    check(!a.potionRechargeEnabled&&!a.unlimitedPotionsEnabled&&a.potionRechargeMultiplier==2,"potions opt in by default");
    check(a.hotkeys==kDefaultHotkeys,"existing F1-F12 assignments are unchanged");
    a.potionRechargeEnabled=true;a.unlimitedPotionsEnabled=true;a.potionRechargeMultiplier=10;
    a.hotkeys[9]=Action::RefillPotions;a.hotkeys[10]=Action::PotionRecharge;a.hotkeys[11]=Action::UnlimitedPotions;
    check(a.SaveNow(),"potion configuration writes");
    Store b;b.SetPath(path);check(b.Load(),"potion configuration reloads");
    check(b.potionRechargeEnabled&&b.unlimitedPotionsEnabled&&b.potionRechargeMultiplier==10,"all potion options persist");
    check(a.hotkeys==b.hotkeys,"three appended actions round-trip as INI tokens");
    const auto& rt=dg::runtime::Get();
    check(rt.potionRechargeEnabled.load()&&rt.unlimitedPotionsEnabled.load()&&rt.potionRechargeMultiplier.load()==10,"all potion settings reach runtime atomics");
    for(auto v:{L"1",L"2.5",L"10",L"100"}){WritePrivateProfileStringW(L"Values",L"PotionRechargeMultiplier",v,path);check(b.Load(),"legal potion rate loads");}
    WritePrivateProfileStringW(L"Values",L"PotionRechargeMultiplier",L"0",path);b.Load();check(b.potionRechargeMultiplier==1,"zero clamps to native rate");
    WritePrivateProfileStringW(L"Values",L"PotionRechargeMultiplier",L"999",path);b.Load();check(b.potionRechargeMultiplier==100,"upper rate bound");
    WritePrivateProfileStringW(L"Values",L"PotionRechargeMultiplier",L"nan",path);b.Load();check(b.potionRechargeMultiplier==2,"nonfinite rate falls back");
    WritePrivateProfileStringW(L"Features",L"PotionRecharge",nullptr,path);WritePrivateProfileStringW(L"Features",L"UnlimitedPotions",nullptr,path);
    WritePrivateProfileStringW(L"Values",L"PotionRechargeMultiplier",nullptr,path);b.Load();
    check(!b.potionRechargeEnabled&&!b.unlimitedPotionsEnabled&&b.potionRechargeMultiplier==2,"legacy INIs opt out of new native changes");
    check(b.hotkeys==a.hotkeys,"loading missing potion settings preserves explicit keys");
    b.ResetDefaults(false);check(!b.potionRechargeEnabled&&!b.unlimitedPotionsEnabled&&b.potionRechargeMultiplier==2,"reset restores potion defaults");
    check(b.hotkeys==kDefaultHotkeys&&b.menuKey==VK_INSERT,"reset retains established default bindings");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
    std::printf("Potion config: %u failures\n",failed);return failed?1:0;
}
