#include "WarningPresentationPolicy.h"
#include <cstdio>
int main(){
    using namespace dg::skip_warning;
    unsigned failed=0;
    const auto check=[&](bool pass,const char* label){if(!pass){++failed;std::printf("FAIL %s\n",label);}};
    constexpr auto save=L"/Game/UI/Textures/UI_SaveIconStatic_1.UI_SaveIconStatic_1";
    constexpr auto pad=L"/Game/UI/Textures/ButtonArt/UI_GamepadDiagram_XboneFrontOnly.UI_GamepadDiagram_XboneFrontOnly";
    check(IsKnownWarning(0,save)&&IsKnownWarning(2,pad),"exact known warning names and modes accepted");
    check(!IsKnownWarning(2,save)&&!IsKnownWarning(0,pad),"wrong mode rejected");
    check(!IsKnownWarning(0,L"/Game/UI/Textures/UI_SaveIconStatic_1"),"partial path rejected");
    check(!IsKnownWarning(0,L"/Game/UI/Textures/Unknown.Unknown"),"unknown content stays native");
    check(CanAdvanceWarning(-1,0,2,3),"initial fade into verified first page allowed");
    check(CanAdvanceWarning(0,0,2,3),"known page duration allowed");
    check(CanAdvanceWarning(0,1,2,1),"known page can finish native fade into unknown page");
    check(!CanAdvanceWarning(1,1,2,1),"stop immediately on unknown page");
    check(!CanAdvanceWarning(0,1,2,2),"unknown page preceding known page stays native");
    check(CanAdvanceWarning(1,2,2,3),"last known page reaches original native cleanup");
    check(!CanAdvanceWarning(2,2,2,3),"completed index rejected");
    check(!CanAdvanceWarning(-1,-1,2,3),"uninitialized target rejected");
    check(!CanAdvanceWarning(0,3,2,3),"invalid target rejected");
    check(!CanAdvanceWarning(0,0,17,3),"unbounded list rejected");
    check(!CanAdvanceWarning(0,0,2,7),"mask outside verified child count rejected");
    check(!CanAdvanceWarning(0,0,2,0),"no named pages means native behavior");
    std::printf("Warning presentation: %u failures\n",failed);return failed?1:0;
}
