# V1.141 — reçu de livraison

**CONSTRUITE / CONTRÔLÉE. Défauts APPROUVÉS ; présentation à valider en jeu.** Base cumulative V1.140. Les états expérimentaux des fonctions restent inchangés.

- Source compilée : 7ae8c8ed240b6877a83362d751714ee872ce9e3b ; archive debug : 89a973bd2c0dcf09ef0148c893d5fba4bd04c3cb.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41332088332) : 70/70 suites, zéro avertissement/erreur MSVC.
- [ZIP direct immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/89a973bd2c0dcf09ef0148c893d5fba4bd04c3cb/downloads/DarksidersGenesis_V1.141_APPROVED_DEFAULTS_MENU_REFRESH_TEST.zip) : 2,440,093 octets ; SHA-256 159C21D0DF9A78B8D209F5CE2ADFF38E90A27DF8D595D15175568705022742E1.
- Quatre fichiers à la racine, CRC valides, stockage sans compression, ASI/proxy AMD64 ; 205 anciennes archives Git identiques.
- 228 clés dont exactement 48 changements approuvés ; profil exhaustif comparé à l'INI du ZIP. Identifiants des 55 Hotkeys inchangés ; nouvelles affectations par défaut intégrées.
- Updater debug : sélection de V1.141, téléchargement HTTP 200 identique. Interface du jeu non observée.
- Defender : ZIP et quatre fichiers sans menace signalée, sans remédiation ni changement de protection.
- Aucune installation, lancement ou promotion main par l'agent. ASI/proxy/INI locaux identiques au début des travaux : **True**. Aucun PAK/sauvegarde modifié.

## Essai

1. Installer via l'updater debug ou manuellement en gardant l'INI personnel.
2. Vérifier la lisibilité des onglets, en-têtes, valeurs, descriptions et états à sa police habituelle ; ouvrir/refermer les groupes et tester une saisie numérique.
3. Vérifier que ses raccourcis/réglages personnels sont conservés après mise à jour.
4. Pour examiner les nouveaux défauts, utiliser un profil séparé ou choisir volontairement le reset après sauvegarde. Aucune remise à zéro automatique : l'INI existant prime.

Nouveaux raccourcis par défaut : F1 trois jauges, F2 munitions, F3 Hotstreak, F4 potions, F5–F11 aucun, F12 HUD ; caméra native sans touches ; rangement L ou LB+L3 ; raccourci manette visée sans tir OFF.

Les tests ImGui réels couvrent 12 combinaisons de police/largeur, le bouton Default, les groupes fermés et la restauration des couleurs/variables. Les tests de configuration vérifient la génération complète, les valeurs approuvées indépendamment du fichier fourni, le reset et la priorité des préférences explicites. Cela ne constitue pas une validation visuelle en jeu.

Rollback : fichiers V1.140, INI personnel conservé. Suite : retour utilisateur sur le menu, puis préparation distincte de la version stable. [Périmètre détaillé](V1_141_APPROVED_DEFAULTS_MENU_REFRESH_TEST.md).
