## V1.73 — lot montures / DOF et retrait du Recovery rejeté

20 octobre 2026, debug uniquement. Base cumulative V1.72 construite et contrôlée, pas installée ; acquis utilisateur conservés. Nouveau retour V1.71 : **Dash movement recovery ne fonctionne pas dans l'essai utilisateur, candidat REJETÉ et retiré**. La prochaine recherche Recovery commencera par le **ground slam**, après le reste de la file, conformément à la demande. Aucun nouveau candidat Recovery dans cette version.

Lot : correction du filtre qui supprimait Despair/Rampage au chargement, maintien du choix même disponibilité bonus désactivée (comme les armures), distance DOF Native/TPS jusqu'à **5 000 m**, retrait du diagnostic périodique et du candidat Recovery sans effet. Le nouveau filtre de skins et le maximum DOF attendent leur validation en jeu. **PRÉPARÉ / EXPÉRIMENTAL**, compilation et paquet à contrôler.

Le dash revient exactement au code de V1.70 pour la visée, les tirs et leurs effets validés ; seule la phrase périmée du bilan est nettoyée. Politique, état, appels natifs, checkbox, réglage runtime/INI fourni et tests spécifiques du Recovery rejeté supprimés. Les anciens identifiants de raccourci 3 et 54 restent réservés/inertes pour préserver les affectations personnelles ; ils ne sont plus proposés à l'assignation. Une ancienne clé personnelle n'est ni relue ni réécrite par le mod. **154 réglages fournis, 55 identifiants conservés dont les deux retirés**. Aucun INI personnel remplacé.

Essais groupés de cette version :

1. Despair et Rampage : choisir, sauvegarde normale, recharger puis redémarrer ; vérifier sélection et modèle. Décocher Bonus mount skins et recharger : garder le bonus équipé avec disponibilité du menu native. Choisir ensuite le cheval de base : il doit rester de base après chargement.
2. Profils DOF Native/TPS : 20 / 1 250,5 / 5 000 m, quantité 0 / 0,6 / 2 ; saisie et rechargement, arène → mission/hub, chapitre 8 et transformations. Le flou V1.71 semble bon dans le dernier retour, couverture complète non supposée.
3. Régression courte du dash acquis : déplacement + visée + tir + dash avec munition spéciale, native/TPS. Pas de test Recovery demandé : le candidat est retiré.

OFF complet des pistolets reste ouvert ; couches de pose identifiées mais aucun masquage partiel livré comme complet. HUD général après refus d'arène verrouillée intermittent, autres FX P2 reportés. Suite : OFF par main et HUD s'il réapparaît, puis recherche ground slam. [Preuves du filtre de chargement](V1_72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.md).

Archive prévue `DarksidersGenesis_V1.73_MOUNT_DOF_CLEANUP_TEST.zip`, quatre fichiers racine. Anciennes archives, main/archive, INI/raccourcis, PAK et sauvegardes préservés. Jeu fermé par l'utilisateur ; installation après contrôles avec sauvegarde, sans lancement par l'agent.

---

## V1.72 — CONSTRUITE / CONTRÔLÉE, nouveaux comportements EXPÉRIMENTAUX

20 octobre 2026. Base cumulative V1.71, acquis validés préservés. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39468587155) : **44/44 suites, zéro avertissement MSVC ASI/DXGI**. Avis CMake/MinHook et Node des outils distincts. Source `2973184516b1b035d47e80f800b0833af6411d37`, archive `9dc0eef9afdb237fea1953fea2df7c9632fdbbee`. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9dc0eef9afdb237fea1953fea2df7c9632fdbbee/downloads/DarksidersGenesis_V1.72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.zip) : **829605 octets**, SHA256 `A2D87C852B5CBB836B57ECC5EECF3ABCC8FC33B4D59CA829D72BE2A7F78190D2`. HTTP 200/hash, CRC, AMD64 et exactement quatre fichiers racine contrôlés. **155 clés et 55 actions conservées ; 136 anciennes archives inchangées.** Main/archive et v1.43 préservés.

Installation locale prête mais non effectuée : le jeu est encore ouvert. Demande de fermeture en attente ; aucun fichier du jeu remplacé par cette livraison à ce stade. INI, raccourcis, PAK et sauvegardes conservés. Aucun lancement par l'agent. Le README du ZIP décrit la préparation avant CI ; ce reçu complète son statut. **Nouveau résultat en jeu NON VALIDÉ.**

Lot : maintien des deux choix de monture bonus au chargement et distance DOF native/TPS jusqu'à **5 000 m**, sans changer les valeurs par défaut. L'option de skins contrôle seulement leur disponibilité dans le menu : un bonus déjà équipé reste conservé même OFF, comme les armures. Choisir le modèle de base le conserve normalement aux chargements suivants, sans réimposition du bonus. Le filtre natif reste complet pour les autres skins et droits DLC. L'ancien diagnostic périodique V1.71 et son appel par image sont retirés ; un seul hook de chargement ciblé, pas d'empilement de correctifs de personnage.

Preuves : sur la session V1.71, Despair existe dans le joueur, le SaveGame actif et le disque avant rechargement ; après, les deux armures restent et Despair manque au joueur/SaveGame, alors que le disque le contient encore. Filtre natif 6A2460 identifié : retrait des lignes marquées DLC absent. Réinsertion des seuls choix bonus retirés via SetSkin natif, dans cet appel. Classes, appel, signatures, types et listes bornés contrôlés ; pas d'écriture directe de sauvegarde ni de droits DLC. Les simulations vérifient notamment OFF conservant le choix mais pas sa disponibilité, modèle de base/non-bonus non forcé, autre choix natif prioritaire et absence de doublon. [Audit, limites et rollback](V1_72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.md).

