## V1.67 test — Darksiders Genesis Enhanced PC Mod

Mod Windows x64 pour Darksiders Genesis : caméra native/TPS, souris et manette, coop locale, réglages de gameplay, raccourcis et mise à jour intégrée. **Canal expérimental `debug`** ; la branche stable `main` et les anciennes archives sont conservées.

**Nouveau lot V1.67 EXPÉRIMENTAL : pistolet droit/gauche au choix pour chaque main de Strife ; montures bonus et apparences de progression proposées séparément dans la page Skins native. Nouveaux comportements à valider dans le jeu.** Base retenue V1.66, sauts et holsters validés par l’utilisateur le 19 octobre 2026 ; acquis conservés, canal `debug`. [Preuves, inventaire et essais V1.67](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_67_PISTOL_CHOICES_AND_SKINS.md).

[Télécharger le ZIP cumulatif V1.67](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/c760123057e66f2b3d7053b7acf3441de2cee44e/downloads/DarksidersGenesis_V1.67_PISTOL_CHOICES_SKINS_TEST.zip) · [Contrôles Windows : 42/42 suites, aucun avertissement MSVC](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39253436601) · [Inventaire et essais V1.67](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_67_PISTOL_CHOICES_AND_SKINS.md).

Le ZIP vérifié contient exactement quatre fichiers à la racine : `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`. Taille : 809061 octets. SHA256 : `8E37AE2BBBAD9465BEFE2BC75F2566B74311EB0908E01682D1D6CD02EF3B9591`. **CONSTRUIT / CONTRÔLÉ ; nouveaux comportements EXPÉRIMENTAUX, non validés en jeu.** La compilation et l’intégrité du paquet ne valident pas le comportement en jeu.

### Installation et mise à jour

Placer les quatre fichiers à côté de `DarksidersGenesis-Win64-Shipping.exe`, dans `ProjectMayhem/Binaries/Win64`. Sauvegarder les fichiers déjà présents. Pour mettre à jour une installation existante, **conserver l’INI personnel et ses raccourcis** ; les réglages manquants prennent leur valeur par défaut compilée. Fermer le jeu avant de remplacer les fichiers chargés.

Le menu s’ouvre avec **Inser** dans le profil fourni ; cette touche est reconfigurable. Onglets : **Gameplay**, **Camera**, **Hotkeys**, **Updates**, **About**. Les valeurs numériques disposent des contrôles indiqués dans le menu ; utiliser les descriptions de chaque option pour son unité et le sens de son facteur.

Dans **Updates**, choisir le canal voulu, puis vérifier les mises à jour. Le changement de canal seul n’installe rien. L’installation à la fermeture et la restauration utilisent le système de sauvegarde intégré. **Auto check update**, facultatif et désactivé par défaut, vérifie au démarrage le canal choisi ; une mise à jour trouvée produit un avis temporaire puis un rappel jaune dans le menu. Son comportement en jeu attend encore un retour explicite.

### Fonctions et état

| Groupe | Contenu | État à retenir |
| --- | --- | --- |
| Caméra et commandes | TPS/native, souris relative, manette, échange des sticks, FOV coop, profils pied/cheval/ultime, cinématiques et roues | Chemins acquis conservés ; réglages et commandes reconfigurables |
| Visée et dash | Visée souris/manette, trajectoires, reprise AIM et retrait des tirs/sons/effets Standard parasites pendant le dash | Corrections confirmées dans les essais utilisateur, conservées |
| Netteté et interaction HUD | Focus TPS, taille et orientation des indications d’interaction P1/P2 | Validés dans les essais rapportés ; autres icônes/effets P2 reportés |
| Ressources | Gains, remplissages et Unlimited ; munitions, Hotstreak, vie, potions, ultime ; Standard limité | Fonctions validées conservées ; facteurs et descriptions propres à chaque réglage |
| Mouvement et grappin | Réglages de déplacement/saut/plané/cheval, sprint, portée native du grappin | Acquis conservés ; portée validée dans l’essai utilisateur |
| Rangement | Délais séparés lames Strife / pistolets Strife / épée War ; action commune avec raccourcis clavier et manette | Délais validés V1.65 ; holsters validés dans l’essai V1.66 |
| Sauts supplémentaires | OFF / After gliding / Always on | Sauts validés dans l’essai utilisateur V1.66 |
| Action Recovery | Ancienne fin anticipée retirée depuis V1.58 | Pas d’option active ; nouvelle approche en étude |
| Modèles 3D des pistolets | Choix indépendant du modèle droit ou gauche par main de Strife | Expérimental V1.67 ; aucun asset supplémentaire |
| Skins | Deux montures bonus ; six variantes de progression dans deux groupes séparés | Expérimental V1.67 ; disponibilité cosmétique native, aucun déblocage de récompense écrit |

