#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace dg::render_safety {
// Exact supported executable: eight 24-byte color views, count/flags,
// depth view/flags, eight UAV pointers and NumUAVs. Not a modern UE ABI.
inline constexpr size_t TargetsBytes=0x128;
static_assert(sizeof(uintptr_t)==8,"Supported RHI packet requires Windows x64");
struct alignas(8) TargetsCopy {std::array<unsigned char,TargetsBytes> bytes{};};
static_assert(sizeof(TargetsCopy)==TargetsBytes);
template<class T> inline T Field(const void* input,size_t offset){
    T value{};std::memcpy(&value,static_cast<const unsigned char*>(input)+offset,sizeof(T));return value;
}
inline bool CapturedInvalidStencilClear(const void* input){
    if(!input)return false;
    constexpr unsigned char capturedActions[]={0,0,2,1,0x20,0,0,0};
    const auto* bytes=static_cast<const unsigned char*>(input);
    return Field<uintptr_t>(input,0)!=0&&Field<uint32_t>(input,0xC0)==1&&bytes[0xC4]==0&&
        Field<uintptr_t>(input,0xC8)==0&&bytes[0xD8]==0&&bytes[0xD9]==1&&
        !std::memcmp(bytes+0xD0,capturedActions,sizeof(capturedActions))&&Field<uint32_t>(input,0x120)==0;
}
using SetTargets=void(*)(void*,const void*);
// Borrowed input is never changed. Valid/disabled requests keep pointer identity.
// Copy the full packet, including UAV tail, only for the captured invalid case.
inline bool Forward(SetTargets original,void* context,const void* input,bool enabled){
    if(!enabled||!CapturedInvalidStencilClear(input)){original(context,input);return false;}
    TargetsCopy corrected;
    std::memcpy(corrected.bytes.data(),input,TargetsBytes);
    corrected.bytes[0xD9]=0;
    original(context,corrected.bytes.data());
    return true;
}
}
