# Full Technical Audit - V0.16A

Date: 2026-10-06

Branch:

```text
dev/v0.16a-clean-horse-shared-hook
HEAD 093517c1390b966a1b12dfd8aaeae14863b0c462
```

Canonical public/stable main remains:

```text
main
HEAD 0d0efab72c022f43a30019f2993d47ff36f84edf
V0.13B
```

The V0.16A branch is 45 commits ahead of main and 0 commits behind.

Target executable recorded by the project:

```text
DarksidersGenesis-Win64-Shipping.exe
62,113,280 bytes
SHA-256 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54
```

## 1. Build and packaging status

Latest V0.16A CI run:

```text
Run 37468232294
Configure PASS
Build PASS
Package PASS
Upload PASS
```

Artifact:

```text
DarksidersGenesis_ASI_V0.16A_CLEAN_HORSE_SHARED_HOOK_TEST
SHA-256 1ea01763a3186c7f2af28baca5c83338a7c01d2f826adffeb9476bbaa7de9b69
```

Individual packaged files from CI:

```text
dxgi.dll                     1DC82378A19AF939576863659735DE2E2B3D895D5A6595EE76B2826D596CCC06
DarksidersGenesisMod.asi     D71C5B23E44AAC93D5C6DC119C74B75372D16EE80D8AB2A9A68369435FA25DE5
DarksidersGenesisMod.ini     74AF6158E8D91B96DFB033FBC13177DDCA49C3145A23878907324E5D00ED28EB
README.md                    D4EF90B9B6627D240BB933AB1E5212A6707414D7AD59872C3CD6D95E4F417647
```

Packaging is flat and correct.

## 2. Runtime-active architecture

The code that is actually installed by `MainThread` is smaller than the source
file suggests.

Runtime installation order:

```text
Config load
HorseFeature initialize
Skip Intro native CVar resolver
D3D11 Present / ResizeBuffers
HUD native getter hook
UMayhemCharacterMovementComponent::GetMaxSpeed
Action Recovery V0.8B IsActionEnabled
Hotstreak AddJuice
Final outgoing damage filter
```

The DXGI proxy is intentionally minimal. It loads the real System32 DXGI and
then loads local `*.asi` files.

## 3. Feature status matrix

| Feature | Runtime status | Audit classification |
|---|---|---|
| Overlay / Insert menu | Active | Stable |
| Menu key remap | Active | Stable |
| F1 Toggle HUD | Active native getter hook | Validated |
| F2 Movement Speed | Active GetMaxSpeed hook | Validated, but player-identity filter needs correction |
| F3 Action Recovery | Active IsActionEnabled V0.8B | Validated tail-only policy |
| F4 Skip Intro | Active native CVar | Validated |
| Jump Height | Active direct movement properties | Validated |
| Glide / Flight Duration | Active direct movement property | Validated |
| Pistol Damage | Active final outgoing filter, BaseJuice > 0 | Functional / heuristic discriminator |
| Melee Damage | Active final outgoing filter, BaseJuice == 0 path | Functional but over-broad diagnostic discriminator |
| Hotstreak Charge | Active AddJuice hook | Validated |
| Horse Speed | V0.16A candidate | Test only |
| Horse Sprint Duration | V0.16A candidate | Test only |
| Horse Sprint Speed | No gameplay implementation | Placeholder |
| FOV | No gameplay implementation | Placeholder |
| Third Person | No gameplay implementation | Placeholder |
| Skip Logos | Removed from V0.16A | Paused after rejected V0.15A/B/C |

## 4. HIGH - Local pawn is not the same thing as player character

The current helper:

```cpp
IsLocallyControlledMayhemCharacter()
```

calls the audited `APawn::IsLocallyControlled` virtual.

That is not a reliable player-character discriminator.

A locally controlled horse can also return true.

Current consequences inside `HookCharacterGetMaxSpeed`:

