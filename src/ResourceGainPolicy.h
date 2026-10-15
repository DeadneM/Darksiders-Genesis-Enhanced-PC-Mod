#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
namespace dg::resource_gain {
// Only sites proved to add a native reward to the current gauge. Manual
// setters, spends, initialization, upgrades and generic absolute setters
// are intentionally absent. Returns are the instruction AFTER each call.
constexpr bool RewardCaller(unsigned gauge,uintptr_t rva){
    return gauge==0?(rva==0x6EA9C9||rva==0x6EAB2E):
        gauge==1&&(rva==0x67EB54||rva==0x67EB8C);
}
inline float RewardRequest(float before,float requested,float maximum,
                          bool enabled,float multiplier){
    if(!enabled||!std::isfinite(before)||!std::isfinite(requested)||
       !std::isfinite(maximum)||!std::isfinite(multiplier)||
       maximum<=0||maximum>100000||before<0||before>maximum||
       multiplier<0||multiplier>100)return requested;
    if(requested<=before||multiplier==1.0f)return requested;
    const double target=double(before)+(double(requested)-before)*multiplier;
    return static_cast<float>(std::min(double(maximum),target));
}
}
