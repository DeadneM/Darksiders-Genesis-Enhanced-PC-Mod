#pragma once
#include <array>
#include <cstddef>
#include <span>
#include <cstdint>
namespace dg::dash_script {
// Fingerprints of complete native Strife dash/aim scripts. Only resolved
// UObject references/FScriptNames are normalized, then checked by name.
// Unknown or altered scripts retain their complete native event.
struct Block {unsigned offset,width;const char* name;};
template<std::size_t N,class Verify> bool Matches(std::span<const unsigned char> code,
    unsigned size,uint64_t expected,const std::array<Block,N>& blocks,Verify verify){
    if(code.size()!=size)return false;
    uint64_t hash=14695981039346656037ull;std::size_t next=0;
    for(std::size_t i=0;i<code.size();){
        if(next<N&&i==blocks[next].offset){
            const auto& b=blocks[next++];if(b.width>code.size()-i||!verify(b))return false;
            for(unsigned j=0;j<b.width;++j)hash*=1099511628211ull;
            i+=b.width;
        }else{hash=(hash^code[i])*1099511628211ull;++i;}
    }
    return next==N&&hash==expected;
}
inline constexpr unsigned GraphSize=185;
inline constexpr uint64_t GraphHash=15091677296391271832ull;
inline constexpr std::array<Block,16> GraphBlocks={{
{2,8,"EntryPoint"},
{11,8,"CallFunc_CreateDashInfoForPlayer_ReturnValue"},
{20,8,"CallFunc_CreateDashInfoForPlayer_ReturnValue"},
{30,8,"Character"},
{42,8,"CallFunc_CreateDashInfoForPlayer_ReturnValue"},
{51,8,"CreateDashInfoForPlayer"},
{62,8,"DoAbility"},
{71,8,"MayhemAbility_Strife_Ground_Dash_Start_Level1_Blueprint_C"},
{81,8,"CallFunc_CreateDashInfoForPlayer_ReturnValue"},
{96,8,"PlayMontage"},
{105,8,"Strife_AirDash_Fall_Montage"},
{119,12,"None"},
{139,8,"K2Node_SwitchEnum_CmpSuccess"},
{148,8,"NotEqual_ByteByte"},
{157,8,"K2Node_Event_Cause"},
{174,8,"K2Node_SwitchEnum_CmpSuccess"},
}};
inline constexpr unsigned FinishSize=36;
inline constexpr uint64_t FinishHash=9167806210151048089ull;
inline constexpr std::array<Block,3> FinishBlocks={{
{1,8,"K2Node_Event_Cause"},
{10,8,"Cause"},
{19,8,"ExecuteUbergraph_MayhemAbility_Strife_AirDash_Blueprint"},
}};
// SetAimMode contains only AimMode = Mode, return and end-script. A caller
// additionally checks both property references, their owners and byte schema.
inline constexpr unsigned ReleasedAimSize=30;
inline constexpr uint64_t ReleasedAimHash=4422810318088863023ull;
inline constexpr std::array<Block,3> ReleasedAimBlocks={{
{1,8,"AimMode"},
{10,8,"AimMode"},
{19,8,"Mode"},
}};
}
