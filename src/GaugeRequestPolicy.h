#pragma once
#include <cmath>
#include <cstdint>

namespace dg::manual_gauges {
enum Mask : unsigned { Wrath=1, Ultimate=2, Synergy=4, All=7 };
struct Request {
    unsigned mask=0;
    float percent=100;
    uintptr_t actor=0;
    uint32_t generation=0;
    uint64_t created=0;
};
enum class RequestValidity { Accept, WrongActor, Expired };
inline RequestValidity ValidateRequest(const Request& r,uintptr_t actor,
                                       uint32_t generation,uint64_t now){
    if(!r.actor||r.actor!=actor||r.generation!=generation)
        return RequestValidity::WrongActor;
    if(now<r.created||now-r.created>=30000)
        return RequestValidity::Expired;
    return RequestValidity::Accept;
}
inline bool TargetValue(float maximum,float percent,float& target){
    if(!std::isfinite(maximum)||maximum<=0||maximum>100000||
       !std::isfinite(percent)||percent<0||percent>100)return false;
    target=maximum*(percent*0.01f);
    return std::isfinite(target)&&target>=0&&target<=maximum;
}
}
