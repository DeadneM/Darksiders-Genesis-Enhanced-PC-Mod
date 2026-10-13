#include "OverlayUi.h"
#include "GaugeChargeFeature.h"
#include "ShaderAimFeature.h"
#include "UpdateFeature.h"

#include "HorseFeature.h"
#include "CameraTraceFeature.h"

#include "imgui.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

namespace dg::overlay {
namespace {

// Consistent setting rows: identical control placement for combat, gauges,
// movement and mounts. Notes stay visible, native status stays unobtrusive.
void DrawTunableFeature(config::Store& config, const char* label,
    const char* id, bool* enabled, float* value,
    float minValue, float maxValue, float defaultValue,
    const char* format, const char* note, bool hookReady,
    const char* featureStatus=nullptr) {
    ImGui::PushID(id);
    if(ImGui::Checkbox(label,enabled))config.Save();
    // Status gets its own line so long labels never overlap in narrow windows.
    ImGui::TextDisabled("%s",featureStatus?featureStatus:
        hookReady?"Ready":"Unavailable");
    if(note) {
        ImGui::Indent(22.0f);
        ImGui::TextWrapped("%s",note);
        ImGui::Unindent(22.0f);
    }
    if(*enabled){
        ImGui::Indent(22.0f);
        const bool compact = ImGui::GetContentRegionAvail().x < 560.0f;
        ImGui::SetNextItemWidth(compact ? std::max(80.0f, ImGui::GetContentRegionAvail().x - 90.0f) : 280.0f);
        bool changed=ImGui::SliderFloat("Multiplier##Slider",value,
            minValue,maxValue,format);
        if (!compact) ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        changed|=ImGui::InputFloat("##Manual",value,0.0f,0.0f,"%.2f");
        if(ImGui::IsItemHovered())ImGui::SetTooltip("Enter an exact multiplier.");
        ImGui::SameLine();
        if(ImGui::Button("Default")){*value=defaultValue;changed=true;}
        if(changed){
            if(!std::isfinite(*value))*value=defaultValue;
            *value=std::clamp(*value,minValue,maxValue);
            config.Save();
        }
        ImGui::Unindent(22.0f);
    }
    ImGui::Spacing();
    ImGui::PopID();
}

void DrawSectionTitle(const char* title) {
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextUnformatted(title);
    ImGui::Spacing();
}

// Action, current physical key, and controls occupy distinct table columns.
// No fixed SameLine pixel offsets: long key/action names remain readable.
bool BeginKeyboardTable(const char* id){
    if(!ImGui::BeginTable(id,5,
        ImGuiTableFlags_RowBg|ImGuiTableFlags_BordersInnerH|
        ImGuiTableFlags_SizingStretchProp))return false;
    ImGui::TableSetupColumn("Action",ImGuiTableColumnFlags_WidthStretch,3.3f);
    ImGui::TableSetupColumn("Current key",ImGuiTableColumnFlags_WidthStretch,1.4f);
    ImGui::TableSetupColumn("Change",ImGuiTableColumnFlags_WidthFixed,95.0f);
    ImGui::TableSetupColumn("Clear",ImGuiTableColumnFlags_WidthFixed,72.0f);
    ImGui::TableSetupColumn("Reset",ImGuiTableColumnFlags_WidthFixed,76.0f);
    ImGui::TableHeadersRow();
    return true;
}
void DrawKeyboardRow(Context& c,config::Store& config,const char* label,
    int& value,int original,int bindingIndex){
    ImGui::PushID(bindingIndex);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextWrapped("%s",label);
    ImGui::TableSetColumnIndex(1);
    const std::string name=config::KeyDisplayName(value);
    if(value)ImGui::Text("%s",name.c_str());
    else ImGui::TextDisabled("UNBOUND");
    ImGui::TableSetColumnIndex(2);
    const bool capturing=c.captureCameraKeyIndex->load()==bindingIndex;
    if(ImGui::Button(capturing?"Press key...":"Change",ImVec2(-1.0f,0.0f))){
        c.captureMenuKey->store(false);
        c.captureCameraKeyIndex->store(bindingIndex);
    }
    ImGui::TableSetColumnIndex(3);
    if(ImGui::Button("Clear",ImVec2(-1.0f,0.0f))){
        value=0;c.captureCameraKeyIndex->store(-1);config.Save();
    }
    ImGui::TableSetColumnIndex(4);
    if(ImGui::Button("Default",ImVec2(-1.0f,0.0f))){
        value=original;c.captureCameraKeyIndex->store(-1);config.Save();
    }
    ImGui::PopID();
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
        ImVec2(990.0f, 760.0f),
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

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(9.0f,8.0f));
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

