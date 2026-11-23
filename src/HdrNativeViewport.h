#pragma once
#include "HdrEngineProfile.h"
#include <d3d11.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <intrin.h>

namespace dg::hdr::viewport {
using Microsoft::WRL::ComPtr;
using Constructor=void*(*)(void*,void*,HWND,UINT,UINT,bool,int);
using CreateChain=HRESULT(STDMETHODCALLTYPE*)(IDXGIFactory*,IUnknown*,DXGI_SWAP_CHAIN_DESC*,IDXGISwapChain**);
using Reset=void(*)(void*,bool);

inline uintptr_t base=0;
inline engine_profile::LogFn logger=nullptr;
inline Constructor construct=nullptr;
inline CreateChain createChain=nullptr;
inline Reset reset=nullptr;
inline volatile long* hdrFormat=nullptr;
inline std::atomic<IDXGISwapChain*> flipChain{nullptr};
inline std::atomic_uint generation{0};
inline constexpr long FloatRGBA=10, RGB10A2=18;

inline bool OwnsFlip(IDXGISwapChain* chain){
    return chain&&chain==flipChain.load(std::memory_order_acquire);
}

inline bool GameWindow(HWND window){
    if(!window)return false;
    DWORD pid=0;wchar_t name[32]{};
    GetWindowThreadProcessId(window,&pid);
    return pid==GetCurrentProcessId()&&GetClassNameW(window,name,32)&&wcscmp(name,L"UnrealWindow")==0;
}

inline bool SelectRgb10(){
    if(!hdrFormat)return false;
    const auto prior=_InterlockedCompareExchange(hdrFormat,RGB10A2,FloatRGBA);
    return prior==FloatRGBA||prior==RGB10A2;
}

// Select the engine's own EPixelFormat before FSceneViewport caches it.
// D3D11's format table maps PF_A2B10G10R10 to DXGI RGB10A2.
inline volatile long* VerifyHdrFormat(){
    constexpr unsigned char sceneCreate[]={0x8B,0x05,0xEA,0xAC,0x21,0x02,0x89,0x81,0xCC,0,0,0};
    constexpr unsigned char sceneResize[]={0x8B,0x05,0xCB,0xEA,0x21,0x02,0x75,0x06};
    constexpr unsigned char rgb10Map[]={0xC7,0x05,0x1E,0x9F,0x93,0x02,0x18,0,0,0};
    if(std::memcmp(reinterpret_cast<void*>(base+0x145D248),sceneCreate,sizeof(sceneCreate))||
       std::memcmp(reinterpret_cast<void*>(base+0x1459467),sceneResize,sizeof(sceneResize))||
       std::memcmp(reinterpret_cast<void*>(base+0xD3E924),rgb10Map,sizeof(rgb10Map)))return nullptr;
    auto* address=reinterpret_cast<long*>(base+0x3677F38);
    MEMORY_BASIC_INFORMATION region{};
    if(!VirtualQuery(address,&region,sizeof(region))||region.State!=MEM_COMMIT||
       region.Type!=MEM_IMAGE||region.AllocationBase!=reinterpret_cast<void*>(base)||
       (region.Protect!=PAGE_READWRITE&&region.Protect!=PAGE_WRITECOPY))return nullptr;
    return address;
}

inline bool HasHdrDisplay(IDXGISwapChain* chain){
    ComPtr<IDXGIOutput> output;
    ComPtr<IDXGIOutput6> advanced;
    DXGI_OUTPUT_DESC1 desc{};
    return SUCCEEDED(chain->GetContainingOutput(&output))&&SUCCEEDED(output.As(&advanced))&&
        SUCCEEDED(advanced->GetDesc1(&desc))&&desc.AttachedToDesktop&&
        desc.ColorSpace==DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
}

inline HRESULT SetColorSpace(IDXGISwapChain* chain,DXGI_COLOR_SPACE_TYPE color){
    ComPtr<IDXGISwapChain3> modern;
    auto hr=chain->QueryInterface(IID_PPV_ARGS(modern.GetAddressOf()));
    if(FAILED(hr))return hr;
    UINT support=0;
    hr=modern->CheckColorSpaceSupport(color,&support);
    if(FAILED(hr))return hr;
    if(!(support&DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))return DXGI_ERROR_UNSUPPORTED;
    return modern->SetColorSpace1(color);
}

inline HRESULT STDMETHODCALLTYPE CreateHdrChain(IDXGIFactory* factory,IUnknown* nativeDevice,
                                                DXGI_SWAP_CHAIN_DESC* native,IDXGISwapChain** out){
    if(_ReturnAddress()!=reinterpret_cast<void*>(base+0xD3F8DF)||
       !native||!nativeDevice||!out||!GameWindow(native->OutputWindow)||
       native->BufferDesc.Format!=DXGI_FORMAT_R10G10B10A2_UNORM||
       native->SampleDesc.Count!=1||native->SampleDesc.Quality!=0)
        return createChain(factory,nativeDevice,native,out);

    ComPtr<IDXGIFactory2> modernFactory;
    ComPtr<ID3D11Device> device;
    auto hr=factory->QueryInterface(IID_PPV_ARGS(modernFactory.GetAddressOf()));
    if(SUCCEEDED(hr))hr=nativeDevice->QueryInterface(IID_PPV_ARGS(device.GetAddressOf()));
    if(SUCCEEDED(hr)){
        DXGI_SWAP_CHAIN_DESC1 desc{};
        desc.Width=native->BufferDesc.Width;
        desc.Height=native->BufferDesc.Height;
        desc.Format=native->BufferDesc.Format;
        desc.SampleDesc=native->SampleDesc;
        desc.BufferUsage=native->BufferUsage;
        desc.BufferCount=std::max(native->BufferCount,2u);
        desc.Scaling=DXGI_SCALING_STRETCH;
        desc.SwapEffect=DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.AlphaMode=DXGI_ALPHA_MODE_IGNORE;
        desc.Flags=native->Flags;
        const DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreen{
            native->BufferDesc.RefreshRate,native->BufferDesc.ScanlineOrdering,native->BufferDesc.Scaling,native->Windowed
        };
        ComPtr<IDXGISwapChain1> created;
        hr=modernFactory->CreateSwapChainForHwnd(device.Get(),native->OutputWindow,&desc,&fullscreen,nullptr,&created);
        if(SUCCEEDED(hr)){
            hr=HasHdrDisplay(created.Get())?SetColorSpace(created.Get(),DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020):DXGI_ERROR_UNSUPPORTED;
            if(SUCCEEDED(hr)){
                // IDXGISwapChain1 extends IDXGISwapChain without a separate identity.
                *out=created.Detach();
                flipChain.store(*out,std::memory_order_release);
                if(logger)logger("HDR10: RGB10A2 flip swap chain created and PQ accepted");
                return S_OK;
            }
            if(!native->Windowed)created->SetFullscreenState(FALSE,nullptr);
        }
    }
    if(logger){
        char line[120]{};
        sprintf_s(line,"HDR10: flip/PQ unavailable (0x%08lX); using the game's swap chain",
            static_cast<unsigned long>(hr));
        logger(line);
    }
    return createChain(factory,nativeDevice,native,out);
}

inline void HookFactory(void* rhi){
    if(createChain)return;
    native_objects::MemoryReader reader;
    uintptr_t factory=0,table=0,entry=0;
    if(!reader.Read(reinterpret_cast<uintptr_t>(rhi),0x180,factory)||!reader.Read(factory,0,table)||
       !reader.Read(table,0x50,entry)||!entry)return;
    auto* fn=reinterpret_cast<void*>(entry);
    if(MH_CreateHook(fn,reinterpret_cast<void*>(&CreateHdrChain),reinterpret_cast<void**>(&createChain))!=MH_OK)return;
    if(MH_EnableHook(fn)!=MH_OK){MH_RemoveHook(fn);createChain=nullptr;}
}

inline void* CreateViewport(void* viewport,void* rhi,HWND window,UINT width,UINT height,bool fullscreen,int format){
    const bool game=GameWindow(window);
    if(game){
        flipChain.store(nullptr,std::memory_order_release);
        if(format==RGB10A2)HookFactory(rhi);
    }
    auto* result=construct(viewport,rhi,window,width,height,fullscreen,format);
    if(game){
        IDXGISwapChain* chain=nullptr;
        native_objects::MemoryReader reader;
        engine_profile::SetPqReady(reader.Read(reinterpret_cast<uintptr_t>(result),0x60,chain)&&OwnsFlip(chain));
        generation.fetch_add(1,std::memory_order_release);
    }
    return result;
}

// D3D11's viewport resize releases its back buffer, sets the mode, then
// rebuilds the RHI target. Flip swap chains need a resize in that same window
// immediately after a successful fullscreen transition.
inline void ResetViewport(void* viewport,bool ignoreFocus){
    const auto caller=reinterpret_cast<uintptr_t>(_ReturnAddress());
    native_objects::MemoryReader reader;
    const auto object=reinterpret_cast<uintptr_t>(viewport);
    IDXGISwapChain* chain=nullptr;
    uintptr_t backBuffer=0;
    BOOL before=FALSE;
    const bool nativeResize=(caller==base+0xD60E61||caller==base+0xD60E97)&&
        reader.Read(object,0x60,chain)&&OwnsFlip(chain)&&reader.Read(object,0x68,backBuffer)&&!backBuffer&&
        SUCCEEDED(chain->GetFullscreenState(&before,nullptr));

    reset(viewport,ignoreFocus);

    BOOL after=FALSE;DXGI_SWAP_CHAIN_DESC desc{};
    if(nativeResize&&SUCCEEDED(chain->GetFullscreenState(&after,nullptr))&&before!=after&&SUCCEEDED(chain->GetDesc(&desc))){
        const auto hr=chain->ResizeBuffers(0,0,0,DXGI_FORMAT_UNKNOWN,desc.Flags);
        if(FAILED(hr)&&logger){
            char line[128]{};
            sprintf_s(line,"HDR10: post-transition ResizeBuffers failed (0x%08lX)",static_cast<unsigned long>(hr));
            logger(line);
        }
    }
}

inline bool Initialize(Settings choice,bool exact,engine_profile::LogFn log){
    if(!exact||!ManagedPq(choice))return false;
    base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if(!base)return false;
    logger=log;
    constexpr unsigned char ctorSig[]={0x48,0x8B,0xC4,0x55,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D,0x68,0xC8};
    constexpr unsigned char factoryCall[]={0x48,0x8B,0x88,0x80,0x01,0,0,0x4C,0x89,0x6B,0x60,0x48,0x8B,0x01};
    constexpr unsigned char resetSig[]={0x40,0x56,0x57,0x41,0x56,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x30,0xFE,0xFF};
    constexpr unsigned char resizeSig[]={0xBA,0x01,0,0,0,0x89,0x44,0x24,0x20,0x8B,0xF8,0x4C,0x8B,0x11,0x41,0xFF,0x52,0x68};
    auto* ctor=reinterpret_cast<void*>(base+0xD3F450);
    auto* resetEntry=reinterpret_cast<void*>(base+0xD449A0);
    if(std::memcmp(ctor,ctorSig,sizeof(ctorSig))||
       std::memcmp(reinterpret_cast<void*>(base+0xD3F8C1),factoryCall,sizeof(factoryCall))||
       std::memcmp(resetEntry,resetSig,sizeof(resetSig))||
       std::memcmp(reinterpret_cast<void*>(base+0xD60DC7),resizeSig,sizeof(resizeSig)))return false;
    hdrFormat=VerifyHdrFormat();
    if(!hdrFormat)return false;
    const auto state=MH_Initialize();
    if(state!=MH_OK&&state!=MH_ERROR_ALREADY_INITIALIZED)return false;
    if(MH_CreateHook(resetEntry,reinterpret_cast<void*>(&ResetViewport),reinterpret_cast<void**>(&reset))!=MH_OK)return false;
    if(MH_CreateHook(ctor,reinterpret_cast<void*>(&CreateViewport),reinterpret_cast<void**>(&construct))!=MH_OK){
        MH_RemoveHook(resetEntry);reset=nullptr;return false;
    }
    if(MH_EnableHook(resetEntry)==MH_OK&&MH_EnableHook(ctor)==MH_OK)return true;
    MH_DisableHook(resetEntry);
    MH_RemoveHook(resetEntry);MH_RemoveHook(ctor);
    reset=nullptr;construct=nullptr;
    return false;
}
}
