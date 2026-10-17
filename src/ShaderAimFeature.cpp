#include "ShaderAimFeature.h"
#include "RuntimeSettings.h"

#include <MinHook.h>
#include <windows.h>
#include <atomic>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace dg::aim_shader {
namespace {

// Captured by user with ShaderToggler: CRC32 of pixel shader bytecode.
constexpr uint32_t kAimingLineHash = 1212955419u; // 0x484C3B1B
constexpr uint32_t kAimingArcHash  = 2294978555u; // 0x88CA97FB

using CreatePSFn = HRESULT(STDMETHODCALLTYPE*)(
    ID3D11Device*, const void*, SIZE_T, ID3D11ClassLinkage*, ID3D11PixelShader**);
using SetPSFn = void(STDMETHODCALLTYPE*)(
    ID3D11DeviceContext*, ID3D11PixelShader*, ID3D11ClassInstance* const*, UINT);
using DrawFn = void(STDMETHODCALLTYPE*)(ID3D11DeviceContext*, UINT, UINT);
using DrawIndexedFn = void(STDMETHODCALLTYPE*)(ID3D11DeviceContext*, UINT, UINT, INT);
using DrawInstancedFn = void(STDMETHODCALLTYPE*)(ID3D11DeviceContext*, UINT, UINT, UINT, UINT);
using DrawIndexedInstancedFn = void(STDMETHODCALLTYPE*)(
    ID3D11DeviceContext*, UINT, UINT, UINT, INT, UINT);
using DrawAutoFn = void(STDMETHODCALLTYPE*)(ID3D11DeviceContext*);
using DrawIndirectFn = void(STDMETHODCALLTYPE*)(ID3D11DeviceContext*, ID3D11Buffer*, UINT);

CreatePSFn g_originalCreatePS = nullptr;
SetPSFn g_originalSetPS = nullptr;
DrawFn g_originalDraw = nullptr;
DrawIndexedFn g_originalDrawIndexed = nullptr;
DrawInstancedFn g_originalDrawInstanced = nullptr;
DrawIndexedInstancedFn g_originalDrawIndexedInstanced = nullptr;
DrawAutoFn g_originalDrawAuto = nullptr;
DrawIndirectFn g_originalDrawIndexedIndirect = nullptr;
DrawIndirectFn g_originalDrawIndirect = nullptr;

SRWLOCK g_lock = SRWLOCK_INIT;
std::unordered_set<ID3D11PixelShader*> g_targetShaders;
std::unordered_map<ID3D11DeviceContext*, bool> g_contextTargetPS;
std::atomic_bool g_ready{false};
std::atomic_uint32_t g_matchCount{0}, g_drawCount{0}, g_blockCount{0};
LogFn g_logger = nullptr;

// ShaderToggler's bytecode hash: standard reflected CRC32 (0xEDB88320).
uint32_t BytecodeCRC32(const void* bytes, size_t length) {
    auto* p = static_cast<const uint8_t*>(bytes);
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < length; ++i) {
        crc ^= p[i];
        for (unsigned bit = 0; bit < 8; ++bit)
            crc = (crc >> 1) ^ ((crc & 1u) ? 0xEDB88320u : 0u);
    }
    return ~crc;
}

bool EnabledByExistingMode() {
    const auto& rt = dg::runtime::Get();
    const int mode = rt.aimIndicatorHideMode.load(std::memory_order_relaxed);
    const auto now=GetTickCount64(),view=rt.tpsGameplayViewTick.load(std::memory_order_acquire);
    const bool liveView=rt.tpsViewGameplay.load(std::memory_order_acquire)&&view&&now>=view&&now-view<=350;
    const bool tps = rt.thirdPersonEnabled.load(std::memory_order_relaxed) &&
        !rt.tpsTemporarilySuspended.load(std::memory_order_relaxed)&&
        !rt.tpsCutsceneSuspended.load(std::memory_order_relaxed)&&
        (liveView||rt.tpsUiViewHeld.load(std::memory_order_acquire));
    return mode == 0 || (mode == 1 && !tps) || (mode == 2 && tps);
}

