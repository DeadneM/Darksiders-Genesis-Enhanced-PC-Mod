#include "ManualActionBinding.h"
#include <iostream>
#include <stdexcept>
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
int main(){try{
    using namespace dg::manual_binding;
    Check(!Down(0,0,true,true),"two OFF inputs disable the binding");
    Check(Down(1,0,true,false)&&Down(0,2,false,true),"either slot alone works");
    Check(!Down(1,2,true,false)&&!Down(1,2,false,true)&&Down(1,2,true,true),"a chord needs both inputs");
    Edge first,second;
    Check(!first.Observe(1,2,true,true,10),"a held input cannot fire when first observed");
    Check(!first.Observe(1,2,false,true,10)&&first.Observe(1,2,true,true,10),"release then chord fires once");
    Check(!first.Observe(1,2,true,true,10),"holding never repeats");
    first.Observe(1,2,false,false,10);
    Check(!first.Observe(1,2,true,false,10)&&!first.Observe(1,2,true,true,10),"menu/focus suppression consumes the press");
    Check(!first.Observe(3,0,true,true,10),"rebinding while held cannot fire");
    first.Observe(3,0,false,true,10);
    Check(first.Observe(3,0,true,true,10),"one enabled button remains usable");
    Check(!first.Observe(3,0,true,true,20),"controller ownership change consumes a held press");
    first.Reset();Check(!first.Observe(3,0,true,true,20),"reconnection while held cannot fire");
    second.Observe(3,0,false,true,30);Check(second.Observe(3,0,true,true,30),"P2 has an independent edge");
    Check(!first.Observe(3,0,true,true,20),"P2 cannot reset a held P1 action");
    std::cout<<"Manual binding OFF, chords, focus, held keys and player isolation passed\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
