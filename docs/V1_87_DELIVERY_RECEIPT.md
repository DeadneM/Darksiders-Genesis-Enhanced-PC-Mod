# V1.87 — reçu du correctif de crash réticule

23 octobre 2026. Base cumulative V1.86 `7763e99de8817362e69099cc339287ea07732902` ; son interception de réticule est **REJETÉE pour crash**. Nouveautés War/Léviathan conservées sans validation supplémentaire supposée ; acquis des effets de lame V1.85, exécutions V1.82 et bras OFF V1.81 inchangés. Branche debug seulement, installation manuelle, garder son INI.

- Sources compilées : `03398bd4e736a2c01ee96dd764009fb9d67417ea` ; archive : `64b612f3341a1abebf3dc2961de1580a73904089`.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39998309541) : run 39998309541, job 120567525994, success, **52/52 suites**, zéro erreur et zéro avertissement MSVC, analyse native activée. Consommateur de curseur 32 bits compilé dans une unité distincte, tous les modes et retours de menus/focus, passage inchangé des enums, absence de troncature et configuration contrôlés. Avertissements CMake/MinHook et Node d'outillage consignés séparément ; aucun avertissement ASI masqué.
- [ZIP public HTTP200 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/64b612f3341a1abebf3dc2961de1580a73904089/downloads/DarksidersGenesis_V1.87_CURSOR_ENUM_ABI_FIX_TEST.zip) : **905522 octets**, SHA256 `B55E34E80343A7BDF168D1228C7C36B0E964F419F2CA043C5545A91E72A0B978`. Téléchargement identique au fichier local, CRC et architecture AMD64 vérifiés.

| Fichier racine | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1643008 | `EE3EC8EB0CAC96D2D0D17C0D05B9C31FAF277E25D50D1A8BEA403C11386DE1BA` |
| `DarksidersGenesisMod.ini` | 4691 | `99E3EECAD438A6735DD4389D544535FF798BB7F9E9EF0E4E243BC40CC4F13524` |
| `dxgi.dll` | 155136 | `FD01711D382CD2DA31D8D98195D059076D3DAA6D5FFDF07E541B3078870FC1D4` |
| `README.md` | 40357 | `6E710DCCDAB49E543FA2443A3AAB086FB043ADDDB5074FB91B3F186227951D5E` |

## Crash et preuve croisée

Minidump utilisateur : lecture hors tableau à `B10F66`, réponse Slate et paramètre de curseur `0xEDFB8D08`, dont l'octet bas est 8. Le calcul de l'index signé reconstitue exactement l'adresse fautive. Requête native `1ACAA40` : stockage byte, retour entier enum32 (`movzx eax, byte [rbx+551]`). Hook V1.86 déclaré à tort byte ; désassemblage de l'ASI publiée confirme un retour `mov al,8`, les autres bits EAX n'étant pas définis pour le consommateur natif. Les anciens tests lisaient eux aussi un octet.

V1.87 : hook et trampoline `std::int32_t`, signature du résultat natif ajoutée, contrat de type contrôlé à la compilation et consommateur externe qui stocke EAX entier. Comparaison indépendante du nouvel ASI : `mov eax,8` remplace le retour partiel `mov al,8`, et le retour natif est désormais `mov eax,edi`. Analyse des deux binaires conservée avec les contrôles. Aucun nouvel hook, pas d'interception de SetCursor/ShowCursor ni changement d'entrée. La preuve du crash et les contrôles du correctif ne constituent pas encore une validation en jeu. [Détail et contrat public](V1_87_CURSOR_ENUM_ABI_FIX_TEST.md).

## Préservation et essai

**168 clés INI inchangées**, 55 ID Hotkeys inchangés ; **151 archives antérieures intactes**, 152 ZIP au total. **121 anciens fichiers source inchangés**, y compris toutes les fonctions de caméra, tirs, manette, roues, cheval, ressources, effets de lame, bras OFF et pouvoirs. Parties du bras War et de rafraîchissement strictement identiques à V1.86 ; initialisation Léviathan conservée. Fichier principal : seule clarification du log de curseur. README/INI du ZIP correspondent aux sources compilées ; ce reçu complète le README de préparation sans réécrire le ZIP.

Test ciblé : Vanilla puis trois modes de masquage, TPS/vue normale, pause/inventaire et les trois roues, ouvertures/fermetures répétées, menu du mod et Alt-Tab. Curseur disponible dans les menus, masquage selon vue, aucune régression des sticks ou tirs. Nouveau résultat en jeu **non encore validé par l'utilisateur**.

Rapports lus et copiés dans l'analyse privée ; aucun minidump brut ni identifiant de compte publié. Aucun fichier du jeu, INI personnel, PAK ou sauvegarde modifié, aucune relance automatique. Fichier de reprise non suivi conservé. main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` et v1.43 `902861eb1ff34c14dc5ded092581b360767da817` intacts.

Rollback sûr pour ce nouveau module : ASI V1.85 conservé, même INI, avec la régression Léviathan connue. **Vanilla dans V1.86 n'est pas un contournement garanti**, son hook restant présent. Suite après confirmation du correctif : cristal Stoneskin et cinq transferts de pouvoirs ; autres FX P2 reportés, réception lourde incertaine.
