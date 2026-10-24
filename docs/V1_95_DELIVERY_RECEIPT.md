# V1.95 — reçu de livraison

24 octobre 2026. **CONSTRUITE / CONTRÔLÉE ; Geyser sur Strife EXPÉRIMENTAL, NON VALIDÉ EN JEU.** Base validée V1.87 dans les cas rapportés ; cumul V1.93, dont le transfert Geyser reste rejeté dans le dernier essai. La nouvelle restauration de présentation n’est pas un résultat de jeu acquis. Installation manuelle avec INI/raccourcis/PAK/sauvegardes personnels conservés ; aucun lancement ou remplacement de fichier du jeu par l’assistant.

- Sources compilées : `dcef9ac95154f9c304b6d7865eb57b853166fa17` ; archive au commit `027dae183a714754e66263e4f0f3bc0622498c42`.
- [GitHub Actions 40114886605](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40114886605) : **54/54 suites**, zéro erreur et zéro avertissement MSVC, garde de provenance exécutée.
- [ZIP cumulatif public vérifié HTTP 200](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/027dae183a714754e66263e4f0f3bc0622498c42/downloads/DarksidersGenesis_V1.95_STRIFE_GEYSER_PRESENTATION_TEST.zip) : 941487 octets, SHA256 `63DDDFC77BBC1459C78F00AAF8BDCFA13F151252BD7FC73D68948B65FA0D5F26`. CRC valides, binaires AMD64, quatre fichiers à la racine ; octets distants identiques aux locaux.

| Fichier | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1726464 | `EB0550B9F3DFFEA4B5A6250A5EE4DE02B070C8DC4D33EDF36E3F14A7D3242FD0` |
| `DarksidersGenesisMod.ini` | 4691 | `99E3EECAD438A6735DD4389D544535FF798BB7F9E9EF0E4E243BC40CC4F13524` |
| `dxgi.dll` | 155136 | `B0037EC492EAFD7CA7BA8A70840C9C939529055670BE79926AE6AA3D21D6C229` |
| `README.md` | 58574 | `F9342EE6CADE82D25E5FC30B551AAFB8810EA8F73186D754ED7035EFE8065529` |

## Changement et limites

Le montage privé garde un seul SpawnBlast et réunit le son de lancement avec les lames/nova et leurs variantes, à leurs temps natifs. Les effets sur Bone_War_Root sont copiés vers la propre racine Strife ; offsets, rotation précalculée, taille et mode d’attachement sont préservés. Voix, balayages et effets propres à l’épée de War exclus. Source War, CDO et assets natifs inchangés. Le tableau privé est agrandi avec le helper natif de réserve pour éléments 184 octets, signature contrôlée ; aucun buffer CRT confié à Unreal. Pas de nouveau hook/minuterie, ni second blast ou modification des dégâts/coûts.

La seconde lecture indépendante des dix fichiers d’assets extraits avec SHA1 vérifié produit **17 / 25 / 33 / 41 événements** pour les quatre variantes : un callback, un son, temps valides et racines compatibles dans chaque cas. Les tests Windows vérifient ordre et temps, conservation/exclusion des effets, racine/offset/quaternion, copies sans alias, échecs de réserve/clone, doublons, dépassement et arrêt isolé. Leurs succès ne prouvent pas le rendu ni les dégâts en jeu.

**La pose de lancement reste Strife_Idle** ; le geste de War n’est pas transféré. Geyser sur Strife doit être vérifié visuellement et en dégâts en mission, avec fin de cast normale. Le diagnostic temporaire limité à huit appels reste pour comparer l’unicité réelle de SpawnBlast et sera retiré après cette vérification. P2 reporté. Caltrops sur War V1.93 reste expérimental, Stoneskin et pouvoirs propres inchangés ; Clone/World Ender vers War et Rampage vers Strife indisponibles. Aucun statut ancien transformé en validation.

Caméras, tirs, XInput/roues, cheval, bras/holsters, effets des lames, réticule, Léviathan, autres acquis et leurs sources sont inchangés dans ce lot. **168 clés INI, 55 ID Hotkeys, 159 anciennes archives intacts** (160 ZIP au total). main/archive/v1.43 vérifiés inchangés. Les avertissements d’outillage CI, s’ils sont présents, sont consignés séparément dans les mesures ; aucun avertissement du compilateur n’est masqué.

## Test et suite

Après installation manuelle en gardant son INI : Strife à pied en mission, Geyser près d’ennemis. Distinguer son/effets/placement, dégâts/Courroux, puis reprise du mouvement et des attaques. Tester une amélioration si disponible, Geyser propre de War sur Native, puis retour Strife sur Native. En cas d’échec, remettre Native et conserver le journal ; éviter les activations répétées d’un cast en défaut. Pas de nouvelle capture/relance demandée avant ce retour.

Rollback : Native dans le sélecteur, ou restauration de son ASI précédent en gardant l’INI ; V1.87 reste la base validée dans les cas rapportés. Suite : analyser le résultat de présentation et les dégâts séparément, puis poursuivre les transferts encore indisponibles. Le README embarqué décrit la préparation ; ce reçu confirme la compilation sans modifier l’archive publiée.
