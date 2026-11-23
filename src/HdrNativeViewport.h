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
using Resize=void(*)(void*,void*,UINT,UINT,bool,int);
using SetResolution=void(*)(int,int,int);
using HdrEnabled=bool(*)();

inline uintptr_t base=0;
inline engine_profile::LogFn logger=nullptr;
inline Constructor construct=nullptr;
inline CreateChain createChain=nullptr;
inline Resize resizeViewport=nullptr;
inline SetResolution setResolution=nullptr;
inline std::atomic_bool reportedBorderless{false};
inline HdrEnabled nativeHdrEnabled=nullptr;
inline bool requestRgb10=false;
inline std::atomic_bool formatSelected{false};
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

inline bool D3D11ViewportRhi(){
    native_objects::MemoryReader reader;
    uintptr_t rhi=0,table=0,createViewport=0;
    return reader.Read(base,0x3A1E908,rhi)&&reader.Read(rhi,0,table)&&
        reader.Read(table,0x378,createViewport)&&createViewport==base+0xD58AF0;
}

// Both scene viewport paths query HDR immediately before reading the native
// pixel format. This also covers a worker that missed UE's earlier CVar setup.
inline bool SelectViewportFormat(){
    const auto caller=reinterpret_cast<uintptr_t>(_ReturnAddress());
    const bool enabled=nativeHdrEnabled();
    if(enabled&&requestRgb10&&(caller==base+0x145D240||caller==base+0x1459231)&&
       !formatSelected.load(std::memory_order_acquire)&&D3D11ViewportRhi()){
        const auto prior=_InterlockedCompareExchange(hdrFormat,RGB10A2,FloatRGBA);
        if((prior==FloatRGBA||prior==RGB10A2)&&!formatSelected.exchange(true)&&logger)
            logger("HDR10: selected native RGB10A2 at the DX11 scene viewport");
    }
    return enabled;
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
        ComPtr<IDXGISwapChain1> created;
        hr=modernFactory->CreateSwapChainForHwnd(device.Get(),native->OutputWindow,&desc,nullptr,nullptr,&created);
        if(SUCCEEDED(hr)){
            hr=HasHdrDisplay(created.Get())?SetColorSpace(created.Get(),DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020):DXGI_ERROR_UNSUPPORTED;
            if(SUCCEEDED(hr)){
                // IDXGISwapChain1 extends IDXGISwapChain without a separate identity.
                *out=created.Detach();
                flipChain.store(*out,std::memory_order_release);
                if(logger)logger("HDR10: RGB10A2 flip swap chain created and PQ accepted");
                return S_OK;
            }
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

inline bool HookFactory(void* rhi){
    if(createChain)return true;
    native_objects::MemoryReader reader;
    uintptr_t factory=0,table=0,entry=0;
    if(!reader.Read(reinterpret_cast<uintptr_t>(rhi),0x180,factory)||!reader.Read(factory,0,table)||
       !reader.Read(table,0x50,entry)||!entry)return false;
    auto* fn=reinterpret_cast<void*>(entry);
    if(MH_CreateHook(fn,reinterpret_cast<void*>(&CreateHdrChain),reinterpret_cast<void**>(&createChain))!=MH_OK)return false;
    if(MH_EnableHook(fn)==MH_OK)return true;
    MH_RemoveHook(fn);createChain=nullptr;
    return false;
}

inline void* CreateViewport(void* viewport,void* rhi,HWND window,UINT width,UINT height,bool fullscreen,int format){
    const bool game=GameWindow(window);
    if(game){
        flipChain.store(nullptr,std::memory_order_release);
        if(logger){
            char line[128]{};
            sprintf_s(line,"HDR10: DX11 viewport pixel format=%d fullscreen=%d",format,fullscreen);
            logger(line);
        }
        if(format==RGB10A2&&!HookFactory(rhi)&&logger)logger("HDR10: DXGI factory hook unavailable");
    }
    // Also cover a configuration request made before this module loaded.
    if(game&&format==RGB10A2)fullscreen=false;
    auto* result=construct(viewport,rhi,window,width,height,fullscreen,format);
    if(game){
        IDXGISwapChain* chain=nullptr;
        native_objects::MemoryReader reader;
        engine_profile::SetPqReady(reader.Read(reinterpret_cast<uintptr_t>(result),0x60,chain)&&OwnsFlip(chain));
        generation.fetch_add(1,std::memory_order_release);
    }
    return result;
}

// The UE4 DX11 exclusive focus-reset path can switch modes without releasing
// the RHI target. Keep the owned flip chain in composed presentation.
inline void RequestResolution(int width,int height,int mode){
    const int resolved=PqWindowMode(mode);
    if(resolved!=mode&&!reportedBorderless.exchange(true)&&logger)
        logger("HDR10: requested exclusive mode converted to borderless for task switching");
    setResolution(width,height,resolved);
}

inline void ResizeViewport(void* rhi,void* viewport,UINT width,UINT height,bool fullscreen,int format){
    native_objects::MemoryReader reader;
    IDXGISwapChain* chain=nullptr;
    if(fullscreen&&reader.Read(reinterpret_cast<uintptr_t>(viewport),0x60,chain)&&OwnsFlip(chain))
        fullscreen=false;
    resizeViewport(rhi,viewport,width,height,fullscreen,format);
}

inline bool Initialize(Settings choice,bool exact,engine_profile::LogFn log){
    if(!exact||!ManagedPq(choice))return false;
    base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if(!base)return false;
    logger=log;
    requestRgb10=choice.buffer!=1;
    constexpr unsigned char querySig[]={0x40,0x57,0x48,0x83,0xEC,0x30,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF};
    constexpr unsigned char createQuery[]={0xE8,0xC0,0x85,0xE9,0xFF,0x84,0xC0,0x74,0x10};
    constexpr unsigned char resizeQuery[]={0xE8,0xCF,0xC5,0xE9,0xFF,0x44,0x0F,0xB6,0xE8};
    constexpr unsigned char createRhiSig[]={0x48,0x8B,0xC4,0x48,0x89,0x50,0x10,0x57,0x41,0x56,0x41,0x57};
    constexpr unsigned char ctorSig[]={0x48,0x8B,0xC4,0x55,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D,0x68,0xC8};
    constexpr unsigned char factoryCall[]={0x48,0x8B,0x88,0x80,0x01,0,0,0x4C,0x89,0x6B,0x60,0x48,0x8B,0x01};
    constexpr unsigned char resizeRhiSig[]={0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x30,0xFE,0xFF,0xFF,0xFF};
    constexpr unsigned char resolutionSig[]={0x48,0x8B,0xC4,0x55,0x48,0x8B,0xEC,0x48,0x83,0xEC,0x60,0x48,0xC7,0x45,0xD0,0xFE,0xFF,0xFF,0xFF};
    constexpr unsigned char borderlessText[]={0x77,0,0x66,0,0,0};
    constexpr unsigned char resizeSig[]={0xBA,0x01,0,0,0,0x89,0x44,0x24,0x20,0x8B,0xF8,0x4C,0x8B,0x11,0x41,0xFF,0x52,0x68};
    auto* ctor=reinterpret_cast<void*>(base+0xD3F450);
    auto* resizeEntry=reinterpret_cast<void*>(base+0xD5C260);
    auto* resolution=reinterpret_cast<void*>(base+0x1C55DF0);
    auto* query=reinterpret_cast<void*>(base+0x12F5800);
    if(std::memcmp(query,querySig,sizeof(querySig))||
       std::memcmp(reinterpret_cast<void*>(base+0x145D23B),createQuery,sizeof(createQuery))||
       std::memcmp(reinterpret_cast<void*>(base+0x145922C),resizeQuery,sizeof(resizeQuery))||
       std::memcmp(reinterpret_cast<void*>(base+0xD58AF0),createRhiSig,sizeof(createRhiSig))||
       std::memcmp(ctor,ctorSig,sizeof(ctorSig))||
       std::memcmp(reinterpret_cast<void*>(base+0xD3F8C1),factoryCall,sizeof(factoryCall))||
       std::memcmp(resizeEntry,resizeRhiSig,sizeof(resizeRhiSig))||
       std::memcmp(resolution,resolutionSig,sizeof(resolutionSig))||
       std::memcmp(reinterpret_cast<void*>(base+0x2DE70A8),borderlessText,sizeof(borderlessText))||
       std::memcmp(reinterpret_cast<void*>(base+0xD60DC7),resizeSig,sizeof(resizeSig)))return false;
    hdrFormat=VerifyHdrFormat();
    if(!hdrFormat)return false;
    const auto state=MH_Initialize();
    if(state!=MH_OK&&state!=MH_ERROR_ALREADY_INITIALIZED)return false;
    struct Hook {void* target;void* detour;void** original;};
    const Hook hooks[]{
        {query,reinterpret_cast<void*>(&SelectViewportFormat),reinterpret_cast<void**>(&nativeHdrEnabled)},
        {ctor,reinterpret_cast<void*>(&CreateViewport),reinterpret_cast<void**>(&construct)},
        {resizeEntry,reinterpret_cast<void*>(&ResizeViewport),reinterpret_cast<void**>(&resizeViewport)},
        {resolution,reinterpret_cast<void*>(&RequestResolution),reinterpret_cast<void**>(&setResolution)},
    };
    size_t created=0,queued=0;
    for(const auto& hook:hooks){
        if(MH_CreateHook(hook.target,hook.detour,hook.original)!=MH_OK)break;
        ++created;
        if(MH_QueueEnableHook(hook.target)!=MH_OK)break;
        ++queued;
    }
    if(queued==sizeof(hooks)/sizeof(hooks[0])&&MH_ApplyQueued()==MH_OK)return true;
    for(size_t i=0;i<created;++i){
        MH_DisableHook(hooks[i].target);
        MH_RemoveHook(hooks[i].target);
        *hooks[i].original=nullptr;
    }
    return false;
}
}
