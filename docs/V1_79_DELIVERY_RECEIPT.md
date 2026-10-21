# V1.79 — reçu de livraison vérifiée

22 octobre 2026, branche debug. **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE**, aucune validation en jeu des nouveaux comportements. Base cumulative V1.78, acquis utilisateur conservés dans les limites des essais rapportés. Installation manuelle uniquement ; aucun fichier du jeu, INI personnel, PAK ou sauvegarde remplacé par l’agent.

- Source initiale : `86ec926a6ff43496603036d1a4cfe0beb5b18c74` ; source finale avec garde du menu : `9753044f0c5abb278252326ee9cbf97c26a63a2b`.
- [Actions Windows x64 Release + analyse statique](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39683569838) : **48/48 suites réussies**, dont MountedJump, PistolVisual et ProjectileSpawn. **Zéro avertissement MSVC** ; un avertissement de dépréciation CMake préexistant dans MinHook reste présent. Aucun changement de dépendance ni masquage de cet avertissement.
- Commit d’archive : `7f579ed3f068ff1cba6f25a7a6ba680ba5759517`. L’archive numérotée reste immuable ; le premier run de la source initiale a été remplacé avant livraison.
- [ZIP cumulatif direct vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/7f579ed3f068ff1cba6f25a7a6ba680ba5759517/downloads/DarksidersGenesis_V1.79_NATIVE_WORLDENDER_BLADES_MOUNTED_JUMP_TEST.zip) : **867383 octets**, HTTP 200 ; téléchargement public comparé octet pour octet par longueur et SHA256.
- SHA256 ZIP : `2279F3712E120454866C892DA12506BB93E37679D5663FD78AA15258E43C443B`.

| Fichier racine | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1537536 | `292B479EC9759C10E599AA05BFF909DD0294F39ADAB12990548B5CFEDF18C708` |
| `DarksidersGenesisMod.ini` | 4627 | `6F24118C109AE0F9A78D34A99871F004F20EC5BB2BA0DE89037C9F8DD1DB2F56` |
| `dxgi.dll` | 155136 | `505D11948D227B6A64E9E50FE33E8DA9180A42DD27C749EC2E7DC905CB0A66EA` |
| `README.md` | 22243 | `11AE085EF3C060A61ED5B1E1249078FF746FA5975EEF25886FAC349A22353886` |

Quatre fichiers à la racine, CRC ZIP valide, deux PE AMD64. INI : **165 clés**, les **163 anciennes valeurs inchangées**, deux ajouts (`MountedJump=0`, `StrifeBladeVisibility=0`). Les **55 identifiants d’actions Hotkeys** conservent leur définition ; le nombre de clés d’une section INI n’est pas le nombre de ces actions. Sauvegarde/chargement des nouveaux choix et raccourcis personnels contrôlés dans les fixtures Windows.

Double lecture du lot : comparaison source/configuration/protections, puis confrontation avec les relevés natifs et tests des frontières. **100 fichiers source hors lot inchangés**, aucun nouveau hook. L’interception monture initialement envisagée a été retirée après constat d’absence de JUMP 30 ; le build lit l’action réaffectable Jump puis utilise DoJump natif. Les appels de recherche de touche lisent les états propres au contrôleur, sans transfert de propriété d’un FKey emprunté. La troisième capture des états Jump n’a pas fourni de nouvel appui exploitable ; ne pas la présenter comme une validation du saut monté.

World Ender : les relevés et logs horodatés prouvent le passage dans notre correction de faisceau ordinaire pendant le pouvoir. Le build l’exclut par provenance de son projectile et conserve l’axe natif du canon/de l’animation, 3D aiming ON/OFF. Aucun changement de caméra, matériau, largeur ou dégâts. **Visibilité rapprochée et résultat du candidat à valider en jeu** ; pas de réparation matérielle ou de rendu intérieur annoncée.

Les **143 anciennes archives** sont identiques au manifeste de départ. Fichier de reprise préexistant inchangé et non suivi. Références protégées attendues : main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, v1.43 `902861eb1ff34c14dc5ded092581b360767da817` ; vérification distante effectuée avant publication, à renouveler après le commit de ce reçu.

[Changements, limites, essais groupés et rollback](V1_79_NATIVE_WORLDENDER_BLADES_MOUNTED_JUMP_TEST.md). Garder son INI personnel ; tester surtout les deux bras après actions, les côtés de holsters, les quatre choix d’épées, World Ender dans les deux modes de visée et le saut de chacune des deux montures. Coop ultérieure, réception lourde encore incertaine, six capacités entre personnages et suivi vertical des exécutions restent ouverts. Le README empaqueté décrit la préparation du lot ; ce reçu consigne les résultats obtenus après sa construction, sans réécrire le ZIP.
