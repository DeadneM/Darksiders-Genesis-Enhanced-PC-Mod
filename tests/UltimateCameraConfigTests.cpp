#include "ConfigStore.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <cmath>
#include <cstdio>
int main(){
    unsigned failed=0;
    const auto check=[&](bool pass,const char* name){if(!pass){++failed;std::printf("FAIL %s\n",name);}};
    const auto almostEqual=[](float a,float b){return std::fabs(a-b)<0.0001f;};
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    if(!GetTempPathW(MAX_PATH,directory)||!GetTempFileNameW(directory,L"DGU",0,path))return 1;
    WritePrivateProfileStringW(L"Meta",L"ConfigRevision",L"2140",path);
    WritePrivateProfileStringW(L"Values",L"TPSFOVDegrees",L"97.500",path);
    WritePrivateProfileStringW(L"Values",L"ThirdPersonDistanceMultiplier",L"0.620",path);
    WritePrivateProfileStringW(L"Values",L"ThirdPersonHeightOffset",L"123.000",path);
    dg::config::Store c;c.SetPath(path);check(c.Load(),"old INI loads");
    check(almostEqual(c.tpsUltimateDistanceMultiplier,0.62f)&&almostEqual(c.tpsUltimateHeightOffset,123),"missing Ultimate keys inherit personal camera");
    check(almostEqual(c.tpsHorseDistanceMultiplier,0.62f)&&almostEqual(c.tpsHorseHeightOffset,123),"missing Horse keys inherit personal camera");
    check(almostEqual(c.thirdPersonCoopFovDegrees,97.5f),"missing coop FOV inherits personal solo FOV");
    c.thirdPersonCoopFovDegrees=83.0f;
    c.tpsUltimateDistanceMultiplier=0.9f;c.tpsUltimateHeightOffset=260;
    c.tpsHorseDistanceMultiplier=1.1f;c.tpsHorseHeightOffset=320;
    check(c.SaveNow(),"new camera values saved");
    dg::config::Store read;read.SetPath(path);check(read.Load(),"new INI reloads");
    check(almostEqual(read.tpsUltimateDistanceMultiplier,0.9f)&&almostEqual(read.tpsUltimateHeightOffset,260),"Ultimate values persist");
    check(almostEqual(read.tpsHorseDistanceMultiplier,1.1f)&&almostEqual(read.tpsHorseHeightOffset,320),"Horse values persist independently of Ultimate");
    check(almostEqual(read.thirdPersonDistanceMultiplier,0.62f)&&almostEqual(read.thirdPersonHeightOffset,123),"normal camera remains independent");
    check(almostEqual(read.thirdPersonFovDegrees,97.5f)&&almostEqual(read.thirdPersonCoopFovDegrees,83.0f),"separate solo and coop FOV values persist");
    auto& rt=dg::runtime::Get();
    check(almostEqual(rt.thirdPersonFovDegrees.load(),97.5f)&&almostEqual(rt.thirdPersonCoopFovDegrees.load(),83.0f),"runtime receives both independent FOV values");
    check(almostEqual(rt.tpsUltimateDistanceMultiplier.load(),0.9f)&&almostEqual(rt.tpsUltimateHeightOffset.load(),260),"runtime receives Ultimate values");
    check(almostEqual(rt.tpsHorseDistanceMultiplier.load(),1.1f)&&almostEqual(rt.tpsHorseHeightOffset.load(),320),"runtime receives Horse values");
    read.thirdPersonFovDegrees=102.0f;
    read.thirdPersonDistanceMultiplier=0.25f;read.thirdPersonHeightOffset=10;
    check(read.SaveNow(),"on-foot camera edit saves");
    dg::config::Store independent;independent.SetPath(path);
    check(independent.Load()&&almostEqual(independent.thirdPersonDistanceMultiplier,0.25f)&&
        almostEqual(independent.thirdPersonHeightOffset,10)&&
        almostEqual(independent.tpsUltimateDistanceMultiplier,0.9f)&&almostEqual(independent.tpsUltimateHeightOffset,260)&&
        almostEqual(independent.tpsHorseDistanceMultiplier,1.1f)&&almostEqual(independent.tpsHorseHeightOffset,320),
        "later on-foot edits preserve explicit Horse and Ultimate settings");
    check(almostEqual(independent.thirdPersonFovDegrees,102.0f)&&almostEqual(independent.thirdPersonCoopFovDegrees,83.0f),"later solo FOV edits preserve explicit coop FOV");
    WritePrivateProfileStringW(L"Values",L"TPSCoopFOVDegrees",L"999",path);
    check(read.Load()&&almostEqual(read.thirdPersonCoopFovDegrees,140),"coop FOV respects native upper limit");
    WritePrivateProfileStringW(L"Values",L"TPSCoopFOVDegrees",L"nan",path);
    check(read.Load()&&almostEqual(read.thirdPersonCoopFovDegrees,102),"invalid coop FOV inherits personal solo FOV");
    WritePrivateProfileStringW(L"Values",L"TPSUltimateDistanceMultiplier",L"99",path);
    WritePrivateProfileStringW(L"Values",L"TPSUltimateHeightOffset",L"-2000",path);
    check(read.Load()&&almostEqual(read.tpsUltimateDistanceMultiplier,3)&&almostEqual(read.tpsUltimateHeightOffset,-1000),"native camera limits respected");
    WritePrivateProfileStringW(L"Values",L"TPSHorseDistanceMultiplier",L"-99",path);
    WritePrivateProfileStringW(L"Values",L"TPSHorseHeightOffset",L"2000",path);
    check(read.Load()&&almostEqual(read.tpsHorseDistanceMultiplier,0)&&almostEqual(read.tpsHorseHeightOffset,1000),"Horse camera limits respected");
    WritePrivateProfileStringW(L"Values",L"TPSHorseDistanceMultiplier",L"nan",path);
    WritePrivateProfileStringW(L"Values",L"TPSHorseHeightOffset",L"inf",path);
    check(read.Load()&&almostEqual(read.tpsHorseDistanceMultiplier,0.25f)&&almostEqual(read.tpsHorseHeightOffset,10),"invalid Horse values fall back to current normal camera");
    read.ResetTpsCameraValues();
    check(almostEqual(read.thirdPersonFovDegrees,110)&&almostEqual(read.thirdPersonCoopFovDegrees,110),"TPS reset includes solo and coop FOV");
    check(almostEqual(read.tpsUltimateDistanceMultiplier,0.30f)&&almostEqual(read.tpsUltimateHeightOffset,80),"TPS reset includes Ultimate values");
    check(almostEqual(read.tpsHorseDistanceMultiplier,0.25f)&&almostEqual(read.tpsHorseHeightOffset,40),"TPS reset includes user-chosen Horse values");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
    std::printf("TPS form camera config: %u failures\n",failed);return failed?1:0;
}
