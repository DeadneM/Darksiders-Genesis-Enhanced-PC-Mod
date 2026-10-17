#pragma once
#include <array>
namespace dg::tps_input {
// Window thread only. Remember the DOWN destination across mode/menu/rebind
// transitions so the native action ALWAYS receives its matching release.
struct MouseFireRouting {
    std::array<int,7> held{}; // five Win32 mouse VKs, zero=unowned, -1=suppressed
    int Begin(int physical,int destination){
        if(physical<=0||physical>=int(held.size()))return 0;
        if(!held[physical])held[physical]=destination;
        return held[physical];
    }
    int Release(int physical){
        if(physical<=0||physical>=int(held.size()))return 0;
        const int destination=held[physical];held[physical]=0;return destination;
    }
};
}
