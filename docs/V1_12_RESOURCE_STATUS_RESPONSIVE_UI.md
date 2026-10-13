# V1.12 - Resource status and responsive controls - 2026-10-13

## Base and goal
Cumulative debug base: 0a13e93 (V1.11 compiled TEST), retaining previously validated functions. V1.11 as a whole has not been user validated. No main/archive writes. Archive remains at 10e0f220; main at 2bc3122c.
Goal: readable common resource controls and truthful functional status.
Acceptance: Windows x64 compilation, exactly four ZIP root files, no changes to native hooks/configuration, then user checks compact overlay and gameplay.

## Changes and preservation
Common tuner status now occupies a separate line instead of fixed x=520.
Below 560px available width, the multiplier slider and exact-value/default controls occupy separate rows.
Wrath and Ultimate explicitly say no confirmed gameplay multiplier effect. Hook installed does not mean gain path works. Synergy retains validated status only when its hook is available.
Build identity corrected: V1.11 still logged V1.10. New identity is 1.12-resource-status-responsive-ui-test.
INI, shortcuts, native LB+Y, RT/RB injection, R3, horse, cameras, HUD, Hotstreak and Synergy hooks unchanged. Existing light debug and startup timeline preserved.

## Native audit and limits
Wrath player+0xEA0 -> component+0x3A0 remains read-only candidate: historical V1.09 14 positive / 9 negative changes; AddWrath and AddChaos zero observed callbacks.
No executable or fresh game log available in this workspace to establish the true gain authority. Do not multiply sampled deltas: spend/refill/maximum changes and local ownership must first be distinguished.
Boot order reported by user: THQ -> Airship -> save warning -> controller warning -> intro.
StartupScreens flags 1/1/0 do not identify the drawing callback. SkipWarning remains OFF; native initialization preserved. Presentation/duration suppression remains unresolved.

## Build / test / rollback
GitHub Windows CI builds the cumulative ASI and DXGI loader with pinned dependencies, then checks the four root entries:
dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md.
Status before CI: EXPERIMENTAL, not yet CONSTRUIT. CI result and ZIP integrity must be checked separately.
Test: Insert, reduce window width, enable the three charge controls, enter x1/x2 then Default; verify key tables, Escape capture cancellation, INI save/reload. Compare Synergy gains at x1/x2; Wrath/Ultimate remain unverified. Check horse, RT/RB, LB+Y, LB wheel, R3 and cold boot/cursor.
Rollback: reinstall preserved V1.11 ZIP with backup of personal INI.

## Cumulative TODO / next step
- User validation of V1.12 layout and non-regression.
- Obtain exact supported EXE and fresh gain/spend log; trace actual Wrath/Chaos writes rather than repeat inactive Add hooks.
- Identify warning presentation/duration authority; preserve initialization before implementing skip.
- TPS mouse remains deferred.