#include "AimAssistSettings.h"
#include "ConfigStore.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <limits>
namespace {
void Check(bool b,const char* label){if(!b)throw std::runtime_error(label);}
void Run(){
    using namespace dg::aim_assist;
    float v=0,h=0;
    Check(Angles(90,45,100,v,h)&&v==90&&h==45,"100 preserves native angular limits exactly");
    Check(Angles(90,45,50,v,h)&&v==45&&h==22.5f,"lower strength narrows only acquisition angles");
    Check(Angles(90,45,200,v,h)&&v==180&&h==90,"higher strength widens acquisition within valid native angular domain");
    Check(Angles(90,45,0,v,h)&&v==0&&h==0,"zero restricts acquisition to exact axis without a post-selection target deletion");
    Check(!Angles(std::numeric_limits<float>::quiet_NaN(),45,50,v,h)&&Strength(999)==200&&Strength(-5)==0&&Strength(std::numeric_limits<float>::quiet_NaN())==100,"invalid inputs retain native reference or reject transaction");
    wchar_t dir[MAX_PATH]{},path[MAX_PATH]{};
    Check(GetTempPathW(MAX_PATH,dir)&&GetTempFileNameW(dir,L"DGA",0,path),"isolated INI");
    dg::config::Store a;a.SetPath(path);Check(a.Load()&&a.tpsAimAssistMode==0&&a.tpsAimAssistGlobal==100,"legacy profile defaults to native acquisition");
    for(float value:a.tpsAimAssistAmmo)Check(value==100,"per-ammunition defaults use native reference");
    Check(!a.tpsAimAssistEnabled,"native adjustment defaults OFF");
    WritePrivateProfileStringW(L"Values",L"TPSAimAssistGlobal",L"0",path);Check(a.Load()&&a.tpsAimAssistGlobal==100,"legacy added-pull zero cannot become native-assist zero");
    const auto keys=a.hotkeys;a.tpsAimAssistEnabled=true;a.menuKey=VK_F10;a.tpsAimAssistGlobal=35;a.tpsAimAssistMode=1;
    for(unsigned i=0;i<Count;++i)a.tpsAimAssistAmmo[i]=float(i)*12.5f;
    Check(a.SaveNow(),"save distinct values");dg::config::Store b;b.SetPath(path);Check(b.Load(),"reload");
    Check(b.menuKey==VK_F10&&b.hotkeys==keys&&b.tpsAimAssistEnabled&&b.tpsAimAssistMode==1&&b.tpsAimAssistGlobal==35,"mode/global/personal bindings round trip");
    for(unsigned i=0;i<Count;++i)Check(b.tpsAimAssistAmmo[i]==float(i)*12.5f&&dg::runtime::Get().tpsAimAssistAmmo[i].load()==b.tpsAimAssistAmmo[i],"seven independent values reach runtime");
    WritePrivateProfileStringW(L"Values",Keys[0],L"nan",path);WritePrivateProfileStringW(L"Values",Keys[1],L"999",path);
    Check(b.Load()&&b.tpsAimAssistAmmo[0]==100&&b.tpsAimAssistAmmo[1]==200,"disk values normalized safely");
    b.ResetDefaults(false);Check(b.tpsAimAssistMode==0&&b.tpsAimAssistGlobal==100&&!b.tpsAimAssistEnabled,"reset restores native acquisition reference");DeleteFileW(path);
}
}
int main(){try{Run();std::cout<<"Native acquisition scaling, legacy schema isolation and independent INI values passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
