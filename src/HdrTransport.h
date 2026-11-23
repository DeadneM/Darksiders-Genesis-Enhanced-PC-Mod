#pragma once
#include "HdrEngineProfile.h"
#include <d3d11.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
namespace dg::hdr {
// Only the engine creates/resizes its buffers. One supported handshake per
// chain/resize, with no vendor condition or per-frame COM calls.
struct TransportStatus {
    bool attempted=false,applied=false,formatMismatch=false,engineMismatch=false;
    bool hdrDisplay=false;
    unsigned format=0,swapEffect=0;
    int renderBefore=-1,renderAfter=-1;
    HRESULT result=S_FALSE;
};
inline TransportStatus transport{};inline IDXGISwapChain* attemptedChain=nullptr;
inline std::atomic_uint resizeGeneration{0};
inline unsigned attemptedGeneration=0;
inline ULONGLONG engineRetryAt=0;
inline bool retriedAfterEngineApply=false;
inline void InvalidateTransport(){resizeGeneration.fetch_add(1,std::memory_order_release);}

inline bool HasActiveHdrDisplay(IDXGISwapChain* chain) {
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    if(FAILED(chain->GetDevice(IID_PPV_ARGS(device.GetAddressOf()))))return false;
    Microsoft::WRL::ComPtr<IDXGIOutput> output;
    if(FAILED(chain->GetContainingOutput(&output)))return false;
    Microsoft::WRL::ComPtr<IDXGIOutput6> advanced;
    DXGI_OUTPUT_DESC1 desc{};
    return SUCCEEDED(output.As(&advanced))&&SUCCEEDED(advanced->GetDesc1(&desc))&&
        desc.AttachedToDesktop&&desc.ColorSpace==DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
}

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
    if(d.BufferDesc.Format!=DXGI_FORMAT_R16G16B16A16_FLOAT){
        transport.formatMismatch=true;transport.result=DXGI_ERROR_UNSUPPORTED;return true;
    }
    transport.swapEffect=unsigned(d.SwapEffect);
    transport.renderBefore=transport.renderAfter=engine_profile::ReadRenderCapability();
    // Require the engine's scRGB profile before announcing its encoding.
    if(!engine_profile::Read(true).Matches(startup.mode)){
        transport.engineMismatch=true;transport.result=S_FALSE;return true;
    }
    IDXGISwapChain3* modern=nullptr;
    transport.result=chain->QueryInterface(__uuidof(IDXGISwapChain3),reinterpret_cast<void**>(&modern));if(FAILED(transport.result))return true;
    constexpr auto color=DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
    UINT support=0;transport.result=modern->CheckColorSpaceSupport(color,&support);
    if(SUCCEEDED(transport.result)&&!(support&DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))transport.result=DXGI_ERROR_UNSUPPORTED;
    if(SUCCEEDED(transport.result))transport.result=modern->SetColorSpace1(color);
    modern->Release();transport.applied=SUCCEEDED(transport.result);
    // The UE4 DX11 probe can miss HDR on current hardware. Authorize the
    // renderer only after the actual game output and DXGI handshake agree.
    if(transport.applied){
        transport.hdrDisplay=HasActiveHdrDisplay(chain);
        if(transport.hdrDisplay&&engine_profile::EnableRenderCapability())transport.renderAfter=1;
    }
    return true;
}
}