Essais groupés :

1. Choisir Despair et Rampage, laisser la sauvegarde normale, recharger puis redémarrer ; vérifier sélection et modèle. Refaire case décochée : bonus équipé conservé, disponibilité du menu native. Choisir ensuite le cheval de base et recharger : il doit rester de base.
2. DOF : Native/TPS, saisie 20 / 1 250,5 / 5 000 m et quantité 0 / 0,6 / 2 ; redémarrage, arène → mission/hub, chapitre 8, angles, pied/cheval/ultime et changements de personnage. Dernier retour V1.71 : flou « ça a l'air ok », limité à cet essai.
3. Dash movement recovery V1.71 OFF/ON, Strife troisième/quatrième dash et War, déplacement/visée/tir, native/TPS ; toujours OFF par défaut, bénéfice sans nouveau verdict. Rejeter en cas de glissement ou régression.

OFF complet des pistolets et HUD général intermittent après refus d'arène verrouillée restent ouverts ; autres effets P2 reportés. La première compilation V1.72 a été annulée pendant la construction après la précision utilisateur sur OFF ; elle n'a publié aucune archive. La CI ci-dessus inclut cette précision. Les états historiques ci-dessous ne remplacent pas ce reçu.

---

## Préparation V1.71 — lot cumulé, 20 octobre2026

Demande utilisateur : poursuivre et regrouper les essais. Base V1.70 construite/installée, rendu encore expérimental ; acquis validés conservés. Nouveau candidat **Dash movement recovery** OFF par défaut : reprise par la cause MOVE native après décélération observée et arrêt, sans fin d’ability forcée. Observation des deux listes de skins chevaux, bornée et sans écritures. OFF complet pistolet : slots gauche/droite identifiés, couche de visée partagée ; pas encore livré. **CONSTRUIT / CONTRÔLÉ / INSTALLÉ**,44/44 suites ; aucune validation de jeu supplémentaire. [Preuves, critère, essais groupés](V1_71_NATIVE_DASH_MOVE_BATCH_TEST.md).

## Reprise V1.70 — rendu par vue, 20 octobre 2026

Base cumulative V1.69 ; Léviathan et menu validés par l’utilisateur. Le flou V1.69 a un nouveau contre-exemple après l’arène : région volume3000 remplacée par environ55 dans la caméra TPS, capture439 relevés sans erreur. V1.70 préserve la région du volume en mode automatique et ajoute deux profils DOF indépendants Native/TPS (Disable, Custom distance/intensité), tous OFF par défaut. **CONSTRUIT / CONTRÔLÉ / INSTALLÉ**,43/43 suites ; rendu à vérifier. [Critères, preuves, essais et rollback](V1_70_PER_VIEW_DOF_TEST.md).

Skins chevaux : **ouverts**, retour modèle natif au chargement avec Bonus mount skins coché, sélection à refaire. Armures semblent conservées. HUD général après refus d’arène verrouillée : ancien défaut confirmé, non reproduit dans l’essai actuel ; aucune correction annoncée. Aucun patch de sélection/save ou HUD aveugle.

# File active — 20 octobre 2026

Base cumulative : V1.68, acquis V1.66/V1.67 conservés et retours V1.68 ci-dessous. Les validations portent sur les essais rapportés. Branche debug uniquement. [V1.69 construite et contrôlée, critères](V1_69_NATIVE_VISUAL_FOCUS_TEST.md).

| Sujet | État | Suite |
| --- | --- | --- |
| Sauts, holsters, délais de rangement, durée ultime 0–10, grappin, ressources, visée et dash | Acquis utilisateur conservés | Ne rouvrir que sur un nouveau défaut |
| Choix du pistolet par main | VALIDÉS V1.67 | Conserver les deux modèles et variantes montées |
| Skins bonus et progression | Sélection/OFF confirmés sans PAK ; armures semblent rester ; chevaux perdus au chargement avec option cochée | Bug ouvert, identifier le point qui retire la sélection ; aucun patch save/DLC |
| Focus au changement de personnage | Changement et netteté validés ; nouveau défaut V1.69 après arène : région nette trop courte | V1.70 préserve la région du niveau + contrôles par vue ; rendu à tester |
| Durée ultime infinie | VALIDÉE par l’utilisateur V1.68 | Conserver |
| Visuels Léviathan | Hauteur V1.69 VALIDÉE dans l’essai utilisateur | Conserver ; ne pas extrapoler tous les états P2 |
| HUD général après refus d’arène verrouillée | Clic sur une arène verrouillée → message de refus → menu transparent → disparition du HUD général ; cause ouverte, non reproduit dans le dernier essai V1.69 | Garder intermittent/ouvert ; aucune intervention aveugle |
| Autres éléments HUD manquants | Impression incertaine | Identifier les éléments avant toute intervention |
| OFF complet pistolet | Ouvert : modèle, effets et bras tendu à retirer, projectiles à garder | Séparer la pose d’un bras sans modifier le chemin de tir ; aucun OFF partiel présenté comme terminé |
| Action Recovery | Ancien système retiré ; nouveau candidat V1.71 de reprise MOVE après arrêt, OFF par défaut | Effet à tester ; aucune garantie de bénéfice ni de couverture coffre/attaque au sol |
| Autres icônes/effets P2 | Reportés explicitement | Ne pas intervenir ; exception Léviathan nouvellement demandée |
| Auto check update | Construit, retour visuel distinct absent | Confirmer avis temporaire et rappel jaune |

Préserver INI, raccourcis, PAK, sauvegardes, main/archive et les anciennes archives. [État courant](CURRENT_STATE.md) · [Carnet cumulatif](TECHNICAL_NOTEBOOK.md). Les anciens états sont conservés dans l’historique et Git.