1. HorseFeature now correctly observes the component before this filter.
2. After observation, a locally controlled mount may still enter the player path.
3. `g_localPlayerCharacter` can therefore be overwritten with the mount.
4. `ApplyPlayerMovementTunings` can be attempted on the mount.
5. Movement Speed return scaling can be applied to a locally controlled non-player pawn.
6. Action Recovery uses `g_localPlayerCharacter` to validate ability instigators, so replacing it with a mount can temporarily invalidate the player filter.

This is the most important correctness issue currently present.

Recommended correction:

- separate `IsLocallyControlledPawn` from `IsValidatedPlayerCharacter`;
- only assign `g_localPlayerCharacter` after a player-character structural
  signature is proven;
- only apply Movement Speed / Jump / Glide to that validated player component;
- let HorseFeature remain independent.

## 5. HIGH - Target executable hash is displayed but never validated

The project logs and displays the expected target SHA-256, but the ASI does not
calculate or verify the executable hash at runtime.

There is currently no implementation using:

- BCrypt / SHA-256;
- file-size validation;
- PE timestamp validation;
- executable identity manifest.

Most native hooks are signature-resolved and fail open, which helps, but the mod
also contains direct structure offsets and virtual slots.

Recommendation:

Add a lightweight target validator before installing gameplay hooks.

Minimum:

```text
expected executable size = 62,113,280
expected SHA-256 = 9f470202...3d54
```

Policy:

```text
exact target -> full gameplay hooks
unknown target -> overlay/log only, gameplay hooks disabled
```

This would convert an implicit compatibility assumption into a real fail-closed
safety boundary.

## 6. HIGH/MEDIUM - Shared Config object has cross-thread data races

`g_config` is a normal C++ object containing plain bools/floats/arrays.

It is read from gameplay hook threads and modified from the D3D11/ImGui path.
There is no lock or atomic snapshot around these accesses.

On x64, aligned bool/float reads are usually physically atomic, but the C++
memory model still considers unsynchronized concurrent reads/writes a data race.

Recommendation:

Use one of:

1. atomic runtime values per active feature; or
2. immutable `RuntimeConfig` snapshots swapped atomically after UI edits.

Disk persistence should remain separate from runtime state.

## 7. MEDIUM - Approximately 1,100 lines of historical hook code are compiled but unreachable

The main source is about 4,200 lines.

Several complete experimental hook families remain compiled even though
`MainThread` never installs them.

Definition-only installers:

```text
InstallPistolDamageHook
InstallMeleeDamageHook
InstallActionRecoveryHook
InstallDodgeRecoveryHooks
InstallInputSuppressWindowHooks
InstallCommonActionRecoveryHook
```

Their associated resolvers, detours, globals and telemetry remain too.

Approximate historical/dead regions include:

```text
old DoDamageToActor pistol path
old GetBaseDamage melee path
old movement-gate recovery path
dash recovery experiment
input-suppress experiment
IsInterruptEnabled recovery experiment
```

They have effectively zero runtime cost beyond binary/static-state footprint,
but they create major maintenance risk.

Recommendation:

Delete them from runtime source.

Preserve findings in:

```text
docs/experiments/
docs/DODGE_RECOVERY_AUDIT.md
README/build history
Git history
```

The code should represent only the selected implementation.

## 8. MEDIUM - Main source remains too monolithic

Current runtime source:

```text
src/DarksidersGenesisMod.cpp ~4,200 lines
src/HorseFeature.cpp          ~550 lines
src/dxgi_proxy.cpp            ~129 lines
```

HorseFeature is a good first extraction.

Recommended Q-Protocol-style split:

```text
Core.cpp / Core.h
Config.cpp / Config.h
Overlay.cpp / Overlay.h
NativeResolver.cpp / NativeResolver.h
HudFeature.cpp
PlayerMovementFeature.cpp
ActionRecoveryFeature.cpp
CombatFeature.cpp
HotstreakFeature.cpp
SkipIntroFeature.cpp
HorseFeature.cpp
dxgi_proxy.cpp
```

