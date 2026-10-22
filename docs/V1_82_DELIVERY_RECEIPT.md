# V1.82 — reçu de livraison contrôlé

22 octobre 2026. **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE**. Base V1.81 validée par l'utilisateur dans les trois essais rapportés ; nouveaux effets de mêlée et suivi d'exécution haute à confirmer visuellement. Aucune validation coop supposée.

- Base cumulative : `adb5ae898f5ebab45ae0f11b6bd1c0cda7fb165f`, debug uniquement.
- Sources compilées : `6a26b76c6c39c0138022697ae0f859f2310454f1`.
- Archive immuable : `03a45347f8484ca4af9fa17cf5b49d3e66062cf9`.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39834733781) : run `39834733781`, job `120019937953`, success, **50/50 suites**, zéro erreur et zéro avertissement MSVC. Nouvelles suites ExecutionCamera et MeleeEffect réussies.
- Avertissements d'outillage inchangés : minimum CMake ancien de MinHook, Node.js 20 forcé vers 24, dépréciations punycode/url.parse des actions. Aucun avertissement du code ASI masqué.
- [ZIP public contrôlé, HTTP 200](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/03a45347f8484ca4af9fa17cf5b49d3e66062cf9/downloads/DarksidersGenesis_V1.82_BARE_BLADE_EXECUTION_CAMERA_TEST.zip) : **888346 octets**, SHA256 `58FF7DD24DB2AAF7AA3943CACF2AB92F2C6EB05BCFF58C17C7250519474FC36B`. Octets du téléchargement public identiques à l'archive locale.

## Paquet et acquis

Quatre fichiers racine exactement, CRC valides, ASI et dxgi AMD64, version V1.82 retrouvée. README et INI comparés au commit compilé.

| Fichier | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1608704 | `773E8D37D04BBFCE66F3D032799D4E622A1D149380F36F5F80D7A91597A3E023` |
| `DarksidersGenesisMod.ini` | 4647 | `03D37FB4E6D4C21DF3BE5596FA9451EEEECE21B3A7FF276A8027C49161B30B22` |
| `dxgi.dll` | 155136 | `3B8226C9E0A67F1CD332767A7889C53B698844B3693DCE779DFF5FEBCC6E84F5` |
| `README.md` | 28237 | `B34A812143CB501312D0E7F20A5715E0CD22B7143867A9F109B8FF5725672FA0` |

Les **146 anciennes archives sont inchangées**, 147 ZIP au total. **166 valeurs INI et 55 ID Hotkeys conservés**, aucune nouvelle clé. **108 anciens fichiers source inchangés**, dont les bras OFF animés V1.81, modèles/holsters, cheval/saut/sons, trajectoires/World Ender, souris/manette/roues et ressources. Le chemin caméra ajoute uniquement un décalage vertical à la sortie TPS ; la notification de particules existante ouvre un contexte de mêlée après ses filtres habituels. Un hook facultatif nouveau du spawn de particule, pas de nouveau hook caméra/input. Signatures réelles et EXE contrôlés ; originaux et ABI conservés.

Références distantes protégées : main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, v1.43 `902861eb1ff34c14dc5ded092581b360767da817`. Aucun fichier du jeu, INI personnel, PAK ou sauvegarde remplacé ; aucune relance ni écriture de mémoire externe. Le fichier de reprise non suivi reste intact et hors commit.

## Résultat contrôlé et limites

Effets des lames cachées : copie du transform de l'effet autorisé, origine à la main, taille ×0,4 ; traces de dégâts et portée inchangées. Les arcs communs restent natifs avec une seule épée visible. Pas de prétention de couverture universelle des effets.

Exécutions : type natif finisher possédé, bassin animé et baseline propre à chaque joueur. Le suivi n'agit que sur la hausse supérieure à 40 unités. **Relecture hors jeu des 211 lignes de finisher capturées au sol : hausse maximale de 31,5055 unités, décalage caméra candidat maximal zéro.** Trois baselines d'entrée : −28,532 / −20,750 / −21,854. Cette simulation complète les fixtures ; elle ne valide pas le comportement en jeu. L'utilisateur confirme que ces exécutions étaient au sol ; aucune exécution haute de Strife ou War observée. [Contrats et contrôles](evidence/V1_82_PRESENTATION_CONTRACT.json).

Test : lames gauche/droite/deux cachées, combo et attaques aériennes, effets des côtés encore visibles ; exécution au sol puis réellement haute en TPS, retour de cadrage et visée normale ; contrôle court des bras OFF, World Ender, roues, cheval et ultime. [Essai groupé et détails](V1_82_BARE_BLADE_EXECUTION_CAMERA_TEST.md).

Les six capacités étrangères restent ouvertes : montage dynamique et statuts Stoneskin étudiés, dont un nettoyage de matériaux explicitement lié à War ; aucun adaptateur incomplet ou choix étranger présenté fonctionnel. Réception lourde encore incertaine ; autres FX P2 reportés, HUD général en surveillance. [Étude et prochaine étape](V1_82_FOREIGN_WRATH_RESEARCH.md).

Installation manuelle, garder l'INI personnel. Rollback : ASI du ZIP V1.81 conservé, même INI. Le README inclus dans le ZIP reflète sa préparation avant CI ; ce reçu consigne la livraison contrôlée sans modifier l'archive numérotée.
