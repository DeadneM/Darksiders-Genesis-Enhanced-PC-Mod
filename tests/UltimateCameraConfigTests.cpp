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
    WritePrivateProfileStringW(L"Values",L"ThirdPersonDistanceMultiplier",L"0.620",path);
    WritePrivateProfileStringW(L"Values",L"ThirdPersonHeightOffset",L"123.000",path);
    dg::config::Store c;c.SetPath(path);check(c.Load(),"old INI loads");
    check(almostEqual(c.tpsUltimateDistanceMultiplier,0.62f)&&almostEqual(c.tpsUltimateHeightOffset,123),"missing Ultimate keys inherit personal camera");
    c.tpsUltimateDistanceMultiplier=0.9f;c.tpsUltimateHeightOffset=260;
    check(c.SaveNow(),"new camera values saved");
    dg::config::Store read;read.SetPath(path);check(read.Load(),"new INI reloads");
    check(almostEqual(read.tpsUltimateDistanceMultiplier,0.9f)&&almostEqual(read.tpsUltimateHeightOffset,260),"Ultimate values persist");
    check(almostEqual(read.thirdPersonDistanceMultiplier,0.62f)&&almostEqual(read.thirdPersonHeightOffset,123),"normal camera remains independent");
    auto& rt=dg::runtime::Get();
    check(almostEqual(rt.tpsUltimateDistanceMultiplier.load(),0.9f)&&almostEqual(rt.tpsUltimateHeightOffset.load(),260),"runtime receives Ultimate values");
    WritePrivateProfileStringW(L"Values",L"TPSUltimateDistanceMultiplier",L"99",path);
    WritePrivateProfileStringW(L"Values",L"TPSUltimateHeightOffset",L"-2000",path);
    check(read.Load()&&almostEqual(read.tpsUltimateDistanceMultiplier,3)&&almostEqual(read.tpsUltimateHeightOffset,-1000),"native camera limits respected");
    read.ResetTpsCameraValues();
    check(almostEqual(read.tpsUltimateDistanceMultiplier,0.30f)&&almostEqual(read.tpsUltimateHeightOffset,80),"TPS reset includes Ultimate values");
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
    std::printf("Ultimate camera config: %u failures\n",failed);return failed?1:0;
}
