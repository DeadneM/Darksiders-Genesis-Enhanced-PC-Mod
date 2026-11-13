#include "TemporalAASettings.h"
#include <array>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace {
using namespace dg::temporal_aa;
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
struct Access {
    std::array<int32_t,2> words{std::bit_cast<int32_t>(.04f),std::bit_cast<int32_t>(.04f)};
    bool race=false;
    unsigned writes=0;
    int32_t Read(unsigned i){return words[i];}
    int32_t ExchangeIf(unsigned i,int32_t value,int32_t expected){
        if(race){words[i]=std::bit_cast<int32_t>(.2f);race=false;}
        const int32_t prior=words[i];
        if(prior==expected){words[i]=value;++writes;}
        return prior;
    }
    float Value(unsigned i){return std::bit_cast<float>(words[i]);}
};
void Run(){
    const Settings defaults;
    Check(!defaults.enabled,"existing installations retain native TAA by default");
    auto settings=Normalize({true,std::numeric_limits<float>::quiet_NaN(),9});
    Check(settings.enabled&&settings.frameWeight==.08f&&settings.sharpen==2,"invalid settings bounded without NaN");
    settings=Normalize({false,-1,-9});
    Check(!settings.enabled&&settings.frameWeight==.01f&&settings.sharpen==0,"limits and disabled flag preserved");
    Access access;ShadowOverride state;
    state.Update(access,false,.08f,0,1);
    Check(access.writes==0,"disabled path leaves both native shadows untouched");
    state.Update(access,true,.04f,0,1);state.Update(access,false,0,0,1);
    Check(access.writes==0,"already matching native values are never claimed");
    state.Update(access,true,.08f,0,1);
    Check(access.Value(0)==.08f&&access.Value(1)==.08f,"game and render shadows updated");
    state.Update(access,true,.1f,0,1);state.Update(access,false,0,0,1);
    Check(access.Value(0)==.04f&&access.Value(1)==.04f,"slider changes retain original values for restore");
    state.Update(access,true,.08f,0,1);access.words[1]=std::bit_cast<int32_t>(.3f);
    state.Update(access,false,0,0,1);
    Check(access.Value(0)==.04f&&access.Value(1)==.3f,"restore preserves intervening native changes per shadow");
    state.Update(access,true,.08f,0,1);access.words[0]=std::bit_cast<int32_t>(.4f);
    state.Update(access,true,.1f,0,1);state.Update(access,false,0,0,1);
    Check(access.Value(0)==.4f&&access.Value(1)==.3f,"new external baseline restored after override continues");
    Access replaced;state.Update(replaced,true,.08f,0,1);state.Forget();
    Access fresh;fresh.words.fill(std::bit_cast<int32_t>(.5f));state.Update(fresh,false,0,0,1);
    Check(fresh.Value(0)==.5f&&fresh.Value(1)==.5f,"allocation change never restores an older allocation's values");
    Access racing;racing.race=true;state.Update(racing,true,.08f,0,1);state.Update(racing,false,0,0,1);
    Check(racing.Value(0)==.2f,"failed compare-exchange never claims ownership");
    Access corrupt;corrupt.words.fill(std::bit_cast<int32_t>(std::numeric_limits<float>::infinity()));
    state.Update(corrupt,true,.08f,0,1);Check(corrupt.writes==0,"invalid native data never overwritten");
}
}
int main(){try{Run();std::cout<<"Temporal AA ownership/settings checks passed\n";return 0;}
    catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
