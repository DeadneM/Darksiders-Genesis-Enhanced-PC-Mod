#include "CameraAuditFeature.h"

#include <windows.h>
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

} // namespace dg::camera_audit
