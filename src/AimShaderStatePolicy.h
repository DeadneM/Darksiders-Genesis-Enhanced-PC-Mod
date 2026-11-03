#pragma once
namespace dg::aim_shader {
enum class PixelState {Unknown,Other,Aim};
// Only an observed shader bind or native reset may establish a cached state.
// Command-list replay can bypass PSSetShader: the next draw must query once.
template<class Actual,class Remember>
bool ShouldSkip(PixelState state,bool enabled,Actual actual,Remember remember){
    if(!enabled||state==PixelState::Other)return false;
    const bool matched=actual();
    if(state==PixelState::Unknown||!matched)remember(matched?PixelState::Aim:PixelState::Other);
    return matched;
}
}
