#include "AirDashGesture.h"
#include <iostream>
int main(){using namespace dg::air_dash;unsigned failed=0;
    auto check=[&](bool ok,const char* why){if(!ok){++failed;std::cerr<<why<<'\n';}};
    Press p;
    check(p.Begin(1000,false)==Gesture::Native&&!p.down,"ground/horse/unknown actions keep immediate native dispatch");
    check(p.Begin(1000,true)==Gesture::Swallow,"air press waits for duration classification");
    check(p.Update(1179,true)==Gesture::Swallow,"hold threshold has no early forward action");
    check(p.Release(1179,true)==Gesture::Down,"short press selects downward once, regardless of movement axes");
    check(p.Release(1180,true)==Gesture::Native,"duplicate release cannot create another dash");
    p={};p.Begin(1000,true);
    check(p.Update(1180,true)==Gesture::Forward,"exact threshold commits the forward dash without waiting for release");
    check(p.Update(1300,true)==Gesture::Native&&p.Begin(1300,true)==Gesture::Swallow,"a held or repeated press never repeats the action");
    check(p.Release(1500,true)==Gesture::Native,"release after forward cannot add a stomp");
    p={};p.Begin(1000,true);
    check(p.Release(1180,true)==Gesture::Forward,"slow frame before threshold service still classifies a long release correctly");
    for(bool focusLoss:{false,true}){p={};p.Begin(1000,true);p.Update(1100,false);
        check(p.Release(1300,!focusLoss)==Gesture::Native,"cancelled menu/focus/landing press cannot create a delayed action");
        check(p.Begin(1400,true)==Gesture::Swallow&&p.Release(1450,true)==Gesture::Down,"first new press after focus loss works even if the old release was lost");}
    p={};p.Begin(1000,true);check(p.Release(999,true)==Gesture::Swallow,"invalid clock order cancels the queued action");
    Press other;p={};p.Begin(1000,true);other.Begin(1100,true);
    check(p.Update(1180,true)==Gesture::Forward&&other.Update(1180,true)==Gesture::Swallow,"players never share their press duration");
    std::cout<<"Air dash gesture: "<<failed<<" failures\n";return failed?1:0;
}
