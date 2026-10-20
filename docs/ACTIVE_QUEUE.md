# File active — 20 octobre 2026

Base cumulative : V1.68, acquis V1.66/V1.67 conservés et retours V1.68 ci-dessous. Les validations portent sur les essais rapportés. Branche debug uniquement. [V1.69 construite et contrôlée, critères](V1_69_NATIVE_VISUAL_FOCUS_TEST.md).

| Sujet | État | Suite |
| --- | --- | --- |
| Sauts, holsters, délais de rangement, durée ultime 0–10, grappin, ressources, visée et dash | Acquis utilisateur conservés | Ne rouvrir que sur un nouveau défaut |
| Choix du pistolet par main | VALIDÉS V1.67 | Conserver les deux modèles et variantes montées |
| Skins bonus et progression | Sélection chevaux/armures et retour natif OFF confirmés V1.68 sans PAK, journal moteur vérifié | Maintien après redémarrage à vérifier |
| Focus au changement de personnage | Semble VALIDÉ V1.68 ; personnage net au chapitre 8, décors trop nets | V1.69 : méthode du niveau et bande nette du personnage à tester |
| Durée ultime infinie | VALIDÉE par l’utilisateur V1.68 | Conserver |
| Visuels Léviathan | Fonctionnels V1.68, trop bas ; attache native racine démontrée | V1.69 : hauteur corrigée à tester, transformations/P2 et retour natif |
| HUD général après refus d’arène verrouillée | Clic sur une arène verrouillée → message de refus → menu transparent → disparition du HUD général ; cause ouverte | Comparer fermeture/opacité du menu, message et pile UI avant/après le refus ; ne pas cibler seulement UI_ArenaHUD |
| Autres éléments HUD manquants | Impression incertaine | Identifier les éléments avant toute intervention |
| OFF complet pistolet | Ouvert : modèle, effets et bras tendu à retirer, projectiles à garder | Séparer la pose d’un bras sans modifier le chemin de tir ; aucun OFF partiel présenté comme terminé |
| Action Recovery | Ancien système retiré, nouvelle interruption native étudiée | Preuve du bénéfice et du nettoyage avant candidat |
| Autres icônes/effets P2 | Reportés explicitement | Ne pas intervenir ; exception Léviathan nouvellement demandée |
| Auto check update | Construit, retour visuel distinct absent | Confirmer avis temporaire et rappel jaune |

Préserver INI, raccourcis, PAK, sauvegardes, main/archive et les anciennes archives. [État courant](CURRENT_STATE.md) · [Carnet cumulatif](TECHNICAL_NOTEBOOK.md). Les anciens états sont conservés dans l’historique et Git.
