## Installation locale V1.142 — 1er novembre 2026

**INSTALLÉE à la demande explicite de l'utilisateur, jeu fermé ; empreintes ASI/proxy conformes au ZIP contrôlé.** Sauvegarde V1.141 et INI dans le dossier local before-V1.142-20261101-231817-8451d5. INI personnel/raccourcis, README local, EXE original, Engine.ini et inventaire PAK conservés. Aucun lancement du jeu ni validation du menu déduit de la copie. [Preuve d'installation](evidence/V1_142_INSTALL_CHECK.json).

---

# V1.142 — reçu de livraison

**CONSTRUITE / CONTRÔLÉE ; à valider en jeu.** Base V1.141 acceptée par l'utilisateur, défauts et gameplay conservés. Accès aim assist standardisé, distribution binaire simplifiée.

- Source : 87c72f732a14957ef59eb552cdc53ab2b7644b4e ; archive debug : 78229baae6258fa8bb51750a2f4134d6604cfb63.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41336835208) : 70/70 suites, zéro avertissement/erreur MSVC. Comprend les tests des formats ZIP, refus des entrées invalides, sauvegardes anciennes/nouvelles et conservation des préférences.
- [ZIP immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/78229baae6258fa8bb51750a2f4134d6604cfb63/downloads/DarksidersGenesis_V1.142_AIM_ASSIST_MENU_BINARY_PACKAGE_TEST.zip) : 2,276,590 octets ; SHA-256 95453271B56BB172F4234B1FE43A51F24BF18AB620C24550A94916E378C9A4E9.
- Exactement DarksidersGenesisMod.asi et dxgi.dll à la racine. Aucun INI, README ou dossier ; CRC et AMD64 contrôlés, stockage sans compression.
- 206 anciennes archives Git identiques ; main/archive et tag v1.43 inchangés. Aucune archive autonome supplémentaire publiée.
- Updater debug : archive sélectionnée et téléchargement HTTP 200 identique. Son ancien moteur V1.141 ne peut pas installer ce format.
- Defender : ZIP et deux binaires sans menace signalée, sans remédiation ni changement de protection.
- Aucune installation/lancement par l'agent ; binaires et INI locaux identiques au début : **True**.

## Passage manuel accepté

Fermer complètement le jeu. Sauvegarder les deux binaires V1.141, puis extraire les deux fichiers du ZIP dans ProjectMayhem/Binaries/Win64. Conserver son INI : aucun nouveau fichier de configuration n'est livré. S'il est absent, l'ASI crée ses défauts validés au lancement.

L'updater embarqué dans V1.142 gère ensuite les nouveaux ZIP à deux fichiers et les anciennes archives à quatre. Il remplace/restaure seulement la paire binaire. Pas de nouvel exécutable autonome à installer.

## À vérifier

Dans les contrôles TPS, vérifier la case TPS aim assist puis les champs Global/Per ammunition immédiatement visibles lorsqu'elle est ON ; OFF/ON doit conserver les valeurs. Vérifier ses raccourcis et paramètres personnels et utiliser Check for Updates. Le prochain remplacement réel via updater restera à observer jeu fermé ; les tests de fichiers n'en constituent pas une validation réelle.

Rollback : remettre les deux binaires V1.141 en conservant l'INI. README/carnet/reçus restent sur GitHub, liés aux commits de construction. [Périmètre et garde-fous](V1_142_AIM_ASSIST_MENU_BINARY_PACKAGE_TEST.md).
