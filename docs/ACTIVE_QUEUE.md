## V1.72 — montures sélectionnées au chargement et distance DOF étendue

Base cumulative V1.71 ; acquis validés conservés. **PRÉPARÉ / EXPÉRIMENTAL**, construction Windows et contrôle du paquet à effectuer. Retour V1.71 : le flou semble bon dans l'essai utilisateur ; le skin cheval reste perdu au rechargement, l'armure reste. Aucun nouveau verdict sur Dash movement recovery.

Objectifs : conserver Despair/Rampage déjà choisis lors du chargement même après désactivation de Bonus mount skins, comme les armures, et permettre une distance nette personnalisée supérieure à 1 000 m dans les deux vues. Limite commune portée à **5 000 m** (curseur, saisie, INI et rendu), valeurs par défaut 10 m/0,6 % inchangées. Aucun nouveau réglage ni raccourci.

Le filtre natif de chargement retirait les lignes marquées DLC avant leur copie au joueur. Le candidat laisse ce filtre fonctionner puis réinsère uniquement les deux choix bonus qu'il vient réellement de supprimer, avec le setter natif de skin. Pas de choix automatique, droit DLC, bonus de gameplay, modification de table ou écriture directe de fichier de sauvegarde. La case agit seulement sur la disponibilité du menu ; OFF ne retire pas un choix bonus déjà enregistré. Le modèle de base reste sélectionnable. Le diagnostic périodique de sélection V1.71 et son appel par image sont retirés : son rôle est terminé. **Nouveau résultat en jeu non validé.**

Critère de réussite : un bonus choisi reste sélectionné et rendu après chargement puis redémarrage, que l'option soit cochée ou décochée ; les choix d'armure, autres règles DLC et sauvegardes restent natifs. Sans choix bonus, aucun bonus n'est ajouté. OFF rend la disponibilité native du menu ; sélectionner le modèle de base remplace durablement le choix bonus. La distance 1 250,5 m puis 5 000 m doit survivre à la sauvegarde/relecture et déplacer la limite nette sans déplacer la caméra ou le personnage.

Essais groupés :

1. Choisir Despair et Rampage, laisser le jeu sauvegarder, recharger puis redémarrer avec Bonus mount skins coché. Vérifier sélection dans la page et monture réellement affichée. Décocher l'option puis recharger : bonus équipé conservé, disponibilité du menu native. Choisir ensuite le modèle de base et recharger : pas de retour bonus imposé.
2. Rendu automatique : arène → hub/mission, chapitre 8, changements de personnage et pied/cheval/ultime. Retour V1.71 encourageant, couverture complète non supposée.
3. Profils DOF Native/TPS : Disable puis Custom, valeurs différentes par vue, 20 / 1 250,5 / 5 000 m, quantité 0 / 0,6 / 2. Saisie, redémarrage, roues/pause, cinématiques et coop.
4. Dash movement recovery V1.71 OFF/ON, Strife troisième/quatrième dash et War, visée/tir en déplacement, native/TPS. Toujours expérimental, OFF par défaut ; rejeter si glissement, aucun bénéfice ou régression de tir/visée.

OFF complet des pistolets reste ouvert : modèles validés conservés ; couches Shoot_Left/Right et couche partagée Spine1 relevées en lecture seule, pas de modification de pose ou de notifies. HUD général après refus d'arène verrouillée intermittent ; autres effets P2 reportés sur demande.

[Audit et preuves](V1_72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.md). Installation : fermer le jeu, sauvegarder les anciens fichiers, conserver l'INI personnel et les raccourcis. Rollback vers V1.71 conservée ; main/archive et anciennes archives intacts. Aucun lancement du jeu par l'agent.

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
