# War Caltrops - measured air-state mismatch and proposed public API trial

## Evidence and accepted baseline

V1.123 installed and tested. User validates Strife Stoneskin visuals and native War Stoneskin, and accepts Caltrops stomp/burst except its terminal hop. Geyser and those paths must stay unchanged. Read-only capture PID41568 recorded two casts at23:05:26 and23:05:41,9757 samples/0 errors over120s. Both change mode1 ->5 for~1.83s ->3 for~47ms ->1. Root Z never rises above162.1283; its small downward excursion is~1.3-1.4 engine units. Component-space pelvis reaches~110.4 from idle~94-95. AnimInstance bIsInAir/bIsInAirAndNotSliding are true during the nominally grounded cast, and bIsFalling briefly follows the mode3 transition. This separates an aerial body pose from an actual upward capsule jump. It does not by itself prove an intervention will fix the rendered transition.

## Concrete candidate

Only inside the ASI, after a verified owned foreign War Caltrops starts from native walking1/custom0: one request to the existing public engine API CharacterMovementComponent.SetMovementMode(NewMovementMode=1,NewCustomMode=0). Reuse ProcessEvent already used by the ASI, no new hook. Current exact-executable reflection confirms native thunk0x1CF1600, native flags0x4020400, parameter size2, two ByteProperties at0/1. The engine reconciles its own movement state; never write MovementMode, animation flags, velocity, floor or VM code directly.

Capture weak identities for movement component and its owned UpdatedComponent at cast start, initial root position and world. Require the existing local authoritative foot-War contract, walking/custom0 at start, reciprocal CharacterOwner and UpdatedComponent owner. On the existing game-thread observer, require the same actor/world/index, current active ability and private montage, live actor/mesh/movement/root identities, flying5/custom0 only, current native age0..0.15s, finite near-zero vertical velocity, root Z within1.5 units and horizontal displacement within25 units of start. Verify the exact public function/owner/native entry/flags/parameter layout before dispatch. Mark attempted before calling, never repeat or hold walking. The native engine retains subsequent gravity/floor/ledge handling. If it returns falling rather than walking, do not force a second call. Skip stale, remote, airborne, transformed, paused/menu/cinematic, cancelled/taken-over, custom-mode or materially moved actors. Ordinary Strife Caltrops and every other War power remain native. Cost, damage, sound, stomp timing and montage duration/blends remain V1.123.

## Boundaries and review

This is a new movement-API candidate, separate from the explicitly authorized Stoneskin particle API. It is not assumed covered by that limited visual permission. The earlier V1.118 global thunk hook/VM-cursor mutation remains absent and abandoned; this method neither intercepts that thunk nor consumes or rewrites script instructions. Runtime correction must pass approval review before inclusion. External readers retain QUERY_LIMITED_INFORMATION|VM_READ and never invoke this API, write the game or launch it. No live intervention has occurred.

Native method calls can still crash if their ABI/object contract is wrong, and a locomotion change can alter ledge/interruption behavior. Exact two-byte reflection, weak identity/ownership checks and one early request reduce that scope; they do not certify gameplay. Required tests: exactly one request per qualifying cast, fail-closed thread/owner/layout/state/position/age cases, no raw source/actor/root/velocity mutation by the adapter, native source assets untouched, no new hook or timer, same accepted gesture/spawn. User test remains mandatory before declaring the hop fixed. Coop remains last.

## Automatic review result - explicit authorization pending

Automatic approval review rejected the source patch before application: the public SetMovementMode runtime call and locomotion change are distinct from the user's limited SpawnEmitterAttached permission; the user has not explicitly authorized this potentially crashing engine trial. The proposed patch was not applied, compiled or executed. Source/test/config/control paths remain unchanged. Do not retry through another route or execute the call from the external reader. Human authorization must explicitly name this bounded SetMovementMode trial before inclusion. V1.123 remains installed and its accepted Stoneskin/Geyser behavior is retained.

## User decision superseding the pending question

User explicitly selected: Conserver V1.123 pour l'instant. The proposed SetMovementMode trial is set aside and is NOT authorized. No patch, build, installation or live intervention. Keep V1.123 and all accepted features; terminal Caltrops hop remains open. Do not retry this method or repeat its approval question unless the user explicitly reopens the topic. Earlier review text is retained as history, not an active permission request.
