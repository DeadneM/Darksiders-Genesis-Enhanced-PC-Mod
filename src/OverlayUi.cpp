#include "OverlayUi.h"
#include "GaugeChargeFeature.h"
#include "ShaderAimFeature.h"

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
            const auto aimShaders = dg::aim_shader::GetTelemetry();
            ImGui::TextWrapped("V0.91: two user-validated shader IDs suppress the blue aim line and arc. "
                "The same GPU draw gating and selected visibility mode are preserved from V0.90.");
            ImGui::TextDisabled("D3D11 shader hooks %s | matching shaders %u | suppressed draws %u",
                aimShaders.hooksReady ? "READY" : "UNAVAILABLE",
                aimShaders.matchedShaders, aimShaders.blockedDraws);
            ImGui::TextDisabled("Ground FX calls: %u | Target UI events: %u | Position UI events: %u",
                t.aimNativeFxCalls,t.aimUiTargetingEvents,t.aimUiPositionEvents);
            ImGui::TextDisabled("Target UI probe is passive: no visual or gameplay event is blocked.");
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
                100.00f,
                2.00f,
                "%.2fx",
                "Multiplies earned Hotstreak charge.",
                t.hotstreakHookReady
            );

            { // V1.03: three always-visible independent native gain controls.
                DrawSectionTitle("Wrath / Ultimate Form / Synergy");
                const auto charge=dg::gauges::GetTelemetry();
                ImGui::TextDisabled("Native hooks: Wrath %s | Ultimate/Chaos %s | Synergy %s",
                    charge.wrathReady?"READY":"UNAVAILABLE",
                    charge.chaosReady?"READY":"UNAVAILABLE",
                    charge.synergyReady?"READY":"UNAVAILABLE");
                ImGui::TextWrapped("ON by default: these native resource hooks are experimental. Verify in combat.");
                auto GaugeRow=[&](const char* label,const char* id,bool& enabled,float& value){
                    if(ImGui::Checkbox(label,&enabled))config.Save();
                    ImGui::PushID(id);
                    bool changed=false;
                    ImGui::SetNextItemWidth(220.0f);
                    changed|=ImGui::SliderFloat("##Slider",&value,0.0f,100.0f,"%.2fx");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(115.0f);
                    changed|=ImGui::InputFloat("##Manual",&value,0.0f,0.0f,"%.3f");
                    ImGui::SameLine();
                    if(ImGui::Button("Default")){value=2.0f;changed=true;}
                    if(changed){
                        if(!std::isfinite(value))value=2.0f;
                        value=std::clamp(value,0.0f,100.0f);
                        config.Save();
                    }
                    ImGui::PopID();
                };
                GaugeRow("Wrath charge","wrath",config.wrathChargeEnabled,config.wrathChargeMultiplier);
                GaugeRow("Ultimate Form / Chaos charge","chaos",config.ultimateChargeEnabled,config.ultimateChargeMultiplier);
                GaugeRow("Synergy charge","synergy",config.synergyChargeEnabled,config.synergyChargeMultiplier);
                ImGui::TextDisabled("Calls W/C/S: %u/%u/%u | multiplied %u/%u/%u",
                    charge.wrathObserved,charge.chaosObserved,charge.synergyObserved,
                    charge.wrathBoostCalls,charge.chaosBoostCalls,charge.synergyBoostCalls);
            }

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
            constexpr const char* horseAreas[]={
                "Off", "In all area", "Restricted only", "Banned zone only"
            };
            ImGui::SetNextItemWidth(285.0f);
            if(ImGui::Combo("Horse Everywhere##HorseAreaMode",
                    &config.horseAreaMode,horseAreas,4)){
                config.horseEverywhereEnabled=config.horseAreaMode!=0;
                config.Save();
            }
            ImGui::TextDisabled("Default: Restricted only. In all area also enables native dismount protection.");
            ImGui::TextDisabled("Restricted = no native desummon flag; banned = native desummon flag.");
            ImGui::TextDisabled("Horse summon icon visibility is controlled in the game options.");
            ImGui::TextDisabled("Keyboard and both pad buttons trigger the same native horse action.");
            ImGui::TextDisabled("The two controller buttons must be pressed simultaneously.");
            ImGui::PushID("horse_kbd");
            const size_t horseIndex=static_cast<size_t>(config::TpsKeyboardAction::Horse);
            const int horseBindingIndex=static_cast<int>(config.cameraKeys.size()+horseIndex);
            ImGui::Text("Keyboard:");
            ImGui::SameLine(225.0f);
            ImGui::Text("%s",config::KeyDisplayName(config.tpsKeyboardKeys[horseIndex]).c_str());
            ImGui::SameLine(335.0f);
            const bool horseCapture=c.captureCameraKeyIndex->load()==horseBindingIndex;
            if(ImGui::Button(horseCapture?"Press key...":"Rebind")){
                c.captureMenuKey->store(false);
                c.captureCameraKeyIndex->store(horseBindingIndex);
            }
            ImGui::SameLine();
            if(ImGui::Button("Unbind")){
                config.tpsKeyboardKeys[horseIndex]=0;
                c.captureCameraKeyIndex->store(-1);
                config.Save();
            }
            ImGui::SameLine();
            if(ImGui::SmallButton("Default##HorseKey")){
                config.tpsKeyboardKeys[horseIndex]=config::kDefaultTpsKeyboardKeys[horseIndex];
                c.captureCameraKeyIndex->store(-1);
                config.Save();
            }
            ImGui::PopID();
            constexpr int horsePadValues[]={0x0100,0x0200,0x0040,0x0080,
                0x1000,0x2000,0x4000,0x8000,0x0001,0x0002,0x0004,0x0008,
                0x0010,0x0020,0x10000,0x20000};
            constexpr const char* horsePadNames[]={"LB / L1","RB / R1",
                "L3","R3","A / Cross","B / Circle","X / Square","Y / Triangle",
                "D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right",
                "Start","Back","LT / L2","RT / R2"};
            auto HorsePadCombo=[&](const char* label,int& mask){
                int index=0;
                for(int i=0;i<16;++i)if(horsePadValues[i]==mask){index=i;break;}
                ImGui::SetNextItemWidth(270.0f);
                if(ImGui::Combo(label,&index,horsePadNames,16)){
                    mask=horsePadValues[index];config.Save();
                }
            };
            HorsePadCombo("Controller button 1##Horse",config.horsePadButtonA);
            ImGui::SameLine();
            if(ImGui::SmallButton("Default##HorsePadA")){config.horsePadButtonA=256;config.Save();}
            HorsePadCombo("Controller button 2##Horse",config.horsePadButtonB);
            ImGui::SameLine();
            if(ImGui::SmallButton("Default##HorsePadB")){config.horsePadButtonB=512;config.Save();}
            if(config.horsePadButtonA==config.horsePadButtonB)
                ImGui::TextDisabled("Choose two DIFFERENT buttons.");
            if(ImGui::TreeNode("Horse Everywhere information / debug")){
                ImGui::TextDisabled("Native spawn requests %u | while enabled %u",
                    horseTelemetry.summonServerCalls,horseTelemetry.summonServerCallsWhileRequested);
                ImGui::TextDisabled("Region permission %u/%u",
                    horseTelemetry.regionOverrides,horseTelemetry.regionQueries);
                ImGui::TextDisabled("Desummon prevented %u/%u",
                    horseTelemetry.desummonOverrides,horseTelemetry.desummonQueries);
                ImGui::TextDisabled("Separate mission-script desummons may still apply.");
                ImGui::TreePop();
            }

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

            ImGui::TextDisabled("Skip Warning: TEMPORARILY DISABLED for reticle/crosshair safety.");
            ImGui::TextDisabled("Native warning patch is never installed in V1.04.");
            ImGui::TextDisabled("Skip Logos and Skip Intro are unaffected.");

            ImGui::Spacing();
            ImGui::TextDisabled("Skip Logos / Intro take effect after a game restart.");

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
                ImGui::TextDisabled("TPS follows the live local player permanently.");
                ImGui::TextDisabled("TPS pivot follows player feet on the character axis.");
                ImGui::TextDisabled("Foot Origin appears with other TPS camera values below.");
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
                    config.thirdPersonDistanceMultiplier=0.15f;
                    distanceChanged=true;
                }
                if (distanceChanged) {
                    if (!std::isfinite(config.thirdPersonDistanceMultiplier))
                        config.thirdPersonDistanceMultiplier=0.15f;
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
                if (ImGui::Button("Default##TPHeight")) {config.thirdPersonHeightOffset=0.0f;poseChanged=true;}
                ImGui::SetNextItemWidth(145.0f);
                poseChanged |= ImGui::InputFloat("Manual height##TP",&config.thirdPersonHeightOffset,0,0,"%.1f");
                if (poseChanged) {
                    if (!std::isfinite(config.thirdPersonPitchDegrees)) config.thirdPersonPitchDegrees=-12.0f;
                    if (!std::isfinite(config.thirdPersonHeightOffset)) config.thirdPersonHeightOffset=0.0f;
                    config.thirdPersonPitchDegrees=std::clamp(config.thirdPersonPitchDegrees,-75.0f,65.0f);
                    config.thirdPersonHeightOffset=std::clamp(config.thirdPersonHeightOffset,-1000.0f,1000.0f);
                    config.Save();
                }
                ImGui::Spacing();
                constexpr const char* stickOptions[]={"OFF","Left Stick","Right Stick"};
                ImGui::SetNextItemWidth(285.0f);
                if(ImGui::Combo("TPS camera joystick",&config.tpsCameraStickMode,stickOptions,3)){
                    config.cameraControllerOrbitEnabled=config.tpsCameraStickMode!=0;
                    config.cameraOrbitInputEnabled=config.cameraControllerOrbitEnabled;
                    config.Save();
                }
                if(config.tpsCameraStickMode!=0){
                    ImGui::SetNextItemWidth(245.0f);
                    if(ImGui::SliderFloat("Stick speed##TP",&config.cameraStickSpeed,30.0f,360.0f,"%.0f deg/s"))
                        config.Save();
                }
                ImGui::TextDisabled("Only Third Person, focused game, closed overlay.");
                ImGui::TextDisabled("Right Stick default. OFF is native aim; Left shares movement.");
                ImGui::SetNextItemWidth(245.0f);
                if(ImGui::SliderFloat("Foot origin correction##TPS",&config.tpsFootAnchorOffset,
                   -500.0f,500.0f,"%+.0f units"))config.Save();
                ImGui::SameLine();
                if(ImGui::Button("Default##TPSFoot")){config.tpsFootAnchorOffset=0.0f;config.Save();}
                if(ImGui::Button("TPS Camera Values Default"))config.ResetTpsCameraValues();
                ImGui::Unindent();
            }
            if (config.thirdPersonEnabled) {
                ImGui::Indent();
                DrawSectionTitle("TPS controls and aiming");
                ImGui::TextDisabled("Controller-only TPS experiment: native mouse input is untouched.");
                if(ImGui::Checkbox("Auto suspend TPS during detected cutscenes (test)",
                    &config.tpsCutsceneAutoSuspend))config.Save();
                ImGui::TextDisabled("ON: native view + no TPS input during detected cinematics.");
                ImGui::TextDisabled("OFF: existing camera heuristic only.");
                ImGui::Separator();
                ImGui::TextDisabled("Native movement and camera isolation are automatic in TPS.");
                ImGui::Separator();
                ImGui::TextDisabled("Unified TPS aiming (on foot + mounted)");
                constexpr const char* kMountedModes[]={
                    "Camera Forward (free camera while firing)",
                    "Vanilla Aim (pause camera while firing)"
                };
                ImGui::SetNextItemWidth(510.0f);
                if(ImGui::Combo("TPS aiming mode##MountedMode",
                    &config.tpsMountedAimMode,kMountedModes,2))config.Save();
                ImGui::TextWrapped(config.tpsMountedAimMode==0
                    ? "Camera Forward: native aim receives a forward stick while firing; camera stays free (foot mode EXPERIMENTAL)."
                    : "Vanilla Aim: camera free at idle, paused during firing while game native aiming takes over.");
                if(ImGui::TreeNode("TPS aim technical information")){
                    ImGui::TextDisabled("Forward native look samples %u | yaw difference >35 deg %u",
                        t.forwardLookSamples,t.forwardYawGapCount);
                    ImGui::TextDisabled("Native yaw %.1f / TPS yaw %.1f",
                        t.forwardNativeYaw,t.forwardCameraYaw);
                    ImGui::TextDisabled("Native idle look blocked %u | mounted fire camera frames %u",
                        t.mountedIdleLookBlocks,camera.mountedFireHorseSelected);
                    ImGui::TreePop();
                }
                ImGui::Separator();
                ImGui::Text("Keyboard + controller: same actions, side by side");
                ImGui::TextDisabled("OFF disables only that shortcut; existing native game buttons are unchanged.");
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
                auto BindGamepadMask=[&](const char* title,int& mask,int defaultMask) {
                    int index=0;
                    for(int i=0;i<kButtonCount;++i)
                        if(mask==kButtons[i]){index=i;break;}
                    ImGui::SetNextItemWidth(285.0f);
                    if(ImGui::Combo(title,&index,kButtonLabels,kButtonCount)){
                        mask=kButtons[index];config.Save();
                    }
                    ImGui::SameLine();
                    ImGui::PushID(title);
                    if(ImGui::SmallButton("Default")){mask=defaultMask;config.Save();}
                    ImGui::PopID();
                };
                auto BindTPSKeyboard=[&](const char* label,dg::config::TpsKeyboardAction action){
                    const size_t i=static_cast<size_t>(action);
                    const int bindingIndex=static_cast<int>(config.cameraKeys.size()+i);
                    ImGui::PushID(6100+bindingIndex);
                    ImGui::Text("%s",label);
                    ImGui::SameLine(305.0f);
                    ImGui::Text("%s",config::KeyDisplayName(config.tpsKeyboardKeys[i]).c_str());
                    ImGui::SameLine();
                    const bool capture=c.captureCameraKeyIndex->load()==bindingIndex;
                    if(ImGui::SmallButton(capture?"Press key...":"Rebind")){
                        c.captureMenuKey->store(false);
                        c.captureCameraKeyIndex->store(bindingIndex);
                    }
                    ImGui::SameLine();
                    if(ImGui::SmallButton("Unbind")){
                        config.tpsKeyboardKeys[i]=0;
                        c.captureCameraKeyIndex->store(-1);
                        config.Save();
                    }
                    ImGui::SameLine();
                    if(ImGui::SmallButton("Default")){
                        config.tpsKeyboardKeys[i]=config::kDefaultTpsKeyboardKeys[i];
                        c.captureCameraKeyIndex->store(-1);
                        config.Save();
                    }
                    ImGui::PopID();
                };
                BindTPSKeyboard("TPS ON/OFF (long hold)",dg::config::TpsKeyboardAction::Toggle);
                BindGamepadMask("Controller long hold##TPS",config.tpsToggleHoldButtonMask,64);
                ImGui::SetNextItemWidth(245.0f);
                if(ImGui::SliderInt("Long press delay##ToggleTPS",
                    &config.tpsToggleHoldMs,250,2000,"%d ms"))config.Save();
                ImGui::Spacing();
                BindTPSKeyboard("Recenter camera",dg::config::TpsKeyboardAction::Recenter);
                BindGamepadMask("Controller recenter##TPS",config.tpsRecenterButtonMask,64);
                ImGui::Spacing();
                BindTPSKeyboard("Temporary vanilla view",dg::config::TpsKeyboardAction::Suspend);
                BindGamepadMask("Controller temporary view##TPS",config.tpsSuspendButtonMask,128);
                if(config.tpsKeyboardKeys[static_cast<size_t>(dg::config::TpsKeyboardAction::Suspend)]=='E')
                    ImGui::TextDisabled("E deliberately does BOTH: native gear + temporary view after hold delay.");
                ImGui::SetNextItemWidth(245.0f);
                if(ImGui::SliderInt("Temporary view delay##HoldTPS",
                    &config.tpsSuspendDelayMs,0,3000,"%d ms"))config.Save();
                ImGui::Spacing();
                BindGamepadMask("Use selected equipment (R3)##TPS",config.tpsGrenadeButtonMask,128);
                ImGui::TextWrapped("R3 USES selected equipment; it does NOT open the equipment wheel. "
                    "The native hold and stow/release pulse are preserved.");
                ImGui::Separator();
                ImGui::TextDisabled("Native GAME bindings: manually match in-game controls");
                BindGamepadMask("Detect primary fire (default RT)##TPS",config.tpsFirePrimaryMask,131072);
                ImGui::TextWrapped("Match the in-game SHOOT button. This detects firing but does not rebind the game.");
                BindGamepadMask("Detect alternate fire (default RB)##TPS",config.tpsFireSecondaryMask,512);
                ImGui::TextWrapped("Optional second SHOOT input; OFF disables this detector.");
                BindGamepadMask("Detect wheel opening (default LB)##TPS",config.tpsWheelPadMask,256);
                ImGui::TextWrapped("This held button lets the game's native right stick select wheel items, "
                    "instead of being intercepted by TPS camera control.");
                BindGamepadMask("Charged shot (default Y)##TPS",config.tpsChargedFirePadMask,32768);
                ImGui::TextWrapped("TPS: wheel button (LB) + charged shot uses native CAMERA FORWARD. "
                    "The wheel button stays native. A custom charged button remaps to native Y "
                    "only during the chord.");
                if(config.tpsChargedFirePadMask!=0 && config.tpsChargedFirePadMask==config.tpsWheelPadMask)
                    ImGui::TextDisabled("Binding conflict: wheel and charged button must differ.");
                BindTPSKeyboard("Equipment wheel detector (G)",dg::config::TpsKeyboardAction::WheelG);
                BindTPSKeyboard("Ammunition / abilities wheel detector (Tab)",dg::config::TpsKeyboardAction::WheelTab);
                ImGui::TextWrapped("Wheel detector keys never emit fake game input. "
                    "Change them here too when rebinding the game's wheel controls.");
                ImGui::TextDisabled("Keyboard Use Gear follows the game's own native mapping.");
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
            if(config.thirdPersonEnabled)
                ImGui::TextDisabled("Vanilla camera settings are disabled during TPS; saved values stay intact.");
            ImGui::BeginDisabled(config.thirdPersonEnabled);
            if(ImGui::Button("Camera Values Default"))config.ResetVanillaCameraValues();
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
            ImGui::TextWrapped("Vanilla camera height offset, default zero. TPS height is independent.");
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
            ImGui::TextWrapped("Vanilla camera pitch offset. TPS uses its own view pitch.");
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
                "Default: Up/Down = height, PageUp/PageDown = zoom. "
                "Home/End = tilt; Left/Right = yaw. "
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
                ImGui::SameLine();
                if(ImGui::SmallButton("Default")){
                    config.cameraKeys[i]=config::kDefaultCameraKeys[i];
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
            ImGui::EndDisabled();
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

            ImGui::SameLine();
            if(ImGui::Button("Default##MenuKey")){config.menuKey=VK_INSERT;config.Save();}
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

                    ImGui::SetNextItemWidth(390.0f);

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
                    ImGui::SameLine();
                    ImGui::PushID(i+8600);
                    if(ImGui::SmallButton("Default")){
                        config.hotkeys[static_cast<size_t>(i)]=config::kDefaultHotkeys[static_cast<size_t>(i)];
                        config.Save();
                    }
                    ImGui::PopID();
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
