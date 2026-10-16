# V1.43 — profil utilisateur et release

16 octobre 2026. Base V1.42 livrée `45571dd354e316fcb4f1fa255741844c5dd09d54`. Travail et publication depuis **debug**, avec main/archive conservés.

Objectif : adopter le profil enregistré de l'utilisateur comme défauts ASI/INI et publier une release Windows x64. Réussite : parité des112 clés, mêmes réglages via génération/reset/runtime, CI et archive vérifiés puis release attachée au commit source réellement construit. Statut final : **CONSTRUIT / CONTRÔLÉ / PUBLIÉ / INSTALLÉ**. Aucune nouvelle validation complète en jeu revendiquée ; Recovery toujours indisponible.

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

## Publication contrôlée

Premier essai CI38880117769 : compilation arrêtée par C2660, appel du logger Store à plusieurs arguments au lieu d'une chaîne formatée. Réparé par formatage préalable dans un buffer borné puis appel normal du logger. Tests, packaging et publication ont été sautés sur cet essai ; aucune archive/release V1.43 n'en provient. La construction suivante38880517949 a réussi et est contrôlée séparément dans le reçu ci-dessous.

V1.43, archive `DarksidersGenesis_V1.43_USER_DEFAULTS_RELEASE.zip`, quatre entrées à la racine, modules AMD64. Tests et analyse statique Windows avant packaging/publication. Conservation des106 anciennes archives, vérification du téléchargement direct, publication GitHub Releasev1.43 attachée au commit source construit avec ZIP et checksum SHA-256.

Le workflow debug publie une release uniquement pour le marqueur explicite `[release v1.43]`. L'ancien workflow main reste intact et n'est pas déclenché. Une nouvelle release est créée en draft avec ses assets, puis publiée ; aucun remplacement d'une ancienne release ou archive. Tag, target_commitish, état public non-prerelease et octets des assets ont été contrôlés après publication. Le numéro Updates vient de BuildInfoV1.43.

## Installation et rollback

Installer seulement jeu fermé, après sauvegarde des quatre fichiers et du log, en conservant l'INI personnel intégralement. Retour arrière : restaurer les quatre fichiers sauvegardés ou les binaires V1.42 en gardant l'INI ; les mêmes clés existent déjà. Aucun jeu lancé automatiquement.

Test simple : regarder les boutons Default du cheval (0,25/+40), le reset TPS/INI et UpdatesV1.43 ; vérifier que le cadrage pied/ultime et les raccourcis restent conservés. Le prochain travail Recovery porte sur une capture synchronisée du dash, sans coffre.


## Reçu de livraison

Source `902861eb1ff34c14dc5ded092581b360767da817` ; [CI38880517949](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38880517949), job116905862398 : **SUCCESS**, 13/13 suites. Tests de configuration : les 112 clés du profil livré correspondent aux valeurs compilées, générées, reset et runtime, avec compatibilité des anciens INI. Analyse MSVC : les mêmes huit avertissements qu'en V1.42 (DXGI C6385 ; caméra C4456/C6246/C6393 ; SkipLogos C6011 ; SkipWarning C6385 ; updater deux C6262). Ces avertissements restent documentés, pas masqués.

[Release v1.43](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases/tag/v1.43), id414771221, publiée 2026-10-16T09:29:27Z, publique et non-prerelease, désignée latest. Tag léger et target_commitish = source `902861e`. Asset `DarksidersGenesis_V1.43_USER_DEFAULTS_RELEASE.zip` = **807377 octets**, SHA256 `A680334FC4E634AC24AF92DF59A00BEFCCA8B4716EEABDDCB08BCE1C64FAF990`, plus checksum `V1.43_SHA256.txt`. Octets du téléchargement HTTPS identiques à l'archive Git/CI, digest GitHub et checksum. Publication de l'archive sur debug : `4f6115ff9a2cf30bb3523e6b7544c6a7bc645a6f`. Les 106 ZIP antérieurs sont identiques à leur manifeste de départ ; main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7` et archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` inchangés.

Quatre entrées racine vérifiées : ASI, dxgi.dll, INI et README. Les deux binaires sont PE AMD64 ; ASI identité1.43 et libellé Updates dynamique. ASI SHA256 `13955763047FCD747F68E64912F1FAD9B63EE525A13B290E5B143292A696D27E` ; DXGI SHA256 `60EDD4E5D50DF34A77BEB0E12BB0305D3CE64E6555DADD11120B0712E9859E38`. Les dix-huit fichiers natifs/règles protégés sont identiques à V1.42. Les deux nouvelles constantes par défaut et le message du reset constituent les seules modifications de comportement hors identité de build ; aucun hook nouveau.

Installation jeu fermé, sauvegarde `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v143-20261016-113324`, comprenant quatre fichiers précédents et log. Hashes installés conformes ; INI personnel intégralement conservé, avant=après `5766A97774293C8C30F14D0855EAEC4A4B1138C899548A0088DC9A0E3BA9D0D7`. README installé actualisé après le reçu ; le ZIP historique reste immuable avec le README de préparation. Aucun lancement de jeu. Ce contrôle établit construction, publication et installation, pas un nouveau résultat visuel.

TODO : vérifier Default cheval 0,25/+40, reset TPS, UpdatesV1.43, pied/ultime et raccourcis. Pour Recovery, observer seulement le dash en lecture seule avec chronologie complète avant une modification native ; coffres reportés. Critère futur : reprise mesurable plus tôt sans glissement, altération de distance ou invulnérabilité prolongée. L'ancien hook sans effet reste retiré.
