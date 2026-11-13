#pragma once
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdint>

namespace dg::temporal_aa {
struct Settings {
    bool enabled=false;
    float frameWeight=0.08f,sharpen=1.0f;
};
inline Settings Normalize(Settings value){
    value.frameWeight=std::isfinite(value.frameWeight)?std::clamp(value.frameWeight,0.01f,1.0f):0.08f;
    value.sharpen=std::isfinite(value.sharpen)?std::clamp(value.sharpen,0.0f,2.0f):1.0f;
    return value;
}

// Float CVar words are exchanged as aligned 32-bit bit patterns. A restore
// only touches our last write; an intervening game/console change wins.
// One state per registered allocation; never follow an old allocation.
class ShadowOverride {
    bool owned_[2]{};
    int32_t original_[2]{},written_[2]{};
public:
    void Forget(){*this=ShadowOverride{};}
    template<class Access>
    void Update(Access& access,bool enabled,float desired,float minimum,float maximum){
        const auto bits=std::bit_cast<int32_t>(desired);
        for(unsigned i=0;i<2;++i){
            const int32_t current=access.Read(i);
            if(!enabled){
                if(owned_[i])access.ExchangeIf(i,original_[i],written_[i]);
                owned_[i]=false;
                continue;
            }
            if(owned_[i]&&current!=written_[i])owned_[i]=false;
            const float native=std::bit_cast<float>(current);
            if(!std::isfinite(desired)||desired<minimum||desired>maximum||
               !std::isfinite(native)||native<minimum||native>maximum||current==bits)continue;
            const int32_t before=owned_[i]?original_[i]:current;
            if(access.ExchangeIf(i,bits,current)==current){
                original_[i]=before;written_[i]=bits;owned_[i]=true;
            }
        }
    }
};
}
