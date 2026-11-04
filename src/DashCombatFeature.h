#pragma once
namespace dg::local_players {struct Context;}
namespace dg::dash_combat {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
void Shutdown();
// Existing live player-camera callback, also available when no native aim
// ability remains to call a look getter. Independent of the camera mode.
void OnLivePlayer(void* actor);
void ObserveAirLanding(const local_players::Context& owner);
bool SuppressAirLandingEvent(void* ability,void* function,void* parameters);
void Report();
bool PresentationReady();
}
