# V1.164 — un sol privé sous les sabots

V1.162 : mécanisme maintien/sauts/relâchement apprécié. V1.163 : **REJETÉ comme solution au rendu flottant**. La demande est une marche native sur un sol invisible, pas un mode de vol avec une autre inertie. Arbre source parent debug `d1c192a6c9dfc57566af38108d2c299badcff5a8` ; acquis conservés, main V1.159 intacte.

## Objectif et réussite

Même case, action Jump réaffectable, délai de relâchement et quatre chevaux. À l'acquisition : appui horizontal privé sous la capsule, mode natif **Walking (1)**. Le moteur gère ses propres marche/course, freinage, virages, sprint et animations au sol. Nouvel appui : appui retiré avant le saut natif ; relâchement : appui retiré après le délai puis chute native. Critère EN JEU : cheval qui marche/court réellement dans le vide, et non pose flottante ; deux sauts successifs, chute et sorties propres. **EXPÉRIMENTAL, pas validé en jeu.**

## Remplacement et nettoyage

- Supprimés : mode Flying de cette option, StopMovementImmediately, les deux hooks CalcVelocity/GetMaxSpeed de V1.163, MountedAirWalkGroundMotion.h et les tests de ce chemin rejeté.
- Un seul hook : ComputeFloorDist moteur, appelé normalement une fois. Pour la monture locale sous notre lease Walking, fournir un résultat de sol horizontal si le sol réel n'est pas plus haut/proche. Les autres acteurs et l'option OFF reçoivent le résultat original.
- Aucun acteur/plateforme/asset créé, aucun déplacement forcé de capsule, aucune modification de vitesse, friction, gravité, animation ou volume physique. Ce plan ne participe pas aux collisions des autres acteurs.

Le bas de la capsule donne la hauteur initiale : position mondiale du root validé moins GetScaledCapsuleHalfHeight natif. Le plan garde cette hauteur pendant les déplacements horizontaux. Un sol réel plus haut reste prioritaire et relève le plan pour une sortie naturelle depuis cette hauteur. Écriture limitée au résultat de requête de sol de cette monture ; aucun faux composant/acteur physique. Références faibles vides correctement initialisées. Balayage valide requis ; pas de rattrapage hors portée ni de correction de pénétration profonde.

Le moteur reste en Walking au lieu de simuler cet état sur un cheval volant. Disparition de l'appui avant Falling/jump, perte de focus, pause/roue/cinématique, OFF, changement de monde/forme/identité/propriétaire. Une lease abandonnée ne peut plus fournir de sol ; le solveur natif retrouve ainsi la chute même si une requête explicite n'était plus possible. Menu mod : appui acquis conservé, nouveaux appuis réservés à l'éditeur.

## Preuves et garde-fous

EXE local existant SHA256 9F4702024DF5EEA1D51DF7745B0AD1EA95B97009982F73DDC1218C53DFF33D54. Descripteurs générés de l'EXE : FindFloorResult **148 octets**, HitResult **136**, alignement4 ; six et dix-huit propriétés, masques et offsets contrôlés. ComputeFloorDist : slot cheval +0x870 → 0x173C300, ABI confirmée par K2 wrapper 0x1746DB0/thunk 0x1CF0E70. Hauteur native : thunk 0x1CED210, ReturnValue float4. [Contrat statique extrait](V1_164_NATIVE_FLOOR_CONTRACT.json).

Au démarrage : EXE supporté, entrée/prologue/slot exacts avant le hook. Avant acquisition : réflexion runtime stricte de toutes les propriétés écrites et de leurs masques, méthode/paramètres/package de SetMovementMode et hauteur capsule. Thread jeu, autorité locale, possession réciproque, identités faibles, root = UpdatedComponent et root propriétaire de la monture. À chaque résultat privé : même lease récente, objets vivants, Walking/custom0 ; sinon résultat natif intact. La demande de lecture externe a trouvé le jeu fermé ; aucun nouveau relevé, lancement ou appel externe effectué.

[Documentation Epic ComputeFloorDist](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/Engine/UCharacterMovementComponent/ComputeFloorDist) utilisée uniquement pour son rôle ; ABI/layout du jeu 4.22.2 déduits de son propre EXE, pas d'un SDK UE5. La présence des signatures et les simulations ne prouvent ni la stabilité physique ni les animations en jeu.

Tests préparés : géométrie et vraie surface prioritaire, portée/valeurs invalides, contrat incorrect, Walking refusé, plan disponible dès la requête d'entrée de mode, sauts, délai, OFF/focus/pause/menu, monde/forme/possession/recyclage et isolation autre acteur/P2. Tests de saut au sol existants conservés. Windows GitHub à venir ; pas de compilation locale ni d'installation.

Configuration inchangée (250 valeurs, 55 actions Hotkeys). Paquet : deux binaires seuls à la racine, aucune ancienne archive remplacée. Écharpe et autres fonctions hors périmètre. Rollback : option OFF pour retrouver le cheval natif, ou V1.162 disponible pour le précédent maintien expérimental.
