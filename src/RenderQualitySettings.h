#pragma once
#include <array>
#include <cstddef>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <optional>

namespace dg::render_quality {
struct Spec{const char* key;const char* label;float minimum,maximum,trial;bool integer;};
// Limits are bounded test ranges, not hardware maxima.
inline constexpr size_t ResolutionIndex=1, ReflectionsIndex=6;
inline constexpr bool IsLiveOption(size_t index){return index==ResolutionIndex||index==ReflectionsIndex;}
inline constexpr std::array<Spec,7> Specs={{
 {"MaxAnisotropy","Anisotropic filtering",1,16,16,true},
 {"ScreenPercentage","Internal resolution (%)",25,200,100,false},
 {"ShadowQuality","Shadow quality (0 = off)",0,5,5,true},
 {"ShadowMaxResolution","Maximum shadow resolution",64,8192,2048,true},
 {"ShadowDistanceScale","Shadow distance multiplier",0,5,1,false},
 {"ViewDistanceScale","View distance multiplier",0,5,1,false},
 {"SSRQuality","Reflection quality (0 = off)",0,4,3,true}
}};
using Values=std::array<float,Specs.size()>;
inline constexpr Values NativeValues(){Values v{};for(auto& x:v)x=-1;return v;}
inline float Normalize(size_t i,float v){if(i>=Specs.size()||!std::isfinite(v)||v<0)return -1;
 const auto& s=Specs[i];v=std::clamp(v,s.minimum,s.maximum);return s.integer?std::round(v):v;}

struct ConsoleValue {
    int32_t bits=0;
    uint32_t priority=0;
    bool operator==(const ConsoleValue&) const = default;
};

// Compare both the value and who set it before restoring a live override.
class LiveOverride {
    bool owned=false;
    ConsoleValue original{},written{};
public:
    template<class Write>
    void Update(ConsoleValue current,std::optional<int32_t> desired,Write write){
        if(owned&&current!=written)owned=false;
        if(!desired){
            if(owned&&write(original))owned=false;
            return;
        }
        if(current.bits==*desired)return;
        const ConsoleValue next{*desired,current.priority};
        if(!write(next))return;
        if(!owned)original=current;
        written=next;
        owned=true;
    }
};
}
