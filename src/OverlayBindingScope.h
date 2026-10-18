#pragma once
#include "imgui.h"

namespace dg::overlay {
struct BindingRowScope {
    explicit BindingRowScope(int index){
        // TableHeadersRow also pushes its integer column index. A row index
        // alone makes row 3's Clear collide with the Clear column header.
        ImGui::PushID("KeyboardBindingRow");
        ImGui::PushID(index);
    }
    ~BindingRowScope(){ImGui::PopID();ImGui::PopID();}
    BindingRowScope(const BindingRowScope&)=delete;
    BindingRowScope& operator=(const BindingRowScope&)=delete;
};
}
