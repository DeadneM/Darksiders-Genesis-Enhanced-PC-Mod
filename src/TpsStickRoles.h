#pragma once
#include <utility>

namespace dg::tps_input {
// Physical camera selection is independent of the game's native look/move
// swap. Convert once into the input layout the native game will consume.
template<class Pad>
void MapGameplayRoles(Pad& pad, bool cameraLeft, bool nativeSwap) {
    if (cameraLeft != nativeSwap) {
        std::swap(pad.sThumbLX,pad.sThumbRX);
        std::swap(pad.sThumbLY,pad.sThumbRY);
    }
}
template<class Pad>
void SetNativeLook(Pad& pad, bool nativeSwap, short x, short y) {
    if (nativeSwap) { pad.sThumbLX=x; pad.sThumbLY=y; }
    else { pad.sThumbRX=x; pad.sThumbRY=y; }
}
template<class Pad>
int CameraX(const Pad& pad, bool cameraLeft) {
    return cameraLeft?pad.sThumbLX:pad.sThumbRX;
}
template<class Pad>
int CameraY(const Pad& pad, bool cameraLeft) {
    return cameraLeft?pad.sThumbLY:pad.sThumbRY;
}
}
