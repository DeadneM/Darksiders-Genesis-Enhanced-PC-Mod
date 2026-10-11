# V0.90 research | Strife cyan aiming line and semicircle: user-captured pixel shaders

Date: 2026-10-11. **Research only. No new ASI binary.** Based on user-supplied `ShaderToggler.ini` (19 lines). This branch forks the already compiled/archived **V0.89** and does not change its gameplay, input, horse regions, controller, HUD or R3 grenade code.

## Direct evidence from the supplied ShaderToggler.ini

- Group count 1; group name `Default`; startup active **False**; raw toggle key `335544320` (preserved, not interpreted).
- Vertex hashes: none. Compute hashes: none.
- Pixel shader 0: decimal `1212955419`, hexadecimal `0x484C3B1B`.
- Pixel shader 1: decimal `2294978555`, hexadecimal `0x88CA97FB`.
- Original configuration mirrored faithfully under `docs/shaders/ShaderToggler.ini`; not automatically included in the V0.89 ASI release.

Upstream `FransBouma/ShaderToggler` computes each hash via CRC32 of the **full shader bytecode** (`shader_desc.code`, `code_size`), polynomial `0xEDB88320`, standard reflected initialization/final inversion. That is **not** a texture/material identifier, a game RVA or an arbitrary shader-object pointer. Original upstream source:
- https://github.com/FransBouma/ShaderToggler/blob/main/src/Main.cpp (calculateShaderHash)
- https://github.com/FransBouma/ShaderToggler/blob/main/src/crc32_hash.hpp
- https://github.com/FransBouma/ShaderToggler/blob/main/README.md

## Target hypothesis and historical evidence

The user has been seeking removal of both (a) the cyan straight line emitted around Strife's dual pistols and (b) the cyan semicircular world-space aim arc. Neither is the screen cursor/reticle nor ordinary Hide HUD. Prior `HideGroundTargetingEffect` and native targeting-UI dispatch candidates logged **zero calls**; V0.80 broad beam suppression was rejected. The newly supplied pixel-shader group is a strong *target candidate* but there is **no supplied side-by-side ON/OFF game screenshot or separate per-hash proof** that these two hashes uniquely render the line/arc. Do **not** describe the issue as fixed yet.

## Isolation protocol (no new ASI hook before confirmation)

1. Keep **V0.89** installed and test independently of Horse Everywhere/Vanilla Aim. Make a backup of the original `ShaderToggler.ini` and do not overwrite the mod's own `dxgi.dll`.
2. Use the existing ReShade add-on setup that produced the file; use the archived combined profile unchanged to confirm that toggling its single group alters the cyan line/arc and **does not hide unrelated objects, projectiles, HUD or hit effects**.
3. Then test separately with only `ShaderHash0=1212955419`, `AmountHashes=1`; restore and test only `ShaderHash0=2294978555`, `AmountHashes=1`. Record which visual disappears per hash; test on-foot and mounted, aiming and not aiming, Strife and War, TPS on/off and both aim modes. Watch for masked scene/UI elements.
4. The file's `IsActiveAtStartup=False` means the group starts inactive: activate its assigned toggle to conduct the test. Do not assume startup suppression.
5. Report a screenshot and in-game result. At that point consider an **optional**, default-OFF native D3D11 draw-suppression path isolated from gameplay: hash actual `CreatePixelShader` bytecode with the same CRC32, map COM shader instances to hash with lifetime-safe management, monitor pixel-shader bindings **per device context**, gate relevant Draw/DrawIndexed/instanced/indirect calls only when target hashes are active, and count suppressed draws. Avoid a global VS/HUD suppression or disabling player targeting/game projectile logic. Test with ReShade absent as well.

Potential DXGI collision: ReShade often uses the same `dxgi.dll` filename as our existing DXGI proxy; placing a second file with that name would overwrite/break the mod. Any coexistence or chain-loading must be explicitly tested before distribution. The captured hash numbers alone do not make the ASI able to hide shaders.

## Immutable gameplay baseline and next acceptance criteria

- V0.89 source/binary intact. No promotion to main, no official release change.
- Frozen: mounted TPS **Vanilla Aim** (user validated), R3 grenade hold+100ms gap+200ms second pulse, controller bindings/strafe/rotation behavior, horse stats, FPS/FOV/camera controls, HUD/reticle, intro/logo, graphics adapter and log overwrite.
- Open issues independent of this shader work: `Horse Everywhere` restriction bypass in summon-denied and horse-banned zones, Camera Forward projectile **raycast** alignment, keyboard/mouse 360-degree camera yaw.
- Shader success requires an **observable line + arc disappearance**, no game rendering regressions, clean ON/OFF restoration, and user confirmation. Compilation, log or valid hash alone does not prove visual success.
