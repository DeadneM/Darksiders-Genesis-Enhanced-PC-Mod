## V1.75 — lot visée manette et réception lourde, préparation

Base V1.73 validée dans les essais rapportés, visée V1.74 construite/contrôlée non installée. Nouveau candidat **Heavy landing movement recovery**, OFF par défaut : classes HeavyLand exactes, deux verrous natifs de mouvement après impact, sans hook, Finish, timer, touche ou changement de dash. L'utilisateur confirme qu'il s'agit de la réception lourde automatique après chute, pas de l'attaque plongeante. Capture7480échantillons, trois rejets, six capacités natives et trois HeavyLand Strife ; aucun War_HeavyLand relevé. Nouvelle visée et nouveau candidat **PRÉPARÉS / EXPÉRIMENTAUX**, efficacité en jeu non validée. [Objectif, limites, essais et rollback](V1_75_PAD_AIM_HEAVY_LANDING_TEST.md).

Les capacités ne sont pas encore échangeables : les deux lookups ne reconnaissent que leurs propres trois IDs, les montages utilisent deux squelettes et des composants diffèrent. OFF complet par pistolet non livré ; frontière de copie de rendu identifiée sans modification. Ces sujets restent ouverts. [Preuves](V1_75_WRATH_HEAVY_LANDING_PISTOL_RESEARCH.md). HUD en surveillance, autres FX P2 reportés.

Installation **manuelle par l'utilisateur à la fin de sa session**. Aucun nouveau fichier du jeu, INI, PAK ou sauvegarde remplacé par l'agent. Anciennes archives et branches protégées conservées ; ne pas demander de fermer le jeu. CI et paquet encore à vérifier à cette étape.

---

## V1.74 — construite et contrôlée, installation manuelle reportée au prochain lot

Base cumulative V1.73, ses trois essais rapportés validés. Nouvelle visée manette sans tir : **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE**, pas validée en jeu. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39498950576) : **44/44 suites, zéro avertissement MSVC**. Source `61d9d4b01405ceb45ae802c348cce6dcfaf6a053`, archive `53e2b1645f83e2449eea98d01d3f19bc39ab921f`. [ZIP cumulatif vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/53e2b1645f83e2449eea98d01d3f19bc39ab921f/downloads/DarksidersGenesis_V1.74_NATIVE_PAD_AIM_TEST.zip), 831912 octets, SHA256 `578DC4AC279472C28556F7B930AFCB2FB6C4E6D53D84E4E8B91616A7AC29AA6B`. Quatre fichiers racine, CRC, AMD64, 156 clés fournies, 55 ID Hotkeys conservés ; les 154 anciens réglages gardent exactement leurs valeurs. Les 138 anciennes archives, main/archive, tag v1.43 et fichier de reprise préexistant sont inchangés.

Le README du ZIP contient la préparation V1.74 avant CI sous le titre historique de sa base V1.73 ; le présent reçu et la version compilée V1.74 font référence. L'archive publiée reste immuable. Une tentative d'installation s'est arrêtée dès le contrôle GAME_RUNNING, **avant sauvegarde ou remplacement**. L'annonce précédente de fermeture déduite d'une lecture sandbox était erronée : le contrôle final avec les permissions d'installation a vu le processus actif. Aucun fichier du jeu remplacé. L'utilisateur précise ensuite qu'il installera **manuellement à la fin de sa session** et demande de regrouper davantage de travail ; ne plus demander sa fermeture ou installer automatiquement ce lot.

Suite : capacités et réception lourde, puis OFF pistolet selon preuves. Choix demandé : trois emplacements par personnage, Native ou n'importe laquelle des six capacités. Chaque Blueprint GetWrathAbilityById ne reconnaît que ses propres trois ID ; les six montages utilisent deux squelettes distincts. Le remplacement de FName seul serait inopérant. Ne pas livrer six options présentées fonctionnelles avant adaptation et contrôles des animations/composants. [Recherche du prochain lot](V1_75_WRATH_HEAVY_LANDING_PISTOL_RESEARCH.md).

Capture VM_READ achevée : **7 480 échantillons, trois lectures incohérentes rejetées**, six capacités natives des deux personnages relevées. Trois HeavyLand de Strife au sol, position verticale stable, durées observées d'environ 0,9–1,2 s ; aucun HeavyLand de War capturé. Rampage Slam et AirDash Land de War sont distincts. Clarification utilisateur : Recovery vise **la réception lourde automatique après chute**, pas l'attaque plongeante. Pas encore de nouveau correctif Recovery construit ou testé.

---

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
