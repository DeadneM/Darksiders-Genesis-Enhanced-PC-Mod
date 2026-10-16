#pragma once
#include <cstdint>
namespace dg::camera_continuity {
struct Identity {uintptr_t local=0,world=0,character=0;unsigned char type=255;};
// Retail MayhemCharacterType: Strife=1, War=2. Zero is not War.
// Both values were captured across reciprocal LocalPlayers[0] swaps.
inline bool CharacterSwap(const Identity& before,const Identity& after){
    return before.local&&before.local==after.local&&before.world&&before.world==after.world&&
        before.character&&after.character&&before.character!=after.character&&
        (before.type==1||before.type==2)&&(after.type==1||after.type==2)&&before.type!=after.type;
}
enum class ReferenceChange { Unchanged, Reset, KeepCharacterSwap };
struct ReferenceTracker {
    Identity identity{};
    uint32_t generation=UINT32_MAX;
    ReferenceChange Observe(const Identity* owner,uint32_t liveGeneration,bool ownershipRequired){
        // During a native character blend the old camera can run before the
        // new owned camera. It must not consume the new player's generation.
        if(!owner&&ownershipRequired)return ReferenceChange::Unchanged;
        if(generation==liveGeneration){
            if(owner&&!identity.local)identity=*owner;
            return ReferenceChange::Unchanged;
        }
        const bool keep=owner&&CharacterSwap(identity,*owner);
        generation=liveGeneration;identity=owner?*owner:Identity{};
        return keep?ReferenceChange::KeepCharacterSwap:ReferenceChange::Reset;
    }
};
}
