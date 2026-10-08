#include "CameraAuditFeature.h"

#include <windows.h>
#include <MinHook.h>
#include <atomic>
#include <cmath>
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace dg::camera_audit {
namespace {

struct Section {
    const unsigned char* ptr = nullptr;
    size_t size = 0;
};

struct Marker {
    std::string name;
    bool wide = false;
    uintptr_t address = 0;
    unsigned references = 0;
    std::array<size_t, 4> referenceRvas{};
};

bool GetSection(HMODULE module, const char* name, Section& out) {
    if (!module || !name) return false;
    const auto* base = reinterpret_cast<const unsigned char*>(module);
    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE ||
        dos->e_lfanew < static_cast<LONG>(sizeof(IMAGE_DOS_HEADER)) ||
        dos->e_lfanew > 0x1000) return false;

    const auto* nt =
        reinterpret_cast<const IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE ||
        nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) return false;

    const uint64_t sizeOfImage = nt->OptionalHeader.SizeOfImage;
    const auto* sections = IMAGE_FIRST_SECTION(nt);
    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; ++i) {
        char sectionName[9]{};
        std::memcpy(sectionName, sections[i].Name, 8);
        if (std::strcmp(sectionName, name) != 0) continue;
        const uint64_t rva = sections[i].VirtualAddress;
        const uint64_t length = sections[i].Misc.VirtualSize;
        if (!length || rva >= sizeOfImage || length > sizeOfImage - rva)
            return false;
        out = {base + rva, static_cast<size_t>(length)};
        return true;
    }
    return false;
}

void Emit(LogFn log, const char* text) {
    if (log && text) log(text);
}

void SearchMarker(const Section& section, uintptr_t base,
                  const char* name, bool wide,
                  std::vector<Marker>& found, unsigned& occurrences) {
    occurrences = 0;
    std::vector<unsigned char> pattern;
    const size_t length = std::strlen(name);
    pattern.reserve((length + 1) * (wide ? 2u : 1u));
    for (size_t i = 0; i < length; ++i) {
        pattern.push_back(static_cast<unsigned char>(name[i]));
        if (wide) pattern.push_back(0);
    }
    pattern.push_back(0);
    if (wide) pattern.push_back(0);
    if (pattern.size() > section.size) return;

    // Limit collected matches to two per marker/encoding. The count itself
    // is bounded, not allowed to flood the session log.
    for (size_t i = 0; i <= section.size - pattern.size(); ++i) {
        if (section.ptr[i] != pattern[0] ||
            std::memcmp(section.ptr + i, pattern.data(), pattern.size()) != 0)
            continue;

        ++occurrences;
        if (occurrences <= 2) {
            found.push_back({
                name, wide,
                reinterpret_cast<uintptr_t>(section.ptr + i), 0, {}
            });
        }
        if (occurrences == 100) break;
    }
    (void)base;
}

} // namespace

