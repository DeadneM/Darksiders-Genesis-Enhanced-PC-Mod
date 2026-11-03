#include "AimShaderStatePolicy.h"
#include <iostream>
using dg::aim_shader::PixelState;
int main(){
    unsigned queries=0,updates=0,failures=0;
    auto state=PixelState::Other;bool actualIsAim=false;
    const auto draw=[&](bool enabled){return dg::aim_shader::ShouldSkip(state,enabled,[&]{++queries;return actualIsAim;},[&](auto next){state=next;++updates;});};
    const auto check=[&](bool value,const char* label){if(!value){++failures;std::cerr<<label<<'\n';}};
    check(!draw(true)&&queries==0,"ordinary known shader keeps the no-query fast path");
    // Native command-list/state restoration changes the real shader without
    // an observed PSSetShader bind. Invalidate then classify the first draw.
    actualIsAim=true;state=PixelState::Unknown;
    check(draw(true)&&queries==1&&state==PixelState::Aim&&updates==1,"unobserved command-list shader change cannot bypass hiding");
    check(draw(true)&&queries==2,"aim candidate still validates its real shader");
    actualIsAim=false;
    check(!draw(true)&&queries==3&&state==PixelState::Other,"stale positive state never hides an unrelated draw");
    check(!draw(true)&&queries==3,"ordinary follow-up draws avoid repeated native queries");
    actualIsAim=true;state=PixelState::Unknown;
    check(!draw(false)&&queries==3&&state==PixelState::Unknown,"disabled mode never blocks or queries a draw");
    check(draw(true)&&queries==4,"re-enabled mode classifies pending unknown state");
    state=PixelState::Other;actualIsAim=false; // ClearState restores default null PS.
    check(!draw(true)&&queries==4,"cleared context cannot retain the old aim match");
    std::cout<<"Aim shader context invalidation: "<<failures<<" failures\n";return failures?1:0;
}
