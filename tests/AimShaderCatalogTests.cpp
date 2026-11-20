#include "AimShaderCatalog.h"
#include <iostream>
#include <array>
#include <limits>
#include <vector>

namespace {
// Retain the original shader CRC implementation as an independent oracle.
uint32_t OriginalBytecodeCRC32(const void* bytes,std::size_t length){
    auto* p=static_cast<const uint8_t*>(bytes);
    uint32_t crc=0xFFFFFFFFu;
    for(std::size_t i=0;i<length;++i){
        crc^=p[i];
        for(unsigned bit=0;bit<8;++bit)
            crc=(crc>>1)^((crc&1u)?0xEDB88320u:0u);
    }
    return ~crc;
}
constexpr std::array<uint8_t,9> CRCCheck={'1','2','3','4','5','6','7','8','9'};
static_assert(dg::aim_shader_catalog::BytecodeCRC32(CRCCheck)==0xCBF43926u);
static_assert(dg::aim_shader_catalog::BytecodeCRC32(std::span<const uint8_t>{})==0u);
static_assert(dg::aim_shader_catalog::MayMatchSize(dg::aim_shader_catalog::Signatures[0].bytes));
static_assert(!dg::aim_shader_catalog::MayMatchSize(0));
}

int main(){using namespace dg::aim_shader_catalog;unsigned failures=0;
    auto check=[&](bool ok,const char* label){if(!ok){++failures;std::cerr<<label<<'\n';}};
    check(Signatures.size()==168&&MaxTrackedShaders==336,"native catalog and bounded COM capacity");
    unsigned beam=0,arc=0;uint32_t previous=0;
    for(const auto& s:Signatures){
        check(s.crc>previous&&s.bytes>32&&s.fingerprint,"unique sorted complete native shaders");previous=s.crc;
        check(Find(s.crc)==&s,"binary lookup preserves exact entry identity");
        check(MayMatchSize(s.bytes),"every catalog size passes the exact prefilter");
        if(s.kind==Kind::Beam)++beam;else ++arc;
    }
    std::size_t largestSize=0;
    for(const auto& s:Signatures)if(s.bytes>largestSize)largestSize=s.bytes;
    for(std::size_t bytes=0;bytes<=largestSize+1;++bytes){
        bool catalogContainsSize=false;
        for(const auto& s:Signatures)if(s.bytes==bytes){catalogContainsSize=true;break;}
        check(MayMatchSize(bytes)==catalogContainsSize,"size prefilter accepts exactly the catalog sizes, including holes");
    }
    check(!MayMatchSize(std::numeric_limits<std::size_t>::max()),"extreme impossible size is rejected before indexing");
    check(BytecodeCRC32(CRCCheck)==0xCBF43926u,"standard reflected CRC32 check vector 123456789");
    check(BytecodeCRC32(nullptr,0)==0u,"empty pointer bytecode preserves the original CRC");
    constexpr std::array<std::size_t,28> lengths={0,1,2,3,4,7,8,15,16,31,32,63,64,127,128,255,256,257,511,512,513,1023,1024,4095,4096,4097,65535,65536};
    std::vector<uint8_t> data(65536+8);
    auto checkCRC=[&](std::size_t length,std::size_t offset){
        const auto* bytes=data.data()+offset;
        const auto expected=OriginalBytecodeCRC32(bytes,length);
        check(BytecodeCRC32(bytes,length)==expected,"table CRC equals original bitwise CRC for pointer input");
        check(BytecodeCRC32(std::span<const uint8_t>{bytes,length})==expected,"table CRC equals original bitwise CRC for span input");
    };
    for(unsigned pattern=0;pattern<4;++pattern){
        uint32_t random=0x9E3779B9u;
        for(std::size_t i=0;i<data.size();++i){
            random^=random<<13;random^=random>>17;random^=random<<5;
            data[i]=pattern==0?0u:pattern==1?0xFFu:pattern==2?static_cast<uint8_t>(i):static_cast<uint8_t>(random);
        }
        for(const auto length:lengths)for(std::size_t offset=0;offset<8;++offset)checkCRC(length,offset);
        for(const auto& s:Signatures)checkCRC(s.bytes,0);
    }
    check(beam==56&&arc==112,"only the two native aim materials included");
    const auto* arcKnown=Find(1212955419u);const auto* beamKnown=Find(2294978555u);
    check(arcKnown&&arcKnown->kind==Kind::Arc&&beamKnown&&beamKnown->kind==Kind::Beam,"both user-captured shaders preserved with proven material ownership");
    check(!Find(0)&&!Find(0xFFFFFFFFu),"unknown shader CRC is never a match");
    constexpr std::array<uint8_t,5> hello={'h','e','l','l','o'};
    check(Fingerprint(hello)==0xa430d84680aabd0bull,"full bytecode fingerprint uses FNV1a64");
    const Signature synthetic{1,5,0xa430d84680aabd0bull,Kind::Beam};
    check(Matches(synthetic,hello),"matching length and complete fingerprint accepted");
    auto wrong=synthetic;wrong.bytes=4;check(!Matches(wrong,hello),"same fingerprint with wrong length rejected");
    wrong=synthetic;wrong.fingerprint^=1;check(!Matches(wrong,hello),"same length with wrong fingerprint rejected");
    for(unsigned i=0;i<hello.size();++i){auto changed=hello;changed[i]^=1;check(!Matches(synthetic,changed),"a changed byte cannot reuse a catalog identity");}
    std::cout<<"Aim material shader catalog: "<<failures<<" failures\n";return failures?1:0;
}
