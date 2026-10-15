#include "AmmoConsumptionPolicy.h"
#include <cstdio>
#include <limits>
int main(){
    using namespace dg::ammo;
    unsigned failed=0;
    auto check=[&](bool value,const char* label){if(!value){++failed;std::printf("FAIL %s\n",label);}};
    Credit credit{};
    auto spend=[&](int before,int requested,bool reduce=true,float factor=10,bool unlimited=false){
        return ConsumptionRequest(credit,1,2,3,before,requested,reduce,factor,unlimited);
    };
    check(spend(100,99,false)==99,"OFF native spend");
    check(spend(100,99,true,0)==99&&spend(100,99,true,1)==99,"zero and one native spend");
    int count=100;
    for(int i=0;i<10;++i){count=spend(count,count-1);check(spend(count,count)==count,"duplicate write retained");}
    check(count==99,"division by ten spends one after ten single costs");
    credit={};count=100;
    for(int i=0;i<21;++i)count=spend(count,count-1,true,2);
    check(count==90,"half consumption accumulates odd remainder");
    check(spend(count,count-1,true,2)==89,"remaining half is charged next time");
    credit={};check(spend(100,70,true,10)==97,"burst cost scaled");
    check(spend(100,90,true,10,true)==100,"Unlimited stops finite spend");
    check(spend(100,90,true,10,false)==99,"Unlimited OFF resumes chosen reduction");
    check(spend(100,90,false,10,false)==90,"both OFF resumes full native spend");
    check(spend(-1,-1,true,10,true)==-1,"native infinite sentinel unchanged");
    check(spend(100,-1,true,10,true)==-1,"native sentinel assignment unchanged");
    check(spend(10,30,true,10,true)==30,"pickup gain unchanged");
    check(spend(100,90,true,std::numeric_limits<float>::quiet_NaN())==90,"invalid factor native spend");
    credit={};spend(100,99);
    check(ConsumptionRequest(credit,4,2,3,100,99,true,10,false)==100&&credit.remainder<0.11,"actor change clears debt");
    for(int i=0;i<8;++i)ConsumptionRequest(credit,4,2,3,100,99,true,10,false);
    check(ConsumptionRequest(credit,4,2,4,100,99,true,10,false)==100,"generation change clears debt");
    check(ConsumptionRequest(credit,4,5,4,100,99,true,10,false)==100&&credit.remainder<0.11,"weapon change clears debt");
    credit={};count=100;
    for(int i=0;i<15;++i)count=spend(count,count-1,true,2.5f);
    check(count==94,"fractional divisor accumulates correct total");
    check(spend(0,0,true,10,true)==0,"zero duplicate is not a gain");
    std::printf("Ammo consumption: %u failures\n",failed);return failed?1:0;
}
