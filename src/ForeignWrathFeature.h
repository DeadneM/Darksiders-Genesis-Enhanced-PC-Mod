#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::foreign_wrath {
struct Name {int index=-1,number=0;bool operator==(const Name&)const=default;};
using ResolveFn=uintptr_t*(*)(void*,uintptr_t*,Name,bool*,bool*);
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
// Requests one bounded loaded-asset pass. 0=unavailable, 1=preparing, 2=ready.
int Status(int choice=6);
void Observe(const local_players::Context& owner);
bool Prepare(void* actor,uintptr_t& abilityClass,int choice=6);
uintptr_t* Resolve(ResolveFn native,void* actor,uintptr_t* result,Name id,bool* unlocked,bool* affordable,uintptr_t abilityClass);
// Temporary V1.91 read-only Geyser trace on the existing dispatch boundary.
// Always forwards the complete native call exactly once when supplied.
using DispatchFn=void(*)(void*,void*,void*,void*);
void DispatchEvent(DispatchFn native,void* object,void* function,void* parameters,void* outParameters);
void Shutdown();
}
