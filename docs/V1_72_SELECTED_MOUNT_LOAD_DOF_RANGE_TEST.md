# V1.72 — choix de monture au chargement et distance DOF

20 octobre 2026, debug uniquement. Base V1.71 construite/installée, acquis validés préservés. **CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL** : 44/44 suites Windows x64, zéro avertissement MSVC ; nouveau résultat en jeu à vérifier. [Reçu et lien](CURRENT_STATE.md).

## Preuves du défaut

Session V1.71 lancée par l'utilisateur, lectures externes QUERY_LIMITED_INFORMATION | VM_READ seulement. À 19:23:51 : Strife Silver, War Gold et Despair figurent ensemble dans le PlayerState, son getter (pas d'alias), le MayhemSaveGame actif et le fichier disque de la partie en cours. Après rechargement à 19:25 : les deux armures restent dans le nouveau PlayerState ; Despair manque aussi au SaveGame actif. À 19:27:46, le fichier disque contient encore Despair. Retour humain : cheval désélectionné, armure conservée. Il ne s'agit donc pas d'une simple erreur de modèle monté ou d'une absence systématique d'enregistrement.

Le corps natif 6A2460–6A261A parcourt SaveGame+240, entrées de 24 octets, à rebours. Il résout chaque ligne ; si bUnlockedByDlc (+60) est vrai et le DLC (+61) absent de l'ensemble lu via le second paramètre+AD0, il enlève l'entrée. Appel direct 6C3F49, retour 6C3F4E. Les copies 582887 (SaveGame vers profil) et 574FFB (profil vers PlayerState) sont de simples copies ; OnRep 578250 diffuse et nettoie des références faibles, pas ce filtre DLC. Le hook de disponibilité IsSkinUnlocked ne couvre pas ce chemin.

## Candidat limité

Un seul nouveau hook, sur ce filtre, distinct du hook IsSkinUnlocked validé. Exécutable exact exigé, contrôles du prologue, du test DLC interne, de l'appel direct et du setter SetSkin 617A50. Contrôle des classes MayhemSaveGame et MayhemGameInstance, listes bornées à 32 et types uniques. Les valeurs natives des lignes sont MAYHEM_BP=8, RUIN_BP=6.

Avant l'appel, retenir seulement les choix Despair/Rampage exacts (table, package, nom et type) déjà présents. Le filtre original s'exécute entièrement, exactement une fois. Ensuite restaurer seulement les types devenus absents via SetSkin sur cette même liste. Une autre sélection native de ce type reste prioritaire. Listes incohérentes, classes inconnues, mauvais appel, réentrance : aucune restauration. La conservation du choix ne dépend pas de la case de disponibilité : l'utilisateur demande le comportement des armures, dont un choix équipé reste présent même lorsque sa disponibilité bonus est désactivée. Le modèle de base reste sélectionnable ; sa sélection supprime naturellement le bonus de la liste, sans retour imposé. Aucun pointeur conservé après le callback ; aucune recherche par image, écriture de fichier, modification des droits DLC ou des tables partagées. Le travail reste dans l'appel synchrone qui modifie déjà cette liste et utilise le même allocateur natif ; pas de service de personnage ni d'appel retardé.

Le périodique SELECTION_READ_ONLY V1.71 et son en-tête sont retirés maintenant que la cause est localisée. Un message borné SELECTED_BONUS_KEPT confirme une réinsertion, sans certifier le rendu.

## Distance et contrôles

Limite centralisée 5 000 m, mêmes bornes au menu, chargement INI et sortie DOF. Conversion m × 100 vers la région native ; la mise au point du personnage, la géométrie et les garde-fous de distance réelle de caméra restent inchangés. Ne pas relever les garde-fous de pose en confondant région nette choisie et distance caméra/personnage.

Les simulations existantes couvrent maintenant : filtre une fois, deux bonus, conservation armure, retrait d'un DLC étranger, OFF conservant le choix mais pas sa disponibilité, choix déjà admis nativement/PAK sans doublon, autre choix natif prioritaire, types ambigus/corrompus, mauvais appel/classe, réentrance, désactivation pendant l'appel sans perte de sélection, état DLC/tables inchangé. Les contrôles DOF couvrent 1 000 / 1 250,5 / 5 000 m au consommateur et sauvegarde/relecture. Ces simulations ne sont pas une validation du jeu.

## Limites et essais

Reprendre la liste groupée en tête de CURRENT_STATE. Sélection de monture, nouveau maximum de distance et callback de chargement encore non validés dans cette version. Retour sur le flou V1.71 : « ça a l'air ok », à limiter à cet essai. Recovery sans nouveau verdict. OFF complet pistolets non livré ; visée et notifies intacts. HUD général intermittent, autres FX P2 reportés.

Rollback : archive V1.71 préservée, INI personnel à conserver. Aucun lancement/rechargement par l'agent. Aucun asset original extrait n'entre dans Git ou le ZIP. Références privées des captures et désassemblages conservées dans l'audit local.

## Livraison contrôlée

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
