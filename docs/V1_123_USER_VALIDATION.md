# V1.123 - User validation and remaining Caltrops end hop

## Accepted baseline

User reports test1 OK (Stoneskin on Strife), test3 OK (ordinary Stoneskin on War), and test2 everything good except the small hop at the end of War Caltrops. The current Strife explosion/ash, gesture/material/status behavior and native War behavior are VALIDATED by the user in the tested scenarios. Caltrops stomp and burst are retained; only terminal hop remains OPEN. P2 is still deferred. Geyser and all accepted abilities remain frozen. This feedback does not certify every form/skin/cooperative edge case.

## Video and log review

Attached local video ProjectMayhem   2026-10-27 22-56-49.mp4 (13.85s,2560x1440) decoded read-only into private contact sheets. The5.4-6.8s sequence shows the accepted stomp/burst, then a sharp body/leg lift around5.5-5.7s and crouched recovery around5.8-6.0s, returning to standing. Camera/visible pose cannot prove whether the collision capsule itself leaves the ground. No inference of movement modes from pixels alone.

V1.123 logs record one STOMP native-spawner request for each logged cast, including22:56:52.202 ->52.948 and22:56:56.973 ->57.722. Stoneskin native explosion/complete-expiration entries are present. User visual validation complements these request logs; counts alone do not prove rendering or damage. [Evidence](evidence/V1_123_USER_VALIDATION.json).

Existing complete Blueprint decode already proves Caltrops OnStart requests flying5 after PlayMontage, OnFinish requests falling3 if locally controlled and still flying, and ProcessInterrupt calls SetFalling (falling3 plus0.7*InitialVelocity). The final hop persists across several authored gestures, including the now-accepted full native War Stoneskin stomp. Therefore another blind duration/return-pose edit is not justified. The native flight/fall transition is a strong candidate, still requiring current live position/mode/pose timing to distinguish it from animation-only blending.

## Next step and boundaries

Prepared private read-only capture of local War root/mesh transforms, component-space pelvis, reflected movement velocity, movement mode/custom byte and active ability/helper montage. No writes, injection or native calls; no game launch. Wait for user to relaunch War and confirm ready, preflight before two Caltrops activations. This capture is evidence gathering, not a delivered fix. No V1.124, ASI patch or new ZIP yet.

Previous V1.118 VM-cursor/movement guard remains rejected/excluded; user selected animation work rather than expressly authorizing that method. The Stoneskin particle-API authorization is separate and is not extended to movement manipulation. Do not revive the rejected guard or alter accepted Stone/Geyser/other abilities on the basis of this video alone. If a different native movement correction is proposed, make its measured scope reviewable first and follow approval results.
