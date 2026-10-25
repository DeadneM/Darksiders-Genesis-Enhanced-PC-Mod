#pragma once
#include "MenuAppearance.h"
#include "imgui.h"
namespace dg::overlay {
// Scope the existing ImGui 1.91 font scale to this menu and its popups. Restore
// before PopFont recomputes the previous font, including the early Begin exit.
class MenuFontScope {
    float previous_;
public:
    explicit MenuFontScope(float size):previous_(ImGui::GetIO().FontGlobalScale){
        auto* font=ImGui::GetFont();
        ImGui::GetIO().FontGlobalScale=menu::FontSize(size)/(font->FontSize*font->Scale);
        ImGui::PushFont(font);
    }
    ~MenuFontScope(){ImGui::GetIO().FontGlobalScale=previous_;ImGui::PopFont();}
    MenuFontScope(const MenuFontScope&)=delete;
    MenuFontScope& operator=(const MenuFontScope&)=delete;
};
}
