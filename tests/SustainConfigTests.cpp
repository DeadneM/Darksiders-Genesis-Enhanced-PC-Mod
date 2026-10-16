#include "ConfigStore.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <cstdio>
int main(){
    using namespace dg::config;
    static_assert(int(Action::UnlimitedPotions)==39&&int(Action::RefillHealth)==40&&int(Action::UnlimitedHealth)==41&&int(Action::UnlimitedGlide)==42&&int(Action::UnlimitedHorseSprint)==43);
    unsigned failed=0;auto check=[&](bool v,const char* msg){if(!v){++failed;std::printf("FAIL %s\n",msg);}};
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    if(!GetTempPathW(MAX_PATH,directory)||!GetTempFileNameW(directory,L"DGS",0,path))return 1;
    Store a;a.SetPath(path);
    check(!a.unlimitedHealthEnabled&&!a.unlimitedGlideEnabled&&!a.unlimitedHorseSprintEnabled,"all new native changes are opt-in");
    check(a.hotkeys==kDefaultHotkeys,"existing twelve hotkeys are retained");
    a.unlimitedHealthEnabled=true;a.unlimitedGlideEnabled=true;a.unlimitedHorseSprintEnabled=true;
    a.hotkeys[8]=Action::RefillHealth;a.hotkeys[9]=Action::UnlimitedHealth;a.hotkeys[10]=Action::UnlimitedGlide;a.hotkeys[11]=Action::UnlimitedHorseSprint;
    check(a.SaveNow(),"new options and four actions save");Store b;b.SetPath(path);check(b.Load(),"saved options load");
    check(b.unlimitedHealthEnabled&&b.unlimitedGlideEnabled&&b.unlimitedHorseSprintEnabled&&b.hotkeys==a.hotkeys,"new options and appended action tokens round-trip");
    const auto& rt=dg::runtime::Get();check(rt.unlimitedHealthEnabled.load()&&rt.unlimitedGlideEnabled.load()&&rt.unlimitedHorseSprintEnabled.load(),"all options reach native runtime atomics");
    WritePrivateProfileStringW(L"Features",L"UnlimitedHealth",nullptr,path);WritePrivateProfileStringW(L"Features",L"UnlimitedGlide",nullptr,path);WritePrivateProfileStringW(L"Features",L"UnlimitedHorseSprint",nullptr,path);
    check(b.Load(),"legacy profile loads");check(!b.unlimitedHealthEnabled&&!b.unlimitedGlideEnabled&&!b.unlimitedHorseSprintEnabled&&b.hotkeys==a.hotkeys,"missing new keys opt out without replacing personal hotkeys");
    b.ResetDefaults(false);check(!b.unlimitedHealthEnabled&&!b.unlimitedGlideEnabled&&!b.unlimitedHorseSprintEnabled&&b.hotkeys==kDefaultHotkeys,"reset restores opt-in defaults and established bindings");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
    std::printf("Sustain config: %u failures\n",failed);return failed?1:0;
}
