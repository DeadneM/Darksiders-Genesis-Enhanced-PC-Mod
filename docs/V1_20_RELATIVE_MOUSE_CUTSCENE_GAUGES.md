# V1.20 - Relative TPS mouse input, cutscene transitions, and native resource gauge research

Date 2026-10-13. Branch debug only. The user tested V1.19 and said TPS mouse was **very promising**, but camera turns stop when the cursor reaches the screen edge. They asked to push GitHub and advance camera TPS, gauges, and investigate game cutscenes. Preserve V1.19 code and all defaults, especially ThirdPerson=1 / ShowTPSMousePointer=1. Reject all V1.16/V1.17 mouse/cursor patches and avoid new config options or additional mouse fire modes.

## Camera mouse V1.20
- V1.19 fallback uses WM_MOUSEMOVE screen-relative coordinates; it saturates on window edges. Raw relative WM_INPUT works regardless of cursor pixel position. Few WM_INPUT observed in V1.19, and previous audit did not distinguish mouse vs other HID.
- New EnsureRelativeMouse uses GetRegisteredRawInputDevices. If the game already has a registered raw mouse, it is preserved unchanged. **Only if no raw mouse registration exists**, register usagePage=1/usage=2 on existing game HWND, dwFlags=0 foreground only: keeps normal legacy WM_MOUSEMOVE, native WM_INPUT dispatch and mouse wheel. No RIDEV_NOLEGACY, no cursor warp/recenter, no fake controller or new toggle. Existing V1.19 Camera Forward mouse firing/orbit logic unchanged. Registration attempted only once, when TPS enabled and game WndProc installed; on query/registration failure native fallback remains.
- Diagnostics: TPS V1.20 RAW_REGISTER, RELATIVE_INPUT valid mouse raw vs other HID and count of raw packets recorded while cursor is near client border. **User test must confirm infinite rotation**; successful build alone is insufficient.
- Overlay, wheel, Alt-Tab, cinematic/temporary vanilla view gates remain unchanged. Native player input always receives WM_INPUT. Do not silently change pointer visibility or default config values.

## Cutscene research (no playback changes)
- CameraTraceFeature already has a V0.97 auto-suspend heuristic: if the native view is non-gameplay for >=500ms after an active TPS view, relinquish TPS until native game camera is valid. In V1.19 attached log there are multiple CINEMATIC resume/suspend transitions; cannot yet prove actual engine Sequencer events versus false-positive native camera rejections.
- V1.20 adds bounded CINEMATIC_ENTER/EXIT logs, counts of actor-missing and far-view native camera samples, transition summaries on exit via CameraTraceFeature::GetTelemetry. **No change to the cutscene camera, cinematic playback, native MoviePlayer, and no skip-cinematic injection.** User should note real cutscene entry/exit times for comparison.

## Gauge research (no writes)
- Preserve validated Synergy native positive additive multiplier (V1.19 log x100 because user's INI multiplier x100). Native AddWrath and AddChaos hooks still have no observed calls in known logs. Existing Wrath candidate sampled values from 0..20 change during fights.
- V1.20 logs/aggregates candidate Wrath positive vs negative deltas, with context whether pad/LMB fire was held and whether a native outgoing damage callback for the local player recently occurred. The relationship is **correlation only**: not proof a gem, kill, or ability caused the gain. No modified resource object, no gauge writes, no new unknown memory offsets. Ultimate gain function remains unresolved.

## Tests
1. TPS enabled / mouse pointer enabled, physically move mouse to all edges; keep moving and see whether camera rotation continues even when pointer is stationary at border. The logs should show RAW_REGISTER and RELATIVE_INPUT/rawAtScreenEdge >0 when doing this.
2. Test LMB Camera Forward shooting and pad RT/RB aim, input wheel, R3 equipment, horse, Alt-Tab, overlay open, game cutscene. TPS mouse events must not operate in overlay/cutscene/TPS OFF.
3. Note timing of cutscene scenes and send logs CINEMATIC_ENTER/EXIT. Gain Wrath/Ultimate via independent gem collection and enemy kill, check WRATH_CANDIDATE and gauge summaries; avoid claiming multiplied before evidenced.
4. Verify CI Windows x64 and 4 ZIP root files: dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md. Preserve V1.19 ZIP, main/archive untouched. Next build V1.21 with strict numeric increment.
