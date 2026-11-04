#include "DashScriptGuard.h"
#include <iostream>
#include <vector>
int main(){using namespace dg::dash_script;unsigned failures=0;
auto check=[&](bool ok,const char* why){if(!ok){++failures;std::cerr<<why<<'\n';}};
{
std::vector<unsigned char> code={78,0,0,0,0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,25,1,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,28,0,0,0,0,0,0,0,0,40,22,28,0,0,0,0,0,0,0,0,32,0,0,0,0,0,0,0,0,42,0,0,0,0,0,0,0,0,0,22,6,182,0,0,0,28,0,0,0,0,0,0,0,0,32,0,0,0,0,0,0,0,0,30,0,0,128,63,33,0,0,0,0,0,0,0,0,0,0,0,0,22,6,182,0,0,0,20,0,0,0,0,0,0,0,0,0,104,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,36,38,22,7,10,0,0,0,0,0,0,0,0,0,0,0,0,4,11,83};
auto matches=[&](bool metadata=true){return Matches(code,GraphSize,GraphHash,GraphBlocks,[&](const auto&){return metadata;});};
check(matches(),"complete known script accepted with verified references");
check(!matches(false),"same bytecode with unknown referenced objects rejected");
std::vector<bool> opaque(code.size(),false);
for(const auto& b:GraphBlocks){for(unsigned i=0;i<b.width;++i)opaque[b.offset+i]=true;}
for(size_t i=0;i<code.size();++i){code[i]^=0x40;check(matches()==opaque[i],"every opcode/constant is protected; references require independent metadata validation");code[i]^=0x40;}
code.pop_back();check(!matches(),"truncated script rejected");
code.push_back(0x53);code.push_back(0x53);check(!matches(),"additional script work rejected");
}
{
std::vector<unsigned char> code={100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,70,0,0,0,0,0,0,0,0,29,137,0,0,0,22,4,11,83};
auto matches=[&](bool metadata=true){return Matches(code,FinishSize,FinishHash,FinishBlocks,[&](const auto&){return metadata;});};
check(matches(),"complete known script accepted with verified references");
check(!matches(false),"same bytecode with unknown referenced objects rejected");
std::vector<bool> opaque(code.size(),false);
for(const auto& b:FinishBlocks){for(unsigned i=0;i<b.width;++i)opaque[b.offset+i]=true;}
for(size_t i=0;i<code.size();++i){code[i]^=0x40;check(matches()==opaque[i],"every opcode/constant is protected; references require independent metadata validation");code[i]^=0x40;}
code.pop_back();check(!matches(),"truncated script rejected");
code.push_back(0x53);code.push_back(0x53);check(!matches(),"additional script work rejected");
}
{
std::vector<unsigned char> code={15,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,11,83};
auto matches=[&](bool metadata=true){return Matches(code,ReleasedAimSize,ReleasedAimHash,ReleasedAimBlocks,[&](const auto&){return metadata;});};
check(matches(),"pure native AimMode setter accepted with checked property references");
check(!matches(false),"same setter with unknown property references rejected");
std::vector<bool> opaque(code.size(),false);
for(const auto& b:ReleasedAimBlocks)for(unsigned i=0;i<b.width;++i)opaque[b.offset+i]=true;
for(size_t i=0;i<code.size();++i){code[i]^=0x40;check(matches()==opaque[i],"setter cannot gain an opcode or change target without rejecting");code[i]^=0x40;}
code.push_back(0x53);check(!matches(),"extended setter rejected");
}
std::cout<<"Dash complete-script guards: "<<failures<<" failures\n";return failures?1:0;}
