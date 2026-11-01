# V1.140 — reçu de livraison

**CONSTRUITE / CONTRÔLÉE ; EXPÉRIMENTALE, aucun résultat visuel/performance annoncé validé.** Base source V1.139. Les acquis sont préservés ; correction Abyssale et allègements ciblés, pas de réécriture de la visée.

- Source : `ee92a00b06d0871aa1557a65dbe308e36e8554d5` ; archive debug : `35428f8142a685bf8833d4268be14c99a4187e02`.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41323327523) : 70/70 suites, zéro avertissement/erreur MSVC.
- [ZIP direct immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/35428f8142a685bf8833d4268be14c99a4187e02/downloads/DarksidersGenesis_V1.140_STRIFE_COST_ABYSSAL_STONESKIN_TEST.zip) : 2,438,344 octets ; SHA-256 `F6830EE1A8020F8464F789EB5EE41986BBCD59BB414630A84771556EB35DF9DC`.
- Quatre fichiers à la racine, CRC valides, conteneur sans compression, ASI/proxy AMD64 ; 204 anciennes archives Git identiques.
- 228 clés INI et 55 actions Hotkeys identiques ; branches main/archive et tag v1.43 vérifiés inchangés.
- Updater debug : archive sélectionnée et téléchargement HTTP 200 identique au ZIP contrôlé. Interface du jeu non observée.
- Defender : ZIP et quatre fichiers sans menace signalée, sans remédiation ni changement de protection.
- Aucune installation ni lancement par l'agent. ASI/proxy V1.139 et INI personnel identiques aux empreintes relevées avant livraison. Aucun PAK/sauvegarde modifié.

## Essai groupé

Installation via l'updater debug ou manuelle, en gardant l'INI personnel. En mission, hors menu/pause :

1. Strife à pied, immobile une minute, puis déplacement/tir une minute. Noter le ressenti et garder le journal PERF.
2. Comparaison même scène avec War, puis Strife à cheval. Les mesures sont inclusives et échantillonnées ; elles ne donnent pas les FPS.
3. Armure Abyssale choisie avant Stoneskin : vérifier pierre/feu et disparition/restauration ; comparer à l'armure normale.
4. Contrôle de non-régression : pistolet OFF/bras en marche, pause et roues ; trait/arc masqué selon la vue.

Suite : exploiter ces mesures pour traiter le véritable poste coûteux si le ralentissement persiste. Ne pas supprimer une fonction validée sur la seule base d'un compteur. Rollback : fichiers V1.139 et INI personnel conservé. [Analyse, limites et deux passes de revue](V1_140_STRIFE_COST_ABYSSAL_STONESKIN_TEST.md).
