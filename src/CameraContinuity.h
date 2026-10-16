#pragma once
#include <cstdint>
namespace dg::camera_continuity {
struct Identity {uintptr_t local=0,world=0,character=0;unsigned char type=255;};
inline bool CharacterSwap(const Identity& before,const Identity& after){
    return before.local&&before.local==after.local&&before.world&&before.world==after.world&&
        before.character&&after.character&&before.character!=after.character&&
        before.type<=1&&after.type<=1&&before.type!=after.type;
}
}
