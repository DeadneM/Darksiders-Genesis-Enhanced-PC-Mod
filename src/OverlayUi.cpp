#include "OverlayUi.h"
#include "OverlayBindingScope.h"
#include "OverlayLayout.h"
#include "MenuFontScope.h"
#include "InteractionHudFeature.h"
#include "WeaponSheathingFeature.h"
#include "ManualAimFeature.h"
#include "RecoveryTailFeature.h"
#include "HeavyLandingFeature.h"
#include "MountedHolsterFeature.h"
#include "PistolVisualFeature.h"
#include "WrathSlotFeature.h"
#include "PlayerPresentationFeature.h"
#include "CosmeticUnlockFeature.h"
#include "LeviathanVisualFeature.h"
#include "EffectAppearanceFeature.h"
#include <cstring>
#include "UltimateDurationFeature.h"
#include "GaugeChargeFeature.h"
#include "ResourceGainFeature.h"
#include "ManualGaugeFeature.h"
#include "AmmoConsumptionFeature.h"
#include "HotstreakFeature.h"
#include "PotionFeature.h"
#include "PotionPolicy.h"
#include "ShaderAimFeature.h"
#include "UpdateFeature.h"
#include "UpdateNoticeUi.h"
#include "BuildInfo.h"
#include "HealthFeature.h"
#include "GlideFeature.h"
#include "GrappleRangeFeature.h"
#include "AirJumpFeature.h"
#include "MountedJumpFeature.h"
#include "DashCombatFeature.h"

#include "HorseFeature.h"
#include "CameraTraceFeature.h"
#include "TpsCameraFocus.h"

#include "imgui.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

