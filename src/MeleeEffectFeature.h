#pragma once
namespace dg::melee_effect {
using LogFn=void(*)(const char*);
using NotifyFn=void(*)(void*,void*,void*);
void Initialize(LogFn log,bool exactExecutable);
void Particle(void* notify,void* mesh,void* animation,NotifyFn original);
void Shutdown();
}
