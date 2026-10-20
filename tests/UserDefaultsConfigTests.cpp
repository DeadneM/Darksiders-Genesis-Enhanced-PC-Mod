#include "ConfigStore.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <cstdio>
#include <cwchar>
#include <map>
#include <string>
struct CaseLess {bool operator()(const std::wstring& a,const std::wstring& b)const{return _wcsicmp(a.c_str(),b.c_str())<0;}};
using Section=std::map<std::wstring,std::wstring,CaseLess>;
using Ini=std::map<std::wstring,Section,CaseLess>;
Ini ReadIni(const wchar_t* path){
    wchar_t sections[8192]{},entries[16384]{};
    Ini result;
    if(!GetPrivateProfileSectionNamesW(sections,8192,path))return result;
    for(const wchar_t* section=sections;*section;section+=std::wcslen(section)+1){
        GetPrivateProfileSectionW(section,entries,16384,path);
        auto& values=result[section];
        for(const wchar_t* item=entries;*item;item+=std::wcslen(item)+1){
            const wchar_t* equals=std::wcschr(item,L'=');
            if(equals)values.emplace(std::wstring(item,equals),equals+1);
        }
    }
    return result;
}
int main(int argc,char** argv){
    unsigned failed=0;
    auto check=[&](bool value,const char* label){if(!value){++failed;std::printf("FAIL %s\n",label);}};
    if(argc!=2)return 1;
    wchar_t source[32768]{};
    if(!MultiByteToWideChar(CP_UTF8,0,argv[1],-1,source,32768))return 1;
    const auto shipped=ReadIni(source);check(shipped.size()==10,"shipped profile has all ten sections");
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    if(!GetTempPathW(MAX_PATH,directory)||!GetTempFileNameW(directory,L"DGP",0,path))return 1;
    if(!DeleteFileW(path))return 1; // only this test's empty file, to exercise missing-INI generation
    dg::config::Store generated;generated.SetPath(path);
    check(generated.Load(),"missing INI generated from compiled defaults");
    check(!generated.updaterAutoCheck,"automatic network checks remain opt-in for old and missing profiles");
    check(generated.tps3DAimEnabled&&dg::runtime::Get().tps3DAimEnabled.load(),"missing option enables 3D aiming by default");
    check(generated.tpsLensFocusEnabled&&dg::runtime::Get().tpsLensFocusEnabled.load(),"TPS lens experiment defaults ON in store and runtime");
    check(!generated.nativeBlur.disabled&&!generated.tpsBlur.disabled&&!generated.nativeBlur.custom&&!generated.tpsBlur.custom&&
        !dg::runtime::Get().nativeBlur.Get().custom&&!dg::runtime::Get().tpsBlur.Get().disabled,
        "old and missing profiles keep both independent blur controls opt-in");
    const auto actual=ReadIni(path);
    size_t keys=0;
    check(actual.size()==shipped.size(),"generated and shipped sections match");
    for(const auto& [section,values]:shipped){
        const auto sectionIt=actual.find(section);
        check(sectionIt!=actual.end(),"compiled defaults retain section");
        if(sectionIt==actual.end())continue;
        check(values.size()==sectionIt->second.size(),"compiled/shipped key counts match");
        for(const auto& [key,value]:values){
            ++keys;const auto item=sectionIt->second.find(key);
            const bool same=item!=sectionIt->second.end()&&_wcsicmp(item->second.c_str(),value.c_str())==0;
            if(!same)std::wprintf(L"Mismatch [%ls] %ls expected=%ls\n",section.c_str(),key.c_str(),value.c_str());
            check(same,"compiled default matches actual shipped INI");
        }
    }
    check(keys>100,"complete profile parity checked");
    auto& rt=dg::runtime::Get();
    check(rt.reduceAmmoEnabled.load()&&!rt.unlimitedAmmoEnabled.load(),"runtime receives requested ammo defaults");
    check(rt.wrathChargeMultiplier.load()==4&&rt.ultimateChargeMultiplier.load()==4&&rt.synergyChargeMultiplier.load()==4,"runtime receives x4 gains");
    check(rt.tpsUltimateDistanceMultiplier.load()==0.30f&&rt.tpsUltimateHeightOffset.load()==80,"runtime receives Ultimate camera defaults");
    check(rt.tpsHorseDistanceMultiplier.load()==0.25f&&rt.tpsHorseHeightOffset.load()==40,"runtime receives user-chosen Horse camera defaults");
    WritePrivateProfileStringW(L"Overlay",L"MenuKey",L"VK_C0",path);
    WritePrivateProfileStringW(L"Updater",L"Beta",L"0",path);
    WritePrivateProfileStringW(L"Updater",L"AutoCheckUpdates",L"1",path);
    WritePrivateProfileStringW(L"Features",L"ReduceAmmoConsumption",L"0",path);
    WritePrivateProfileStringW(L"Features",L"TPSLensFocus",L"0",path);
    WritePrivateProfileStringW(L"Features",L"DisableNativeDOF",L"1",path);
    WritePrivateProfileStringW(L"Features",L"TPSCustomDOF",L"1",path);
    WritePrivateProfileStringW(L"Values",L"TPSDOFDistanceMeters",L"17.250",path);
    WritePrivateProfileStringW(L"Values",L"TPSDOFAmount",L"1.125",path);
    WritePrivateProfileStringW(L"Values",L"TPSUltimateDistanceMultiplier",L"0.900",path);
    WritePrivateProfileStringW(L"Values",L"TPSUltimateHeightOffset",L"123.000",path);
    WritePrivateProfileStringW(L"Values",L"WrathChargeMultiplier",L"2.000",path);
    WritePrivateProfileStringW(L"Hotkeys",L"F9",L"FOV",path);
    dg::config::Store personal;personal.SetPath(path);check(personal.Load(),"explicit older personal profile loads");
    check(personal.menuKey==0xC0&&!personal.updaterBeta&&!personal.reduceAmmoEnabled,"explicit older toggles/menu retained");
    check(personal.updaterAutoCheck&&!personal.updaterBeta,"auto check loads independently of the selected stable channel");
    check(personal.tpsUltimateDistanceMultiplier==0.9f&&personal.tpsUltimateHeightOffset==123&&personal.wrathChargeMultiplier==2,"explicit personal values retained");
    check(personal.hotkeys[8]==dg::config::Action::FOV,"explicit older hotkey retained");
    check(!personal.tpsLensFocusEnabled&&!rt.tpsLensFocusEnabled.load(),"explicit lens OFF survives load and runtime publication");
    check(personal.nativeBlur.disabled&&!personal.tpsBlur.disabled&&personal.tpsBlur.custom&&!personal.nativeBlur.custom&&
        rt.nativeBlur.Get().disabled&&rt.tpsBlur.Get().custom&&rt.tpsBlur.Get().distanceMeters==17.25f&&rt.tpsBlur.Get().amount==1.125f,
        "separate native suppression and TPS numeric controls reach the runtime independently");
    const auto originalBindings=personal.cameraKeys;
    for(size_t i=0;i<personal.cameraKeys.size();++i){
        personal.cameraKeys=originalBindings;personal.cameraKeys[i]=0;
        check(personal.SaveNow(),"cleared camera binding saves");
        dg::config::Store cleared;cleared.SetPath(path);
        check(cleared.Load(),"cleared camera binding reloads");
        auto expected=originalBindings;expected[i]=0;
        check(cleared.cameraKeys==expected,"each explicit None, including Zoom In, stays unbound without changing other bindings");
    }
    personal.cameraKeys=originalBindings;
    check(generated.menuKey==VK_INSERT,"Insert is the authoritative menu default");
    personal.tps3DAimEnabled=false;personal.tpsAimPitchOffsetDegrees=17.5f;check(personal.SaveNow(),"simple aim option and independent shot angle save");
    dg::config::Store reloaded;reloaded.SetPath(path);check(reloaded.Load()&&!reloaded.tps3DAimEnabled&&!rt.tps3DAimEnabled.load()&&reloaded.tpsAimPitchOffsetDegrees==17.5f&&rt.tpsAimPitchOffsetDegrees.load()==17.5f,"explicit simple aim and manual shot angle survive reload and runtime publication");
    check(reloaded.updaterAutoCheck&&!reloaded.updaterBeta,"auto check and stable channel survive save and reload");
    check(reloaded.nativeBlur.disabled&&reloaded.tpsBlur.custom&&reloaded.tpsBlur.distanceMeters==17.25f&&reloaded.tpsBlur.amount==1.125f&&
        !reloaded.nativeBlur.custom&&!reloaded.tpsBlur.disabled,
        "both view profiles survive save/reload independently of lens focus and bindings");
    personal.tpsMousePrimaryButton=VK_XBUTTON1;personal.tpsMouseSecondaryButton=VK_MBUTTON;
    personal.tpsFirePrimaryMask=0x4000;personal.tpsFireSecondaryMask=0x8000;
    personal.tpsWheelPadMask=0x0020;personal.tpsGrenadeButtonMask=0x0040;
    personal.horsePadButtonA=0x0001;personal.horsePadButtonB=0x0002;
    personal.Save();
    check(rt.tpsMousePrimaryButton.load()==VK_XBUTTON1&&rt.tpsMouseSecondaryButton.load()==VK_MBUTTON,
        "native callbacks receive rebound mouse controls");
    check(rt.tpsFirePrimaryMask.load()==0x4000&&rt.tpsFireSecondaryMask.load()==0x8000&&
        rt.tpsWheelPadMask.load()==0x0020&&rt.tpsGrenadeButtonMask.load()==0x0040&&
        rt.horsePadButtonA.load()==0x0001&&rt.horsePadButtonB.load()==0x0002,
        "native callbacks receive all rebound controller controls");
    personal.ResetTpsCameraValues();
    check(personal.nativeBlur.disabled&&personal.tpsBlur.custom&&personal.tpsBlur.distanceMeters==17.25f,"TPS geometry reset preserves separate rendering preferences");
    check(personal.tpsLensFocusEnabled&&rt.tpsLensFocusEnabled.load(),"TPS camera reset restores the lens option default");
    check(personal.tpsUltimateDistanceMultiplier==0.30f&&personal.tpsUltimateHeightOffset==80,"camera reset uses requested Ultimate defaults");
    personal.ResetDefaults(false);check(personal.SaveNow(),"full compiled defaults reset saves");
    check(!personal.updaterAutoCheck,"full defaults reset restores manual update checks");
    check(!personal.nativeBlur.disabled&&!personal.tpsBlur.custom&&!rt.nativeBlur.Get().disabled&&!rt.tpsBlur.Get().custom&&
        personal.tpsBlur.distanceMeters==10.0f&&personal.nativeBlur.amount==0.6f,
        "full defaults restore both authored view policies and their inactive numeric defaults");
    const auto reset=ReadIni(path);
    for(const auto& [section,values]:shipped){
        const auto sectionIt=reset.find(section);check(sectionIt!=reset.end(),"reset section retained");
        if(sectionIt==reset.end())continue;
        for(const auto& [key,value]:values){const auto item=sectionIt->second.find(key);
            check(item!=sectionIt->second.end()&&_wcsicmp(item->second.c_str(),value.c_str())==0,"reset reproduces shipped user defaults");}
    }
    WritePrivateProfileStringW(L"Values",L"NativeDOFDistanceMeters",L"-2",path);
    WritePrivateProfileStringW(L"Values",L"TPSDOFDistanceMeters",L"9999",path);
    WritePrivateProfileStringW(L"Values",L"NativeDOFAmount",L"nan",path);
    WritePrivateProfileStringW(L"Values",L"TPSDOFAmount",L"9",path);
    dg::config::Store bounded;bounded.SetPath(path);check(bounded.Load(),"out-of-range blur profile loads safely");
    check(bounded.nativeBlur.distanceMeters==0&&bounded.tpsBlur.distanceMeters==dg::camera_blur::MaxDistanceMeters&&bounded.nativeBlur.amount==0.6f&&bounded.tpsBlur.amount==5,
        "distance and amount reject NaN and clamp each independent view to its documented bounds");
    bounded.nativeBlur.distanceMeters=1250.5f;bounded.tpsBlur.distanceMeters=5000.0f;
    check(bounded.SaveNow(),"long native and TPS distances save");
    dg::config::Store longDistance;longDistance.SetPath(path);
    check(longDistance.Load()&&longDistance.nativeBlur.distanceMeters==1250.5f&&longDistance.tpsBlur.distanceMeters==5000.0f&&
        rt.nativeBlur.Get().distanceMeters==1250.5f&&rt.tpsBlur.Get().distanceMeters==5000.0f,
        "distances beyond one kilometre survive save/reload and reach both runtime profiles");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
    std::printf("User defaults: %zu keys checked, %u failures\n",keys,failed);return failed?1:0;
}
