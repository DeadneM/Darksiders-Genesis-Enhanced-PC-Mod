# V1.80 — mélange des six pouvoirs : contrats ouverts

22 octobre 2026. Les trois sélecteurs par personnage continuent de réaffecter ses trois capacités natives. Le choix des six n'est pas encore livré et ne doit pas être décrit comme un simple réglage à dégriser. Les retours utilisateur confirment les pouvoirs propres à chacun dans les essais rapportés, hors historique de présentation World Ender maintenant retenu en V1.79.

## Ce qui est démontré

Le point natif 663220 résout une classe puis vérifie déverrouillage et coût. Il appelle le getter Blueprint 7AAC30, limité à la famille du personnage. V1.77 modifie un ID sémantique avant ces contrôles, sans réaffecter les boutons. Les 40 fonctions Blueprint des six capacités ont été entièrement décodées, avec bornes sérialisées et en mémoire exactes.

Nouvelle vérification V1.80 : les montages cités par ces fonctions ont été rapprochés de leurs exports de notifications natifs, depuis le PAK avec SHA1 contrôlé. Les notifications font partie du contrat du pouvoir : une animation visuellement compatible n'est pas suffisante pour en déclencher l'effet et assurer sa fin. Les fichiers privés du jeu ne sont pas publiés.

| Pouvoir | Contrat à conserver avant transfert |
| --- | --- |
| Shadow Clone | Montages avant/arrière Strife ; notification SpawnShadowClone ; ShadowCharacterComponent appartenant au receveur, création et destruction du clone, variantes des cœurs. |
| Caltrops | Montage de pose Strife ; notification DoSpawnCaltropsSpawner ; spawner et upgrades, fin native de l'action. L'effet peut être générique, son déclenchement reste porté par l'animation. |
| World Ender | Montage de visée Strife et StartBeam ; composant MayhemWorldEnder du receveur, origine et direction cohérentes, fin du faisceau. L'absence de ce composant chez War est un contrat manquant, pas une raison de lui donner le composant du joueur Strife. |
| Blade Geyser | Montage de War et déclenchement SpawnBlast ; position au sol, instances de blast et cœurs, contrôles d'annulation et fin. |
| Rampage | Montage de départ puis dash/finish ; déplacement, blasts et régions de dégâts périodiques, sortie et interruptions. Ne pas remplacer par un simple boost de vitesse. |
| Stoneskin | Montages normal/Molten de War ; application et durée des statuts, variantes Molten/Steamroller, notifications et fin. Un montage étranger rejeté peut empêcher le statut de base même si le Courroux est consommé. |

## Approche retenue et limite

Ne pas partager l'acteur, l'arme ou la ressource de l'autre joueur. Ne pas modifier les classes Blueprint/CDO globales, désactiver la vérification des squelettes, appeler une fonction à partir de son seul nom ou consommer le coût sans effet. Chaque transfert doit posséder un adaptateur d'animation/notifications et, si nécessaire, des composants dont le receveur est réellement propriétaire. Cela vaut séparément pour P1 et P2 et pour annulation, changement de personnage et chargement.

L'étape suivante est l'adaptateur des pouvoirs sans composant spécialisé (Caltrops / Blade Geyser / Stoneskin), avant Clone / World Ender / Rampage. Aucun choix étranger inerte ajouté au menu de V1.80, aucune capacité étrangère appelée dans le jeu. Cette étude n'est pas une fonctionnalité validée ; les sources de lookup V1.77 restent inchangées dans le lot apparence/sons.

## Montages vérifiés et notifications

- `Strife_Aim_WorldEnder_Montage` : AnimNotify_CallAbilityFunction (StartBeam:0); AnimNotify_CallAbilityFunction (DestroyBeam:0); AnimNotify_EnableAbilityInterrupt; AnimNotify_PlayParticleEffect; AnimNotify_SoundLoopRange; AnimNotifyState_TimedParticleEffect.
- `Strife_Caltrops_Air_B_Montage` : AnimNotify_CallAbilityFunction (SetFalling:0); AnimNotify_PlayParticleEffect; AnimNotify_PlaySound; AnimNotify_PlaySound.
- `Strife_Caltrops_Air_F_Montage` : AnimNotify_CallAbilityFunction (SetFalling:0); AnimNotify_PlayParticleEffect; AnimNotify_PlaySound; AnimNotify_PlaySound.
- `Strife_Caltrops_Ground_B_Montage` : AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlaySound; AnimNotify_PlaySound; AnimNotify_PlaySurfaceParticleEffect [propriétés non décodées].
- `Strife_Caltrops_Ground_F_Montage` : AnimNotify_PlayParticleEffect; AnimNotify_PlaySound; AnimNotify_PlaySound; AnimNotify_PlaySurfaceParticleEffect [propriétés non décodées].
- `Strife_SpawnClone_Back_Montage` : AnimNotify_PlayFootstep [propriétés non décodées]; AnimNotify_PlayFootstep [propriétés non décodées]; AnimNotify_PlayFootstep [propriétés non décodées]; AnimNotify_PlayFootstep [propriétés non décodées]; AnimNotify_PlaySound.
- `Strife_SpawnClone_F_Montage` : AnimNotify_PlayFootstep [propriétés non décodées]; AnimNotify_PlayFootstep [propriétés non décodées]; AnimNotify_PlayFootstep [propriétés non décodées]; AnimNotify_PlaySound.
- `War_Atk_Rampage_Start_Montage` : notifications portées par les séquences associées.
- `War_Atk_Stoneskin_Molten_Montage` : notifications portées par les séquences associées.
- `War_Atk_Stoneskin_Montage` : notifications portées par les séquences associées.
- `War_Wrath_BladeGeyser_Level1_Montage` : AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlaySound.
- `War_Wrath_BladeGeyser_Level2_Montage` : AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlaySound.
- `War_Wrath_BladeGeyser_Level3_Montage` : AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlayParticleEffect; AnimNotify_PlaySound.
- `War_Wrath_BladeGeyser_Montage` : AnimNotify_PlayParticleEffect; AnimNotify_PlaySound.