[État courant](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/CURRENT_STATE.md) et [file active](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ACTIVE_QUEUE.md) font référence pour les éléments restant à tester ou réparer. Une validation utilisateur porte sur l’essai rapporté, sans supposer tous les personnages, chapitres et états coop vérifiés.

### Apparences V1.67

Dans Gameplay, section Appearance, choisir **Native / Right pistol model / Left pistol model** indépendamment pour la main droite et la main gauche. Les holsters montés utilisent le modèle rangé correspondant. Les choix concernent Strife des deux joueurs locaux ; prise en main, effets et pose restent expérimentaux. Native conserve l’apparence d’origine de chaque main.

Deux cases distinctes, OFF par défaut : **Bonus mount skins** pour Rampage (War) et Despair (Strife) ; **Progression skins** pour les variantes Abyssal, Silver et Gold de chacun. Cela propose les apparences dans la page Skins du jeu, sans récompense attribuée par le mod. Le jeu garde la sélection et peut mémoriser l’apparence équipée. Le PAK de montures existant n’est ni remplacé ni nécessaire au nouveau contrôle ASI ; s’il reste installé, OFF ne retire pas son déblocage natif. Choisir une apparence possédée avant de désactiver un groupe.

Essais groupés : deux pistolets identiques puis inversés à pied/cheval, tirs/rangement, retour Native ; fermer et rouvrir la page Skins après chaque changement, vérifier les groupes, retour OFF et comparaison sans le seul PAK de montures pour prouver l’autonomie de l’ASI. L’INI personnel est prioritaire ; les quatre nouveaux réglages sont Native/Native et OFF/OFF par défaut. Pour retirer le lot, reprendre le ZIP V1.66 en conservant l’INI.

### Raccourcis du profil fourni

Toutes les affectations suivantes sont reconfigurables et l’INI personnel reste prioritaire.

| Touches | Actions fournies |
| --- | --- |
| F1–F4 | Remplir toutes les jauges / Ultime / Synergie / Courroux |
| F5–F8 | Unlimited toutes les jauges / Ultime / Synergie / Courroux |
| F9 | Unlimited munitions |
| F10–F11 | Non attribuées |
| F12 | Afficher/masquer le HUD |

Les commandes de caméra native et de TPS ont leurs sections dédiées ; elles ne remplacent pas la configuration rebindable du jeu. Le rangement dispose aussi de raccourcis directement dans Gameplay, en plus des actions Hotkeys. **Les raccourcis de rangement dédiés sont non attribués par défaut.**

### Validation V1.66 et retour arrière

Retour explicite du 19 octobre 2026 : « pour les sauts c'est bon bravo », « pour les holsters c'est bon bravo », puis « donc tout a lair bon/ ». Ces deux sujets sont clos dans l’essai rapporté ; V1.66 devient la base de travail retenue. Ce retour ne décrit pas une matrice exhaustive de modes, personnages ou situations coop et ne confirme pas séparément Auto check update.

Le code validé est conservé. En cas de nouvelle régression, noter le contexte et garder le journal ; le système de restauration et les anciennes archives permettent un retour arrière en préservant l’INI personnel. Le [ZIP V1.66 conservé](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/12061a956b9257284c8b1779503cf058b75fab1f/downloads/DarksidersGenesis_V1.66_NATIVE_JUMP_CYCLE_TEST.zip) permet de revenir à cette base en conservant l’INI personnel.

### Sources, construction et historique

Source C++20/CMake, Windows x64 ; dépendances MinHook et Dear ImGui fixées par le projet. Le workflow [.github/workflows/build-v090-debug.yml](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/.github/workflows/build-v090-debug.yml) configure, compile, teste et préserve une nouvelle archive numérotée sur `debug`. Une archive publiée n’est jamais remplacée par des octets différents. La release stable n’est pas promue par une simple réussite de CI.

[Carnet technique cumulatif](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/TECHNICAL_NOTEBOOK.md) · [Prochaines étapes](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ROADMAP.md) · [Audit Recovery et modèles](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/NEXT_RECOVERY_AND_MODELS.md) · [Règles du projet](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/DEVELOPMENT_RULES.md).

Le README actuel sépare les informations actives des anciennes préparations. **L’ancien README intégral est préservé, octet pour octet, dans [l’historique jusqu’à V1.66](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/history/README.md)**. Les anciennes affirmations de statut et anciens raccourcis de cet historique ne décrivent pas la version actuelle. Les anciens ZIP restent inchangés.
