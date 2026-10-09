#include "OverlayUi.h"

#include "HorseFeature.h"
#include "CameraTraceFeature.h"

#include "imgui.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

namespace dg::overlay {
namespace {

void DrawTunableFeature(
    config::Store& config,
    const char* label,
    const char* id,
    bool* enabled,
    float* value,
    float minValue,
    float maxValue,
    float defaultValue,
    const char* format,
    const char* note,
    bool hookReady
) {
    if (ImGui::Checkbox(label, enabled)) {
        config.Save();
    }

    if (!*enabled) {
        ImGui::SameLine(310.0f);
        ImGui::TextDisabled("%s", hookReady ? "READY" : "WAIT");
        if (ImGui::IsItemHovered() && note) ImGui::SetTooltip("%s", note);
        return;
    }

    ImGui::Indent();
    ImGui::SetNextItemWidth(245.0f);

    std::string sliderLabel =
        std::string("Value##Slider_") + id;

    bool changed = ImGui::SliderFloat(
        sliderLabel.c_str(),
        value,
        minValue,
        maxValue,
        format
    );

    ImGui::SameLine();
    ImGui::SetNextItemWidth(110.0f);

    std::string inputLabel =
        std::string("Manual##Input_") + id;

    if (ImGui::InputFloat(
            inputLabel.c_str(),
            value,
            0.0f,
            0.0f,
            "%.3f")) {
        changed = true;
    }

    ImGui::SameLine();

    std::string defaultLabel =
        std::string("Default##Reset_") + id;

    if (ImGui::Button(defaultLabel.c_str())) {
        *value = defaultValue;
        changed = true;
    }
    // V0.53: one unobtrusive status beside the selected value.
    ImGui::SameLine();
    ImGui::TextDisabled("%s", hookReady ? "LIVE" : "WAIT");
    if (ImGui::IsItemHovered() && note) ImGui::SetTooltip("%s", note);

    if (changed) {
        if (*value < minValue) {
            *value = minValue;
        }
        if (*value > maxValue) {
            *value = maxValue;
        }
        config.Save();
    }

    ImGui::Unindent();
}

void DrawSectionTitle(const char* title) {
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("%s", title);
    ImGui::Separator();
    ImGui::Spacing();
}

} // namespace

void Draw(Context& c) {
    if (!c.config ||
        !c.targetValidation ||
        !c.overlayVisible ||
        !c.captureMenuKey ||
        !c.captureCameraKeyIndex ||
        !c.hudHidden ||
        !c.lastAction) {
        return;
    }

    auto& config = *c.config;
    const Telemetry& t = c.telemetry;

    ImGui::SetNextWindowSize(
        ImVec2(840.0f, 720.0f),
        ImGuiCond_FirstUseEver
    );
    ImGui::SetNextWindowPos(
        ImVec2(80.0f, 80.0f),
        ImGuiCond_FirstUseEver
    );

    bool open = true;
    if (!ImGui::Begin(
            "Darksiders Genesis Enhanced",
            &open,
            ImGuiWindowFlags_NoCollapse)) {
        ImGui::End();
        if (!open) {
            c.overlayVisible->store(false);
        }
        return;
    }

    ImGui::Text(
        "ASI Overlay  v%s",
        c.build ? c.build : "unknown"
    );
    ImGui::SameLine();

    const std::string menuKeyName =
        config::KeyDisplayName(config.menuKey);

    ImGui::TextDisabled(
        "| %s to close",
        menuKeyName.c_str()
    );
    ImGui::Separator();

    if (ImGui::BeginTabBar("MainTabs")) {
        if (ImGui::BeginTabItem("Gameplay")) {
            ImGui::Spacing();

            DrawSectionTitle("Player");

            bool hudHidden=c.hudHidden->load();
            if (ImGui::Checkbox("HUD Hidden", &hudHidden)) {
                c.hudHidden->store(hudHidden);
                *c.lastAction=hudHidden?"HUD hidden":"HUD visible";
                if (c.log) c.log("Overlay -> HUD %s",hudHidden?"HIDDEN":"VISIBLE");
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled("%s | F1",t.hudHookReady?"READY":"WAIT");

            if (ImGui::Checkbox("Hide Reticle (independent of HUD)", &config.hideReticle)) {
                config.Save();
                *c.lastAction = config.hideReticle
                    ? "Hide Reticle ON (close overlay to apply)"
                    : "Hide Reticle OFF (close overlay to restore)";
                if (c.log) c.log("Overlay -> Hide Reticle %s", config.hideReticle ? "ON" : "OFF");
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled("%s", t.reticleCursorHookReady
                ? "Native UI cursor + Win32 SetCursor hook (F6 default)"
                : "Win32 cursor hook unavailable; native UI fallback");
            ImGui::TextWrapped(
                "Hide Reticle applies in gameplay. The pointer remains visible in "
                "this menu so its controls remain clickable."
            );
            if (ImGui::Button("Apply Hide Reticle and Return to Game")) {
                config.Save();
                c.overlayVisible->store(false);
                *c.lastAction = config.hideReticle
                    ? "Hide Reticle ON (returned to gameplay)"
                    : "Hide Reticle OFF (returned to gameplay)";
                if (c.log) c.log("Reticle V0.42: Hide Reticle apply=%d overlay closed",
                    config.hideReticle ? 1 : 0);
            }
            DrawTunableFeature(
                config,
                "Movement Speed",
                "MovementSpeed",
                &config.movementSpeedEnabled,
                &config.movementSpeedMultiplier,
                0.00f,
                3.00f,
                1.50f,
                "%.2fx",
                t.movementHookReady
                    ? "Runtime hook active"
                    : "Native hook unavailable",
                t.movementHookReady
            );

            if (ImGui::Checkbox(
                    "Action Recovery",
                    &config.actionRecoveryEnabled)) {
                config.Save();
                *c.lastAction =
                    std::string("Action Recovery ") +
                    (config.actionRecoveryEnabled ? "ON" : "OFF");
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled(
                "%s",
                t.recoveryHookReady
                    ? "AllowedActions MOVE, tail-only safety"
                    : "Native hook unavailable"
            );

            if (config.actionRecoveryEnabled) {
                ImGui::Indent();
                ImGui::SetNextItemWidth(280.0f);

                bool recoveryChanged =
                    ImGui::SliderFloat(
                        "Recovery Delay##ActionRecovery",
                        &config.actionRecoveryDelayMs,
                        0.0f,
                        500.0f,
                        "%.0f ms"
                    );

                ImGui::SameLine();
                if (ImGui::Button(
                        "Default##Reset_ActionRecovery")) {
                    config.actionRecoveryDelayMs = 0.0f;
                    recoveryChanged = true;
                }

                if (recoveryChanged) {
                    config.Save();
                }

                if (ImGui::TreeNode("Recovery diagnostics##V053")) {
                ImGui::TextDisabled(
                    "MOVE forced only in AWAITING_FINISH.");
                ImGui::TextDisabled(
                    "Queries %d | Local %d | Blocked %d | Forced %d",
                    t.actionMoveQueries,
                    t.actionMoveLocalQueries,
                    t.actionMoveNativeBlocked,
                    t.actionMoveForced
                );

                if (t.lastActionMoveState >= 0 &&
                    c.abilityStateName) {
                    ImGui::TextDisabled(
                        "Last ability: %s (%d) | elapsed %.3f s",
                        c.abilityStateName(
                            static_cast<unsigned char>(
                                t.lastActionMoveState)),
                        t.lastActionMoveState,
                        t.lastActionMoveElapsed
                    );
                }
                ImGui::TreePop();
                }
                ImGui::Unindent();
            }

            DrawTunableFeature(
                config,
                "Jump Height",
                "JumpHeight",
                &config.jumpHeightEnabled,
                &config.jumpHeightMultiplier,
                0.00f,
                20.00f,
                1.25f,
                "%.2fx",
                t.movementHookReady
                    ? "Runtime property hook | JumpZ + DoubleJumpZ"
                    : "Native movement hook unavailable",
                t.movementHookReady
            );

            DrawTunableFeature(
                config,
                "Glide / Flight Duration",
                "GlideDuration",
                &config.glideDurationEnabled,
                &config.glideDurationMultiplier,
                0.00f,
                100.00f,
                10.00f,
                "%.2fx",
                t.movementHookReady
                    ? "Runtime property hook | GlideDurationSeconds"
                    : "Native movement hook unavailable",
                t.movementHookReady
            );

            DrawSectionTitle("Combat");

            DrawTunableFeature(
                config,
                "Pistol Damage",
                "PistolDamage",
                &config.pistolDamageEnabled,
                &config.pistolDamageMultiplier,
                0.00f,
                100.00f,
                2.00f,
                "%.2fx",
                t.finalDamageHookReady
                    ? "Final outgoing-damage hook | BaseJuice > 0"
                    : "Final outgoing-damage hook unavailable",
                t.finalDamageHookReady
            );

            DrawTunableFeature(
                config,
                "Melee Damage",
                "MeleeDamage",
                &config.meleeDamageEnabled,
                &config.meleeDamageMultiplier,
                0.00f,
                100.00f,
                2.00f,
                "%.2fx",
                t.finalDamageHookReady
                    ? "Final outgoing-damage hook | zero-juice diagnostic"
                    : "Final outgoing-damage hook unavailable",
                t.finalDamageHookReady
            );

            DrawTunableFeature(
                config,
                "Hotstreak Charge",
                "HotstreakCharge",
                &config.hotstreakChargeEnabled,
                &config.hotstreakChargeMultiplier,
                0.00f,
                25.00f,
                2.00f,
                "%.2fx",
                t.hotstreakHookReady
                    ? "Runtime AddJuice hook | local positive gains"
                    : "Native AddJuice hook unavailable",
                t.hotstreakHookReady
            );

            if (ImGui::TreeNode("Combat diagnostics##V053")) {
                ImGui::TextDisabled(
                    "Pistol: %d events | final %.2f -> %.2f | juice %.2f",
                    t.pistolDamageBoostCalls, t.lastNativePistolDamage,
                    t.lastBoostedPistolDamage, t.lastPistolBaseJuice);
                ImGui::TextDisabled(
                    "Melee: %d events | final %.2f -> %.2f | scale %u",
                    t.meleeDamageBoostCalls, t.lastNativeBaseDamage,
                    t.lastBoostedBaseDamage, t.lastOutgoingScaleType);
                ImGui::TextDisabled(
                    "Hotstreak: %d boosts | %.2f -> %.2f",
                    t.hotstreakBoostCalls, t.lastNativeJuiceGain,
                    t.lastBoostedJuiceGain);
                ImGui::TreePop();
            }

            DrawSectionTitle("Horse");

            const horse::Telemetry horseTelemetry =
                horse::GetTelemetry();

            DrawTunableFeature(
                config,
                "Horse Speed",
                "HorseSpeed",
                &config.horseSpeedEnabled,
                &config.horseSpeedMultiplier,
                0.00f,
                3.00f,
                1.25f,
                "%.2fx",
                horseTelemetry.horseMovement
                    ? "HorseMovement: MaxWalkSpeed +0x1DC / MaxAcceleration +0x1F0"
                    : "Horse captured; waiting for native movement resolver",
                horseTelemetry.horseMovement != nullptr
            );

            DrawTunableFeature(
                config,
                "Horse Sprint Speed",
                "HorseSprintSpeed",
                &config.horseSprintSpeedEnabled,
                &config.horseSprintSpeedMultiplier,
                0.00f,
                3.00f,
                1.25f,
                "%.2fx",
                horseTelemetry.horseMovement
                    ? "HorseMovement: SprintingMaxSpeed +0x760"
                    : "Horse captured; waiting for native movement resolver",
                horseTelemetry.horseMovement != nullptr
            );

            DrawTunableFeature(
                config,
                "Horse Sprint Duration",
                "HorseSprintDuration",
                &config.horseSprintDurationEnabled,
                &config.horseSprintDurationMultiplier,
                0.00f,
                20.00f,
                5.00f,
                "%.2fx",
                horseTelemetry.staminaReady
                    ? "0x = vanilla | native StaminaSprintPercentageRate +0x918"
                    : "Waiting for native horse stamina fields",
                horseTelemetry.staminaReady
            );

            if (ImGui::TreeNode("Horse diagnostics##V053")) {
            ImGui::Indent();
            ImGui::TextDisabled(
                "Horse: %s | native horse calls %u | horses found %u",
                horseTelemetry.validated
                    ? "VALIDATED"
                    : "waiting",
                horseTelemetry.uniqueCandidatesLogged,
                horseTelemetry.candidateMatches
            );
            ImGui::TextDisabled(
                "MaxWalkSpeed %.1f -> %.1f | MaxAcceleration %.1f -> %.1f",
                horseTelemetry.nativeMaxWalkSpeed,
                horseTelemetry.appliedMaxWalkSpeed,
                horseTelemetry.nativeMaxAcceleration,
                horseTelemetry.appliedMaxAcceleration
            );
            ImGui::TextDisabled(
                "GetMaxSpeed %.1f -> %.1f | sprinting %s",
                horseTelemetry.nativeGetMaxSpeed,
                horseTelemetry.appliedGetMaxSpeed,
                horseTelemetry.sprinting ? "YES" : "NO"
            );
            ImGui::TextDisabled(
                "Sprint drain %.3f -> %.3f | player %p | horse %p | movement %p",
                horseTelemetry.nativeSprintDrain,
                horseTelemetry.appliedSprintDrain,
                horseTelemetry.playerOwner,
                horseTelemetry.horseOwner,
                horseTelemetry.horseMovement
            );
            ImGui::Unindent();
            ImGui::TreePop();
            }

            DrawSectionTitle("System");

            ImGui::Text("Graphics Adapter");
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled(
                "Engine.ini [SystemSettings] r.GraphicsAdapter | restart required"
            );

            ImGui::Indent();
            ImGui::SetNextItemWidth(245.0f);
            int selectedGraphicsAdapter =
                config.graphicsAdapter;
            const char* graphicsAdapters[] = {
                "0", "1", "2", "3", "4"
            };

            if (ImGui::Combo(
                    "Value##GraphicsAdapter",
                    &selectedGraphicsAdapter,
                    graphicsAdapters,
                    IM_ARRAYSIZE(graphicsAdapters))) {
                config.graphicsAdapter =
                    selectedGraphicsAdapter;
                config.Save();

                const bool applied =
                    c.applyGraphicsAdapter
                        ? c.applyGraphicsAdapter(
                            config.graphicsAdapter)
                        : false;

                *c.lastAction =
                    std::string("Graphics Adapter ") +
                    std::to_string(
                        config.graphicsAdapter) +
                    (applied
                        ? " written to Engine.ini (restart required)"
                        : " Engine.ini write failed");
            }
            ImGui::Unindent();

            if (ImGui::Checkbox(
                    "Skip Logos",
                    &config.skipLogosEnabled)) {
                config.Save();
                const bool applied =
                    c.applySkipLogos
                        ? c.applySkipLogos(
                            config.skipLogosEnabled)
                        : false;
                *c.lastAction =
                    std::string("Skip Logos ") +
                    (config.skipLogosEnabled ? "ON" : "OFF") +
                    (applied
                        ? " (restart required)"
                        : " (startup patch failed)");
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled(
                "%s | restart required",
                t.skipLogosPatched && t.skipLogosTargetValid ? "READY" : "WAIT"
            );
            if (ImGui::IsItemHovered()) ImGui::SetTooltip(
                "Only two exact logo names are substituted; MoviePlayer remains native.");

            if (ImGui::Checkbox(
                    "Skip Intro Videos",
                    &config.skipIntroEnabled)) {
                config.Save();
                if (c.applySkipIntro) {
                    c.applySkipIntro(true);
                }
                *c.lastAction =
                    std::string("Skip Intro ") +
                    (config.skipIntroEnabled ? "ON" : "OFF");
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled(
                "%s | restart required",
                t.skipIntroReady ? "READY" : "WAIT"
            );

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Native g.PlayIntroCinematicOnBoot; independent of Skip Logos");

            if (ImGui::Checkbox("Skip Warning", &config.skipWarningEnabled)) {
                config.Save();
                const bool accepted=c.applySkipWarning ?
                    c.applySkipWarning(config.skipWarningEnabled) : false;
                *c.lastAction=std::string("Skip Warning ")+
                    (config.skipWarningEnabled?"ON":"OFF")+
                    (accepted?" (restart required)":" (proxy unavailable)");
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled("%s | restart required",
                t.skipWarningAttempted ?
                    (t.skipWarningApplied?"APPLIED":"NO MATCH") :
                    (t.skipLogosTargetValid?"READY":"WAIT"));
            if (ImGui::IsItemHovered()) ImGui::SetTooltip(
                "Independent controller/autosave startup screen filter");

            ImGui::Spacing();
            ImGui::TextDisabled(
                "Runtime changes publish immediately; INI persistence is debounced."
            );

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Camera")) {
            ImGui::Spacing();
            DrawSectionTitle("Camera controls");
            ImGui::TextWrapped(
                "These controls use native camera hooks. They may not affect all "
                "gameplay, mounted, or cinematic cameras. Defaults preserve vanilla."
            );

            const dg::camera_trace::Telemetry camera = dg::camera_trace::GetTelemetry();
            ImGui::TextDisabled("Camera: %s | SpringArm: %s",
                camera.viewReady ? "READY" : "WAIT",
                camera.armReady ? "READY" : "WAIT");
            if (ImGui::TreeNode("Camera diagnostics##V053")) {
            ImGui::Text("GetCameraView: %s | calls: %u",
                camera.viewReady ? "READY" : "UNAVAILABLE", camera.viewCalls);
            ImGui::Text("SpringArm: %s | calls: %u",
                camera.armReady ? "READY" : "UNAVAILABLE", camera.armCalls);
            ImGui::TextDisabled("FOV observed %.1f -> %.1f | pitch %.1f -> %.1f",
                camera.nativeFov,camera.appliedFov,
                camera.nativePitch,camera.appliedPitch);
            ImGui::TextDisabled("Yaw observed %.1f -> %.1f",
                camera.nativeYaw, camera.appliedYaw);
            ImGui::TextDisabled("Arm length observed %.1f -> %.1f",
                camera.nativeArmLength,camera.appliedArmLength);
            ImGui::TextDisabled("Camera height Z %.1f -> %.1f",
                camera.nativeHeight,camera.appliedHeight);
                ImGui::TreePop();
            }

            DrawSectionTitle("Third Person camera");
            if (ImGui::Checkbox("Third Person", &config.thirdPersonEnabled)) {
                config.Save();
                *c.lastAction=config.thirdPersonEnabled?"Third Person ON":"Third Person OFF";
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled("%s",camera.viewReady && camera.armReady?"READY":"WAIT");
            if (config.thirdPersonEnabled) {
                ImGui::Indent();
                ImGui::SetNextItemWidth(245.0f);
                bool distanceChanged=ImGui::SliderFloat("Distance##ThirdPerson",
                        &config.thirdPersonDistanceMultiplier,0.25f,3.0f,"%.2fx");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(115.0f);
                distanceChanged |= ImGui::InputFloat("Manual##ThirdPersonDistance",
                    &config.thirdPersonDistanceMultiplier,0.0f,0.0f,"%.3fx");
                ImGui::SameLine();
                if (ImGui::Button("Default##ThirdPersonDistance")) {
                    config.thirdPersonDistanceMultiplier=1.0f;
                    distanceChanged=true;
                }
                if (distanceChanged) {
                    if (!std::isfinite(config.thirdPersonDistanceMultiplier))
                        config.thirdPersonDistanceMultiplier=1.0f;
                    config.thirdPersonDistanceMultiplier=std::clamp(
                        config.thirdPersonDistanceMultiplier,0.25f,3.0f);
                    config.Save();
                }
                ImGui::Unindent();
            }
            DrawSectionTitle("Field of view");
            DrawTunableFeature(config,"Enable FOV Override","FOV",
                &config.fovEnabled,&config.fovDegrees,
                40.0f,140.0f,90.0f,"%.0f deg",
                camera.viewReady ? "Native view output" : "Hook not ready",
                camera.viewReady);
            ImGui::TextDisabled("OFF restores the game's original FOV.");

            DrawSectionTitle("Camera distance");
            ImGui::TextWrapped("Zoom: positive = closer, negative = farther. 0%% = vanilla.");
            bool zoomChanged = false;
            if (ImGui::Button("Zoom -##Camera")) {
                config.cameraZoomPercent -= 10.0f; zoomChanged = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Zoom +##Camera")) {
                config.cameraZoomPercent += 10.0f; zoomChanged = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Vanilla##CameraZoom")) {
                config.cameraZoomPercent = 0.0f; zoomChanged = true;
            }
            ImGui::SetNextItemWidth(340.0f);
            zoomChanged |= ImGui::SliderFloat("Zoom##Camera",
                &config.cameraZoomPercent,-75.0f,200.0f,"%+.0f%%");
            ImGui::SetNextItemWidth(160.0f);
            zoomChanged |= ImGui::InputFloat("Manual Zoom##Camera",
                &config.cameraZoomPercent,0.0f,0.0f,"%.1f");
            if (zoomChanged) {
                config.cameraZoomPercent = std::clamp(config.cameraZoomPercent,-75.0f,200.0f);
                config.Save();
            }

            DrawSectionTitle("Camera height");
            ImGui::TextWrapped("Vertical camera offset in Unreal units. Zero = vanilla.");
            bool heightChanged = false;
            ImGui::SetNextItemWidth(340.0f);
            heightChanged |= ImGui::SliderFloat("Height##Camera",
                &config.cameraHeightOffset,-1500.0f,1500.0f,"%+.0f units");
            ImGui::SameLine();
            if (ImGui::Button("Vanilla##CameraHeight")) {
                config.cameraHeightOffset=0.0f; heightChanged=true;
            }
            ImGui::SetNextItemWidth(160.0f);
            heightChanged |= ImGui::InputFloat("Manual Height##Camera",
                &config.cameraHeightOffset,0.0f,0.0f,"%.1f");
            if (heightChanged) {
                config.cameraHeightOffset =
                    std::clamp(config.cameraHeightOffset,-1500.0f,1500.0f);
                config.Save();
            }

            DrawSectionTitle("Camera angle");
            ImGui::TextWrapped("Pitch offset relative to the native view. 0 degrees = vanilla.");
            bool pitchChanged = false;
            ImGui::SetNextItemWidth(340.0f);
            pitchChanged |= ImGui::SliderFloat("Pitch##Camera",
                &config.cameraPitchDegrees,-35.0f,35.0f,"%+.1f deg");
            ImGui::SameLine();
            if (ImGui::Button("Vanilla##CameraPitch")) {
                config.cameraPitchDegrees = 0.0f; pitchChanged = true;
            }
            if (pitchChanged) {
                config.cameraPitchDegrees = std::clamp(config.cameraPitchDegrees,-35.0f,35.0f);
                config.Save();
            }
            DrawSectionTitle("Horizontal camera rotation (yaw)");
            ImGui::TextWrapped("Turn the camera left/right. 0 degrees = vanilla.");
            bool yawChanged = false;
            ImGui::SetNextItemWidth(340.0f);
            yawChanged |= ImGui::SliderFloat("Yaw##Camera",
                &config.cameraYawDegrees,-180.0f,180.0f,"%+.1f deg");
            ImGui::SameLine();
            if (ImGui::Button("Vanilla##CameraYaw")) {
                config.cameraYawDegrees = 0.0f;
                yawChanged = true;
            }
            ImGui::SetNextItemWidth(160.0f);
            yawChanged |= ImGui::InputFloat("Manual Yaw##Camera",
                &config.cameraYawDegrees,0.0f,0.0f,"%.1f");
            if (yawChanged) {
                config.cameraYawDegrees =
                    std::clamp(config.cameraYawDegrees,-180.0f,180.0f);
                config.Save();
            }

            DrawSectionTitle("Camera keyboard bindings");
            ImGui::TextWrapped(
                "Default: Up/Down = camera height, Left/Right = zoom. "
                "Tilt actions are customizable; NumPad 4/6 rotate left/right (Num Lock ON). "
                "Press Rebind then the new key; Esc cancels. "
                "Bindings work only with the overlay closed and the game focused."
            );
            for (size_t i = 0; i < config.cameraKeys.size(); ++i) {
                ImGui::PushID(static_cast<int>(i) + 3000);
                ImGui::Text("%s:", config::kCameraLabels[i]);
                ImGui::SameLine(200.0f);
                ImGui::Text("%s", config::KeyDisplayName(config.cameraKeys[i]).c_str());
                ImGui::SameLine(340.0f);
                const bool capture = c.captureCameraKeyIndex->load() ==
                    static_cast<int>(i);
                if (ImGui::Button(capture ? "Press key... (Esc cancels)" : "Rebind")) {
                    c.captureMenuKey->store(false);
                    c.captureCameraKeyIndex->store(static_cast<int>(i));
                }
                ImGui::SameLine();
                if (ImGui::Button("Unbind")) {
                    config.cameraKeys[i] = 0;
                    c.captureCameraKeyIndex->store(-1);
                    config.Save();
                }
                ImGui::PopID();
            }
            ImGui::TextDisabled("Hold keys to repeat. Steps: height 50, zoom 10%%, pitch/yaw 5 degrees.");
            ImGui::TextDisabled("Camera-distance adjustments temporarily disable DOF blur.");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("The native r.DepthOfFieldQuality is restored when Zoom and Third Person are off.");
            ImGui::TextDisabled("Native output changes. Actual framing requires in-game verification.");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Hotkeys")) {
            ImGui::Spacing();

            ImGui::Text("Menu key");
            ImGui::Text(
                "Open / close: %s",
                menuKeyName.c_str()
            );
            ImGui::SameLine(280.0f);

            if (c.captureMenuKey->load()) {
                ImGui::TextDisabled(
                    "Press a key...  Esc = cancel"
                );
            } else if (ImGui::Button(
                    "Rebind Menu Key",
                    ImVec2(160.0f, 0.0f))) {
                c.captureMenuKey->store(true);
                *c.lastAction =
                    "Waiting for new menu key";
                if (c.log) {
                    c.log("Menu key capture started");
                }
            }

            ImGui::Spacing();

            if (ImGui::Button(
                    "Save",
                    ImVec2(110.0f, 0.0f))) {
                config.SaveNow();
                *c.lastAction =
                    "Configuration saved";
            }

            ImGui::SameLine();

            if (ImGui::Button(
                    "Reload",
                    ImVec2(110.0f, 0.0f))) {
                config.Load();
                c.captureCameraKeyIndex->store(-1);
                *c.lastAction =
                    "Configuration reloaded";
            }

            ImGui::SameLine();

            if (ImGui::Button(
                    "Reset Defaults",
                    ImVec2(140.0f, 0.0f))) {
                config.ResetDefaults(true);
                c.captureCameraKeyIndex->store(-1);
                c.hudHidden->store(false);
                *c.lastAction =
                    "Defaults restored";
            }

            DrawSectionTitle("F1-F12");

            ImGui::TextWrapped(
                "Each physical F-key slot can be reassigned to any implemented mod action or None."
            );
            ImGui::Spacing();

            if (ImGui::BeginTable(
                    "HotkeyTable",
                    2,
                    ImGuiTableFlags_BordersInnerH |
                        ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn(
                    "Key",
                    ImGuiTableColumnFlags_WidthFixed,
                    90.0f
                );
                ImGui::TableSetupColumn(
                    "Action",
                    ImGuiTableColumnFlags_WidthStretch
                );
                ImGui::TableHeadersRow();

                for (int i = 0; i < 12; ++i) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("F%d", i + 1);

                    ImGui::TableSetColumnIndex(1);

                    char comboId[32]{};
                    sprintf_s(
                        comboId,
                        sizeof(comboId),
                        "##HotkeyF%d",
                        i + 1
                    );

                    int current = static_cast<int>(
                        config.hotkeys[
                            static_cast<std::size_t>(i)]
                    );

                    if (current < 0 ||
                        current >=
                            static_cast<int>(
                                config::Action::Count)) {
                        current = 0;
                    }

                    ImGui::SetNextItemWidth(-1.0f);

                    if (ImGui::BeginCombo(
                            comboId,
                            config::kActionLabels[
                                static_cast<std::size_t>(
                                    current)])) {
                        for (int action = 0;
                             action <
                                 static_cast<int>(
                                     config::Action::Count);
                             ++action) {
                            const bool selected =
                                current == action;

                            if (ImGui::Selectable(
                                    config::kActionLabels[
                                        static_cast<std::size_t>(
                                            action)],
                                    selected)) {
                                config.hotkeys[
                                    static_cast<std::size_t>(i)] =
                                    static_cast<config::Action>(
                                        action);
                                config.Save();
                            }

                            if (selected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }

                        ImGui::EndCombo();
                    }
                }

                ImGui::EndTable();
            }

            ImGui::Spacing();
            ImGui::TextDisabled(
                "Defaults: F1 HUD | F2 Speed | F3 Recovery | F4 Reticle | F5 Third Person | F6-F12 None"
            );
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("About")) {
            ImGui::Spacing();
            ImGui::Text(
                "Darksiders Genesis Enhanced - cleaned ASI core"
            );
            ImGui::Spacing();
            ImGui::TextWrapped(
                "Target executable: DarksidersGenesis-Win64-Shipping.exe"
            );
            ImGui::Text(
                "Target validation: %s",
                c.targetValidation->exact
                    ? "EXACT / gameplay enabled"
                    : "MISMATCH / overlay-only"
            );
            ImGui::TextWrapped(
                "Runtime SHA-256: %s",
                c.targetValidation->sha256.empty()
                    ? "(not available)"
                    : c.targetValidation->sha256.c_str()
            );
            ImGui::TextWrapped(
                "Expected SHA-256: 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54"
            );
            ImGui::TextWrapped(
                "Expected size: 62,113,280 bytes"
            );
            ImGui::Spacing();
            ImGui::TextWrapped(
                "V0.17 separates target validation, player identity, runtime settings, configuration, horse logic and overlay UI from the hook core."
            );
            ImGui::TextWrapped(
                "Unknown executables are fail-closed: overlay/log remain available but gameplay hooks are not installed."
            );
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Separator();
    ImGui::Text(
        "Last action: %s",
        c.lastAction->c_str()
    );

    ImGui::End();

    if (!open) {
        c.overlayVisible->store(false);
    }
}

} // namespace dg::overlay
