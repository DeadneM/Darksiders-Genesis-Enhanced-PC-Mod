## Préparation V1.71 — lot cumulé, 20 octobre2026

Demande utilisateur : poursuivre et regrouper les essais. Base V1.70 construite/installée, rendu encore expérimental ; acquis validés conservés. Nouveau candidat **Dash movement recovery** OFF par défaut : reprise par la cause MOVE native après décélération observée et arrêt, sans fin d’ability forcée. Observation des deux listes de skins chevaux, bornée et sans écritures. OFF complet pistolet : slots gauche/droite identifiés, couche de visée partagée ; pas encore livré. **PRÉPARÉ / EXPÉRIMENTAL**, aucune validation de jeu supplémentaire. [Preuves, critère, essais groupés](V1_71_NATIVE_DASH_MOVE_BATCH_TEST.md).

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
| Action Recovery | Ancien système retiré, nouvelle interruption native étudiée | Preuve du bénéfice et du nettoyage avant candidat |
| Autres icônes/effets P2 | Reportés explicitement | Ne pas intervenir ; exception Léviathan nouvellement demandée |
| Auto check update | Construit, retour visuel distinct absent | Confirmer avis temporaire et rappel jaune |

Préserver INI, raccourcis, PAK, sauvegardes, main/archive et les anciennes archives. [État courant](CURRENT_STATE.md) · [Carnet cumulatif](TECHNICAL_NOTEBOOK.md). Les anciens états sont conservés dans l’historique et Git.
