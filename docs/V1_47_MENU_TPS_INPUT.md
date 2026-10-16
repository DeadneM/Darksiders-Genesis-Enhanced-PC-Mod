# V1.47 — menu unifié, changement de personnage et commandes TPS P2 TEST

## Base, objectif et statuts

Base debug V1.46 `ab65e8c39872671ffd2af0e3a98fe7fea1667f35`, source compilée `baf7d3f`, archive832350octets SHA256 `89B4A051142E37213257C2843B4A229BF6D3BA47E524C2CE2CB2B9CF6173A745`. V1.46 CONSTRUITE/CONTRÔLÉE/INSTALLÉE mais pas globalement VALIDÉE : l'utilisateur rejette le maintien de l'angle Strife/War et signale les commandes P2 manquantes, dont le maintien équipement. Les acquis solo V1.43, options validées ensuite, FOV coop et ajouts V1.46 restent la base.

Objectif V1.47 : un même menu de valeurs et de toggles ; garder l'angle choisi lors d'un vrai échange Strife/War ; donner à P2 les commandes TPS configurées avec ses propres états. Réussite utilisateur : même disposition curseur/saisie/Default pour les options, pas de saut d'angle lors des deux sens d'échange, équipement tenu puis rangé par chaque joueur sans agir sur l'autre, recentrage court/appui long/vue temporaire/roues/chord cheval utilisables et configurables sur P2.

Statut de préparation : SOURCE PRÉPARÉE / EXPÉRIMENTAL ; compilation, paquet, installation et résultat visuel encore à contrôler. Aucun lancement de jeu autorisé ni effectué par l'agent.

## Preuves et corrections

Lecture des README/état/carnet/audits V1.45–46, copie du log courant et de l'INI dans le dossier d'audit local. La session V1.46 a réellement émis des corrections PROJECTILE_BASE : premier exemple18:46:12.219, yaw natif141.64 remplacé par camera72.05. Cela prouve le passage dans le calcul de rotation de base, pas l'impact final ou une réparation de tous les tirs de côté.

Capture du16octobre19:05:06–19:08:06, PID30084 : 25 changements d'état, zéro retry de lecture ; LocalPlayer `2281b594a80`, monde `22823d1a060`, PlayerState `2286930c870` constants. Les corps sont réellement remplacés, Strife_Blueprint_C porte CharacterType+0xC18=1, War_Blueprint_C=2. **Erreur V1.46 : War avait été représenté par0 dans le test et le prédicat rejetait la valeur2.** Les tests et le prédicat emploient maintenant les valeurs natives1/2 ;0 et255 restent invalides. Le même type réapparu, un autre monde ou un autre joueur ne récupèrent pas l'angle d'une ancienne identité.

Le ViewTarget conserve l'ancien corps avec PendingViewTarget sur le nouveau pendant environ0,69–0,72s. L'ancien composant peut donc passer dans le fallback solo après publication de la nouvelle génération. V1.46 consommait cette génération et remettait la référence à zéro avant la première caméra propriétaire du nouveau joueur. ReferenceTracker reporte la décision jusqu'à cette caméra prouvée ; il garde alors le référentiel pour Strife/War, et réinitialise pour un respawn du même type. Le fallback reste disponible si la validation native de propriétaires est désactivée. Aucun angle d'acteur n'est écrit, aucune rotation horizontale de tir ajoutée. Un message borné CHARACTER_SWAP_KEEP permettra le contrôle de la prochaine session.

La branche P2 quittait FilterNativeXInput avant la fonction de maintien/stow d'équipement, et UpdateInput ne recevait pas le bouton/délai d'appui long. V1.47 partage la politique d'équipement déjà utilisée par P1 : maintien continu, relâchement natif, intervalle UP100ms puis une pression stow200ms. Chaque joueur possède son état. Le contrôle source reste configurable, boutons ou gâchettes ; la cible native R3 du chemin validé est conservée, sans nouvel événement clavier ni nouvelle fonction native appelée. Le maintien continue avant la porte « vue native temporaire », car les deux fonctions peuvent partager le contrôle. Les axes des roues restent natifs, focus/menu/cinéma et ancienne identité empêchent les états P2 de fuiter.

