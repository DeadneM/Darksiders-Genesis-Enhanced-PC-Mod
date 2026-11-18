#pragma once
namespace dg::core_ui {
struct Warmup {
    unsigned ticks=0;
    bool completed=false;
    bool Next(){
        if(completed||++ticks<2)return false;
        completed=true; // Consume before native dispatch, including nested events.
        return true;
    }
};
}
