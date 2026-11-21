#pragma once
#include <atomic>
#include <cmath>
#include <cstring>

namespace dg::chromatic {
// Exact FPostProcessSettings metadata + native bOverride setter, checked once.
inline std::atomic_bool layoutReady{false};
inline bool Ready(){return layoutReady.load(std::memory_order_acquire);}
inline bool Apply(void* output,float amount){
    if(!Ready()||!output||!std::isfinite(amount)||amount<0||amount>10)return false;
    auto* bytes=static_cast<unsigned char*>(output);float weight=0;
    std::memcpy(&weight,bytes+0x38,sizeof(weight));
    // Do not raise the weight of unrelated camera postprocess overrides.
    if(weight!=1.0f)return false;
    float authored=0;std::memcpy(&authored,bytes+0x40+0x210,sizeof(authored));
    if(!std::isfinite(authored)||authored<0||authored>10)return false;
    std::memcpy(bytes+0x40+0x210,&amount,sizeof(amount));
    bytes[0x40+5]|=0x10;
    return true;
}
}
