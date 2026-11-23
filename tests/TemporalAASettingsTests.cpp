#include "TemporalAASettings.h"
#include "RenderQualitySettings.h"
#include "HdrSettings.h"
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
    const dg::hdr::Settings nativeHdr{};Check(nativeHdr.mode==-1&&nativeHdr.uiBrightness==-1,"HDR leaves existing engine settings native by default");
    const auto legacy1000=dg::hdr::Normalize({1,1.25f,2,false});
    const auto legacy2000=dg::hdr::Normalize({2,-1,1});
    Check(legacy1000.mode==3&&legacy1000.buffer==0&&legacy1000.uiBrightness==1.25f&&!legacy1000.dxgiSignaling,
        "legacy 1000-nit HDR10 uses scRGB and retains HUD and signaling choices");
    Check(legacy2000.mode==4&&legacy2000.buffer==1,"legacy 2000-nit HDR10 keeps peak and native buffer preference");
    Check(dg::hdr::Normalize(legacy1000).mode==3&&dg::hdr::Normalize(legacy2000).mode==4,
        "scRGB migration is idempotent");
    Check(!dg::hdr::EngineProfile{1,0,0}.Matches(3)&&!dg::hdr::EngineProfile{1,3,2}.Matches(3)&&
        !dg::hdr::EngineProfile{0,5,0}.Matches(3)&&!dg::hdr::EngineProfile{1,5,2}.Matches(3),
        "DXGI signaling requires the matching enabled scRGB encoding");
    Check(dg::hdr::EngineProfile{1,5,0}.Matches(3)&&dg::hdr::EngineProfile{1,6,0}.Matches(4),
        "each scRGB peak requires its own engine tone curve");
    const auto boundedHdr=dg::hdr::Normalize({4,9});Check(boundedHdr.mode==4&&boundedHdr.uiBrightness==4,"HDR UI range remains bounded");
    const auto invalidHdr=dg::hdr::Normalize({99,std::numeric_limits<float>::quiet_NaN()});Check(invalidHdr.mode==-1&&invalidHdr.uiBrightness==-1,"invalid HDR input fails to Native");
    for(size_t i=0;i<dg::render_quality::Specs.size();++i){const auto& spec=dg::render_quality::Specs[i];
        Check(dg::render_quality::Normalize(i,-1)==-1&&dg::render_quality::Normalize(i,std::numeric_limits<float>::quiet_NaN())==-1,"Native and invalid rendering inputs stay Native");
        Check(dg::render_quality::Normalize(i,0)==spec.minimum&&dg::render_quality::Normalize(i,spec.maximum*2)==spec.maximum,"rendering range respects real nonzero minima and bounded ceilings");
        Check(dg::render_quality::Normalize(i,spec.trial)==spec.trial,"render trial choice is representable");}
    Check(dg::render_quality::Normalize(0,3.6f)==4,"integer CVar choices cannot retain fractional values");
    using dg::render_quality::ConsoleValue;
    dg::render_quality::LiveOverride renderOverride;
    ConsoleValue actual{3,0x01000000};
    auto set=[&](ConsoleValue value){actual=value;return true;};
    renderOverride.Update(actual,0,set);
    Check(actual==ConsoleValue{0,0x01000000},"SSR zero applies with the engine's original priority");
    renderOverride.Update(actual,4,set);
    renderOverride.Update(actual,std::nullopt,set);
    Check(actual==ConsoleValue{3,0x01000000},"Native restores the original value across slider changes");
    renderOverride.Update(actual,4,set);
    actual.priority=0x02000000;
    renderOverride.Update(actual,std::nullopt,set);
    Check(actual==ConsoleValue{4,0x02000000},"Native preserves an intervening engine write of equal value at a different priority");
    const Settings defaults;
    Check(!defaults.enabled,"existing installations retain native TAA by default");
    Check(defaults.motionBlurAmount==-1&&defaults.colorFringeAmount==-1,"native image effects retained by default");
    const auto filters=Normalize({false,.08f,1,0,0});
    Check(!filters.enabled&&filters.motionBlurAmount==0&&filters.colorFringeAmount==0,"image filters independent of TAA customization");
    auto settings=Normalize({true,std::numeric_limits<float>::quiet_NaN(),9});
    Check(settings.enabled&&settings.frameWeight==.08f&&settings.sharpen==9,"invalid settings bounded without NaN");
    settings=Normalize({false,-1,-9});
    Check(!settings.enabled&&settings.frameWeight==0&&settings.sharpen==0,"limits and disabled flag preserved");
    const auto explicitAmounts=Normalize({false,.08f,1,.25f,2.5f});
    Check(explicitAmounts.motionBlurAmount==.25f&&explicitAmounts.colorFringeAmount==2.5f,"independent explicit amounts retain exact values without enabling TAA");
    const auto motionTwo=Normalize({false,.08f,1,2,-1});
    Check(motionTwo.motionBlurAmount==2,"native positive motion blur amount is not artificially limited to one");
    Check(Normalize({false,.08f,99,99,-1}).motionBlurAmount==10,"motion amount clamps at the explicit trial limit ten");
    const auto bounded=Normalize({false,.08f,1,9,99});
    Check(bounded.motionBlurAmount==9&&bounded.colorFringeAmount==10,"image amounts bounded to the UI/native consumer ranges");
    const auto invalid=Normalize({false,.08f,1,std::numeric_limits<float>::quiet_NaN(),-9});
    Check(invalid.motionBlurAmount==-1&&invalid.colorFringeAmount==-1,"invalid/negative image amounts return to native ownership");
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
    ShadowOverride filter;Access nativeEffects;nativeEffects.words.fill(std::bit_cast<int32_t>(-1.0f));
    filter.Update(nativeEffects,true,0,-2,10);
    Check(nativeEffects.Value(0)==0&&nativeEffects.Value(1)==0,"image filter suppresses game/render native amount");
    filter.Update(nativeEffects,true,.35f,-2,10);
    Check(nativeEffects.Value(0)==.35f&&nativeEffects.Value(1)==.35f,"adjustable image amount reaches both native shadows");
    const auto writes=nativeEffects.writes;filter.Update(nativeEffects,true,.35f,-2,10);
    Check(nativeEffects.writes==writes,"unchanged image amount does not add writes");
    filter.Update(nativeEffects,false,0,-2,10);
    Check(nativeEffects.Value(0)==-1&&nativeEffects.Value(1)==-1,"native negative no-override sentinel restored exactly");
}
}
int main(){try{Run();std::cout<<"Temporal AA ownership/settings checks passed\n";return 0;}
    catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
