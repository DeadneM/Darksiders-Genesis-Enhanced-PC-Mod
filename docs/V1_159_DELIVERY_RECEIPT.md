# V1.159 stable — reçu de publication main

## Base et statut

Promotion et compilation explicitement demandées le 6 novembre 2026. **CONSTRUIT / CONTRÔLÉ / PUBLIÉ sur main**. Base debug V1.158 `063219c788e83686abaee0e46ab70f3fdc3d5473` ; source stable compilée `7ce851b20326ce811226bcd804bb14a8344469ae` ; commit de l'archive `327a913a91af57980fc2e6b3471e3999484a89ed`. Main précédent `563ae9ca8c2647689af8464d320544afe55b1894` conservé dans l'historique. Aucun build local ni installation dans le jeu.

Seul `src/BuildInfo.h` change parmi les sources, tests, configuration, updater et fichiers de compilation : V1.159 et identifiant stable. Même comportement que V1.158. Le numéro évite de remplacer son ZIP déjà publié. Les défauts, les 55 actions Hotkeys et les 248 clés INI sont conservés ; aucun INI personnel n'a été modifié.

## Contrôles

- GitHub Windows x64 : **82/82 suites réussies**, aucun avertissement C++ détecté. Les avertissements externes de l'outillage restent consignés. [Exécution](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/42212803842).
- ZIP : exactement deux binaires AMD64 à la racine, `DarksidersGenesisMod.asi` et `dxgi.dll`, CRC et octets vérifiés. Aucun INI/README ; documentation gardée dans le dépôt.
- Téléchargement immuable HTTP 200 vérifié ; l'updater sur le canal main sélectionne V1.159, taille et empreinte Git blob concordantes. Ce contrôle de découverte ne constitue pas un nouvel essai d'installation après fermeture.
- 223 anciennes archives inchangées, 224 au total. Debug reste `063219c788e83686abaee0e46ab70f3fdc3d5473` ; archive et tag v1.43 inchangés.
- Defender : aucune menace détectée sur le ZIP et les deux binaires ; protections actives et inchangées.

## Téléchargement

[ZIP stable V1.159](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/327a913a91af57980fc2e6b3471e3999484a89ed/downloads/DarksidersGenesis_V1.159_STABLE_CUMULATIVE.zip) — 2,430,702 octets.

SHA256 : `6AB63FC3D5BCBDE979D3880636D2ECC290A3965ED97AE461A4D87CFE35C3CF15`.

| Fichier | Octets | SHA256 |
| --- | ---: | --- |
| dxgi.dll | 155136 | `D6F9C45EA637DAF330220A01BE6C61B3F62AA2091CC696ACB8BC407E19D8CD83` |
| DarksidersGenesisMod.asi | 2275328 | `A093BEFB4A8D1D4ED4BADC19A84ABF39104EF2F2DBCA7E72A973C0BB68C2F5D9` |

## Validation en jeu et suite

Écharpe et menu acceptés ; cheval favorable avec réserve. L'aura Molten fonctionne à chaque essai du dernier relevé, mais son ancienne intermittence n'a pas été reproduite et aucune nouvelle cause n'est déclarée réparée. Coop/P2 reste différée. La recompilation stable n'a pas fait l'objet d'un nouvel essai en jeu ; aucun gain de FPS n'est annoncé mesuré.

Installation : extraction manuelle des deux binaires après fermeture du jeu, en conservant l'INI existant, ou canal main de l'updater. Aucun jeu lancé par l'agent. Rollback : archive V1.158 conservée ; V1.153 reste dans l'historique de main. Aucun tag ni nouvelle release GitHub créé par cette promotion.

[Périmètre de promotion](V1_159_MAIN_PROMOTION.md) · [CI](evidence/V1_159_CI_CHECK.json) · [Paquet et updater](evidence/V1_159_PACKAGE_CHECK.json) · [Defender](evidence/V1_159_DEFENDER_CHECK.json) · [Sources conservées](evidence/V1_159_MAIN_SCOPE.json).
