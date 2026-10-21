#pragma once
#include <array>

namespace dg::wrath_slots {
// Stable IDs reserve all six requested powers. Foreign-character choices are
// deliberately rejected until their animation/component adapter exists.
using Choices=std::array<std::array<int,3>,2>;
inline constexpr const wchar_t* Keys[2][3]={
    {L"StrifeWrathSlot1",L"StrifeWrathSlot2",L"StrifeWrathSlot3"},
    {L"WarWrathSlot1",L"WarWrathSlot2",L"WarWrathSlot3"}};
constexpr bool Supported(unsigned family,int choice){
    return family<2&&(choice==0||(choice>=int(family)*3+1&&choice<=int(family)*3+3));
}
constexpr int Normalize(unsigned family,int choice){return Supported(family,choice)?choice:0;}
constexpr int NativeNumber(unsigned family,int choice){
    return Supported(family,choice)&&choice?choice-int(family)*3+1:0;
}
}
