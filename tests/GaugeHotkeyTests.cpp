#include "ConfigStore.h"
#include "GaugeRequestPolicy.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <cstdio>
#include <cwchar>

int main(){
    using namespace dg::config;
    static_assert(static_cast<int>(Action::TpsCameraReset)==26);
    static_assert(static_cast<int>(Action::RefillAllGauges)==30);
    static_assert(UnlimitedMask(Action::UnlimitedWrath)==1);
    static_assert(UnlimitedMask(Action::UnlimitedUltimate)==2);
    static_assert(UnlimitedMask(Action::UnlimitedSynergy)==4);
    static_assert(UnlimitedMask(Action::UnlimitedAllGauges)==7);
    static_assert(GaugeRefillMask(Action::RefillWrath)==dg::manual_gauges::Wrath);
    static_assert(GaugeRefillMask(Action::RefillUltimate)==dg::manual_gauges::Ultimate);
    static_assert(GaugeRefillMask(Action::RefillSynergy)==dg::manual_gauges::Synergy);
    static_assert(GaugeRefillMask(Action::RefillAllGauges)==dg::manual_gauges::All);
    unsigned failed=0;
    const auto check=[&](bool pass,const char* what){if(!pass){++failed;std::printf("FAIL %s\n",what);}};
    for(int i=0;i<static_cast<int>(Action::Count);++i){
        const auto action=static_cast<Action>(i);
        check(ParseAction(ActionToken(action))==action,"all action tokens round-trip");
        check(ActionLabel(action)[0]!=0,"all actions have labels");
        if(i<=26)check(GaugeRefillMask(action)==0,"legacy action does not refill a gauge");
    }
    check(ParseAction(L"refillultimate")==Action::RefillUltimate,"case-insensitive INI action");
    check(ParseAction(L"unknown")==Action::None,"unknown action stays None");
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    check(GetTempPathW(MAX_PATH,directory)!=0,"temporary directory available");
    check(GetTempFileNameW(directory,L"DGK",0,path)!=0,"temporary config file created");
    Store write;
    write.SetPath(path);
    check(!write.unlimitedWrathEnabled&&!write.unlimitedUltimateEnabled&&!write.unlimitedSynergyEnabled,"Unlimited defaults OFF");
    check(write.skipWarningEnabled,"new Skip Warning experiment defaults ON");
    const auto defaults=write.hotkeys;
    write.hotkeys[5]=Action::RefillWrath;
    write.hotkeys[6]=Action::RefillUltimate;
    write.hotkeys[7]=Action::RefillSynergy;
    write.hotkeys[8]=Action::RefillAllGauges;
    write.hotkeys[0]=Action::UnlimitedWrath;
    write.hotkeys[1]=Action::UnlimitedUltimate;
    write.hotkeys[2]=Action::UnlimitedSynergy;
    write.hotkeys[3]=Action::UnlimitedAllGauges;
    write.unlimitedWrathEnabled=true;write.unlimitedUltimateEnabled=true;
    write.unlimitedSynergyEnabled=true;write.skipWarningEnabled=false;
    check(write.SaveNow(),"config save succeeds");
    Store read;
    read.SetPath(path);
    check(read.Load(),"config load succeeds");
    check(read.hotkeys==write.hotkeys,"refill and Unlimited actions survive actual INI save/reload");
    check(read.unlimitedWrathEnabled&&read.unlimitedUltimateEnabled&&read.unlimitedSynergyEnabled,"three Unlimited options persist");
    check(!read.skipWarningEnabled,"explicit Skip Warning OFF persists");
    const auto& rt=dg::runtime::Get();
    check(rt.unlimitedWrathEnabled.load()&&rt.unlimitedUltimateEnabled.load()&&rt.unlimitedSynergyEnabled.load(),"runtime receives three Unlimited options");
    for(size_t i=0;i<defaults.size();++i)
        if(i==4||i>8)check(read.hotkeys[i]==defaults[i],"other assignments preserved");
    read.ResetDefaults(false);
    check(read.hotkeys==kDefaultHotkeys,"original default bindings retained");
    check(!read.unlimitedWrathEnabled&&!read.unlimitedUltimateEnabled&&!read.unlimitedSynergyEnabled,"reset restores Unlimited OFF");
    WritePrivateProfileStringW(L"Features",L"UnlimitedWrath",nullptr,path);
    WritePrivateProfileStringW(L"Features",L"UnlimitedUltimate",nullptr,path);
    WritePrivateProfileStringW(L"Features",L"UnlimitedSynergy",nullptr,path);
    WritePrivateProfileStringW(L"Features",L"SkipWarning",nullptr,path);
    check(read.Load(),"legacy config without new keys loads");
    check(!read.unlimitedWrathEnabled&&!read.unlimitedUltimateEnabled&&!read.unlimitedSynergyEnabled,"missing Unlimited keys remain OFF");
    check(read.skipWarningEnabled,"missing Skip Warning key uses requested ON default");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);
    DeleteFileW(path); // only the exact temporary file created by this test
    std::printf("Gauge hotkeys: %u failures\n",failed);
    return failed?1:0;
}
