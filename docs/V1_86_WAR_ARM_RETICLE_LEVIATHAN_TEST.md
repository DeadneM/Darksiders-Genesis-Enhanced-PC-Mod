# V1.86 — lot cumulatif de présentation

23 octobre 2026. **EXPÉRIMENTAL, sources préparées ; compilation et contrôles à consigner après CI.**

## Base, objectif et réussite

Base cumulative V1.85 `46aff8644d065f0a25ee31895fa131e762b1bef5`, dont les effets des lames masquées sont validés dans le dernier retour utilisateur. Conserver ces effets, les bras OFF V1.81 et le suivi d'exécution V1.82. Objectif : choix visuel du bras de War, masquage configurable du réticule en jeu, retour du Léviathan sans conflit avec les lames. Réussite en jeu : les deux bras sont sélectionnables et réversibles sans changer l'équipement, le curseur reste utilisable dans toutes les pages/roues, et les éclairs fonctionnent avec les effets de mêlée.

## Frontières vérifiées

Nouveau module PlayerPresentationFeature, deux sorties natives interceptées sur l'EXE exact, signatures locales vérifiées. War : requête d'affichage `7283A0` des seuls DefaultGauntletMesh/TremorGauntletMesh d'un War local normal à pied. Requête native exécutée une fois avant toute exception ; table/ligne de déblocage, booléens inverses, propriétaires et politique des skins parents contrôlés. Rafraîchissement natif `7152C0` sur changement du choix ou de l'acteur, aucun remplissage répété par frame, aucune écriture de déblocage, de CDO, d'asset ou de sauvegarde. P1/P2 possédés ; inconnu ou incompatible reste natif. Le remplacement visuel ne donne pas le Gantelet et n'enlève pas ses fonctions.

Souris : résultat natif `GetMouseCursor` `1ACAA40`, uniquement réticule 8 → None 0. Flèche 1 et None 0 déjà renvoyés par le jeu sont conservés. PC/LocalPlayer/UIManager réciproques, P1, focus, thread, états pause/roue/cinématique et fraîcheur TPS vérifiés. Les deux piles natives `m_pageStack` et `m_pagesToCull` doivent être vides : les transitions de fermeture sont aussi exclues. Relevé VM_READ : réticule 8 en jeu, flèche 1 dans pause/lore/cœurs/carte, et sept échantillons réticule 8 avec pile de fermeture non vide. Aucun hook ShowCursor/SetCursor, déplacement de pointeur, capture RawInput ou touche physique.

Léviathan : preuve VM_READ du 23 octobre sur V1.85 : FX lame READY à 19:04:04.738, signature Léviathan refusée à 19:04:04.905 ; entrée `189A510` déjà détournée. La vérification Léviathan est déplacée avant l'installation de l'interception de lame. Le pointeur natif passe ensuite par cette interception, dont la transformation reste limitée aux notifications exactes de mêlée de Strife. Pas de désactivation des signatures ni d'exception générale. [Contrats résumés sans assets ni adresses de session](evidence/V1_86_PRESENTATION_CONTRACT.json).

## Capacités et défauts ouverts

Le cristal Stoneskin reste incorrect sur War native et Strife ; cause non établie. L'adaptateur existant ne traite que Strife, son test conserve le chemin War, sans écritures de montage/material partagé : cela limite le périmètre du prototype mais ne prouve pas l'origine du défaut. Aucun correctif de cristal dans ce build. La protection transférée reste partiellement testée par l'utilisateur.

Étude poursuivie pour les cinq autres transferts : séquences et notifications de Geyser (`SpawnBlast`), Caltrops (`DoSpawnCaltropsSpawner`, `SetFalling`) et Rampage (`EnableStopOnCollision`) relevées ; le callback natif CallAbilityFunction cherche une fonction sur la capacité active puis la déclenche sans paramètre. Ce chemin seul ne suffit pas : le montage compatible ne contient pas les événements et le suivi de capacité doit survivre aux interruptions. Rampage possède aussi déplacement/root motion et plusieurs phases ; Clone et World Ender dépendent de composants absents du War normal observé. Ne pas emprunter ceux de l'autre joueur, déclencher un effet immédiatement ou exposer un choix inopérant. Les prochains transferts restent ouverts.

## Essais groupés

1. War normal à pied : Native → Basic → Augmented → Native, marche/combat, puis changement de personnage, rechargement et armure alternative. Vérifier l'aspect et que les fonctions du Gantelet restent natives. Si disponible, refaire sur War P2.
2. Masquage du réticule : chacun des quatre modes, TPS puis vue normale ; pause, inventaire/carte, roues, menu du mod et Alt-Tab. Le pointeur doit toujours être disponible dans les menus et revenir correctement après fermeture.
3. Léviathan : OFF puis trois effets, avec et sans lames Strife masquées, changement de personnage et transformation. Contrôle court que les arcs de mêlée validés restent compacts et leurs dégâts inchangés.
4. Facultatif pour le diagnostic encore ouvert : Stoneskin sur War avec slots Native puis Strife ; comparer protection/expiration, et décrire séparément le cristal. Unlimited Health désactivé pour juger les dégâts. Le visuel n'est pas annoncé corrigé.

## Préservation et rollback

168 clés INI, dont deux nouvelles à zéro ; 55 ID Hotkeys et 166 anciennes valeurs conservés. Caméras, tirs, visée, XInput/RT/RB/R3, inversion des sticks, roues, RawInput/Alt-Tab, cheval, bras OFF, sauts, ressources, effets d'épées et prototype Stoneskin inchangés. Aucun fichier du jeu installé, aucune relance ou écriture externe de mémoire ; observations VM_READ uniquement. Les 150 archives antérieures restent immuables, main/archive/v1.43 intacts.

Installation manuelle du ZIP cumulatif à quatre fichiers racine. Garder son INI et ses raccourcis. Désactiver les nouveaux choix avec Native/Vanilla ; rollback ASI V1.85 conservé et même INI, sachant que cette version a la régression Léviathan documentée. Ne pas écraser un ZIP numéroté après publication. Autres effets P2 reportés à la demande ; HUD non reproduit, réception lourde incertaine.
