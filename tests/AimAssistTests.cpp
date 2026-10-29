#include "AimAssistSettings.h"
#include "ConfigStore.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <limits>
namespace {
void Check(bool b,const char* label){if(!b)throw std::runtime_error(label);}
struct Vec {float x,y,z;};struct Ray {Vec origin,direction;bool mounted=true;};
void Run(){
    using namespace dg::aim_assist;
    const Ray ray{{0,0,0},{1,0,0}};const Vec candidate{1000,50,20},farPoint{100000,0,0};Ray quarter{},half{},full{};
    Check(!Pull(ray,candidate,farPoint,false,0,full),"zero adds no assistance");
    Check(Pull(ray,candidate,farPoint,false,25,quarter)&&Pull(ray,candidate,farPoint,false,50,half)&&Pull(ray,candidate,farPoint,false,100,full),"all positive strengths work in the bounded cone");
    Check(quarter.direction.y>0&&quarter.direction.y<half.direction.y&&half.direction.y<full.direction.y,"strength changes progressively rather than switching a lock");
    Check(std::abs(full.direction.y/full.direction.x-.05f)<.0001f&&full.direction.z>0&&full.mounted,"full correction aims in3D and preserves ray context");
    Check(!Pull(ray,Vec{1000,150,0},farPoint,false,100,full)&&!Pull(ray,Vec{-100,0,0},farPoint,false,100,full),"lateral and backward candidates cannot attract the shot");
    Check(!Pull(ray,candidate,Vec{500,0,0},true,100,full),"a closer reported collision has priority");
    Check(ClearPath(ray,Vec{1000,0,0},farPoint,false,false),"a character that overlaps Projectile does not need to block the visibility ray");
    Check(ClearPath(ray,Vec{1000,0,0},Vec{1500,0,0},true,false),"world geometry behind the selected character does not hide it");
    Check(!ClearPath(ray,Vec{1000,0,0},Vec{500,0,0},true,false),"a different blocking actor in front still hides the selected character");
    Check(ClearPath(ray,Vec{1000,0,0},Vec{950,0,0},true,true),"the selected actor's near surface is visible before its root point");
    Check(!ClearPath(ray,Vec{1000,0,0},Vec{std::numeric_limits<float>::quiet_NaN(),0,0},true,true),"invalid blocking points remain rejected");
    Check(!Pull(ray,candidate,farPoint,false,std::numeric_limits<float>::quiet_NaN(),full)&&Strength(150)==100&&Strength(-4)==0,"invalid settings cannot create an unbounded correction");
    wchar_t dir[MAX_PATH]{},path[MAX_PATH]{};
    Check(GetTempPathW(MAX_PATH,dir)&&GetTempFileNameW(dir,L"DGA",0,path),"isolated INI");
    dg::config::Store a;a.SetPath(path);Check(a.Load()&&a.tpsAimAssistMode==0&&a.tpsAimAssistGlobal==0,"legacy profile preserves V1.130 by default");
    for(float value:a.tpsAimAssistAmmo)Check(value==0,"per-ammunition defaults also preserve V1.130");
    const auto keys=a.hotkeys;a.menuKey=VK_F10;a.tpsAimAssistGlobal=35;a.tpsAimAssistMode=1;
    for(unsigned i=0;i<Count;++i)a.tpsAimAssistAmmo[i]=float(i)*12.5f;
    Check(a.SaveNow(),"save distinct values");dg::config::Store b;b.SetPath(path);Check(b.Load(),"reload");
    Check(b.menuKey==VK_F10&&b.hotkeys==keys&&b.tpsAimAssistMode==1&&b.tpsAimAssistGlobal==35,"mode/global/personal bindings round trip");
    for(unsigned i=0;i<Count;++i)Check(b.tpsAimAssistAmmo[i]==float(i)*12.5f&&dg::runtime::Get().tpsAimAssistAmmo[i].load()==b.tpsAimAssistAmmo[i],"seven independent values reach runtime");
    WritePrivateProfileStringW(L"Values",Keys[0],L"nan",path);WritePrivateProfileStringW(L"Values",Keys[1],L"999",path);
    Check(b.Load()&&b.tpsAimAssistAmmo[0]==0&&b.tpsAimAssistAmmo[1]==100,"disk values normalized safely");
    b.ResetDefaults(false);Check(b.tpsAimAssistMode==0&&b.tpsAimAssistGlobal==0,"reset restores the validated baseline");DeleteFileW(path);
}
}
int main(){try{Run();std::cout<<"Bounded TPS assistance, collision priority and independent INI values passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
