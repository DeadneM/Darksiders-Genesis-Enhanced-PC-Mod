#pragma once
#include "imgui.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <string_view>
#include <utility>

namespace dg::overlay::layout {
class StyleScope {
public:
    StyleScope(){
        const float scale=ImGui::GetFontSize()/13.f;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(14*scale,12*scale));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(7*scale,4*scale));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(9*scale,6*scale));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing,ImVec2(6*scale,4*scale));
        ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing,18*scale);
    }
    ~StyleScope(){ImGui::PopStyleVar(5);}
    StyleScope(const StyleScope&)=delete;StyleScope& operator=(const StyleScope&)=delete;
};
inline float ValueIndent(){return ImGui::GetFrameHeight()+ImGui::GetStyle().ItemInnerSpacing.x;}
inline float ControlWidth(float maximum=360.f){
    return std::max(1.f,std::min(ImGui::GetContentRegionAvail().x,maximum*ImGui::GetFontSize()/13.f));
}
inline void Label(const char* label){
    const auto text=std::string_view(label);const auto end=text.find("##");
    ImGui::TextWrapped("%.*s",int(end==text.npos?text.size():end),label);
}
// Keep one sentence visible. Decimal points are not sentence boundaries.
// The complete original explanation remains available on hover.
inline std::string_view Summary(std::string_view text){
    constexpr size_t limit=100;
    for(size_t i=0;i<std::min(text.size(),limit);++i){
        if((text[i]=='.'||text[i]=='!'||text[i]=='?')&&
           (i+1==text.size()||std::isspace(static_cast<unsigned char>(text[i+1]))))return text.substr(0,i+1);
    }
    if(text.size()<=limit)return text;
    size_t end=text.rfind(' ',limit);if(end==text.npos||end<40)end=limit;
    return text.substr(0,end);
}
inline void HelpText(const char* text){
    if(!text||!*text)return;
    const std::string_view full(text);const auto brief=Summary(full);const bool details=brief.size()<full.size();
    ImGui::PushStyleColor(ImGuiCol_Text,ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
    ImGui::TextWrapped("%.*s%s",int(brief.size()),text,details?"  (?)":"");
    ImGui::PopStyleColor();
    if(details&&ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort)){
        ImGui::BeginTooltip();ImGui::PushTextWrapPos(ImGui::GetFontSize()*30.f);
        ImGui::TextUnformatted(text);ImGui::PopTextWrapPos();ImGui::EndTooltip();
    }
}
inline void SameLineIfFits(float width){
    const float right=ImGui::GetCursorScreenPos().x+ImGui::GetContentRegionAvail().x;
    if(ImGui::GetItemRectMax().x+ImGui::GetStyle().ItemSpacing.x+width<=right)ImGui::SameLine();
}
template<class... Args> bool Choice(const char* label,int* value,Args&&... args){
    ImGui::PushID(label);Label(label);ImGui::SetNextItemWidth(ControlWidth());
    const bool changed=ImGui::Combo("##Choice",value,std::forward<Args>(args)...);
    ImGui::PopID();return changed;
}
inline bool NumberRow(const char* label,const char* id,float* value,
    float minimum,float maximum,float original,const char* format,const char* inputFormat="%.2f"){
    ImGui::PushID(id);Label(label);
    const float available=std::max(1.f,ImGui::GetContentRegionAvail().x);
    const float scale=ImGui::GetFontSize()/13.f,gap=ImGui::GetStyle().ItemSpacing.x;
    const float resetWidth=ImGui::CalcTextSize("Default").x+2*ImGui::GetStyle().FramePadding.x;
    const float manualWidth=std::min(100.f*scale,available);
    const bool oneLine=available>=160.f*scale+manualWidth+resetWidth+2*gap;
    ImGui::SetNextItemWidth(oneLine?available-manualWidth-resetWidth-2*gap:available);
    bool changed=ImGui::SliderFloat("##Slider",value,minimum,maximum,format);
    if(oneLine)ImGui::SameLine();
    ImGui::SetNextItemWidth(manualWidth);changed|=ImGui::InputFloat("##Manual",value,0,0,inputFormat);
    if(ImGui::IsItemHovered())ImGui::SetTooltip("Enter an exact value.");
    SameLineIfFits(resetWidth);
    if(ImGui::Button("Default")){*value=original;changed=true;}
    if(changed){if(!std::isfinite(*value))*value=original;*value=std::clamp(*value,minimum,maximum);}
    ImGui::PopID();return changed;
}
}
