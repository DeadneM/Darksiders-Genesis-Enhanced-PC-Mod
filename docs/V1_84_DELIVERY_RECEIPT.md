# V1.84 — reçu de livraison contrôlée

22 octobre 2026. **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE**, effets corrigés à tester visuellement. Base cumulative V1.83 `81d61dd4fd8e44b83c652db12081e80094a8b609`, acquis validés V1.81 et suivi TPS des exécutions V1.82 conservés. Aucun résultat coop supplémentaire supposé.

- Sources compilées : `0382cbee93a52f6df5270f616b2f774a28da35e4` ; archive : `c58304fd20466e173e0960ca09e47b399455aa7a`.
- [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39844030589) : run 39844030589, job 120052417242, success, **51/51 suites**, zéro erreur et **un avertissement C4324** : Plan rembourré pour aligner Transform. Réordonner les champs et utiliser un indicateur de 32 bits donnera 128 octets sans padding implicite ; nouvelle compilation V1.85, aucun pragma de masquage. Tests des neuf arguments du spawn attaché, origine main sous rotation/échelle non uniforme, P2 possédé, refus des contextes incompatibles et chemin détaché réussis.
- Avertissements d'outillage inchangés : minimum CMake MinHook ancien, Node20 forcé24 et dépréciations punycode/url.parse. Aucun avertissement du code ASI masqué.
- [ZIP public HTTP200 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/c58304fd20466e173e0960ca09e47b399455aa7a/downloads/DarksidersGenesis_V1.84_ATTACHED_BARE_BLADE_FX_TEST.zip) : **898679 octets**, SHA256 `66A021510E3C961DF36B6C33F1BF28F877E887360BBFBE3CFA868DC1308794EE`. Téléchargement public identique à l'archive locale.

| Fichier racine | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1633280 | `3D1D1EA986A73B14B81649AA43E2A8DB1E614B0A047B3451A022F56A381F796D` |
| `DarksidersGenesisMod.ini` | 4647 | `03D37FB4E6D4C21DF3BE5596FA9451EEEECE21B3A7FF276A8027C49161B30B22` |
| `dxgi.dll` | 155136 | `3C335544B6A2FD44DCA6BBF8548841607B71C266D155FA14CDC2C93604A25C34` |
| `README.md` | 33358 | `9129328245AA7ABA7282A08DBFD4C863774D05A1D38EE4EDEE11485ADE13F2D8` |

Quatre fichiers racine exactement, CRC valides, ASI/dxgi AMD64, version et fichiers README/INI comparés aux sources compilées. **148 anciennes archives inchangées**, 149 ZIP au total. **166 valeurs INI et 55 ID Hotkeys conservés**. **119 anciens fichiers source inchangés**, dont toutes les caméras/exécutions, bras/pistolets/holsters, tirs/World Ender, inputs/roues, cheval et ressources. Le prototype Stoneskin est inchangé.

## Cause et correctif

L'utilisateur confirme que le suivi des exécutions TPS V1.82 est parfait dans son essai, tandis que les effets de lames masquées ne changent pas. Cette validation du suivi n'est pas étendue à tous les contextes ou à la coop.

Le relevé VM_READ trouve huit notifications slash/stab attachées à Bone_Strife_Root, exclues par V1.82, et six notifications détachées du troisième coup plus six dans sa variante `_03_b`. V1.84 couvre le spawn attaché et reconnaît cette variante. Le hook supplémentaire `189A510` ne change que des copies de position/échelle dans le contexte exact de notification et de propriétaire. La position mondiale de la main est exprimée dans le repère de la racine ; parent, socket, orientation, durée et pooling natifs restent conservés. Les signatures réelles des trois fonctions et le contrat des mains sont contrôlés. Aucun asset/notify partagé, dégâts, portée ou animation modifié.

Arcs communs : ajustés uniquement si les deux épées sont masquées. Les autres particules ne sont pas couvertes universellement. Le contrat de callback animation=asset propriétaire reste à confirmer par le passage en jeu : les lectures d'objets et fixtures ne le prouvent pas. [Détails et essais](V1_84_ATTACHED_BARE_BLADE_FX_TEST.md).

## Capacités et suite

Inclus : prototype V1.83 de protection Stoneskin vers Strife normal à pied, dans un slot débloqué, avec coût natif et reprise des contrôles. **Partiel / non validé en jeu** : onde de choc de lancement, sons et apparence de pierre non transférés. Les cinq autres transferts restent ouverts. [Limites V1.83](V1_83_DELIVERY_RECEIPT.md).

Tester lames Native/gauche/droite/deux masquées, combo au sol et aérien, puis contrôle court des bras, roues et exécutions. Stoneskin peut être essayé séparément : vérifier coût/protection/expiration et retour Native, Unlimited Health/Wrath désactivés et variantes Molten retirées pour comparer la protection de base. Ne pas attendre une aura visible de ce prototype.

main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` et v1.43 `902861eb1ff34c14dc5ded092581b360767da817` conservés. Fichier de reprise non suivi intact et hors commit. Aucun fichier de jeu installé, INI/PAK/sauvegardes personnels conservés, aucune relance ni écriture externe de mémoire. Installation manuelle ; rollback ASI V1.82 conservé pour le dernier suivi d'exécutions validé, même INI. README emballé figé à sa préparation ; ce reçu ajoute les résultats sans réécrire l'archive.

Suite : retour visuel des arcs, compléter Stoneskin puis Caltrops vers War. Réception lourde incertaine, FX P2 reportés, HUD général non reproduit.