namespace dg::overlay {
namespace {

using layout::NumberRow;
using layout::HelpText;
using layout::Choice;

bool DrawDelayRow(const char* label,const char* id,int* value,int minimum,int maximum,int original){
    float edited=static_cast<float>(*value);
    if(!NumberRow(label,id,&edited,float(minimum),float(maximum),float(original),"%.0f ms","%.0f"))return false;
    *value=static_cast<int>(std::lround(edited));return true;
}
void DrawRefillResult(const char* player,dg::refill::Result result){
    switch(result){
    case dg::refill::Result::Queued:ImGui::TextDisabled("%s: refill queued; return to gameplay.",player);break;
    case dg::refill::Result::Applied:ImGui::TextDisabled("%s: reserve filled.",player);break;
    case dg::refill::Result::Rejected:ImGui::TextDisabled("%s: refill unavailable here; retry during gameplay.",player);break;
    case dg::refill::Result::Cancelled:ImGui::TextDisabled("%s: refill cancelled or expired.",player);break;
    default:break;
    }
}
void DrawFeatureDescription(const char* note,bool ready,const char* status=nullptr){
    ImGui::Indent(layout::ValueIndent());
    if(status||!ready)ImGui::TextWrapped("%s",status?status:"Unavailable in this session.");
    HelpText(note);
    ImGui::Unindent(layout::ValueIndent());
}
void DrawToggleFeature(config::Store& config,const char* label,const char* id,bool* enabled,
    const char* note,bool ready=true,const char* status=nullptr){
    ImGui::PushID(id);
    if(ImGui::Checkbox(label,enabled))config.Save();
    DrawFeatureDescription(note,ready,status);
    ImGui::Spacing();ImGui::PopID();
}
void DrawTunableFeature(config::Store& config,const char* label,const char* id,
    bool* enabled,float* value,float minimum,float maximum,float original,
    const char* format,const char* note,bool ready,const char* status=nullptr,
    const char* valueLabel="Multiplier"){
    ImGui::PushID(id);
    if(ImGui::Checkbox(label,enabled))config.Save();
    DrawFeatureDescription(note,ready,status);
    if(*enabled){
        ImGui::Indent(layout::ValueIndent());
        if(NumberRow(valueLabel,"Value",value,minimum,maximum,original,format))config.Save();
        ImGui::Unindent(layout::ValueIndent());
    }
    ImGui::Spacing();ImGui::PopID();
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
    ImGui::TableSetupColumn("Change",ImGuiTableColumnFlags_WidthFixed,ImGui::CalcTextSize("Press key...").x+2*(ImGui::GetStyle().FramePadding.x+ImGui::GetStyle().CellPadding.x));
    ImGui::TableSetupColumn("Clear",ImGuiTableColumnFlags_WidthFixed,ImGui::CalcTextSize("Clear").x+2*(ImGui::GetStyle().FramePadding.x+ImGui::GetStyle().CellPadding.x));
    ImGui::TableSetupColumn("Reset",ImGuiTableColumnFlags_WidthFixed,ImGui::CalcTextSize("Default").x+2*(ImGui::GetStyle().FramePadding.x+ImGui::GetStyle().CellPadding.x));
    ImGui::TableHeadersRow();
    return true;
}
void DrawManualPadRow(config::Store& config,const char* label,int& value){
    constexpr int masks[]={0,0x0100,0x0200,0x0040,0x0080,0x1000,0x2000,0x4000,0x8000,
        0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x10000,0x20000};
    constexpr const char* labels[]={"OFF","LB / L1","RB / R1","L3","R3","A / Cross","B / Circle",
        "X / Square","Y / Triangle","D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right",
        "Start","Back","LT / L2","RT / R2"};
    int selected=0;for(int i=0;i<17;++i)if(masks[i]==value)selected=i;
    if(Choice(label,&selected,labels,17)){value=masks[selected];config.Save();}
}
void DrawKeyboardRow(Context& c,config::Store& config,const char* label,
    int& value,int original,int bindingIndex){
    const BindingRowScope rowScope(bindingIndex);
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
    const MenuFontScope menuFont(config.menuFontSize);
    const layout::StyleScope menuStyle;
    const Telemetry& t = c.telemetry;

    ImGui::SetNextWindowSize(
        ImVec2(990.0f, 760.0f),
        ImGuiCond_FirstUseEver
    );
    ImGui::SetNextWindowPos(
        ImVec2(80.0f, 80.0f),
        ImGuiCond_FirstUseEver
    );

    const auto display=ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowSizeConstraints(
        ImVec2(std::min(std::max(480.f,34*ImGui::GetFontSize()),std::max(1.f,display.x-32)),std::min(300.f,std::max(1.f,display.y-32))),
        ImVec2(std::max(1.f,display.x-16),std::max(1.f,display.y-16)));
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

    ImGui::PushTextWrapPos(0.f);
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

    const auto update = dg::update::GetStatus();
    if(update.hasLatest&&update.newer&&update.checkedBeta==config.updaterBeta){
        DrawUpdateBanner(update.latest.c_str(),config.updaterBeta?"debug":"main");
        ImGui::Separator();
    }
    ImGui::TextDisabled("Hover descriptions marked (?) for details.");
    if (ImGui::BeginTabBar("MainTabs")) {
        if (ImGui::BeginTabItem("Gameplay")) {
            ImGui::Spacing();
            HelpText("Multipliers: 0.5 = half, 1 = native, 2 = double. Cost divisors work in reverse; each option explains its rules.");

            if(ImGui::CollapsingHeader("HUD / aiming effects")){
            ImGui::Indent();

            DrawTunableFeature(config,"Interaction prompt size","InteractionHudScale",&config.interactionHudScaleEnabled,
                &config.interactionHudScale,0.1f,2.0f,0.65f,"%.2fx",
                "Scales only the interaction prompt above your character. 0.5 is half size; 1 is native. OFF restores its native size. Each local player's prompt follows its own camera; shared world sprites are unchanged.",interaction_hud::Ready());
            bool hudHidden=c.hudHidden->load();
            if (ImGui::Checkbox("HUD Hidden", &hudHidden)) {
                c.hudHidden->store(hudHidden);
                *c.lastAction=hudHidden?"HUD hidden":"HUD visible";
                if (c.log) c.log("Overlay -> HUD %s",hudHidden?"HIDDEN":"VISIBLE");
            }
            ImGui::TextDisabled("%s | binding in Hotkeys",t.hudHookReady?"Hook ready":"Unavailable");
            ImGui::TextDisabled("Hides normal on-screen HUD. The blue 3D aim line/arc are separate.");

            ImGui::Text("Blue aiming line and semicircle");
            constexpr const char* kAimModes[]={
                "Hide Everywhere", "Default Only", "TPS Only", "Never (always visible)"
            };
            if(Choice("Visibility mode##AimIndicator",
                    &config.aimIndicatorHideMode,kAimModes,4))config.Save();
            ImGui::TextDisabled("TPS Only is the default. Default Only means normal/non-TPS camera.");
            const auto aimShaders = dg::aim_shader::GetTelemetry();
            ImGui::TextDisabled("Aim-effect filter: %s | suppressed draws: %u",
                aimShaders.hooksReady ? "Ready" : "Unavailable",aimShaders.blockedDraws);
            ImGui::Unindent();
            }
            if(ImGui::CollapsingHeader("Aim without firing")){
                ImGui::Indent();ImGui::PushID("ManualAimBindings");
                ImGui::BeginDisabled(!manual_aim::Ready());
                DrawManualPadRow(config,"Controller input 1",config.aimPadButtonA);
                DrawManualPadRow(config,"Controller input 2",config.aimPadButtonB);
                ImGui::EndDisabled();
                HelpText("Hold to aim without firing. One input on OFF: hold the other button alone. Two active inputs: hold both together. Both OFF: disabled. Each controller affects its own player. In TPS, aim follows that player's camera.");
                HelpText("Native game bindings are kept; choose an unused button or combination. Release after closing a menu, changing character or reconnecting the controller before aiming again.");
                if(!manual_aim::Ready())ImGui::TextWrapped("Unavailable for this executable; native controls remain active.");
                ImGui::PopID();ImGui::Unindent();
            }
            if(ImGui::CollapsingHeader("Weapon sheathing")){
            ImGui::Indent();
            DrawToggleFeature(config,"Mounted pistol holsters (experimental)","MountedPistolHolsters",&config.mountedHolstersEnabled,
                "Uses the stowed pistol models while Strife is mounted. Turn OFF to restore the previous appearance. Per-hand appearance choices use the corresponding stowed models. Experimental: check alignment in the holsters.",mounted_holsters::Ready());
            DrawToggleFeature(config,"Control automatic weapon sheathing","WeaponSheathing",&config.weaponSheathingEnabled,
                "Separate automatic delays for Strife's melee blades, Strife's pistols and War's sword. OFF restores native delays. Foot forms only; horse and Ultimate remain native.",sheathing::Ready());
            if(config.weaponSheathingEnabled){
                ImGui::Indent(layout::ValueIndent());
                const char* labels[]={"Strife melee blades","Strife pistols","War sword"};
                bool* never[]={&config.sheathStrifeMeleeNeverEnabled,&config.sheathNeverEnabled,&config.sheathWarNeverEnabled};
                float* seconds[]={&config.sheathStrifeMeleeDelaySeconds,&config.sheathDelaySeconds,&config.sheathWarDelaySeconds};
                for(int i=0;i<3;++i){
                    ImGui::PushID(labels[i]);ImGui::TextUnformatted(labels[i]);
                    if(ImGui::Checkbox("Never automatically sheathe",never[i]))config.Save();
                    if(!*never[i]&&NumberRow("Automatic sheathing delay (seconds)","SheathDelay",seconds[i],0,300,i==0?0.0f:10.0f,"%.2f s"))config.Save();
                    ImGui::PopID();
                }
                ImGui::Unindent(layout::ValueIndent());
            }
            ImGui::PushID("SheatheBindings");
            ImGui::TextUnformatted("Sheathe weapons shortcuts");
            if(BeginKeyboardTable("Keyboard")){
                DrawKeyboardRow(c,config,"Sheathe weapons",config.sheathKeyboardKey,0,config::kSheathKeyboardBindingIndex);
                ImGui::EndTable();
            }
            DrawManualPadRow(config,"Controller input 1",config.sheathPadButtonA);
            DrawManualPadRow(config,"Controller input 2",config.sheathPadButtonB);
            HelpText("One controller input on OFF: use the other button alone. Two active inputs: press both together. Both OFF: disabled. Each controller affects its own player. Native game bindings are kept; choose an unused button or combination.");
            HelpText("The keyboard shortcut and F1-F12 Hotkeys remain available together for eligible local players, independently of automatic control. One press queues one request per player, even with the same key assigned twice. Release before pressing again. Menu/focus changes do not trigger held shortcuts. Active attacks/aim, cinematics and transformations stay native.");
            ImGui::PopID();

            ImGui::Unindent();
            }
            if(ImGui::CollapsingHeader("Wrath ability slots (experimental)")){
                ImGui::Indent();
                HelpText("Choose any of the six powers for each slot. Native restores the original power. Transferred powers keep their costs; character-specific animations and visuals are adapted. Local co-op still needs a separate test.");
                int status[7]={2,wrath_slots::ForeignStatus(1),wrath_slots::ForeignStatus(2),wrath_slots::ForeignStatus(3),wrath_slots::ForeignStatus(4),wrath_slots::ForeignStatus(5),wrath_slots::ForeignStatus(6)};
                constexpr const char* names[7]={"Native","Shadow Clone","World Ender","Caltrops","Blade Geyser","Rampage","Stoneskin"};
                constexpr const char* familyNames[2]={"Strife","War"};
                ImGui::BeginDisabled(!wrath_slots::Ready());
                for(unsigned family=0;family<2;++family){
                    ImGui::PushID(int(family));ImGui::TextUnformatted(familyNames[family]);
                    for(unsigned slot=0;slot<3;++slot){
                        ImGui::PushID(int(slot));
                        const int choice=wrath_slots::Normalize(family,config.wrathSlots[family][slot]);
                        char label[32]{};sprintf_s(label,"Ability slot %u",slot+1);
                        if(ImGui::BeginCombo(label,names[choice])){
                            for(int candidate=0;candidate<7;++candidate){
                                if(!wrath_slots::Supported(family,candidate))continue;
                                ImGui::BeginDisabled(wrath_slots::Foreign(family,candidate)&&status[candidate]==0);
                                if(ImGui::Selectable(names[candidate],choice==candidate)){
                                    config.wrathSlots[family][slot]=candidate;config.Save();
                                }
                                if(choice==candidate)ImGui::SetItemDefaultFocus();
                                ImGui::EndDisabled();
                            }
                            ImGui::EndCombo();
                        }
                        ImGui::PopID();
                    }
                    ImGui::PopID();
                }
                ImGui::EndDisabled();
                if(!wrath_slots::Ready())ImGui::TextDisabled("Native ability boundary unavailable; original slots retained.");
                HelpText("The original slot must be unlocked. Transferred powers keep their native Wrath cost and do not unlock War's progression. Wheel icons and hints may still describe the original slot. Horse and ultimate abilities stay native. Select Native to restore a slot.");
                for(int power:{1,3,4}){
                    if(status[power]==1)ImGui::TextDisabled("%s: preparing loaded assets.",names[power]);
                    if(status[power]==0)ImGui::TextDisabled("%s transfer unavailable; select Native after an unsuccessful cast.",names[power]);
                }
                ImGui::Unindent();
            }
            if(ImGui::CollapsingHeader("Player movement")){
            ImGui::Indent();
            DrawTunableFeature(config,"Heavy landing movement recovery","HeavyLandingRecovery",&config.heavyLandingRecoveryEnabled,
                &config.heavyLandingDelaySeconds,.05f,.8f,.15f,"%.3f s",
                "Delay from the start of a heavy landing before movement can interrupt it. Uses the timing accepted in V1.126. Staying still keeps the animation; the game handles cancellation. Native and TPS views. Does not change dashes or attacks.",heavy_landing::Ready(),nullptr,"Recovery delay");
            if(config.heavyLandingRecoveryEnabled&&ImGui::TreeNode("Advanced landing settings")){
                if(NumberRow("Ground stability time","HeavyLandingGroundSeconds",&config.heavyLandingGroundSeconds,0.f,.25f,.05f,"%.3f s","%.3f"))config.Save();
                HelpText("Minimum continuous contact with the ground. 0.05 s filters brief contacts before recovery can open.");
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Recovery experiments")){
                DrawToggleFeature(config,"Ground dash after-animation recovery (experimental)","RecoveryDashTail",&config.recoveryDashTailEnabled,
                    "Only after an observed native montage stops completely and forced dash motion has ended. Opens native MOVE interruption; hold movement to test. Does not shorten an active dash, change ammo or simulate a button. May do nothing if native cleanup has already finished.",recovery_tail::Ready());
                DrawToggleFeature(config,"Chest after-animation recovery (experimental)","RecoveryChestTail",&config.recoveryChestTailEnabled,
                    "Only exact chest interactions, after observing their native montage play then stop. Loot and animation remain native. Opens MOVE interruption if the action is still running. Check loot, movement and a second interaction; OFF restores a window owned by this experiment.",recovery_tail::Ready());
                if(NumberRow("Wait after animation stops","RecoveryTailDelaySeconds",&config.recoveryTailDelaySeconds,0.f,1.f,0.f,"%.3f s","%.3f"))config.Save();
                HelpText("Dash and chest experiments default OFF. The last delay is shared by dash and chest; 0 adds no wait beyond a 30 ms stopped-animation confirmation. Unknown actions, menus, transformations and cinematics keep native behavior. Test Native and TPS separately.");
                ImGui::TreePop();
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
                "Changes walking and running speed for the player.",
                t.movementHookReady
            );

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
            DrawToggleFeature(config,"Unlimited glide","UnlimitedGlide",&config.unlimitedGlideEnabled,
                "Holds the duration budget while gliding. Release and landing stay native. Assign a toggle in Hotkeys.",dg::glide::Ready());

            constexpr const char* jumpModes[]{"Off","After gliding","Always on"};
            if(Choice("Infinite double jump",&config.infiniteDoubleJumpMode,jumpModes,3))config.Save();
            DrawFeatureDescription("After gliding rearms one double jump per glide. Always on allows repeated air jumps. In either mode, pressing your jump control ends a glide before requesting the jump. OFF restores normal behavior. Experimental.",dg::air_jump::Ready());
            DrawTunableFeature(config,"Grapple range","GrappleRange",&config.grappleRangeEnabled,
                &config.grappleRangeMultiplier,1.0f,10.0f,2.0f,"%.2fx",
                "Scales native acquisition, relay and pull distances together. Applies on the next use; target rules and obstacles remain native. Experimental: traversal limits still require in-game checks.",dg::grapple::Ready());
            ImGui::Unindent();
            }
            if(ImGui::CollapsingHeader("Combat and resource gauges")){
            ImGui::Indent();
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
            ImGui::TextWrapped("In local co-op, these settings and refill commands apply to both available players.");
            const auto health=dg::health::GetTelemetry(0),health2=dg::health::GetTelemetry(1);
            ImGui::BeginDisabled((!health.available&&!health2.available)||health.pending||health2.pending);
            if(ImGui::Button("Fill health"))dg::health::RequestRefill();
            ImGui::EndDisabled();
            if(health.available)ImGui::TextDisabled("P1 health: %.0f / %.0f",health.current,health.maximum);
            if(health2.available)ImGui::TextDisabled("P2 health: %.0f / %.0f",health2.current,health2.maximum);
            DrawToggleFeature(config,"Unlimited health","UnlimitedHealth",&config.unlimitedHealthEnabled,
                "Keeps living players at their native maximum health. Fill and Unlimited are assignable in Hotkeys. Does not revive a defeated character.",health.supported);
            ImGui::Separator();
            const auto charge=dg::gauges::GetTelemetry();
            const auto rewards=dg::resource_gain::GetTelemetry();
            const auto manual=dg::manual_gauges::GetTelemetry(0),manual2=dg::manual_gauges::GetTelemetry(1);
            const bool gaugePending=manual.pending||manual2.pending;
            ImGui::TextUnformatted("Set gauges for testing");
            static float gaugePercent=100.0f;
            NumberRow("Target level","ManualGauges",&gaugePercent,0.0f,100.0f,100.0f,"%.0f %%");
            const unsigned masks[3]={dg::manual_gauges::Wrath,
                dg::manual_gauges::Ultimate,dg::manual_gauges::Synergy};
            const char* names[3]={"Set Wrath","Set Ultimate","Set Synergy"};
            for(unsigned i=0;i<3;++i){
                ImGui::BeginDisabled(gaugePending||!((manual.readyMask|manual2.readyMask)&masks[i]));
                if(ImGui::Button(names[i]))dg::manual_gauges::RequestSet(masks[i],gaugePercent);
                ImGui::EndDisabled();
                if(manual.values[i].available){
                    ImGui::SameLine();
                    ImGui::TextDisabled("P1: %.1f / %.1f",manual.values[i].current,manual.values[i].maximum);
                }
                if(manual2.values[i].available){
                    ImGui::SameLine();
                    ImGui::TextDisabled("P2: %.1f / %.1f",manual2.values[i].current,manual2.values[i].maximum);
                }
            }
            ImGui::BeginDisabled(gaugePending||(manual.readyMask!=dg::manual_gauges::All&&manual2.readyMask!=dg::manual_gauges::All));
            if(ImGui::Button("Set all three gauges"))
                dg::manual_gauges::RequestSet(dg::manual_gauges::All,gaugePercent);
            ImGui::EndDisabled();
            if(gaugePending){
                ImGui::TextWrapped("Waiting for gameplay to apply the request.");
                if(ImGui::Button("Cancel gauge request"))dg::manual_gauges::CancelRequest();
            }else if(manual.result==dg::manual_gauges::Result::Applied){
                ImGui::TextWrapped("Gauge values applied. Check the gauges and Ultimate activation in game.");
            }else if(manual.result==dg::manual_gauges::Result::Partial||
                     manual.result==dg::manual_gauges::Result::Rejected){
                ImGui::TextWrapped("Some gauges could not be set here. See the log and retry in normal gameplay.");
            }else if(manual.result==dg::manual_gauges::Result::Expired||
                     manual.result==dg::manual_gauges::Result::Cancelled){
                ImGui::TextWrapped("Request cancelled or expired. Return to gameplay and try again.");
            }
            if(!manual.readyMask&&!manual2.readyMask)
                ImGui::TextWrapped("Available during normal local-host gameplay with a supported player.");
            ImGui::TextWrapped("One click changes the selected level once; it does not keep gauges full or unlock the Ultimate form.");
            ImGui::Separator();
            ImGui::TextUnformatted("Health potions (test)");
            const auto potions=dg::potions::GetTelemetry(0),potions2=dg::potions::GetTelemetry(1);
            ImGui::BeginDisabled((!potions.available&&!potions2.available)||potions.pending||potions2.pending);
            if(ImGui::Button("Fill health potions"))dg::potions::RequestRefill();
            ImGui::EndDisabled();
            if(potions.available)ImGui::Text("P1 reserve: %d / %d",potions.current,potions.maximum);
            if(potions2.available)ImGui::Text("P2 reserve: %d / %d",potions2.current,potions2.maximum);
            if(potions.available&&potions.current<potions.maximum&&std::isfinite(potions.overflow)&&
               std::isfinite(potions.nativeThreshold)&&potions.nativeThreshold>0){
                const float threshold=dg::potions::RechargeThreshold(potions.nativeThreshold,config.potionRechargeEnabled,config.potionRechargeMultiplier);
                ImGui::TextDisabled("Charge for next potion: %.0f%%",std::clamp(potions.overflow/threshold,0.0f,1.0f)*100.0f);
            }
            if(potions.pending)ImGui::TextDisabled("Refill queued; return to gameplay.");
            if(potions.result==dg::potions::Result::Applied)ImGui::TextDisabled("Potion reserve filled.");
            if(potions.result==dg::potions::Result::Rejected||potions.result==dg::potions::Result::Cancelled)
                ImGui::TextDisabled("Refill unavailable or cancelled; retry during gameplay.");
            DrawTunableFeature(config,"Potion recharge rate","PotionRecharge",
                &config.potionRechargeEnabled,&config.potionRechargeMultiplier,
                0.0f,100.0f,2.0f,"%.2fx",
                "From excess healing at full health. 0 or 1: normal. 0.5: half recharge rate. 2: double rate. Keeps the native reserve limit and healing amount.",potions.supported);
            DrawToggleFeature(config,"Unlimited potions","UnlimitedPotions",&config.unlimitedPotionsEnabled,
                "Keeps the health-potion reserve full. Healing, cooldown and use conditions stay native. Assign the refill, recharge and Unlimited actions in Hotkeys.",potions.supported);
            ImGui::Separator();
            ImGui::TextUnformatted("Ammunition");
            const bool ammoReady=dg::ammo::GetTelemetry().ready;
            const auto ammo1=dg::ammo::GetReserve(0),ammo2=dg::ammo::GetReserve(1);
            ImGui::BeginDisabled((!ammo1.available&&!ammo2.available)||ammo1.pending||ammo2.pending);
            if(ImGui::Button("Fill ammunition"))dg::ammo::RequestRefill();
            ImGui::EndDisabled();
            DrawRefillResult("P1 ammunition",ammo1.result);DrawRefillResult("P2 ammunition",ammo2.result);
            DrawTunableFeature(config,"Reduce ammo consumption","ReduceAmmo",&config.reduceAmmoEnabled,
                &config.ammoConsumptionDivisor,0.0f,10.0f,2.0f,"%.2f",
                "Consumption divisor: 0 or 1 is normal. 0.5 spends twice as much; 2 spends half; 10 spends one tenth. Unlimited takes priority. Assign a toggle in Hotkeys.",
                ammoReady,nullptr,"Consumption divisor");
            DrawToggleFeature(config,"Limited Standard ammunition","FiniteStandardAmmo",&config.finiteStandardAmmoEnabled,
                "Makes native Standard reserves finite. Starts full when enabled; changing capacity clamps the reserve without refilling. OFF restores native infinity. Fill ammunition, reduced consumption and Unlimited also apply. Native Hotstreak rules stay unchanged.",dg::ammo::StandardReady());
            if(config.finiteStandardAmmoEnabled){
                ImGui::Indent(layout::ValueIndent());
                float capacity=float(config.standardAmmoCapacity);
                if(NumberRow("Standard reserve capacity","StandardAmmoCapacity",&capacity,1,10000,200,"%.0f rounds","%.0f")){
                    config.standardAmmoCapacity=int(std::lround(capacity));config.Save();
                }
                ImGui::Unindent(layout::ValueIndent());
            }
            DrawToggleFeature(config,"Unlimited ammo","UnlimitedAmmo",&config.unlimitedAmmoEnabled,
                "Keeps finite ammunition full, including Standard when limited. Takes priority over reduced consumption. Assign a toggle in Hotkeys.",ammoReady);
            ImGui::Separator();
            ImGui::TextUnformatted("Unlimited gauges (test)");
            const bool gaugesReady=manual.supported||manual2.supported;
            DrawToggleFeature(config,"Unlimited Wrath (Courroux)","UnlimitedWrath",&config.unlimitedWrathEnabled,
                "Keeps Wrath full. Assign this gauge or all three together in Hotkeys.",gaugesReady);
            DrawToggleFeature(config,"Unlimited Ultimate form","UnlimitedUltimate",&config.unlimitedUltimateEnabled,
                "Keeps the Ultimate gauge full, including while transformed. Assign this gauge or all three together in Hotkeys.",gaugesReady);
            DrawToggleFeature(config,"Unlimited Synergy","UnlimitedSynergy",&config.unlimitedSynergyEnabled,
                "Keeps Synergy full. Assign this gauge or all three together in Hotkeys.",gaugesReady);
            ImGui::TextDisabled("Unlimited takes priority over the target level above.");
            ImGui::Separator();

            ImGui::TextDisabled("For working gain paths: x0 stops gains, x1 is vanilla. Settings save to INI.");
            DrawTunableFeature(config,"Hotstreak charge","HotstreakCharge",
                &config.hotstreakChargeEnabled,&config.hotstreakChargeMultiplier,
                0.0f,100.0f,2.0f,"%.2fx",
                "Multiplier for earned Hotstreak charge.",
                t.hotstreakHookReady);
            const auto hotstreak1=dg::hotstreak::GetTelemetry(0),hotstreak2=dg::hotstreak::GetTelemetry(1);
            const bool hotstreakReady=hotstreak1.supported;
            DrawTunableFeature(config,"Reduce Hotstreak decrease","ReduceHotstreak",&config.reduceHotstreakEnabled,
                &config.hotstreakDecreaseDivisor,0.0f,10.0f,2.0f,"%.2f",
                "Decrease divisor: 0 or 1 is normal. 0.5 doubles decay/costs, giving a shorter Hotstreak; 2 halves them; 10 gives one tenth. Resets stay native. Assign a toggle in Hotkeys.",
                hotstreakReady,nullptr,"Decrease divisor");
            DrawToggleFeature(config,"Unlimited Hotstreak","UnlimitedHotstreak",&config.unlimitedHotstreakEnabled,
                "Keeps Strife's Hotstreak full. Native activation, weapon behavior and resets are preserved. Assign a toggle in Hotkeys.",hotstreakReady);
            ImGui::BeginDisabled((!hotstreak1.available&&!hotstreak2.available)||hotstreak1.pending||hotstreak2.pending);
            if(ImGui::Button("Fill Hotstreak"))dg::hotstreak::RequestRefill();
            ImGui::EndDisabled();
            if(hotstreak1.available)ImGui::Text("P1 Hotstreak: %.1f / %.1f",hotstreak1.current,hotstreak1.maximum);
            if(hotstreak2.available)ImGui::Text("P2 Hotstreak: %.1f / %.1f",hotstreak2.current,hotstreak2.maximum);
            DrawRefillResult("P1 Hotstreak",hotstreak1.result);DrawRefillResult("P2 Hotstreak",hotstreak2.result);
            ImGui::TextDisabled("Refill Hotstreak and ammunition can also be assigned in Hotkeys.");
            DrawTunableFeature(config,"Wrath (Courroux) charge","WrathCharge",
                &config.wrathChargeEnabled,&config.wrathChargeMultiplier,
                0.0f,100.0f,defaults::WrathGain,"%.2fx",
                "Multiplier for native Wrath pickup gains; validated in game.",
                rewards.ready[0],rewards.ready[0]?"Gains validated by user":"Gain path unavailable");
            DrawTunableFeature(config,"Ultimate decrease rate","UltimateDecreaseRate",
                &config.ultimateDecreaseEnabled,&config.ultimateDurationMultiplier,
                0.0f,10.0f,2.0f,"%.2fx duration",
                "Duration multiplier: 0.5 halves the duration, 1 is native, 2 doubles it, 10 lasts ten times longer. 0 ends on the next positive native drain tick. Gains and refills stay unchanged; Unlimited Ultimate takes priority.",
                dg::ultimate_duration::Ready());
            DrawToggleFeature(config,"Infinite Ultimate duration","InfiniteUltimateDuration",&config.ultimateInfiniteDurationEnabled,
                "Stops only the active form's duration drain, at its current gauge level. Does not fill the gauge or bypass native activation requirements. Takes priority over the duration multiplier; OFF resumes native drain or the enabled multiplier. Assign a toggle in Hotkeys.",dg::ultimate_duration::Ready());
            DrawTunableFeature(config,"Ultimate Form / Chaos charge","UltimateCharge",
                &config.ultimateChargeEnabled,&config.ultimateChargeMultiplier,
                0.0f,100.0f,defaults::UltimateGain,"%.2fx",
                "Multiplier for native Ultimate combat rewards; validated in game.",
                rewards.ready[1],rewards.ready[1]?"Gains validated by user":"Gain path unavailable");
            ImGui::TextDisabled("Native gain events: Wrath %u (%u modified) | Ultimate %u (%u modified)",
                rewards.positive[0],rewards.boosted[0],rewards.positive[1],rewards.boosted[1]);
            DrawTunableFeature(config,"Synergy charge","SynergyCharge",
                &config.synergyChargeEnabled,&config.synergyChargeMultiplier,
                0.0f,100.0f,defaults::SynergyGain,"%.2fx",
                "Multiplies verified local Synergy gains through the native additive path.",
                charge.synergyReady,charge.synergyReady?"Additive gains verified":"Unavailable");
            ImGui::TextDisabled("Synergy: %u gains boosted / %u setter calls (not all calls are gains)",
                charge.synergyBoostCalls,charge.synergyObserved);
            ImGui::TextDisabled("Natural gains and Unlimited are independent settings.");
            ImGui::TextDisabled("V1.15 gain-value categories are recorded in the per-session log.");
            ImGui::TextDisabled("Observed in-game: pistol %d | melee-like %d | Hotstreak %d",
                t.pistolDamageBoostCalls,t.meleeDamageBoostCalls,t.hotstreakBoostCalls);

            ImGui::Unindent();
            }
            if(ImGui::CollapsingHeader("Horse and riding")){
            ImGui::Indent();

            const horse::Telemetry horseTelemetry =
                horse::GetTelemetry();
            constexpr const char* horseAreas[]={
                "Off", "In all area", "Restricted only", "Banned zone only"
            };
            if(Choice("Horse Everywhere##HorseAreaMode",
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
                if(Choice(label,&index,horsePadNames,16)){
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

            DrawToggleFeature(config,"Jump on horseback","MountedJump",&config.mountedJumpEnabled,
                "Use the game's rebindable Jump action while riding either mount. A new press requests a native ground leap, with native animations and landing. No air jump. OFF restores the original behavior. Confirmed in the user's solo test; co-op remains to be checked.",mounted_jump::Ready());

            constexpr const char* horseJumpSounds[]={"Native jump (varied)","Horse snort (varied)","Off"};
            if(Choice("Horse jump sound",&config.mountedJumpSound,horseJumpSounds,3))config.Save();
            HelpText("A sound on a successful mod jump and its landing. Uses the game's existing random samples and Sfx volume. Experimental; loaded assets only.");
            if(config.mountedJumpEnabled&&config.mountedJumpSound!=2){
                const unsigned required=config.mountedJumpSound==1?6u:3u;
                if((mounted_jump::SoundAssetsReady()&required)!=required)
                    ImGui::TextDisabled("Waiting for native jump sound assets; the jump remains available.");
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

            ImGui::TextDisabled("Horse state: %s | detection events: %u",
                horseTelemetry.validated?"Ready":"Waiting for mount",
                horseTelemetry.uniqueCandidatesLogged);
            DrawToggleFeature(config,"Unlimited horse sprint duration","UnlimitedHorseSprint",&config.unlimitedHorseSprintEnabled,
                "Stops sprint stamina drain for both local riders. Native recovery and sprint-start conditions stay active. Assign a toggle in Hotkeys.",
                horseTelemetry.staminaReady,horseTelemetry.staminaReady?"Ready":"Waiting for horse stamina");

            ImGui::Unindent();
            }
            if(ImGui::CollapsingHeader("System and startup")){
            ImGui::Indent();

            ImGui::Text("Graphics Adapter");
            ImGui::TextDisabled(
                "Select which GPU the game uses. Requires a restart to take effect."
            );

            ImGui::Indent();
            int selectedGraphicsAdapter =
                config.graphicsAdapter;
            const char* graphicsAdapters[] = {
                "0", "1", "2", "3", "4"
            };

            if (Choice(
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
            ImGui::TextDisabled(
                "%s | restart required",
                t.skipIntroReady ? "Boot setting ready" : "Unavailable"
            );

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Native g.PlayIntroCinematicOnBoot; independent of Skip Logos");

            if(ImGui::Checkbox("Skip Warnings (test)",&config.skipWarningEnabled)){
                config.Save();
                const bool ready=c.applySkipWarning&&c.applySkipWarning(config.skipWarningEnabled);
                *c.lastAction=std::string("Skip Warnings ")+(config.skipWarningEnabled?"ON":"OFF")+
                    (ready?" | next startup":" | unavailable");
            }
            ImGui::TextDisabled("%s | restart required",t.skipWarningReady?"Experimental":"Unavailable");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Advance the save and controller notices automatically. Please test startup, reticle and menus.");
            ImGui::TextDisabled("Save / controller pages advanced: %s | needs gameplay testing",
                t.skipWarningApplied?"observed":"not observed");

            ImGui::Spacing();
            ImGui::TextDisabled("Startup options take effect after a game restart.");
            ImGui::Unindent();
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Appearance")) {
            ImGui::TextWrapped("Choose ability, weapon, mount and core effects below. Model and armor controls follow the effect library.");
            ImGui::PushID("AppearanceTab");
            if(ImGui::CollapsingHeader("Effect appearances (experimental)")){
                ImGui::Indent();
                HelpText("Visual effects only. Native keeps the game and equipped cores. Alternatives change new emitted effects; Hidden keeps their native lifetime and hides their rendering. Models, materials, damage, cost and controls stay native. Some single-look families offer Native/Hidden only.");
                constexpr const char* categories[]={"Tools","Powers","Strife ammunition","War weapons","Mounts and forms","Creature cores","Other effects"};
                for(const auto* category:categories)if(ImGui::CollapsingHeader(category)){
                    ImGui::Indent();ImGui::BeginDisabled(!effect_appearance::Ready());
                    for(unsigned family=0;family<effect_appearance::FamilyCount;++family){const auto& info=effect_appearance::Families[family];
                        if(std::strcmp(category,info.category))continue;ImGui::PushID(int(family));if(Choice(info.label,&config.effectAppearances[family],info.choices.data(),int(info.count)))config.Save();
                        const int choice=effect_appearance::Normalize(family,config.effectAppearances[family]);
                        if(choice&&!(effect_appearance::LoadedMask(family)&(1u<<choice)))ImGui::TextColored(ImVec4(1,.85f,.25f,1),"Effect assets not ready: native appearance retained.");
                        ImGui::PopID();
                    }
                    ImGui::EndDisabled();ImGui::Unindent();
                }
                HelpText("Loaded assets only: no synchronous loading. Existing emitters may keep their previous appearance until the next use. Stoneskin here changes particles, not the body coating. Leviathan keeps its dedicated option below. Checkpoint/armor/model choices stay in their existing controls.");
                ImGui::Unindent();
            }

            if(ImGui::CollapsingHeader("Character models")){
            ImGui::Indent();
            constexpr const char* pistolModels[]={"Native (original for this hand)","Right pistol model","Left pistol model","OFF (experimental)"};
            const auto pistolChoice=[&](const char* label,int& value){
                ImGui::PushID(label);layout::Label(label);ImGui::SetNextItemWidth(layout::ControlWidth());
                if(ImGui::BeginCombo("##Choice",pistolModels[(value>=0&&value<=3)?value:0])){
                    for(int i=0;i<4;++i){
                        ImGui::BeginDisabled(i==3&&!pistol_visual::Ready());
                        if(ImGui::Selectable(pistolModels[i],value==i)){value=i;config.Save();}
                        if(value==i)ImGui::SetItemDefaultFocus();
                        ImGui::EndDisabled();
                    }
                    ImGui::EndCombo();
                }
                ImGui::PopID();
            };
            ImGui::BeginDisabled(!mounted_holsters::Ready());
            pistolChoice("Strife right hand##PistolModel",config.pistolRightModel);
            pistolChoice("Strife left hand##PistolModel",config.pistolLeftModel);
            ImGui::EndDisabled();
            HelpText("Choose either existing pistol independently for each hand, for both local players. Mounted holsters use the matching stowed model. Native restores that hand's previous model. Experimental: check grip alignment and firing effects.");
            if(config.pistolRightModel==3||config.pistolLeftModel==3){
                HelpText("OFF hides this pistol and its foot holster. On foot, its arm uses native unarmed idle/walk/run animation while aiming or shooting. Melee, powers and other actions keep their native animations. Shooting remains active. Mounted hand and stowed models are also hidden; mounted arms, mounted leather holsters and Ultimate remain native. Shared effects stay with one visible pistol.");
                if(!pistol_visual::Ready())ImGui::TextDisabled("OFF unavailable for this executable; native presentation retained.");
                else ImGui::TextDisabled("Animated OFF arm: P1 %s | P2 %s (while aiming/firing)",pistol_visual::PoseReady(0)?"active":"native / waiting",pistol_visual::PoseReady(1)?"active":"native / waiting");
            }
            constexpr const char* warArms[]={"Native","Basic","Augmented"};
            constexpr const char* reticles[]={"Vanilla","Hide in TPS","Hide in normal view","Hide in both views"};
            ImGui::BeginDisabled(!presentation::Ready());if(Choice("War arm appearance",&config.warArmAppearance,warArms,3))config.Save();
            HelpText("Visual arm variant only, normal War on foot, both local players. Does not grant the Tremor Gauntlet or change equipment. Armor that replaces child meshes keeps its native appearance. Native restores the game's choice.");
            if(Choice("Mouse reticle",&config.mouseReticleHideMode,reticles,4))config.Save();
            ImGui::EndDisabled();
            HelpText("Hide the gameplay mouse crosshair in the selected view. Native menus, wheels and the mod menu keep their cursors. Mouse movement, aiming and controller inputs stay unchanged. Vanilla is the default.");
            constexpr const char* bladeModes[]={"Native","Hide left blade","Hide right blade","Hide both blades"};
            ImGui::BeginDisabled(!pistol_visual::Ready());if(Choice("Strife melee blades",&config.strifeBladeVisibility,bladeModes,4))config.Save();
            ImGui::EndDisabled();
            HelpText("Hide each melee blade model independently. Attacks, damage, hand animations and weapon sheathing stay native. Both local players; normal Strife only.");
            ImGui::Unindent();
            }
            if(ImGui::CollapsingHeader("Unlockable skins")){
            ImGui::Indent();
            DrawToggleFeature(config,"Bonus mount skins (experimental)","BonusMountSkins",&config.bonusMountSkinsEnabled,
                "Makes Rampage (War) and Despair (Strife) available in the game's Skins page without the optional PAK. A bonus mount already selected stays equipped when loading, even with this option OFF, like armor skins. The native mount remains selectable. No automatic equip. OFF restores native menu availability; a PAK or an existing unlock can still keep them available.",cosmetics::Ready());
            if(!cosmetics::SelectionRetentionReady())
                ImGui::TextDisabled("Keeping mount selection on load is unavailable in this session.");
            DrawToggleFeature(config,"Progression skins (experimental)","ProgressionSkins",&config.progressionSkinsEnabled,
                "Separately makes Abyssal, Silver and Gold appearances available for War and Strife in the game's Skins page. Cosmetic availability only: no quests, armor pieces, stats or achievements granted. OFF restores native availability; choose an owned skin before disabling. Native skin selection may remember your equipped appearance.",cosmetics::Ready());

            ImGui::Unindent();
            }
            if(ImGui::CollapsingHeader("Leviathan lightning")){
            ImGui::Indent();
            DrawToggleFeature(config,"Override Leviathan lightning (experimental)","LeviathanVisual",&config.leviathanVisualEnabled,
                "Visual only, for both local players. Select OFF or one of the three native lightning effects without granting the core, its stats or its status effect. Uncheck to restore native visuals. Requires the chosen effect asset to be loaded; otherwise leaves native lightning unchanged. Test foot, mount and Ultimate transitions.",leviathan_visual::Ready());
            if(config.leviathanVisualEnabled){
                constexpr const char* effects[]={"OFF (hide Leviathan lightning)","Effect 1","Effect 2","Effect 3"};
                ImGui::BeginDisabled(!leviathan_visual::Ready());if(Choice("Leviathan effect",&config.leviathanVisualMode,effects,4))config.Save();
                ImGui::EndDisabled();
                if(config.leviathanVisualMode>0&&config.leviathanVisualMode<=3&&
                   !(leviathan_visual::LoadedMask()&(1u<<unsigned(config.leviathanVisualMode-1))))
                    ImGui::TextColored(ImVec4(1,.85f,.25f,1),"Selected effect unavailable here; native visual retained. Change selection or re-enable to retry.");
            }

            ImGui::Unindent();
            }
            ImGui::PopID();
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

            // Temporary V1.28 debugger. These are *heuristic camera*
            // events, never asset / LevelSequence names or proof of dialogue.
            if(ImGui::CollapsingHeader("Cutscenes: detection history (DEBUG)")){
                const auto history=dg::camera_trace::GetCinematicDebugSnapshot();
                ImGui::Indent();
                HelpText("Shows up to 24 recent camera-transition events. "
                    "An ENTER means the mod switched to native view; "
                    "it does NOT identify a cinematic asset.");
                ImGui::Text("Detected: %u | Returned: %u | Candidates: %u",
                    camera.cinematicEntered,camera.cinematicExited,
                    camera.cinematicCandidateBegins);
                ImGui::Text("Currently: %s | Candidate #%u",
                    history.currentlySuspended?"Native view (TPS suspended)":
                    history.activeCandidateId?"Candidate / waiting":"TPS / normal",
                    history.activeCandidateId);
                if(ImGui::Button("Mark cinematic playing NOW (manual debug)")){
                    const bool recorded=dg::camera_trace::MarkCurrentCinematicForDebug();
                    if(c.log)c.log(recorded
                        ?"TPS V1.28 DEBUG UI: scene manually marked by player"
                        :"TPS V1.28 DEBUG UI: no native camera sample yet");
                }
                if(ImGui::IsItemHovered())
                    ImGui::SetTooltip("Use during a cinematic that the mod did NOT detect. "
                        "Captures the most recent native camera view; does not change the game.");
                if(history.hasLastNativeSample){
                    const auto& frame=history.lastNativeSample;
                    const ULONGLONG now=GetTickCount64();
                    const auto age=now>=history.lastNativeSampleTickMs
                        ?now-history.lastNativeSampleTickMs:0ull;
                    ImGui::TextDisabled("Last native view %llu ms ago: pitch %.1f, "
                        "FOV %.1f, gap3D %.0f, threshold %.0f, TPS selection %.0f",
                        static_cast<unsigned long long>(age),
                        frame.nativePitch,frame.nativeFov,frame.gap3D,
                        frame.candidateDistanceThreshold,frame.selectedTpsDistance);
                }
                ImGui::Spacing();
                if(ImGui::BeginTable("CutsceneDetectionHistory",5,
                    ImGuiTableFlags_RowBg|ImGuiTableFlags_BordersInnerH|
                    ImGuiTableFlags_SizingStretchProp|
                    ImGuiTableFlags_ScrollY,ImVec2(0.0f,235.0f))){
                    ImGui::TableSetupColumn("#",ImGuiTableColumnFlags_WidthFixed,48.0f);
                    ImGui::TableSetupColumn("Event",ImGuiTableColumnFlags_WidthFixed,88.0f);
                    ImGui::TableSetupColumn("Scene",ImGuiTableColumnFlags_WidthFixed,62.0f);
                    ImGui::TableSetupColumn("Timing",ImGuiTableColumnFlags_WidthStretch,1.0f);
                    ImGui::TableSetupColumn("Why / native camera",ImGuiTableColumnFlags_WidthStretch,3.0f);
                    ImGui::TableHeadersRow();
                    const ULONGLONG now=GetTickCount64();
                    for(uint32_t index=history.count;index>0;--index){
                        const auto& event=history.events[index-1];
                        const ULONGLONG age=now>=event.tickMs?now-event.tickMs:0ull;
                        const char* reason=event.noActorStatic?"STATIC_NO_ACTOR":
                            !event.actorValid?"MOVING_NO_ACTOR":
                            event.distanceExceeded?"TPS_DISTANCE":
                            !event.nearPawn?"FAR_OR_HEIGHT":
                            !event.normalAngles?"PITCH":"GAMEPLAY_SHAPE";
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%u",event.serial);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextUnformatted(
                            dg::camera_trace::CinematicDebugPhaseLabel(event.phase));
                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("%u",event.candidateId);
                        ImGui::TableSetColumnIndex(3);
                        ImGui::Text("%.1fs ago",static_cast<double>(age)/1000.0);
                        if(event.phase==dg::camera_trace::CinematicDebugPhase::ManualMark)
                            ImGui::TextDisabled("frame age %llu ms",
                                static_cast<unsigned long long>(event.durationMs));
                        else if(event.durationMs)
                            ImGui::TextDisabled("duration %llu ms",
                                static_cast<unsigned long long>(event.durationMs));
                        ImGui::TableSetColumnIndex(4);
                        ImGui::Text("%s | 3D %.0f / %.0f | pitch %.1f FOV %.1f",
                            reason,event.gap3D,event.candidateDistanceThreshold,
                            event.nativePitch,event.nativeFov);
                        if(ImGui::IsItemHovered())
                            ImGui::SetTooltip("Native view shift %.0f; normal baseline gap %.0f + TPS %.0f + 40. Camera-only diagnostic, not real cinematic title. Native yaw %.1f",
                                event.nativeViewShift,event.distanceBaseline,
                                event.selectedTpsDistance,event.nativeYaw);
                    }
                    ImGui::EndTable();
                }
                if(history.count==0)
                    ImGui::TextDisabled("No events yet. Trigger a camera transition in TPS.");
                HelpText("V1.29 TPS_DISTANCE = native camera has moved beyond "
                    "its calibrated normal position by your TPS camera distance. "
                    "Native camera baseline is NOT the visible TPS camera distance.");
                HelpText("SHORT_CUT = large native-camera jump below 500 ms, "
                    "not proof of a named cutscene. STATIC_NO_ACTOR means "
                    "player disappeared but camera did not move: pause/menu possible. "
                    "REJECTED = brief unconfirmed camera change.");
                ImGui::TextWrapped("USER_MARK = user observation only. "
                    "In-place cinematic with a static native camera may still be missed.");
                ImGui::Unindent();
            }

            if(ImGui::CollapsingHeader("Depth of field")){
                ImGui::Indent();
                HelpText("Independent settings for each gameplay view and both local players. Cinematics keep their authored effect. Motion blur and fog are unchanged.");
                for(int i=0;i<2;++i){
                    auto& profile=i==0?config.nativeBlur:config.tpsBlur;
                    ImGui::PushID(i==0?"NativeDOF":"TPSDOF");
                    ImGui::Separator();ImGui::TextUnformatted(i==0?"Native view":"TPS view");
                    DrawToggleFeature(config,"Disable depth-of-field blur","Disable",&profile.disabled,
                        "Takes precedence over custom settings. Uncheck to restore the effect below.",camera_focus::BlurReady());
                    ImGui::BeginDisabled(profile.disabled);
                    if(ImGui::Checkbox("Customize background blur",&profile.custom))config.Save();
                    if(profile.custom){
                        if(NumberRow("Clear distance behind character","Distance",&profile.distanceMeters,0,camera_blur::MaxDistanceMeters,10,"%.1f m","%.2f"))config.Save();
                        if(NumberRow("Blur amount","Amount",&profile.amount,0,5,0.6f,"%.2f %%","%.2f"))config.Save();
                        HelpText("The whole character stays sharp. More distance pushes the background blur farther away. Amount is an absolute Gaussian blur size, not a multiplier; zero removes it. Custom mode replaces the level's DOF method for this view.");
                    }else ImGui::TextDisabled("Level effect%s",i==0?"":" with your TPS character-focus setting");
                    ImGui::EndDisabled();ImGui::PopID();
                }
                ImGui::Unindent();
            }
            if(ImGui::CollapsingHeader("Third Person camera")){
            ImGui::Indent();
            if (ImGui::Checkbox("Third Person", &config.thirdPersonEnabled)) {
                config.Save();
                *c.lastAction=config.thirdPersonEnabled?"Third Person ON":"Third Person OFF";
            }
            ImGui::TextDisabled("%s | TPS camera attachments observed: %u",
                camera.viewReady && camera.armReady?"Hook ready":"Hook unavailable",
                camera.tpsAttachedFrames);
            ImGui::TextWrapped("Moves the camera behind each local player. Use the configured Hotkey or long controller press to toggle TPS.");
            ImGui::Spacing();
            if (config.thirdPersonEnabled) {
                ImGui::Indent();
                DrawSectionTitle("TPS mouse camera");
                ImGui::TextDisabled("Mouse reticle visibility: Gameplay -> Appearance. Menus keep their native cursor.");
                ImGui::TextDisabled("Relative mouse and Camera Forward firing remain automatic.");
                ImGui::Spacing();
                DrawSectionTitle("TPS camera attachment");
                ImGui::TextDisabled("TPS follows the live local player permanently.");
                ImGui::TextDisabled("TPS pivot follows player feet on the character axis.");
                ImGui::TextDisabled("Foot Origin appears with other TPS camera values below.");

                DrawSectionTitle("TPS-only FOV");
                DrawToggleFeature(config,"Enable TPS FOV override","TPSFov",&config.thirdPersonFovEnabled,
                    "Solo and local co-op have separate values. Coop FOV applies to both split-screen views.",camera.viewReady);
                if(config.thirdPersonFovEnabled){
                    if(NumberRow("FOV solo","TPSFovSolo",&config.thirdPersonFovDegrees,40,140,110,"%.0f deg","%.1f"))config.Save();
                    if(NumberRow("FOV coop","TPSFovCoop",&config.thirdPersonCoopFovDegrees,40,140,110,"%.0f deg","%.1f"))config.Save();
                }
                DrawSectionTitle("TPS depth of field");
                DrawToggleFeature(config,"TPS character focus (experimental)","TPSLensFocus",&config.tpsLensFocusEnabled,
                    "Keep the level's depth-of-field method, clear region and distant blur. In TPS, disable the near Gaussian layer; only enlarge a clear region already overridden by the camera. Foot, mount and Ultimate; both local players. OFF keeps focal-distance correction alone. The separate depth-of-field controls can replace this effect.",camera_focus::LensReady());
                DrawSectionTitle("Normal-form TPS camera");
                if(NumberRow("Distance","TPSDistance",&config.thirdPersonDistanceMultiplier,0,3,0.15f,"%.2fx","%.3f"))config.Save();
                if(NumberRow("View pitch","TPSPitch",&config.thirdPersonPitchDegrees,-75,65,-12,"%.1f deg","%.1f"))config.Save();
                if(NumberRow("Vertical offset","TPSHeight",&config.thirdPersonHeightOffset,-1000,1000,0,"%+.0f units","%.1f"))config.Save();
                ImGui::Spacing();
                constexpr const char* stickOptions[]={"OFF (native sticks)","Camera Left / Move Right","Camera Right / Move Left"};
                if(Choice("TPS camera joystick",&config.tpsCameraStickMode,stickOptions,3)){
                    config.cameraControllerOrbitEnabled=config.tpsCameraStickMode!=0;
                    config.cameraOrbitInputEnabled=config.cameraControllerOrbitEnabled;
                    config.Save();
                }
                if(config.tpsCameraStickMode!=0){
                    if(NumberRow("Stick speed","TPSStickSpeed",&config.cameraStickSpeed,30,360,180,"%.0f deg/s","%.1f"))config.Save();
                }
                ImGui::TextDisabled("Only Third Person, focused game, closed overlay.");
                ImGui::TextDisabled("TPS swaps complete roles: camera Left = movement Right.");
                ImGui::TextDisabled("Native in-game stick swap is accounted for by Camera Forward.");
                if(NumberRow("Foot origin correction","TPSFoot",&config.tpsFootAnchorOffset,-500,500,0,"%+.0f units","%.1f"))config.Save();
                DrawSectionTitle("Ultimate form TPS camera");
                ImGui::TextWrapped("Separate distance and vertical offset while transformed. Normal-form settings stay independent.");
                if(NumberRow("Distance (Ultimate form)","TPSUltimateDistance",&config.tpsUltimateDistanceMultiplier,0,3,
                    defaults::UltimateDistance,"%.2fx","%.3f"))config.Save();
                if(NumberRow("Vertical offset (Ultimate form)","TPSUltimateHeight",&config.tpsUltimateHeightOffset,-1000,1000,
                    defaults::UltimateHeight,"%+.0f units","%.1f"))config.Save();
                DrawSectionTitle("Horseback TPS camera");
                ImGui::TextWrapped("Separate distance and vertical offset while riding. Foot and Ultimate settings stay independent.");
                if(NumberRow("Distance (Horseback)","TPSHorseDistance",&config.tpsHorseDistanceMultiplier,0,3,
                    defaults::HorseDistance,"%.2fx","%.3f"))config.Save();
                if(NumberRow("Vertical offset (Horseback)","TPSHorseHeight",&config.tpsHorseHeightOffset,-1000,1000,
                    defaults::HorseHeight,"%+.0f units","%.1f"))config.Save();
                if(ImGui::Button("TPS Camera Values Default"))config.ResetTpsCameraValues();
                ImGui::Unindent();
            }
            if (config.thirdPersonEnabled) {
                ImGui::Indent();
                DrawSectionTitle("TPS controls and aiming");
                ImGui::TextDisabled("TPS mouse fires are rebindable without changing controller bindings.");
                DrawToggleFeature(config,"Auto suspend TPS during detected cutscenes (test)","TPSCutscene",
                    &config.tpsCutsceneAutoSuspend,"Yield to a native cinematic camera, then return to TPS. Experimental.",camera.viewReady);
                ImGui::TextDisabled("Pause/wheels keep TPS; temporary-view shortcut has priority.");
                ImGui::TextDisabled("Camera Forward offers 3D aiming or simple native trajectories.");
                ImGui::Separator();
                ImGui::TextDisabled("Native movement and camera isolation are automatic in TPS.");
                ImGui::Separator();
                ImGui::TextDisabled("Unified TPS aiming (on foot + mounted)");
                if(ImGui::Checkbox("3D aiming (experimental)",&config.tps3DAimEnabled))config.Save();
                HelpText("Same camera and charged-shot correction in both modes. ON adds vertical aiming; OFF keeps native vertical aiming/arc. Mouse/controller, P1/P2, Camera Forward only.");
                if(config.tps3DAimEnabled){
                    if(NumberRow("Shot vertical angle","TPSAimPitchOffset",&config.tpsAimPitchOffsetDegrees,-45,45,12,"%+.1f deg","%.1f"))config.Save();
                    ImGui::TextDisabled("Positive raises the firing ray. +12 deg compensates the default -12 deg view pitch; camera settings do not change.");
                }
                constexpr const char* kMountedModes[]={
                    "Camera Forward (free camera while firing)",
                    "Vanilla Aim (pause camera while firing)"
                };
                if(Choice("TPS aiming mode##MountedMode",
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
                    if(Choice(title,&index,kButtonLabels,kButtonCount)){
                        mask=kButtons[index];config.Save();
                    }
                    layout::SameLineIfFits(ImGui::CalcTextSize("Default").x+2*ImGui::GetStyle().FramePadding.x);
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
                DrawSectionTitle("TPS mouse fire bindings");
                ImGui::TextWrapped("TPS only: route physical mouse buttons to native primary or secondary fire.");
                const int mouseCodes[]={0,VK_LBUTTON,VK_RBUTTON,VK_MBUTTON,VK_XBUTTON1,VK_XBUTTON2};
                const char* mouseLabels[]={"Unbound","Left click","Right click","Middle click","Mouse X1","Mouse X2"};
                auto MouseFireBind=[&](const char* label,int& binding,int& other,int original){
                    int index=0;
                    for(int i=0;i<6;++i)if(mouseCodes[i]==binding){index=i;break;}
                    if(Choice(label,&index,mouseLabels,6)){
                        const int selected=mouseCodes[index];
                        if(selected && selected==other)other=binding;
                        binding=selected;
                        config.Save();
                    }
                    layout::SameLineIfFits(ImGui::CalcTextSize("Default").x+2*ImGui::GetStyle().FramePadding.x);
                    ImGui::PushID(label);
                    if(ImGui::SmallButton("Default")){
                        binding=original;
                        if(other==binding)other=original==VK_LBUTTON?VK_RBUTTON:VK_LBUTTON;
                        config.Save();
                    }
                    ImGui::PopID();
                };
                MouseFireBind("Primary fire##MouseTPS",config.tpsMousePrimaryButton,
                    config.tpsMouseSecondaryButton,VK_LBUTTON);
                MouseFireBind("Secondary fire##MouseTPS",config.tpsMouseSecondaryButton,
                    config.tpsMousePrimaryButton,VK_RBUTTON);
                ImGui::TextDisabled("Defaults: Left=primary, Right=secondary; both use TPS Camera Forward.");
                ImGui::TextDisabled("Choosing a button already assigned to the other fire swaps them.");
                ImGui::Spacing();
                DrawSectionTitle("Controller bindings");
                ImGui::TextDisabled("Native in-game controls take priority. These bindings are mod detectors.");
                BindGamepadMask("Controller long hold##TPS",config.tpsToggleHoldButtonMask,64);
                if(DrawDelayRow("Long press delay","ToggleTPSDelay",&config.tpsToggleHoldMs,250,2000,500))config.Save();
                ImGui::Spacing();
                BindGamepadMask("Controller recenter##TPS",config.tpsRecenterButtonMask,64);
                ImGui::Spacing();
                BindGamepadMask("Controller temporary view##TPS",config.tpsSuspendButtonMask,128);
                if(config.tpsKeyboardKeys[static_cast<size_t>(dg::config::TpsKeyboardAction::Suspend)]=='E')
                    ImGui::TextDisabled("E deliberately does BOTH: native gear + temporary view after hold delay.");
                if(DrawDelayRow("Temporary view delay","HoldTPSDelay",&config.tpsSuspendDelayMs,0,3000,300))config.Save();
                ImGui::Spacing();
                BindGamepadMask("Use selected equipment##TPS",config.tpsGrenadeButtonMask,128);
                HelpText("Uses selected equipment for either local player. This action is separate from wheel selection. "
                    "Hold to equip; release uses the existing native stow pulse.");
                ImGui::Separator();
                ImGui::TextDisabled("Native GAME bindings: manually match in-game controls");
                BindGamepadMask("Detect primary fire (default RT)##TPS",config.tpsFirePrimaryMask,131072);
                ImGui::TextWrapped("Match the in-game SHOOT button. This detects firing but does not rebind the game.");
                BindGamepadMask("Detect alternate fire (default RB)##TPS",config.tpsFireSecondaryMask,512);
                ImGui::TextWrapped("Optional second SHOOT input; OFF disables this detector.");
                BindGamepadMask("Detect wheel opening (default LB)##TPS",config.tpsWheelPadMask,256);
                HelpText("This held button lets the game's native right stick select wheel items, "
                    "instead of being intercepted by TPS camera control.");
                ImGui::TextDisabled("LB+Y: original native ability behavior, no modded aiming or key translation.");
                HelpText("Wheel detector keys never emit fake game input. "
                    "Change them here too when rebinding the game's wheel controls.");
                ImGui::TextDisabled("Keyboard Use Gear follows the game's own native mapping.");
                HelpText("Controller bindings apply to P1 and P2. Each player has independent equipment hold/stow, temporary view and recenter. A long press toggles the shared TPS option.");
                ImGui::TextDisabled("P1 equipment stow pulses: %u | DOWN %u / UP %u",
                    t.grenadeNativeReleasePulses,t.grenadePhysicalDown,t.grenadePhysicalUp);
                if(config.tpsGrenadeButtonMask!=0 &&
                   (config.tpsGrenadeButtonMask==config.tpsToggleHoldButtonMask ||
                    config.tpsGrenadeButtonMask==config.tpsSuspendButtonMask ||
                    config.tpsGrenadeButtonMask==config.tpsRecenterButtonMask))
                    ImGui::TextDisabled("Shared binding: one button triggers multiple actions.");
                if(config.tpsToggleHoldButtonMask!=0 &&
                   config.tpsToggleHoldButtonMask==config.tpsSuspendButtonMask)
                    ImGui::TextDisabled("Long TPS and temporary view share one button.");
                HelpText("Native game actions remain unless deliberately remapped "
                    "for grenade. LT/RT and other game buttons can conflict with "
                    "their original actions. OFF disables the mod shortcut only.");
                ImGui::Unindent();
            }
            ImGui::Unindent();
            }
            if(ImGui::CollapsingHeader("Original game camera")){
            ImGui::Indent();
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
                camera.viewReady,nullptr,"Degrees");
            ImGui::TextDisabled("Global FOV applies outside TPS, or when TPS FOV is OFF.");

            DrawSectionTitle("Camera distance");
            ImGui::TextWrapped("SpringArm Zoom is mainly for vanilla. TPS keeps its own camera distance.");
            bool zoomChanged=false;
            if(ImGui::Button("Zoom -##Camera")){config.cameraZoomPercent-=10;zoomChanged=true;}
            ImGui::SameLine();
            if(ImGui::Button("Zoom +##Camera")){config.cameraZoomPercent+=10;zoomChanged=true;}
            zoomChanged|=NumberRow("Zoom","VanillaZoom",&config.cameraZoomPercent,-75,200,0,"%+.0f%%","%.1f");
            if(zoomChanged){config.cameraZoomPercent=std::clamp(config.cameraZoomPercent,-75.0f,200.0f);config.Save();}
            DrawSectionTitle("Camera height");
            ImGui::TextWrapped("Vanilla camera height offset, default zero. TPS height is independent.");
            if(NumberRow("Height","VanillaHeight",&config.cameraHeightOffset,-1500,1500,0,"%+.0f units","%.1f"))config.Save();
            DrawSectionTitle("Camera angle");
            ImGui::TextWrapped("Vanilla camera pitch offset. TPS uses its own view pitch.");
            if(NumberRow("Pitch","VanillaPitch",&config.cameraPitchDegrees,-35,35,0,"%+.1f deg","%.1f"))config.Save();
            DrawSectionTitle("Horizontal camera rotation (yaw)");
            ImGui::TextWrapped("Turn the camera left/right. Zero degrees is the native view.");
            if(NumberRow("Yaw","VanillaYaw",&config.cameraYawDegrees,-180,180,0,"%+.1f deg","%.1f"))config.Save();

            ImGui::EndDisabled();
            DrawSectionTitle("Camera keyboard bindings");
            HelpText(
                "Default: Up/Down = height, PageUp/PageDown = zoom. "
                "Home/End = tilt; Left/Right = yaw. "
                "Press Rebind then the new key; Esc cancels. "
                "Bindings can be changed or cleared in either camera mode. "
                "They work only in native view, with the overlay closed and the game focused."
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
            ImGui::TextDisabled("Native-view zoom temporarily disables DOF; TPS uses per-player focus.");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("The native DOF quality is restored when native-view zoom is off. Supported TPS views retain the level method and per-player focus.");
            ImGui::TextDisabled("Native output changes. Actual framing requires in-game verification.");
            ImGui::Unindent();
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
            layout::SameLineIfFits(ImGui::CalcTextSize("Rebind Menu Key").x+2*ImGui::GetStyle().FramePadding.x);

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

            layout::SameLineIfFits(ImGui::CalcTextSize("Default").x+2*ImGui::GetStyle().FramePadding.x);
            if(ImGui::Button("Default##MenuKey")){config.menuKey=defaults::MenuKey;config.Save();}
            ImGui::Spacing();

            if (ImGui::Button(
                    "Save",
                    ImVec2(110.0f, 0.0f))) {
                *c.lastAction = config.SaveNow() ? "Configuration saved"
                    : "Save failed; changes remain pending. See log.";
            }

            ImGui::SameLine();

            if (ImGui::Button(
                    "Reload",
                    ImVec2(110.0f, 0.0f))) {
                const bool loaded=config.Load();
                c.captureCameraKeyIndex->store(-1);
                *c.lastAction = loaded ? "Configuration reloaded"
                    : "Reload failed. See log.";
            }

            ImGui::TextDisabled(
                "Save writes the current INI; Reload reads it without resetting defaults.");

            DrawSectionTitle("F1-F12");
            ImGui::TextWrapped("Fill Wrath, Ultimate, Synergy or all three gauges: one press sets 100%. Existing assignments stay unchanged.");

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

                    ImGui::SetNextItemWidth(layout::ControlWidth(390.f));

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
                            if(action==static_cast<int>(config::Action::ActionRecovery)||action==static_cast<int>(config::Action::DashMovementRecovery))continue;
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
                "Defaults: F1 all gauges | F2 Ultimate | F3 Synergy | F4 Wrath | F5-F9 Unlimited | F12 HUD"
            );
            ImGui::EndTabItem();
        }


        if (ImGui::BeginTabItem("Updates")) {
            ImGui::Spacing();
            DrawSectionTitle("In-game mod updates");
            ImGui::TextWrapped("Check GitHub and queue a verified update. "
                "Loaded DLLs are replaced only AFTER the game closes.");

            if(ImGui::Checkbox("Auto check update",&config.updaterAutoCheck)){
                config.Save();
                if(config.updaterAutoCheck)dg::update::Check(config.updaterBeta);
            }
            HelpText("Check the selected channel once at startup. A newer build shows an 8-second notification and a yellow reminder above the tabs. Enabling this option also checks now.");
            ImGui::BeginDisabled(update.busy);
            if (ImGui::Checkbox("Beta updates (debug)", &config.updaterBeta)) {
                config.Save();
                dg::update::Invalidate();
                *c.lastAction = config.updaterBeta ?
                    "Updates channel: debug (beta)" : "Updates channel: main (stable)";
            }
            ImGui::EndDisabled();
            ImGui::TextDisabled("OFF: main (stable)  |  ON: debug (experimental)");
            ImGui::TextDisabled("Current channel: %s", config.updaterBeta ? "debug" : "main");
            ImGui::Spacing();

            ImGui::Text("Running mod: %s",dg::build::Version);
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
                    ImGui::TextWrapped("WARNING: this version is not newer than %s. You may be downgrading the mod.",dg::build::Version);
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
            if(ImGui::CollapsingHeader("Menu appearance")){
                if(NumberRow("Font size","MenuFontSize",&config.menuFontSize,menu::MinFontSize,menu::MaxFontSize,menu::DefaultFontSize,"%.1f px","%.1f"))config.Save();
                ImGui::TextWrapped("Changes mod menu text and its popups only. 13 px is the original size. Saved in the INI; takes effect on the next frame.");
            }
            ImGui::Text(
                "Darksiders Genesis Enhanced"
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
    ImGui::TextWrapped(
        "Last action: %s",
        c.lastAction->c_str()
    );

    ImGui::PopTextWrapPos();
    ImGui::End();

    if (!open) {
        c.overlayVisible->store(false);
    }
}

} // namespace dg::overlay
