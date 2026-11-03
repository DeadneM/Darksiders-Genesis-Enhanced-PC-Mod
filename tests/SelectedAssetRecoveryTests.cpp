#include "SelectedAssetRecovery.h"
#include <iostream>
int main(){
    using namespace dg::selected_recovery;unsigned failed=0;
    auto check=[&](bool value,const char* message){if(!value){++failed;std::cerr<<message<<'\n';}};
    Requests<4> library,materials;nextLoadAt=0;
    Scope first{1,2,3,4};library.Enter(first,1000);materials.Enter(first,1000);
    check(!library.Begin(0,1749,true,true),"new world or selection first settles");
    check(!library.Begin(0,1750,false,true)&&!library.Begin(0,1750,true,false)&&!library.entries[0].count,"pause/cutscene/UI or missing loader never consumes an attempt");
    check(library.Begin(0,1750,true,true),"only the requested missing asset starts");
    check(!materials.Begin(0,1750,true,true)&&!library.Begin(1,1750,true,true),"one new visual load across consumers within a frame/budget window");
    check(materials.Begin(0,2000,true,true),"another selected resource gets the next budget window");
    check(!library.Begin(0,6749,true,true)&&library.Begin(0,6750,true,true),"failed selected resource retry is throttled five seconds");
    check(library.Begin(0,11750,true,true)&&library.Exhausted(0)&&!library.Begin(0,99999,true,true),"three failures stop repeated native load calls");
    library.Forget(0);check(library.Begin(0,12000,true,true),"resource invalidation or observed unload re-arms recovery");
    library.Enter({1,2,3,5},13000);check(!library.entries[0].count&&!library.Begin(0,13749,true,true)&&library.Begin(0,13750,true,true),"selection change retries from a fresh stable context");
    library.Enter({1,2,4,5},14000);check(!library.entries[0].count,"same address with new identity resets attempts");
    library.Enter({9,2,4,5},15000);check(!library.Begin(0,14999,true,true),"clock/context rollback never bypasses settling");
    check(!library.Begin(99,16000,true,true),"out-of-catalog index never starts a load");
    std::cout<<"Selected resource recovery: "<<failed<<" failures\n";return failed?1:0;
}
