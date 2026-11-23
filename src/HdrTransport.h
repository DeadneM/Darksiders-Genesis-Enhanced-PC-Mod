#pragma once
#include "HdrEngineProfile.h"
#include <dxgi1_6.h>
namespace dg::hdr {
// Only the engine creates/resizes its buffers. One supported handshake per
// chain/resize, with no vendor condition or per-frame COM calls.
struct TransportStatus {
    bool attempted=false,applied=false,formatMismatch=false,engineMismatch=false;
    unsigned format=0;
    HRESULT result=S_FALSE,metadata=S_FALSE;
};
inline TransportStatus transport{};inline IDXGISwapChain* attemptedChain=nullptr;
inline std::atomic_uint resizeGeneration{0};
inline unsigned attemptedGeneration=0;
inline ULONGLONG engineRetryAt=0;
inline bool retriedAfterEngineApply=false;
inline void InvalidateTransport(){resizeGeneration.fetch_add(1,std::memory_order_release);}

inline bool ServiceTransport(IDXGISwapChain* chain){
    if(!chain||!engine_profile::supported.load(std::memory_order_acquire))return false;
    const auto& startup=engine_profile::startup;
    if(startup.mode<=0||!startup.dxgiSignaling)return false;
    const unsigned generation=resizeGeneration.load(std::memory_order_acquire);
    const bool same=attemptedChain==chain&&attemptedGeneration==generation;
    if(same){
        // A late attach can fix the game-side values after the first Present.
        // Let the render-thread copies catch up before the single retry.
        if(!transport.engineMismatch||retriedAfterEngineApply||!engine_profile::applied.load(std::memory_order_acquire))return false;
        const auto now=GetTickCount64();
        if(!engineRetryAt){engineRetryAt=now+250;return false;}
        if(now<engineRetryAt)return false;
        retriedAfterEngineApply=true;
    }
    DXGI_SWAP_CHAIN_DESC d{};if(FAILED(chain->GetDesc(&d))||!d.OutputWindow)return false;
    wchar_t windowClass[64]{};DWORD pid=0;GetWindowThreadProcessId(d.OutputWindow,&pid);
    if(pid!=GetCurrentProcessId()||!GetClassNameW(d.OutputWindow,windowClass,64)||wcscmp(windowClass,L"UnrealWindow"))return false;
    if(!same){engineRetryAt=0;retriedAfterEngineApply=false;}
    attemptedGeneration=generation;
    attemptedChain=chain;transport={};transport.attempted=true;transport.format=unsigned(d.BufferDesc.Format);
    const bool pq=PQ(startup.mode);
    if(d.BufferDesc.Format!=(pq?DXGI_FORMAT_R10G10B10A2_UNORM:DXGI_FORMAT_R16G16B16A16_FLOAT)){
        transport.formatMismatch=true;transport.result=DXGI_ERROR_UNSUPPORTED;return true;
    }
    // Tag only pixels that the engine is actually rendering in this profile.
    if(!engine_profile::Read(true).Matches(startup.mode)){
        transport.engineMismatch=true;transport.result=S_FALSE;return true;
    }
    IDXGISwapChain3* modern=nullptr;
    transport.result=chain->QueryInterface(__uuidof(IDXGISwapChain3),reinterpret_cast<void**>(&modern));if(FAILED(transport.result))return true;
    const auto color=pq?DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020:DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
    UINT support=0;transport.result=modern->CheckColorSpaceSupport(color,&support);
    if(SUCCEEDED(transport.result)&&!(support&DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))transport.result=DXGI_ERROR_UNSUPPORTED;
    if(SUCCEEDED(transport.result))transport.result=modern->SetColorSpace1(color);
    modern->Release();transport.applied=SUCCEEDED(transport.result);
    if(!pq||!transport.applied)return true;
    IDXGISwapChain4* metadata=nullptr;
    transport.metadata=chain->QueryInterface(__uuidof(IDXGISwapChain4),reinterpret_cast<void**>(&metadata));if(FAILED(transport.metadata))return true;
    DXGI_HDR_METADATA_HDR10 m{};
    m.RedPrimary[0]=35400;m.RedPrimary[1]=14600;m.GreenPrimary[0]=8500;m.GreenPrimary[1]=39850;
    m.BluePrimary[0]=6550;m.BluePrimary[1]=2300;m.WhitePoint[0]=15635;m.WhitePoint[1]=16450;
    const float profile=startup.mode==2?2000.0f:1000.0f;
    m.MaxMasteringLuminance=UINT(startup.customMetadata?startup.peakNits:profile);
    m.MinMasteringLuminance=UINT((startup.customMetadata?startup.minNits:.001f)*10000.0f);
    m.MaxContentLightLevel=UINT16(startup.customMetadata?startup.maxCLL:profile);
    m.MaxFrameAverageLightLevel=UINT16(startup.customMetadata?startup.maxFALL:400);
    transport.metadata=metadata->SetHDRMetaData(DXGI_HDR_METADATA_TYPE_HDR10,sizeof(m),&m);metadata->Release();
    return true;
}
}
