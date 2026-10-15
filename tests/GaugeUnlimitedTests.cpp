#include "GaugeUnlimitedPolicy.h"
#include <cstdio>
#include <limits>
int main(){
    using namespace dg::manual_gauges;
    unsigned failed=0;
    const auto check=[&](bool pass,const char* label){if(!pass){++failed;std::printf("FAIL %s\n",label);}};
    UnlimitedContext c{};c.actor=0x100;c.holders[0]=0x200;c.holders[1]=0x200;c.holders[2]=0x300;
    c.generation=7;c.observedAt=1000;c.readyMask=All;
    check(UnlimitedContextMatches(c,Wrath,0x200,0x100,7,1100),"live local Wrath accepted");
    check(UnlimitedContextMatches(c,Ultimate,0x200,0x100,7,1499),"live Ultimate accepted at 499ms");
    check(UnlimitedContextMatches(c,Synergy,0x300,0x100,7,1100),"typed local Synergy identity accepted");
    check(!UnlimitedContextMatches(c,All,0x200,0x100,7,1100),"combined mask cannot target one native setter");
    check(!UnlimitedContextMatches(c,0,0x200,0x100,7,1100),"zero gauge rejected");
    check(!UnlimitedContextMatches(c,Wrath,0x300,0x100,7,1100),"foreign holder rejected");
    check(!UnlimitedContextMatches(c,Wrath,0x200,0x101,7,1100),"player change rejected");
    check(!UnlimitedContextMatches(c,Wrath,0x200,0x100,8,1100),"generation change rejected");
    check(!UnlimitedContextMatches(c,Wrath,0x200,0x100,7,1500),"expired context rejected");
    check(!UnlimitedContextMatches(c,Wrath,0x200,0x100,7,999),"clock before observation rejected");
    c.readyMask=Synergy;
    check(!UnlimitedContextMatches(c,Wrath,0x200,0x100,7,1100),"unavailable native setter rejected");
    check(UnlimitedValue(false,13,20)==13,"OFF preserves requested value");
    check(UnlimitedValue(false,-1,20)==-1,"OFF preserves native reset");
    check(UnlimitedValue(true,0,20)==20,"enabled drain held at native cap");
    check(UnlimitedValue(true,500,20)==20,"never exceeds native cap");
    check(UnlimitedValue(true,7,0)==7,"locked or zero cap left native");
    check(UnlimitedValue(true,7,-1)==7,"invalid cap left native");
    const float nan=std::numeric_limits<float>::quiet_NaN();
    check(std::isnan(UnlimitedValue(true,nan,20)),"invalid native request left native");
    check(UnlimitedValue(true,7,nan)==7,"nonfinite cap left native");
    std::printf("Gauge Unlimited: %u failures\n",failed);return failed?1:0;
}
