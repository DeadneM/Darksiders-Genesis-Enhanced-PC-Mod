#pragma once
#include <cmath>
namespace dg::potions {
inline float RechargeThreshold(float native,bool enabled,float multiplier){
    if(!enabled||!std::isfinite(native)||native<=0||
       !std::isfinite(multiplier)||multiplier<1||multiplier>100)return native;
    return native/multiplier;
}
inline int ConsumeAmount(int native,bool unlimited){
    return unlimited&&native>0?0:native;
}
inline int RefillAmount(int current,int maximum){
    return maximum>0&&maximum<=100&&current>=0&&current<maximum?maximum-current:0;
}
}
