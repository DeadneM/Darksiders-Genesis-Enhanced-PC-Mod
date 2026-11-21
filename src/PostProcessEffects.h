#pragma once
#include <array>
#include <atomic>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstddef>
#include <cstring>

namespace dg::postprocess {
struct Spec {
    const char* key;const char* group;const char* label;float maximum,trial;
    uint32_t offset,flag,mask,metadata,boolMetadata,setter,name,boolName;
};
// Maxima are explicit exploratory limits, not claims of an engine-wide maximum.
inline constexpr std::array<Spec,17> Specs={{
    {"BloomIntensity","Bloom","Bloom intensity",10.0f,1.0f,0x218,0x6,0x2,0x3796548,0x3798a00,0x1dd2c30,0x2eb7c80,0x2eb8fc8},
    {"BloomDirtMaskIntensity","Bloom","Dirt mask intensity",10.0f,1.0f,0x2c8,0x9,0x1,0x3796168,0x37984f8,0x1dd2c00,0x2eb7ab8,0x2eb8d08},
    {"LensFlareIntensity","Lens flares","Lens flare intensity",10.0f,1.0f,0x33c,0xb,0x2,0x3795e00,0x3798140,0x1dd30d0,0x2eb7918,0x2eb8b20},
    {"LensFlareThreshold","Lens flares","Brightness threshold",100.0f,8.0f,0x354,0xb,0x40,0x3795d80,0x3798028,0x1dd30e0,0x2eb78d8,0x2eb8a88},
    {"VignetteIntensity","Grain and vignette","Vignette intensity",5.0f,0.4f,0x3e0,0xb,0x80,0x3795cf8,0x3797ff0,0x1dd31b0,0x265bda0,0x265bec8},
    {"GrainIntensity","Grain and vignette","Film grain intensity",5.0f,0.1f,0x3e8,0xc,0x1,0x3795ca8,0x3797fb8,0x1ce37a0,0x2eb7890,0x2eb8a68},
    {"GrainJitter","Grain and vignette","Grain jitter",5.0f,0.1f,0x3e4,0xc,0x2,0x3795cd0,0x3797f80,0x1ce36e0,0x2eb78a0,0x2eb8a50},
    {"AmbientOcclusionIntensity","Ambient occlusion","Occlusion intensity",10.0f,1.0f,0x3ec,0xc,0x4,0x3795c80,0x3797f48,0x1d07dd0,0x2eb7870,0x2eb8a28},
    {"AmbientOcclusionRadius","Ambient occlusion","Occlusion radius",5000.0f,200.0f,0x3f4,0xc,0x10,0x3795c30,0x3797ed8,0x1ce36f0,0x2eb7838,0x2eb89d0},
    {"AmbientOcclusionPower","Ambient occlusion","Occlusion power",10.0f,2.0f,0x408,0xd,0x2,0x3795b58,0x3797dc0,0x1ce3790,0x2eb77a0,0x2eb8910},
    {"ScreenSpaceReflectionIntensity","Reflections","Reflection intensity (%)",100.0f,100.0f,0x4dc,0x19,0x20,0x3795378,0x3797260,0x1dd3170,0x2eb7368,0x2eb8378},
    {"ScreenSpaceReflectionQuality","Reflections","Reflection quality (%)",100.0f,50.0f,0x4e0,0x19,0x40,0x3795350,0x3797228,0x1dd3190,0x2eb7348,0x2eb8350},
    {"ScreenSpaceReflectionMaxRoughness","Reflections","Roughness limit",1.0f,0.8f,0x4e4,0x19,0x80,0x3795328,0x37971f0,0x1dd3180,0x2eb7310,0x2eb8320},
    {"AutoExposureBias","Exposure and color","Exposure compensation (EV)",10.0f,0.0f,0x30c,0xa,0x20,0x3795fc0,0x3798220,0x1dd2a70,0x2eb7a28,0x2eb8ba0},
    {"AmbientCubemapIntensity","Exposure and color","Native cubemap intensity",10.0f,1.0f,0x2ec,0x5,0x80,0x37960e0,0x3798a70,0x1dd2a40,0x2eb7a70,0x2eb9000},
    {"ColorGradingIntensity","Exposure and color","Native color LUT strength",1.0f,1.0f,0x444,0x16,0x2,0x3795960,0x3797880,0x1dd2d10,0x2eb7678,0x2eb8778},
    {"MotionBlurMax","Motion blur maximum","Maximum blur (% of screen)",100.0f,5.0f,0x49c,0x19,0x4,0x3795630,0x3797308,0x1dd3120,0x2eb73b0,0x2eb83f0},
}};
inline constexpr size_t Count=Specs.size();
inline constexpr size_t MotionMaximumIndex=16;
static_assert(Specs[MotionMaximumIndex].offset==0x49C);
static_assert(Count<64);
using Values=std::array<float,Count>;
inline constexpr Values NativeValues(){Values v{};for(auto& x:v)x=-1;return v;}
inline float Normalize(size_t i,float value){return i<Count&&std::isfinite(value)&&value>=0?std::min(value,Specs[i].maximum):-1.0f;}
inline std::atomic_uint64_t verifiedMask{0};
inline bool Ready(size_t i){return i<Count&&(verifiedMask.load(std::memory_order_acquire)&(uint64_t{1}<<i));}
struct ImageReader {
    const unsigned char* base;
    template<class T>T Read(uintptr_t rva)const{T v{};std::memcpy(&v,base+rva,sizeof(v));return v;}
};
inline bool Verify(const unsigned char* base,const Spec& s){
    if(!base||s.offset+4>0x530||s.flag>=0x20||!s.mask||s.mask>0x80||(s.mask&(s.mask-1)))return false;
    ImageReader r{base};const auto address=reinterpret_cast<uintptr_t>(base);
    const unsigned char setter[]={0x80,0x49,static_cast<unsigned char>(s.flag),static_cast<unsigned char>(s.mask),0xC3};
    return r.Read<uintptr_t>(s.metadata)==address+s.name&&r.Read<uint32_t>(s.metadata+24)==0xA&&
        r.Read<uint32_t>(s.metadata+28)==0x45&&r.Read<uint32_t>(s.metadata+32)==1&&r.Read<uint32_t>(s.metadata+36)==s.offset&&
        r.Read<uintptr_t>(s.boolMetadata)==address+s.boolName&&r.Read<uint32_t>(s.boolMetadata+24)==0xC&&
        r.Read<uint32_t>(s.boolMetadata+40)==0x530&&r.Read<uintptr_t>(s.boolMetadata+48)==address+s.setter&&
        !std::memcmp(base+s.setter,setter,sizeof(setter))&&!std::memcmp(base+s.name,s.key,std::strlen(s.key)+1)&&
        !std::memcmp(base+s.boolName,"bOverride_",10)&&!std::memcmp(base+s.boolName+10,s.key,std::strlen(s.key)+1);
}
inline unsigned Initialize(const unsigned char* base,bool exact){
    uint64_t mask=0;unsigned found=0;
    if(exact)for(size_t i=0;i<Count;++i)if(Verify(base,Specs[i])){mask|=uint64_t{1}<<i;++found;}
    verifiedMask.store(mask,std::memory_order_release);return found;
}
template<class ReadValue> bool Apply(void* output,uint64_t active,ReadValue read){
    if(!output||!active)return false;
    const auto selected=active&verifiedMask.load(std::memory_order_acquire);
    if(!selected)return false;
    auto* p=static_cast<unsigned char*>(output);float weight=0;
    std::memcpy(&weight,p+0x38,4);if(weight!=1.0f)return false;
    bool changed=false;
    for(size_t i=0;i<Count;++i)if(selected&(uint64_t{1}<<i)){
        const float value=read(i);if(!std::isfinite(value)||value<0||value>Specs[i].maximum)continue;
        const auto& s=Specs[i];float native=0;std::memcpy(&native,p+0x40+s.offset,4);
        if(!std::isfinite(native))continue;
        std::memcpy(p+0x40+s.offset,&value,4);p[0x40+s.flag]|=static_cast<unsigned char>(s.mask);changed=true;
    }
    return changed;
}
}