void Run(LogFn log) {
    HMODULE mainModule = GetModuleHandleW(nullptr);
    Section text{}, rdata{};
    if (!GetSection(mainModule, ".text", text) ||
        !GetSection(mainModule, ".rdata", rdata)) {
        Emit(log, "CameraAudit V0.35A: unavailable PE sections; fail-closed");
        return;
    }

    const uintptr_t base = reinterpret_cast<uintptr_t>(mainModule);
    const char* const names[] = {
        "CameraComponent", "PlayerCameraManager", "CameraCachePrivate",
        "FieldOfView", "FOVAngle", "DefaultFOV", "FOV",
        "GetFOVAngle", "GetCameraView", "UpdateCamera",
        "BlueprintUpdateCamera", "CameraDistance", "CameraZoom",
        "CameraBoom", "TargetArmLength", "OrthoWidth",
        "ProjectionMode", "DesiredFOV", "AspectRatioAxisConstraint",
        "ViewTarget", "PlayerController", "GetPlayerViewPoint",
        "CameraPitch", "CameraRotation", "MayhemCamera",
        "CameraSettings", "CameraOffset"
    };

    std::vector<Marker> found;
    for (const char* name : names) {
        for (int mode = 0; mode < 2; ++mode) {
            unsigned count = 0;
            SearchMarker(rdata, base, name, mode == 1, found, count);
            if (count > 0) {
                char line[220]{};
                sprintf_s(line,
                    "CameraAudit V0.35A: name=%s encoding=%s matches=%u%s",
                    name, mode ? "UTF16" : "ASCII", count,
                    count >= 100 ? "+" : "");
                Emit(log, line);
            }
        }
    }

    // One pass through read-only executable .text. Only RIP-relative LEA /
    // MOV operands are candidates, never executing or patching code.
    for (size_t i = 0; i + 7 <= text.size; ++i) {
        const unsigned char* p = text.ptr + i;
        if ((p[0] != 0x48 && p[0] != 0x4C) ||
            (p[1] != 0x8D && p[1] != 0x8B) ||
            (p[2] & 0xC7) != 0x05)
            continue;

        int32_t disp = 0;
        std::memcpy(&disp, p + 3, sizeof(disp));
        const intptr_t target = reinterpret_cast<intptr_t>(p + 7) + disp;

        for (auto& m : found) {
            if (target != static_cast<intptr_t>(m.address)) continue;
            if (m.references < m.referenceRvas.size())
                m.referenceRvas[m.references] =
                    reinterpret_cast<uintptr_t>(p) - base;
            ++m.references;
        }
    }

    unsigned xrefs = 0;
    for (const auto& m : found) {
        char line[280]{};
        sprintf_s(line,
            "CameraAudit V0.35A: marker=%s/%s RVA=0x%zX codeRefs=%u "
            "firstRVA=[0x%zX,0x%zX,0x%zX,0x%zX]",
            m.name.c_str(), m.wide ? "W" : "A",
            static_cast<size_t>(m.address - base),
            m.references,
            m.referenceRvas[0], m.referenceRvas[1],
            m.referenceRvas[2], m.referenceRvas[3]);
        Emit(log, line);
        xrefs += m.references;
    }

    char finalLine[220]{};
    sprintf_s(finalLine,
        "CameraAudit V0.35A: complete markers=%zu RIPrefs=%u; "
        "read-only, no camera mutation, FOV UI intentionally locked",
        found.size(), xrefs);
    Emit(log, finalLine);
}


namespace {

using NativeThunkFn = void(*)(void*, void*, void*);
NativeThunkFn g_origGetFov = nullptr;
NativeThunkFn g_origGetView = nullptr;
NativeThunkFn g_origSetFov = nullptr;
std::atomic_uint g_fovCalls{0};
std::atomic_uint g_viewCalls{0};
std::atomic_uint g_setCalls{0};
LogFn g_probeLog = nullptr;
uintptr_t g_moduleBase = 0;

void LogNativeTarget(const char* label, void* instance, size_t vtableSlot) {
    if (!instance || !g_probeLog) return;
    // This method runs exclusively in the original Unreal native script
    // thunk with its live UObject `this`; no pointers are retained.
    void** const vtable = *reinterpret_cast<void***>(instance);
    if (!vtable) return;
    const uintptr_t target = reinterpret_cast<uintptr_t>(
        vtable[vtableSlot / sizeof(void*)]);
    char line[290]{};
    if (target >= g_moduleBase && target < g_moduleBase + 0x3DDF000) {
        sprintf_s(line,
            "CameraAudit V0.35C: %s live UObject=%p vtable=%p "
            "virtualSlot=0x%zX nativeTargetRVA=0x%zX",
            label, instance, vtable, vtableSlot,
            static_cast<size_t>(target - g_moduleBase));
    } else {
        sprintf_s(line,
            "CameraAudit V0.35C: %s live UObject=%p vtable=%p "
            "virtualSlot=0x%zX target not in supported EXE",
            label, instance, vtable, vtableSlot);
    }
    Emit(g_probeLog, line);
}

void HookGetFov(void* object, void* frame, void* result) {
    if (g_origGetFov) g_origGetFov(object, frame, result);
    const unsigned count = g_fovCalls.fetch_add(1) + 1;
    if (count > 8) return;
    LogNativeTarget("PlayerCameraManager.GetFOVAngle", object, 0x690);
    if (result) {
        const float fov = *reinterpret_cast<const float*>(result);
        char line[180]{};
        sprintf_s(line, "CameraAudit V0.35C: GetFOVAngle call=%u "
            "returnedFOV=%.3f plausible=%d", count, fov,
            std::isfinite(fov) && fov > 0.0f && fov < 180.0f ? 1 : 0);
        Emit(g_probeLog, line);
    }
}

void HookGetView(void* object, void* frame, void* result) {
    if (g_origGetView) g_origGetView(object, frame, result);
    const unsigned count = g_viewCalls.fetch_add(1) + 1;
    if (count > 8) return;
    LogNativeTarget("CameraComponent.GetCameraView", object, 0x508);
    char line[180]{};
    sprintf_s(line, "CameraAudit V0.35C: GetCameraView call=%u "
        "result=%p (output untouched)", count, result);
    Emit(g_probeLog, line);
}

void HookSetFov(void* object, void* frame, void* result) {
    if (g_origSetFov) g_origSetFov(object, frame, result);
    const unsigned count = g_setCalls.fetch_add(1) + 1;
    if (count > 8) return;
    LogNativeTarget("CameraComponent.SetFieldOfView", object, 0x500);
    if (object) {
        const float fov = *reinterpret_cast<const float*>(
            static_cast<const unsigned char*>(object) + 0x25C);
        char line[190]{};
        sprintf_s(line, "CameraAudit V0.35C: SetFieldOfView call=%u "
            "reflectedField+0x25C=%.3f plausible=%d", count, fov,
            std::isfinite(fov) && fov > 0.0f && fov < 180.0f ? 1 : 0);
        Emit(g_probeLog, line);
    }
}

bool ProbeOne(const char* name, uintptr_t rva,
              const unsigned char* expected, size_t expectedLength,
              void* hook, NativeThunkFn& original) {
    const auto* target = reinterpret_cast<const unsigned char*>(g_moduleBase + rva);
    if (std::memcmp(target, expected, expectedLength) != 0) {
        char line[160]{};
        sprintf_s(line, "CameraAudit V0.35C: %s exact bytes mismatch, skipped", name);
        Emit(g_probeLog, line);
        return false;
    }
    const MH_STATUS created = MH_CreateHook(
        const_cast<unsigned char*>(target), hook,
        reinterpret_cast<void**>(&original));
    if (created != MH_OK) {
        char line[170]{};
        sprintf_s(line, "CameraAudit V0.35C: %s create hook failed=%d",
            name, static_cast<int>(created));
        Emit(g_probeLog, line);
        return false;
    }
    const MH_STATUS enabled = MH_EnableHook(const_cast<unsigned char*>(target));
    if (enabled != MH_OK && enabled != MH_ERROR_ENABLED) {
        char line[170]{};
        sprintf_s(line, "CameraAudit V0.35C: %s enable hook failed=%d",
            name, static_cast<int>(enabled));
        Emit(g_probeLog, line);
        return false;
    }
    char line[185]{};
    sprintf_s(line, "CameraAudit V0.35C: %s read-only native thunk READY RVA=0x%zX",
        name, static_cast<size_t>(rva));
    Emit(g_probeLog, line);
    return true;
}

} // namespace

