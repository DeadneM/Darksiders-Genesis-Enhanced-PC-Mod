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
        ImGui::TextDisabled("%s", hookReady ? "Hook ready" : "Hook unavailable");
        if (note) { ImGui::Indent(); ImGui::TextDisabled("%s",note); ImGui::Unindent(); }
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
    ImGui::TextDisabled("%s", hookReady ? "Hook ready" : "Hook unavailable");
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

    if (note) ImGui::TextDisabled("%s", note);
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

            DrawSectionTitle("HUD and aiming effects");
            ImGui::TextDisabled("Statuses reflect real game hooks and observed events, not just ON/OFF switches.");

            bool hudHidden=c.hudHidden->load();
            if (ImGui::Checkbox("HUD Hidden", &hudHidden)) {
                c.hudHidden->store(hudHidden);
                *c.lastAction=hudHidden?"HUD hidden":"HUD visible";
                if (c.log) c.log("Overlay -> HUD %s",hudHidden?"HIDDEN":"VISIBLE");
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled("%s | F1",t.hudHookReady?"Hook ready":"Unavailable");
            ImGui::TextDisabled("Hides normal on-screen HUD. The blue 3D aim line/arc are separate.");

            if (ImGui::Checkbox("Hide Reticle (independent of HUD)", &config.hideReticle)) {
                config.Save();
                *c.lastAction = config.hideReticle
                    ? "Hide Reticle ON (close overlay to apply)"
                    : "Hide Reticle OFF (close overlay to restore)";
                if (c.log) c.log("Overlay -> Hide Reticle %s", config.hideReticle ? "ON" : "OFF");
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled("%s", t.reticleCursorHookReady
                ? "Cursor hide hook ready" : "Cursor hide hook unavailable");
            ImGui::TextWrapped(
                "Hides the aiming pointer in gameplay without hiding the HUD. "
                "The pointer stays visible here for menu interaction."
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
            ImGui::Text("Blue aiming line and semicircle");
            constexpr const char* kAimModes[]={
                "Hide Everywhere", "Default Only", "TPS Only"
            };
            ImGui::SetNextItemWidth(260.0f);
            if(ImGui::Combo("Visibility mode##AimIndicator",
                    &config.aimIndicatorHideMode,kAimModes,3))config.Save();
            ImGui::TextDisabled("TPS Only is the default. Default Only means normal/non-TPS camera.");
            ImGui::TextWrapped("Visual shader still unidentified: the chosen mode is saved, "
                "but disappearance of the blue line and semicircle is NOT confirmed.");
            ImGui::TextDisabled("Old ground-FX hook calls observed: %u (0 = not the correct effect)",
                t.aimNativeFxCalls);
            DrawSectionTitle("Player");
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
                "Changes walking and running speed for the player.",
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
                    ? "Hook ready" : "Hook unavailable"
            );
            ImGui::TextDisabled("Reduces the delay before moving after actions. Unlocks observed: %d",
                t.actionMoveForced);
            if (ImGui::TreeNode("More recovery details")) {
                ImGui::TextDisabled("Movement checks: %d | player checks: %d | native blocks: %d",
                    t.actionMoveQueries,t.actionMoveLocalQueries,t.actionMoveNativeBlocked);
                ImGui::TreePop();
            }

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
                "Adjusts both normal and double-jump height.",
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
                "Changes how long the character can glide.",
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
                "Scales outgoing pistol-like hits; verified adjustments shown below.",
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
                "Scales melee-like hits (heuristic; may also affect some abilities).",
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
                "Multiplies earned Hotstreak charge.",
                t.hotstreakHookReady
            );

            ImGui::TextDisabled("Observed in-game: pistol %d | melee-like %d | Hotstreak %d",
                t.pistolDamageBoostCalls,t.meleeDamageBoostCalls,t.hotstreakBoostCalls);
            if (ImGui::TreeNode("Advanced combat diagnostics")) {
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
            if(ImGui::Checkbox("Horse Everywhere (experimental research)",
                    &config.horseEverywhereEnabled)) config.Save();
            ImGui::TextDisabled("Experimental: tests the native horse-area checks on H / LB+RB only.");
            ImGui::TextDisabled("%s | server requests observed: %u (while selected: %u)",
                horseTelemetry.summonProbeReady ? "Detection ready; bypass NOT implemented" :
                    "Detection unavailable; bypass NOT implemented",
                horseTelemetry.summonServerCalls,
                horseTelemetry.summonServerCallsWhileRequested);
            ImGui::TextDisabled("Turning this ON does not yet remove zone restrictions. No forced spawning.");

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
                    ? "Horse movement data observed during riding."
                    : "Waiting for a horse to be summoned.",
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
                    ? "Horse sprint data available."
                    : "Waiting for a horse to be summoned.",
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
                    ? "Adjusts how slowly the horse's stamina drains. 0 = original game."
                    : "Waiting for horse stamina data.",
                horseTelemetry.staminaReady
            );

            ImGui::TextDisabled("Horse seen: %s | native activity: %u | horses detected: %u",
                horseTelemetry.validated?"YES":"NOT YET",
                horseTelemetry.uniqueCandidatesLogged,horseTelemetry.candidateMatches);
            if (ImGui::TreeNode("Advanced horse diagnostics")) {
            ImGui::Indent();
            ImGui::TextDisabled(
                "Native horse state: %s | calls %u | discovered %u",
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
                "Select which GPU the game uses. Requires a restart to take effect."
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
                t.skipLogosPatched && t.skipLogosTargetValid ? "Boot patch ready" : "Unavailable"
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
                t.skipIntroReady ? "Boot setting ready" : "Unavailable"
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
                    (t.skipWarningApplied?"Applied at boot":"No matching screen") :
                    (t.skipLogosTargetValid?"Available":"Unavailable"));
            if (ImGui::IsItemHovered()) ImGui::SetTooltip(
                "Independent controller/autosave startup screen filter");

            ImGui::Spacing();
            ImGui::TextDisabled("Skip Logos / Intro / Warning take effect after a game restart.");

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
            ImGui::TextDisabled("Camera: %s | native camera frames observed: %u",
                camera.viewReady && camera.armReady ? "Hook ready" : "Hook unavailable",
                camera.viewCalls);
            if (ImGui::TreeNode("Advanced camera diagnostics")) {
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
            ImGui::TextDisabled("%s | TPS camera attachments observed: %u",
                camera.viewReady && camera.armReady?"Hook ready":"Hook unavailable",
                camera.tpsAttachedFrames);
            ImGui::TextDisabled("Moves the camera behind the player. F5 or long L3 toggles it.");
            if (config.thirdPersonEnabled) {
                ImGui::Indent();
                DrawSectionTitle("TPS camera attachment");
                if(ImGui::Checkbox("Attach TPS camera to player",&config.tpsFollowPlayer))
                    config.Save();
                ImGui::TextDisabled("TPS pivot follows player feet on the character axis.");
                ImGui::SetNextItemWidth(245.0f);
                if(ImGui::SliderFloat("Foot origin correction##TPS",&config.tpsFootAnchorOffset,
                        -500.0f,500.0f,"%+.0f units")) config.Save();
                ImGui::SameLine();
                if(ImGui::Button("Default##TPSFoot")) {
                    config.tpsFootAnchorOffset=0.0f;config.Save();
                }
                ImGui::TextDisabled("Approximate root-to-foot height; adjust for the character model.");
                if(ImGui::TreeNode("Advanced attachment counters")){
                    ImGui::TextDisabled("Attached: %u | zone overrides: %u | deferred: %u",
                        camera.tpsAttachedFrames,camera.nativeZoneOverridesIgnored,
                        camera.tpsAttachDeferred);
                    ImGui::TreePop();
                }
                DrawSectionTitle("TPS-only FOV");
                if(ImGui::Checkbox("Enable TPS FOV override",&config.thirdPersonFovEnabled))
                    config.Save();
                if(config.thirdPersonFovEnabled) {
                    ImGui::SetNextItemWidth(245.0f);
                    if(ImGui::SliderFloat("FOV##TPS",&config.thirdPersonFovDegrees,40.0f,140.0f,"%.0f deg"))
                        config.Save();
                    ImGui::SameLine();
                    if(ImGui::Button("Default##TPSFOV")) {
                        config.thirdPersonFovDegrees=110.0f;config.Save();
                    }
                }
                ImGui::TextDisabled("Only applies when both Third Person and TPS FOV are ON.");
                ImGui::SetNextItemWidth(245.0f);
                bool distanceChanged=ImGui::SliderFloat("Distance##ThirdPerson",
                        &config.thirdPersonDistanceMultiplier,0.0f,3.0f,"%.2fx");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(115.0f);
                distanceChanged |= ImGui::InputFloat("Manual##ThirdPersonDistance",
                    &config.thirdPersonDistanceMultiplier,0.0f,0.0f,"%.3fx");
                ImGui::SameLine();
                if (ImGui::Button("Default##ThirdPersonDistance")) {
                    config.thirdPersonDistanceMultiplier=0.20f;
                    distanceChanged=true;
                }
                if (distanceChanged) {
                    if (!std::isfinite(config.thirdPersonDistanceMultiplier))
                        config.thirdPersonDistanceMultiplier=0.20f;
                    config.thirdPersonDistanceMultiplier=std::clamp(
                        config.thirdPersonDistanceMultiplier,0.0f,3.0f);
                    config.Save();
                }

                bool poseChanged=false;
                ImGui::TextDisabled("Third Person pose (previously fixed)");
                ImGui::SetNextItemWidth(245.0f);
                poseChanged |= ImGui::SliderFloat("View pitch##TP",&config.thirdPersonPitchDegrees,-75.0f,65.0f,"%.1f deg");
                ImGui::SameLine();
                if (ImGui::Button("Default##TPPitch")) {config.thirdPersonPitchDegrees=-12.0f;poseChanged=true;}
                ImGui::SetNextItemWidth(145.0f);
                poseChanged |= ImGui::InputFloat("Manual pitch##TP",&config.thirdPersonPitchDegrees,0,0,"%.1f");
                ImGui::SetNextItemWidth(245.0f);
                poseChanged |= ImGui::SliderFloat("Vertical offset##TP",&config.thirdPersonHeightOffset,-1000.0f,1000.0f,"%+.0f units");
                ImGui::SameLine();
                if (ImGui::Button("Default##TPHeight")) {config.thirdPersonHeightOffset=120.0f;poseChanged=true;}
                ImGui::SetNextItemWidth(145.0f);
                poseChanged |= ImGui::InputFloat("Manual height##TP",&config.thirdPersonHeightOffset,0,0,"%.1f");
                if (poseChanged) {
                    if (!std::isfinite(config.thirdPersonPitchDegrees)) config.thirdPersonPitchDegrees=-12.0f;
                    if (!std::isfinite(config.thirdPersonHeightOffset)) config.thirdPersonHeightOffset=120.0f;
                    config.thirdPersonPitchDegrees=std::clamp(config.thirdPersonPitchDegrees,-75.0f,65.0f);
                    config.thirdPersonHeightOffset=std::clamp(config.thirdPersonHeightOffset,-1000.0f,1000.0f);
                    config.Save();
                }
                ImGui::Spacing();
                if(ImGui::Checkbox("Control camera with mouse##TPS",&config.cameraMouseOrbitEnabled)){
                    config.cameraOrbitInputEnabled=config.cameraMouseOrbitEnabled||config.cameraControllerOrbitEnabled;
                    config.Save();
                }
                if(config.cameraMouseOrbitEnabled){
                    ImGui::SetNextItemWidth(245.0f);
                    if(ImGui::SliderFloat("Mouse sensitivity##TP",&config.cameraMouseSensitivity,0.01f,0.75f,"%.2f deg/px"))
                        config.Save();
                }
                if(ImGui::Checkbox("Control camera with controller right stick##TPS",&config.cameraControllerOrbitEnabled)){
                    config.cameraOrbitInputEnabled=config.cameraMouseOrbitEnabled||config.cameraControllerOrbitEnabled;
                    config.Save();
                }
                if(config.cameraControllerOrbitEnabled){
                    ImGui::SetNextItemWidth(245.0f);
                    if(ImGui::SliderFloat("Stick speed##TP",&config.cameraStickSpeed,30.0f,360.0f,"%.0f deg/s"))
                        config.Save();
                }
                ImGui::TextDisabled("Only Third Person, focused game, closed overlay.");
                ImGui::TextDisabled("Right stick filter below is separate from orbit enable.");
                ImGui::Unindent();
            }
            if (config.thirdPersonEnabled) {
                ImGui::Indent();
                DrawSectionTitle("TPS camera controls");
                if (ImGui::Checkbox("Block vanilla right-stick player rotation (even while firing)",
                        &config.tpsSuppressNativeRightStick)) config.Save();
                ImGui::TextDisabled("TPS mouse uses native flat-plane targeting (3D aim not yet fixed).");
                ImGui::TextDisabled("No fixed crosshair or simulated aim.");
                ImGui::Separator();
                ImGui::TextDisabled("Firing follows the TPS camera direction while keeping native projectile physics.");
                ImGui::TextDisabled("Controller / camera right stick");
                if (ImGui::Checkbox("Strafe while firing (player movement)##PadTPS",
                        &config.tpsControllerStrafeWhileFiring)) config.Save();
                if (ImGui::Checkbox("Rotate on foot while firing (camera-forward shooting)##PadTPS",
                        &config.tpsControllerRotateWhileFiring)) config.Save();
                ImGui::TextDisabled("Strafe changes movement while shooting; Rotate makes shots camera-forward.");
                ImGui::Separator();
                ImGui::TextDisabled("Mounted TPS firing mode (overrides on-foot options)");
                constexpr const char* kMountedModes[]={
                    "Camera Forward (free camera, shots follow view)",
                    "Vanilla Aim (camera locked while firing)"
                };
                ImGui::SetNextItemWidth(510.0f);
                if(ImGui::Combo("Aim while mounted##MountedMode",
                    &config.tpsMountedAimMode,kMountedModes,2))config.Save();
                ImGui::TextWrapped(config.tpsMountedAimMode==0
                    ? "Default: move TPS camera freely while shooting on horseback; shots use camera direction."
                    : "While firing on horseback, TPS orbit stops and native aim takes over. Orbit resumes on release.");
                ImGui::TextDisabled("Mounted camera frames while firing: %u",
                    camera.mountedFireHorseSelected);
                ImGui::Separator();
                ImGui::Text("Controller bindings");
                ImGui::TextDisabled("One button per action. OFF disables the mod hotkey.");
                constexpr int kButtons[]={
                    0,0x0040,0x0080,0x0100,0x0200,0x10000,0x20000,
                    0x1000,0x2000,0x4000,0x8000,0x0001,0x0002,
                    0x0004,0x0008,0x0010,0x0020
                };
                constexpr const char* kButtonLabels[]={
                    "OFF","L3 (Left Stick Click)","R3 (Right Stick Click)",
                    "LB / L1","RB / R1","LT / L2","RT / R2",
                    "A / Cross","B / Circle","X / Square","Y / Triangle",
                    "D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right",
                    "Start / Menu","Back / View"
                };
                constexpr int kButtonCount=sizeof(kButtons)/sizeof(kButtons[0]);
                auto BindGamepadMask=[&](const char* title,int& mask) {
                    int index=0;
                    for(int i=0;i<kButtonCount;++i)
                        if(mask==kButtons[i]){index=i;break;}
                    ImGui::SetNextItemWidth(285.0f);
                    if(ImGui::Combo(title,&index,kButtonLabels,kButtonCount)){
                        mask=kButtons[index];config.Save();
                    }
                };
                BindGamepadMask("Grenade: hold equip / release stow##TPS",config.tpsGrenadeButtonMask);
                BindGamepadMask("Hold for temporary vanilla camera##TPS",config.tpsSuspendButtonMask);
                if(config.tpsSuspendButtonMask!=0){
                    ImGui::SetNextItemWidth(245.0f);
                    if(ImGui::SliderInt("Temporary view delay##HoldTPS",
                        &config.tpsSuspendDelayMs,0,3000,"%d ms"))config.Save();
                }
                BindGamepadMask("Long press: TPS ON/OFF##TPS",config.tpsToggleHoldButtonMask);
                if(config.tpsToggleHoldButtonMask!=0){
                    ImGui::SetNextItemWidth(245.0f);
                    if(ImGui::SliderInt("Long press delay##ToggleTPS",
                        &config.tpsToggleHoldMs,250,2000,"%d ms"))config.Save();
                }
                BindGamepadMask("Recenter TPS behind player##TPS",config.tpsRecenterButtonMask);
                ImGui::TextDisabled("Default R3 grenade/temporary view, L3 short recenter/long TPS toggle.");
                ImGui::TextDisabled("Grenade stow pulses: %u | DOWN %u / UP %u",
                    t.grenadeNativeReleasePulses,t.grenadePhysicalDown,t.grenadePhysicalUp);
                if(config.tpsGrenadeButtonMask!=0 &&
                   (config.tpsGrenadeButtonMask==config.tpsToggleHoldButtonMask ||
                    config.tpsGrenadeButtonMask==config.tpsSuspendButtonMask ||
                    config.tpsGrenadeButtonMask==config.tpsRecenterButtonMask))
                    ImGui::TextDisabled("Shared binding: one button triggers multiple actions.");
                if(config.tpsToggleHoldButtonMask!=0 &&
                   config.tpsToggleHoldButtonMask==config.tpsSuspendButtonMask)
                    ImGui::TextDisabled("Long TPS and temporary view share one button.");
                ImGui::TextWrapped("Native game actions remain unless deliberately remapped "
                    "for grenade. LT/RT and other game buttons can conflict with "
                    "their original actions. OFF disables the mod shortcut only.");
                ImGui::Unindent();
            }
            ImGui::TextDisabled("Normal aiming and camera return when TPS is OFF.");
            DrawSectionTitle("Field of view");
            DrawTunableFeature(config,"Enable FOV Override","FOV",
                &config.fovEnabled,&config.fovDegrees,
                40.0f,140.0f,90.0f,"%.0f deg",
                camera.viewReady ? "Native view output" : "Hook not ready",
                camera.viewReady);
            ImGui::TextDisabled("Global FOV applies outside TPS, or when TPS FOV is OFF.");

            DrawSectionTitle("Camera distance");
            ImGui::TextWrapped("SpringArm Zoom is mainly for vanilla. TPS keeps its own camera distance.");
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
            ImGui::TextWrapped("Extra global height. In attached TPS it is ADDED to Vertical offset##TP.");
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
            ImGui::TextWrapped("Native-view pitch offset: mainly vanilla. TPS uses View pitch##TP, overriding this setting.");
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
            if(ImGui::TreeNode("Advanced executable details")){
                ImGui::TextWrapped("Detected SHA-256: %s",
                    c.targetValidation->sha256.empty()?"unknown":
                    c.targetValidation->sha256.c_str());
                ImGui::TextDisabled("Supported EXE SHA-256: 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54");
                ImGui::TextDisabled("Supported EXE size: 62,113,280 bytes");
                ImGui::TreePop();
            }
            ImGui::Spacing();
            ImGui::TextWrapped("Options show what they do and whether their hooks are ready. Advanced details are optional.");
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
