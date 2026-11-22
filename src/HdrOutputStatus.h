#pragma once
#include <cstdint>
namespace dg::hdr {
enum class NativePath { Unknown, IntelUnavailable, VendorImplementation };
struct Status {
    bool checked=false,deviceKnown=false,outputKnown=false,advancedOutputKnown=false,desktopHdrActive=false;
    uint32_t vendor=0,bufferFormat=0;unsigned bitsPerColor=0;float peakNits=0;
    NativePath nativePath=NativePath::Unknown;
    char adapter[128]{},display[64]{};
};
inline NativePath PathFor(uint32_t vendor,bool exact){
    if(!exact)return NativePath::Unknown;
    if(vendor==0x8086)return NativePath::IntelUnavailable;
    return vendor==0x10DE||vendor==0x1002?NativePath::VendorImplementation:NativePath::Unknown;
}
}
