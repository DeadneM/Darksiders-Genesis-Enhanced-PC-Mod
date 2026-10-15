#include "ActionRecoveryPolicy.h"
#include <cstdio>
#include <limits>
int main(){
    using namespace dg::recovery;
    unsigned failed=0;
    auto check=[&](bool value,const char* name){if(!value){++failed;std::printf("FAIL %s\n",name);}};
    TailWindow t{};
    check(!TailReady(t,1,2,3,false,1,0.016f,0),"running core is never released");
    check(TailReady(t,1,2,3,true,1,0.016f,0),"zero delay at natural blend tail");
    t={};check(!TailReady(t,1,2,3,true,1,0.1f,200),"delay starts at observed blend");
    check(!TailReady(t,1,2,3,true,1.1f,0.1f,200),"first tenth waits");
    check(TailReady(t,1,2,3,true,1.2f,0.1f,200),"two tenths satisfy delay");
    check(!TailReady(t,1,2,4,true,1.3f,0.1f,200),"generation resets delay");
    check(!TailReady(t,1,4,4,true,1.4f,0.1f,200),"new instance resets delay");
    check(!TailReady(t,1,4,4,true,0.1f,0.1f,200),"new activation resets delay");
    check(!TailReady(t,1,4,4,true,0.1f,0.1f,200),"duplicate tick does not advance delay");
    check(!TailReady(t,1,4,4,true,1,1,0),"long stalled frame is rejected");
    check(!TailReady(t,1,4,4,true,std::numeric_limits<float>::quiet_NaN(),0.1f,0),"invalid animation time rejected");
    check(!TailReady(t,1,4,4,true,1,0.1f,std::numeric_limits<float>::quiet_NaN()),"invalid configured delay uses conservative half second");
    std::printf("Action recovery: %u failures\n",failed);return failed?1:0;
}
