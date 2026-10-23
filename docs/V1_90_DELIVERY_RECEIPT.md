# V1.90 — reçu du diagnostic Geyser

**INTERMÉDIAIRE NON RETENUE pour la comparaison War/Strife.** Second audit : le filtre et son test synthétique utilisaient War=0, alors que le chemin natif du mod exige War=2 (WrathSlotFeature::Map). Les 53 suites passaient avec la même erreur de fixture ; elles ne prouvent donc pas la fidélité de cette identité native. V1.91 corrige le filtre/test et protège explicitement la recherche du cache par le thread de jeu. L’archive V1.90 reste immuable ; ne pas l’utiliser comme candidat final. Geyser lui-même reste non corrigé.

23 octobre 2026. **CONSTRUIT / CONTRÔLÉ, DIAGNOSTIC EXPÉRIMENTAL. Geyser sur Strife reste NON CORRIGÉ.** Base expressément validée V1.87 `c20319f93413f849a41b10fdfd96f7842a172ac6` pour les cas rapportés ; base cumulative V1.89 `6921fb7a19dc93bff331f937299d5fbe0f020a2e`. Transfert Geyser V1.89 rejeté par l’utilisateur : pose de repos, aucune attaque ; retour Native et pouvoirs propres confirmés, P2 reporté.

- Sources compilées : `5fc9860fc0fe4bf8114e68143772bae9b3beec3f` ; archive : `cc3932ad985ba779d47f60b3f2865e01d5970239`.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40015555280) : run 40015555280, job 120626590778, success, **53/53 suites**, zéro erreur et zéro avertissement MSVC ; analyse native activée. Les messages de dépendances/outillage sont consignés séparément.
- [ZIP public HTTP200 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/cc3932ad985ba779d47f60b3f2865e01d5970239/downloads/DarksidersGenesis_V1.90_GEYSER_DISPATCH_DIAGNOSTIC_TEST.zip) : **920812 octets**, SHA256 `F433D3D41B831CB0834DB6067D02103EFD82534CC41E872748F7BFBD05EC5981`. Contenu distant identique au local, CRC valides et deux binaires AMD64.

| Fichier racine | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1682432 | `1A6DB8FEF837B7538B7A9DB0A0BAF4933AD8CC8B00C137228D0566616085E6B6` |
| `DarksidersGenesisMod.ini` | 4691 | `99E3EECAD438A6735DD4389D544535FF798BB7F9E9EF0E4E243BC40CC4F13524` |
| `dxgi.dll` | 155136 | `CF3C5A06C9E536B6D941AD6081F8628702ECFCF931878E601AF9A981937BA2A7` |
| `README.md` | 47507 | `D3ABB7650BF7C9F9D5DCCA6BCD663A94BE8C7A698C2C5E0C3D6F628E9F7289D4` |

## Changement, preuve et limites

Comparaison en lecture seule War/Strife : montage avance normalement et la notification native SpawnBlast figure dans les deux files d’animation. Cela ne prouve pas son appel effectif. Aucune raison démontrée de remplacer encore le montage ou de forcer le coût. Le relevé a eu lieu dans Level_00, où l’absence de consommation n’est pas une preuve de défaut.

Trace temporaire intégrée au passage ProcessEvent déjà intercepté : fonction exacte Geyser SpawnBlast, propriétaire local réciproque à pied, thread de jeu, pas de CDO ; War et Strife. Huit appels par session, quatre événements imbriqués de blast par appel. Une seule transmission native, quatre arguments conservés, restauration du contexte de thread même sur exception C++. Aucun hook/timer, coût/dégât/ressource, animation ou fin supplémentaire ajouté. L’événement natif de blast indique son existence à cet instant, pas ses dégâts ; l’absence d’un événement imbriqué n’exclut pas un acteur sans événement. Le compteur global d’objets n’est pas assimilé à une preuve de création de blast.

Suite ForeignWrath étendue : arguments exacts, appel unique, imbrication, budget, scope et exception, War natif, classes/propriétaires/threads non éligibles, cache de fonction exact, objets source inchangés. Les 53 suites passent ; **ni la trace sur le jeu ni une réparation Geyser ne sont validées en jeu à cette livraison**. Recherche des enfants de classe couverte par test ; nouvelle résolution sur une instance actuelle non vérifiée, le jeu étant fermé. Le journal TRACE ready permettra de le constater.

**168 clés INI et défauts, 55 ID Hotkeys, 154 anciennes archives intacts (155 ZIP au total)**. 116 anciens fichiers source strictement inchangés depuis la base validée ; code des caméras/tirs/XInput/roues/bras/holsters/lames/Léviathan conservé. Seul l’enveloppage du passage d’événements existant change dans le module principal. Ancien cast Stoneskin strictement conservé ; quatre autres transferts restent ouverts. Aucun test P2 supposé.

## Installation, essai et suite

Installation **manuelle** en gardant INI/raccourcis/PAK/sauvegardes personnels. Aucun fichier du jeu remplacé ni jeu lancé par l’agent. Le README embarqué décrit la préparation ; ce reçu confirme la compilation, sans réécrire le ZIP.

Préparer un emplacement Strife sur Geyser ; faire une activation avec War Native, puis une avec Strife, à pied, hors menus, de préférence en mission. Revenir à Native et conserver le journal. Si TRACE ready/DISPATCH manque, ne pas multiplier les activations. Aucun essai coop demandé. Le résultat attendu est une trace exploitable, pas une amélioration annoncée du pouvoir.

Rollback : Native dans l’emplacement ou ASI V1.87 validé, en conservant son INI personnel. Suite : identifier le maillon réellement défaillant à partir de cette trace, puis retirer la trace temporaire avant stabilisation ; si le point n’est pas observable, revoir l’approche avant tout autre remplacement d’animation. Rampage vers Strife, trois pouvoirs Strife vers War, réception lourde incertaine et autres FX P2 restent ouverts/reportés.

main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, v1.43 `902861eb1ff34c14dc5ded092581b360767da817` vérifiés intacts. Fichier de reprise non suivi préservé.
