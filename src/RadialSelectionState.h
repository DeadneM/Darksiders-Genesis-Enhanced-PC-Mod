#pragma once
#include <cstdint>
namespace dg::native_tps {
// EHudWidgetState in the supported executable: SHOWN=0, SHOWING=1,
// HIDING=2, HIDDEN=3. Construction defaults to SHOWN, so visibility alone
// must NEVER arm a selector. Only an actual native show request can arm it.
struct RadialSelectionState {
    bool requested=false;
    void Request(unsigned state){requested=state<=1;}
    bool Selecting(unsigned state,bool visible) const {
        return requested&&state<=1&&visible;
    }
};
}
