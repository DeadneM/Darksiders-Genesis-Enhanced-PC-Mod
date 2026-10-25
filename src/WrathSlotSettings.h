#pragma once
#include <array>

namespace dg::wrath_slots {
// Stable IDs reserve all six requested powers. Foreign adapters are isolated;
// powers without an adapter still normalize to Native.
using Choices=std::array<std::array<int,3>,2>;
inline constexpr const wchar_t* Keys[2][3]={
    {L"StrifeWrathSlot1",L"StrifeWrathSlot2",L"StrifeWrathSlot3"},
    {L"WarWrathSlot1",L"WarWrathSlot2",L"WarWrathSlot3"}};
constexpr bool Own(unsigned family,int choice){
    return family<2&&choice>=int(family)*3+1&&choice<=int(family)*3+3;
}
constexpr bool Foreign(unsigned family,int choice){return (family==0&&(choice==4||choice==6))||(family==1&&(choice>=1&&choice<=3));}
constexpr bool Supported(unsigned family,int choice){return family<2&&(choice==0||Own(family,choice)||Foreign(family,choice));}
constexpr int Normalize(unsigned family,int choice){return Supported(family,choice)?choice:0;}
constexpr int NativeNumber(unsigned family,int choice){
    return Own(family,choice)?choice-int(family)*3+1:0;
}
}
