#include "OverlayBindingScope.h"
#include <array>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
namespace {
void Check(bool ok,const char* reason){if(!ok)throw std::runtime_error(reason);}
void Run(){
    ImGui::CreateContext();
    auto& io=ImGui::GetIO();io.IniFilename=nullptr;io.DisplaySize={1600,1200};io.DeltaTime=1.0f/60;
    unsigned char* pixels=nullptr;int width=0,height=0;
    io.Fonts->GetTexDataAsRGBA32(&pixels,&width,&height);
    ImGui::NewFrame();ImGui::SetNextWindowSize({1500,1100});ImGui::Begin("Binding regression");
    const char* headings[]={"Action","Current key","Change","Clear","Reset"};
    for(const char* table:{"NativeBindings","TPSBindings","OtherBindings","SheathingBindings"}){
        Check(ImGui::BeginTable(table,5),"headless native table available");
        std::unordered_set<ImGuiID> ids;
        std::array<ImGuiID,5> headers{};
        for(int column=0;column<5;++column){
            ImGui::TableSetupColumn(headings[column]);ImGui::PushID(column);
            headers[column]=ImGui::GetID(headings[column]);ImGui::PopID();
            Check(ids.insert(headers[column]).second,"headers have distinct real ImGui identifiers");
        }
        ImGui::TableHeadersRow();
        ImGui::PushID(3);Check(ImGui::GetID("Clear")==headers[3],
            "reproduce the exact pre-fix Zoom In / Clear-header collision");ImGui::PopID();
        for(int row=0;row<15;++row){
            const dg::overlay::BindingRowScope scope(row);
            ImGui::TableNextRow();
            for(int column=2;column<5;++column){
                ImGui::TableSetColumnIndex(column);
                const char* label=column==2?"Change":column==3?"Clear":"Default";
                const auto id=ImGui::GetID(label);
                Check(ids.insert(id).second,"each actual row control is independent of every header and other row");
                ImGui::Button(label);
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();ImGui::Render();ImGui::DestroyContext();
}
}
int main(){try{Run();std::cout<<"Real Dear ImGui binding headers and controls have unique identities\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
