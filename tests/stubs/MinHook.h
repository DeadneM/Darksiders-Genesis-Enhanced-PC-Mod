#pragma once
// Hooks are never installed in the isolated native-boundary tests.
using MH_STATUS=int;
constexpr MH_STATUS MH_OK=0,MH_ERROR_ALREADY_INITIALIZED=1,MH_ERROR_ENABLED=2;
inline MH_STATUS MH_Initialize(){return MH_OK;}
inline MH_STATUS MH_CreateHook(void*,void*,void**){return -1;}
inline MH_STATUS MH_EnableHook(void*){return -1;}
inline MH_STATUS MH_DisableHook(void*){return MH_OK;}
inline MH_STATUS MH_RemoveHook(void*){return MH_OK;}
