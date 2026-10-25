#include "ConfigStore.h"
#include "MenuFontScope.h"
#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <limits>
namespace {
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
bool Near(float a,float b){return std::fabs(a-b)<.001f;}
void Config(){
 wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
 Check(GetTempPathW(MAX_PATH,directory)&&GetTempFileNameW(directory,L"DGF",0,path),"temporary profile");
 dg::config::Store a;a.SetPath(path);Check(a.Load()&&a.menuFontSize==13,"old INI preserves original font size");
 a.menuFontSize=22.5f;a.menuKey=0xC0;a.wrathSlots[0][0]=6;Check(a.SaveNow(),"font and existing settings saved");
 dg::config::Store b;b.SetPath(path);Check(b.Load()&&b.menuFontSize==22.5f&&b.menuKey==0xC0&&b.wrathSlots[0][0]==6,"manual font, binding and Stoneskin survive reload");
 for(const auto* value:{L"nan",L"oops",L"-10",L"999"}){
  WritePrivateProfileStringW(L"Overlay",L"FontSize",value,path);Check(b.Load(),"font profile loads");
  const float expected=value==std::wstring(L"-10")?10.f:(value==std::wstring(L"999")?32.f:13.f);
  Check(b.menuFontSize==expected,"invalid/nonfinite font safely defaults or clamps");
 }
 b.menuFontSize=std::numeric_limits<float>::infinity();Check(b.SaveNow()&&b.menuFontSize==13,"save also rejects nonfinite size");
 b.ResetDefaults(false);Check(b.menuFontSize==13,"factory reset restores font only with normal full reset");DeleteFileW(path);
}
void Scope(){
 ImGui::CreateContext();auto& io=ImGui::GetIO();io.IniFilename=nullptr;io.DisplaySize={1600,1200};io.DeltaTime=1.f/60;
 unsigned char* pixels=nullptr;int width=0,height=0;io.Fonts->GetTexDataAsRGBA32(&pixels,&width,&height);
 io.FontGlobalScale=1.25f;ImGui::NewFrame();const float before=ImGui::GetFontSize();
 {
  const dg::overlay::MenuFontScope scope(26);ImGui::Begin("Mod menu");Check(Near(ImGui::GetFontSize(),26),"menu renders at requested size");
  ImGui::OpenPopup("Combo or modal");if(ImGui::BeginPopup("Combo or modal")){Check(Near(ImGui::GetFontSize(),26),"menu popup inherits size");ImGui::EndPopup();}
  ImGui::End();
 }
 Check(io.FontGlobalScale==1.25f&&Near(ImGui::GetFontSize(),before),"font stack and global scale restored after menu");
 ImGui::Begin("Outside menu");Check(Near(ImGui::GetFontSize(),before),"other overlays retain original font");ImGui::End();ImGui::EndFrame();ImGui::DestroyContext();
}
}
int main(){try{Config();Scope();std::cout<<"Menu font config, popup scope and outside overlay preservation passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
