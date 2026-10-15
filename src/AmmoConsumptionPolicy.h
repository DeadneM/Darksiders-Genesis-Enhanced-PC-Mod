#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
namespace dg::ammo {
struct Credit {
    uintptr_t actor=0,holder=0;
    uint32_t generation=0;
    float divisor=0;
    bool unlimited=false;
    double remainder=0;
};
inline int ConsumptionRequest(Credit& credit,uintptr_t actor,uintptr_t holder,
    uint32_t generation,int before,int requested,bool reduce,float divisor,bool unlimited){
    const float factor=reduce&&std::isfinite(divisor)?std::clamp(divisor,0.0f,10.0f):0.0f;
    if(credit.actor!=actor||credit.holder!=holder||credit.generation!=generation||
       credit.divisor!=factor||credit.unlimited!=unlimited){
        credit={actor,holder,generation,factor,unlimited,0};
    }
    // Preserve the game's -1 unlimited sentinel and any initialization/reset.
    // Only a finite, positive spend is eligible; gains are untouched.
    if(before<0||requested<0||requested>before||before>1000000){
        credit.remainder=0;return requested;
    }
    if(requested==before)return requested; // duplicate writes must not erase fractions
    if(unlimited){credit.remainder=0;return before;}
    if(factor<=1.0f){credit.remainder=0;return requested;}
    const double debt=credit.remainder+double(before-requested)/factor;
    const int spend=static_cast<int>(std::floor(debt+1e-8));
    credit.remainder=std::clamp(debt-spend,0.0,0.999999999);
    return before-std::clamp(spend,0,before-requested);
}
}