void InstallNativeProbes(LogFn log) {
    g_probeLog = log;
    g_moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if (!g_moduleBase) return;

    const MH_STATUS init = MH_Initialize();
    if (init != MH_OK && init != MH_ERROR_ALREADY_INITIALIZED) {
        Emit(log, "CameraAudit V0.35C: MinHook unavailable, no probes installed");
        return;
    }

    // These RVAs and prefixes come from the exact, user-supplied retail PE64
    // with SHA-256 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54.
    // They are reflected UE4 script thunks, NOT the native camera methods.
    // A live thunk call exposes the actual virtual method address safely.
    static constexpr unsigned char kGetter[] = {
        0x40, 0x53, 0x48, 0x83, 0xEC, 0x20, 0x48, 0x8B, 0x42, 0x20
    };
    static constexpr unsigned char kView[] = {
        0x48, 0x8B, 0xC4, 0x57, 0x48, 0x81, 0xEC, 0xC0, 0x05, 0x00, 0x00
    };
    static constexpr unsigned char kSetter[] = {
        0x48, 0x89, 0x5C, 0x24, 0x10, 0x57, 0x48, 0x83, 0xEC, 0x20
    };
    ProbeOne("GetFOVAngle", 0x1DB6F10,
        kGetter, sizeof(kGetter),
        reinterpret_cast<void*>(&HookGetFov), g_origGetFov);
    ProbeOne("GetCameraView", 0x1CE7D30,
        kView, sizeof(kView),
        reinterpret_cast<void*>(&HookGetView), g_origGetView);
    ProbeOne("SetFieldOfView", 0x1CE9980,
        kSetter, sizeof(kSetter),
        reinterpret_cast<void*>(&HookSetFov), g_origSetFov);

    Emit(log, "CameraAudit V0.35C: probes record only first 8 calls per thunk; "
        "zero FOV/zoom writes, no UObject pointer caches");
}

} // namespace dg::camera_audit
