## V1.77 test — Darksiders Genesis Enhanced PC Mod

Lot cumulatif debug **CONSTRUIT / CONTRÔLÉ : 47/47 suites Windows x64, zéro avertissement MSVC**. Nouveau panneau Gameplay → **Wrath ability slots** : trois choix indépendants par personnage, parmi ses trois capacités natives. Tous les choix fournis valent Native. Le jeu conserve lancement, déverrouillage, coût et animations. **EXPÉRIMENTAL, non validé en jeu.**

**L'échange libre des six capacités reste à terminer.** Les pouvoirs étrangers nécessitent l'adaptation des animations et composants. Les indications de roue peuvent encore décrire l'emplacement initial. Candidats visée sans tir/réception lourde V1.75 et OFF à pied V1.76 repris sans modification, toujours à tester. Base validée V1.73 dans les essais rapportés.

[Télécharger le ZIP V1.77 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b5da4721104b0b5a08911bf8cc51af88a4559f6f/downloads/DarksidersGenesis_V1.77_NATIVE_WRATH_SLOTS_TEST.zip) · [Reçu et intégrité](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_77_DELIVERY_RECEIPT.md) · [Essais groupés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_77_NATIVE_WRATH_SLOTS_TEST.md). Installation manuelle ultérieure, garder l'INI personnel et ses raccourcis ; les six nouvelles clés absentes restent Native. Aucun fichier du jeu remplacé par l'agent. Les sections suivantes décrivent les versions précédentes et leurs statuts historiques.

---

## V1.76 test — Darksiders Genesis Enhanced PC Mod

Lot cumulatif debug, **CONSTRUIT / CONTRÔLÉ : 46/46 suites Windows x64, zéro avertissement MSVC**. Nouvelle option **OFF (foot, experimental)** par pistolet de Strife dans Gameplay → Appearance. **Non validée en jeu.** Reprend la visée manette sans tir et le Recovery de réception lourde V1.75, encore à tester. Base validée V1.73 dans les essais rapportés.

Pour OFF à pied : ranger les armes puis rester immobile brièvement ; le menu indique quand la pose est prête. Le candidat masque le pistolet choisi et repose son bras pendant la visée, avec tirs actifs. Retour Native réversible. Mêlée, cheval et ultime restent natifs. Les effets connus propres à une main sont filtrés ; certains effets communs restent avec un pistolet visible, puis sont retirés avec les deux OFF. **OFF universel des effets/formes encore ouvert**, ainsi que l'échange libre des six capacités.

[Télécharger le ZIP V1.76 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/026664c3e06a85b3714af5591ff3bef135ff2cb1/downloads/DarksidersGenesis_V1.76_FOOT_PISTOL_OFF_TEST.zip) · [Reçu et intégrité](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_76_DELIVERY_RECEIPT.md) · [Essais groupés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_76_FOOT_PISTOL_OFF_TEST.md). Installation manuelle ultérieure : conserver l'INI personnel et ses raccourcis. Aucun fichier du jeu remplacé par l'agent. Les sections suivantes décrivent les bases précédentes et leurs statuts historiques.

---

## V1.75 test — Darksiders Genesis Enhanced PC Mod

Lot cumulatif **debug** sur la base V1.73 validée dans les essais rapportés : **visée sans tir à la manette** avec un ou deux boutons configurables, et candidat **Heavy landing movement recovery** pour reprendre le déplacement après une réception lourde automatique. Version **CONSTRUITE / CONTRÔLÉE**, 45/45 suites Windows x64, zéro avertissement MSVC. Les deux nouveautés sont **EXPÉRIMENTALES**, OFF par défaut et non validées en jeu. [ZIP vérifié, intégrité et reçu de livraison](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_75_DELIVERY_RECEIPT.md). [Objectif et essais groupés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_75_PAD_AIM_HEAVY_LANDING_TEST.md).

