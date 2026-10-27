#pragma once
#include <cstdint>

namespace dg::foreign_montage {
using LogFn=void(*)(const char*);
struct Source {uintptr_t sequence=0,skeleton=0;int slotIndex=0,slotNumber=0,rootIndex=0,rootNumber=0,spineIndex=0,spineNumber=0;};
void Initialize(LogFn,bool exactExecutable);
bool Ready();
// Private foreign montages only: preserve longer authored fades.
bool SmoothExit(void* privateMontage);
bool CanBuild(const Source&);
// Reuse the existing bounded object scan; no loader, timer or source edits.
void ObserveSequence(uintptr_t object);
bool GesturesReady();
uintptr_t CastingSequence(uintptr_t fallback,uintptr_t skeleton,float& length);
// Private War Clone casts only; append the native grounded return.
// Missing return leaves the previous valid cast intact.
bool CompleteGroundCast(void* privateMontage,uintptr_t skeleton,float returnSeconds);
// NewObject with the source as template: only private, non-aliased native
// arrays are edited. The engine owns allocation, notify timing and cleanup.
void* Geyser(uintptr_t nativeMontage,const Source&);
// Retained Clone transfer: native callback timeline,
// compatible pose, and root-only presentation. Weapon sockets are excluded.
void* Compatible(uintptr_t nativeMontage,const Source&);
// War-owned World Ender: finite native beam timeline and cancellation states.
void* WorldEnder(uintptr_t nativeMontage,const Source&);
// Root motion uses the first track, on a slot absent from the live graph.
void* RampageMotion(uintptr_t nativeMontage,const Source&);
// Successful foreign casts only: native opening audio once on the receiver's
// own mesh. Later sounds on dynamic gestures remain engine-timed notifies.
void StartSounds(uintptr_t nativeMontage,void* liveAbility);
bool CopyLaterSounds(uintptr_t nativeMontage,void* privateMontage);
// Strife Stoneskin only: copy the two native, unattached launch particles.
// Status, Stinger, blast, surface effects and camera notifies stay native.
bool CopyStoneskinLaunchEffects(uintptr_t nativeMontage,void* privateMontage);
}
