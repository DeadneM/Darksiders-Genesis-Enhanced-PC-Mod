# V1.173 — livraison debug vérifiée

Base cumulative V1.172 ; fonctions V1.171 validées. Main/release V1.168 conservées. **CONSTRUIT / CONTRÔLÉ ; gain de fluidité EXPÉRIMENTAL**, à mesurer en jeu. Aucune installation automatique.

## Changements et réactivité

Comptage atomique total des draws retiré, filtrage et sept hooks conservés ; CRC shader à table et préfiltre des tailles exactes ; lectures d'identité regroupées avec retour aux lectures séparées si refus ; focus partagé dans les deux chemins P2 en plus des chemins P1 cumulés. Pas de nouveau délai, throttle ou cache entre images. Aucun changement de fréquence des skins/effets/services/entrées, aucun réglage/défaut/binding modifié. Les autres optimisations de l'audit restent hors du lot. [Périmètre et rollback](V1_173_REVIEW.md).

## Contrôles

- [GitHub Actions Windows x64 Release](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/44609069811) : **86/86 tests**, zéro erreur C++. Tests CRC contre l'ancien algorithme, tailles/signatures, identités/refus/frontières/GC/recyclage, caméra et isolation P1/P2 compris.
- **Un avertissement d'analyse C28020** dans AimShaderCatalog.h:212 : indice de génération de la table constexpr. Revue indépendante : boucle byte<table.size() (256), aucune possibilité d'indice hors bornes ; table évaluée à la compilation, accès CRC runtime masqué &0xFF. L'avertissement n'est pas masqué. Pas de seconde compilation/ZIP uniquement pour ce diagnostic ; le seuil strict zéro avertissement n'est donc pas atteint. Revoir la forme de cette boucle lors du prochain lot de source.
- [ZIP direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/d84168bd5184247f09088e61bbee529438eaf51f/downloads/DarksidersGenesis_V1.173_REACTIVE_CPU_OPTIMIZATION_TEST.zip) : **2504942 octets**, SHA256 `9CAD7BC844E97E0DA74570EB9662F6DCDC3DC67FEF6246F50F92C6A8E5544292`.
- Deux fichiers AMD64 à la racine : DarksidersGenesisMod.asi et dxgi.dll ; CRC ZIP, empreintes, blob GitHub et lien public HTTP200 vérifiés. Aucun INI/README inclus.
- Microsoft Defender : analyse terminée sur les deux fichiers extraits du nouveau ZIP, aucune détection correspondante.
- **259 clés de référence et leurs valeurs inchangées** ; defaults, UI et bindings conservés. **237 anciennes archives intactes**, 238 au total. Main/archive/tags protégés vérifiés inchangés. ZIP neuf récupéré une seule fois via Git, sans téléchargement des anciens binaires. Aucun INI personnel/sauvegarde remplacé.

## Test groupé conseillé

1. Même scène et mêmes réglages : comparer fluidité et aspect/mise au point TPS, à pied puis cheval/ultime. Pas de gain GPU ou FPS encore démontré.
2. Changer rapidement skins et effets, puis personnage ; charger une partie. Vérifier application et restauration sans retard supplémentaire, texture/effet manquant ou ancienne apparence persistante. Utiliser Stoneskin au passage.
3. Vérifier les traits/arcs, tirs, World Ender et sortie de pause/roue. Coop en dernier : caméra P2 et pause.

Installation manuelle ou updater debug en conservant l'INI. Rollback : remettre les deux binaires V1.172 avec le même profil personnel. L'appel aérien V1.172 reste expérimental ; ce lot ne le déclare pas validé. [Preuves détaillées](evidence/V1_173_DELIVERY_CHECK.json).