Gameplay → **Aim without firing** : un champ OFF laisse l'autre bouton fonctionner seul ; deux entrées actives nécessitent leur maintien ensemble ; OFF/OFF désactive la fonction. Le maintien appelle l'action native, sans simuler Ctrl ou ajouter de tir. En TPS, il suit la caméra du joueur ; chaque manette concerne son propre joueur. Les affectations natives restent actives. Relâcher après menu, changement de personnage ou reconnexion avant de viser à nouveau.

Gameplay → Player movement → **Heavy landing movement recovery (experimental)** : ouvre les deux verrous de déplacement après l'impact d'une chute assez haute, sans forcer l'arrêt de l'animation. Ce candidat vise seulement les HeavyLand normaux de Strife/War ; dash, attaques, Rampage, transformations et changements de personnage restent conservés. L'efficacité réelle et les effets doivent être vérifiés en jeu ; trois réceptions Strife observées, pas de War_HeavyLand dans la capture.

**L'échange libre des six capacités et le OFF complet par pistolet restent en étude et ne sont pas annoncés fonctionnels.** Les dépendances de squelettes, composants et rendu sont documentées. [File restante](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ACTIVE_QUEUE.md). Installation manuelle par l'utilisateur à la fin de sa session, INI personnel et raccourcis à conserver. Aucun fichier du jeu remplacé pendant la préparation.

---

### Acquis de la base V1.73

Canal **debug**, lot cumulatif : maintien de Despair/Rampage au chargement, distance DOF native/TPS jusqu'à **5 000 m**, nettoyage du Recovery du dash signalé sans effet. **CONSTRUIT / CONTRÔLÉ / INSTALLÉ** ; trois essais V1.73 confirmés par l’utilisateur : 43/43 suites Windows x64, zéro avertissement MSVC ; installation avec sauvegarde et INI personnel conservé. Chargement des montures, réglages DOF et contrôle visée/tir/dash confirmés dans les essais V1.73 rapportés. [ZIP immuable contrôlé](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/516db3d8cd898e9d544c571a2ba73fdfa38d070b/downloads/DarksidersGenesis_V1.73_MOUNT_DOF_CLEANUP_TEST.zip). [Critères et essais groupés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_73_MOUNT_DOF_CLEANUP_TEST.md).

La case Bonus mount skins règle la disponibilité du menu ; le bonus déjà équipé reste conservé même OFF, comme les armures. Le modèle de base reste sélectionnable et son choix n'est jamais remplacé automatiquement. Chargement des montures, réglages DOF et contrôle visée/tir/dash confirmés dans les essais V1.73 rapportés. Le flou V1.71 semble bon dans le dernier retour utilisateur ; les réglages DOF V1.73 sont également confirmés dans les essais rapportés.

Le candidat Dash movement recovery V1.71 est **REJETÉ dans l'essai utilisateur et retiré**, avec son état, sa checkbox et son réglage runtime. Les corrections validées de visée/tir pendant le dash sont conservées. Les anciens raccourcis Recovery restent inertes sans décaler les autres. V1.75 ajoute un nouveau candidat distinct pour la réception lourde automatique après chute, OFF par défaut et encore expérimental.

Archive vérifiée : `DarksidersGenesis_V1.73_MOUNT_DOF_CLEANUP_TEST.zip`, quatre fichiers racine. OFF complet des pistolets reste ouvert ; modèles gauche/droite validés conservés.

Suite en cours : [audit OFF des pistolets et HUD général](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/AUDIT_PISTOL_OFF_HUD_2026_10_20.md). Les montages de tir ne séparent pas les mains par leur nom de slot ; aucun OFF partiel ajouté. Essai V1.73 après refus d’arène : HUD toujours visible, relevés avant/après effectués ; défaut historique placé en surveillance.

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
| Recovery | Ancien candidat dash retiré ; nouveau Heavy landing movement recovery pour réception lourde automatique | OFF par défaut, expérimental V1.75 ; efficacité à confirmer en jeu |
| Modèles 3D des pistolets | Choix indépendant du modèle droit ou gauche par main de Strife | Choix gauche/droite validés ; OFF complet encore ouvert |
| Skins | Deux montures bonus ; six variantes de progression dans deux groupes séparés | Sélection et OFF confirmés V1.68 ; sélection sans PAK confirmée ; maintien au chargement V1.73 confirmé dans les essais rapportés |

