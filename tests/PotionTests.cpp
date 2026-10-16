#include "PotionPolicy.h"
#include <cstdio>
#include <initializer_list>
#include <limits>
int main(){
    using namespace dg::potions;
    unsigned failed=0;auto check=[&](bool x,const char* s){if(!x){++failed;std::printf("FAIL %s\n",s);}};
    check(RechargeThreshold(250,false,10)==250,"OFF retains native conversion threshold");
    check(RechargeThreshold(250,true,1)==250,"x1 is native");
    check(RechargeThreshold(250,true,2)==125&&RechargeThreshold(250,true,10)==25,"same overflow grants potions twice or ten times as fast");
    check(RechargeThreshold(250,true,100)==2.5f,"maximum permitted rate remains finite and positive");
    for(float n:{0.0f,-1.0f,101.0f,std::numeric_limits<float>::infinity(),std::numeric_limits<float>::quiet_NaN()})
        check(RechargeThreshold(250,true,n)==250,"invalid multipliers preserve native threshold");
    check(RechargeThreshold(0,true,2)==0&&RechargeThreshold(-1,true,2)==-1,"invalid native threshold is not replaced");
    check(ConsumeAmount(1,false)==1&&ConsumeAmount(3,false)==3,"OFF spends all native charges");
    check(ConsumeAmount(1,true)==0&&ConsumeAmount(3,true)==0,"Unlimited blocks native positive charge costs");
    check(ConsumeAmount(0,true)==0&&ConsumeAmount(-2,true)==-2,"Unlimited does not turn zero or negative requests into rewards");
    check(RefillAmount(0,4)==4&&RefillAmount(3,4)==1&&RefillAmount(4,4)==0,"refill requests exactly the missing native charges");
    check(RefillAmount(4,5)==1,"newly earned native capacity remains authoritative");
    check(RefillAmount(-1,4)==0&&RefillAmount(0,0)==0&&RefillAmount(0,101)==0&&RefillAmount(6,4)==0,"invalid or overfull reserves are never lowered or filled");
    std::printf("Potion policy: %u failures\n",failed);return failed?1:0;
}
