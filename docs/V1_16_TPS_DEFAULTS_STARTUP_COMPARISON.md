# V1.16: TPS defaults + safe historical StartupScreens comparison

Date: 2026-10-13. Branch: debug. Previous numbered V1.15 archive retained. No writing to main/archive.

## User-requested default values (both ASI + shipped INI)

- `[Features] ShowTPSMousePointer=1`, `[Features] ThirdPerson=1`.
- Consistent changes to `config/DarksidersGenesisMod.ini`, `src/ConfigStore.h` member initializers, `src/ConfigStore.cpp::ResetDefaults`, `Store::Load` missing-key fallbacks, and both `runtime::Snapshot` and `runtime::State` initializers in `src/RuntimeSettings.h`. `Store::PublishRuntime()` unchanged, continues propagating loaded choices.
- In-game Camera > Third Person description now reflects ON-by-default native pointer. This is pointer **visibility**, not native mouse camera rotation.
- **Existing user INI explicit values are preserved** by Store::Load and the updater. User-requested *new defaults* must not overwrite an intentionally saved OFF. To get new values with an existing INI, change two checkboxes or manually set both keys to 1; About > Reset INI to Factory Defaults will apply all new factory defaults and preserve a dated backup, but resets ALL other preferences. No ConfigRevision migration required for missing-key fallback; keep current 2139.

## Historical aggressive Skip Logos versus safe implementation (source/notes confirmed)

### Early rejected V0.19 paths
- V0.19A: bypass FStartupScreensModule::StartupModule RVA `0x25FE40`; logos disappear but **intro cinematic also disappears** even with Skip Intro OFF. Not acceptable.
- V0.19B: zero StartupMovies array copied count around `0x25FF31`; logos disappear but independent intro cinematic also disappears. Not acceptable.
- V0.19C: in DXGI DllMain patch five bytes at `RVA 0x260244` with jump `E9 0E 00 00 00` to `0x260257`. Skips the original 19-byte MoviePlayer attachment block: call `0x1608B90`, dereference player object, invoke virtual `[r9+0x20]` with FLoadingScreenAttributes at `[rbp-0x78]`. User found logos omitted, later reproducible cross-shaped OS cursor after loading save. F5/Alt-Tab could restore it. **Correlation proven; exact missing callback not proven.** No evidence this implementation safely isolated both warning cards.
- V0.44 added an "observer" mid-function 19-byte trampoline at `0x260244`; user observed startup crash. Rejected and removed in V0.45.

### Current safe V0.52..V1.16
- Exact binary executable guard, targeted `0x25FF31` seven-byte startup copy interception. Only the exact THQ_LogoBasic and AS_LogoBasic path entries are substituted with nonexistent movie names; **nonempty movie list** and native MoviePlayer::SetupLoadingScreen attachment block `0x260244` remain intact. Skip Intro via independent boot CVar.
- V1.05 read-only `StartupScreens` definition audit: 3 entries of stride `0x40` at settings +0x50/+0x58, per-entry call near `0xAC8760`; current record bytes `[+0x38]` / `[+0x39]` are logged **without modifications**. User ordering: two logos, two black-background warnings (save/controller), intro video.
- Earlier V0.54/55 -> V1.02 `InstallSkipWarningLoopHook` attempted a signature-guarded loop gate at `RVA 0x25FEBF`. It could bypass an initialization-loop span but does **not prove actual warning presentation was skipped**. It was quarantined in V1.04 after reticle/cursor problems: the runtime setter always returns false, safe `SkipWarning=0` shipped/saved, early hook no longer installed.
- **V1.16 does not reinstate the aggressive patch or modify startup code.** Direction: identify actual warning widget/presentation dispatcher after normal three-record setup, and selectively mark or suppress only save/controller warning **draw/display** at a verified phase. Preserve MoviePlayer/Slate startup, pointer handoff, real intro, original native setup and full game initialization. First instrument read-only; fail open when uncertain. Do not guess warning identity from 3 records alone.

### Native TPS mouse camera (historical evidence)
- V0.97/V0.98: RAW `WM_INPUT` relative deltas with fallback `WM_MOUSEMOVE` provided camera orbit, no `SetCursorPos`; user previously confirmed orbit works without firing and physical controller/native gear wheel is a frozen baseline.
- Mouse fired-projectile direction, orbit during LMB, and center-aim were **not reliably validated**; V1.00 mouse-forward attempt was explicitly rejected. V1.02 controller-only redesign removed RAW orbit and virtual pad, retaining native mouse passthrough (see V1.03 historical audit).
- User now asks whether to reintroduce mouse-driven camera: YES as a **separate opt-in TPS Mouse Camera option OFF by default in V1.17 or later**, starting from V0.98 idle RAW orbit behavior. Do not confuse `ShowTPSMousePointer=1` (visible cursor) with rotating TPS camera. Gate by foreground TPS gameplay / overlay / native wheel / cinematic and preserve gamepad and firing native behavior; no unconditional global WndProc input modification, cursor warps or retrograde XInput hacks. User test idle orbit before changing fire direction.

## V1.16 scope / QA
Only changes new defaults + user-facing text + versioning/docs. No native hook changes; skip-warning remains disabled. Verify CI Windows x64, exactly four root ZIP files `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`. Test fresh install/After reset defaults TPS=1/pointer=1 and with existing INI explicit zero both remain zero. Test logo/warnings/intro, reticle UI, ALT-TAB, native wheel, controller/mouse without regressions, updater main/debug choices and four-file install after game exits.