[État courant](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/CURRENT_STATE.md) et [file active](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ACTIVE_QUEUE.md) font référence pour les éléments restant à tester ou réparer. Une validation utilisateur porte sur l’essai rapporté, sans supposer tous les personnages, chapitres et états coop vérifiés.

### Apparences et durée ultime

Les choix **Native / Right pistol model / Left pistol model** par main de Strife sont conservés. Les holsters montés utilisent la variante rangée correspondante. Le futur OFF complet (modèle, effets et bras, avec projectiles conservés) n’est pas livré dans cette version.

**Bonus mount skins** et **Progression skins** restent indépendants et OFF par défaut. V1.67 interceptait à tort « équipé » ; V1.68 remplace ce branchement par le vrai « débloqué ». Aucun droit DLC, récompense ou bonus de gameplay accordé par le mod. Le jeu garde la sélection native ; fermer et rouvrir la page Skins après un changement. L’essai V1.68 de chevaux/armures et le retour natif OFF sont confirmés sans PAK cosmétique par le journal moteur. Les armures restent dans le dernier essai. Le défaut de chargement des montures a été localisé dans le filtre DLC natif ; V1.72 conserve les deux choix bonus déjà présents même lorsque leur disponibilité dans le menu est désactivée, comportement confirmé dans les essais V1.73 rapportés. Un PAK éventuellement réinstallé peut maintenir des montures disponibles avec l’option ASI OFF.

**Infinite Ultimate duration** bloque seulement le débit de la transformation déjà active, au niveau courant de jauge. Il ne remplit rien et ne rend pas l’activation gratuite. Décocher reprend le multiplicateur de durée si sa case est active, sinon le débit natif. Nouvelle action assignable dans Hotkeys, non attribuée par défaut. **Unlimited Ultimate** conserve son rôle de remplissage.

**Override Leviathan lightning**, décoché par défaut, rend le comportement natif. Une fois coché : **OFF / Effect 1 / Effect 2 / Effect 3**. Visuel seulement, sans accorder le cœur ou son bonus. Si le visuel choisi est indisponible, le menu l’indique et conserve l’effet natif. Pied/cheval/ultime et P2 à tester, ainsi que le retour natif sans doublon.

**TPS character focus** garde la méthode DOF du niveau. En TPS, la couche Gaussian proche est désactivée et la région nette protège la profondeur du modèle ; flou lointain, qualité et ouverture restent natifs. Clé INI `TPSLensFocus` conservée. OFF garde la correction de distance seule. V1.69 remplace le forçage CircleDOF qui rendait les décors trop nets. V1.70 conserve aussi la région nette du niveau tant que la caméra ne la surcharge pas déjà. Dans Camera → Depth of field, Native view et TPS view ont chacun Disable et Custom. Custom remplace explicitement le DOF par Gaussian ; distance0–5000m, quantité0–5% absolue (pas un facteur), saisie manuelle et Default. Distance10m et quantité0,6% sont inactives tant que Custom est OFF. Disable est prioritaire et garde les valeurs mémorisées. Les réglages DOF sont confirmés dans les essais V1.73 rapportés.

Léviathan s'attache à la racine comme le natif ; l'ancien rattachement au modèle de Strife était 97 unités plus bas. Hauteur et rangement du menu validés par l’utilisateur dans l’essai V1.69. **Appearance** et **Weapon sheathing** sont repliables et distinctes de **Player movement** ; réglages et raccourcis conservés.

La disparition du HUD après refus d’arène n’est pas reproduite dans le dernier essai V1.73 ; surveillance, sans correction dédiée ni réaffichage global forcé. OFF complet pistolet et nouvelle approche Recovery ouverts. [Essais et rollback V1.69](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_69_NATIVE_VISUAL_FOCUS_TEST.md).

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
