#pragma once
#include "BuildInfo.h"
#include "HdrSettings.h"
#include "NativeMemoryReader.h"
#include <MinHook.h>
#include <atomic>
#include <cstdio>

namespace dg::hdr {
// Retail UE 4.22 int registration stores the interface and its two shadow
// values (game +0 / render +4). Set uses the engine's own thread propagation.
namespace engine_profile {
using LogFn=void(*)(const char*);
using BootFn=void(*)();
using SetFn=void(*)(void*,const wchar_t*,unsigned);
inline constexpr uintptr_t OutputVar=0x39CB5B0, GamutVar=0x39CB440, EnabledVar=0x39CB5C8;
inline constexpr uintptr_t IntVtable=0x2822458, IntSetter=0x81E5F0;
inline constexpr unsigned SetByCode=0x08000000;
inline uintptr_t base=0;
inline BootFn original=nullptr;
inline LogFn logger=nullptr;
inline Settings startup{};
inline std::atomic_bool supported{false}, attempted{false}, applied{false};

struct Variable {uintptr_t object=0, values=0;};

inline bool Resolve(uintptr_t slot, Variable& var) {
    native_objects::MemoryReader reader;
    uintptr_t vtable=0;
    return reader.Read(base,slot,var.object) && reader.Read(var.object,0,vtable) &&
        vtable==base+IntVtable && reader.Read(base,slot+8,var.values) && var.values==var.object+0x38;
}

inline EngineProfile Read(bool render) {
    EngineProfile profile{};
    if(!supported.load(std::memory_order_acquire))return profile;
    native_objects::MemoryReader reader;
    Variable output{}, gamut{}, enabled{};
    const size_t offset=render?4:0;
    if(Resolve(OutputVar,output))reader.Read(output.values,offset,profile.outputDevice);
    if(Resolve(GamutVar,gamut))reader.Read(gamut.values,offset,profile.gamut);
    if(Resolve(EnabledVar,enabled))reader.Read(enabled.values,offset,profile.enabled);
    return profile;
}

// The cooked Windows profile wins over both Engine.ini sections in this
// build. Use the same typed int setter and priority as native engine code.
inline void ApplyOnGameThread(bool startupBoundary=false) {
    if(!supported.load(std::memory_order_acquire) || startup.mode<0 || attempted.load(std::memory_order_relaxed))return;
    native_objects::MemoryReader reader;
    Variable output{}, gamut{}, enabled{};
    if(!Resolve(OutputVar,output) || !Resolve(GamutVar,gamut) || !Resolve(EnabledVar,enabled))return;
    const auto before=Read(false);
    unsigned outputFlags=0,gamutFlags=0;
    const bool matchingStartup=before.enabled==(startup.mode>0?1:0);
    if(startupBoundary&&!matchingStartup)return;
    if(attempted.exchange(true))return;
    const bool allowed=reader.Read(output.object,0x18,outputFlags) && reader.Read(gamut.object,0x18,gamutFlags) &&
        (outputFlags&0xFF000000)<=SetByCode && (gamutFlags&0xFF000000)<=SetByCode;
    if(matchingStartup&&allowed){
        const wchar_t value[]={wchar_t(L'0'+(startup.mode>0?startup.mode+2:0)),0};
        const auto set=reinterpret_cast<SetFn>(base+IntSetter);
        set(reinterpret_cast<void*>(output.object),value,SetByCode);
        set(reinterpret_cast<void*>(gamut.object),L"0",SetByCode);
    }
    const auto after=Read(false);
    if(after.Matches(startup.mode))applied.store(true,std::memory_order_release);
    if(logger){
        char line[256]{};
        sprintf_s(line,"HDR %s startup profile=%d written=%d engine enabled=%d output=%d gamut=%d previous=%d/%d",
            build::Version,startup.mode,matchingStartup&&allowed?1:0,after.enabled,after.outputDevice,after.gamut,before.outputDevice,before.gamut);
        logger(line);
    }
}

inline void AfterStartupCVars() {
    original();
    ApplyOnGameThread(true);
}

inline void Initialize(Settings requested, bool exact, LogFn log) {
    if(!exact)return;
    base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    logger=log;
    startup=Normalize(requested);
    constexpr unsigned char setter[]={0x48,0x89,0x6C,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xEC,0x20};
    if(!base || std::memcmp(reinterpret_cast<void*>(base+IntSetter),setter,sizeof(setter)) ||
        *reinterpret_cast<const uintptr_t*>(base+IntVtable+0x60)!=base+IntSetter)return;
    supported.store(true,std::memory_order_release);
    if(startup.mode<0)return;
    constexpr unsigned char boot[]={0x48,0x83,0xEC,0x58,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48,0x8D,0x4C,0x24,0x28};
    auto* entry=reinterpret_cast<void*>(base+0x8BB4A0);
    if(std::memcmp(entry,boot,sizeof(boot)))return;
    const auto result=MH_Initialize();
    if(result!=MH_OK&&result!=MH_ERROR_ALREADY_INITIALIZED)return;
    if(MH_CreateHook(entry,reinterpret_cast<void*>(&AfterStartupCVars),reinterpret_cast<void**>(&original))!=MH_OK)return;
    if(MH_EnableHook(entry)!=MH_OK){MH_RemoveHook(entry);original=nullptr;}
}
}
}
