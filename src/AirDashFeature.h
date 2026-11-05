#pragma once
#include <cstdint>
namespace dg::local_players {struct Context;}
namespace dg::air_dash {
using LogFn=void(*)(const char*);
using Dispatch=void(*)(void*,unsigned char);
void Initialize(LogFn,bool exactExecutable,bool finishBoundaryReady);
void OnLivePlayer(const local_players::Context&,uint32_t generation,bool gameplayAllowed);
// Called by the EXISTING native action and ability-tick detours only.
bool ProcessAction(Dispatch,void* actionInterface,unsigned char action);
void BeforeTick(void* liveAbility);
void AfterTick(void* liveAbility);
// Private parameter copy for this gesture's completion only; never edits
// the caller's argument buffer or the Blueprint execution cursor.
bool FinishParameters(void* liveAbility,void* function,void* parameters,unsigned char& cause);
void Shutdown();
}
