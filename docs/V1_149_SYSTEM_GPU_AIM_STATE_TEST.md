# V1.149 — System/startup, GPU et état du filtre de visée

Base debug `8020e964b940994340d594db35b776eb32a7d0a3`, V1.148. **Correctif du crash sous-titres validé par l'utilisateur** dans la transition de Moloch ; autres changements ci-dessous EXPÉRIMENTAUX en jeu. Compilation exclusivement GitHub, aucune installation par l'agent.

## Objectif et réussite attendue

Case Subtitle crash fix dans **Gameplay → System and startup**, même valeur sauvegardée et même protection validée. Choix GPU conservé et pris en compte après redémarrage. Trait/arc masqué à Moloch lorsque le mode le demande, effets de combat conservés.

## Changements et limites

- Le garde RHI V1.148 conserve son code, ses signatures et son prédicat : log/menu uniquement déplacés. Compteurs réels et validation utilisateur consignés dans [la preuve V1.148](evidence/V1_148_USER_VALIDATION.json). [Note technique pour Nexus](NEXUS_SUBTITLE_CRASH_FIX.md).
- GPU : le proxy chargé par l'import statique dxgi.dll prépare le r.GraphicsAdapter choisi avant la lecture Engine.ini par UE. Même préférence System.GraphicsAdapter0..4, défaut0 ; fichiers existants seulement, vérification de l'écriture et export de confirmation. Suppression de l'écriture ASI tardive ; choix du menu enregistré immédiatement avant de préparer l'INI pour le prochain lancement. Aucune fonction native UE supplémentaire ni changement de sélection implicite. Le résultat réel GPU/INI après fermeture/rechargement reste à tester, pas déduit du test temporaire de fichier.
- Trait/arc : cache du shader invalide avant/après ExecuteCommandList et après ClearState ; état inconnu contrôlé au premier dessin, état Other connu toujours sans requête supplémentaire. Les deux CRC admis sont identiques à V1.148. Le filtre avait bien capturé deux shaders, mais une autre variante dans ce niveau n'est pas exclue. Corriger cette invalidation ne prouve pas encore que le cas Moloch soit résolu. Aucun tir, ciblage, contrôle ou matériau du personnage modifié.

Les deux interceptions D3D11 ajoutées utilisent les entrées58 et110 du [ID3D11DeviceContextVtbl officiel Microsoft](https://github.com/microsoft/win32metadata/blob/main/generation/WinSDK/RecompiledIdlHeaders/um/d3d11.h). Arguments natifs conservés, appel original unique ; aucune modification des listes de commandes. Les shaders de combat non reconnus restent dessinés.

## Contrôles

Nouveaux tests GPU (fichiers temporaires uniquement) : choix0..4, clamp, valeurs étrangères conservées, refus de chemins/fichiers absents, scénario de sauvegarde après lecture initiale. Nouveau test filtre : changement de shader non observé, faux positif évité, modeOFF, chemin rapide des dessins ordinaires. Suites existantes conservées. **Ces tests ne constituent pas un test en jeu.** [Revue du périmètre](evidence/V1_149_SCOPE_REVIEW.json).

246 anciennes clés et 55 identifiants Hotkeys inchangés. ZIP deux fichiers racine ASI/dxgi.dll, sans INI/README. Le nouveau couple de binaires est nécessaire à l'application GPU précoce ; avec un chargeur tiers/ancien, l'ASI indique l'absence de confirmation et ne réécrit pas Engine.ini tardivement au démarrage.

## Essais groupés

1. Retrouver Subtitle crash fix dans Gameplay → System and startup ; vérifier sa valeur conservée et rejouer la transition Moloch avec sous-titres ON pour la non-régression.
2. Choisir un GPU valide déjà testé, fermer complètement puis relancer soi-même le jeu. Vérifier le choix conservé, r.GraphicsAdapter dans Engine.ini et l'adaptateur réellement choisi dans ProjectMayhem.log. Journal attendu : GPU startup V1.149 prepared=N, puis earlyPrepared=N dans l'ASI. Le changement ne peut pas déplacer une session graphique déjà ouverte vers un autre GPU.
3. Dans Moloch, vérifier trait/arc en Hide Everywhere puis TPS Only ; remettre le réglage personnel ensuite. Vérifier aussi que les effets de tirs restent visibles.

## Réparations encore ouvertes

- Pose de tir bloquée : relevés anciens conservés, aucun correctif aveugle. Attendre le prochain signal spontané de l'utilisateur plutôt que demander une reproduction répétée.
- Stoneskin intermittent : matériaux pierre/feu observés avec Silver et Abyssal dans le dernier relevé ; leur présence n'est pas une validation du rendu à l'écran. Aucun nouveau correctif de matériau livré.
- Dash aérien : demande précisée stomp sans direction, dash orienté avec déplacement maintenu. Le dernier relevé ne contient pas de capacité AirDash identifiable ; les actifs/classes natifs sont à analyser avant un correctif. Aucun nouveau hook ou changement des sauts/dashs validés dans V1.149.

Repli : V1.148 reste disponible et immuable. Restituer le réglage personnel de visibilité/GPU si nécessaire, sans modifier INI, sauvegardes ou PAK depuis l'agent. Main/archive restent intacts.
