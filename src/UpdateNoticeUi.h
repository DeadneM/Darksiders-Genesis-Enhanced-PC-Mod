#pragma once
#include <imgui.h>
#include <algorithm>

namespace dg::overlay {
inline void DrawUpdateBanner(const char* version,const char* channel){
    ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(1.0f,0.85f,0.2f,1.0f));
    ImGui::TextWrapped("Update available: %s (%s). Open Updates to install on game exit.",version,channel);
    ImGui::PopStyleColor();
}
inline void DrawUpdateToast(const char* version,const char* channel){
    const auto* viewport=ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x+viewport->WorkSize.x-20,
                                 viewport->WorkPos.y+20),ImGuiCond_Always,ImVec2(1,0));
    ImGui::SetNextWindowBgAlpha(0.92f);
    constexpr auto flags=ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_AlwaysAutoResize|
        ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_NoNav|ImGuiWindowFlags_NoFocusOnAppearing|
        ImGuiWindowFlags_NoSavedSettings;
    if(ImGui::Begin("##UpdateAvailableToast",nullptr,flags)){
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX()+std::min(400.0f,viewport->WorkSize.x-64.0f));
        DrawUpdateBanner(version,channel);
        ImGui::PopTextWrapPos();
    }
    ImGui::End();
}
}
