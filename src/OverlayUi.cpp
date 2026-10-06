#include "OverlayUi.h"

#include "HorseFeature.h"

#include "imgui.h"

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
    const char* note
) {
    if (ImGui::Checkbox(label, enabled)) {
        config.Save();
    }

    ImGui::SameLine(310.0f);
    ImGui::TextDisabled("%s", note);

    if (!*enabled) {
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

            if (ImGui::Checkbox(
                    "Toggle HUD",
                    &config.toggleHudEnabled)) {
                config.Save();
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled(
                "%s",
                t.hudHookReady
                    ? "Native ui.HideHud getter hooked"
                    : "Native hook unavailable"
            );

            if (config.toggleHudEnabled) {
                bool hudHidden = c.hudHidden->load();
                ImGui::Indent();
                if (ImGui::Checkbox(
                        "HUD Hidden##RuntimeHUD",
                        &hudHidden)) {
                    c.hudHidden->store(hudHidden);
                    *c.lastAction =
                        std::string("HUD ") +
                        (hudHidden ? "hidden" : "visible");
                    if (c.log) {
                        c.log(
                            "Overlay -> HUD %s",
                            hudHidden ? "HIDDEN" : "VISIBLE"
                        );
                    }
                }
                ImGui::SameLine();
                ImGui::TextDisabled("F1 default");
                ImGui::Unindent();
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
                    : "Native hook unavailable"
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

                ImGui::TextDisabled(
                    "MOVE is never forced during STARTING/RUNNING. Only AWAITING_FINISH is shortened."
                );
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
                    : "Native movement hook unavailable"
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
                    : "Native movement hook unavailable"
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
                    : "Final outgoing-damage hook unavailable"
            );

            if (t.finalDamageHookReady) {
                ImGui::Indent();
                ImGui::TextDisabled(
                    "Pistol events: %d | Last final %.2f -> %.2f | BaseJuice %.2f",
                    t.pistolDamageBoostCalls,
                    t.lastNativePistolDamage,
                    t.lastBoostedPistolDamage,
                    t.lastPistolBaseJuice
                );
                ImGui::Unindent();
            }

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
                    : "Final outgoing-damage hook unavailable"
            );

            if (t.finalDamageHookReady) {
                ImGui::Indent();
                ImGui::TextDisabled(
                    "Melee-diag events: %d | Last final %.2f -> %.2f",
                    t.meleeDamageBoostCalls,
                    t.lastNativeBaseDamage,
                    t.lastBoostedBaseDamage
                );
                ImGui::TextDisabled(
                    "Last DamageRecord: ScaleType %u | Tags %d",
                    t.lastOutgoingScaleType,
                    t.lastOutgoingTagCount
                );
                ImGui::Unindent();
            }

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
                    : "Native AddJuice hook unavailable"
            );

            if (t.hotstreakHookReady) {
                ImGui::Indent();
                ImGui::TextDisabled(
                    "Boost calls: %d | Last gain %.2f -> %.2f",
                    t.hotstreakBoostCalls,
                    t.lastNativeJuiceGain,
                    t.lastBoostedJuiceGain
                );
                ImGui::Unindent();
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
                horseTelemetry.validated
                    ? "Validated on existing shared GetMaxSpeed hook"
                    : "Waiting for exact horse signature 1300/600 + stamina"
            );

            ImGui::BeginDisabled();
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
                "Pending dedicated RunSpeed primitive"
            );
            ImGui::EndDisabled();

            DrawTunableFeature(
                config,
                "Horse Sprint Duration",
                "HorseSprintDuration",
                &config.horseSprintDurationEnabled,
                &config.horseSprintDurationMultiplier,
                0.00f,
                10.00f,
                2.00f,
                "%.2fx",
                horseTelemetry.staminaReady
                    ? "Validated StaminaSprintPercentageRate"
                    : "Waiting for exact horse stamina signature"
            );

            ImGui::Indent();
            ImGui::TextDisabled(
                "Horse: %s | candidate snapshots %u | matches %u",
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
                "Sprint drain %.3f -> %.3f | owner %p | movement %p",
                horseTelemetry.nativeSprintDrain,
                horseTelemetry.appliedSprintDrain,
                horseTelemetry.horseOwner,
                horseTelemetry.horseMovement
            );
            ImGui::Unindent();

            DrawSectionTitle("Camera");
            ImGui::TextDisabled(
                "Not implemented in V0.17 core. Controls stay locked until a native camera hook is proven."
            );
            ImGui::BeginDisabled();

            DrawTunableFeature(
                config,
                "FOV",
                "FOV",
                &config.fovEnabled,
                &config.fovDegrees,
                60.0f,
                140.0f,
                90.0f,
                "%.0f deg",
                "Pending camera hook"
            );

            DrawTunableFeature(
                config,
                "Third Person",
                "ThirdPerson",
                &config.thirdPersonEnabled,
                &config.thirdPersonDistanceMultiplier,
                0.00f,
                3.00f,
                1.00f,
                "%.2fx",
                "Pending camera hook | distance"
            );

            ImGui::EndDisabled();

            DrawSectionTitle("System");

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
                "%s",
                t.skipIntroReady
                    ? "Native g.PlayIntroCinematicOnBoot control | restart applies boot state"
                    : "Native CVar control unavailable"
            );

            if (t.skipIntroReady && t.skipIntroData) {
                ImGui::Indent();
                ImGui::TextDisabled(
                    "Native CVar now: %ld | vanilla captured: %ld",
                    *t.skipIntroData,
                    t.skipIntroOriginalValue
                );
                ImGui::Unindent();
            }

            ImGui::Spacing();
            ImGui::TextDisabled(
                "Runtime changes publish immediately; INI persistence is debounced."
            );

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
                *c.lastAction =
                    "Configuration reloaded";
            }

            ImGui::SameLine();

            if (ImGui::Button(
                    "Reset Defaults",
                    ImVec2(140.0f, 0.0f))) {
                config.ResetDefaults(true);
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
                "Default: F1 HUD | F2 Movement | F3 Recovery | F4 Skip Intro | F5-F12 None"
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