P2 reçoit aussi la combinaison cheval configurée (deux contrôles simultanés, traduction du chord natif, pré-armement existant, aucun spawn supplémentaire), la vue temporaire et son délai, le recentrage court au relâchement quand il partage l'appui long, et l'appui long TPS. L'option TPS reste partagée par le menu : une demande P2 la bascule pour les deux ; la composition/recentrage/vue temporaire de chaque joueur reste privée. Deux demandes simultanées P1/P2 ne font pas deux bascules dans la même frame. Les détecteurs de tir et de roue, rôles complets des sticks et option native d'interversion restent employés.

Menu : tous les float et les deux délais TPS utilisent le même rendu (nom, curseur, champ exact, Default) ; la largeur réelle de la zone détermine une ou deux lignes. Les états et descriptions des Unlimited reprennent le modèle des multiplicateurs. Munitions0–10, bornes, unités, défauts et callbacks de sauvegarde gardés ; les anciennes lignes redondantes sont retirées. Les callbacks système/startup restent identiques. Aucune clé, valeur par défaut, action Hotkeys, raccourci ou INI livré n'est modifié :119clés et44IDs inchangés.

## Contrôles prévus

Les21 suites existantes sont conservées et étendues : vrais IDs de personnages, transition ancienne caméra sans propriétaire → nouvelle caméra prouvée → échange inverse → respawn ; deux états d'équipement indépendants, limites exactes100/200ms, rebind gâchette/OFF ; recentrage court/appui long/consommation/focus ; vrai module P2 pour équipement avant vue temporaire, refus hors focus, chord cheval configurable et axes des roues. Les tests natifs/gains/munitions/santé/vol plané/potions/updater et parité des119clés restent exécutés. Pas de prétendue validation visuelle à partir d'un test isolé.

## Récapitulatif utilisateur et TODO

1. Menu : munitions comme les multiplicateurs ; tous les champs exacts/Default, y compris FOV, distances, offsets, vitesses et délais ; lisibilité fenêtre redimensionnée, sauvegarde/reload.
2. Caméra solo : tourner, échanger Strife/War plusieurs fois sans entrée caméra, puis refaire en déplacement. Tester aussi respawn/changement de zone séparément.
3. Caméras coop : indépendance P1/P2, réapparition, cheval et deux ultimes, recentrage. FOV coop commun aux deux vues, solo indépendant.
4. Commandes P2 : maintenir/relâcher équipement, rebind du contrôle, vue temporaire et délai, appui court/long partagé, chord cheval modifié, toutes roues et sélection au stick. Vérifier P1 en parallèle.
5. Tirs : premier primaire/secondaire à pied/cheval/Strife ultime, souris puis manette, Camera Forward/Vanilla. Le lien entre angle sauté et balles latérales est plausible mais non démontré ; la correction projectile V1.46 est conservée, pas élargie sans preuve.
6. Scènes : entrée/retour de caméra native ; pause et trois roues gardent TPS. Scènes gardant une caméra attachée encore non démontrées.
7. Options P2 : déplacements/dégâts/gains/munitions/jauges/potions/santé ; refills et Hotkeys. Unlimited santé/vol plané/sprint, chacun ON puis OFF ; les morts de script restent hors garantie, relâchement/sol/reprise sprint natifs.
8. **Visée native après dash NON RÉSOLUE ; Action Recovery INDISPONIBLE.** Prochaine investigation : intention native d'aim et fin réelle de l'action/timers du troisième dash Strife. Aucun StartAiming ou MOVE forcé sans lifecycle prouvé ; coffres différés. Test des coûts en mission, pas dans le Néant où le Courroux est natif gratuit.

## Livraison et rollback

V1.47 prévue `DarksidersGenesis_V1.47_MENU_TPS_INPUT_TEST.zip`, quatre fichiers ASI/dxgi.dll/INI/README à la racine, AMD64. Debug exclusivement ; main/archive/releasev1.43 et110 archives antérieures inchangés. Installation après fermeture réelle, sauvegarde des quatre fichiers et du log, INI personnel courant conservé byte pour byte ; aucun lancement automatique. Retour possible par la sauvegarde V1.46 ou son ZIP immuable, en connaissant les deux corrections rejetées. Les préparations et captures ne constituent pas une validation en jeu de V1.47.
