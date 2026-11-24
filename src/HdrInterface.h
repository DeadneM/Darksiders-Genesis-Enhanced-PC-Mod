#pragma once
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <intrin.h>
#include <windows.h>
#include <cstring>

namespace dg::hdr::ui {
using LogFn=void(*)(const char*);
using Upload=void(*)(void*,void*,const void*,const void*,unsigned);
inline Upload original=nullptr;
inline uintptr_t base=0;
inline std::atomic_bool blendSeen{false};

// Both Slate HDR composite variants copy the UILevel scalar into the
// command list here. Leave all other shader parameters to the engine.
inline void UploadUiLevel(void* commands,void* shader,const void* parameter,const void* value,unsigned index){
    const auto caller=reinterpret_cast<uintptr_t>(_ReturnAddress());
    if(index==0&&(caller==base+0x1472755||caller==base+0x1472CB5)){
        if(!blendSeen.load(std::memory_order_relaxed))blendSeen.store(true,std::memory_order_relaxed);
        const float level=runtime::Get().hdrUiBrightness.load(std::memory_order_relaxed);
        if(level>=0){original(commands,shader,parameter,&level,index);return;}
    }
    original(commands,shader,parameter,value,index);
}

inline bool Initialize(bool exact,int hdrMode,LogFn log){
    if(!exact||hdrMode<=0)return false;
    base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if(!base)return false;
    constexpr unsigned char uploadSig[]={0x48,0x89,0x54,0x24,0x10,0x55,0x57,0x41,0x54,0x48,0x83,0xEC,0x30,0x8B,0x44,0x24,0x70};
    constexpr unsigned char copySig[]={0x44,0x8B,0xC7,0x49,0x8B,0xD4,0x48,0x8B,0xCB,0x48,0x89,0x45,0x30,0xE8,0xA4,0xE5,0x73,0x01};
    constexpr unsigned char levelA[]={0x48,0x8B,0x05,0x45,0x6D,0x5B,0x02,0xF3,0x0F,0x10,0x40,0x04};
    constexpr unsigned char levelB[]={0x48,0x8B,0x05,0xE5,0x67,0x5B,0x02,0xF3,0x0F,0x10,0x40,0x04};
    constexpr unsigned char uploadA[]={0x4C,0x8D,0x4C,0x24,0x70,0x48,0x8B,0x56,0x60,0x48,0x8B,0xCB,0xE8,0xAB,0xA6,0x8B,0xFF};
    constexpr unsigned char uploadB[]={0x4C,0x8D,0x4C,0x24,0x70,0x48,0x8B,0x56,0x60,0x48,0x8B,0xCB,0xE8,0x4B,0xA1,0x8B,0xFF};
    auto* entry=reinterpret_cast<void*>(base+0xD2CE00);
    if(std::memcmp(entry,uploadSig,sizeof(uploadSig))||
       std::memcmp(reinterpret_cast<void*>(base+0xD2CE8D),copySig,sizeof(copySig))||
       std::memcmp(reinterpret_cast<void*>(base+0x1472714),levelA,sizeof(levelA))||
       std::memcmp(reinterpret_cast<void*>(base+0x1472C74),levelB,sizeof(levelB))||
       std::memcmp(reinterpret_cast<void*>(base+0x1472744),uploadA,sizeof(uploadA))||
       std::memcmp(reinterpret_cast<void*>(base+0x1472CA4),uploadB,sizeof(uploadB)))return false;
    const auto state=MH_Initialize();
    if(state!=MH_OK&&state!=MH_ERROR_ALREADY_INITIALIZED)return false;
    if(MH_CreateHook(entry,reinterpret_cast<void*>(&UploadUiLevel),reinterpret_cast<void**>(&original))!=MH_OK)return false;
    if(MH_EnableHook(entry)!=MH_OK){MH_RemoveHook(entry);original=nullptr;return false;}
    if(log)log("HDR UI Level: live Slate HDR composite parameter ready");
    return true;
}
}
