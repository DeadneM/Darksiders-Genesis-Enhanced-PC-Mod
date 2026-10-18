#pragma once
#include <algorithm>
#include <cstdint>

namespace dg::update {
// Presentation time only: an unfocused game does not consume the notice.
// The serial changes only after a successful check finds a newer build.
class ToastTimer {
    uint64_t serial_=0;
    double remaining_=0,last_=0;
    bool wasVisible_=false;
public:
    bool Tick(uint64_t serial,double now,bool visible){
        if(serial!=serial_){serial_=serial;remaining_=serial?8.0:0;wasVisible_=false;}
        if(visible&&wasVisible_)remaining_=std::max(0.0,remaining_-std::max(0.0,now-last_));
        last_=now;wasVisible_=visible;
        return serial&&visible&&remaining_>0;
    }
};
}
