#include "UpdateNoticePolicy.h"
#include "UpdateNoticeUi.h"
#include "imgui_internal.h"
#include <iostream>
#include <stdexcept>
namespace {
void Check(bool ok,const char* reason){if(!ok)throw std::runtime_error(reason);}
void Run(){
    dg::update::ToastTimer timer;
    Check(!timer.Tick(0,0,true),"no newer result means no toast");
    Check(!timer.Tick(3,1,false)&&!timer.Tick(3,60,false),"background result waits without losing its duration");
    Check(timer.Tick(3,100,true)&&timer.Tick(3,107,true),"toast remains visible during its eight second interval");
    Check(!timer.Tick(3,108,true)&&!timer.Tick(3,109,true),"expired result cannot keep rendering ImGui");
    Check(timer.Tick(5,110,true),"a later successful newer check can notify again");
    Check(!timer.Tick(5,111,false)&&!timer.Tick(5,1000,false),"Alt-Tab hides the toast");
    Check(timer.Tick(5,1001,true)&&timer.Tick(5,1007,true),"Alt-Tab time is excluded");
    Check(!timer.Tick(0,1007,true),"invalidating or changing channel removes the toast");
    Check(!timer.Tick(0,1008,true),"current build, old build or failed check does not notify");

    ImGui::CreateContext();auto& io=ImGui::GetIO();io.IniFilename=nullptr;
    io.DisplaySize={1280,720};io.DeltaTime=1.0f/60;
    unsigned char* pixels=nullptr;int w=0,h=0;io.Fonts->GetTexDataAsRGBA32(&pixels,&w,&h);
    ImGui::NewFrame();
    dg::overlay::DrawUpdateToast("V9.99","debug");
    auto* toast=ImGui::FindWindowByName("##UpdateAvailableToast");
    Check(toast&&(toast->Flags&ImGuiWindowFlags_NoInputs)==ImGuiWindowFlags_NoInputs,
          "actual toast window rejects mouse and navigation input");
    Check((toast->Flags&ImGuiWindowFlags_NoFocusOnAppearing)!=0&&GImGui->NavWindow!=toast,
          "actual toast cannot take focus from the game or menu");
    ImGui::Begin("Mod header");dg::overlay::DrawUpdateBanner("V9.99","main");ImGui::End();
    Check(io.MouseDrawCursor==false,"passive UI never requests a mouse cursor");
    ImGui::Render();ImGui::DestroyContext();
}
}
int main(){try{Run();std::cout<<"Update notification lifetime and passive Dear ImGui window passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
