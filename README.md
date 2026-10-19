## V1.66 — Darksiders Genesis Enhanced PC Mod

Mod Windows x64 pour Darksiders Genesis : caméra native/TPS, souris et manette, coop locale, réglages de gameplay, raccourcis et mise à jour intégrée. **Canal expérimental `debug`** ; la branche stable `main` et les anciennes archives sont conservées.

**Dernier lot : CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL.** V1.66 teste le cycle des sauts supplémentaires avec animation et réduit l’attente initiale des modèles de holster en anticipant leur recherche à pied. Retour utilisateur encore attendu. Base V1.65 : placement des holsters, trois délais de rangement et durée ultime validés ; ancien double saut défectueux.

[Télécharger le ZIP cumulatif V1.66](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/12061a956b9257284c8b1779503cf058b75fab1f/downloads/DarksidersGenesis_V1.66_NATIVE_JUMP_CYCLE_TEST.zip) · [Contrôles Windows : 41/41 suites, aucun avertissement MSVC](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39217494013) · [Notes et tests V1.66](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_66_NATIVE_JUMP_CYCLE_TEST.md).

Le ZIP vérifié contient exactement quatre fichiers à la racine : `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`. Taille : 934413 octets. SHA256 : `CC15584E210717E4F173967BE47024CA949983FD24870B03190191936888F67F`. La compilation et l’intégrité du paquet ne valident pas le comportement en jeu.

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
| Rangement | Délais séparés lames Strife / pistolets Strife / épée War ; action commune avec raccourcis clavier et manette | Validés ; holsters montés validés V1.65, délai initial à revérifier en V1.66 |
| Sauts supplémentaires | OFF / After gliding / Always on | Expérimental V1.66 ; aucun saut animé répétable encore validé |
| Action Recovery | Ancienne fin anticipée retirée depuis V1.58 | Pas d’option active ; nouvelle approche en étude |
| Modèles 3D des pistolets | Inventaire des représentations et de leurs contraintes | Pas de remplacement alternatif livré |

[État courant](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/CURRENT_STATE.md) et [file active](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ACTIVE_QUEUE.md) font référence pour les éléments restant à tester ou réparer. Une validation utilisateur porte sur l’essai rapporté, sans supposer tous les personnages, chapitres et états coop vérifiés.

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

### Essai V1.66 et retour arrière

1. Always : au moins dix nouveaux sauts **avec animation**, en TPS puis vue native ; tester aussi quelques appuis rapprochés.
2. After gliding : un saut rendu par nouveau plané, y compris demandé pendant le plané ; contrôler OFF et le retour au sol.
3. Holsters : rester quelques secondes à pied avant la première montée ; vérifier délai, alignement, tir, rangement et descente.

Si les sauts restent sans animation ou se bloquent, remettre cette option OFF et conserver le journal avec le nombre de sauts observé. Pour revenir à V1.65, utiliser le système de restauration ou son archive conservée, en gardant l’INI personnel. Les modèles montés et les autres fonctions validées restent distincts de l’essai des sauts.

### Sources, construction et historique

Source C++20/CMake, Windows x64 ; dépendances MinHook et Dear ImGui fixées par le projet. Le workflow [.github/workflows/build-v090-debug.yml](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/.github/workflows/build-v090-debug.yml) configure, compile, teste et préserve une nouvelle archive numérotée sur `debug`. Une archive publiée n’est jamais remplacée par des octets différents. La release stable n’est pas promue par une simple réussite de CI.

[Carnet technique cumulatif](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/TECHNICAL_NOTEBOOK.md) · [Prochaines étapes](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ROADMAP.md) · [Audit Recovery et modèles](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/NEXT_RECOVERY_AND_MODELS.md) · [Règles du projet](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/DEVELOPMENT_RULES.md).

Le README actuel sépare les informations actives des anciennes préparations. **L’ancien README intégral est préservé, octet pour octet, dans [l’historique jusqu’à V1.66](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/history/README.md)**. Les anciennes affirmations de statut et anciens raccourcis de cet historique ne décrivent pas la version actuelle. Les anciens ZIP restent inchangés.
