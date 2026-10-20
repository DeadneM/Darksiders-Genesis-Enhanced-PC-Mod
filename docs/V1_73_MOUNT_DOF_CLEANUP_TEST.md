## V1.73 — lot montures / DOF et retrait du Recovery rejeté

20 octobre 2026, debug uniquement. Base cumulative V1.72 construite et contrôlée, pas installée ; acquis utilisateur conservés. Nouveau retour V1.71 : **Dash movement recovery ne fonctionne pas dans l'essai utilisateur, candidat REJETÉ et retiré**. La prochaine recherche Recovery commencera par le **ground slam**, après le reste de la file, conformément à la demande. Aucun nouveau candidat Recovery dans cette version.

Lot : correction du filtre qui supprimait Despair/Rampage au chargement, maintien du choix même disponibilité bonus désactivée (comme les armures), distance DOF Native/TPS jusqu'à **5 000 m**, retrait du diagnostic périodique et du candidat Recovery sans effet. Le nouveau filtre de skins et le maximum DOF attendent leur validation en jeu. **CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL**, 43/43 suites, zéro avertissement MSVC ; nouveau résultat en jeu à vérifier.

Le dash revient exactement au code de V1.70 pour la visée, les tirs et leurs effets validés ; seule la phrase périmée du bilan est nettoyée. Politique, état, appels natifs, checkbox, réglage runtime/INI fourni et tests spécifiques du Recovery rejeté supprimés. Les anciens identifiants de raccourci 3 et 54 restent réservés/inertes pour préserver les affectations personnelles ; ils ne sont plus proposés à l'assignation. Une ancienne clé personnelle n'est ni relue ni réécrite par le mod. **154 réglages fournis, 55 identifiants conservés dont les deux retirés**. Aucun INI personnel remplacé.

Essais groupés de cette version :

1. Despair et Rampage : choisir, sauvegarde normale, recharger puis redémarrer ; vérifier sélection et modèle. Décocher Bonus mount skins et recharger : garder le bonus équipé avec disponibilité du menu native. Choisir ensuite le cheval de base : il doit rester de base après chargement.
2. Profils DOF Native/TPS : 20 / 1 250,5 / 5 000 m, quantité 0 / 0,6 / 2 ; saisie et rechargement, arène → mission/hub, chapitre 8 et transformations. Le flou V1.71 semble bon dans le dernier retour, couverture complète non supposée.
3. Régression courte du dash acquis : déplacement + visée + tir + dash avec munition spéciale, native/TPS. Pas de test Recovery demandé : le candidat est retiré.

OFF complet des pistolets reste ouvert ; couches de pose identifiées mais aucun masquage partiel livré comme complet. HUD général après refus d'arène verrouillée intermittent, autres FX P2 reportés. Suite : OFF par main et HUD s'il réapparaît, puis recherche ground slam. [Preuves du filtre de chargement](V1_72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.md).

Archive prévue `DarksidersGenesis_V1.73_MOUNT_DOF_CLEANUP_TEST.zip`, quatre fichiers racine. Anciennes archives, main/archive, INI/raccourcis, PAK et sauvegardes préservés. Jeu fermé par l'utilisateur ; installation après contrôles avec sauvegarde, sans lancement par l'agent.

---

## Livraison contrôlée

## V1.73 — CONSTRUITE / CONTRÔLÉE / INSTALLÉE, nouveaux comportements EXPÉRIMENTAUX

