#pragma once
#include "LocalPlayerOwnership.h"
#include "NativeBonePoint.h"
namespace dg::shield_visual {
using LogFn=void(*)(const char*);
using VisibleFn=void(*)(void*,bool,unsigned char);
void Initialize(uintptr_t base,VisibleFn,LogFn);
bool CanTrack();
void ReleaseReused(void* component);
void Track(void* component,void* original,void* rendered,void* world);
void OnLivePlayer(const local_players::Context&);
bool AdjustAttached(void* asset,void* mesh,native_bone::Name socket,const native_bone::Point* original,native_bone::Point& output);
void Shutdown();
}