Each feature should own:

- resolver;
- hook state;
- runtime settings;
- telemetry;
- shutdown/restore.

## 9. MEDIUM - No global shutdown / unhook path

There is no `DLL_PROCESS_DETACH` cleanup and no central shutdown routine.

Missing normal teardown includes:

```text
HorseFeature::Shutdown()
MH_DisableHook(...)
MH_Uninitialize()
WndProc restoration
ImGui backends shutdown
ImGui context destroy
D3D11 reference release
```

The current process-exit behavior may be harmless for this game, but it is not a
clean lifecycle.

Recommendation:

Add an explicit shutdown path. Avoid heavy work directly inside DllMain; signal
a controlled teardown routine instead.

## 10. MEDIUM - INI persistence happens synchronously on every UI value change

`DrawTunableFeature` calls `g_config.Save()` whenever a slider/InputFloat
changes.

`Config::Save()` rewrites every feature, every value and all twelve hotkeys,
then logs `INI saved`.

Dragging a slider can therefore generate many synchronous filesystem writes and
log lines in a short period.

This behavior was visible in previous runtime logs.

Recommendation:

- mark config dirty on UI change;
- debounce persistence, e.g. save after interaction ends or after a short quiet
  period;
- explicit Save button remains available;
- do not log every intermediate slider frame.

## 11. MEDIUM - Horse V0.16A diagnostic has an eight-candidate visibility cap

HorseFeature logs at most eight unique movement-component fingerprints.

This prevents log storms, which is good, but if eight unrelated components are
seen before the horse then a failed horse match can become invisible in the log.

Recommendation for diagnostic builds:

- keep runtime writes strict;
- use a ring buffer or reset candidate snapshots when the overlay requests a new
  horse diagnostic session;
- alternatively log only candidates near the horse movement signature.

## 12. MEDIUM/LOW - Horse restore uses cached object pointers

HorseFeature captures owner/movement pointers and restores original values after
a timeout.

`WriteFloat` checks that the target memory is currently writable before
writing, which is safer than a blind write.

However, if an Unreal object is destroyed and memory is later reused for another
writable object at the same address, the restore check cannot detect semantic
pointer reuse.

Recommendation:

Before restore, revalidate at least:

```text
movement CharacterOwner back-pointer == captured owner
current movement values remain compatible with the captured horse state
```

Then restore.

## 13. MEDIUM - Melee Damage discriminator is broader than its UI label

Current final outgoing damage logic classifies:

```text
BaseJuice > 0  -> pistol-like
BaseJuice <= 0 -> melee diagnostic
```

The zero-juice path can include non-melee player abilities.

The source itself correctly calls this diagnostic, but the UI simply says
`Melee Damage`.

Recommendation:

Either:

- keep the control but label it clearly as experimental; or
- tighten the discriminator using proven ScaleType/tags/ability data before
  considering it final.

## 14. LOW/MEDIUM - Placeholder features are enabled in the INI

The default INI contains:

```ini
ThirdPerson=1
HorseSprintSpeed=1
FOV=1
```

but these have no gameplay implementation.

F5 Third Person intentionally resolves to `[hook pending]`.

This is not dangerous but makes the configuration look more complete than the
runtime really is.

Recommendation:

Represent unavailable features as disabled/locked in the overlay until their
hook is installed, or add explicit `Implemented=false` UI state.

## 15. LOW/MEDIUM - README is now a technical notebook rather than a concise front page

README is approximately 74 KB and the V0.16A branch adds more than 800 README
lines relative to main.

The historical detail is valuable, but it obscures installation and current
status.

Recommended documentation split:

