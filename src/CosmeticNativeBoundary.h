#pragma once
#include "NativeMemoryReader.h"
#include <array>
#include <cstring>

namespace dg::cosmetics {
inline constexpr uintptr_t UnlockRva=0x574870;
inline constexpr uintptr_t SkinFilterRva=0x6A2460,SetSkinRva=0x617A50,SkinFilterCaller=0x6C3F4E;
inline bool SkinFilterBoundaryMatches(uintptr_t base){
    native_objects::MemoryReader reader;
    std::array<unsigned char,16> filter{},setter{};std::array<unsigned char,5> call{};
    std::array<unsigned char,6> dlcTest{};
    constexpr unsigned char filterCode[]={0x48,0x8B,0xC4,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x83,0xEC,0x60};
    constexpr unsigned char setterCode[]={0x48,0x89,0x5C,0x24,0x10,0x56,0x48,0x83,0xEC,0x20,0x48,0x83,0x3A,0x00,0x48,0x8B};
    constexpr unsigned char testCode[]={0x80,0x78,0x60,0x00,0x74,0x5F};
    int32_t displacement=0;
    if(!base||!reader.Read(base,SkinFilterRva,filter)||std::memcmp(filter.data(),filterCode,16)||
       !reader.Read(base,SetSkinRva,setter)||std::memcmp(setter.data(),setterCode,16)||
       !reader.Read(base,0x6A2583,dlcTest)||std::memcmp(dlcTest.data(),testCode,6)||
       !reader.Read(base,SkinFilterCaller-5,call)||call[0]!=0xE8)return false;
    std::memcpy(&displacement,call.data()+1,4);
    return int64_t(SkinFilterCaller)+displacement==int64_t(SkinFilterRva);
}
// Native registration pairs are {name, wrapper}, never {wrapper, name}.
// Bind the exact IsSkinUnlocked name to its wrapper and its direct native call.
// This rejects the adjacent IsSkinEquipped pair mistakenly used in V1.67.
inline bool UnlockBoundaryMatches(uintptr_t base){
    native_objects::MemoryReader reader;
    uintptr_t name=0,wrapper=0;std::array<char,15> text{};
    std::array<unsigned char,16> prologue{};std::array<unsigned char,5> call{};
    constexpr unsigned char expected[]={0x48,0x8B,0xC4,0x56,0x57,0x41,0x56,0x48,0x83,0xEC,0x70,0x48,0xC7,0x40,0xB8,0xFE};
    int32_t displacement=0;
    if(!base||!reader.Read(base,0x27D7870,name)||name!=base+0x27D61D0||
       !reader.Read(base,0x27D7878,wrapper)||wrapper!=base+0x7C4440||
       !reader.Read(name,0,text)||std::memcmp(text.data(),"IsSkinUnlocked",15)||
       !reader.Read(base,UnlockRva,prologue)||std::memcmp(prologue.data(),expected,sizeof(expected))||
       !reader.Read(base,0x7C44C7,call)||call[0]!=0xE8)return false;
    std::memcpy(&displacement,call.data()+1,4);
    return int64_t(0x7C44CC)+displacement==int64_t(UnlockRva);
}
}
