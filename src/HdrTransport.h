#pragma once
#include "HdrNativeViewport.h"
namespace dg::hdr {
// One pair of format and encoding checks per surface/resize, with no
// vendor restriction or repeated COM calls while the surface stays active.
struct TransportStatus {
    bool attempted=false,applied=false,formatMismatch=false,engineMismatch=false;
    bool hdrDisplay=false,presentationMismatch=false;
    unsigned format=0,swapEffect=0;
    int renderBefore=-1,renderAfter=-1;
    HRESULT result=S_FALSE,metadata=S_FALSE;
};
inline TransportStatus transport{};inline IDXGISwapChain* attemptedChain=nullptr;

inline unsigned attemptedGeneration=0;
inline ULONGLONG engineRetryAt=0;
inline bool retriedAfterEngineApply=false;
inline void InvalidateTransport(){viewport::generation.fetch_add(1,std::memory_order_release);}

inline HRESULT ApplyHDR10Metadata(IDXGISwapChain* chain,const Settings& settings){
    Microsoft::WRL::ComPtr<IDXGISwapChain4> output;
    const auto hr=chain->QueryInterface(IID_PPV_ARGS(output.GetAddressOf()));
    if(FAILED(hr))return hr;
    DXGI_HDR_METADATA_HDR10 m{};
    m.RedPrimary[0]=35400;m.RedPrimary[1]=14600;m.GreenPrimary[0]=8500;m.GreenPrimary[1]=39850;
    m.BluePrimary[0]=6550;m.BluePrimary[1]=2300;m.WhitePoint[0]=15635;m.WhitePoint[1]=16450;
    const float profile=settings.mode==2?2000.0f:1000.0f;
    m.MaxMasteringLuminance=UINT(settings.customMetadata?settings.peakNits:profile);
    m.MinMasteringLuminance=UINT((settings.customMetadata?settings.minNits:.001f)*10000.0f);
    m.MaxContentLightLevel=UINT16(settings.customMetadata?settings.maxCLL:profile);
    m.MaxFrameAverageLightLevel=UINT16(settings.customMetadata?settings.maxFALL:400);
    return output->SetHDRMetaData(DXGI_HDR_METADATA_TYPE_HDR10,sizeof(m),&m);
}

inline bool ServiceTransport(IDXGISwapChain* chain){
    if(!chain||!engine_profile::supported.load(std::memory_order_acquire))return false;
    const auto& startup=engine_profile::startup;
    if(startup.mode<=0||!CompatibleBuffer(startup)||!startup.dxgiSignaling)return false;
    const unsigned generation=viewport::generation.load(std::memory_order_acquire);
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
    const auto expectedFormat=pq?DXGI_FORMAT_R10G10B10A2_UNORM:DXGI_FORMAT_R16G16B16A16_FLOAT;
    transport.swapEffect=unsigned(d.SwapEffect);
    transport.renderBefore=transport.renderAfter=engine_profile::ReadRenderCapability();
    if(d.BufferDesc.Format!=expectedFormat){
        transport.formatMismatch=true;transport.result=DXGI_ERROR_UNSUPPORTED;return true;
    }
    if(pq&&!viewport::OwnsFlip(chain)){
        transport.presentationMismatch=true;transport.result=DXGI_ERROR_UNSUPPORTED;return true;
    }
    // Require the actual engine encoding before announcing it.
    if(!engine_profile::Read(true).Matches(startup.mode)){
        transport.engineMismatch=true;transport.result=S_FALSE;return true;
    }
    const auto color=pq?DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020:DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
    transport.result=viewport::SetColorSpace(chain,color);
    transport.applied=SUCCEEDED(transport.result);
    // The UE4 DX11 probe can miss HDR on current hardware. Authorize the
    // renderer only after the actual game output and DXGI handshake agree.
    if(transport.applied){
        Microsoft::WRL::ComPtr<ID3D11Device> device;
        transport.hdrDisplay=SUCCEEDED(chain->GetDevice(IID_PPV_ARGS(device.GetAddressOf())))&&viewport::HasHdrDisplay(chain);
        if(transport.hdrDisplay&&engine_profile::EnableRenderCapability())transport.renderAfter=1;
        if(pq)transport.metadata=ApplyHDR10Metadata(chain,startup);
    }
    return true;
}
}
