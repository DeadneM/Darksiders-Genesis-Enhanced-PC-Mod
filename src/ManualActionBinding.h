#pragma once
#include <cstdint>

namespace dg::manual_binding {
// OFF is an absent part of a chord, except that two OFF parts disable it.
constexpr bool Down(int first,int second,bool firstDown,bool secondDown){
    return (first||second)&&(!first||firstDown)&&(!second||secondDown);
}
struct Hold {
    int first=-1,second=-1;
    uint64_t owner=0;
    bool initialized=false,blocked=false;
    bool Observe(int a,int b,bool down,bool allowed,uint64_t identity=0){
        if(!initialized||first!=a||second!=b||owner!=identity){
            first=a;second=b;owner=identity;initialized=true;blocked=down;
        }
        if(!allowed&&down)blocked=true;
        if(!down)blocked=false;
        return allowed&&down&&!blocked;
    }
    void Reset(){initialized=false;}
};
struct Edge {
    int first=-1,second=-1;
    uint64_t owner=0;
    bool initialized=false,held=false;
    bool Observe(int a,int b,bool down,bool allowed,uint64_t identity=0){
        // Rebinding/reconnecting while held must not create an action.
        if(!initialized||first!=a||second!=b||owner!=identity){
            first=a;second=b;owner=identity;held=down;initialized=true;return false;
        }
        const bool pressed=down&&!held;held=down;
        return allowed&&pressed;
    }
    void Reset(){initialized=false;}
};
}
