#include "ResourceGainPolicy.h"
#include <cmath>
#include <cstdio>
#include <limits>
int main(){
    using namespace dg::resource_gain;
    static_assert(RewardCaller(0,0x6EA9C9)&&RewardCaller(0,0x6EAB2E));
    static_assert(!RewardCaller(0,0x65C080)); // consumable potion is a separate path
    static_assert(RewardCaller(1,0x67EB54)&&RewardCaller(1,0x67EB8C));
    static_assert(!RewardCaller(0,0x6B9656)); // max-Wrath upgrade
    static_assert(!RewardCaller(1,0x64B8C7)); // form drain
    static_assert(!RewardCaller(0,0)&&!RewardCaller(1,0)); // mod/manual calls
    unsigned failed=0;
    const auto check=[&](bool pass,const char* name){if(!pass){++failed;std::printf("FAIL %s\n",name);}};
    check(RewardRequest(10,12,20,true,2)==14,"multiply gain only, not whole gauge");
    check(RewardRequest(10,12,20,false,4)==12,"disabled native passthrough");
    check(RewardRequest(10,12,20,true,1)==12,"x1 unchanged");
    check(RewardRequest(10,12,20,true,0)==10,"x0 stops positive reward");
    check(RewardRequest(10,7,20,true,100)==7,"spend unchanged");
    check(RewardRequest(10,10,20,true,4)==10,"equal set unchanged");
    check(RewardRequest(18,22,20,true,0.5f)==20,"scale unclamped native gain before cap");
    check(RewardRequest(10,12,20,true,100)==20,"native maximum retained");
    check(RewardRequest(10,12,0,true,2)==12,"invalid cap native passthrough");
    check(RewardRequest(-1,12,20,true,2)==12,"invalid prior native passthrough");
    check(RewardRequest(10,12,20,true,-1)==12,"invalid multiplier native passthrough");
    const float nan=std::numeric_limits<float>::quiet_NaN();
    check(RewardRequest(10,12,20,true,nan)==12,"NaN multiplier rejected");
    check(std::isnan(RewardRequest(10,nan,20,true,2)),"NaN request preserved for native handling");
    check(RewardRequest(10,std::numeric_limits<float>::max(),20,true,100)==20,"large finite reward cannot overflow");
    std::printf("Native resource rewards: %u failures\n",failed);return failed?1:0;
}
