# V1.91 — reçu du diagnostic Geyser corrigé

23 octobre 2026. **CONSTRUIT / CONTRÔLÉ, DIAGNOSTIC EXPÉRIMENTAL. Geyser sur Strife reste NON CORRIGÉ.** Base expressément validée V1.87 `c20319f93413f849a41b10fdfd96f7842a172ac6` pour les cas rapportés ; cumul V1.90 archive `cc3932ad985ba779d47f60b3f2865e01d5970239`. Transfert Geyser V1.89 rejeté par l’utilisateur : pose de repos, aucune attaque ; retour Native et pouvoirs propres confirmés, P2 reporté.

- Sources compilées : `bb0ff79fab3abcd7eab8eca9f5000db7b475060f` ; archive : `0be129010adbb857e0d3de6bbcdd32b3d7022c1b`.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40016586657) : run 40016586657, job 120630097079, success, **53/53 suites**, zéro erreur et zéro avertissement MSVC, analyse native activée. Messages de dépendances/outillage consignés séparément.
- [ZIP public HTTP200 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/0be129010adbb857e0d3de6bbcdd32b3d7022c1b/downloads/DarksidersGenesis_V1.91_GEYSER_DISPATCH_DIAGNOSTIC_TEST.zip) : **921400 octets**, SHA256 `A63E57ABE0B82392CFE39A875420D5B6BBD9F732688F3204CA51D936269B32CC`. Contenu distant identique au local, CRC valides, deux binaires AMD64.

| Fichier racine | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1682432 | `07B83A8F1483776E1E1DEB448F13DE8E7801EA1638EC65E60AEF348526469C98` |
| `DarksidersGenesisMod.ini` | 4691 | `99E3EECAD438A6735DD4389D544535FF798BB7F9E9EF0E4E243BC40CC4F13524` |
| `dxgi.dll` | 155136 | `3890D51C6E3ED7E9A61E7DE2FC50948EEFC948EE5BB46335B63DB3593B3484E3` |
| `README.md` | 50001 | `B23B5D60D9BE15DDDF7AEC93A2C36A23592A9F5007892B202BFB646DCBAC6720` |

## Second contrôle et changement exact

V1.90 intermédiaire a passé ses 53 suites sans avertissement MSVC, mais le second audit a trouvé War=0 dans le filtre et la fixture. Le chemin natif validé WrathSlotFeature::Map utilise Strife=1 / War=2. Le test partageait l’erreur d’identité : son succès ne prouvait pas la fidélité au jeu. **V1.91 corrige filtre et fixture vers War=2**, vérifie le rejet War=0 et protège aussi la recherche du cache par le thread de jeu. V1.90 reste archivée sans modification ; [limite documentée](V1_90_DELIVERY_RECEIPT.md). Aucun changement du montage, des dégâts, coût, ressources ou fin du pouvoir.

La comparaison externe en lecture seule a trouvé la notification native dans les files War et Strife, pas la preuve de son appel effectif. Trace désormais ciblée sur l’entrée/sortie du SpawnBlast Geyser exact et les événements imbriqués de ses quatre classes de blast, propriétaire local à pied, thread de jeu, pas de CDO. Huit appels par session, quatre événements de blast par appel. Le passage ProcessEvent existant reçoit ses quatre arguments une seule fois ; contexte restauré même sur exception C++. Aucun nouvel hook/timer et aucune écriture de gameplay dans le diagnostic. **Événement de blast ne prouve pas les dégâts ; absence d’événement imbriqué n’exclut pas la création d’un acteur sans événement.** Compteur global d’objets lu sans lui attribuer une causalité de blast.

Suite ForeignWrath : arguments et transmission unique, imbrication/budget, scope/exception, War natif type 2, rejet type 0, propriétaires/classes/threads inéligibles, cache exact et hors thread, absence de mutation des acteurs/ability/blast. Les 53 suites passent. Ces tests ne valident pas la trace sur une partie réelle ni une réparation Geyser. Recherche d’enfants de classe couverte par test ; nouveau cache non vérifié sur une instance actuelle, le jeu étant fermé. Le journal TRACE ready permettra de constater sa résolution réelle.

**168 clés INI et défauts, 55 ID Hotkeys, 155 anciennes archives intacts (156 ZIP au total)**. 116 anciens fichiers source inchangés depuis la base validée. Caméras, tirs, XInput/roues, bras/holsters, lames, Léviathan et ancien cast Stoneskin conservés ; module principal modifié uniquement pour envelopper son passage d’événements déjà utilisé. Pas de test coop supposé ni de nouveau changement des quatre transferts encore ouverts.

## Essai, repli et suite

Installation **manuelle**, conserver INI/raccourcis/PAK/sauvegardes personnels. Aucun fichier du jeu remplacé ni jeu lancé par l’agent. Le README du ZIP décrit la préparation ; ce reçu confirme la compilation sans réécrire l’archive.

Préparer un emplacement Strife sur Blade Geyser ; faire une activation avec War sur Native puis une avec Strife, à pied et hors menus, de préférence en mission. Revenir à Native et conserver le journal. Si TRACE ready/DISPATCH manque, ne pas multiplier les activations. Aucun test P2 demandé. Le résultat attendu est une trace exploitable, **pas une amélioration annoncée du pouvoir**.

Rollback : Native dans l’emplacement ou ASI V1.87 validé en gardant son INI. Suite : corriger seulement le maillon démontré par cette exécution, retirer la trace temporaire après diagnostic. Si le passage reste non observable, revoir l’approche avant une autre modification de montage. Rampage vers Strife, Clone/World Ender/Caltrops vers War restent ouverts ; réception lourde incertaine et autres FX P2 reportés, HUD général non reproduit.

main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, v1.43 `902861eb1ff34c14dc5ded092581b360767da817` vérifiés intacts ; fichier de reprise non suivi préservé.
