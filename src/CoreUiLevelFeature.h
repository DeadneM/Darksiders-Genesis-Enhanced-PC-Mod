#pragma once
#include "NativeObjectIdentity.h"
namespace dg::core_ui {
using LogFn=void(*)(const char*);
using Dispatch=void(*)(void*,void*,void*,void*);
void Initialize(LogFn log,bool exactExecutable,bool observerInstalled);
bool Ready();
native_objects::Identity CaptureTick(void* object,void* function);
void AfterTick(const native_objects::Identity& widget,Dispatch nativeDispatch);
}
