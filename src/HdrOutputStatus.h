#pragma once
#include <cstdint>
namespace dg::hdr {
struct Status {
    bool checked=false,deviceKnown=false,outputKnown=false,advancedOutputKnown=false,desktopHdrActive=false;
    uint32_t bufferFormat=0;unsigned bitsPerColor=0;float peakNits=0;
    char adapter[128]{},display[64]{};
};
}
