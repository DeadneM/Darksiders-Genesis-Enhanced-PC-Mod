# File active — 20 octobre 2026

Base : V1.66 validée pour les sauts et holsters, plus les choix gauche/droite par main V1.67 validés. Les validations portent sur les essais rapportés. Branche debug uniquement. [Lot V1.68 préparé et critères](V1_68_VISUALS_ULTIMATE_TEST.md).

| Sujet | État | Suite |
| --- | --- | --- |
| Sauts, holsters, délais de rangement, durée ultime 0–10, grappin, ressources, visée et dash | Acquis utilisateur conservés | Ne rouvrir que sur un nouveau défaut |
| Choix du pistolet par main | VALIDÉS V1.67 | Conserver les deux modèles et variantes montées |
| Skins bonus | ÉCHEC V1.67, mauvais IsSkinEquipped identifié et retiré dans la préparation V1.68 | Tester la sélection avec le PAK actuel, puis autonomie ASI séparément ; progression encore non testée |
| Focus au changement de personnage | Défaut signalé dans plusieurs niveaux ; trou de focale capturé ; candidat V1.68 | Comparer transition et netteté générale, lentille ON/OFF et vue native |
| Durée ultime infinie | Préparée V1.68, débit seul et Hotkeys ajouté | Test arrêt/reprise au niveau courant, sans Unlimited Ultimate |
| Visuels Léviathan | Trois templates exacts identifiés ; sélecteur visuel seul préparé V1.68 | Contrôler les trois effets/OFF, retour natif, transformations et P2 ; aucune validation de rendu |
| HUD après arène verrouillée | Reproductible selon l’utilisateur ; pas de cause observée | Relevé avant/après des états natifs et widgets, sans forcer tout visible |
| Autres éléments HUD manquants | Impression incertaine | Identifier les éléments avant toute intervention |
| OFF complet pistolet | Ouvert : modèle, effets et bras tendu à retirer, projectiles à garder | Séparer la pose d’un bras sans modifier le chemin de tir ; aucun OFF partiel présenté comme terminé |
| Action Recovery | Ancien système retiré, nouvelle interruption native étudiée | Preuve du bénéfice et du nettoyage avant candidat |
| Autres icônes/effets P2 | Reportés explicitement | Ne pas intervenir ; exception Léviathan nouvellement demandée |
| Auto check update | Construit, retour visuel distinct absent | Confirmer avis temporaire et rappel jaune |

Préserver INI, raccourcis, PAK, sauvegardes, main/archive et les anciennes archives. [État courant](CURRENT_STATE.md) · [Carnet cumulatif](TECHNICAL_NOTEBOOK.md). Les anciens états sont conservés dans l’historique et Git.
