#include "OverlayLayout.h"
#include "MenuFontScope.h"
#include <array>
#include <iostream>
#include <stdexcept>
#include <string_view>
namespace {
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
struct FrameResult {ImVec2 click;bool changed;};
FrameResult Frame(float width,float font,float& value,int& choice){
    ImGui::NewFrame();const auto before=ImGui::GetStyle();FrameResult result{};
    {
        dg::overlay::MenuFontScope fontScope(font);dg::overlay::layout::StyleScope style;
        ImGui::SetNextWindowPos({10,10});ImGui::SetNextWindowSize({width,900});
        ImGui::Begin("Menu layout test",nullptr,ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoMove);
        const float right=ImGui::GetCursorScreenPos().x+ImGui::GetContentRegionAvail().x;
        result.changed=dg::overlay::layout::NumberRow("Value with a wrapping label","value",&value,0,10,1,"%.2f");
        Check(ImGui::GetItemRectMax().x<=right+.5f,"Default button fits at every font/window width");
        const auto lo=ImGui::GetItemRectMin(),hi=ImGui::GetItemRectMax();result.click={(lo.x+hi.x)/2,(lo.y+hi.y)/2};
        const char* choices[]={"Native","Alternative"};
        dg::overlay::layout::Choice("Selection##test",&choice,choices,2);
        Check(ImGui::GetItemRectMax().x<=right+.5f,"choice control fits at every width");
        dg::overlay::layout::HelpText("A concise first sentence. A longer explanation remains in hover help without adding another permanent paragraph.");
        ImGui::End();
    }
    const auto& after=ImGui::GetStyle();
    Check(after.WindowPadding.x==before.WindowPadding.x&&after.FramePadding.y==before.FramePadding.y&&after.ItemSpacing.y==before.ItemSpacing.y&&after.ItemInnerSpacing.x==before.ItemInnerSpacing.x&&after.IndentSpacing==before.IndentSpacing,"menu spacing scope restores the surrounding ImGui style");
    ImGui::Render();return result;
}
void Run(){
    using dg::overlay::layout::Summary;
    Check(Summary("0.05 seconds of ground contact. More details.")=="0.05 seconds of ground contact.","decimal values remain intact in short help");
    Check(Summary("Short help")=="Short help","short explanation retained");
    ImGui::CreateContext();auto& io=ImGui::GetIO();io.IniFilename=nullptr;io.DisplaySize={1800,1400};io.DeltaTime=1.f/60;
    unsigned char* pixels=nullptr;int w=0,h=0;io.Fonts->GetTexDataAsRGBA32(&pixels,&w,&h);
    for(float font:{10.f,13.f,20.f,32.f})for(float width:{240.f,480.f,990.f}){
        float value=2.75f;int choice=1;io.AddMousePosEvent(-100,-100);
        auto result=Frame(width,font,value,choice);Check(value==2.75f&&choice==1&&!result.changed,"layout-only frame does not alter saved values");
        io.AddMousePosEvent(result.click.x,result.click.y);io.AddMouseButtonEvent(0,true);Frame(width,font,value,choice);
        io.AddMouseButtonEvent(0,false);result=Frame(width,font,value,choice);
        Check(result.changed&&value==1.f&&choice==1,"Default remains clickable on one line and wrapped layouts");
    }
    ImGui::DestroyContext();
}
}
int main(){try{Run();std::cout<<"Actual ImGui number/choice bounds, Default interaction and style restoration passed across 12 font/width combinations\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
