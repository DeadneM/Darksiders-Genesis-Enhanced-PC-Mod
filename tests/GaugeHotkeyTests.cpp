#include "ConfigStore.h"
#include "GaugeRequestPolicy.h"
#include <windows.h>
#include <cstdio>
#include <cwchar>

int main(){
    using namespace dg::config;
    static_assert(static_cast<int>(Action::TpsCameraReset)==26);
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
    const auto defaults=write.hotkeys;
    write.hotkeys[5]=Action::RefillWrath;
    write.hotkeys[6]=Action::RefillUltimate;
    write.hotkeys[7]=Action::RefillSynergy;
    write.hotkeys[8]=Action::RefillAllGauges;
    check(write.SaveNow(),"config save succeeds");
    Store read;
    read.SetPath(path);
    check(read.Load(),"config load succeeds");
    check(read.hotkeys==write.hotkeys,"four new hotkeys survive actual INI save/reload");
    for(size_t i=0;i<defaults.size();++i)
        if(i<5||i>8)check(read.hotkeys[i]==defaults[i],"other assignments preserved");
    read.ResetDefaults(false);
    check(read.hotkeys==kDefaultHotkeys,"original default bindings retained");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);
    DeleteFileW(path); // only the exact temporary file created by this test
    std::printf("Gauge hotkeys: %u failures\n",failed);
    return failed?1:0;
}
