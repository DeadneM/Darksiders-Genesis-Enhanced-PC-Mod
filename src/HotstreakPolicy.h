#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
namespace dg::hotstreak {
// Explicit native resets are deliberately excluded from this caller set.
constexpr bool DepletionCaller(uintptr_t caller){
    return caller==0x68B007||caller==0x68BA89||caller==0x67E81E||caller==0x793580;
}
inline float DepletionAmount(float amount,bool reduce,float divisor,bool unlimited){
    if(!std::isfinite(amount)||amount<=0||amount>1000000)return amount;
    if(unlimited)return 0;
    if(!reduce||!std::isfinite(divisor))return amount;
    const float factor=std::clamp(divisor,0.0f,10.0f);
    return factor>1?amount/factor:amount;
}
}
