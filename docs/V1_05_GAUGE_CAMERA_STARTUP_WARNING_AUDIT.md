# V1.05 | Native gauge identity, camera reset consistency, safe startup warnings audit

Exact supported game EXE SHA-256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`; branch `debug`, `main` unchanged.

## V1.04 user log
All 3 native hook signatures READY; totals `Wrath=0 Chaos=0 Synergy=35 boosted=0/0/0`. Cannot claim gauges work. Mounted CAMERA_FORWARD injected analog 946 times HORSE and 137 FOOT, 4 R3 hold/stow pulses and 2 Strife shaders matched. Other behavior accepted by user.

## Source/INI/runtime mismatches
Camera Values Default and fresh/default INI erroneously assigned vanilla height=50; `Vanilla##CameraHeight` user button assigns 0. V1.05 fixes height=0 across `ConfigStore.cpp`, `ConfigStore.h`, `RuntimeSettings.h`, shipped INI and default reset. Existing identical old saved default 50 migrates once to 0, preserving other custom values. TPS framing has its own historical +50 constant, kept independent. TPS Camera Values Default previously failed to restore TPS FOV override ON; now does. Both actions save/publish runtime immediately. TPS enable state and keybinds unchanged.

Three separate gauges now ON default and x2, migrate old all-OFF shipped preset only if all 3 OFF and config revision<2138. Native return type of `SetCurrentSynergy` RVA 0x583040 corrected from float to void: exact EXE disassembly shows void ret after writing field `+0x5A4`. First 12 observed calls log raw owner pointer versus local owner, requested value, current and max; session summary prints eligibility reasons. Never patch unknown actor or fabricate gain.

## Two visible startup warnings: real code lead, unproven visual owner
Native exact-EXE RVA 0x25FE90 calls startup settings getter RVA 0x2603F0. Owner+0xF8 yields settings; pointer+0x50 contains a record array and +0x58 count. Loop at RVA 0x25FED0 strides 0x40 per entry, tests byte +0x39 and, when set, passes entry+0x20 into function RVA 0xAC8760. This function may transform/prepare FText and NOT directly draw warnings. Old bypass set loop end=start (3 records skipped), possibly skipping required initialization and creating the historic crosshair regression. V1.05 only reads/logs record count and flags from existing startup movie-copy callback, AFTER the native loop, with bounds and address checks. No warning suppression or native lifecycle bypass. Trace actual widget/Slate presentation and the two localized screen text assets before attempting display-only hiding.

## Next test
Verify fresh/old unchanged INI starts height 0, both camera reset buttons and saved values, all three gauge toggles ON x2, inspect `Gauge V1.05 AUDIT` and owner counters after combat. Check `StartupScreens V1.05` record flags, reticle normal and original two messages. Do not claim 3 native gauges or warning skip fixed until runtime test.
