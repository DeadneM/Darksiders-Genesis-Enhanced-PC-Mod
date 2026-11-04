#include "RangedStatePolicy.h"
#include "RangedScriptContract.h"
#include <vector>
#include <iostream>
int main(){using dg::ranged_state::Policy;unsigned fail=0;
    auto check=[&](bool ok,const char* why){if(!ok){++fail;std::cerr<<why<<'\n';}};
    Policy p;
    check(!p.Recover(1,true,false,true),"held aim/fire remains native");
    check(!p.Recover(1,false,true,true),"interruption action must finish before repair");
    check(!p.Recover(1,false,false,false),"airborne, lock, menus or unknown context cannot repair");
    check(p.Recover(1,false,false,true),"orphan state is handled independently of interrupt type");
    check(!p.Recover(1,false,false,true),"one request only, no per-frame fight with native state");
    check(!p.Recover(0,false,false,true),"normal native exit resets episode");
    check(p.Recover(1,false,false,true),"a later independent interruption can be handled");
    check(!p.Recover(2,false,false,true),"unrelated modes untouched");
    {
        using namespace dg::ranged_script;
        std::vector<unsigned char> code={15,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,11,83};
        auto matches=[&](bool metadata=true){return Matches(code,ReleasedAimSize,ReleasedAimHash,ReleasedAimBlocks,[&](const auto&){return metadata;});};
        check(matches(),"complete pure native setter retained");
        check(!matches(false),"unknown property reference cannot be called");
        std::vector<bool> opaque(code.size(),false);
        for(const auto& b:ReleasedAimBlocks)for(unsigned i=0;i<b.width;++i)opaque[b.offset+i]=true;
        for(size_t i=0;i<code.size();++i){code[i]^=0x40;check(matches()==opaque[i],"every opcode/constant remains protected independently of identifier case");code[i]^=0x40;}
        code.push_back(0x53);check(!matches(),"extra script work rejected");
    }
    std::cout<<"Ranged state policy: "<<fail<<" failures\n";return fail?1:0;
}
