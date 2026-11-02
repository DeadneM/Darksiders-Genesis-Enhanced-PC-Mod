#pragma once
#include <array>
#include <atomic>
#include <cmath>
#include <algorithm>
namespace dg::effect_transform {
// Attachment-space offsets, in UE units. Separate from gameplay and native assets.
struct Values {float size=1,x=0,y=0,z=0;};
using Profiles=std::array<Values,2>; // Strife, War; independent of local player number.
inline float Finite(float value,float fallback,float lo,float hi){return std::isfinite(value)?std::clamp(value,lo,hi):fallback;}
inline Values Normalize(Values v){return {Finite(v.size,1,.05f,5),Finite(v.x,0,-500,500),Finite(v.y,0,-500,500),Finite(v.z,0,-500,500)};}
struct AtomicValues {
    std::atomic<float> size{1},x{0},y{0},z{0};
    void Store(Values v){v=Normalize(v);size.store(v.size);x.store(v.x);y.store(v.y);z.store(v.z);}
    Values Load()const{return Normalize({size.load(),x.load(),y.load(),z.load()});}
};
inline constexpr const wchar_t* Keys[2][2][4]={
    {{L"ShieldStrifeSize",L"ShieldStrifeX",L"ShieldStrifeY",L"ShieldStrifeZ"},{L"ShieldWarSize",L"ShieldWarX",L"ShieldWarY",L"ShieldWarZ"}},
    {{L"LeviathanStrifeSize",L"LeviathanStrifeX",L"LeviathanStrifeY",L"LeviathanStrifeZ"},{L"LeviathanWarSize",L"LeviathanWarX",L"LeviathanWarY",L"LeviathanWarZ"}}
};
}
