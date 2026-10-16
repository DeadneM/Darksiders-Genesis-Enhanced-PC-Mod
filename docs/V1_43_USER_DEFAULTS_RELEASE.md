# V1.43 — profil utilisateur et release

16 octobre 2026. Base V1.42 livrée `45571dd354e316fcb4f1fa255741844c5dd09d54`. Travail et publication depuis **debug**, avec main/archive conservés.

Objectif : adopter le profil enregistré de l'utilisateur comme défauts ASI/INI et publier une release Windows x64. Réussite : parité des112 clés, mêmes réglages via génération/reset/runtime, CI et archive vérifiés puis release attachée au commit source réellement construit. Statut initial : **PRÉPARÉ**, construction/publication/installation à vérifier.

## Profil constaté

Comparaison intégrale de l'INI installé et du profil livré V1.42 : deux différences seulement.

| Paramètre | Ancien défaut | Nouveau défaut choisi |
| --- | ---: | ---: |
| TPSHorseDistanceMultiplier | 0,15× | **0,25×** |
| TPSHorseHeightOffset | 0 | **+40 unités** |

Récapitulatif : à pied0,15/0 ; cheval0,25/+40 ; ultime0,30/+80 ; pitch−12°, FOV TPS110°. Gains Courroux/Ultime/Synergy4×, Hotstreak2×, réduction munitions2× activée et tous les Unlimited désactivés. Inser, profil des raccourcis et les110 autres valeurs restent inchangés.

L'INI enregistré est sauvegardé dans les preuves locales et repris dans `config/`. Les constantes HorseDistance/HorseHeight alimentent déjà les constructeurs, l'état runtime, les resets et les boutons Default. Les tests de reset/runtime et de parité complète sont adaptés aux nouvelles valeurs. Le journal du reset affiche désormais les valeurs réellement appliquées, sans ancien nombre figé.

## Périmètre et statuts

Les fichiers caméra, visée/entrée du principal, cheval, UI, munitions, ressources, jauges, warning, DXGI et updater sont identiques à V1.42. La modification exécutable est le profil par défaut, l'identité de version et le message de reset ; aucun hook nouveau ou réparation recovery dans cette release. Les validations acquises restent attachées aux fonctions déjà testées, pas étendues arbitrairement à tous les cas.

Action Recovery : priorité **DASH UNIQUEMENT** demandée par l'utilisateur. Coffres reportés. La libération native après déplacement utile du dash doit être observée et identifiée avant un essai ciblé. L'ancien hook reste retiré ; voir l'investigation dédiée.

## Publication prévue

V1.43, archive `DarksidersGenesis_V1.43_USER_DEFAULTS_RELEASE.zip`, quatre entrées à la racine, modules AMD64. Tests et analyse statique Windows avant packaging/publication. Conservation des106 anciennes archives, vérification du téléchargement direct, publication GitHub Releasev1.43 attachée au commit source construit avec ZIP et checksum SHA-256.

Le workflow debug publie une release uniquement pour le marqueur explicite `[release v1.43]`. L'ancien workflow main reste intact et n'est pas déclenché. Une nouvelle release est créée en draft avec ses assets, puis publiée ; aucun remplacement d'une ancienne release ou archive. Après succès, contrôler tag, target_commitish, état public non-prerelease et octets des assets. Le numéro Updates vient de BuildInfoV1.43.

## Installation et rollback

Installer seulement jeu fermé, après sauvegarde des quatre fichiers et du log, en conservant l'INI personnel intégralement. Retour arrière : restaurer les quatre fichiers sauvegardés ou les binaires V1.42 en gardant l'INI ; les mêmes clés existent déjà. Aucun jeu lancé automatiquement.

Test simple : regarder les boutons Default du cheval (0,25/+40), le reset TPS/INI et UpdatesV1.43 ; vérifier que le cadrage pied/ultime et les raccourcis restent conservés. Le prochain travail Recovery porte sur une capture synchronisée du dash, sans coffre.
