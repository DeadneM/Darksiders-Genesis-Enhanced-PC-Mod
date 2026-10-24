# V1.92 — transfert expérimental Caltrops vers War, au sol

**EXPÉRIMENTAL NON TESTÉ EN JEU ; compilation et contrôles V1.92 à consigner après CI.**

Base validée V1.87 `c20319f93413f849a41b10fdfd96f7842a172ac6` dans les cas rapportés ; cumul V1.91 `d974c184aad840b248806598696da57ac2443837`. L’utilisateur autorise explicitement le travail sans essai immédiat. **Aucune validation V1.91 ni V1.92 inventée.** Geyser sur Strife reste rejeté après V1.89 ; son diagnostic V1.91 reste inchangé et en attente de test. Objectif de ce build : proposer un premier effet Caltrops natif appartenant à War, au sol, sans détourner les entrées ni modifier Strife. Réussite de gameplay, différée : apparition et dégâts de Caltrops, un seul lancer par entrée de cast, coût natif en mission, retour aux commandes et Native, sans régression des pouvoirs propres.

## Preuve hors jeu et approche retenue

PAK lu et exporté seulement dans l’audit privé, blocs/SHA1 contrôlés. Blueprint Caltrops entièrement décodé : OnStart choisit une branche de montage au sol ou en l’air ; DoSpawnCaltropsSpawner choisit le projectile de base au sol et utilise Bone_Strife_Pelvis ; OnFinish/ProcessInterrupt conservent les transitions natives de mouvement. Les quatre notifies/modèles étrangers ne peuvent pas être simplement rejoués sur War.

Le thunk natif 7AF460 parse une classe et un FName de huit octets, puis appelle **5AE2B0(ability, projectileSubclass, socketName)**. Ce helper lit le Character de l’ability à +1B0, exige l’autorité et utilise son spawner natif ; sa classe par défaut vient de +1E0. Le chemin 687B30 utilise les sockets du composant du **même propriétaire**, puis sa création de projectile. Pas de cast Strife explicite dans ce chemin de création inspecté. Cela ne prouve pas que toutes les mises à jour/dégâts du projectile accepteront War : c’est un point à tester.

War_Idle, son squelette et le bassin d’index 1 ont été identifiés dans les assets exacts : War_Idle 5,333333 s, War_Mesh_Skeleton, Bone_War_Pelvis parent 0. Les montages Caltrops au sol avant/arrière durent 2,333333 s et utilisent le slot TPS. Réutilisation de la création native de montage dynamique et du cycle play/finish déjà employés ; aucun montage ou asset source modifié.

Le prototype demande l’effet **au début** du montage compatible. Il ne copie pas les notifications dont l’exécution réelle reste à diagnostiquer sur Geyser, et n’ajoute pas une minuterie pour simuler leur date. La pose de War est un repos compatible, pas une animation de lancer retargetée ; les effets/sons spécifiques à l’animation Strife et son mouvement de lancer ne sont pas reproduits. Les Caltrops aériens et leurs trajectoires avant/arrière sont volontairement exclus avant le résolveur/coût. Au sol, avant/arrière utilise le projectile de base natif, orienté par le socket actuel de War.

## Isolation, échec et préservation

