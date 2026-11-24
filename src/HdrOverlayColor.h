#pragma once
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

namespace dg::hdr::overlay_color {
// The overlay palette and font atlas use sRGB. Encode only these pixels
// for the negotiated HDR surface, with a 160-nit reference white.
inline HRESULT Compile(bool pq,ID3DBlob** bytecode){
    static constexpr char source[]=R"HLSL(
Texture2D atlas : register(t0);
SamplerState atlasSampler : register(s0);
struct Pixel {float4 position : SV_POSITION;float4 color : COLOR0;float2 uv : TEXCOORD0;};
float DecodeSrgb(float c){
    c=saturate(c);
    return c<=0.04045?c/12.92:pow((c+0.055)/1.055,2.4);
}
float4 main(Pixel input) : SV_Target {
    const float4 ui=input.color*atlas.Sample(atlasSampler,input.uv);
    const float3 rgb=float3(DecodeSrgb(ui.r),DecodeSrgb(ui.g),DecodeSrgb(ui.b));
#if HDR10
    const float3 bt2020=mul(float3x3(0.627404,0.329283,0.043313,
        0.069097,0.919540,0.011363,0.016391,0.088013,0.895596),rgb);
    const float3 p=pow(max(bt2020,0.0)*(160.0/10000.0),0.1593017578125);
    const float3 encoded=pow((0.8359375+18.8515625*p)/(1.0+18.6875*p),78.84375);
#else
    const float3 encoded=rgb*(160.0/80.0);
#endif
    return float4(encoded,ui.a);
}
)HLSL";
    const D3D_SHADER_MACRO macros[]={{"HDR10",pq?"1":"0"},{nullptr,nullptr}};
    return D3DCompile(source,sizeof(source)-1,"DGE HDR overlay",macros,nullptr,"main","ps_4_0",
        D3DCOMPILE_OPTIMIZATION_LEVEL3,0,bytecode,nullptr);
}

class PixelShader {
    Microsoft::WRL::ComPtr<ID3D11Device> device_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> shader_;
    bool pq_=false;

    static void Bind(const ImDrawList*,const ImDrawCmd* command){
        const auto* render=static_cast<const ImGui_ImplDX11_RenderState*>(ImGui::GetPlatformIO().Renderer_RenderState);
        if(render)render->DeviceContext->PSSetShader(static_cast<ID3D11PixelShader*>(command->UserCallbackData),nullptr,0);
    }
public:
    void Use(ID3D11Device* device,bool pq){
        if(!device)return;
        if(device_.Get()!=device||pq_!=pq){
            shader_.Reset();
            device_=device;
            pq_=pq;
            Microsoft::WRL::ComPtr<ID3DBlob> bytecode;
            if(SUCCEEDED(Compile(pq,&bytecode)))
                device->CreatePixelShader(bytecode->GetBufferPointer(),bytecode->GetBufferSize(),nullptr,&shader_);
        }
        if(!shader_)return;
        // The DX11 backend executes this before drawing and restores the
        // engine's original pixel shader when it finishes the overlay.
        ImGui::GetBackgroundDrawList()->AddCallback(&Bind,shader_.Get());
    }

    void Reset(){
        shader_.Reset();
        device_.Reset();
    }
};
}
