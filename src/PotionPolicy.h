#pragma once
#include <cmath>
#include <limits>
namespace dg::potions {
inline float RechargeThreshold(float native,bool enabled,float multiplier){
    if(!enabled||!std::isfinite(native)||native<=0||
       !std::isfinite(multiplier)||multiplier<=0||multiplier>100)return native;
    const double threshold=double(native)/multiplier;
    return threshold<=std::numeric_limits<float>::max()?static_cast<float>(threshold):native;
}
inline int ConsumeAmount(int native,bool unlimited){
    return unlimited&&native>0?0:native;
}
inline int RefillAmount(int current,int maximum){
    return maximum>0&&maximum<=100&&current>=0&&current<maximum?maximum-current:0;
}
}