            if(ImGui::CollapsingHeader("HUD / aiming effects",ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::Indent(12.0f);
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
            ImGui::TextDisabled("Aim-effect filter: %s | suppressed draws: %u",
                aimShaders.hooksReady ? "Ready" : "Unavailable",aimShaders.blockedDraws);
            ImGui::Unindent(12.0f);
            }
            if(ImGui::CollapsingHeader("Player movement",ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::Indent(12.0f);
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
            ImGui::TextDisabled("Shortens recovery before movement. Applied: %d",
                t.actionMoveForced);

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

            ImGui::Unindent(12.0f);
            }
            if(ImGui::CollapsingHeader("Combat and resource gauges",ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::Indent(12.0f);
            DrawSectionTitle("Combat damage");

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

            DrawSectionTitle("Resource recharge");
            const auto charge=dg::gauges::GetTelemetry();
            ImGui::TextDisabled("For working gain paths: x0 stops gains, x1 is vanilla. Settings save to INI.");
            DrawTunableFeature(config,"Hotstreak charge","HotstreakCharge",
                &config.hotstreakChargeEnabled,&config.hotstreakChargeMultiplier,
                0.0f,100.0f,2.0f,"%.2fx",
                "Multiplier for earned Hotstreak charge.",
                t.hotstreakHookReady);
            DrawTunableFeature(config,"Wrath (Courroux) charge","WrathCharge",
                &config.wrathChargeEnabled,&config.wrathChargeMultiplier,
                0.0f,100.0f,2.0f,"%.2fx",
                "Candidate value observed; recharge multiplier has no confirmed gameplay effect.",
                charge.wrathReady,charge.wrathReady?"Unverified gain path":"Hook unavailable");
            DrawTunableFeature(config,"Ultimate Form / Chaos charge","UltimateCharge",
                &config.ultimateChargeEnabled,&config.ultimateChargeMultiplier,
                0.0f,100.0f,2.0f,"%.2fx",
                "Recharge multiplier has no confirmed gameplay effect; native gain path unresolved.",
                charge.chaosReady,charge.chaosReady?"Unverified gain path":"Hook unavailable");
            DrawTunableFeature(config,"Synergy charge","SynergyCharge",
                &config.synergyChargeEnabled,&config.synergyChargeMultiplier,
                0.0f,100.0f,2.0f,"%.2fx",
                "Multiplies verified local Synergy gains through the native additive path.",
                charge.synergyReady,charge.synergyReady?"Additive gains verified":"Unavailable");
            ImGui::TextDisabled("Synergy: %u gains boosted / %u setter calls (not all calls are gains)",
                charge.synergyBoostCalls,charge.synergyObserved);
            ImGui::TextDisabled("Wrath: %u calls | Ultimate: %u calls (gain paths unverified)",
                charge.wrathObserved,charge.chaosObserved);
            ImGui::TextDisabled("V1.15 gain-value categories are recorded in the per-session log.");
            ImGui::TextDisabled("Observed in-game: pistol %d | melee-like %d | Hotstreak %d",
                t.pistolDamageBoostCalls,t.meleeDamageBoostCalls,t.hotstreakBoostCalls);

            ImGui::Unindent(12.0f);
            }
            if(ImGui::CollapsingHeader("Horse and riding")){
            ImGui::Indent(12.0f);
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
            if(BeginKeyboardTable("HorseKeyboardBindings")){
                const size_t index=static_cast<size_t>(config::TpsKeyboardAction::Horse);
                DrawKeyboardRow(c,config,"Summon / dismount horse",
                    config.tpsKeyboardKeys[index],
                    config::kDefaultTpsKeyboardKeys[index],
                    static_cast<int>(config.cameraKeys.size()+index));
                ImGui::EndTable();
            }
            ImGui::TextDisabled("Keyboard: press Change, then a key. Esc cancels.");
            ImGui::Spacing();
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

            ImGui::TextDisabled("Horse state: %s | detection events: %u",
                horseTelemetry.validated?"Ready":"Waiting for mount",
                horseTelemetry.uniqueCandidatesLogged);

            ImGui::Unindent(12.0f);
            }
            if(ImGui::CollapsingHeader("System and startup")){
            ImGui::Indent(12.0f);
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

            bool skipWarningsNotReady = false;
            ImGui::BeginDisabled(true);
            ImGui::Checkbox("Skip Warnings", &skipWarningsNotReady);
            ImGui::EndDisabled();
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled("Not working yet | native warning initialization protected");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("The old startup-loop bypass broke the reticle. A safe screen-display hook is not verified.");
            ImGui::TextDisabled("Save and controller warnings are not currently skipped.");

            ImGui::Spacing();
            ImGui::TextDisabled("Skip Logos / Intro take effect after a game restart.");
            ImGui::Unindent(12.0f);
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Camera")) {
            ImGui::Spacing();
            DrawSectionTitle("Camera controls");
            ImGui::TextWrapped("TPS and the original game camera have independent saved settings.");

            const dg::camera_trace::Telemetry camera = dg::camera_trace::GetTelemetry();
            ImGui::TextDisabled("Camera hooks: %s | frames observed: %u",
                camera.viewReady && camera.armReady ? "Ready" : "Unavailable",
                camera.viewCalls);

            if(ImGui::CollapsingHeader("Third Person camera",ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::Indent(12.0f);
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
            ImGui::Spacing();
            if (config.thirdPersonEnabled) {
                ImGui::Indent();
                DrawSectionTitle("TPS mouse and pointer");
                if (ImGui::Checkbox("Show Mouse Pointer in TPS", &config.showTpsMousePointer)) {
                    config.Save();
                    *c.lastAction=config.showTpsMousePointer
                        ? "TPS mouse pointer visible (TPS only)"
                        : "TPS mouse pointer hidden (TPS only)";
                    if(c.log)c.log("TPS V1.19 pointer preference %d; TPS active=1",
                        config.showTpsMousePointer?1:0);
                }
                ImGui::TextDisabled("Visible only when Third Person is ON. Default: ON.");
                ImGui::TextWrapped("Mouse rotates TPS camera automatically; firing uses Camera Forward. "
                    "No second mouse firing mode or activation checkbox.");
                ImGui::Spacing();
                DrawSectionTitle("TPS camera attachment");
                ImGui::TextDisabled("TPS follows the live local player permanently.");
                ImGui::TextDisabled("TPS pivot follows player feet on the character axis.");
                ImGui::TextDisabled("Foot Origin appears with other TPS camera values below.");

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

                ImGui::Separator();
                ImGui::Text("Keyboard and controller shortcuts");
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
                DrawSectionTitle("Keyboard shortcuts");
                ImGui::TextDisabled("Change then press a key; Esc cancels. Unbound disables the mod shortcut.");
                if(BeginKeyboardTable("TpsKeyboardBindings")){
                    for(size_t i=1;i<static_cast<size_t>(config::TpsKeyboardAction::Count);++i){
                        DrawKeyboardRow(c,config,config::kTpsKeyboardLabels[i],
                            config.tpsKeyboardKeys[i],config::kDefaultTpsKeyboardKeys[i],
                            static_cast<int>(config.cameraKeys.size()+i));
                    }
                    ImGui::EndTable();
                }
                DrawSectionTitle("Controller bindings");
                ImGui::TextDisabled("Native in-game controls take priority. These bindings are mod detectors.");
                BindGamepadMask("Controller long hold##TPS",config.tpsToggleHoldButtonMask,64);
                ImGui::SetNextItemWidth(245.0f);
                if(ImGui::SliderInt("Long press delay##ToggleTPS",
                    &config.tpsToggleHoldMs,250,2000,"%d ms"))config.Save();
                ImGui::Spacing();
                BindGamepadMask("Controller recenter##TPS",config.tpsRecenterButtonMask,64);
                ImGui::Spacing();
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
                ImGui::TextDisabled("LB+Y: original native ability behavior, no modded aiming or key translation.");
                ImGui::TextDisabled("Old TPSChargedFirePadMask INI is retained but ignored.");
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
            ImGui::Unindent(12.0f);
            }
            if(ImGui::CollapsingHeader("Original game camera")){
            ImGui::Indent(12.0f);
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
            if(BeginKeyboardTable("VanillaCameraKeyboardBindings")){
                for(size_t i=0;i<config.cameraKeys.size();++i){
                    DrawKeyboardRow(c,config,config::kCameraLabels[i],
                        config.cameraKeys[i],config::kDefaultCameraKeys[i],
                        static_cast<int>(i));
                }
                ImGui::EndTable();
            }
            ImGui::TextDisabled("Hold keys to repeat. Steps: height 50, zoom 10%%, pitch/yaw 5 degrees.");
            ImGui::TextDisabled("Camera-distance adjustments temporarily disable DOF blur.");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("The native r.DepthOfFieldQuality is restored when Zoom and Third Person are off.");
            ImGui::TextDisabled("Native output changes. Actual framing requires in-game verification.");
            ImGui::EndDisabled();
            ImGui::Unindent(12.0f);
            }
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

            ImGui::TextDisabled(
                "Save writes the current INI; Reload reads it without resetting defaults.");

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


        if (ImGui::BeginTabItem("Updates")) {
            ImGui::Spacing();
            DrawSectionTitle("In-game mod updates");
            ImGui::TextWrapped("Check GitHub and queue a verified update. "
                "Loaded DLLs are replaced only AFTER the game closes.");

            if (ImGui::Checkbox("Beta updates (debug)", &config.updaterBeta)) {
                config.Save();
                dg::update::Invalidate();
                *c.lastAction = config.updaterBeta ?
                    "Updates channel: debug (beta)" : "Updates channel: main (stable)";
            }
            ImGui::TextDisabled("OFF: main (stable)  |  ON: debug (experimental)");
            ImGui::TextDisabled("Current channel: %s", config.updaterBeta ? "debug" : "main");
            ImGui::Spacing();

            const auto update = dg::update::GetStatus();
            ImGui::Text("Running mod: V1.20 (ASI updater)");
            const bool checked = update.hasLatest && update.checkedBeta == config.updaterBeta;
            if (checked) {
                ImGui::Text("Latest %s build: %s",
                    config.updaterBeta ? "debug" : "main", update.latest.c_str());
            } else {
                ImGui::TextDisabled("Latest build: not checked");
            }
            ImGui::TextWrapped("Status: %s", update.message.c_str());
            if (update.busy) ImGui::TextDisabled("Background operation in progress...");
            ImGui::Spacing();

            ImGui::BeginDisabled(update.busy);
            if (ImGui::Button("Check for Updates", ImVec2(195.0f, 0.0f))) {
                dg::update::Check(config.updaterBeta);
            }
            ImGui::EndDisabled();
            ImGui::Spacing();

            const bool canInstall = !update.busy && checked && !update.sameBuild;
            ImGui::BeginDisabled(!canInstall);
            if (ImGui::Button("Install on Game Exit", ImVec2(195.0f, 0.0f))) {
                ImGui::OpenPopup("Confirm Mod Update");
            }
            ImGui::EndDisabled();
            ImGui::SameLine();
            ImGui::BeginDisabled(update.busy);
            if (ImGui::Button("Restore Previous", ImVec2(155.0f, 0.0f))) {
                ImGui::OpenPopup("Confirm Mod Rollback");
            }
            ImGui::EndDisabled();
            ImGui::Spacing();

            ImGui::TextWrapped("Your custom DarksidersGenesisMod.ini will be kept, "
                "and previous mod files backed up before replacement.");
            ImGui::TextDisabled("Switching channels never silently installs or downgrades.");

            if (ImGui::BeginPopupModal("Confirm Mod Update", nullptr,
                    ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextWrapped("Install %s from %s on next game exit?",
                    update.latest.c_str(), config.updaterBeta ? "debug / beta" : "main / stable");
                if (!update.newer)
                    ImGui::TextWrapped("WARNING: this version is not newer than V1.14. "
                        "You may be downgrading the mod.");
                ImGui::TextWrapped("The game files will change only AFTER Darksiders Genesis closes. "
                    "Your existing INI is preserved.");
                ImGui::Spacing();
                if (ImGui::Button("Confirm Update", ImVec2(160.0f, 0.0f))) {
                    dg::update::InstallAfterExit(config.updaterBeta);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel##ModUpdate", ImVec2(110.0f, 0.0f)))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            if (ImGui::BeginPopupModal("Confirm Mod Rollback", nullptr,
                    ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextWrapped("Restore the latest backed-up mod binaries after game exit?");
                ImGui::TextDisabled("The existing mod INI will not be replaced.");
                ImGui::Spacing();
                if (ImGui::Button("Confirm Restore", ImVec2(160.0f, 0.0f))) {
                    dg::update::RestoreAfterExit();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel##ModRollback", ImVec2(110.0f, 0.0f)))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
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
            DrawSectionTitle("Configuration");
            ImGui::TextWrapped(
                "Factory reset affects ALL mod settings and bindings. Other Default buttons only restore their own value.");

            // OpenPopup and BeginPopupModal MUST share the same ImGui ID stack.
            // Previously the popup was looked up after EndTabBar(): clicking
            // Reset INI in About did nothing because popup IDs did not match.
            static std::string factoryResetFeedback;
            static bool factoryResetSuccess = false;
            if (ImGui::Button("Reset INI to Factory Defaults")) {
                if (c.log) c.log("Overlay INI Factory Reset: button clicked; awaiting confirmation");
                ImGui::OpenPopup("Reset INI to Factory Defaults?");
            }
            ImGui::TextDisabled("Confirmation required. Previous INI is kept as a dated .bak.");
            ImGui::TextDisabled("Game saves and Engine.ini untouched. Restart after success.");

            if (ImGui::BeginPopupModal("Reset INI to Factory Defaults?", nullptr,
                    ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextWrapped("Reset ALL mod settings and keyboard/gamepad bindings?");
                ImGui::TextWrapped("Your current INI is preserved in a dated .bak beside the new INI.");
                ImGui::TextDisabled("Game saves and Engine.ini are NOT changed.");
                ImGui::Spacing();
                if (ImGui::Button("Confirm Reset", ImVec2(160.0f, 0.0f))) {
                    if (c.log) c.log("Overlay INI Factory Reset: confirmation received");
                    const bool ok = config.ResetIniFileToDefaults();
                    factoryResetSuccess = ok;
                    if (ok) {
                        c.captureCameraKeyIndex->store(-1);
                        c.captureMenuKey->store(false);
                        c.hudHidden->store(false);
                        factoryResetFeedback =
                            "SUCCESS: Factory defaults restored. Previous INI saved as dated .bak. Restart the game.";
                        *c.lastAction = factoryResetFeedback;
                    } else {
                        factoryResetFeedback =
                            "FAILED: INI was not reset. See DarksidersGenesisMod.log for details; original/backup preserved.";
                        *c.lastAction = factoryResetFeedback;
                    }
                    if (c.log) c.log("Overlay INI Factory Reset: result=%s",
                        ok ? "SUCCESS (dated backup and default INI)" : "FAILURE (inspect INI backup)");
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(110.0f, 0.0f))) {
                    factoryResetFeedback = "CANCELLED: The INI was not changed.";
                    factoryResetSuccess = false;
                    *c.lastAction = factoryResetFeedback;
                    if (c.log) c.log("Overlay INI Factory Reset: cancelled; no INI modification");
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            if (!factoryResetFeedback.empty()) {
                ImGui::Spacing();
                ImGui::TextColored(factoryResetSuccess
                        ? ImVec4(0.45f, 0.95f, 0.55f, 1.0f)
                        : ImVec4(1.0f, 0.72f, 0.35f, 1.0f),
                    "%s", factoryResetFeedback.c_str());
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Separator();
    ImGui::Text(
        "Last action: %s",
        c.lastAction->c_str()
    );

    ImGui::PopStyleVar();
    ImGui::End();

    if (!open) {
        c.overlayVisible->store(false);
    }
}

} // namespace dg::overlay