HRESULT STDMETHODCALLTYPE HookCreatePS(
    ID3D11Device* device, const void* code, SIZE_T length,
    ID3D11ClassLinkage* linkage, ID3D11PixelShader** out) {
    const auto original = g_originalCreatePS;
    if (!original) return E_FAIL;
    // Compute hashes ONLY for real creations, never for failed validation calls.
    const HRESULT result = original(device, code, length, linkage, out);
    if (SUCCEEDED(result) && out && *out && code && length && length < 4u * 1024u * 1024u) {
        const uint32_t hash = BytecodeCRC32(code, length);
        if (hash == kAimingLineHash || hash == kAimingArcHash) {
            AcquireSRWLockExclusive(&g_lock);
            // Keep a reference so a freed COM pointer cannot be reused as a
            // false match. Very small and bounded: only matched shaders.
            if (g_targetShaders.size() < 64 && g_targetShaders.insert(*out).second) {
                (*out)->AddRef();
                ++g_matchCount;
            }
            ReleaseSRWLockExclusive(&g_lock);
        }
    }
    return result;
}

void STDMETHODCALLTYPE HookSetPS(
    ID3D11DeviceContext* ctx, ID3D11PixelShader* shader,
    ID3D11ClassInstance* const* classes, UINT count) {
    if (g_originalSetPS) g_originalSetPS(ctx, shader, classes, count);
    if (!g_ready.load(std::memory_order_relaxed) || !ctx) return;
    AcquireSRWLockShared(&g_lock);
    const bool matched = shader && g_targetShaders.count(shader) != 0;
    ReleaseSRWLockShared(&g_lock);
    AcquireSRWLockExclusive(&g_lock);
    if(g_contextTargetPS.count(ctx)||g_contextTargetPS.size()<64)g_contextTargetPS[ctx] = matched;
    ReleaseSRWLockExclusive(&g_lock);
}

bool Skip(ID3D11DeviceContext* ctx) {
    if (!g_ready.load(std::memory_order_relaxed) || !EnabledByExistingMode()) return false;
    if (g_matchCount.load(std::memory_order_relaxed) == 0) return false;
    AcquireSRWLockShared(&g_lock);
    const auto it = g_contextTargetPS.find(ctx);
    const bool matched = it != g_contextTargetPS.end() && it->second;
    ReleaseSRWLockShared(&g_lock);
    if (matched) {
        // ClearState and command lists can bypass our PSSetShader observer.
        // Verify only would-be blocked draws, retaining the fast path for
        // ordinary rendering; a stale context entry cannot hide other PSs.
        ID3D11PixelShader* actual=nullptr;
        ctx->PSGetShader(&actual,nullptr,nullptr);
        AcquireSRWLockShared(&g_lock);
        const bool stillMatched=actual&&g_targetShaders.count(actual)!=0;
        ReleaseSRWLockShared(&g_lock);
        if(actual)actual->Release();
        if(!stillMatched)return false;
        ++g_blockCount;
        return true;
    }
    return false;
}

void STDMETHODCALLTYPE HookDraw(ID3D11DeviceContext* ctx, UINT vertices, UINT first) {
    ++g_drawCount;
    if (!Skip(ctx) && g_originalDraw) g_originalDraw(ctx, vertices, first);
}
void STDMETHODCALLTYPE HookDrawIndexed(ID3D11DeviceContext* ctx, UINT count, UINT first, INT base) {
    ++g_drawCount;
    if (!Skip(ctx) && g_originalDrawIndexed) g_originalDrawIndexed(ctx, count, first, base);
}
void STDMETHODCALLTYPE HookDrawInstanced(
    ID3D11DeviceContext* ctx, UINT verts, UINT instances, UINT startVertex, UINT startInstance) {
    ++g_drawCount;
    if (!Skip(ctx) && g_originalDrawInstanced)
        g_originalDrawInstanced(ctx, verts, instances, startVertex, startInstance);
}
void STDMETHODCALLTYPE HookDrawIndexedInstanced(ID3D11DeviceContext* ctx,
    UINT indices, UINT instances, UINT first, INT base, UINT start) {
    ++g_drawCount;
    if (!Skip(ctx) && g_originalDrawIndexedInstanced)
        g_originalDrawIndexedInstanced(ctx, indices, instances, first, base, start);
}
void STDMETHODCALLTYPE HookDrawAuto(ID3D11DeviceContext* ctx) {
    ++g_drawCount;
    if (!Skip(ctx) && g_originalDrawAuto) g_originalDrawAuto(ctx);
}
void STDMETHODCALLTYPE HookDrawIndexedIndirect(ID3D11DeviceContext* ctx, ID3D11Buffer* args, UINT offset) {
    ++g_drawCount;
    if (!Skip(ctx) && g_originalDrawIndexedIndirect) g_originalDrawIndexedIndirect(ctx, args, offset);
}
void STDMETHODCALLTYPE HookDrawIndirect(ID3D11DeviceContext* ctx, ID3D11Buffer* args, UINT offset) {
    ++g_drawCount;
    if (!Skip(ctx) && g_originalDrawIndirect) g_originalDrawIndirect(ctx, args, offset);
}
} // namespace

