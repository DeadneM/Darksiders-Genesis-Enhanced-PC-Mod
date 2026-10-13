# V1.11 | Readable grouped overlay and lightweight debug

## User-requested scope

- Retain current cumulative V1.10 game behavior and native LB+Y ability. Avoid TPS mouse until the very end.
- User clarified exact **visual** boot order: THQ logo, Airship Syndicate logo, full-screen black Save Data warning, full-screen black Controller warning, boot cinematic. The game-reported three native 0x40-byte StartupScreens records have active flags 1,1,0, consistent with two warnings, but actual per-record text identity and presentation callback are not yet proven. The V1.10 native startup timeline continues, Skip Logos and Skip Intro continue, and Skip Warning loop bypass remains OFF to avoid cursor/reticle regression. User-observed visual order is not claimed as dynamically measured by the native hooks.
- Wrath: V1.09 located a **candidate float source** at local player+0xEA0 -> component+0x3A0. It changed 14 times positively and 9 negatively in V1.09; V1.10 13 positive, 3 negative events. The previously installed exact signature AddWrath RVA 0x660570 was never called in runtime (observed 0), likewise AddChaos RVA 0x62E060, so the multipliers remain *unverified* rather than being represented as finished. Synergy native additive hook RVA 0x65A1A0 is gameplay-validated; user previously confirmed multipliers.

## Overlay V1.11 user interface

Gameplay groups become collapsible and independent: HUD / aiming effects; Player movement; Combat and resource gauges; Horse and riding; System and startup. TPS camera and original game camera become separate camera groups. Grouping does not change any feature or binding.

All four **Hotstreak, Wrath, Ultimate / Chaos and Synergy** now use the same common checkbox/multiplier/manual/default helper with identical spacing and range x0-x100, default x2. Verified Synergy and unverified Wrath/Ultimate are labeled separately; a ready hook is not called a working game behavior. Options save exactly as before.

The keyboard binding controls for TPS (toggle, recenter, temporary view, equipment wheel, ammunition/ability wheel), horse summons, and original-camera shortcuts are now five-column readable tables: Action / Current key / Change / Clear / Default. Current key is separated from action text; unbound keys are visibly marked; Escape cancels capture. Gamepad binding options remain as before, grouped distinctly from keyboard tables. Standard keys F1-F12 remain their own Hotkeys tab. The native Use Gear action is still R3 (not a wheel).

Window has more horizontal room and consistent item spacing; verbose advanced diagnostics sections were removed from menu. Each feature retains short descriptions plus light status. Full exact executable details remain optionally visible on About tab.

## Log cleanup

- Retain DXGI-to-ASI V1.10 `Startup V1.10 TIMELINE` event stamps and original 3-record enabled flags, but retire redundant per-record raw pointers/word fingerprints.
- Retain bounded potential Wrath candidate read and first 16 meaningful resource deltas; stop per-shot yaw/caller callsite logging while keeping aggregate path counters for shutdown.
- Cut high-volume outgoing damage logging (first 3 and each 1,000), reticle interception (first 2 and each 10,000), pad edges (first 4), duplicate gauge boost/setter diagnostics (first few, then very occasional).
- Retire one-time direct CALL byte scan producing zero native entries; do not confuse this ineffective scan with proof of missing UE dynamic dispatch. Existing hook readiness and runtime call counts remain.

No modifications to Wrath/Chaos gameplay gain, player movement or warning presentation logic are included. ZIP distribution remains four root entries `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`, Windows x64 and `debug` branch only. Preserve previous experimental archives and stable `main`.

## V1.11 test plan

1. Open overlay Insert, confirm grouped Gameplay and Camera subsections, four identical recharge controls, no overlapping labels when keys are long (e.g. Page Down), active Rebind and Escape cancel.
2. Change a keyboard shortcut and Default it; save/reload INI; verify F1-F12 untouched.
3. Confirm live Synergy multiplier, Hotstreak, horse, RT/RB and native LB+Y unchanged; native LB-wheel selects correctly and R3 uses selected equipment.
4. Cold launch; two black-background warning messages should still appear (no SkipWarning suppression attempted), original boot order maintained, intro skipped if configured. Per-session log should have significantly less repeated noise and still contain startup phase timeline and resource counters.
