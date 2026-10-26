# V1.113 - Verified debug delivery

**CONSTRUIT / CONTROLE ; EXPERIMENTAL, NON VALIDE EN JEU.** Base V1.112. Latest user validation: Strife Rampage ending, all three Strife powers on War, Stoneskin expiration on Strife. Preserve those. New tests limited to static Geyser landing, Stoneskin grounded gesture/movement and mineral body/native cloth. P2 remains last; opening Rampage charge damage still awaits explicit comparison on an enemy.

Gameplay source `1d3605e414358b6b8747e99b7e3745b1bd5e0e0b`, final compiled source `6c0b9cee531eee27fe54829c6c39fa8370853bf2`, archive `84583fc7f21b497a747733ddb3f1ea923fa3c6c9`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40410419216): **62/62 suites**, zero MSVC warnings/errors. First run 40409718808 compiled successfully, passed 61/62; the new montage test rejected a valid single-entry native sound allocation because its stub retained an old Geyser-only lower bound of 3. Test-only correction accepts positive native counts, fixes no runtime code, and the rerun passes. No ZIP published by the failing run. Runner Node notice is separate from MSVC.

[Immutable ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/84583fc7f21b497a747733ddb3f1ea923fa3c6c9/downloads/DarksidersGenesis_V1.113_GROUNDED_GEYSER_STONESKIN_ROLES_TEST.zip), 2201877 bytes, SHA256 `46AC7B6608C9971BE6777C407DEA733871D8AADA3B8E40F5DF3F85D9358D72A2`. Exactly four root entries, NoCompression method 0, CRC valid, ASI/DXGI AMD64, README and INI matched to the compiled source. ASI SHA256 `6BBBC8E378AB75747EE5CB0B783B222F90B674F790B32E655353EFC1A1513446`. INI 169 keys / 55 Hotkey IDs retained. All 177 old ZIP Git blobs unchanged (178 total); missing local V1.108 quarantine not restored or staged for deletion. main/archive/v1.43 unchanged. Public debug updater selection and download bytes checked, HTTP 200, blob `9201c3d18fd5c23fdba90d51095c8c278fc77e1f`; no in-game updater UI action claimed.

Defender custom ZIP and extracted-payload scans with DisableRemediation: both exit 0, no threats; realtime/on-access/antivirus remain enabled, no exclusions/protection changes. Platform 4.18.26090.9, engine 1.1.26090.9, signatures 1.461.211.0. This is a local diagnostic result, not universal certification.

## Included and retained

Geyser keeps accepted Start and native blast/duration/events, replacing only airborne hold with its authored Aether return and grounded rest. Stoneskin own ground Start/End play at authored speed; native duration/audio and private copies of its native interruption windows remain on the original timeline. MOVE at 0.9422869086 s can interrupt through the function's ordinary path. Native state enum arrays and cached weak targets are private, source untouched. No manual Finish, input binding, velocity or timer.

Body/gear selected by real Strife material slot names, cloth retained native. Body MID emissive base/scroll powers set to zero, suppressing War's misplaced UV glow. Status expiry and restoration remain retained; unsupported material copy is rolled back and cached until observed material changes. This aims for mineral body and native cloth; it does not duplicate War's glowing Molten cloak on Strife. Final visual fit and movement still need the user's test.

Rampage handler/pose sampler, three War power handlers and common retarget/audio/motion routines, camera/XInput/aiming, primary/WingClip/Lava, HeavyLanding and configuration are unchanged. Existing Geyser/Rampage/beam interruption state reuse remains a pre-existing P2 audit item; only the new Stoneskin states are separately instanced here. No runtime coop readiness claim. No local build, installation, game launch or external memory write by the agent.

## User test

1. Geyser stationary then moving, native/TPS: land sooner with the spike ending, same damage and control recovery.
2. Stoneskin stationary then holding movement: short grounded gesture, walking legs instead of sliding, second cast/attack still work.
3. Mineral body/gear, scarf retains its native color/textures, no red skin patch; accepted expiry/armor restoration retained. Base/Molten if available; original War unchanged.
4. When convenient, compare damage during the initial Rampage charge on an enemy. Ending and War transfers already accepted. Coop last.

Rollback: affected slots Native or preserved V1.112, keeping the personal INI. [Implementation, evidence and limits](V1_113_GROUNDED_GEYSER_STONESKIN_ROLES_TEST.md). Next: user feedback on these three points; Caltrops visual-only selector remains queued after corrections.

Evidence: [CI](evidence/V1_113_CI_CHECK.json), [package](evidence/V1_113_PACKAGE_CHECK.json), [updater](evidence/V1_113_UPDATER_CHECK.json), [Defender](evidence/V1_113_DEFENDER_CHECK.json), [source preservation](evidence/V1_113_STATIC_REVIEW.json), [native recovery contracts](evidence/V1_113_NATIVE_RECOVERY_CONTRACTS.json).