20 octobre 2026. Base cumulative V1.72 contrôlée, retours V1.71 pris en compte. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39469924007) : **43/43 suites, zéro avertissement MSVC ASI/DXGI**. La suite propre au candidat Recovery supprimé est retirée ; compatibilité testée dans LegacyRecovery. Avis CMake/MinHook et Node des outils distincts. Source `65f43520c99eda56cf0b63acbb37b7e4d1e83050`, archive `516db3d8cd898e9d544c571a2ba73fdfa38d070b`. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/516db3d8cd898e9d544c571a2ba73fdfa38d070b/downloads/DarksidersGenesis_V1.73_MOUNT_DOF_CLEANUP_TEST.zip) : **826993 octets**, SHA256 `1D768FD2F9E178AE969685B23DB91220BEBCDCC71F5BC8DB2FADB98F3A84AF38`. HTTP 200/hash, CRC, AMD64 et quatre fichiers racine contrôlés. **154 réglages fournis, 55 identifiants de raccourci conservés, 137 anciennes archives inchangées** ; main/archive et v1.43 préservés.

Installation locale contrôlée après fermeture du jeu par l'utilisateur et sauvegarde : ASI/DXGI/README vérifiés, INI personnel et raccourcis conservés, aucun PAK ni sauvegarde modifiés. Aucun lancement par l'agent. Le README du ZIP décrit la préparation avant CI ; ce reçu complète son statut. **Nouveau résultat en jeu NON VALIDÉ.**

Lot : maintien des montures bonus déjà choisies pendant le chargement, y compris disponibilité désactivée, comme les armures ; distance DOF native/TPS jusqu'à **5 000 m** ; retrait du diagnostic périodique des skins et du Recovery dash sans effet. Le modèle de cheval de base reste sélectionnable et son choix n'est jamais remplacé automatiquement. Le filtre natif reste complet pour les autres skins/DLC. Les valeurs par défaut et toutes les fonctions validées restent conservées.

Retour utilisateur : Recovery V1.71 sans effet dans l'essai, **candidat REJETÉ**. Code, politique, état, appel de permission MOVE, checkbox, réglage runtime et clé de l'INI fourni supprimés. Le module de dash revient au code V1.70, vérifié à l'identique pour le comportement ; seule la phrase périmée du bilan a changé. Les anciens ID 3 et 54 et leurs tokens restent réservés/inertes pour conserver les affectations personnelles, exclus des nouvelles assignations ; aucune lecture/écriture de l'ancienne clé de fonctionnalité. La prochaine recherche Recovery commencera par le **ground slam**, après le reste de la file, comme demandé.

Preuves du défaut de monture : Despair existe dans le joueur, le SaveGame actif et le disque avant chargement ; après, seules les armures restent au joueur/SaveGame alors que le disque contient encore Despair. Filtre natif 6A2460 de retrait des lignes DLC identifié. Le candidat conserve seulement les deux choix bonus réellement retirés, via SetSkin sur cette liste. Classes, appel, signatures, types et tailles bornés contrôlés. Aucun choix imposé, droit DLC modifié, patch global de table ou écriture directe de fichier. [Audit natif](V1_72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.md).

Essais groupés :

1. Choisir Despair et Rampage, sauvegarde normale, recharger puis redémarrer ; vérifier sélection ET modèle. Décocher Bonus mount skins et recharger : bonus équipé conservé avec disponibilité du menu native. Choisir le cheval de base et recharger : il reste de base.
2. DOF Native/TPS : 20 / 1 250,5 / 5 000 m, quantité 0 / 0,6 / 2 ; saisie et rechargement, arène → mission/hub, chapitre 8 et transformations. Le dernier rendu V1.71 semble bon dans l'essai utilisateur ; nouvelles distances à valider.
3. Vérification courte des acquis : déplacement + visée + tir spécial + dash, native/TPS. Pas de nouvel essai Recovery demandé.

Suite : OFF complet par main des pistolets (modèle, effets, bras ; projectiles conservés), HUD général intermittent après refus d'arène verrouillée s'il réapparaît, puis étude du ground slam. Autres FX P2 reportés explicitement. V1.72 est préservée, mais n'a pas été installée : V1.73 intègre le dernier retour avant installation. [Critères et rollback](V1_73_MOUNT_DOF_CLEANUP_TEST.md). Les états historiques suivants ne remplacent pas ce reçu.

---
