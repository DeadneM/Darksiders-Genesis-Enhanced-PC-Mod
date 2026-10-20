## V1.71 test — Darksiders Genesis Enhanced PC Mod

Canal expérimental **debug**, base cumulative V1.70 et fonctions validées conservées. Lot groupé : contrôles DOF native/TPS et correction de région V1.70, nouveau candidat **Dash movement recovery** OFF par défaut, observations bornées du défaut de sélection des montures. **CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL** :44/44 suites Windows x64, zéro avertissement MSVC ; ZIP et installation locale contrôlés avec sauvegarde et INI conservé. **Effet en jeu non validé.** [ZIP immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/82a5c678985c5ed397466a37f6ef28fc56ce1bb1/downloads/DarksidersGenesis_V1.71_DASH_MOVE_BATCH_TEST.zip),826278octets, SHA256 `ABE9F9E24C7BBD53E315D12B142E74EE82B74B9FCD608DE924A05E3148310A82`. [Critères et essais cumulés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_71_NATIVE_DASH_MOVE_BATCH_TEST.md).

Le candidat de reprise utilise la cause de mouvement native après la décélération observée et l’arrêt du dash ; pas de fin forcée d’ability, changement de timer, touche ou axe simulé. Checkbox Gameplay → Player et action Hotkeys sans affectation par défaut. Coffres et attaques d’atterrissage exclus. OFF conserve le chemin validé et arrête les prochaines demandes. Les anciens réglages Recovery restent retirés.

Les contrôles DOF ont deux groupes Native/TPS : Disable et Custom distance/intensité, OFF par défaut, curseur et saisie. **Skins chevaux au chargement toujours ouverts** ; les nouvelles traces sont un diagnostic, sans écriture save. **OFF complet pistolet non livré**, modèles gauche/droite validés conservés. Archive : `DarksidersGenesis_V1.71_DASH_MOVE_BATCH_TEST.zip`, quatre fichiers racine.

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
| Action Recovery | Ancienne fin anticipée retirée ; candidat Dash movement recovery V1.71 par permission native après arrêt | OFF par défaut ; bénéfice en jeu à tester, coffres et attaques au sol exclus |
| Modèles 3D des pistolets | Choix indépendant du modèle droit ou gauche par main de Strife | Choix gauche/droite validés ; OFF complet encore ouvert |
| Skins | Deux montures bonus ; six variantes de progression dans deux groupes séparés | Sélection et OFF confirmés V1.68 ; sélection sans PAK confirmée, redémarrage à vérifier |

[État courant](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/CURRENT_STATE.md) et [file active](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ACTIVE_QUEUE.md) font référence pour les éléments restant à tester ou réparer. Une validation utilisateur porte sur l’essai rapporté, sans supposer tous les personnages, chapitres et états coop vérifiés.

### Apparences et durée ultime

Les choix **Native / Right pistol model / Left pistol model** par main de Strife sont conservés. Les holsters montés utilisent la variante rangée correspondante. Le futur OFF complet (modèle, effets et bras, avec projectiles conservés) n’est pas livré dans cette version.

**Bonus mount skins** et **Progression skins** restent indépendants et OFF par défaut. V1.67 interceptait à tort « équipé » ; V1.68 remplace ce branchement par le vrai « débloqué ». Aucun droit DLC, récompense ou bonus de gameplay accordé par le mod. Le jeu garde la sélection native ; fermer et rouvrir la page Skins après un changement. L’essai V1.68 de chevaux/armures et le retour natif OFF sont confirmés sans PAK cosmétique par le journal moteur. Les armures semblent rester ; les montures demandent une nouvelle sélection au chargement malgré Bonus mount skins coché : bug ouvert. Un PAK éventuellement réinstallé peut maintenir des montures disponibles avec l’option ASI OFF.

**Infinite Ultimate duration** bloque seulement le débit de la transformation déjà active, au niveau courant de jauge. Il ne remplit rien et ne rend pas l’activation gratuite. Décocher reprend le multiplicateur de durée si sa case est active, sinon le débit natif. Nouvelle action assignable dans Hotkeys, non attribuée par défaut. **Unlimited Ultimate** conserve son rôle de remplissage.

**Override Leviathan lightning**, décoché par défaut, rend le comportement natif. Une fois coché : **OFF / Effect 1 / Effect 2 / Effect 3**. Visuel seulement, sans accorder le cœur ou son bonus. Si le visuel choisi est indisponible, le menu l’indique et conserve l’effet natif. Pied/cheval/ultime et P2 à tester, ainsi que le retour natif sans doublon.

**TPS character focus** garde la méthode DOF du niveau. En TPS, la couche Gaussian proche est désactivée et la région nette protège la profondeur du modèle ; flou lointain, qualité et ouverture restent natifs. Clé INI `TPSLensFocus` conservée. OFF garde la correction de distance seule. V1.69 remplace le forçage CircleDOF qui rendait les décors trop nets. V1.70 conserve aussi la région nette du niveau tant que la caméra ne la surcharge pas déjà. Dans Camera → Depth of field, Native view et TPS view ont chacun Disable et Custom. Custom remplace explicitement le DOF par Gaussian ; distance0–200m, quantité0–5% absolue (pas un facteur), saisie manuelle et Default. Distance10m et quantité0,6% sont inactives tant que Custom est OFF. Disable est prioritaire et garde les valeurs mémorisées. Le rendu reste à vérifier en jeu.

Léviathan s'attache à la racine comme le natif ; l'ancien rattachement au modèle de Strife était 97 unités plus bas. Hauteur et rangement du menu validés par l’utilisateur dans l’essai V1.69. **Appearance** et **Weapon sheathing** sont repliables et distinctes de **Player movement** ; réglages et raccourcis conservés.

Le HUD après arène verrouillée reste ouvert ; aucun réaffichage global forcé. OFF complet pistolet et nouvelle approche Recovery ouverts. [Essais et rollback V1.69](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_69_NATIVE_VISUAL_FOCUS_TEST.md).

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
