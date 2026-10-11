# V0.83 | Horse Everywhere native restriction research + human-readable live overlay

Date: 2026-10-11. Based on **dev/v0.78-tps-off-aim-line-arc-audit**, the previously user-validated camera, controller, horse speed/stamina, combat, HUD, aiming and cursor code. The V0.82 UI component ProcessEvent experiment was unvalidated and is deliberately **not merged** here, nor is V0.80's failed beam suppression. This is a separate branch.

## HUD and Strife's turquoise line/semicircle
User reports the cyan world-space straight aiming line and semicircle **stay visible even with Hide HUD enabled**. That proves they are *not suppressed by the game's global HUD-hide CVar*, but it does NOT by itself prove they have no UI system origin. They may be world-space VFX/targeting components. **DO NOT MODIFY** native `ui.HideHud` to hide them, nor treat the wrong `HideBeamEffect` path as working.

## Horse Everywhere
The user requests ON/OFF removal of horse summoning restrictions. Auditing the EXACT exe confirmed:
- `AMayhemPlayerCharacter::SERVER_SpawnHorseMount_Implementation` native function **RVA 0x682860**, signature `48 8B C4 55 57 41 54 41 55 41 56 48 8B EC 48 81 EC 80 00 00`. Its first branch checks whether the mount already exists, followed by substantial actor/class conditions, so arbitrary calling or unconditional guard bypass is **NOT SAFE** and might break quest/level/network functionality.
- Native functions are named `DoSummonHorseMount` and `DebugDoSummonHorseMount` in the game's UE reflection metadata. The actual no-summon **AREA** condition was NOT yet established as a stable native gate. It may occur in Blueprint, before server summon.
- **New INI** `[Features] HorseEverywhere=0`, default **OFF**; runtime toggle exists in the overlay labeled **"Horse Everywhere (experimental research)"**. ConfigRevision 2124 safely adds the option while preserving existing user values including `ThirdPerson=0`.
- New **signature-checked, passive server summon probe** logs when this exact handler runs. The original is always called with unchanged arguments. It tracks all server spawn calls and calls while the new option is ON. Native horse tunings are untouched.
- **Important acceptance limit:** at V0.83 the switch authorizes diagnostics ONLY; **it does not currently lift zone restrictions**. This is intentionally explicit in the UI and logs. Do not claim it works! An actual bypass requires identifying the exact zone/ability check; no direct spawn until then.
- Test by attempting the game's normal summon control once in an allowed area and once in a forbidden area; collect `Horse Everywhere V0.83 probe totals` to learn whether denied summons reach the native server. No spoofed input, no forced mount, no pointer lifetimes violated.

## Overlay cleanup
- All tunable features show plain-language explanations, with "Hook ready" distinct from real activity.
- HUD and Hide Reticle explain what they actually hide, while preserving world-space aim line.
- Recovery shows action unlock event count and optional expanded movement diagnostics.
- Pistol/melee/Hotstreak show observed events separately; exact damage and engine fields hidden under "Advanced combat diagnostics".
- Horse speed, sprint, stamina show useful "waiting for horse", horse count and native activity. Their raw addresses and pointers remain only in "Advanced horse diagnostics".
- Camera shows observed game frames and attached TPS views, with advanced counters collapsed.
- Controller controls show user-facing behavior rather than version tags; aim visual experiment clearly NOT YET WORKING.
- About tab hides hash and EXE size in "Advanced executable details".
- Startup options explain that a restart is needed. Graphics adapter keeps its existing native behavior.

## Preserve and rollback
Unchanged: F5, L3, R3 controls, controller strafe/aim direction, horse speed and sprint, camera pose, DOF, cursor/reticle, UI hide native, Skip Logos/Intro/Warning, action recovery and damage. This branch differs in config/runtime declaration of the research flag, one isolated passive summon probe in HorseFeature, overlay explanation/status and documentation. The old V0.78 ZIP remains the stable gameplay checkpoint. No official release without user test.
