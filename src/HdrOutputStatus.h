#pragma once
#include <cstdint>
namespace dg::hdr {
struct Status {
    bool checked=false,deviceKnown=false,outputKnown=false,advancedOutputKnown=false,desktopHdrActive=false;
    bool transportAttempted=false,transportApplied=false,formatMismatch=false;int32_t transportResult=0;
    bool engineMismatch=false;
    int startupMode=-1,engineEnabled=-1,engineDevice=-1,engineGamut=-1,renderCapability=-1;
    uint32_t bufferFormat=0;unsigned bitsPerColor=0;float peakNits=0;
    char adapter[128]{},display[64]{};
};
}
