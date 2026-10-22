# V1.80 — reçu de livraison contrôlé

22 octobre 2026. **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE**. Saut à cheval V1.79 confirmé par l'utilisateur ; sons et nouveau candidat de bras OFF **non validés en jeu**. La compilation, les fixtures et l'intégrité du ZIP ne remplacent pas cet essai.

- Base cumulative : V1.79, `5e2221ba8c0d6ed75f743c0cebeb073301b18487`, `debug` seulement.
- Sources compilées : `68200719a7a5c5cd2da3a1871aa4b07485a017a5`.
- Commit de l'archive immuable : `8124f3e683d16dafbabf6bc610c7f2998a2dce4f`.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39793144590) : run `39793144590`, job `119873907314`, résultat success ; **48/48 suites, zéro erreur et zéro avertissement MSVC**.
- Les avertissements d'outillage restent visibles : ancien minimum CMake dans MinHook, Node.js 20 des actions checkout/upload et dépréciation url.parse de leur environnement. Aucun avertissement masqué ; ils ne sont pas des erreurs du code ASI.
- [ZIP public vérifié HTTP 200](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/8124f3e683d16dafbabf6bc610c7f2998a2dce4f/downloads/DarksidersGenesis_V1.80_MOUNTED_SOUNDS_PACKET_ARMS_TEST.zip), **871028 octets**, SHA256 `91BBDAEC32D44B8B0473705C1C943CE199A9DF56565582F6195265952A6009CE`. Téléchargement public comparé à l'archive locale, taille et empreinte identiques.

## Contenu et préservation

Quatre fichiers exactement à la racine, CRC valides, ASI et dxgi AMD64. README/INI comparés aux sources compilées, marqueur V1.80 retrouvé dans l'ASI.

| Fichier | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1546240 | `B5747844803B4FB7D8E84BD60C231708B532B7D69B4EE6D0B0EAB9ACB371CB76` |
| `DarksidersGenesisMod.ini` | 4647 | `03D37FB4E6D4C21DF3BE5596FA9451EEEECE21B3A7FF276A8027C49161B30B22` |
| `dxgi.dll` | 155136 | `6E191AB51EB6BBFE152532AC3A10824B716E5576595243D9B26C9237FE171B87` |
| `README.md` | 24482 | `08A8EBC2685DB7208BDFC1E2AF91A2A7FBC814322E562D93B30166DA84FFDA94` |

Les **144 anciennes archives sont inchangées**, 145 ZIP au total. **165 anciennes valeurs INI conservées**, une nouvelle clé `Values/MountedJumpSound=0`, soit 166 clés fournies. Les 55 ID d'actions Hotkeys n'ont pas été décalés ; la section Hotkeys fournie compte séparément 12 entrées INI. 102 fichiers source ne changent pas dans le lot. Aucun nouveau hook.

Les fonctions de caméra/visée, World Ender, XInput et roues, dash, ressources et lookup Courroux sont préservées. Les anciens réglages et choix utilisateur restent la référence. Aucun fichier du jeu, INI personnel, sauvegarde ou PAK remplacé ; aucune relance ou écriture de mémoire externe par l'agent. Le fichier non suivi REPRISE_AUDIT_2026_10_13.md reste intact et hors commit.

Références distantes contrôlées après publication de l'archive : main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, v1.43 `902861eb1ff34c14dc5ded092581b360767da817`.

## Changements et essai

Gameplay → Horse and riding regroupe le saut. Sons de départ et réception issus des SoundCue du jeu, choix Native jump (varied) / Horse snort (varied) / Off, seulement sur les sauts réussis par le mod et leurs réceptions. Si les assets ne sont pas chargés, indication Waiting et saut toujours disponible ; disponibilité réelle et mixage à confirmer en jeu.

Bras OFF : calcul depuis les matrices du même paquet, suppression de la seconde lecture component-space et de son convertisseur ; pose de repos compatible conservée après cheval/ultime. Aucun socket, projectile, pose native ou autre bras forcé. Le défaut visuel complet reste à juger par l'utilisateur.

Tester sons puis chaque pistolet OFF : viser/tirer après mêlée, dash, saut, roue, cheval et ultime. Comparer Native pour restauration et vérifier les acquis V1.79. [Liste détaillée et limites](V1_80_MOUNTED_SOUNDS_PACKET_ARMS_TEST.md). Coop reportée ; six capacités entre personnages, réception lourde incertaine, suivi vertical des exécutions et autres FX P2 restent ouverts. HUD général en surveillance.

Installation manuelle ; conserver l'INI personnel. Rollback : remettre l'ASI V1.79 avec le même INI, nouvelle clé ignorée. Le README du ZIP correspond à la préparation des sources avant CI ; ce reçu et l'état courant consignent la livraison finale contrôlée. Aucune nouvelle archive créée après le ZIP vérifié.
