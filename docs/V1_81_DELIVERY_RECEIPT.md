# V1.81 — reçu de livraison contrôlé

22 octobre 2026. **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE**, bras animés non validés en jeu. Le cheval V1.80 est confirmé par l'utilisateur dans son essai et son code reste inchangé. Les fixtures et la compilation ne prouvent pas le rendu visuel.

- Base cumulative V1.80 : `3533b3d070a6c41b7976203c7d3e2e27501c9dd3`, debug seulement.
- Sources compilées : `08499a5a62513143986e282127f2775954acbc6c`.
- Archive immuable : `36d31209abfbf4c0fea6b5afd64184855f1f964b`.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39803846206) : run `39803846206`, job `119911675567`, success, **48/48 suites**, zéro erreur et zéro avertissement MSVC.
- Avertissements d'outillage toujours visibles : minimum CMake ancien dans MinHook, Node.js 20 forcé vers 24, dépréciations punycode et url.parse des actions. Aucun avertissement du code ASI masqué.
- [ZIP public contrôlé, HTTP 200](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/36d31209abfbf4c0fea6b5afd64184855f1f964b/downloads/DarksidersGenesis_V1.81_NATIVE_UNARMED_ANIMATION_TEST.zip) : **877586 octets**, SHA256 `7B046F24B43F19B74A2A4C18E4B5D1DB4DDA7C4B399434BCF13572ABBC9B5C99`. Téléchargement public comparé octet pour octet à l'archive locale.

## Paquet et préservation

Quatre fichiers racine exactement, CRC valides, ASI et dxgi AMD64, marqueur V1.81 retrouvé. README et INI comparés au commit compilé.

| Fichier | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1590272 | `6A5947F4ACC0D40C0C152441E1C241F2116C00C4652857F79FC14B72B6F826F0` |
| `DarksidersGenesisMod.ini` | 4647 | `03D37FB4E6D4C21DF3BE5596FA9451EEEECE21B3A7FF276A8027C49161B30B22` |
| `dxgi.dll` | 155136 | `9AD572BE64C85661B56337D4D013C6F9C1CC1C286503F8507A626B75470A8492` |
| `README.md` | 26175 | `1AC186926338C13E62B7BFD9D865A0542F2B0B565A1D0B62B8196EB3FF078FEC` |

Les **145 archives précédentes sont inchangées**, 146 ZIP au total. Les **166 valeurs INI et 55 ID d'actions Hotkeys sont conservés**, aucune nouvelle clé. 108 fichiers source existants inchangés. Un nouveau hook facultatif de racine d'animation, signatures/EXE/identités/proxy/mapping exacts, est ajouté pour le seul échantillonnage privé du bras OFF ; tous les originaux sont conservés. Les chemins cheval/sons, tir/visée/caméra, World Ender, XInput/roues, dash et ressources restent inchangés.

Références distantes contrôlées après publication : main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, v1.43 `902861eb1ff34c14dc5ded092581b360767da817`.

Aucun fichier du jeu, INI personnel, PAK ou sauvegarde remplacé ; aucune relance et aucune écriture externe de mémoire. Le fichier non suivi REPRISE_AUDIT_2026_10_13.md reste intact et hors commit. Les diagnostics privés ne sont pas livrés dans le ZIP.

## Changement, limites et test

Le candidat remplace la capture immobile et la conservation d'un repos figé par les clips désarmés natifs Idle/Walk/Run, échantillonnés dans un contexte privé puis transférés à la seule branche de rendu OFF. Ancienne attente de capture et ancien timestamp de rendu inutilisé supprimés. Aucune rotation des points de tir ni modification de pose/socket de gameplay. Modèles/holsters et effets connus du côté OFF restent indépendants de la disponibilité de cette animation.

Critère : bras détendu mais animé pendant visée/tir/déplacement, à droite comme à gauche, avec mêlée et pouvoirs natifs puis reprise désarmée. [Essais et contrats détaillés](V1_81_NATIVE_UNARMED_ANIMATION_TEST.md). Tester les côtés séparés puis deux OFF, Native, mêlée/pouvoirs, roues, cheval et ultime. Pas besoin de ranger puis attendre pour enregistrer une pose. Mapping autre que les 80 os observés, instance/échantillon périmés ou action inconnue : bras natif. Transitions et phase de marche, autres skins et coop restent à vérifier.

Installation manuelle, conserver son INI. Rollback : ASI V1.80 déjà archivé, même INI. Les six capacités étrangères, réception lourde et suivi vertical des exécutions restent ouverts ; autres FX P2 reportés, HUD général en surveillance. Aucun autre comportement déclaré réparé par ce lot. Le README inclus reflète la préparation avant CI ; ce reçu consigne la livraison finale contrôlée sans modifier l'archive numérotée.
