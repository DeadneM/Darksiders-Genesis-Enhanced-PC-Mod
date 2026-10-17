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
    check(generated.tps3DAimEnabled&&dg::runtime::Get().tps3DAimEnabled.load(),"missing option enables 3D aiming by default");
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
    WritePrivateProfileStringW(L"Features",L"ReduceAmmoConsumption",L"0",path);
    WritePrivateProfileStringW(L"Values",L"TPSUltimateDistanceMultiplier",L"0.900",path);
    WritePrivateProfileStringW(L"Values",L"TPSUltimateHeightOffset",L"123.000",path);
    WritePrivateProfileStringW(L"Values",L"WrathChargeMultiplier",L"2.000",path);
    WritePrivateProfileStringW(L"Hotkeys",L"F9",L"FOV",path);
    dg::config::Store personal;personal.SetPath(path);check(personal.Load(),"explicit older personal profile loads");
    check(personal.menuKey==0xC0&&!personal.updaterBeta&&!personal.reduceAmmoEnabled,"explicit older toggles/menu retained");
    check(personal.tpsUltimateDistanceMultiplier==0.9f&&personal.tpsUltimateHeightOffset==123&&personal.wrathChargeMultiplier==2,"explicit personal values retained");
    check(personal.hotkeys[8]==dg::config::Action::FOV,"explicit older hotkey retained");
    check(generated.menuKey==VK_INSERT,"Insert is the authoritative menu default");
    personal.tps3DAimEnabled=false;check(personal.SaveNow(),"simple aim option saves");
    dg::config::Store reloaded;reloaded.SetPath(path);check(reloaded.Load()&&!reloaded.tps3DAimEnabled&&!rt.tps3DAimEnabled.load(),"explicit simple aim survives reload and runtime publication");
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
    check(personal.tpsUltimateDistanceMultiplier==0.30f&&personal.tpsUltimateHeightOffset==80,"camera reset uses requested Ultimate defaults");
    personal.ResetDefaults(false);check(personal.SaveNow(),"full compiled defaults reset saves");
    const auto reset=ReadIni(path);
    for(const auto& [section,values]:shipped){
        const auto sectionIt=reset.find(section);check(sectionIt!=reset.end(),"reset section retained");
        if(sectionIt==reset.end())continue;
        for(const auto& [key,value]:values){const auto item=sectionIt->second.find(key);
            check(item!=sectionIt->second.end()&&_wcsicmp(item->second.c_str(),value.c_str())==0,"reset reproduces shipped user defaults");}
    }
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
    std::printf("User defaults: %zu keys checked, %u failures\n",keys,failed);return failed?1:0;
}
