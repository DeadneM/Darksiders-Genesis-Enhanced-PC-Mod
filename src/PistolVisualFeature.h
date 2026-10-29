#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::pistol_visual {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
void OnLivePlayer(const local_players::Context& owner,uint32_t generation,bool focused);
bool SkipParticle(void* notify,void* mesh);
void OnMeshAssigned(void* liveComponent);
bool Ready();
bool PoseReady(unsigned player);
void Shutdown();
}