```text
README.md
  current release
  installation
  current validated features
  controls
  compatibility
  links

docs/BUILD_HISTORY.md
docs/TECHNICAL_NOTEBOOK.md
docs/REFERENCE_PAK_AUDIT.md
docs/experiments/...
```

Do not delete history; move it.

## 16. LOW - CI workflow rebuilds for README-only changes

The workflow path filter includes `README.md`.

With:

```yaml
concurrency:
  cancel-in-progress: true
```

a sequence of source/config/docs commits repeatedly cancels earlier builds.

This happened during V0.16A work.

Recommendation:

- do not trigger binary CI for README/docs-only changes;
- batch source changes before pushing when practical;
- optionally add a separate lightweight documentation workflow.

## 17. LOW - Build warnings

Latest CI has no compiler errors.

Warnings observed:

```text
HorseFeature.cpp: WIN32_LEAN_AND_MEAN macro redefinition
MinHook CMake minimum version deprecation
GitHub Actions Node 20 deprecation notices
```

The macro warning is ours and should be removed.

The dependency/action warnings are maintenance items, not current runtime
failures.

## 18. LOW - Dependency reproducibility

MinHook and ImGui are fetched by release tag.

This is acceptable, but commit-SHA pinning would make builds more reproducible
against upstream tag mutation.

Recommendation for release builds:

- pin exact upstream commit SHA;
- record dependency versions in README/release metadata.

## 19. LOW - DXGI proxy compatibility surface

The proxy exports the DXGI entry points required by the current target and has
already proven functional.

It also automatically loads every local `*.asi`.

That is convenient but means third-party ASIs in the game folder are part of the
same process and can conflict with hooks/state.

Recommendation:

Document this behavior. For debugging, always reproduce with only the Enhanced
ASI installed before declaring a core regression.

## 20. Git branch hygiene

Repository currently contains 22 branches.

This is useful for experimental preservation, and main has correctly remained on
V0.13B.

No branch protection is enabled on main.

Recommendation:

- preserve important rejected branches until V1;
- after V1, tag meaningful milestones and archive/delete disposable experiment
  branches;
- consider protecting main once release automation is finalized.

## 21. Recommended order of work

Do not add FOV, Third Person, Skip Logos or Horse Sprint Speed yet.

### Phase A - Core cleanup

1. Fix player identity versus locally controlled pawn.
2. Add exact executable validation.
3. Remove unreachable historical hooks from runtime source.
4. Split Config/Overlay/active features into modules.
5. Replace live mutable `g_config` reads with an atomic runtime snapshot.
6. Debounce INI writes.
7. Add controlled shutdown/restore.

### Phase B - Revalidate existing features

Regression test:

```text
HUD
Movement Speed
Action Recovery
Jump
Glide
Pistol Damage
Melee diagnostic
Hotstreak
Skip Intro
overlay input / key rebind
Alt-Tab / ResizeBuffers
clean game exit
```

### Phase C - Horse

Use the supplied working reference PAK as the source of truth.

Proven horse defaults:

```text
MaxWalkSpeed                      1300
MaxAcceleration                    600
BrakingFrictionFactor                1
GallopSpawnSpeedThreshold           300
StaminaRecoveryPercentageRate        15
StaminaTotalRecoveryPercentageRate   40
StaminaSprintPercentageRate          25
```

The preferred long-term implementation is direct control of the same
MayhemHorseCharacter Blueprint/CDO properties rather than adding more movement
hook families.

### Phase D - New features

Only after the cleaned core is revalidated:

```text
Horse Sprint Speed / RunSpeed
FOV
Third Person camera
Skip Logos
```

## 22. Final audit verdict

The project is not fundamentally broken.

The validated runtime core is relatively small and has survived repeated tests.

The primary problem is **source-code archaeology accumulating around that core**.

The correct next move is consolidation, not another experimental feature.

Recommended merge policy:

```text
main stays V0.13B
V0.16A remains test branch
next branch = core cleanup / architecture pass
merge only after regression validation
```
