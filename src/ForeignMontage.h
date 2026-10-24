#pragma once
#include <cstdint>

namespace dg::foreign_montage {
using LogFn=void(*)(const char*);
struct Source {uintptr_t sequence=0,skeleton=0;int slotIndex=0,slotNumber=0,rootIndex=0,rootNumber=0;};
void Initialize(LogFn,bool exactExecutable);
bool Ready();
bool CanBuild(const Source&);
// NewObject with the source as template: only private, non-aliased native
// arrays are edited. The engine owns allocation, notify timing and cleanup.
void* Geyser(uintptr_t nativeMontage,const Source&);
// Remaining experimental powers: native callback/blast timeline,
// compatible pose, and root-only presentation. Weapon sockets are excluded.
void* Compatible(uintptr_t nativeMontage,const Source&);
}
