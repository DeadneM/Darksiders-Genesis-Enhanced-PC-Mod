# V1.160 — reçu de livraison bêta

**CONSTRUIT / CONTRÔLÉ / PUBLIÉ sur debug ; EXPÉRIMENTAL en jeu pour les changements de ce lot.** Base V1.159 `d6414404ba268add00834323c6da064af5d5ce42`, source `eef048ac69c7181a36f4e260003cc9fc8a4bcd4c`, archive `1e8c3b672e86e4ad95bb1948faf7add96bae047c`. Coop/P2 acceptée par l'utilisateur le 11 novembre ; ce retour concerne la base et ne remplace pas l'essai ciblé du nouveau binaire.

## Changements

- Saut : reconnaissance de Mayhem et Rampage, en plus de Despair et Ruin. Commande Jump native réaffectable et garde de possession/mouvement conservés.
- Visée sans tir : D-pad bas seul par défaut, seconde entrée OFF. Un INI existant avec OFF ou un autre raccourci est conservé ; choisir D-pad bas dans le menu pour changer ce profil, ou réinitialiser uniquement le réglage concerné si disponible.
- Explosion d'activation Stoneskin : jusqu'à trois demandes cosmétiques si la création est refusée, séparées de 150 ms. Abandon après une seconde depuis la première tentative. Pas de répétition après succès ; perte de propriétaire, monde, forme ou statut annule la demande. Pas d'appel pendant pause/roue/cinématique ; recast réentrant isolé.

Cette réparation couvre l'abandon de création constaté dans le code ; elle ne prouve pas qu'il expliquait toutes les absences visuelles signalées. Aucun changement des matériaux pierre/feu, de l'écharpe, de l'aura Molten ou des dégâts. Aucun nouveau hook/API ajouté.

## Contrôles et livraison

GitHub Windows x64 : **82/82 suites réussies**, aucun avertissement C++ détecté ; dépréciations externes d'outillage consignées. [Exécution](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/43034241054). Tests étendus sur refus puis succès, bornes/intervalle, annulation et réentrance ; quatre classes de cheval, possession réciproque, défaut D-pad et OFF personnel.

ZIP : `2431726` octets, uniquement `DarksidersGenesisMod.asi` et `dxgi.dll` à la racine, CRC/AMD64 vérifiés. SHA256 `395731235F8530ACF5FF1A12A931F61CE8F375E4F73BBE48C5393E036EFC38D8`. 224 anciennes archives inchangées, 225 au total ; 248 clés INI et 55 actions Hotkeys conservées. Seule valeur par défaut changée : AimWithoutFirePadButtonA 0 → 2. Main reste V1.159 ; archive/tag intacts.

[Téléchargement immuable V1.160](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/1e8c3b672e86e4ad95bb1948faf7add96bae047c/downloads/DarksidersGenesis_V1.160_MOUNT_JUMP_STONE_RETRY_TEST.zip). Disponibilité HTTP HEAD 200 et version la plus élevée du canal debug vérifiées depuis l'arbre Git et les métadonnées du fichier. Le nouveau ZIP a été récupéré une fois par Git ; aucun ancien paquet ni binaire brut HTTP retéléchargé. Les décompressions et contrôles se font localement. Defender : aucune menace détectée, protections actives et inchangées.

## Test et rollback

1. Saut avec Mayhem et Rampage, puis contrôle rapide Ruin/Despair : un appui, un saut ; aucune répétition à l'atterrissage.
2. Visée sans tir au D-pad bas : nouveau profil/reset, ou réglage manuel de l'entrée 1 sur D-pad bas et entrée 2 OFF dans le profil existant. Garder ses autres touches.
3. Plusieurs Stoneskin sur Strife : explosion d'activation, peau/écharpe, puis cendres de fin. Signaler une explosion manquante avec le niveau et l'armure ; l'acceptation d'un composant dans les logs ne garantit pas son rendu.

Aucune installation ni relance du jeu effectuée. Garder son INI. Revenir au ZIP V1.159 local si nécessaire. [Périmètre et limites](V1_160_REVIEW.md) · [CI](evidence/V1_160_CI_CHECK.json) · [Paquet](evidence/V1_160_PACKAGE_CHECK.json) · [Defender](evidence/V1_160_DEFENDER_CHECK.json).
