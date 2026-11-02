# V1.143 — Stoneskin et suivi des anomalies

**PRÉPARATION / CHECK ONLY. Aucun ZIP livré ou installation.** Base V1.142 installée et acceptée hors anomalies suivantes : ralentissement pendant Stoneskin Strife ; effet de cœur/bouclier inadapté en TPS ; pose de tir parfois bloquée, un nouveau tir la rétablit.

## Preuve et optimisation CPU

Journal de session V1.142 du 1er/2 novembre 2026 sauvegardé en référence privée avant remplacement. Dernier rapport cumulatif : gameplay-service 1 668,85 µs de moyenne ; stoneskin-observe 1 025,51 µs (maximum observé 15 040,70 µs), pistolets 89,32 µs, découverte pouvoirs 370,52 µs. Échantillonnage un appel sur 64, durées inclusives, pas de mesure FPS/GPU. Les moyennes cumulatives mélangent états/vues/personnages : elles ne prouvent pas que chaque pic correspond au pouvoir actif.

L'observateur parcourait à chaque appel la hiérarchie des classes de tous les statuts, avec comparaison répétée de noms, et recherchait le champ instigateur même pour des statuts incapables de porter Stoneskin/Molten. Le nouveau cache borné à 128 classes mémorise cette classification immuable, avec identité complète adresse/index/série vérifiée avant chaque réutilisation. Les statuts sans rapport évitent la réflexion de leur instigateur. Les statuts pertinents conservent le contrôle de propriétaire existant.

Une adresse réutilisée invalide le cache ; une hiérarchie tronquée/incomplète n'est pas mémorisée ; une table pleine remplace une entrée au lieu de ne plus apprendre les classes. Aucun délai ajouté, changement de cadence, shader/particule supprimé, écriture dans un statut, nouvel appel moteur ou hook. Apparition, matériaux, skin Abyssal, expiration et restauration du rendu restent dans leur chemin validé. Gain visible non encore mesuré.

Tests ciblés ajoutés : classification réutilisée, changement de série à même adresse, cache plein et cycle de hiérarchie ; fixtures existantes conservées pour les transitions, propriétaires, autres personnages, transformations, ressources tardives et restauration.

## Autres sujets ouverts

- Bouclier : nom du cœur demandé à l'utilisateur. Démon au bouclier et Stinger sont deux effets distincts ; pas de remplacement visuel appliqué sans identification. Le catalogue actuel propose Shield Demon natif/masqué seulement.
- Pose : capture en lecture seule préparée et en attente du signal utilisateur « prêt ». Elle comparera états de combat/visée, capacités et variables d'animation pendant tir, déplacement et Stoneskin. Aucune réparation de touche, réinitialisation forcée du personnage ou correction du moteur sans preuve.

Compilation/contrôles GitHub uniquement, sans publication de ZIP pour cette passe. INI, 228 clés/55 Hotkeys, archives, main/archive et fichiers de jeu conservés. Suite : exploiter le relevé de pose et l'identification du cœur, puis regrouper les corrections retenues dans une build test. Les deux sujets ouverts ne sont pas annoncés corrigés.
