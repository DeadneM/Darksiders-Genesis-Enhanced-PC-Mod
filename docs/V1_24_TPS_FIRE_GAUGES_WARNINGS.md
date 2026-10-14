# V1.24 - TPS mouse shooting, alternate Camera Forward, resource and warning research

**Date:** 2026-10-14. **Branch:** `debug` only. **Base:** V1.23, user-confirmed as working for TPS camera 360-degree rotation. **Status:** EXPERIMENTAL until CI and user gameplay test.

## Verified V1.23 user log
- Physical relative RawInput: 17,154 packets, including 3,817 near a window border. Edge recapture: 37, zero failures. User confirms unlimited camera rotation. **Preserve this implementation.**
- Synergy native additive path: 55 events, 27 positive (27 boosted), 28 zero, none positive skipped. This is the verified multiplier; leave it untouched.
- Wrath read-only candidate field rises +4, +1, +4 (total +9) with zero AddWrath callbacks. AddChaos/Ultimate zero. No proven multipliers on these two resources.
- TPS cutscene heuristic 2 entries, 1 exit, 288 missing-actor samples. Not verified Sequencer or dialogue identity. R3 temporary vanilla camera is preserved.

## V1.24 mouse primary / secondary fire
- Two drop-down choices in Camera > Third Person > TPS controls, accepting Left, Right, Middle, X1, X2, Unbound; defaults Left primary and Right secondary, with default buttons and collision swapping. Config revision 2140 adds [Controls] TPSMousePrimaryButton=1 and TPSMouseSecondaryButton=2. Personal INIs remain preserved and missing keys use these defaults.
- WndProc translates native Win32 button down/up/double-click messages to the game's original left/right fire events **only** in fresh focused TPS gameplay. Default L/R passes through unchanged. Outside TPS, overlay, radial wheel, native temporary view, alt-tab and detected cutscenes, mouse events pass through unchanged. Non-fire MMB/X events not selected for fire also retain native behavior. This is an experiment, since any Unreal path bypassing WndProc may need a different solution.
- Both *configured* mouse fire buttons now trigger the existing native camera-heading corrections via GetPlayerLookInput/GetPlayerLookRotation. Previous V1.23 code checked only physical LMB, missing RMB secondary. Logging includes V1.24 MOUSE_FIRE secondaryForwardLook, routed buttons and suppressed native left/right events.
- No new Hide Reticle code, no mouse cursor visibility hooks, no virtual XInput for mouse. V1.23 border recapture, gamepad RT/RB, LB native wheel, R3 selected equipment, horse and validated modules unchanged.

## Resource research (read-only)
- On a verified **positive local Synergy native Add**, store a monotonic timestamp, leaving gain math unchanged. Wrath candidate changes now print milliseconds since the last positive Synergy event along with charge/damage timing. This will help distinguish coincident combat rewards vs pickups, not prove a shared gain path.
- Continue searching the actual positive Wrath/Ultimate native source. AddWrath/AddChaos RVA hooks still saw **0 calls** and should not be presented as functional multipliers. No guessed field patch or broad UObject scan.

## Skip Warnings research (read-only)
- The native StartupScreens settings array has 3 records of 0x40 bytes, with two candidate active flags. A newly bounded 8-byte payload fingerprint (record +0x20..+0x27) is printed once per boot, along with record type/enable. These bytes may contain pointers and are **not comparable across process ASLR without analysis**. No claim that they identify the two displayed warning images.
- Keep exact-name THQ/Airship logo substitution and the original MoviePlayer/StartupScreens initialization. Skip Warnings remains OFF and disabled. The known early loop-skipping strategy must not return because it previously regressed cursor/UI behavior. Next step: identify the actual Slate/widget presentation callback after native initialization, then test *presentation only*.

## User acceptance
Test left click primary fire and right click secondary fire **both toward camera heading** on foot and optionally mounted. Swap them, try middle/X1/X2, Unbound and defaults, verify no stuck-down fire when opening menu. Check TPS 360° recapture, R3, LB wheel, controller RT/RB, camera mode, and Alt-Tab. Provide in-game log. Future V1.25 should focus on true dialogue/cutscene identity and native Wrath/Ultimate gain source.

### Delivery
Windows x64 CI must compile, verify a ZIP containing exactly the four root-level files `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`, and push the numbered archive on debug. Preserve V1.23 and older ZIPs. Never touch `main` or `archive`.

## V1.24 Windows x64 build and publication verified (2026-10-14)

GitHub Actions [run 38537645671](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38537645671) completed **SUCCESS**: configure, Release compile, four-root-file ZIP verification, artifact upload, and permanent `debug` archive publication. [Download V1.24 cumulative TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.24_TPS_MOUSE_FIRE_AUDIT_TEST.zip): **753,948 bytes**, Git blob SHA-1 `4048d7eaa1d23ef35b4560ed737065ebfc96c937`. Archived V1.23 ZIP remains present: 752,055 bytes, SHA-1 `567ad7f7854f515ae4f3e091dd87e4a5c65dc013`. The workflow checked exactly `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, and `README.md` at ZIP root. `main` and `archive` branch heads are unchanged. **Windows compilation is not in-game verification**: native secondary shot direction and alternate button remapping remain EXPERIMENTAL awaiting the user's test.

