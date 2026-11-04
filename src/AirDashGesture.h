#pragma once
#include <cstdint>
namespace dg::air_dash {
enum class Gesture { Native, Swallow, Down, Forward };
struct Press {
    static constexpr uint64_t HoldMilliseconds=180;
    bool down=false,pending=false,sent=false;
    uint64_t since=0;
    Gesture Begin(uint64_t now,bool eligible){
        if(down)return Gesture::Swallow;
        if(!eligible)return Gesture::Native;
        down=pending=true;sent=false;since=now;return Gesture::Swallow;
    }
    Gesture Update(uint64_t now,bool allowed){
        if(!pending)return Gesture::Native;
        if(!allowed||now<since){*this={};return Gesture::Swallow;}
        if(now-since<HoldMilliseconds)return Gesture::Swallow;
        pending=false;sent=true;return Gesture::Forward;
    }
    Gesture Release(uint64_t now,bool allowed){
        if(!down)return Gesture::Native;
        down=false;
        if(!pending)return sent?Gesture::Native:Gesture::Swallow;
        pending=false;
        if(!allowed||now<since)return Gesture::Swallow;
        sent=true;return now-since<HoldMilliseconds?Gesture::Down:Gesture::Forward;
    }
};
}