- Module ForeignCaltrops séparé ; branchement sur le hook PlayMontage existant, pas de nouvelle interception.
- Choix 3 sur War via les trois clés existantes. Les familles natives, leurs IDs sémantiques, le déblocage de l’emplacement et le calcul de coût du résolveur sont conservés. Valeurs par défaut Native inchangées ; aucun bouton/clavier/XInput spécifique.
- Thread de jeu, acteur local à pied, type War=2 exact, autorité, contrôleur/pawn réciproques, composant de mouvement propre en mode Walking, mesh/animation/squelette/bassin propres. Pas de composant, acteur, projectile ou ressource prêté par l’autre joueur.
- Classe de pouvoir et War_Idle déjà chargés seulement, identités faibles vérifiées ; recherche limitée à 512 objets par observation, intervalle de 15 ms pour la recherche seulement. Aucun chargement forcé d’asset. Une sélection peut rester inactive tant que les assets ne sont pas chargés ; elle ne doit pas consommer le coût avant préparation.
- Classe native du projectile et packages exacts vérifiés ; source sans notifications. Revalidation des identités après création et lecture native du montage, puis du propriétaire avant la demande de spawner. Une destruction ou réaffectation pendant un appel ne doit pas entraîner un appel sur l’ancien propriétaire.
- Factory ou contrat inconnu sur un cast War identifié : Finish normal cause 0 du seul cast, puis arrêt de ce transfert pour la session. Les essais suivants sont bloqués avant le résolveur ; le **premier coût peut déjà avoir été consommé**. Ne pas prétendre rembourser une ressource. Remettre Native ; les autres capacités restent disponibles.
- Geyser/Stoneskin et le chemin natif Strife Caltrops restent distincts. Le nouveau type étranger n’est pas ajouté au classificateur des montages Strife de Stoneskin/Geyser, pour éviter de traiter Caltrops natif comme Stoneskin. Source/chemin de la trace Geyser V1.91 inchangés.
- INI 168 clés et défauts, 55 ID Hotkeys, caméras/visée/tirs, XInput/roues, cheval, bras/holsters, lames, réticule, Léviathan, anciennes archives et main/archive/v1.43 conservés. Aucun fichier de jeu remplacé, aucun jeu lancé.

## Contrôles prévus et essais différés

54 suites Windows prévues : nouvelle suite du helper, ABI de création huit arguments et spawner trois arguments/FName, propriétaire/bassin exacts, durée, réentrance, classes/poses/threads inéligibles, cache borné, arrêt natif et destruction/changement de propriétaire pendant les appels. Suite WrathSlots étendue aux trois emplacements War et persistance de la valeur 3 avec raccourcis personnalisés. Ces contrôles ne remplacent pas le jeu.

Quand le jeu sera disponible, tester en lot :

1. War au sol en mission, un emplacement déjà débloqué sur Caltrops : apparition, dégâts, coût, fin et annulation ; mouvements avant/arrière. Si aucun effet ou verrouillage, revenir à Native et garder le journal, sans répéter l’échec.
2. Les trois emplacements indépendants, sauvegarde des choix/raccourcis puis retour Native. En l’air, le prototype doit rester inactif sans Courroux dépensé.
3. Caltrops propre de Strife, pouvoirs propres de War, puis Stoneskin sur Strife si souhaité. Geyser reste un diagnostic : un appel War Native puis Strife, journal conservé ; aucun résultat réparé supposé.
4. P2 seulement lors d’une session ultérieure : même propriété locale requise, aucune validation coop déduite des tests de code.

Rollback : remettre Native dans le sélecteur, ou restaurer l’ASI V1.87 validé en conservant l’INI personnel. Installation manuelle uniquement, quatre fichiers racine dans le ZIP. Aucune demande de lancement ou de capture maintenant.

## Autres transferts, état réel

Clone vers War : le Blueprint utilise un ShadowCharacterComponent du receveur, sa création/destruction, ses montages avant/arrière et les variantes de cœurs. Il faut une instance appartenant à War, un clone compatible et son nettoyage ; emprunter le composant Strife ou simplement réaffecter un ID ne remplit pas ce contrat.

World Ender vers War : composant MayhemWorldEnder du receveur, StartBeam/StopBeam, montages et sockets de départ/direction. Le correctif de présentation TPS validé de Strife n’ajoute pas ce composant à War. Transfert toujours indisponible, aucun composant injecté sans lifecycle vérifié.

Rampage vers Strife : OnUpdate gère blasts/régions de dégâts, et OnFinish déclenche plusieurs capacités Land/Slam selon les cœurs. Le cast de départ et les fins portent des animations War. Adapter seulement le départ laisserait une chaîne de déplacement/annulation étrangère ; pas de simple multiplicateur de vitesse ni de fin forcée ajouté.

Ces trois sujets restent à développer, pas des fonctionnalités validées ou livrées. Prochaine étape après ce candidat : poursuivre leurs contrats de composants/fin hors jeu ; utiliser les essais différés pour décider quels prototypes conserver. Retirer la trace Geyser après identification de sa panne, pas en masquant son échec.
