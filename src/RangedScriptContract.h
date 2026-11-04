#pragma once
#include <array>
#include <cstddef>
#include <span>
#include <cstdint>
namespace dg::ranged_script {
// Fingerprints of complete native Strife aim setter. Only resolved
// UObject references/FScriptNames are normalized, then checked by name.
// Unknown or altered scripts never receive a cleanup request.
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
