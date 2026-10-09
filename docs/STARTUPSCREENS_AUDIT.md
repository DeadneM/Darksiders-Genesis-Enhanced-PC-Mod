# StartupScreens / MoviePlayer: root-cause audit after V0.43

**Status:** investigative notes, no binary change or release.
**Verified supported target:** `DarksidersGenesis-Win64-Shipping.exe`,
62,113,280 bytes, SHA-256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`.
**Safe baseline:** V0.43, `SkipLogos=0`, `SkipIntroVideos` separately configurable.

## Verified in-game observations

- The cross-shaped cursor appeared after loading a saved game with the
  legacy Skip Logos path enabled. The user confirmed that disabling this
  path removes that regression. This is a configuration-level causal A/B
  observation, not identification of a precise missing engine callback.
- Manual F5 / real Alt-Tab could restore the cursor; automatically
  replaying focus transitions in V0.41 was both ineffective and rejected.
- Hide Reticle ON/OFF is validated independently and must not be changed.
- The old V0.19C patch did skip THQ Nordic / Airship Syndicate startup
  logos, while warning + intro cutscene remained with Skip Intro OFF,
  but this was NOT a lifecycle-complete validation.

## Exact existing binary primitive (from repo source, no new disassembly)

Proxy `src/dxgi_proxy.cpp` patches the first FIVE bytes at
`RVA 0x260244` to `E9 0E 00 00 00`, jumping to `RVA 0x260257`.
The full 19-byte native block (repo `kNativeAttachBlock`) is:

```asm
RVA 0x260244: E8 47 89 3A 01       call RVA 0x1608B90
RVA 0x260249: 4C 8B 08             mov r9, [rax]
RVA 0x26024C: 48 8D 55 88          lea rdx, [rbp-0x78]
RVA 0x260250: 48 8B C8             mov rcx, rax
RVA 0x260253: 41 FF 51 20          call qword ptr [r9+0x20]
RVA 0x260257:                     next native instruction
```

By code structure, the first call returns an object (likely
`GetMoviePlayer()`) and the indirect call passes a stack attribute
object via RDX to a virtual method (likely
`IGameMoviePlayer::SetupLoadingScreen`). These C++ identities follow
prior project research and the UE public API; the actual call target,
ABI layout and precise virtual slot must still be confirmed against
the game binary. **The 19-byte call sequence is entirely skipped.**

The proxy applies the patch in `DllMain` before normal game startup.
Its V0.43 legacy-INI guard blocks the unsafe bypass by default; do
not regress this early safety check.

## Rejected alternatives to preserve as lessons

- V0.19A: disable `FStartupScreensModule::StartupModule`
  at `RVA 0x25FE40`: logos disappear, but the independent intro
  cutscene also disappears when Skip Intro is OFF. Rejected.
- V0.19B: zero `StartupMovies` array copied count at
  `RVA 0x25FF31`: logos disappear, intro also disappears despite
  Skip Intro OFF. Rejected.
- V0.19C: bypass MoviePlayer setup at `0x260244`: superficially
  correct logo behavior but confirmed cross-shaped cursor regression.
  Rejected as a default or final implementation.
- V0.41 automatic F5 and V0.40 cursor-source hiding controls:
  removed in V0.42. Do not reinstate.
- General movie-file renames, file-system hooks, Media Foundation
  hooks, arbitrary synthetic input or global startup-module disabling
  are not an acceptable replacement for the mod.

## API-relevant architecture: public UE context (not proof of this game's layout)

The public `FLoadingScreenAttributes` API distinguishes:
- `MoviePaths`: movies under Content/Movies;
- `WidgetLoadingScreen`: standalone or overlaid Slate widget;
- `MinimumLoadingScreenDisplayTime`, playback and auto-completion
  flags, `bMoviesAreSkippable`.

`IGameMoviePlayer::SetupLoadingScreen` accepts those attributes.
`PlayMovie`, `WaitForMovieToFinish`,
`PassLoadingScreenWindowBackToGame` and playback-finished delegates
are part of the normal lifecycle. Unreal has documented
`FDefaultGameMoviePlayer` startup-video keyboard/focus bugs
(UE-37294, fixed upstream long ago), so the lifecycle link is plausible,
but *the Darksiders game's specific missing transition is unproven*.

References (public API, not reverse-engineered local evidence):
- https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/MoviePlayer/FLoadingScreenAttributes
- https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/MoviePlayer/IGameMoviePlayer
- https://issues.unrealengine.com/issue/UE-37294
- https://forums.unrealengine.com/t/how-to-get-viewport-focus-back-from-loading-screen-movie/346728

## Next binary analysis, before implementing a new patch

The game's original executable is **not available among currently
accessible conversation and Project/Library file attachments**; the
repository contains only the patch/mod sources, not the proprietary
game executable. Do not claim to have directly disassembled more
code than the stored instruction bytes.

When a matching legitimately owned EXE is provided:
1. Verify original 62,113,280-byte size and exact SHA-256 above.
2. Disassemble `0x25FE40..0x260300`, recording the ownership and
   lifetime of `SStartupScreens`, `UStartupScreensSettings`, its
   `StartupMovies` TArray, `TimeToShow` entries and the full
   `FLoadingScreenAttributes` stack object at `[rbp-0x78]`.
3. Confirm `0x1608B90` and vtable slot `+0x20` exact semantics
   instead of inferring their names. Trace the gameplay/handoff
   branch at `0x253546..0x2535FD` and playback completion.
4. Locate the specific THQ_LogoBasic / AS_LogoBasic entries and their
   sequence positions. Identify the warning + intro entries and
   distinguish them by source name or validated descriptor.
5. Preferred approach: preserve the full native SetupLoadingScreen /
   MoviePlayer/Slate lifecycle and suppress only the logo media paths
   or per-logo presentation/duration at a narrowly verified point.
   Leave intro and warning untouched. No global empty-movie list.
6. Test crash-free fail-open guard, SkipLogos 0/1, SkipIntro 0/1,
   cursor immediately after load, overlay mouse, F6, F5, and Alt-Tab.

**Release gate:** no code replacement, release, or new ZIP is justified
until callgraph / struct offsets are confirmed, then tested in-game.