bool Install(ID3D11Device* device, ID3D11DeviceContext* context, LogFn logger) {
    if (!device || !context || g_ready.load()) return false;
    g_logger = logger;
    // Device::CreatePixelShader at vtable[15], Context::PSSetShader at [9].
    // Install here, immediately after the project's EARLY dummy device
    // creation, BEFORE the real game compiles its shaders (not in Present).
    void** dv = *reinterpret_cast<void***>(device);
    void** cv = *reinterpret_cast<void***>(context);
    struct Target { void* address; void* hook; void** original; };
    const Target targets[] = {
        {dv[15], reinterpret_cast<void*>(&HookCreatePS), reinterpret_cast<void**>(&g_originalCreatePS)},
        {cv[9], reinterpret_cast<void*>(&HookSetPS), reinterpret_cast<void**>(&g_originalSetPS)},
        {cv[12], reinterpret_cast<void*>(&HookDrawIndexed), reinterpret_cast<void**>(&g_originalDrawIndexed)},
        {cv[13], reinterpret_cast<void*>(&HookDraw), reinterpret_cast<void**>(&g_originalDraw)},
        {cv[20], reinterpret_cast<void*>(&HookDrawIndexedInstanced), reinterpret_cast<void**>(&g_originalDrawIndexedInstanced)},
        {cv[21], reinterpret_cast<void*>(&HookDrawInstanced), reinterpret_cast<void**>(&g_originalDrawInstanced)},
        {cv[38], reinterpret_cast<void*>(&HookDrawAuto), reinterpret_cast<void**>(&g_originalDrawAuto)},
        {cv[39], reinterpret_cast<void*>(&HookDrawIndexedIndirect), reinterpret_cast<void**>(&g_originalDrawIndexedIndirect)},
        {cv[40], reinterpret_cast<void*>(&HookDrawIndirect), reinterpret_cast<void**>(&g_originalDrawIndirect)}
    };
    std::vector<void*> created;
    for (const auto& t : targets) {
        if (!t.address || MH_CreateHook(t.address, t.hook, t.original) != MH_OK) {
            for (auto* address : created) MH_RemoveHook(address);
            if (logger) logger("Aim shaders V0.90: hook installation unavailable; native drawing left unchanged");
            return false;
        }
        created.push_back(t.address);
    }
    g_ready.store(true, std::memory_order_release);
    if (logger) logger("Aim shaders V0.90: CRC32 PS capture and 7 draw call hooks installed, using existing visibility mode");
    return true;
}

Telemetry GetTelemetry() {
    Telemetry t{};
    t.hooksReady = g_ready.load();
    t.matchedShaders = g_matchCount.load();
    t.interceptedDraws = g_drawCount.load();
    t.blockedDraws = g_blockCount.load();
    return t;
}

void Shutdown() {
    g_ready.store(false, std::memory_order_release);
    if (g_logger) g_logger("Aim shaders V0.90 totals: matchedPS=%u drawCalls=%u blockedDraws=%u",
        g_matchCount.load(), g_drawCount.load(), g_blockCount.load());
    AcquireSRWLockExclusive(&g_lock);
    g_contextTargetPS.clear();
    for (auto* shader : g_targetShaders) shader->Release();
    g_targetShaders.clear();
    ReleaseSRWLockExclusive(&g_lock);
}

} // namespace dg::aim_shader
