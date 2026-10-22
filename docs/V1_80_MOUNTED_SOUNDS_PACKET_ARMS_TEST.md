# V1.80 — sons du saut monté et bras OFF cohérents

## Base, objectif et réussite

Canal debug seulement, base V1.79 `5e2221ba8c0d6ed75f743c0cebeb073301b18487`. Le retour utilisateur du 22 octobre valide le saut et les points 1 à 5 dans les essais rapportés, sauf les bras OFF explicitement encore incorrects à droite comme à gauche. La coop demeure différée. Ne pas transformer cette validation en couverture de toutes les montures et périphériques.

Objectif du build : regrouper le saut avec Horse, entendre un départ et un atterrissage par saut réussi sans répétition, et conserver un bras OFF au repos après les actions ordinaires. Réussite = retour utilisateur sur les sons et les deux bras. Nouveaux sons et solve des bras **EXPÉRIMENTAUX** ; construction et contrôles seront consignés séparément dans le reçu.

## Saut et sons

La commande réaffectable Jump et DoJump natif de V1.79 sont conservés. La case passe de Player movement à **Horse**, sans changer `MountedJump=0` fourni ni l'INI personnel. Nouveau `Values/MountedJumpSound=0` : 0 Native jump (varied), 1 Horse snort (varied), 2 Off. Son natif d'atterrissage à 0.9 de volume dans les deux choix sonores ; Off coupe seulement les demandes ajoutées par cette option.

Un saut réussi demande une fois le son de départ ; le retour au sol de ce même saut demande une fois le son d'atterrissage. Pas de son sur un appui refusé en l'air, pas de répétition en maintenant la commande, pas de son de réception différé après perte de focus, menu, pause, cinématique, changement de propriétaire/monture ou désactivation. Les autres chutes restent natives. Le son suit la position du cheval à son déclenchement, avec atténuation et volume Sfx du jeu ; aucune piste externe ou son Windows.

Preuves hors processus : sept assets extraits dans le dossier privé avec SHA1 du PAK vérifiés. `char_ruin_jump` est un SoundCue (1.108 s) avec un mélange de deux tirages aléatoires à trois et deux échantillons. `char_ruin_jump_land` (1.390 s) choisit trois variantes ; `char_ruin_snort_low_vo` (1.079 s) en choisit quatre. Les séquences Strife_Horse_Jump et War_Horse_Jump possèdent déjà les notifications vers char_ruin_jump ; leurs réceptions vers char_ruin_jump_land. Le DoJump utilisé ne lance pas lui-même ces capacités/montages, et l'utilisateur confirme le silence en V1.79.

API vérifiée : execPlaySoundAtLocation `1D16FA0` appelle `18911C0` à `1D17255`, dix arguments, position/rotation par référence, trois floats volume/pitch/start, attenuation/concurrency/owner. Signature de 16 octets vérifiée ; son indisponible sans bloquer le saut si elle diffère. Appels seulement depuis l'observateur synchrone du joueur local sur le thread du jeu. Pas de nouveau hook. Les trois sons sont cherchés dans les objets **déjà chargés**, bornés à 512 objets par observation, classe SoundCue et package exacts, identité index/serial contrôlée avant chaque usage. Aucun chargement de package ou pointeur ancien forcé. Un asset absent produit l'indication Waiting dans Horse ; sa disponibilité dans une session réelle reste à confirmer.

Le saut n'est pas un déblocage de progression documenté : le guide public décrit l'absence de saut monté ([Gamepressure, Horses](https://www.gamepressure.com/darksiders-genesis/horses/z1cde5)). Notre lecture trouve la mécanique et des assets de saut/réception, sans gate de progression dans DoJump. Cela ne prouve pas pourquoi les développeurs ne l'ont pas proposé : fonctionnalité inutilisée, réserve technique ou choix de design restent possibles.

## Bras OFF

L'ancien code combinait les matrices de peau du paquet en cours avec une pose component-space lue séparément à `mesh+5B0+16*selector`. Ces deux lectures ne constituaient pas un contrat d'instant identique. Le candidat supprime cette seconde lecture, son convertisseur FTransform et son accès au buffer natif.

La matrice de peau native est `A_i = inverse(bind_i) * componentPose_i`. Capturer `A_arm_rest * inverse(A_spine_rest)`, puis appliquer `A_spine_current` du **même paquet**, annule correctement l'inverse-bind de l'ancre entre capture et application. Clavicules et branches des deux bras utilisent Spine2 ; os et paquets précédents gardent leur périmètre validé. Les poses et sockets natifs, acteurs, projectiles et trajectoires ne sont pas écrits.

Le cache de repos est conservé pendant cheval/ultime/menus, en matrices copiées seulement, sans déréférencer l'ancien acteur. Il n'est réutilisable que pour la même identité complète du mesh asset (index et serial) et une observation normale valide. Un nouvel asset ou une adresse réutilisée l'invalide. La pose ne s'applique toujours qu'aux actions de pistolets compatibles ; mêlée, pouvoirs et actions inconnues gardent leurs animations. Les bras montés restent natifs. Ce candidat traite un défaut de synchronisation et une invalidation trop large identifiés dans le code ; **il ne prouve pas que tous les mouvements anormaux sont réparés**.

## Essais groupés

1. Dans Horse : saut à l'arrêt, en marche et au sprint. Comparer Native jump, Horse snort et Off, départ et réception ; maintenir Jump, réappuyer en l'air, perdre/reprendre le focus. Aucun doublon ou saut supplémentaire. Vérifier le message Waiting si aucun son.
2. À pied, ranger et rester immobile une seconde une fois. Seul pistolet droit visible, puis seul gauche : viser/tirer, mêlée, dash, saut, retour à la visée. Observer le bras OFF après chaque action.
3. Même contrôle après roue, cheval puis descente, ultime puis retour, sans devoir recapturer un repos à chaque fois. Retour Native réversible ; autre bras, tir, dégâts et sockets conservés.
4. Contrôle court des acquis V1.79 : bons côtés des holsters, quatre choix des épées, World Ender et munitions/faisceaux. Coop ultérieure, pas de nouvelle validation supposée.

Installation manuelle. ZIP cumulatif à quatre fichiers racine ; garder son INI personnel. Nouvelle clé absente = son natif varié. Rollback par ASI V1.79 avec le même INI, nouvelle clé ignorée. Les 144 anciennes archives, main, archive et v1.43 restent protégées. Les six capacités étrangères, le suivi vertical des exécutions, la réception lourde incertaine et les autres FX P2 restent dans la file ; HUD général en surveillance.
