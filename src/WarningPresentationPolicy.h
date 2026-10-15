#pragma once
#include <cstdint>
#include <string_view>
namespace dg::skip_warning {
inline bool IsKnownWarning(unsigned mode,std::wstring_view path){
    return (mode==0&&path==L"/Game/UI/Textures/UI_SaveIconStatic_1.UI_SaveIconStatic_1")||
        (mode==2&&(path==L"/Game/UI/Textures/ButtonArt/UI_GamepadDiagram_XboneFrontOnly.UI_GamepadDiagram_XboneFrontOnly"||
                  path==L"/Game/UI/Textures/ButtonArt/UI_GamepadDiagram_StadiaFrontOnly.UI_GamepadDiagram_StadiaFrontOnly"));
}
inline bool CanAdvanceWarning(int current,int target,int count,uint32_t knownMask){
    if(count<1||count>16||current< -1||current>=count||target<0||target>count||
       (knownMask>>count))return false;
    // Finish a known page through its native fade-out; never advance a page
    // whose identity was not verified, even when a known page follows it.
    const int page=current>=0?current:target;
    return page<count&&(knownMask&(1u<<page));
}
}
