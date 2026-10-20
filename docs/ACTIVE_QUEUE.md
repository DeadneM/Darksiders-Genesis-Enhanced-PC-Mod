# Ajouts actifs — V1.74 en préparation

- **Visée sans tir manette : EN COURS.** Deux entrées, maintien, Native AIM/AIM_RELEASE ; OFF par défaut. Fonction à construire et tester sur base V1.73.
- **Capacités de Courroux : RECHERCHE.** Trois choix indépendants par personnage ; Native ou l'une des six capacités, choix libre confirmé. Dépendances de classes/animations/propriétaire à vérifier avant implémentation.

La file historique ci-dessous est conservée ; OFF pistolet et ground slam restent ouverts, autres FX P2 reportés.

---

# File active — 20 octobre 2026, après le retour V1.73

Branche **debug** uniquement. V1.73 est construite, contrôlée et installée. L'utilisateur répond **« 1. marche / 2. marche / 3. marche / prêt »** à la suite des trois essais V1.73 : conservation du choix des montures au chargement, réglages DOF et contrôle visée/tir/dash. Ces essais sont **VALIDÉS par l'utilisateur** ; aucun cas supplémentaire non décrit n'est supposé testé.

## Travail fonctionnel restant

| Sujet | État et résultat attendu |
| --- | --- |
| OFF indépendant pour chaque pistolet | **OUVERT.** Retirer modèle, effets de cette arme et bras tendu ; conserver projectiles et gameplay. Les choix de modèles gauche/droite et les holsters sont validés. Le nom Shoot_Right ne désigne pas la main : certains montages font tirer les deux armes. Aucun OFF partiel livré. |
| Nouvelle approche Action Recovery | **À REPRENDRE APRÈS LE RESTE.** Commencer par le ground slam, conformément à la décision utilisateur. Le candidat dash sans bénéfice est retiré. Les anciennes demandes dash/coffres restent des pistes ultérieures, sans correctif actif ni résultat annoncé. |

## Reporté ou seulement à confirmer

| Sujet | Décision |
| --- | --- |
| HUD général après refus d'arène verrouillée | **SURVEILLANCE / NON REPRODUIT en V1.73.** L'utilisateur confirme HUD toujours visible. Relevés avant/après complets, sans erreur ; nouvelle racine visible/opacité1, bShowHUD vrai, pile UI vide. Aucun correctif dédié, cause historique inconnue. Reprendre seulement si le défaut revient. |
| Autres icônes au-dessus des personnages et effets de tir plats/mal orientés en P2 | **REPORTÉS expressément par l'utilisateur.** Conserver la correction validée des indications d'interaction ; identifier les autres éléments avant d'intervenir. |
| Autres éléments HUD supposés manquants | Signalement incertain, lié à l'étude HUD ; aucun élément précisément identifié à restaurer. |
| Auto check update | Fonction déjà codée : vérification au démarrage du canal choisi, notification temporaire et rappel jaune. **Retour visuel spécifique encore absent** ; reste un essai, pas une fonction à recoder d'office. |

## Acquis à préserver

Caméra TPS/native, souris/manette et réaffectations ; profils pied/cheval/ultime et coop ; visée/tir/dash ; ressources, munitions/Hotstreak, vie/potions ; grappin, sauts/plané/sprint ; rangement et trois délais ; modèles des pistolets et holsters ; skins bonus et six variantes de progression ; durée ultime/infinie ; Léviathan ; flou et indications d'interaction ; skip warning et mises à jour. Les validations historiques restent bornées aux essais rapportés.

Le nettoyage du code, le retrait des diagnostics devenus inutiles, la cohérence du menu et la documentation accompagnent chaque correction. Ils ne doivent pas réintroduire les candidats rejetés ni modifier les raccourcis personnels.

Préserver INI, raccourcis, PAK, sauvegardes, main/archive et tous les anciens ZIP. Regrouper les changements cohérents ; aucune nouvelle build de diagnostic seule. [Audit OFF/HUD](AUDIT_PISTOL_OFF_HUD_2026_10_20.md) · [État courant](CURRENT_STATE.md) · [Historique intégral de la précédente file](history/ACTIVE_QUEUE_THROUGH_AUDIT_2026_10_20.md).
