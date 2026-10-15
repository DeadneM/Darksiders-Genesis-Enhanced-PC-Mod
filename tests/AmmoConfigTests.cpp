#include "ConfigStore.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <cstdio>
#include <initializer_list>
int main(){
    using namespace dg::config;
    static_assert(int(Action::TpsCameraReset)==26&&int(Action::UnlimitedAllGauges)==34);
    static_assert(int(Action::ReduceAmmo)==35&&int(Action::UnlimitedAmmo)==36);
    unsigned failed=0;
    auto check=[&](bool value,const char* label){if(!value){++failed;std::printf("FAIL %s\n",label);}};
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    check(GetTempPathW(MAX_PATH,directory)!=0,"temporary directory available");
    check(GetTempFileNameW(directory,L"DGA",0,path)!=0,"temporary file created");
    Store a;a.SetPath(path);
    check(!a.reduceAmmoEnabled&&!a.unlimitedAmmoEnabled&&a.ammoConsumptionDivisor==2,"new ammo controls default OFF");
    check(a.hotkeys==kDefaultHotkeys,"original bindings preserved");
    a.reduceAmmoEnabled=true;a.unlimitedAmmoEnabled=true;a.ammoConsumptionDivisor=10;
    a.hotkeys[9]=Action::ReduceAmmo;a.hotkeys[10]=Action::UnlimitedAmmo;
    check(a.SaveNow(),"ammo settings save");
    Store b;b.SetPath(path);check(b.Load(),"ammo settings reload");
    check(b.reduceAmmoEnabled&&b.unlimitedAmmoEnabled&&b.ammoConsumptionDivisor==10,"ammo settings persist");
    check(a.hotkeys==b.hotkeys,"both ammo actions persist in real INI");
    const auto& rt=dg::runtime::Get();
    check(rt.reduceAmmoEnabled.load()&&rt.unlimitedAmmoEnabled.load()&&rt.ammoConsumptionDivisor.load()==10,"runtime receives ammo settings");
    for(auto value:{L"0",L"1",L"2.5",L"10"}){
        WritePrivateProfileStringW(L"Values",L"AmmoConsumptionDivisor",value,path);
        check(b.Load(),"legal ammo divisor reloads");
    }
    WritePrivateProfileStringW(L"Values",L"AmmoConsumptionDivisor",L"999",path);b.Load();
    check(b.ammoConsumptionDivisor==10,"divisor upper bound");
    WritePrivateProfileStringW(L"Values",L"AmmoConsumptionDivisor",L"-2",path);b.Load();
    check(b.ammoConsumptionDivisor==0,"divisor lower bound");
    WritePrivateProfileStringW(L"Values",L"AmmoConsumptionDivisor",L"nan",path);b.Load();
    check(b.ammoConsumptionDivisor==2,"nonfinite divisor uses default");
    WritePrivateProfileStringW(L"Features",L"ReduceAmmoConsumption",nullptr,path);
    WritePrivateProfileStringW(L"Features",L"UnlimitedAmmo",nullptr,path);
    WritePrivateProfileStringW(L"Values",L"AmmoConsumptionDivisor",nullptr,path);b.Load();
    check(!b.reduceAmmoEnabled&&!b.unlimitedAmmoEnabled&&b.ammoConsumptionDivisor==2,"legacy INI needs no migration");
    b.ResetDefaults(false);check(!b.reduceAmmoEnabled&&!b.unlimitedAmmoEnabled&&b.ammoConsumptionDivisor==2,"reset restores ammo defaults");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
    std::printf("Ammo config: %u failures\n",failed);return failed?1:0;
}
