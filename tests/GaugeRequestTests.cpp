#include "GaugeRequestPolicy.h"
#include <cstdio>
#include <initializer_list>
#include <limits>

int main(){
    using namespace dg::manual_gauges;
    unsigned failures=0,checks=0;
    const auto check=[&](bool pass,const char* label){
        ++checks;if(!pass){++failures;std::printf("FAIL %s\n",label);}
    };
    float target=-1;
    for(float cap:{12.0f,100.0f,1000.0f,100000.0f}){
        for(float percent:{0.0f,25.0f,50.0f,100.0f}){
            check(TargetValue(cap,percent,target),"valid percentage accepted");
            check(target>=0&&target<=cap,"target bounded by native cap");
            check(target==cap*(percent*0.01f),"percentage maps to native units");
        }
    }
    const float nan=std::numeric_limits<float>::quiet_NaN();
    const float inf=std::numeric_limits<float>::infinity();
    for(float cap:{0.0f,-1.0f,100001.0f,nan,inf})check(!TargetValue(cap,100,target),"invalid cap rejected");
    for(float percent:{-1.0f,101.0f,nan,inf})check(!TargetValue(100,percent,target),"invalid percentage rejected");
    const Request request{All,100,0x1234,17,1000};
    check(ValidateRequest(request,0x1234,17,1000)==RequestValidity::Accept,"fresh request accepted");
    check(ValidateRequest(request,0x1234,17,30999)==RequestValidity::Accept,"request before expiry accepted");
    check(ValidateRequest(request,0x1234,17,31000)==RequestValidity::Expired,"request expires at 30 seconds");
    check(ValidateRequest(request,0x1234,17,999)==RequestValidity::Expired,"clock rollback rejected");
    check(ValidateRequest(request,0x4321,17,1001)==RequestValidity::WrongActor,"pawn switch cancels request");
    check(ValidateRequest(request,0x1234,18,1001)==RequestValidity::WrongActor,"new generation cancels recycled identity");
    check(ValidateRequest(request,0,17,1001)==RequestValidity::WrongActor,"missing owner rejected");
    std::printf("Gauge requests: %u checks, %u failures\n",checks,failures);
    return failures?1:0;
}
