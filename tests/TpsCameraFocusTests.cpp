#include "../src/TpsCameraFocus.h"
#include <array>
#include <iostream>
#include <limits>
#include <stdexcept>
namespace {
using View=std::array<unsigned char,0x580>;
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
template<class T> void Put(View& v,size_t at,T value){std::memcpy(v.data()+at,&value,sizeof(value));}
float Focus(const View& v){float f=0;std::memcpy(&f,v.data()+0x494,4);return f;}
View Native(){
    View v{};v.fill(0xA5);float frame[7]{0,0,0,0,0,0,90};
    std::memcpy(v.data(),frame,sizeof(frame));Put(v,0x38,1.0f);Put(v,0x494,1000.0f);return v;
}
void Run(){
    auto v=Native(),before=v;
    Check(!dg::camera_focus::Compensate(v.data(),{300,100,20})&&v==before,"unknown layout never changes a native frame");
    dg::camera_focus::layoutReady=true;
    Check(dg::camera_focus::Compensate(v.data(),{300,400,0})&&std::fabs(Focus(v)-300)<.001f,
        "focus follows forward depth, not 500-unit slant distance");
    for(size_t i=0;i<v.size();++i)
        if(i!=0x56&&(i<0x494||i>=0x498))Check(v[i]==before[i],"all non-focus bytes, blend and authored blur settings survive");
    Check(v[0x56]==(before[0x56]|0x08),"only the native focal-distance override bit is added");
    // Retail render blend 1B2F693 consumes bit 0x08 for FocalDistance;
    // bit 0x10 belongs to Fstop. Reproduce that consumer, not our setter.
    auto renderFocus=[](const View& frame){return frame[0x56]&8?Focus(frame):1000.0f;};
    v=Native();v[0x56]=0x10;Put(v,0x494,365.0f);
    Check(renderFocus(v)==1000,"V1.56's wrong flag leaves the render focus native");
    Check(dg::camera_focus::Compensate(v.data(),{365,0,0})&&renderFocus(v)==365,
        "corrected override actually reaches the native focal-distance branch");
    Check(v[0x56]==0x18,"an authored aperture override is preserved");
    v=Native();v[0x56]=0;Put(v,0x340,2.8f);
    dg::camera_focus::Compensate(v.data(),{365,0,0});
    Check(v[0x56]==8&&renderFocus(v)==365,"fresh native frame activates focus alone, never aperture");
    v=Native();Put(v,16,90.0f);Check(dg::camera_focus::Compensate(v.data(),{0,275,0})&&std::fabs(Focus(v)-275)<.001f,"yaw follows the emitted TPS view");
    v=Native();Put(v,12,-30.0f);Check(dg::camera_focus::Compensate(v.data(),{259.80762f,0,-150})&&std::fabs(Focus(v)-300)<.01f,"pitched view uses its own forward plane");
    v=Native();before=v;Check(!dg::camera_focus::Compensate(v.data(),{-100,0,0})&&v==before,"player behind camera cannot create a negative focal plane");
    for(float weight:{0.0f,.5f,2.0f,std::numeric_limits<float>::quiet_NaN()}){
        v=Native();Put(v,0x38,weight);before=v;
        Check(!dg::camera_focus::Compensate(v.data(),{300,0,0})&&v==before,"partial or invalid weights are never raised");
    }
    v=Native();before=v;Check(!dg::camera_focus::Compensate(v.data(),{std::numeric_limits<float>::infinity(),0,0})&&v==before,"invalid pose stays native");
    auto p1=Native(),p2=Native();dg::camera_focus::Compensate(p1.data(),{300,0,0});
    Check(Focus(p2)==1000,"P1 output cannot modify P2");dg::camera_focus::Compensate(p2.data(),{700,0,0});
    Check(Focus(p1)==300&&Focus(p2)==700,"split-screen focus planes are independent");
    Check(Focus(Native())==1000,"fresh native frames retain authored focus without a restore write");
    auto legacy=Native();legacy[0x63]=1;legacy[0x58]=4;Put(legacy,0x340,2.8f);
    auto missingLayout=legacy;
    dg::camera_focus::Compensate(legacy.data(),{400,0,0},false);
    dg::camera_focus::Compensate(missingLayout.data(),{400,0,0},true);
    Check(legacy==missingLayout,"unverified lens layout falls back exactly to the V1.60 focus path");
    dg::camera_focus::lensLayoutReady=true;
    auto lens=Native();lens[0x63]=1;lens[0x58]=4;Put(lens,0x340,2.8f);before=lens;
    // Retail method blend 1B30380 uses mask 02, not adjacent mask 04.
    auto renderMethod=[](const View& frame){return frame[0x58]&2?int(frame[0x63]):1;};
    lens[0x63]=2;Check(renderMethod(lens)==1,"the neighbouring bit cannot activate a method override");
    lens=before;Check(dg::camera_focus::Compensate(lens.data(),{400,0,0},true)&&renderMethod(lens)==2,
        "lens option reaches the native CircleDOF method branch");
    Check(lens[0x58]==6&&lens[0x56]==(before[0x56]|8)&&Focus(lens)==400,
        "only the two documented override bits are added");
    for(size_t i=0;i<lens.size();++i)
        if(i!=0x56&&i!=0x58&&i!=0x63&&(i<0x494||i>=0x498))
            Check(lens[i]==before[i],"lens mode preserves aperture, sizes, regions, blend weights and all view geometry");
    auto off=before;dg::camera_focus::Compensate(off.data(),{400,0,0},false);
    Check(off==legacy,"OFF remains byte-identical to the previous focal-distance-only policy");
    auto unknown=before;unknown[0x63]=9;auto unknownBefore=unknown;
    dg::camera_focus::Compensate(unknown.data(),{400,0,0},true);
    Check(unknown[0x58]==unknownBefore[0x58]&&unknown[0x63]==9,"unknown native methods are never replaced");
    for(float weight:{0.0f,.5f,2.0f}){
        auto partial=before;Put(partial,0x38,weight);auto original=partial;
        Check(!dg::camera_focus::Compensate(partial.data(),{400,0,0},true)&&partial==original,
            "lens mode cannot raise partial weights or modify an invalid native frame");
    }
    p1=before;p2=before;dg::camera_focus::Compensate(p1.data(),{300,0,0},true);
    Check(p2==before,"P1 lens override cannot leak into P2 or native views");
}
}
int main(){try{Run();std::cout<<"TPS per-view focus and native postprocess preservation passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
