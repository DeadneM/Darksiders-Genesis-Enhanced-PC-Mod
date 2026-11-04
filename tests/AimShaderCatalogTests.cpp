#include "AimShaderCatalog.h"
#include <iostream>
#include <array>
int main(){using namespace dg::aim_shader_catalog;unsigned failures=0;
    auto check=[&](bool ok,const char* label){if(!ok){++failures;std::cerr<<label<<'\n';}};
    check(Signatures.size()==168&&MaxTrackedShaders==336,"native catalog and bounded COM capacity");
    unsigned beam=0,arc=0;uint32_t previous=0;
    for(const auto& s:Signatures){
        check(s.crc>previous&&s.bytes>32&&s.fingerprint,"unique sorted complete native shaders");previous=s.crc;
        check(Find(s.crc)==&s,"binary lookup preserves exact entry identity");
        if(s.kind==Kind::Beam)++beam;else ++arc;
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
