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
    Check(v[0x56]==(before[0x56]|0x10),"only the native focal-distance override bit is added");
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
}
}
int main(){try{Run();std::cout<<"TPS per-view focus and native postprocess preservation passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
