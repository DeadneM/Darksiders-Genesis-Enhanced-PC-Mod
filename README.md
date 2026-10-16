## V1.46 — construite, contrôlée et installée : coop / ressources / FOV (16 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation utilisateur en jeu requise.** Base debug V1.45 `1b35ad7` (ses bugs signalés restent la référence de comparaison), acquis solo V1.43 et potions V1.44 conservés. Source finale `baf7d3fb1e6b1d4635a81630a8d3262bd3ebd506`, archive debug `4808373a44b47ee382f84a2fd5a02ff622456632`. [Compilation Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38933030844) **SUCCESS, 21/21 tests**, incluant les vrais modules caméra P2, propriété cheval/ultime, ressources P1/P2, budget de vol plané/sprint, santé et rotation de départ du projectile. Les tests isolés remplacent seulement leurs frontières natives ; ils ne prouvent pas le résultat visuel. Analyse MSVC : huit avertissements historiques, aucun ajouté dans les nouveaux modules. La première tentative a échoué sur un nom du test cheval ; corrigée avant cette compilation et sans installation intermédiaire.

**Camera → Third Person → FOV coop** : réglage distinct du FOV solo, commun aux deux vues, curseur et saisie40–140°, Default110°. L'INI existant sans cette clé hérite du FOV TPS personnel. TPS FOV reste l'activation commune ; aspect ratio et viewport natifs conservés. Autres ajouts : saisie manuelle du diviseur de munitions0–10 ; **Fill health / Unlimited health / Unlimited glide / Unlimited horse sprint duration**, avec quatre nouvelles actions Hotkeys, Unlimited OFF par défaut. Les corrections expérimentales portent sur les propriétaires des caméras cheval/ultime, les options P2, l'angle conservé lors du changement Strife/War, le ViewTarget natif pour les scènes et la rotation de base des tirs Strife Camera Forward à la manette. Les huit fonctions principales RawInput/visée/axes/roues vérifiées restent identiques à V1.45.

[ZIP cumulatif V1.46 — téléchargement direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.46_COOP_RESOURCES_CAMERA_TEST.zip), **832350 octets**, SHA256 `89B4A051142E37213257C2843B4A229BF6D3BA47E524C2CE2CB2B9CF6173A745`. Exactement ASI/dxgi.dll/INI/README à la racine, binaires AMD64, version embarquée1.46,119clés et115 anciens défauts inchangés ; hashes CI/dépôt/téléchargement direct concordants. **109 anciennes archives intactes**, main2bc3122, archive10e0f22 et release publiquev1.43 id414771221 contrôlés inchangés. Le README du ZIP immuable est celui de la préparation ; le présent reçu décrit la livraison effective.

Installée après réponse « JEUX FERMER » et vérification effective des processus, avec sauvegarde des quatre fichiers et du log : `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v146-20261016-183856`. ASI/dxgi de destination conformes au ZIP ; **INI personnel préservé octet pour octet**, SHA256 `FD8C182119BAACC318B03BC77831D57E5662924C229D7A7A0E9258333A3F3FFC` avant/après, raccourcis conservés. Le README installé est actualisé avec ce reçu. Aucun lancement automatique, aucune écriture externe de mémoire ni changement des sauvegardes du jeu.

**Test prioritaire :** régler le FOV coop et vérifier les deux vues, puis revenir en solo ; tester séparément cheval et ultimes P1/P2, premier tir primaire/secondaire, options/remplissages P2 et ON/OFF des nouveaux Unlimited, roues et retour de scène. **Visée native après dash : NON RÉSOLUE. Action Recovery : indisponible.** Trajectoire réelle, scènes gardant une caméra attachée, santé face aux morts de script et armes d'ultime hors classe de pistolets restent des limites explicites. [Preuves, critères de réussite, TODO et rollback](docs/V1_46_COOP_RESOURCES_CAMERA.md). Les entrées ci-dessous sont historiques.

## V1.46 — coop, caméra et ressources (source de TEST)

Base V1.45 debug `1b35ad7` / source `332cbc9`, issue de la base solo V1.43 et des potions V1.44. **SOURCE PRÉPARÉE / EXPÉRIMENTAL : compilation, paquet et installation restent à vérifier.** Les retours V1.45 rejettent encore la visée native après dash, la caméra montée/ultime en coop, plusieurs options P2 et certains déclenchements cinéma ; aucune validation globale V1.45 n'est revendiquée.

V1.46 reconnaît les caméras appartenant au cheval/à la forme active, utilise la pose du bon joueur et route gains, remplissages, Unlimited et coûts de munitions par deux contextes distincts. Le vrai `ViewTarget`/`PendingViewTarget` remplace le simple verrou « mode cinématique » ; une caméra de jeu prouvée n'est plus rejetée à cause de son angle/distance. L'angle TPS reste choisi lors d'un échange Strife/War dans le même monde. Une nouvelle correction vise le départ du projectile des pistolets de Strife au moment du tir, en mode Camera Forward à la manette ; souris, visée native, équipement et boutons gardent leurs chemins.

Ajouts demandés : **FOV coop TPS indépendant du solo**, commun aux deux vues, réglable dans Camera → Third Person (curseur, saisie et Default). Une ancienne configuration hérite du FOV TPS personnel ; aucun aspect ratio ni viewport modifié. Saisie manuelle du diviseur de munitions 0–10 ; **Fill health**, **Unlimited health**, **Unlimited glide**, **Unlimited horse sprint duration**, avec quatre nouvelles actions assignables dans Hotkeys, sans réaffecter les anciennes. Les trois nouveaux Unlimited sont **OFF** par défaut. La santé utilise le setter/plafond natifs des joueurs vivants ; le vol plané conserve son budget de durée tout en exécutant la physique et les sorties natives ; le cheval cesse de consommer son endurance de sprint et conserve récupération/conditions de départ. UI des jauges et potions montre aussi P2.

**Visée native après dash : NON RÉSOLUE. Action Recovery : toujours indisponible.** Les observations cheval/ultime prouvent les propriétaires, pas la réussite visuelle de V1.46. Les nouvelles fonctions et la trajectoire réelle exigent un test utilisateur. [Preuves, limites, objectif, tests et rollback](docs/V1_46_COOP_RESOURCES_CAMERA.md). Les archives antérieures et les branches main/archive restent protégées ; aucune release publique V1.46 demandée.

## V1.45 — construite, contrôlée et installée : coop / potions TEST (16 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation en jeu encore requise.** Base solo V1.43, potions V1.44 conservées ; source `332cbc973c521910522df990df98e0867066726e`, archive debug `b68341ee7d1aa3f44d2dbfce6f560c4bbf09afba`. [Compilation Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38890283190) **SUCCESS, 17/17 tests**, dont le résolveur natif de propriétaires et le vrai module de caméra/commandes P2 isolé. Analyse MSVC : mêmes huit avertissements historiques que V1.44, aucun nouvel avertissement dans les modules coop. Contrôle en lecture seule sur la session coop : les quatre composants Follow/Split reconnaissent leurs deux propriétaires distincts.

[ZIP cumulatif V1.45 — téléchargement direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.45_LOCAL_COOP_OWNERSHIP_TEST.zip), **820459 octets**, SHA256 `D08CA7A180B12AC6F6A61CE1A1E6E8543C7E6A4D76581B657E32F6CD496B6FA7`. Exactement ASI, dxgi.dll, INI et README à la racine ; deux binaires AMD64, identité embarquée1.45,115clés INI et hashes CI/dépôt/téléchargement direct vérifiés. **108 anciennes archives inchangées**, main2bc3122 / archive10e0f22 et release publiquev1.43 vérifiées intactes.

Installation après fermeture effective du jeu, avec sauvegarde complète des quatre fichiers et du log : `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v145-20261016-125910`. ASI/dxgi remplacés et hashes de destination conformes ; **INI personnel préservé octet pour octet**, SHA256 `5766A97774293C8C30F14D0855EAEC4A4B1138C899548A0088DC9A0E3BA9D0D7` avant/après. Le README installé est actualisé avec ce reçu ; le ZIP immuable conserve son README de préparation. Aucun lancement automatique.

Le suivi, l'orbite et la visée P2 utilisent désormais leur propre chaîne joueur/contrôleur/caméra ; les raccourcis existants sont conservés. Les ajouts potions comprennent remplissage, recharge1–100 et Unlimited, assignables dans Hotkeys sans réaffecter les touches existantes. RawInput, Alt-Tab, gains/jauges, munitions, filtres shader et Skip Warning restent identiques ; les raccords de propriété caméra/cheval/manette sont décrits dans l'audit. **Le comportement coop, les potions et le bug de visée native après dash sont à tester. Action Recovery reste indisponible** : la durée supplémentaire se situe dans l'animation Level3 du troisième dash de Strife, sans point sûr de fin anticipée encore démontré.

**Test prioritaire :** éloigner les deux personnages, tourner chaque caméra avec sa propre manette, puis faire réapparaître chaque personnage et vérifier qu'il garde sa vue. Contrôler ensuite roues, vue native + visée + dash, cinématique et potions ON/OFF. [Preuves, limites, TODO et rollback](docs/V1_45_LOCAL_COOP_OWNERSHIP.md). Les préparations ci-dessous sont des enregistrements historiques.

## V1.45 — caméras coop attribuées à leurs joueurs (source de TEST)

Base solo V1.43 validée hors bugs signalés ; ajouts potions V1.44 conservés **CONSTRUITS / CONTRÔLÉS, non encore testés en jeu ni installés**. V1.44 source248db7b, CI38885637165 **SUCCESS,15/15 tests** ; ZIP812291octets SHA256 `7704EE2A7005F149002467826FEB650E7F9579EEF80EDA6C7D3FFA6E950B838F`, quatre fichiers à la racine, binaires AMD64 et téléchargement direct contrôlés. Archive V1.44 conservée immuable ; la release publique V1.43 reste en place.

La capture coop du16octobre prouve deux LocalPlayers, deux contrôleurs et deux caméras `SplitscreenCamera` appartenant chacun à leur personnage. Le mod utilisait pourtant une référence globale remplacée par le dernier personnage observé. V1.45 prépare une identité P1 fixée par `GameInstance.LocalPlayers[0]`, une vue P2 avec suivi/orbite/pause/visée séparés et sa propre manette native `ControllerId`. Les réglages et boutons reconfigurables existants sont repris ; aucune nouvelle touche imposée. L'ancien hook natif de masquage persistant des effets de visée est retiré ; le filtre shader validé reste identique. **Le bug visuel/de direction après dash en vue native reste à retester, sans cause démontrée à ce stade.**

La troisième capture dash identifie `Strife_Dash_Level3_Montage` : durée observée environ0,83s contre0,40s pour les deux premiers dashs, avec une fin de déplacement faible après environ0,5s. Les positions seules ne mesurent pas l'instant d'appui du quatrième dash et ne prouvent pas un point sûr de fin anticipée. **Action Recovery reste indisponible, aucune nouvelle intervention dans l'animation.** [Preuves natives, changement, limites, tests et rollback](docs/V1_45_LOCAL_COOP_OWNERSHIP.md).

**Source préparée / EXPÉRIMENTAL. Construction V1.45, paquet, installation et validation utilisateur pas encore annoncés.** Objectif : chaque vue et commande de caméra suivent leur joueur avant/après réapparition ; aucun vol de caméra entre P1 et P2. RawInput, Alt-Tab, tir souris solo, potions, munitions, jauges, roues et Skip Warning conservés hors raccords de propriété décrits dans l'audit.

## V1.44 — potions natives TEST et capture Strife (16 octobre 2026)

Base V1.43 debug `5fecde0`. **Sources préparées / EXPÉRIMENTAL, construction et installation à vérifier.** Ajout des commandes **Fill health potions**, **Potion recharge rate** (1–100×, défaut2 désactivé), **Unlimited potions** (OFF), chacune assignable dans Hotkeys → F1-F12. Remplissage et maintien du stock par l'ajout/plafond natifs ; recharge par conversion du surplus de soin ; soin et cooldown conservés. Trois nouvelles clés, 112 valeurs et raccourcis antérieurs inchangés. Entrées/visée/caméra/cheval/roues/jauges/munitions/warnings préservés ; aucun nouveau résultat en jeu revendiqué.

Recovery reste indisponible : deux captures en lecture seule,22 puis17 instances natives de dash, seconde confirmée avec Strife. Délai global0 ; mode de mouvement(6,5) maintenu jusqu'à la fin, aucune fin de fondu exploitable observée. Pas de retour de l'ancien hook sans effet ; prochaine recherche limitée à la fin du déplacement réel et à la libération native du dash, coffres reportés. V1.43 publique,107 archives et main/archive protégés. [Preuves natives, configuration, tests, TODO et rollback](docs/V1_44_NATIVE_POTIONS_AND_DASH_AUDIT.md).

## V1.43 publiée et installée — défauts utilisateur (16 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / PUBLIÉ / INSTALLÉ.** Base V1.42 debug `45571dd354e316fcb4f1fa255741844c5dd09d54`. Source `902861eb1ff34c14dc5ded092581b360767da817` ; [Actions Windows x64 38880517949](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38880517949) **SUCCESS, 13/13 tests**. ASI, INI, génération, runtime, resets et boutons Default partagent les **112 valeurs** du profil utilisateur. Seuls les défauts cheval changent : **distance 0,25× / vertical +40** ; pied 0,15/0, ultime 0,30/+80, pitch −12°, FOV TPS 110°. Les 110 autres valeurs, Inser et les raccourcis sont conservés.

[Release publique v1.43](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases/tag/v1.43) depuis **debug**, non-prerelease ; tag et target_commitish vérifiés sur le source construit. [Téléchargement direct du ZIP cumulatif](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases/download/v1.43/DarksidersGenesis_V1.43_USER_DEFAULTS_RELEASE.zip), **807377 octets**, SHA256 `A680334FC4E634AC24AF92DF59A00BEFCCA8B4716EEABDDCB08BCE1C64FAF990`. Quatre fichiers à la racine, deux binaires PE AMD64, identité V1.43 et parité INI vérifiés. Archive Git, asset public, checksum et CI identiques. Publication de l'archive au commit `4f6115ff9a2cf30bb3523e6b7544c6a7bc645a6f` ; **106 anciennes archives, main et archive préservés**. Analyse MSVC exécutée, mêmes huit avertissements déjà examinés, sans nouveau gain de performance ou validation en jeu revendiqué.

Installation jeu fermé avec sauvegarde des quatre fichiers et du log : `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v143-20261016-113324`. Binaires installés conformes ; **INI personnel conservé byte pour byte**, SHA256 avant=après `5766A97774293C8C30F14D0855EAEC4A4B1138C899548A0088DC9A0E3BA9D0D7`. README installé actualisé avec ce reçu ; le ZIP immuable conserve le README de préparation du source. Aucun jeu lancé automatiquement.

**Action Recovery reste indisponible ; prochaine investigation : dash uniquement, coffres reportés.** Aucun hook recovery réintroduit. Fonctions natives caméra/visée/souris/manette/cheval/roues/jauges/munitions/warnings/updater identiques à V1.42 ; validations déjà acquises conservées, sans nouvelle preuve en jeu pour V1.43. Test suivant : Default cheval 0,25/+40 et Updates V1.43 ; puis capture synchronisée du dash et identification de sa libération native avant un essai fonctionnel. [Profil, preuves, TODO et rollback](docs/V1_43_USER_DEFAULTS_RELEASE.md).

## V1.42 construite, contrôlée et installée — caméra cheval et version updater (16 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL en attente de test en jeu.** Base V1.41 debug `04ca27e`. Source `bf02da378a8890c07581045019ac396d0a008dc3` ; [CI Windows x64 38875417060](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38875417060) **SUCCESS,13/13 tests réussis**, dont vraie persistance/héritage/indépendance des vues pied/cheval/ultime et parité des **112 clés**. Analyse MSVC exécutée, mêmes huit avertissements déjà examinés en V1.41. ZIP publié par Actions au commit `64c4e93c2ee02b5a891237b6facb5306bdcc35e8`.

Camera → Third Person → **Horseback TPS camera**, sous l'ultime : **Distance (Horseback)** et **Vertical offset (Horseback)** avec saisie manuelle, curseur et Default. Les anciens INI héritent de leur vue à pied pour démarrer ; valeurs ensuite indépendantes après sauvegarde. Défauts neufs0.15/0 et reset TPS inclus. Pose montée native existante conservée ; trois sélections de valeurs ajoutées, orbite/visée/commandes/roues/gauges/munitions/warnings préservés. **Updates affiche désormais la version compilée**, supprimant le texte V1.22 figé signalé dans la capture ; V1.42 attendue au prochain lancement. La comparaison de version déjà corrigée reste identique.

[Télécharger le ZIP cumulatif V1.42 TEST](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.42_HORSE_CAMERA_SETTINGS_TEST.zip), **806041 octets**, SHA256 `3C47E465DAF03D7C9AE8B6767045A1444CCA03C0A37D67BC15AE30B89F76E2FC`. Exactement quatre entrées à la racine, deux binaires PE AMD64 et identité ASI1.42 vérifiés. Archive Git/téléchargement direct/CI identiques ; **105 archives précédentes**, main et archive vérifiés inchangés.

Installée après fermeture du jeu, sauvegarde complète `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v142-20261016-104624` ; hashes des binaires installés conformes. **INI personnel conservé byte pour byte**, SHA256 avant=après `CD18319DD0468D35E7E0BCDA3A7EEEE9C76F8B7689131C0CA9AD75E313518AE9`. Les deux clés manquantes sont héritées en mémoire et seront écrites par le prochain enregistrement des réglages ; les110 anciens réglages et les changements personnels récents restent conservés. README installé actualisé avec ce reçu ; ZIP immuable conserve son README de préparation. Aucun jeu lancé automatiquement. [Détails, tests et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_42_HORSE_CAMERA_SETTINGS.md).

**Action Recovery reste indisponible.** La relecture de l'ancienne capture montre18 observations/12s, délai global déjà0, quelques états MONTAGE et aucun DASH/INTERACT identifié : manque de synchronisation avec l'attente ressentie. Prochaine étape : capture pendant les deux actions puis identification de leur verrou/libération native, avant un autre essai fonctionnel. [Preuves, piste et protocole](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ACTION_RECOVERY_NEXT_INVESTIGATION.md). Test de V1.42 : réglages cheval distincts, monter/descendre, ultime, tir/roues, puis vérifier Running mod dans Updates ; aucune nouvelle validation en jeu revendiquée.

## V1.42 - préparation historique (reçu ci-dessus)

Base livrée V1.41 debug `04ca27e`. **Sources préparées / EXPÉRIMENTAL**, construction et installation à vérifier. Camera → Third Person → **Horseback TPS camera**, sous la forme ultime : **Distance (Horseback)** et **Vertical offset (Horseback)**, avec curseur, saisie manuelle et Default. Deux valeurs indépendantes persistées ; anciens INI : héritage de leurs paramètres à pied. Défauts neufs0.15/0, reset TPS étendu ; les110 valeurs préexistantes et Inser conservés.

La caméra utilise la pose montée déjà reconnue, avec priorité à la forme ultime si active. Trois sélections de valeurs ajoutées ; provenance de pose, orbite, pitch/FOV partagés, visée/RawInput/XInput, cheval, roues, jauges/munitions et Skip Warning inchangés. Tests de configuration étendus aux trois profils et parité112 clés. Aucun nouveau succès en jeu annoncé. Jeu ouvert pendant la préparation ; installation avec sauvegarde après fermeture. [Détails, critères de test, compatibilité et rollback](docs/V1_42_HORSE_CAMERA_SETTINGS.md).

Correction du retour utilisateur sur la capture Updates : l'affichage « Running mod: V1.22 » était encore écrit en dur malgré la comparaison corrigée en V1.41. Le libellé et le préfixe du journal suivent maintenant BuildInfo ; ils doivent afficher la version compilée V1.42. Ce point manqué dans l'audit est documenté ; logique d'installation et chemins de contrôle inchangés.

## V1.41 construite, contrôlée et installée — 16 octobre 2026

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL en attente du test utilisateur.** Base livrée V1.40 `e230b53` ; source V1.41 `d1bae62d3f191f8efb0a2c4b51a50667920ca08a`. [GitHub Actions Windows x64 38843203433](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38843203433) **SUCCESS**, job116787762522, **13/13 CTest réussis** et analyse native MSVC `/analyze` exécutée. Huit avertissements examinés et détaillés dans l'audit, pas une prétendue absence d'avertissements. Test isolé C# local : zéro échec. Archive publiée au commit `0b42a1942c25819fc50152e151263920fb808067`.

[ZIP cumulatif V1.41 TEST](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.41_AUDIT_CLEANUP_INSERT_TEST.zip), **803900 octets**, SHA-256 `0BEB4EAFC2C1212EC56EB5CC4FEA32F4F8ECB360CC0D2A0F0B1F82AF2DEDB67B`. Exactement quatre fichiers à la racine, deux modules PE AMD64, identité ASI1.41, ancien recovery actif absent, anciens hooks de gains retirés. Le téléchargement direct et l'archive Git ont le même hash que la CI. Les **103 archives antérieures** et main/archive sont inchangées. Updater autonome V1.1 séparé, [CI38843203439 réussie](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38843203439), deux entrées à la racine / PE AMD64 vérifiés ; ancien V1.0 préservé.

**Inser est rétabli** dans les valeurs par défaut et dans l'installation personnelle. Les **109 autres valeurs** de l'INI restent identiques ; hors unique remplacement MenuKey, octets conservés. Sauvegarde complète avant installation : `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v141-20261016-050412`. INI avant `A351972C25730520AC248854F0032C8A659FA94434F2C9F0F8111CCAD772EB8D`, après `FA634ED0793339FC6AE34E72C08D47A86447CCB8300295EE9056C8BFDECF097E` ; hashes des binaires installés conformes au ZIP. Installation jeu fermé, sans lancement automatique. README installé actualisé avec ce reçu ; le ZIP immuable contient le README de préparation du source.

Nettoyage : **802 lignes natives nettes en moins**, trois anciennes installations de hooks supprimées, sondes obsolètes retirées et lecture du yaw réduite à une seule valeur dans deux callbacks fréquents. Erreurs de sauvegarde et de version updater corrigées ; commandes reconfigurables publiées par atomiques. Calculs validés des munitions/gains et fonctions acquises préservés. **Action Recovery retiré car sans effet**, toujours à réétudier après dash/coffre. Aucun gain de FPS chiffré ni nouveau succès en jeu annoncé. Test suivant : Inser, munitions/gauges, TPS pied/cheval, sticks, roues/pause, Alt-Tab, cinéma et forme ultime. [Audit détaillé, limites restantes, TODO et rollback](docs/V1_41_FULL_AUDIT_CLEANUP.md).

## V1.41 - préparation historique de l’audit (reçu ci-dessus)

Base V1.40 debug `e230b53`. **Sources préparées / EXPÉRIMENTAL**, construction et livraison à vérifier. Audit de source, logs, configuration, callbacks natifs, updater et workflows : anciens hooks/sondes sans bénéfice retirés ; essai Action Recovery abandonné après zéro effet mesuré, clairement indiqué indisponible. Correction de la fausse réussite de sauvegarde INI, de la comparaison updater restée V1.22 et de son historique périmé après une installation manuelle. Huit commandes reconfigurables publiées par atomiques ; règles XInput/sticks/tir conservées. Lecture du yaw seule dans deux callbacks fréquents au lieu de copier toute la télémétrie.

**Menu par défaut : Inser**, dans ASI, INI, génération et boutons Default. Les 109 autres valeurs du profil V1.40 restent identiques. Calculs validés munitions/gains/remplissages, RawInput, Alt-Tab, cheval, roues, caméra et présentation Skip Warning conservés ; aucune nouvelle validation en jeu annoncée. Anciennes archives et main/archive préservées. [Audit complet : erreurs, code retiré, risques restants, contrôles, TODO et rollback](docs/V1_41_FULL_AUDIT_CLEANUP.md).

## V1.40 built, checked and installed - user defaults / ammo validated (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE.** Base V1.39 debug474bbfa. User **VALIDATES ammunition behavior**, while **Action Recovery remains unresolved** (last test12132 ticks,8927 local montage observations,zero shortened tails/finishes). This defaults release adds no native recovery intervention and claims no recovery repair. Source86f773cabf0c2a2523c36ffb83b4127538a3e4f6; [Windows x64 CI38807877488](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38807877488) **SUCCESS,11/11 CTest suites passed**. The new real config test compares all **110 shipped INI keys** with compiled missing-INI generation and reset output, checks runtime settings and preservation of explicit older overrides. Archive publication f88880fe8a618b3ad5070170d26613d41d16b07c.

[Download cumulative V1.40 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.40_USER_DEFAULTS_AMMO_VALIDATED_TEST.zip), **811138 bytes**, SHA256 `662BAD16053DA496AF6495AFCBDA306B5EC57AA9493FFE429E17BB0279FBD77A`. Exact four root files, PE AMD64 binaries, embedded ASI1.40 identity and full packaged INI values equal to the saved user profile. CI/repository/direct-download hashes match; all **102 previous archives**, main2bc3122 and archive10e0f22 verified unchanged.

Saved profile now defines new installs, reset and Default buttons: user's menuVK_C0, debug update checks, ammo reductionON/divisor2, Unlimited togglesOFF, gains4/4/4, Ultimate camera0.30/+80, F1-F9 gauge/Unlimited actions and F12 HUD. Existing explicit personal INI values remain authoritative; legacy missing-Ultimate keys still inherit their normal camera. All native ammo/recovery/input/camera/horse/gauge/warning/updater implementations are identical to V1.39; only defaults, descriptive status and build identity changed. The user-selected defaults are incorporated; **no new in-game claim is made for V1.40 or Action Recovery**.

Installed with game closed, complete backup `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v140-20261015-231525`, matching installed binary hashes and **personal INI preserved byte for byte**, SHA256 `A351972C25730520AC248854F0032C8A659FA94434F2C9F0F8111CCAD772EB8D` before/after. Installed README refreshed with this receipt; immutable ZIP retains its source-preparation README. No automatic game launch. Next: retain the validated ammunition path and investigate the real recovery lifecycle/eligibility before another intervention. [Defaults, runtime evidence, compatibility, rollback and TODO](docs/V1_40_USER_DEFAULTS.md).

## V1.40 source-preparation record (historical, superseded above)

Base V1.39 debug474bbfa. User **VALIDATES ammo**, reports Action Recovery still ineffective, and explicitly chooses the current installed INI as the new ASI/INI defaults. Saved profile is promoted: menuVK_C0, debug update channel, ammo reductionON/divisor2, gains4/4/4, Ultimate camera0.30/+80 and current F1-F12 bindings. Store/runtime/reset/Default buttons are synchronized; explicit existing INI overrides and legacy camera-key inheritance are preserved. Ammo/camera/input/gauge/native recovery implementations are unchanged. V1.39 recovery log has12132 ticks/8927 local observations but **zero applied tails/finishes**; UI now labels it unresolved. **Source prepared / EXPERIMENTAL; build/package/install not yet claimed.** [Profile, evidence, acceptance and TODO](docs/V1_40_USER_DEFAULTS.md).

## V1.39 built, checked and installed - ammo / native animation recovery TEST (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL.** Base V1.38 debug e39153e, with Unlimited Wrath and Skip Warning now explicitly **VALIDATED by user**, preserving earlier gauge/TPS validations. Source 1a1684a82d861cc5cc3f066879ff7dcf4d787791; [Windows x64 CI38802359996](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38802359996) **SUCCESS, 10/10 CTest suites passed**, including real ammo INI/hotkey save/reload, fractional cost/Unlimited policy and recovery delay resets. Numbered ZIP publication ee59029851ae9549556418f089d6d75b2339ed75.

[Download cumulative V1.39 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.39_AMMO_ACTION_RECOVERY_TEST.zip), **809940 bytes**, SHA256 `4BE85D59CC89B1D11D6AE4341B78B9221520CE1F95DC691FDF5447F76A2F9441`. Exact four root entries, both binaries PE AMD64, embedded ASI1.39/new-feature identities, former forced-MOVE ready path absent, default ammo controls OFF/divisor2 and all older INI defaults retained. CI/repository/direct-download ZIP hashes match. All **101 earlier archives**, main2bc3122 and archive10e0f22 checked unchanged.

Installed with game closed after the user's closure reply, binary destination hashes verified. Complete prior installation/log backup: `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v139-20261015-225133`. **Current personal INI preserved byte for byte**, SHA256 `50274EA27343151CE44A38374F4C585163D96BC06EFA64D42FA69DC7A60AD51C` before and after; no personal camera value, gauge setting or shortcut replaced. Installed README refreshed with this delivery receipt; the immutable ZIP retains its pre-CI cumulative README. No game was launched automatically and **no V1.39 in-game functionality is claimed validated**.

Gameplay -> Combat/resource controls -> **Ammunition (test)**: optional **Reduce ammo consumption** divisor0-10 (0/1 normal,2 half,10 one tenth), independent **Unlimited ammo**, each assignable in Hotkeys -> F1-F12. Existing assignments retained. Player movement -> **Action Recovery** retains its checkbox/action/delay, now testing normal native completion at natural animation blend-out instead of overriding MOVE. Full XInput/RawInput/camera aim/horse/gauge/warning implementations protected against the V1.38 base. **Next test:** finite special ammo OFF/2/10/Unlimited ON then OFF; dash and chest with Recovery OFF/ON(delay0), retained loot, proper locomotion/attacks and no sliding. Runtime counts are evidence of hook activity/completion, not measured user responsiveness. [Native evidence, remaining limits, acceptance and rollback](docs/V1_39_AMMO_ACTION_RECOVERY.md).

## V1.39 source-preparation record (historical, superseded above)

Base **V1.38 debug e39153e**; user now validates Unlimited Wrath and Skip Warning, extending earlier gauge/TPS validations. Adds saved optional ammo reduction0-10 (0/1 native,2 half,10 one tenth), Unlimited ammo and two independent Hotkeys actions; no existing binding/default is reassigned. Replaces the ineffective forced-MOVE/AWAITING_FINISH recovery route with normal native completion of natural montage blend-out tails. Read-only V1.38 samples show MoveInterruptDelay0 and direct RUNNING-to-FINISHED transitions; exact native blend/finish paths were audited before implementation. Main animation/input path stays native until blend-out; this remains **EXPERIMENTAL**, including loot/locomotion/actual timing and real ammo spending. Windows build/package/install not yet claimed. [Evidence, acceptance, limits, rollback and cumulative TODO](docs/V1_39_AMMO_ACTION_RECOVERY.md).

## V1.38 follow-up - Wrath OFF / Void hub (2026-10-15)

User reports free abilities at zero with Unlimited Wrath OFF, then confirms **Strife in the Void hub**. That hub normally permits free Wrath ability use independently of the mod. Log confirms OFF20:06:13, manual0% with matching readback20:07:01, gauge stays0 until explicit all-ON20:27:39; shutdown has **zero Wrath hold overrides** and one later authorized top-up. **Void behavior explained; no gameplay fix or mission validation claimed.** Current V1.38 binaries/configuration retained; no new build. Next: test the same Wrath ability in an actual mission with Unlimited OFF, then ON/OFF. [Evidence, limits and next test](docs/V1_38_WRATH_OFF_CHECK.md).

## V1.38 built, checked and installed - Unlimited / Ultimate recenter / Skip Warning TEST (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL.** Base V1.37 debug985b0ff; all three natural gauge gain multipliers now **VALIDATED by user**, in addition to prior manual refills and ordinary TPS. Source 729f747770b9e3790775fe580c94357a92f446ae (initial feature commit cb9f130, final warning-counter diagnostic adjustment); [Windows x64 CI38780065798](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38780065798) SUCCESS, all **7/7 CTest** suites passed: stick roles, manual request policy, persisted gauge hotkeys/Unlimited/SkipWarning settings, Ultimate camera INI/runtime, natural reward policy, Unlimited identity/cap policy and exact named warning-page selection. Archive publication f57e9eeaa453b699c8e96a536efd5f2cd4ba59c9.

[Download cumulative V1.38 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.38_UNLIMITED_RECENTER_WARNINGS_TEST.zip), 800204 bytes, SHA256 `DEB367E0A8260B8CF0DC1E2F99974FF16ABBD11487431C2C0C72DF30814C46D0`. Exact four root entries, PE AMD64 binaries, embedded ASI1.38 identity, SkipWarning=1 / Unlimited defaults OFF and direct-download/CI/repository hashes independently checked. Installed with game closed and matching binary hashes. Backup: `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v138-20261015-194914`. Personal INI preserved except the explicitly requested **Features.SkipWarning=1**: exactly one byte changed, previous SHA256 `E43F791EB19ADF3DC289A8459B482B97672066DA9329DDB2146AD737D8D27E45`, new `1AC57E025B9A70EABC6E33B9820C38E70ACE045E1CF4FB5E44FD05178C33486F`; all personal camera values, multipliers and F1 assignments retained. Installed README refreshed with this receipt; immutable ZIP retains source-preparation README. main/archive and every previous archive baseline checked unchanged. Preliminary first CI ZIP retained in commit8829e789 and locally under ../audit-v131; final ZIP uses the corrected counter build.

Gameplay -> Resource recharge -> **Unlimited gauges (test)**: three independent saved checkboxes. Hotkeys -> F1-F12: **Toggle Unlimited Wrath / Ultimate / Synergy / all gauges**, no existing/default assignment changed. Unlimited has priority over manual target percentages, respects actual native maxima, and is OFF by default. Fresh verified form yaw supplies recenter; no input-path changes. **Skip Warnings (test) is ON as requested**, using the new exact-named presentation timer/fade path; rejected asset-load-loop bypass remains OFF. New features remain EXPERIMENTAL: no in-game Unlimited, recenter or warning-skip success is claimed. Test cold startup/notices/reticle/wheels, each Unlimited + all toggle then OFF/native spending, both Ultimate forms/recenter/exit. [Evidence, limitations, rollback and cumulative TODO](docs/V1_38_UNLIMITED_RECENTER_WARNINGS.md).

## V1.38 source-preparation record (historical, superseded above)

Base V1.37 debug985b0ff; **ALL three natural gauge gain multipliers now VALIDATED by user**, alongside prior manual refills and ordinary TPS. Adds three saved Unlimited checkboxes (default OFF) plus independent/all-gauges F1-F12 actions without changing bindings. Fresh active-form yaw repairs recenter's stale normal-pose selection. New named save/controller presentation experiment advances native timers/fades/cleanup; **SkipWarning defaults ON at explicit user request**, while the rejected asset-loading-loop bypass stays OFF. No new gauge hooks; full XInput/RawInput/native aim/horse/legacy startup proxy retained. **EXPERIMENTAL, source prepared; Windows build/install pending; no gameplay success claimed.** [Evidence, acceptance, rollback and cumulative TODO](docs/V1_38_UNLIMITED_RECENTER_WARNINGS.md).

## V1.37 built, checked and installed - Ultimate camera / Strife / native rewards TEST (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL.** Cumulative base V1.36 debug6e1e5fe, preserving validated manual refills/ordinary TPS. Source9ae0f59fe4067f1385fb98000121d41b380ddac2 (gameplay sourcef8c9858, Windows test-name repair only afterward). [Windows x64 CI38758958418](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38758958418) SUCCESS; all five CTest suites passed: stick roles, gauge request policy, saved hotkeys, independent Ultimate camera INI/runtime and positive resource-gain policy. Archive publication44074e32ab2d47aa7a0206bab3d6b8ec1ab5fd41.

[Download cumulative V1.37 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.37_ULTIMATE_CAMERA_AIM_GAUGES_TEST.zip), 793735 bytes, SHA256 `59FA3FB722D608789E79EE700B205A30826A5929C6AE0BFB67AAFEF7CD051857`. Independently verified exact four root entries, PE AMD64 binaries, ASI1.37 identity and direct download/CI/repository hash agreement. Installed with game closed and matching binary hashes; V1.36 backup `../audit-v131/install-backup-before-v137-20261015-171955/`, personal INI preserved exactly SHA256 `D4CCBFD4D2C2BCA1058E66716B870C104F7787A6492A23A69EA7EEF299D56738`. Installed README refreshed; immutable ZIP retains source-preparation README. main/archive and every older ZIP unchanged.

Camera -> Third Person -> below existing TPS values: **Distance (Ultimate form)** and **Vertical offset (Ultimate form)**, with manual entry/Default, independently saved. Missing keys inherit current personal normal distance/height. Strife-only verified form ownership enables the existing camera-forward firing paths; War remains native. New experimental natural-gain multiplier paths target actual Wrath pickups and Ultimate combat rewards, excluding manual refill, spends, potions, upgrades and other actors. Existing Synergy math/manual queue, normal-player XInput body, relative RawInput, horse, wheels and hotkeys retained. **Framing, Strife form attack-helper coverage and natural multiplier effects require user gameplay tests.** Skip Warning still OFF/nonfunctional: native stage identified as asset loading; guarded native asset-path resolution added for subsequent presentation work. [Evidence, limitations, tests, TODO and rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_37_ULTIMATE_CAMERA_AIM_GAUGES.md).

## V1.37 source-preparation record (historical, superseded above)

Base V1.36 debug6e1e5fe; validated manual refill and accepted ordinary TPS retained. Latest log: four typed Ultimate entry/exit pairs,3592 valid poses, only3 missing-actor samples; four combined F1 refills applied. Adds **Distance (Ultimate form)** and **Vertical offset (Ultimate form)** below existing TPS values, independently saved; missing keys inherit personal normal camera. Strife-only verified native owner/type bridge enables existing camera-forward pad/look and native3D mouse target paths; War stays native. New exact-caller local-authority setter hooks test real Wrath pickup and Ultimate combat gains, excluding manual refill, spends, potions and upgrades; validated Synergy unchanged. **EXPERIMENTAL, source prepared; Windows build/install pending.** Skip Warning stays OFF: corrected old stage as asset loading, adds bounded native asset-path resolution for next presentation investigation. [Evidence, limits, rollback and TODO](docs/V1_37_ULTIMATE_CAMERA_AIM_GAUGES.md).

## V1.36 built, checked and installed - gauge hotkeys / Ultimate pose TEST (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE. V1.35 manual fillers VALIDATED by user; new hotkey gameplay and Ultimate correction EXPERIMENTAL.** Base debug56fbedb, accepted ordinary V1.34 TPS with known Ultimate regression. Source44b6d12d1507e74926c54fe414ba93e46aec46eb; [Windows x64 CI38727065139](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38727065139) SUCCESS. CTest TpsStickRoles, GaugeRequests and GaugeHotkeys all passed (3/3), including actual INI save/reload of four new actions. Archive publication4d4ecd62c1b2c5c95fbab2f1cd5d61ff4f8bcf00.

[Download cumulative V1.36 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.36_GAUGE_HOTKEYS_ULTIMATE_POSE_TEST.zip), 788051 bytes, SHA256 `9538DC2BA73A1A649FA5653F51A63A2882C8B2ACC4B8C2BCBB4B5C56BBE55944`. Exact four root files, both binaries PE AMD64, ASI1.36 identity and direct-download hash verified against CI/repository. Installed with game closed, backup `../audit-v131/install-backup-before-v136-20261015-132719/`, matching binary hashes; personal INI retained exactly SHA256 `82AF15917398293EFA674C28EF064E15F85FF4D646B60ECBD11763AC3AD69D68`. Installed README refreshed with this receipt; immutable ZIP retains source-preparation README. All older archives and main/archive unchanged.

Hotkeys -> F1-F12: choose **Fill Wrath (100%)**, **Fill Ultimate (100%)**, **Fill Synergy (100%)**, or **Fill all three gauges (100%)**, then Save. No default or existing assignment changed. Same validated manual native refill queue; hotkeys are100% one-shot requests, the menu slider remains independent. New logs confirm7 manual requests/readbacks,2125 missing normal actor samples and ZERO native cinematic-mode events; exact Ultimate timing was absent. Native local form pose now supplies TPS follow and its native input passes through while transformed; no new hook. This is a bounded experimental correction, not an in-game success claim. Test four chosen hotkeys then Ultimate entry/movement/attacks/exit2-3 times; recheck ordinary foot/horse, wheels, pause, scene, both stick layouts and temporary native view. Natural Wrath/Ultimate gain multipliers remain unresolved. [Evidence, limitations, rollback and TODO](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_36_GAUGE_HOTKEYS_ULTIMATE_POSE.md).

## V1.36 source-preparation record (historical, superseded above)

**V1.35 manual fillers now VALIDATED by user for all three gauges.** Base debug56fbedb / accepted ordinary V1.34 TPS; Ultimate camera/control regression remains. Four100% refill actions added to Hotkeys -> F1-F12, independently and all together, with no changed assignments or INI defaults. Same validated native request path. New log: seven requests/applied readbacks,2125 missing actor samples, ZERO native cinematic-mode events; Ultimate timing was not marked. Native audit identifies the distinct form and adds a strictly typed local-pawn/owned-form pose, with native form input passthrough and no new hooks. **Ultimate follow/controls remain EXPERIMENTAL; source prepared, Windows build/install pending.** Existing normal XInput body, RawInput, horse, wheels, natural Synergy and manual refill engine retained. [Evidence, limits, rollback and acceptance](docs/V1_36_GAUGE_HOTKEYS_ULTIMATE_POSE.md).

## V1.35 built, checked and installed - manual gauge levels (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL, gameplay confirmation pending.** Base V1.34 debug b94de95 (ordinary play accepted, known Ultimate camera/control regression). Source 25f2d94e1ab62a56e9f18f62333bb353f7582498; [Windows x64 CI38717620159](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38717620159) SUCCESS, both C++ tests TpsStickRoles and GaugeRequests passed (2/2). Publication commit 93adad6cbb44182a02ac0474ba874f8baa2ce3ec.

[Download cumulative V1.35 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.35_MANUAL_GAUGE_REFILL_TEST.zip), 784427 bytes, SHA256 `6CBAE44DF0FE7C8B226211170C8464D7DE1EB4A81A077349D3B241245E0136BD`. Exact four root files, PE AMD64 binaries, embedded ASI1.35 identity and direct download hash independently verified. Installed with game closed and matching binary destination hashes. V1.34 backup: `../audit-v131/install-backup-before-v135-20261015-120713/`; personal INI unchanged SHA256 `82AF15917398293EFA674C28EF064E15F85FF4D646B60ECBD11763AC3AD69D68`. Installed README refreshed with this receipt; immutable ZIP retains the source-preparation README.

Test on foot: open mod menu -> Gameplay -> Combat and resource gauges -> Resource recharge -> Target level100% -> Set Ultimate. Close menu and activate Ultimate normally. Set Wrath/Set Synergy and the0-100% slider are available for their respective tests. These are explicit one-shot native level requests, not repaired natural gain multipliers or automatic activation/unlock. Typed local ownership/authority, native maxima, freshness, actor/generation cancellation and expiry guard requests. **No in-game refill or transformed-camera success has been claimed.** Camera/input/natural Synergy/horse/config remain the V1.34 base. Next: verify actual gauges/activation and collect the Ultimate transition log before changing camera ownership. main/archive and every old ZIP verified unchanged. [Native evidence and remaining work](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_35_MANUAL_GAUGE_REFILL.md).

## V1.35 source-preparation record (historical, superseded above)

Base V1.34 debug b94de95: ordinary play accepted by user, **Ultimate overhead-camera/control regression newly reported**. Priority is explicit manual refill before camera repair. V1.35 adds queued one-shot 0-100% native level controls for Wrath, Ultimate/Chaos and Synergy, typed live-local ownership, native caps, readback logs, actor/generation cancellation and expiry. DebugRefill commands were proved shipping no-ops; actual native bounded setters are used. No camera/input/horse/natural Synergy/config changes; no auto refill, unlock or activation. Natural Wrath/Ultimate gain multipliers remain unverified. **EXPERIMENTAL, source prepared; Windows build/install pending.** [Evidence, limits and test](docs/V1_35_MANUAL_GAUGE_REFILL.md).

## V1.34 verified Windows build and local installation - EXPERIMENTAL

**CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL, not user validated.** Base V1.33 debug 2291e80; source d362076f357306db1d45d727ae21622236bf5f6d. Windows x64 [CI38639339105](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38639339105) SUCCESS, including actual C++ stick-role matrix (native swap ON/OFF, physical camera Left/Right, movement retained during aim injection/filtering, buttons/triggers retained). Archive publication commit 77bb0b43e99032dc36a331a3f516bd2ae1f6305e.

[Download cumulative V1.34 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.34_NATIVE_ACTIONS_STICK_ROLES_TEST.zip), 777132 bytes, SHA256 `8D91A23547D30EA335A8E0FD69B27A9CA1E8189183A136438A12179483B8A561`. Independently verified four root files, both binaries PE AMD64, embedded ASI 1.34 identity and direct URL download hash. Installed with game closed; ASI/loader destination hashes matched; personal INI stayed exactly AB2F95CD2AE49AD1706E1E9A420FEEDB1E416D25FCA157E83897554EF2C3E095. Prior V1.33 backup: `../audit-v131/install-backup-before-v134-20261014-232427/`. Installed README is refreshed with this post-CI receipt; the immutable ZIP contains the pre-CI cumulative source README.

Complete TPS camera/opposite movement roles and native swap compensation; native pause now owns copied camera freeze; mouse native target requests continue through release. Existing wheel selection guards retained; passive native radial visibility is diagnostic only. **Secondary impacts, post-fire turn, native swap read in-game and LB/keyboard wheel-follow behavior still require user tests; automatic support for every in-game wheel/fire rebind is NOT claimed.** Next: test those cases FOOT/HORSE, all three wheels including rebound controls, pause/scene/temporary native view. Protected feature modules/config defaults and every earlier ZIP are unchanged; main/archive heads verified unchanged. [Full evidence and acceptance](docs/V1_34_NATIVE_ACTIONS_STICK_ROLES.md).

## V1.34 source-preparation record (historical, superseded above)

Base debug 2291e80 / V1.33. User validates wheel sticks; incomplete stick exchange, wheel-control camera freeze, sideways secondary and post-fire turn remain reported. V1.34 maps camera/opposite movement roles consistently and reads the native swap setting, freezes copied view only for real native pause (or genuinely missing wheel pose), and lets native mouse target requests retain camera-center targeting after button release. UI option describes full role exchange. No new button-specific binding. Legacy configurable wheel-selection guards are preserved while native radial visibility is observed passively; full native action/rebind coverage is NOT solved or claimed. Native scene lifecycle, R3 helper, RawInput/Alt-Tab, horse, Synergy, config defaults and previous archives are preserved. **EXPERIMENTAL, source prepared; compile/install pending.** [Evidence, rollback and test criteria](docs/V1_34_NATIVE_ACTIONS_STICK_ROLES.md).

## V1.33 Windows x64 verified and installed (2026-10-14)

[Download V1.33 cumulative TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.33_CAMERA_CENTER_UI_PAD_TEST.zip) | [Windows CI 38633252781 SUCCESS](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38633252781). Exactly four root files, both binaries AMD64, ZIP 774986 bytes, SHA256 EFF76B7514FFB69F957ABB7FE9688C5122F29C7F2EC917E4ED32B157940D040E. Source 52ee7b4; archive 5e60284. Installed locally with game closed, destination hashes verified, V1.32 backup and personal INI preserved. **CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL, gameplay validation pending.** [Delivery, changes, risks and next test](docs/V1_33_CAMERA_CENTER_UI_PAD.md). This supersedes the pre-CI pending status below.

## V1.33 source prepared - camera-center mouse / UI stick selection (historical pre-CI snapshot)

V1.32 user test validates retained TPS in all three wheels and native handoff for the tested cinematic; equipment-wheel right-stick selection regresses. V1.33 adds native UI ownership to both input-suppression gates and aims mouse attacks through the camera center, retaining V1.32 collision/horse/camera behavior and existing controller gameplay. [Full evidence, bounded repair, checks and next test](docs/V1_33_CAMERA_CENTER_UI_PAD.md).

**NOT BUILT / NOT INSTALLED.** Source checks passed; publication and Windows CI are pending restoration of the automatic approval reviewer after a usage-limit review failure. No V1.33 download exists yet. V1.32 below remains the last verified Windows package.

## V1.32 Windows x64 build verified (2026-10-14)

[Download V1.32 cumulative TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.32_NATIVE_TARGET_CAMERA_OWNERSHIP_TEST.zip) | [Successful Windows CI 38608331116](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38608331116). Exactly four root files, both binaries AMD64, 775037 bytes; SHA256 C0EE1089862D61D37BC355724BA3DB2872346F520828BDFE4F56B709D2EA7124. **CONSTRUIT / CONTROLE / EXPERIMENTAL, in-game validation pending.** This verified-delivery entry supersedes the pre-CI pending status below. Native target/cinematic coverage still requires testing; keep older archives available.

## V1.32 native mouse target / camera ownership TEST (2026-10-14)

User V1.31 test rejects mounted aiming and missed cinematic handoff; pause/wheels also returned overhead with the cinematic option OFF. V1.32 replaces mouse yaw edits with one camera-reticle 3D target path through the game's native collision helper, with a point ahead on no hit. Controller input remains unchanged. Camera policy uses native cinematic lifecycle and native world pause/known radial state, retaining TPS composition during UI and preserving the configured temporary-native shortcut. If that shortcut shares a wheel key, its explicit native view still takes precedence. [Full exact-binary audit, limits, preservation and test plan](docs/V1_32_NATIVE_TARGET_CAMERA_OWNERSHIP.md).

**EXPERIMENTAL, build verification pending.** This is a new native targeting experiment, not yet proof of working projectile accuracy, mounted coverage or every cinematic. No new settings or INI reset. Preserve all old ZIPs; debug only, main/archive frozen.

## 2026-10-14: V1.31 user failure report and exact native audit

The user reports mouse targeting still fails on foot/horse and the tested cinematic does not return to native view; pause still returns overhead. **V1.31 aiming and cinematic classification are not validated.** Exact supported installed EXE is now available and hash-verified. Native analysis found a mouse deprojection -> horizontal plane intersection -> 2D direction path (`0x5719A0`), plus a collision-target helper (`0x571E50`) and two device-dispatched caller blocks. Negative ray-plane intersection is a concrete backward-aim mechanism to verify against actual shots, not a proven complete fix. Camera native passthrough from stale actor poses and shared E/R3 temporary view is independent of cinematic ENTER events; this session has zero ENTER. [Full native evidence, log audit and implementation gates](docs/V1_31_NATIVE_TARGETING_LIFECYCLE_AUDIT.md).

Documentation-only audit on debug; no new build or gameplay changes. Preserve mouse RawInput, Alt-Tab, controller/XInput/RT/RB/R3, native wheels, horse, gauges, INI and all numbered ZIPs. V1.31 remains the last compiled experiment; no V1.32 aiming fix is claimed.

## V1.31 verified Windows x64 CI and preserved test ZIP, 2026-10-14

[Build run 38592694757](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38592694757) completed SUCCESS: CMake Windows x64 configure, Release compile, four exact root ZIP files (`dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`), artifact uploaded and numbered archive committed to `debug`. [Permanent V1.31 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.31_MOUSE_RETICLE_CINEMA_CLASSIFIER_TEST.zip) size **770,718 bytes**, Git blob SHA-1 `c209a723075ad7407bb58981658f96cdc0b88db0`. V1.30 (768,166 bytes) / V1.29 (765,354 bytes) retained. Frozen `main` remains `2bc3122c1ae07b7333798462f85c1d9fd3a739f7` and `archive` remains `10e0f220b85022bc9269e0cb9390a811a3b1cf89`. No in-game claims: camera heuristic, foot look yaw and gun trajectory **require user validation**; vertical mounted bullet raycast not patched. Controller/XInput behavior has no source changes in this build.

## V1.31 - Mouse-only on-foot reticle heading + CINEMATIC SHORT_CUT / pause-like camera separation (TEST)

[Download V1.31 cumulative TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.31_MOUSE_RETICLE_CINEMA_CLASSIFIER_TEST.zip) | `debug` only, gameplay validation pending.

User's V1.30 log confirms 47ms `REJECTED` with a **~66,000-unit native camera jump** (user reports real cutscene), while the long `ENTER` (20.4 sec) showed no native camera movement despite actor absence (user reports pause menu). V1.31 now keeps a last-good native gameplay view snapshot, ignores actor loss alone when native camera remains static (`STATIC_NO_ACTOR`, pause/menu **possible**), and labels short huge jumps `SHORT_CUT` instead of `REJECTED`; real native camera passthrough and 500ms sustained candidate gate unchanged. V1.30 TPS distance threshold still active.

Mouse aim is now **isolated from genuine gamepad inputs**. During mouse-only foot firing, the verified native local-player look getter gets the horizontal azimuth of the V1.30 **screen-reticle world ray**, rather than the fixed camera-center heading. During mouse-only horse fire, native horse/player mouse aim is preserved, reflecting the user's report that mounted aim is mostly correct. **No XInput/controller code changed**, including RT/RB, R3 and radial/chord/stick behavior. This is an early functional mouse yaw test, **not a fix for native bullet raycasts or the mounted upper-half backwards shots**, which still require verified exact-game weapon code. [Full V1.31 technical notebook](docs/V1_31_MOUSE_FOOT_RETICLE_CINEMA_SHORT_CUT.md). Preserve old ZIPs; no main/archive changes.

## V1.30 CI verified / permanent debug ZIP (2026-10-14)

GitHub Actions [run 38587210828](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38587210828) completed **SUCCESS**: Windows x64 CMake configure, Release compilation, exactly-four-root-files ZIP validation, Actions upload, preservation and commit in `debug/downloads/`. [Permanent V1.30 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.30_RETICLE_WORLD_RAY_CINEMA_TEST.zip), size **768,166 bytes**, Git blob SHA-1 `b35917cf069a3cf823da254964b1e00d5c3cccec`. Older V1.29 (765,354), V1.28 (763,306) retained. Main head remains `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive head `10e0f220b85022bc9269e0cb9390a811a3b1cf89`. Compile and ZIP validation do **not** constitute in-game success; reticle WORLD_RAY is a read-only mathematical probe, **NOT** a native projectile hit or bullet redirection.

## V1.30 - Real TPS camera screen-reticle WORLD-RAY trace (read-only) + tighter cinematic distance (TEST)

[Download V1.30 cumulative TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.30_RETICLE_WORLD_RAY_CINEMA_TEST.zip) · `debug` only, compilation and gameplay validation pending.

The new V1.29 user log verified stable mouse `RawInput=27029`, **one fallback movement and zero cursor recaptures**. Of 24 tracked mouse shots, **19 were below the screen midpoint** and **8 mounted**, but there were **zero actual projectile/raycast detours**, so neither on-foot nor mounted shots are yet redirected correctly. V1.30 now reports the normalized world-space 3D ray going through each sampled cursor pixel from the **actual outgoing TPS camera** (position + yaw/pitch + FOV + aspect), separately from the unchanged native weapon behavior. This is mathematical read-only research, **NOT yet the aiming fix**. V1.29 recognized 3 cinematic candidates (2 ENTER, 1 REJECTED) all from `NO_ACTOR`, not `TPS_DISTANCE`. V1.30 tightens the camera-distance trigger to the baseline native isometric camera gap plus selected TPS distance **+40 units** (instead of +100 and the old 300 minimum), keeping the 500 ms validation and debug scene history. Gameplay systems and RawInput unchanged. [Detailed audit](docs/V1_30_RETICLE_WORLD_RAY_CINEMA_DISTANCE.md). Actual bullet redirection still requires inspecting the supported game's native weapon/raycast code; no target EXE is currently checked into this repository. All old ZIPs preserved.

## V1.29 Windows CI and permanent ZIP verified (2026-10-14)

GitHub Actions [run 38558950360](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38558950360) **SUCCESS**: Windows x64 configure + Release compilation, exact 4 root-file cumulative ZIP (`dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`) verified, artifact uploaded, numbered archive preserved in `debug/downloads`. [V1.29 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.29_TPS_DISTANCE_CURSOR_AIM_AUDIT_TEST.zip): **765,354 bytes**, Git blob SHA-1 `512761f52f91661f0fab1eb2e13a2755e982fcc2`. V1.28 (763,306) and V1.27 (759,159) remain. Frozen `main`: `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`. Frozen `archive`: `10e0f220b85022bc9269e0cb9390a811a3b1cf89`. User validation pending. **The screen reticle trajectory is NOT YET repaired**: the actual native projectile raycast source is not identified, even though cursor-click telemetry has been added and the new cutscene threshold is compiled.

## V1.29 - Cutscene candidates follow TPS distance; native reticle shot tracing audit (TEST)

[Download cumulative V1.29 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.29_TPS_DISTANCE_CURSOR_AIM_AUDIT_TEST.zip) | branch `debug`, in-game verification pending.

User's **V1.27** log confirms foot and horse shots still project from top-down aim even with TPS camera behind the player; mouse below horizontal can shoot BACKWARDS. **V1.29 does not claim to redirect native bullets**: actual weapon/raycast function has not been verified for this exact EXE; no risky unverified projectile detour. Instead `CURSOR_AIM_AUDIT` records first 24 mouse-down normalized reticle screen positions (+ camera yaw/pitch/FOV and mounted state) for grounded native raycast research. Mouse RawInput, primary/secondary bindings and mouse/camera TPS remain unchanged. The practical change fixes cutscene **candidate sensitivity**: camera is classified when the native camera moves past its normal isometric baseline by the **configured TPS distance**, plus a 400-unit minimum anti-jitter allowance; unlike comparing native gap directly with tiny rendered TPS distance (~320 vs native 2130), this does not mark all gameplay as cinematic. Original huge-gap fallback, 500ms sustained transition, V1.28 candidate list and user annotation preserved. The panel shows `TPS_DISTANCE`, actual 3D gap, baseline, selected TPS distance, and candidate threshold. [V1.29 full technical notes](docs/V1_29_AIM_RAYCAST_AND_TPS_DISTANCE_CUTSCENE.md). `main` / `archive` frozen and all prior ZIPs retained.

## V1.28 Windows build and publication verified (2026-10-14)

GitHub Actions [run 38557275092](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38557275092) **SUCCESS**: Windows x64 configure, Release compile, exactly four root-file ZIP validation, CI artifact upload, permanent archive commit and push to `debug`. [V1.28 cumulative TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.28_CINEMATIC_HISTORY_DEBUG_TEST.zip): **763,306 bytes**, Git blob SHA-1 `76008c8329e1ef0448815995df4a1b8146216d2b`. V1.27 (759,159 bytes) and V1.26 (756,898 bytes) preserved. `main` SHA `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, `archive` SHA `10e0f220b85022bc9269e0cb9390a811a3b1cf89` unchanged. In-game detection/history usability is **not yet tested**. Event history still provides only camera heuristics, not game asset names.

## V1.28 - TEMPORARY cutscene detection list and missed-scene marker (DEBUG TEST)

[Download cumulative V1.28 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.28_CINEMATIC_HISTORY_DEBUG_TEST.zip) | `debug` branch, Windows/gameplay verification pending.

The user reports **2 of 3** cinematics return to native camera; V1.28 adds an accessible **Camera > Cutscenes: detection history (DEBUG)** panel listing last 24 `CANDIDATE`, `ENTER`, `EXIT`, `REJECTED`, `RESET`, and `USER_MARK` events with per-candidate ID, native yaw/pitch/FOV and reason. A **Mark cinematic playing NOW** button lets the user tag an on-screen cinematic **missed by the camera heuristic**, without forcing TPS suspension or altering gameplay. Each event is reported in `DarksidersGenesisMod.log` (`CINEMATIC_DEBUG`); last 24 entries are summarized at clean exit (`CINEMATIC_LIST`). These are *heuristic camera events, not actual cutscene asset names*, and the third in-place cinematic may be invisible until manually marked. No changes to TPS mouse RawInput, configurable shooting, horse camera, native cinematic behavior, resource gauges or startup warnings. Temporary study until authoritative native cinematic state is identified. [Documentation and test instructions](docs/V1_28_CINEMATIC_HISTORY_DEBUG.md).

## V1.27 Windows build / package verified (2026-10-14)

GitHub Actions [run 38555201166](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38555201166) **SUCCESS**: Windows x64 configure/Release build, archive exactly four root files (`dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`), upload and permanent git push to debug. [V1.27 cumulative TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.27_MOUNTED_MOUSE_FACING_AUDIT_TEST.zip), **759,159 bytes**, Git blob SHA-1 `8db9a8aca29c9d101f9a03c4bcf2adced5840d67`. Older V1.25/V1.26 ZIPs retained. `main` remains `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`; `archive` remains `10e0f220b85022bc9269e0cb9390a811a3b1cf89`. User gaming validation remains pending. **Mounted bullet raycast and idle body orientation are not fixed by this build**; only the mounted mouse-fire camera-pose continuity is improved and diagnostic evidence added.

## V1.27 mounted mouse aiming pose and player-facing native audit (TEST)

[Download V1.27 cumulative TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.27_MOUNTED_MOUSE_FACING_AUDIT_TEST.zip) | branch `debug`, gameplay test pending.

User confirms V1.26 mouse motion works and native cutscene camera returned correctly in **2 of 3** scenes; remaining mounted shot trajectory and idle character orientation are unresolved. V1.27 adds **mouse fire state to previously gamepad-only native horse camera-pose stabilization**, scoped to an actual fresh mounted view. It does **not** redirect native bullets and must not be claimed to fix mounted accuracy. Read-only `MOUNTED_MOUSE_LOOK_GETTER`, `MOUNTED_MOUSE_LOOK_ROTATION`, `FACING_NATIVE`, `FACING_AUDIT` and shutdown `FACING_CAMERA totals` distinguish actor yaw, horse yaw, camera yaw and the actual native getter callsites. No speculative native mesh/root rotation or old rejected bOrientRotationToMovement patch. Proven RawInput registration recovery, dual mouse shooting, vanilla mounted aim, gamepad, cinematics, 2 warning pages, horse stats, synergy x2 remain unchanged. [Full V1.27 technical audit](docs/V1_27_MOUNTED_MOUSE_FACING_AUDIT.md). Full mounted raycast/idle facing fixes require validating the authoritative game-native paths, not tweaking reticle visibility.

## V1.26 Windows CI and cumulative ZIP verified on 2026-10-14

GitHub Actions [run 38544279229](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38544279229) completed **SUCCESS**: Visual Studio Windows x64 configuration and Release compilation, ZIP verified exactly four root files `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`, artifact uploaded, archive committed to `debug`. [Permanent V1.26 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.26_PERSISTENT_RAWINPUT_TEST.zip): **756,898 bytes**, Git blob SHA-1 `29445b6fc0b57001fdae481a2772174a6be534d1`. V1.25 and V1.24 remain present. Frozen `main` remains `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, `archive` remains `10e0f220b85022bc9269e0cb9390a811a3b1cf89`. This verifies compilation and packaging, **not** user-observed no-warp FPS-style gameplay. The root cause of the engine removing RawInput registration is not yet identified.

## V1.26 - Persistent RawInput registration recovery (TEST)

[Download cumulative V1.26 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.26_PERSISTENT_RAWINPUT_TEST.zip) | `debug` only, gameplay validation pending.

The V1.25 log recovers 46,193 relative mouse packets versus 67 in V1.24, with just 324 absolute fallback moves. But the game's mouse RawInput registration repeatedly disappears, and V1.25 exhausted its hard **8-attempt session cap**, ending `lastOwnerState=0`. V1.26 replaces that cap with **a safe minimum 2-second interval between recovery attempts** whenever no process-wide mouse registration exists. It never takes over a native/other HWND registration. Real relative deltas remain the TPS camera source; V1.23 edge recenter remains a fallback only when RawInput goes silent. This is a targeted camera stability fix, not yet proof that cursor recentering can be fully retired. Both V1.24 configurable mouse firing buttons, Camera Forward, controller, gear wheel, mounted controls, Synergy x2, Skip Logos and Skip Intro are preserved. V1.25 also confirmed **Synergy 35/35 positive x2**, **Wrath candidate +6 without AddWrath calls**, two stable active warning identity candidates and still **heuristic-only** cinematic detection. Skip Warnings remains OFF; Wrath/Ultimate boosts unverified. [Technical evidence and V1.26 test plan](docs/V1_26_PERSISTENT_RAWINPUT_AND_AUDIT.md). Old ZIPs remain in `debug/downloads/`.

## V1.25 CI and permanent ZIP publication verified (2026-10-14)

GitHub Actions [run 38542601698](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38542601698): **SUCCESS**, including Windows x64 CMake configuration, Release compilation, exact-four-root-files archive inspection, upload and permanent `debug` push. [Permanent V1.25 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.25_RELATIVE_RAW_RECOVERY_TEST.zip): **756,018 bytes**, Git blob SHA-1 `16dc0637d7ac86c130d8d2d7055af02b79bed801`. V1.24 size 753,948 bytes and V1.23 size 752,055 bytes remain untouched. No modifications to `main` (2bc3122c1ae07b7333798462f85c1d9fd3a739f7) or `archive` (10e0f220b85022bc9269e0cb9390a811a3b1cf89). CI success verifies build and packaging only, **not** real in-game relative-input continuity or a working warning skip. Validation remains pending.

## V1.25 - Continuous relative mouse / RawInput recovery and native studies (TEST)

[Download V1.25 cumulative TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.25_RELATIVE_RAW_RECOVERY_TEST.zip) | debug only, gameplay validation pending.

V1.24 preserved the two working mouse fire bindings but lost almost all native RawInput mouse events (67 vs 17,154 on user-confirmed V1.23). V1.25 polls actual process-level RawInput ownership every ~1.3 seconds, fixes Windows registration enumeration, and re-arms it only when no native mouse owner exists, without stealing input from other game windows. While real RawInput is fresh, there is **NO cursor recentering**; the validated V1.23 edge warp stays only as a failsafe during relative-input outages. [New detailed logs and test instructions](docs/V1_25_RAW_RELATIVE_AND_AUDITS.md). Read-only Synergy/Wrath/Ultimate caller audits, warning identity candidates and cutscene camera classification help identify the remaining native paths. **Skip Warnings stays OFF; Wrath/Ultimate multipliers and real cutscene/dialogue recognition are not yet verified.** Mouse primary/secondary rebind, mounted / gamepad, horse and all confirmed options retained. V1.23 and V1.24 ZIPs preserved.

## V1.24 Windows x64 build and publication verified (2026-10-14)

GitHub Actions [run 38537645671](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38537645671) completed **SUCCESS**: configure, Release compile, four-root-file ZIP verification, artifact upload, and permanent `debug` archive publication. [Download V1.24 cumulative TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.24_TPS_MOUSE_FIRE_AUDIT_TEST.zip): **753,948 bytes**, Git blob SHA-1 `4048d7eaa1d23ef35b4560ed737065ebfc96c937`. Archived V1.23 ZIP remains present: 752,055 bytes, SHA-1 `567ad7f7854f515ae4f3e091dd87e4a5c65dc013`. The workflow checked exactly `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, and `README.md` at ZIP root. `main` and `archive` branch heads are unchanged. **Windows compilation is not in-game verification**: native secondary shot direction and alternate button remapping remain EXPERIMENTAL awaiting the user's test.

## V1.24 - TPS mouse fire bindings + alternate Camera Forward + safe audits (TEST)

[Download cumulative V1.24 ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.24_TPS_MOUSE_FIRE_AUDIT_TEST.zip) · `debug` only · gameplay validation pending.

Following user confirmation that V1.23 now rotates the TPS camera continuously, V1.24 adds **two TPS-only mouse fire selectors** under Camera > Third Person: Left/Right/Middle/X1/X2/Unbound, default Left primary and Right secondary. Selected clicks translate to native left/right firing events during TPS; the two mouse fire buttons use the same existing **Camera Forward** native look fix (the RMB alternate was not covered before). No controller/fire, R3/wheel, edge recapture, cursor hide, mount, Synergy or updater changes. Wrath log events are read-only correlated to verified Synergy gain timestamps; warning record fingerprints are recorded without skipping/altering native initialization. **Skip Warnings remains disabled; true Wrath/Ultimate multiplier paths are still unverified.** [V1.24 audit and tests](docs/V1_24_TPS_FIRE_GAUGES_WARNINGS.md). V1.23 ZIP preserved.

## V1.23 - TPS mouse edge recapture (Windows x64 TEST)

[Download cumulative V1.23 ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.23_TPS_EDGE_RECAPTURE_TEST.zip) · `debug` only · game validation pending.

User confirms V1.22 works except TPS mouse orbit still stops at the window edge. Last log: 687 relative RawInput packets, including 77 at the border, with 1,354 WM_MOUSEMOVE fallbacks. V1.23 experimentally re-centers **only the OS pointer at the window edge during active TPS**, after physical motion, with a cooldown and filtered synthetic WM_MOUSEMOVE. There is still NO Hide Reticle, native cursor visibility hook or F4 reticle shortcut. Relative camera, Camera Forward, horse, controller, gauges, startup, cutscene handling, menu and INI defaults are unchanged. [Technical note and test](docs/V1_23_TPS_EDGE_RECAPTURE.md). V1.22 remains available for rollback.

## V1.22 - Reticle hiding removed from ASI (Windows x64 TEST)

[Download cumulative V1.22 ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.22_NO_RETICLE_HIDE_TEST.zip) · `debug` only · in-game validation pending.

V1.22 **completely removes** native Win32 `SetCursor` and game UI `IsCursorVisible` reticle hiding hooks, TPS automatic cursor suppression, manual **Hide Reticle** menu/INI/runtime setting and its **F4** action (F4 now **None**). Retired keys in personal INIs are ignored, and cleaned only on explicit save. Other user defaults and features are unchanged: V1.20 relative mouse, TPS Camera Forward, F1 Hide HUD, blue aiming effects, gauges, native cutscene handoff, safe Skip Logos, embedded updater. Windows x64 CI verified four root files. **[V1.22 technical audit: warnings, Wrath/Ultimate, cutscenes and latest log](docs/V1_22_REMOVE_RETICLE_FEATURE.md)**.

## V1.21 - Unified Camera Reticle menu-only TPS policy (test)

[**V1.21 cumulative Windows x64 TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.21_RETICLE_MENU_ONLY_TEST.zip) · debug branch, user runtime test pending.

After V1.20 RawInput mouse was user-reported working until toggling two conflicting cursor visibility options, **Hide Reticle** has been moved from Gameplay to **Camera > General**, keeping its single F4 hotkey and default OFF. The former **Show Mouse Pointer in TPS** option is removed, replaced with automatic **Show Reticle in Menus Only While TPS** code: hide cursor in live TPS gameplay; allow native cursor in mod menu/equipment wheels and inactive/non-TPS views. Deprecated ShowTPSMousePointer key is ignored and cleared upon explicit INI save. No other default changes; TPS still defaults ON. V1.20 relative mouse, Camera Forward firing, cutscene/gauge diagnostics, native Skip Logos and updater retained. See [technical note and tests](docs/V1_21_RETICLE_MENU_ONLY.md).

## V1.20 Experimental - Infinite TPS mouse via relative Raw Input; native cutscene/gauge audit

[**V1.20 cumulative Windows x64 TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.20_RELATIVE_MOUSE_CUTSCENE_GAUGES_TEST.zip) · debug branch · game test pending.

User confirms V1.19 mouse TPS is promising but physically stops turning at screen border. V1.20 acquires raw relative mouse movement when the game has not registered it, **preserving any native RawInput registration and all native input messages**, with no cursor warp, no controller emulation, and no extra mouse fire mode. The existing V1.19 unified Camera Forward LMB/controller aiming is kept. Current ThirdPerson=1 and ShowTPSMousePointer=1 defaults and all user INIs are unchanged.

Also adds **read-only cutscene** camera rejection/suspend/resume reason logs and **read-only Wrath gain/spend correlation** with native local-player damage and held fire. Synergy native positive gain multiplier unchanged; Ultimate/Wrath charge multiplication is not yet verified. This does not skip or change cutscenes. [V1.20 technical notebook and test plan](docs/V1_20_RELATIVE_MOUSE_CUTSCENE_GAUGES.md). All numbered old archives retained, updater inside ASI unchanged.

## V1.19 test: TPS mouse Camera Forward + user-requested defaults

[**V1.19 cumulative Windows x64 TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.19_TPS_MOUSE_FORWARD_TEST.zip) · `debug` only, user game test pending.

Rebuilt on **V1.18's canonical V1.15 code**, NOT rejected V1.16/1.17 hooks. Exactly **two defaults** changed at user's request: `ThirdPerson=1` and `ShowTPSMousePointer=1`, with the mouse pointer control *inside* active Third Person, and runtime pointer path disabled when TPS is OFF. Updater still preserves existing personal INIs.

**New native TPS mouse input** (no activation checkbox, no warping, no synthetic XInput): mouse relative motion rotates the same TPS camera as original controller orbit; firing by LMB uses the same camera-forward native look vector/rotation hooks used by controller, not a competing aim mode. Movement and actual projectile/raycast behavior await user game test. **Skip Warnings** appears in Startup UI but is explicitly disabled/unavailable; no unsafe initialization skips were reintroduced. All other settings and defaults unchanged. [Detailed audit and tests](docs/V1_19_TPS_MOUSE_FORWARD.md).

## V1.18: hard rollback to validated V1.15 TPS + separate read-only audits

[**Download V1.18 cumulative debug TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.18_CLEAN_V115_RESEARCH_TEST.zip) · Windows x64, in-game validation pending.

**V1.16 and V1.17 are REJECTED** following user's in-game TPS regression report. V1.18 is rebuilt directly from V1.15 code for TPS, cursor, gamepad, startup and config defaults. The **rejected V1.17 stock cross cursor replacement is not included**. All preceding historical ZIPs are preserved; embedded ASI updater remains user-validated. V1.15 defaults are restored: `ThirdPerson=0` and `ShowTPSMousePointer=0`. The updater preserves existing user INI, so explicit values from prior version are not overwritten; update them in Camera or reset defaults in About if needed.

**Three isolated research threads** in V1.18 (all observation only): safe WndProc mouse event counters (camera injection=0); extra positive/stable Wrath candidate timing from live-player native callback, with Synergy multiplier unchanged; native startup warning record flag summaries without any skipped initialization. **Skip Logos remains the V1.15 implementation** to allow ON/OFF testing of the cross-reticle bug. Do not assume warning records prove two screens are displayed. Next V1.19 will depend on user validation, not on copying previously rejected input patches. [Technical note and test protocol](docs/V1_18_CLEAN_RECOVERY_RESEARCH.md).

## V1.17 experimental - TPS native Windows cross cursor repair

[**Download V1.17 cumulative TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.17_CROSS_CURSOR_REPAIR_TEST.zip) · debug only, in-game validation pending.

V1.16 user testing reported the **cross-shaped cursor bug again** with ThirdPerson=1 and ShowTPSMousePointer=1 as requested defaults. V1.17 keeps both defaults ON and addresses only the **Windows stock IDC_CROSS** cursor in verified active TPS gameplay, replacing its visible shape with an arrow while allowing the pointer to remain on. Overlay, wheels, cutscenes, Alt-Tab, Hide Reticle and all gameplay logic remain isolated. V1.17 logs bounded `TPS V1.17 CURSOR_PROBE` and `CROSS_CURSOR` messages; if a custom or in-engine crosshair is responsible, the targeted OS replacement alone may not cure it, and user log will help determine that. Safe logo initialization preserved and Skip Warning still disabled. See [V1.17 technical notes](docs/V1_17_CROSS_CURSOR_REPAIR.md).

## V1.16 experimental: TPS ON + visible native pointer by default, safe StartupScreens audit

[**Download V1.16 cumulative Windows x64 TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.16_TPS_DEFAULTS_STARTUP_AUDIT_TEST.zip) · debug only · in-game validation pending.

New defaults **`ThirdPerson=1` and `ShowTPSMousePointer=1`** in the bundled INI, ASI runtime initial state, full factory reset, and missing-key INI fallbacks. This permits the game's native pointer during TPS; it does **not** rotate the camera with the mouse. The updater deliberately preserves an existing personal INI, so explicitly saved zero values stay zero until changed in the menu or manually; About > Factory Reset applies both new defaults but resets other settings too. 

Comparative audit of old aggressive Skip Logos V0.19A/B/C versus current V0.52 safe targeted logo path confirms bypassing MoviePlayer setup caused cross-shaped cursor regression; no unsafe warning bypass is restored. True two-warning draw path still TODO. Mouse camera orbit from V0.98 is proposed as a separately switchable, OFF-by-default later experiment rather than mixed into the pointer default. **Next version will be V1.17, no letter suffix.** [Full comparison and test checklist](docs/V1_16_TPS_DEFAULTS_STARTUP_COMPARISON.md).

## V1.15 experimental: resource gain audit (no gameplay changes)

[**V1.15 cumulative Windows TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.15_RESOURCE_GAINS_AUDIT_TEST.zip) · `debug` · game validation pending.

V1.14 log: Synergy **317 total native calls, 317 local, 113 positive, 113 amplified**. The nonpositive 204 calls are not proof of missed pickups. Synergy already multiplies a *native received gain* `A*B*C`, not an object or the bar's absolute value. The V1.15 read-only audit reports separate positive/zero/negative additive amounts, source factor variation, direct setter gains vs helper gains, and whether any actual positive gain was not boosted. It does not change multipliers or gameplay. Wrath/Ultimate true gain paths still require confirmation. **Every future build receives the next numeric version: V1.16, V1.17...**, no A/B/C/D/U. See [V1.15 technical note](docs/V1_15_RESOURCE_GAINS_AUDIT.md). V1.14 builds preserved.

## V1.14U UI-fix - TPS mouse option in Camera, working factory reset feedback

[**V1.14U cumulative TEST ZIP (TPS UI + reset confirmation fix)**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.14_ZZ_TPS_RESET_FEEDBACK_TEST.zip) · debug only.

Moves **Show Mouse Pointer in TPS** from Gameplay/HUD to **Camera > Third Person camera**, retaining OFF by default and Hide Reticle priority. Fixes the **About / Info > Reset INI to Factory Defaults** confirmation modal (previously blocked by an ImGui popup ID-stack mismatch), adds persistent SUCCESS/FAILED/CANCELLED feedback and detailed log events, and keeps existing dated INI backup and game saves safety. In-game Updates tab and embedded after-exit installer remain intact. [Technical log and test checklist](docs/V1_14U_RESET_MODAL_TPS_UI.md).

## V1.14U - IN-GAME ASI Updates menu (Windows x64 TEST)

[**Download cumulative V1.14U ZIP with integrated updater**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.14_Z_ASI_UPDATER_TEST.zip) · `debug` only · **Windows build verified, gameplay test pending**.

Press Insert in the game, open the **Updates** tab (before the last About/Info tab). `Beta updates` unchecked chooses **main** stable (currently V0.72), checked chooses **debug** experimental (current V1.14U). Click `Check for Updates`; choose `Install on Game Exit` and confirm to stage a mod update. `Restore Previous` queues rollback with confirmation. There are **no automatic downgrades**, and the menu warns when the selected channel offers an older build.

The post-exit helper is **embedded inside DarksidersGenesisMod.asi**, so the normal ZIP retains exactly FOUR root files. The helper waits for the game to close, verifies the exact ZIP checksum, game executable and Windows x64 binaries, backs up previous files under `%LOCALAPPDATA%\DarksidersGenesisEnhancedUpdater` and preserves a personalized mod INI. It never replaces loaded game DLLs. The original [V1.14 pointer/reset test ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.14_TPS_POINTER_INFO_RESET_TEST.zip) remains available. The new ASI updates do not automatically read or upload logs.

[Technical implementation and test checklist](docs/V1_14U_INMENU_UPDATER.md) · [Windows Actions CI](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38430137501).

## V1.14 experimental - TPS cursor option and reset in Info only

[**V1.14 cumulative Windows x64 TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.14_TPS_POINTER_INFO_RESET_TEST.zip) · `debug` only, gameplay validation pending.

Requested after V1.13: `Show Mouse Pointer in TPS` is OFF by default. ON lets the original game mouse cursor appear during TPS gameplay; `Hide Reticle` still overrides it. Consolidated the two duplicate factory reset actions into exactly one `Reset INI to Factory Defaults` under the last **About / Info** tab, with its existing confirmation and dated .bak backup; per-setting camera/keyboard Defaults remain untouched. See [V1.14 technical note](docs/V1_14_TPS_POINTER_INFO_RESET.md). Standalone updater remains separate pending true ASI integration.

## Standalone Windows Updater V1.0 | main / beta debug

[**Download DarksidersGenesisUpdater V1.0 (Windows x64)**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesisUpdater_V1.0_Windows.zip)

The updater is a **separate two-file ZIP**, not included in the mandatory four-root-file mod archives. Extract and run `DarksidersGenesisUpdater.exe`. Select the game executable directory once; the updater remembers it.
**Beta unchecked = main** (currently up to V0.72); **Beta checked = debug** (currently V1.13). The program checks GitHub when launched and can automatically install newer builds when the game is closed. An already present manually installed mod is NOT automatically overwritten on the first run; use Install Latest once. Never automatically downgrade from a newer debug build to older main. A manual downgrade requires confirmation.
Before installation the tool verifies the GitHub archive checksum, exact four ZIP entries, supported game EXE and x64 modules. Previous binaries go to dated backups under `%LOCALAPPDATA%` with Restore Previous button. An existing personal `DarksidersGenesisMod.ini` is **never overwritten**, and game saves/`Engine.ini` remain untouched. The updater must be open to check/download/install; it is not a background service or log-sync utility.
[Updater technical design, test plan and caveats](docs/UPDATER_V1_0.md). Windows CI compiled and packaged the updater; **gameplay installation test remains pending**.

## V1.13 experimental - Reset INI safely

[**V1.13 cumulative Windows x64 TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.13_SAFE_INI_FACTORY_RESET_TEST.zip) · debug only.

V1.12 menu usability validated. New **Gameplay > System and startup > Configuration > Reset INI to Factory Defaults** action, also available under Hotkeys. Confirmation required. Original mod INI is retained beside the file as a dated `.bak`; a fresh default INI removes obsolete keys and resets all mod settings/keybindings. Game saves and `Engine.ini` untouched. Restart recommended. Native gameplay functions unchanged. [Technical note and test checklist](docs/V1_13_SAFE_INI_FACTORY_RESET.md). Roll back using preserved V1.12 ZIP.

# Darksiders Genesis Enhanced PC Mod

Modern runtime enhancements for **Darksiders Genesis (PC)** using a local x64
DXGI proxy and ASI plugin.

The project is deliberately fail-open for normal runtime failures and
**fail-closed for unsupported game executables**.

## V1.12 experimental - resource status and responsive controls

[**V1.12 cumulative Windows x64 TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.12_RESOURCE_STATUS_RESPONSIVE_UI_TEST.zip)

Authoritative current summary; older sections below are historical. Based on cumulative V1.11 debug 0a13e93, retaining validated functions. Status: EXPERIMENTAL pending Windows CI and user gameplay validation. Main and archive untouched; existing archives preserved.
Common tuners wrap into two rows in smaller windows and no longer place status at a fixed pixel offset. Wrath/Ultimate clearly display unverified gain paths; Synergy retains its previously validated native additive path. Native hooks, INI defaults, keyboard mappings, horse, RT/RB, native LB+Y, R3, HUD and cameras are unchanged.
Wrath +0xEA0 -> +0x3A0 remains a candidate; AddWrath/AddChaos had zero observed callbacks. Skip Warnings remains OFF: safe presentation-only suppression has not been identified. Native startup initialization is preserved.
[Audit, acceptance tests and cumulative TODO](docs/V1_12_RESOURCE_STATUS_RESPONSIVE_UI.md). Rollback: preserved V1.11 ZIP. Exactly four root files; compilation does not establish gameplay success.

## V1.11 experimental | More readable overlay, grouped features, less log noise

[**V1.11 Windows x64 cumulative TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.11_OVERLAY_GROUPS_DEBUG_LITE_TEST.zip) · branch `debug` · four root files · previous builds archived · `main` unchanged.

The in-game overlay now uses collapsible **HUD, movement, combat/resources, horse, and system** families, with separate TPS/original camera groups. Hotstreak, **Wrath**, **Ultimate/Chaos**, and **Synergy** recharge controls share exactly the same checkbox, x0..100 multiplier, manual field, default button and spacing. Wrath/Ultimate are explicitly marked experimental, Synergy validated. Keybinds for TPS, horse and original-camera inputs use readable **Action | Current key | Change | Clear | Default** tables. F1-F12 remain on the Hotkeys tab. Short help/status text remains while bulky advanced debug panels have been removed.

The log now preserves actionable warnings, startup timeline and rare resource changes while limiting repetitive combat, reticle, gauge and input tracing. No gameplay aim/R3/horse/synergy changes, no TPS mouse work. No new attempt to skip the native warning loop.

**Boot order visually confirmed by user:** two logos, black-background **save data** warning, black-background **controller** warning, intro cinematic. The native startup definition array includes 3 entries with active flags 1/1/0, consistent with the two visible warnings. Which native callback *draws* these screens is not identified yet. Existing Skip Logos substitutes names; existing Skip Intro sets native `g.PlayIntroCinematicOnBoot=0`; **Skip Warnings remains OFF** until safe presentation-only suppression is found.

**Wrath research:** a candidate live field at player+0xEA0 -> component+0x3A0 has observed positive and negative changes; native `AddWrath` and `AddChaos` hooks still receive **zero** runtime callbacks. Do not confuse an installed hook with a working charge multiplier. Synergy additive gains are validated.

[**V1.11 technical notes and testing**](docs/V1_11_OVERLAY_USABILITY_LOG_CLEANUP.md) · [Current project state](docs/CURRENT_STATE.md).

## Current project state: V1.10 native charged ability, startup phase timeline

[**V1.10 Windows x64 cumulative TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.10_NATIVE_LBY_STARTUP_TIMELINE_TEST.zip) · source branch `debug` only; `main` frozen; older ZIPs preserved. Windows CI verifies four files at ZIP root.

**LB+Y rollback, user requested:** V1.09 demonstrated that the triggered charged ability does not follow the same input direction path as RT/RB; user observed it follows character direction or movement. V1.10 retires the experimental LB+Y stick-neutralization, special native look/rotation correction and custom Y translation. It leaves physical LB+Y to the game's own ability logic, preserves original LB-wheel behavior, and **does not touch validated RT/RB camera-forward stick injection**. R3 uses equipment, horses and Synergy unchanged. The old `TPSChargedFirePadMask` INI key is ignored for compatibility and not exposed in TPS overlay.

**Skip Warnings investigation, read-only:** The native `StartupScreens` array has three 0x40-byte records, with active flags 1/1/0 in the user's last logs. This array is processed by native code *before* the startup movie playlist descriptors are copied. The logo playlist has two known movies, `THQ_LogoBasic` and `AS_LogoBasic`, which the existing validated feature substitutes with non-existent names without skipping native MoviePlayer setup. Separately, Skip Intro sets `g.PlayIntroCinematicOnBoot=0` before game reaches its boot movie decision. **Preparing warning records is not proof the warning pages have been drawn.**

V1.10 adds actual startup event timing from DXGI proxy attach across ASI initialization, intro CVar installation, startup record observation and logo copy, up to first D3D11 Present. Search `Startup V1.10 TIMELINE` in the per-session log. Every stage has relative milliseconds, OS thread ID and an observed event name. The warning **skip itself remains disabled** until the rendering stage can be distinguished from essential initialization. [Full timeline research](docs/V1_10_STARTUP_TIMELINE_NATIVE_LBY.md) · [Current State](docs/CURRENT_STATE.md).

**Test requested:** cold launch, note visual screen order after logos and before intro/title, send full `DarksidersGenesisMod.log`; verify LB+Y native ability, normal RT/RB, native LB wheel and reticle. Wrath and Ultimate multiplier paths still unvalidated; Synergy proven. Mouse TPS postponed.

## Current project state: V1.09 simultaneous diagnostics (LB+Y, Wrath, Chaos, Warnings)

[**V1.09 cumulative TEST ZIP**](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.09_MULTI_PATH_NATIVE_AUDIT_TEST.zip) · branch `debug` only, `main` frozen, previous ZIPs preserved. Windows GitHub Actions builds four files at ZIP root.

User V1.08 in-game: LB+Y with **empty Wrath** appears to face correctly; with enough Wrath to actually execute a charged ability the projectile fires randomly. Compare to RT/RB, which actually receive **game-facing right-stick UP** whereas LB+Y V1.08 leaves right-stick neutral to avoid opening radial wheel and instead modifies `GetPlayerLookInput`. User log V1.08: 1333 LB+Y chord observations, 6108 native charge look edits, **0 native look rotation calls**, 373 ordinary right-stick UP injections, 0 Wrath/Chaos Add* calls, 57 Synergy boosts. Actual charged projectile source remains unknown.

**V1.09 = read-only multi-path audit, no speculative aiming fix:**
- Logs original controller input mode edges for ordinary RT/RB versus LB+Y versus wheel, raw right stick, native flags and timestamps; logs *native GetPlayerLookInput direct callsite RVA*, native direction before hook, camera yaw gap and time since input for both shooting modes. The runtime still applies the established V1.08 input behavior, unchanged.
- Samples an audited candidate native Wrath float (`validated local actor+0xEA0 -> component+0x3A0`) ONLY inside live player native look callback and with VirtualQuery checking, at most ~12/s. Tracks positive/negative jumps correlated to charged shot. This is a candidate: verify gameplay correlation before treating as confirmed resource.
- One-time signature-preserving **static direct-call reference scan** in exact game executable for native `AddWrath`, `AddChaos` and known `AddSynergyNative`. These are potential callsites, **not evidence those functions execute**.
- At original loader startup passively log bounded raw field words for all three existing StartupScreens records (active 1,1,0), to identify warning presentation later. **Skip Warnings remains disabled**: never bypass native UI initialization or risk cross-shaped cursor.

**Awaited gameplay tests:** press LB+Y once with Wrath empty and once with Wrath available, shoot ordinary RT/RB for comparison, gain/spend Wrath/Ultimate, check warning screens unchanged. Send one V1.09 log; useful tags `TPS V1.09 PAD EDGE`, `TPS V1.09 LOOK PHASE`, `TPS V1.09 WRATH READ-ONLY`, `Gauge V1.09 STATIC`, `StartupScreens V1.09 READONLY` and `TPS V1.09 PHASE totals`. Mouse TPS intentionally deferred.

[Technical design / test notes](docs/V1_09_MULTIPATH_AUDIT.md) · [Current state](docs/CURRENT_STATE.md).

## Current project state: V1.08 LB+Y charged-shot wheel correction

**debug only; main unchanged.** [V1.08 cumulative Windows x64 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.08_CHARGED_LOOK_NO_WHEEL_TEST.zip) (four root files, after Windows CI success).

**V1.07 rejected for TPS LB+Y:** user reports it now opens weapons wheel, instead of the native charged shot aiming correctly. Root issue: virtual right-stick UP while LB is held selects a radial wheel. V1.08 passes the game's LB + Y buttons unmodified; it neutralizes the game-facing right-stick axes to prevent wheel selection, while driving the game's **native look-input/rotation vector toward the TPS camera** during this specific charged chord, including mounted Vanilla Aim. Plain LB still opens/selects the wheel untouched, RT/RB still uses the previous synthetic UP right stick as before. Charged physical Y remains rebindable (TPSChargedFirePadMask), wheel LB rebind independently shared (TPSWheelPadMask). R3 remains selected-gear USE. Actual charged projectile direction still requires user testing; no projectile raycast has been patched.

**Unchanged from V1.07:** Hotstreak and Wrath/Ultimate/Synergy 0..100× sliders, default 2×; validated Synergy native additive hook (443 local events, 94 boosted on latest user log), all horse/camera/HUD/R3 and native skip intro/logo features. Wrath/Ultimate gain callbacks remain unobserved; Skip Warning remains safely pass-through.

[Technical audit V1.08](docs/V1_08_LBY_CHARGED_NATIVE_LOOK_WHEEL_SAFE.md) · [Current State](docs/CURRENT_STATE.md).

## Downloads (stable links)

**Latest experimental test ZIP:** [DarksidersGenesis_LATEST_TEST.zip](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases/download/test-build/DarksidersGenesis_LATEST_TEST.zip)

**Versioned V0.64 backup:** [DarksidersGenesis_V0.64.zip](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/main/downloads/DarksidersGenesis_V0.64.zip)

**Official validated release:** [GitHub Releases](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases) (currently V0.29).

The experimental `test-build` prerelease is updated automatically only after a successful Windows build.
Never distribute signed GitHub Actions artifact redirect links, since they expire.
See [DOWNLOAD_STANDARD.md](docs/DOWNLOAD_STANDARD.md) for our reusable policy across mod repositories.
## Historical development snapshot (early versions, superseded)

- Latest published binary release: **V0.29**
- Safety hotfix **V0.31** and lazy overlay **V0.32** retained
- Latest failed cursor experiment: **V0.33**
- Earlier V0.34 F5 reticle focus workaround is **not reliable**; reticle bug remains open
- Current status: **V0.38 loads successfully**; earlier V0.38 startup failure was a Windows Defender quarantine false alarm. V0.39 remains an optional loader-diagnostic build, not a required repair. V0.38 yaw and Alt-Tab changes still need separate in-game validation.
- Target executable:

```text
DarksidersGenesis-Win64-Shipping.exe
Size: 62,113,280 bytes
SHA-256: 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54
```

V0.17 calculates the executable SHA-256 at runtime. If the executable does not
match exactly, the overlay/log can still load but **gameplay hooks are not
installed**.


## Historical development and versioning policy (superseded)

Development now advances directly on `main`: one new numbered build per
version (V0.35, V0.36, V0.37...), recorded by normal Git commits. Do not
create a new `dev/v...` branch for every build. A published release is only
created after in-game validation; the latest release V0.29 stays unchanged
until a newer version is approved. Work-in-progress commits on `main` are
not release candidates.

Historic `dev/` branches are legacy experiments. Do not delete them until
their divergent Git history is confirmed reachable from a durable reference.

## Installation

Copy these files next to the game executable:

```text
dxgi.dll
DarksidersGenesisMod.asi
DarksidersGenesisMod.ini
```

Launch the game normally.

Default overlay key:

```text
Insert
```

The menu key and F1-F12 actions can be remapped from the overlay.

## Feature status

| Feature | Status |
|---|---|
| D3D11 / ImGui overlay | Stable |
| Rebindable menu key | Stable |
| Toggle HUD | Validated |
| Movement Speed | Validated; V0.17B restores the proven V0.14F local-pawn + Jump/Glide identity path |
| Action Recovery | Validated V0.8B tail-only policy |
| Jump Height | Validated |
| Glide / Flight Duration | Validated |
| Skip Intro | Validated |
| Hotstreak Charge | Validated |
| Pistol Damage | Functional heuristic: `BaseJuice > 0` |
| Melee Damage | Experimental heuristic: zero-juice outgoing records |
| Horse Speed | Validated V0.29 native movement offset correction; lifetime-safe V0.31 |
| Horse Sprint Duration | **Validated V0.27** - native HorseCharacter stamina drain; 0x vanilla, 5x default, 20x max |
| Horse Sprint Speed | Validated V0.29 native SprintingMaxSpeed; lifetime-safe V0.31 |
| Manual Reticle Focus Refresh | **Unreliable/experimental** - F5 focus pulse may help, but does not fix persistent cross-shaped reticle |
| FOV | Not implemented |
| Third Person camera | Not implemented |
| Skip Logos | **Validated V0.19C/V0.19D** - proprietary `StartupScreens` MoviePlayer attachment bypass |

Unimplemented controls are disabled in the V0.17 overlay/default configuration
instead of pretending to be active.

## Default hotkeys

```text
F1  Toggle HUD
F2  Movement Speed
F3  Action Recovery
F4  Skip Intro Videos
F5  Reticle Focus Test (V0.34 validated manual fix)
F6-F12  None
```

## V0.17 core architecture

The cleanup branch separates the runtime into explicit modules:

```text
dxgi_proxy.cpp
    local DXGI forwarding + ASI loading

DarksidersGenesisMod.cpp
    active hook orchestration + D3D11 renderer integration

ConfigStore.cpp
    INI + hotkeys + debounced persistence

RuntimeSettings.cpp
    atomic gameplay settings consumed by hook threads

TargetValidator.cpp
    exact executable size/SHA-256 gate

OverlayUi.cpp
    ImGui menu only, using an explicit context

HorseFeature.cpp
    isolated horse candidate logic
```

Historical/rejected hook families are no longer compiled into the runtime core.

## Horse reference

The authoritative horse research source is the user-supplied working reference
mod:

```text
ZZZ-Horse_P.pak
SHA-256: 3719ac840e1d0d7f137c9322580a3abe58cc5cf93d4b1ea97352c66490d3a920
```

Proven vanilla -> modified values include:

```text
MaxWalkSpeed                       1300 -> 1500
MaxAcceleration                     600 -> 700
BrakingFrictionFactor                 1 -> 2
GallopSpawnSpeedThreshold            300 -> 400

StaminaRecoveryPercentageRate         15 -> 100
StaminaTotalRecoveryPercentageRate    40 -> 100
StaminaSprintPercentageRate           25 -> 0
```

See `docs/REFERENCE_PAK_AUDIT.md` for the complete reference audit.

## Configuration behavior

Gameplay hook threads do not read the editable UI config directly.

V0.17 publishes settings into atomic runtime values immediately. INI persistence
is debounced, so dragging a slider no longer rewrites the entire file every
render frame.

The explicit **Save** button still persists immediately.

## Documentation

- `docs/FULL_AUDIT_V0.16A.md` - audit that triggered the V0.17 cleanup
- `docs/CORE_CLEANUP_V0.17.md` - V0.17 implementation / verification report
- `docs/REFERENCE_PAK_AUDIT.md` - supplied reference mod findings
- `docs/DODGE_RECOVERY_AUDIT.md` - recovery investigation
- `docs/TECHNICAL_NOTEBOOK.md` - complete historical development notebook

## Build

The project uses CMake and GitHub Actions.

MinHook and Dear ImGui are pinned to exact upstream commits for reproducible
builds.

The test artifact remains a flat package containing:

```text
dxgi.dll
DarksidersGenesisMod.asi
DarksidersGenesisMod.ini
README.md
```

## Release policy

Experimental branches are preserved as technical history.

`main` is not advanced merely because a candidate compiles. Gameplay changes
must be validated in game before promotion.


## V0.17B player-identity rollback fix

The first V0.17 cleanup introduced a new structural identity heuristic using
MaxWalkSpeed / MaxAcceleration. Runtime testing proved that heuristic rejected
the real player and therefore blocked Movement Speed, Jump, Glide and all
features depending on the captured local-player pointer.

V0.17B removes that unvalidated module completely and restores the path already
proven in V0.14F:

```text
APawn::IsLocallyControlled
  + valid JumpZ
  + valid DoubleJumpZ
  + valid GlideDuration
  = local player movement component
```

Only after that proven movement signature succeeds is
`g_localPlayerCharacter` updated.

The V0.17 cleanup work for target validation, atomic runtime settings,
ConfigStore, OverlayUi, debounced INI persistence, shutdown and dead-code
removal is retained.


## Working rule / fil rouge

From this point forward the project follows a deliberately simple rule:

1. Keep validated behavior unless a test proves it wrong.
2. Do not add a second heuristic when an already validated path exists.
3. A feature owns one clear primitive and one clear responsibility.
4. Diagnostics must be bounded and must not become fallback trees.
5. Rejected experiments are removed from runtime code and kept in Git/docs.
6. Change one gameplay subject at a time.
7. The cumulative README records every accepted direction change and rejection.

### V0.17B final player rollback policy

Player identity is restored exactly to the previously validated V0.14F rule:

```text
APawn::IsLocallyControlled(characterOwner)
    -> g_localPlayerCharacter = characterOwner
```

JumpZ / DoubleJumpZ / GlideDuration validation remains useful, but only protects
Jump/Glide property writes. It no longer gates Movement Speed, Action Recovery,
Damage or the local-player pointer.

This is intentionally simpler than the rejected V0.17 structural identity
module and the intermediate V0.17B combined filter.


## V0.18 Skip Logos - single-path test

Scope is intentionally limited to Skip Logos.

External verification confirms the two company startup movies are loose files:

```text
ProjectMayhem/Content/Movies/THQ_LogoBasic.mp4
ProjectMayhem/Content/Movies/AS_LogoBasic.mp4
```

The rejected V0.15 paths stay rejected:

```text
late CreateFile/GetFileAttributes hooks
PEB command-line -nostartupmovies injection
literal nostartupmovies query patch
```

V0.18 uses one new primitive only:

```text
dxgi.dll DllMain, before game entry
    -> patch the main EXE import table
    -> CreateFileW
    -> GetFileAttributesW
    -> GetFileAttributesExW
    -> return FILE_NOT_FOUND only for the two exact logo basenames
```

No game file is renamed, deleted or edited.

No story/cutscene movie is targeted. In particular
`CG_Intro_LowVi.mp4` is intentionally untouched.

This test is forced ON at boot because the hook must exist before UE4 startup.
Runtime configuration will only be added after the primitive is validated.

The ASI log reports both the early hook status and how many matching file
requests were blocked.


## V0.18B - Skip Logos / Skip Intro separation

The menu and configuration now reflect the actual startup order:

```text
Skip Logos
    -> company logos before the intro cinematic
    -> boot-only
    -> applies on next launch

Skip Intro Videos
    -> native g.PlayIntroCinematicOnBoot path
    -> intro cinematic only
```

They are separate options and both default to ON:

```ini
SkipLogos=1
SkipIntroVideos=1
```

Skip Logos is displayed directly above Skip Intro Videos in the System section.

The DXGI proxy reads the same `[Features] SkipLogos` INI value before UE4
startup. The overlay writes that same value, so there is one source of truth.
The menu also shows the actual boot state and blocked-file count.

V0.18's early-IAT primitive remains under test. The previous test produced no
visible skip, so V0.18B does not claim it is validated. No alternate fallback is
stacked into this build.


## V0.18C - Skip Logos Media Foundation test

Runtime evidence from V0.18B:

```text
Skip Logos boot=ON
EARLY_IAT CreateFileW=1
GetFileAttributesW=1
GetFileAttributesExW=1
blocked=0
```

Conclusion: the two startup movies do not travel through those main-EXE Win32
IAT calls. V0.18/V0.18B early-IAT interception is rejected.

V0.18C removes `EarlySkipLogos` from `dxgi.dll` completely. The DXGI proxy is
restored to the validated minimal loader.

Skip Logos now owns one primitive only:

```text
ASI startup on exact target
    -> hook mfplat!MFCreateSourceResolver
    -> capture IMFSourceResolver instance
    -> hook CreateObjectFromURL
    -> block only:
       THQ_LogoBasic.mp4
       AS_LogoBasic.mp4
```

The hook is installed from the ASI after normal DLL loading, never from
`DllMain`.

Diagnostics are bounded to the first eight Media Foundation URLs. The menu
shows:

```text
MF READY/OFF
resolver READY/waiting
URL call count
blocked count
```

`SkipLogos=1` and `SkipIntroVideos=1` remain independent and ON by default.
Skip Intro continues to use only the validated native
`g.PlayIntroCinematicOnBoot` CVar.


## V0.18D - UE4 StartupMovies Skip Logos

V0.18C Media Foundation is rejected after in-game testing: both company logos
remain visible.

The new path is deliberately simpler and targets the UE4 startup-movie
configuration itself.

Darksiders Genesis stores user configuration under:

```text
%LOCALAPPDATA%\THQ Nordic\Darksiders Genesis\Saved\Config\WindowsNoEditor\
```

V0.18D manages a small marked block in `Game.ini`:

```ini
; BEGIN DarksidersGenesisEnhanced SkipLogos
[/Script/MoviePlayer.MoviePlayerSettings]
-StartupMovies=THQ_LogoBasic
-StartupMovies=AS_LogoBasic
; END DarksidersGenesisEnhanced SkipLogos
```

This removes only the two company startup movies from UE4's
`MoviePlayerSettings.StartupMovies` array. It does not touch the intro
cinematic or any game files.

`Skip Logos` remains directly above `Skip Intro Videos` in the menu.

Both default ON. V0.18D introduces config revision `1804`; upgrading from any
older test INI resets these two options to ON exactly once:

```ini
SkipLogos=1
SkipIntroVideos=1
```

After that migration, user choices persist normally.

Skip Logos changes require a game restart because UE4 reads StartupMovies during
boot. Skip Intro remains the independent validated native
`g.PlayIntroCinematicOnBoot` control.


## V0.18E - Early global file-block test

V0.18D UE4 `Game.ini` StartupMovies override is rejected after in-game testing:
the override is written successfully but both company logos are still displayed.

V0.18E returns to the original file-block idea, but moves it earlier and makes
it process-wide.

Single path:

```text
first DXGI factory call
    -> outside DllMain
    -> before ASI loading
    -> MinHook KernelBase!CreateFileW
    -> exact basenames only:
       THQ_LogoBasic.mp4
       AS_LogoBasic.mp4
    -> ERROR_FILE_NOT_FOUND
```

This differs from V0.18/V0.18B:

- no main-EXE IAT patch;
- no Game.ini override;
- no Media Foundation hook;
- no extra fallback tree.

Because the hook targets the actual Win32 function rather than one module's IAT,
calls from any loaded game/UE4 module pass through the same detour.

The proxy records:

```text
CreateFileW total calls
.mp4 calls
blocked target calls
```

The overlay exposes those counters beside Skip Logos.

`SkipLogos=1` and `SkipIntroVideos=1` remain independent and ON by default.
Config revision `1805` resets those two values to ON exactly once when upgrading
from an older test build.

Skip Intro remains exclusively on the validated native
`g.PlayIntroCinematicOnBoot` path.


## V0.18F - native SetupLoadingScreenFromIni bypass

V0.18E proved that a process-wide `CreateFileW` hook was installed and active,
but no MP4 open ever passed through it. That file-open route is rejected.

A direct executable audit identified the actual UE4 startup-movie path.

Exact retail target:

```text
DarksidersGenesis-Win64-Shipping.exe
size       62,113,280 bytes
SHA-256    9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54
SizeOfImage 0x03DDF000
```

Engine initialization contains the marker:

```text
GetMoviePlayer()->SetupLoadingScreenFromIni
```

The caller resolves `GetMoviePlayer()`, reads its vtable and invokes slot
`+0x78`. The corresponding vtable entry points exactly to:

```text
FDefaultGameMoviePlayer::SetupLoadingScreenFromIni
RVA 0x0160BC50
VA  0x14160BC50
```

That function reads, in order:

```text
bWaitForMoviesToComplete
bMoviesAreSkippable
StartupMovies
```

V0.18F therefore owns one primitive only:

```text
first DXGI factory call
    -> outside DllMain
    -> before ASI startup
    -> validate retail SizeOfImage + exact native prologue
    -> MinHook RVA 0x160BC50
    -> Skip Logos ON: return immediately
    -> Skip Logos OFF: call original function
```

No file hook, no Media Foundation hook, no Game.ini override and no fallback
tree remain in the active Skip Logos path.

Overlay telemetry:

```text
NATIVE READY/OFF
target VALID/INVALID
calls
skipped
```

Skip Intro stays entirely separate on the already validated native
`g.PlayIntroCinematicOnBoot` control.

Config revision `1806` resets `SkipLogos` and `SkipIntroVideos` to ON once
when upgrading from an older test build.


## V0.18G - earliest native MoviePlayer RET patch

V0.18F proved that the audited MoviePlayer target was valid and the MinHook
detour installed successfully, but runtime telemetry stayed:

```text
setupCalls=0
skipped=0
```

Therefore the one startup call to `SetupLoadingScreenFromIni` occurs before the
first DXGI factory call where V0.18F installed its hook.

Executable audit confirms `dxgi.dll` is a normal import, not a delay import.
Its `DllMain(DLL_PROCESS_ATTACH)` therefore executes before the game entry
point.

V0.18G removes MinHook from the DXGI proxy entirely and uses a one-byte
fail-closed patch:

```text
DLL_PROCESS_ATTACH
    -> validate PE64
    -> SizeOfImage == 0x03DDF000
    -> validate exact prologue at RVA 0x0160BC50
    -> Skip Logos ON:
       0x48 -> 0xC3
       SetupLoadingScreenFromIni returns immediately
    -> Skip Logos OFF:
       restore native 0x48
```

No thread, no media hook, no file hook, no Game.ini override and no fallback
tree are used.

The ASI only exposes runtime control and telemetry:

```text
EARLY PATCHED/NATIVE
target VALID/INVALID
```

Skip Intro remains entirely separate on the validated native
`g.PlayIntroCinematicOnBoot` path.

Config revision `1807` resets `SkipLogos` and `SkipIntroVideos` to ON once
when upgrading from an older test build.


### V0.18G in-game verdict: REJECTED

Latest runtime test:

```text
Target validation: exact=1
Skip Logos EARLY: proxy=1 target=1 patched=1 enabled=1 RVA=0x160BC50
```

Despite the exact retail target being validated and the one-byte RET patch being
successfully applied before normal game startup, the THQ Nordic / Airship
Syndicate startup logos are still displayed.

Conclusion:

- the patch timing is early enough;
- RVA `0x160BC50` is a real and valid MoviePlayer function;
- bypassing `SetupLoadingScreenFromIni` does **not** control these two observed
  company logos in this build;
- V0.18G is rejected and must not be promoted to `main`;
- do not revisit the V0.18G RET path without new executable evidence.

The validated gameplay core remains unchanged. Skip Intro continues to work
independently through `g.PlayIntroCinematicOnBoot`.

Next Skip Logos work must begin from fresh native analysis of the actual company
logo playback path rather than another timing variation of the rejected
`SetupLoadingScreenFromIni` route.


## V0.18H - CustomSplashScreen branch bypass

V0.18G proved that the MoviePlayer `SetupLoadingScreenFromIni` path was not the
source of the visible THQ Nordic / Airship Syndicate logos: the exact target was
validated and patched before game startup, yet both logos still appeared.

A deeper audit of the surrounding `FEngineLoop::PreInitPostStartupScreen`
control flow identified the second startup-screen path:

```text
GetMoviePlayer()->HasEarlyStartupMovie()
    YES -> Initialize -> PlayEarlyStartupMovies()
    NO  -> FPreLoadScreenManager
           -> HasRegisteredPreLoadScreenType(CustomSplashScreen)
           -> PlayFirstPreLoadScreen(CustomSplashScreen)
```

The executable's `EarlyStartupMovie` block matches the Unreal startup flow:
when no early MoviePlayer startup movie owns the screen, the engine falls back
to a registered `CustomSplashScreen`.

Exact retail call-site:

```text
RVA 0x002535DE  test al, al
RVA 0x002535E0  74 0F     je skip_custom_splash
...
RVA 0x002535EB  call FPreLoadScreenManager::PlayFirstPreLoadScreen
```

V0.18H owns one primitive only:

```text
dxgi.dll DllMain
    -> validate PE64 + SizeOfImage 0x03DDF000
    -> validate exact surrounding bytes
    -> Skip Logos ON:
       RVA 0x2535E0  74 -> EB
       always skip PlayFirstPreLoadScreen(CustomSplashScreen)
    -> Skip Logos OFF:
       restore 74
```

No MoviePlayer setup patch, file hook, media hook, Game.ini override, MinHook,
thread or fallback tree is active for Skip Logos.

This follows the lesson from the validated POSTAL startup-logo fix: target the
actual early startup-screen state/control path rather than the eventual media
file open.

Skip Intro remains fully independent on the validated native
`g.PlayIntroCinematicOnBoot` path.

Config revision `1808` resets `SkipLogos` and `SkipIntroVideos` to ON once
when upgrading from older test builds.


## V0.18I - EarlyStartupMovie / CustomSplashScreen selector bypass

The supplied runtime log was from **V0.18G**, not V0.18H:

```text
Darksiders Genesis Enhanced ASI 0.18G-dllmain-native-ret-test
Skip Logos EARLY: proxy=1 target=1 patched=1 enabled=1 RVA=0x160BC50
```

That confirms the already documented V0.18G failure, but it does not constitute
an in-game test of V0.18H.

A fresh audit of the exact supported retail executable:

```text
DarksidersGenesis-Win64-Shipping.exe
size       62,113,280 bytes
SHA-256    9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54
```

shows the complete early startup-screen selector in
`FEngineLoop::PreInitPostStartupScreen`.

Relevant control flow:

```text
RVA 0x253546  call GetMoviePlayer
...
RVA 0x253551  call [vtable+0x30]
RVA 0x253554  test al,al
RVA 0x253556  je 0x2535A8

TRUE branch:
RVA 0x253558  "EarlyStartupMovie"
...
RVA 0x25359B  call [vtable+0x38]

FALSE branch:
RVA 0x2535A8  "PlayFirstPreLoadScreen"
...
RVA 0x2535EB  call FPreLoadScreenManager::PlayFirstPreLoadScreen

common resume:
RVA 0x2535FD
```

This proves why V0.18H was incomplete: its patch at `0x2535E0` affected only
the `CustomSplashScreen` fallback. When an EarlyStartupMovie exists, execution
takes the TRUE branch and never reaches the patched H branch.

V0.18H is therefore **superseded by native analysis before validation**. It is
not promoted and does not need another in-game test.

V0.18I owns one primitive only:

```text
dxgi.dll DllMain
    -> validate PE64 + SizeOfImage 0x03DDF000
    -> validate exact startup-selector bytes
    -> Skip Logos ON:
       RVA 0x253546
       E8 45 56 3B 01
       ->
       E9 B2 00 00 00

       jump directly to RVA 0x2535FD
       bypass both:
         EarlyStartupMovie
         CustomSplashScreen

    -> Skip Logos OFF:
       restore E8 45 56 3B 01
```

This does **not** patch `SetupLoadingScreenFromIni`, does not hook file I/O,
does not hook Media Foundation, and does not modify `Game.ini`.

Normal game cutscenes remain outside this selector. The separately validated
Skip Intro feature remains exclusively controlled by:

```text
g.PlayIntroCinematicOnBoot
```

Config revision `1809` resets `SkipLogos` and `SkipIntroVideos` to ON once
when upgrading from older test builds.

V0.18I also removes two harmless duplicate `skipLogosEnabled` runtime
publications and replaces the obsolete overlay text `Game.ini write failed`
with the correct startup-patch diagnostic.


## V0.19A/B/C - proprietary StartupScreens audit and final Skip Logos fix

A direct audit of the supported retail executable identified the game-specific
startup screen plugin compiled into Darksiders Genesis:

```text
ProjectMayhem/Plugins/StartupScreens/Source/StartupScreens/Private/SStartupScreens.cpp
ProjectMayhem/Plugins/StartupScreens/Source/StartupScreens/Private/StartupScreensModule.cpp
```

The plugin exposes its own startup-screen configuration and playback path,
including:

```text
UStartupScreensSettings
StartupScreenDef
StartupMovies
TimeToShow
Image
PromptText
FadeTime
bMoviesAreSkippable
SStartupScreens
```

This explains why the previous generic UE4 MoviePlayer / StartupMovies
experiments did not control the visible THQ Nordic / Airship Syndicate logos.

Three independent plugin-level candidates were tested:

### V0.19A - StartupScreens module kill

```text
FStartupScreensModule::StartupModule
RVA 0x25FE40
-> immediate RET
```

**In-game verdict: REJECTED AS TOO BROAD.**

The logos disappear, but the intro cinematic is also skipped even with
`Skip Intro Videos=OFF`.

### V0.19B - empty StartupScreens movie playlist

```text
RVA 0x25FF31
44 8B 76 08
->
45 33 F6 90
```

This forces the copied `StartupMovies` count to zero before
`SStartupScreens` receives the playlist.

**In-game verdict: REJECTED AS TOO BROAD.**

The logos disappear, but the intro cinematic is still skipped when
`Skip Intro Videos=OFF`. The warning screen remains visible, proving this path
controls more than only the two company logo presentations.

### V0.19C - bypass StartupScreens MoviePlayer attachment

```text
RVA 0x260244
-> jump to RVA 0x260257
```

This keeps the proprietary `StartupScreens` module and object construction
intact, but skips only the block that attaches `SStartupScreens` to the engine
MoviePlayer.

**In-game verdict: VALIDATED.**

Observed behavior:

```text
Skip Logos ON
  -> THQ Nordic / Airship Syndicate logos skipped

Skip Intro Videos OFF
  -> warning screen remains
  -> intro cinematic still plays normally

Skip Intro Videos ON
  -> independent validated intro skip remains functional
```

A slightly longer black transition can occur when Skip Intro is OFF, but it is
minor and does not affect correctness or stability.

### V0.19D - cleanup / validated baseline

V0.19D is the clean continuation of V0.19C. It changes no gameplay behavior and
retains exactly the validated primitive:

```text
StartupScreens MoviePlayer attachment bypass
RVA 0x260244 -> 0x260257
```

No generic StartupMovies patch, no Media Foundation hook, no file hook, no
StartupModule kill, and no empty-playlist fallback remain in the active path.

The two user-facing options are now cleanly independent:

```text
Skip Logos
    -> proprietary StartupScreens attachment bypass

Skip Intro Videos
    -> g.PlayIntroCinematicOnBoot
```


## V0.27 / V0.28 - HorseCharacter detection and Sprint Duration validated

V0.27 finally resolves the horse at the correct layer by hooking native
`HorseCharacter` functions directly instead of inferring the mount from player
identity, Blueprint accessors or generic movement components.

Validated native capture paths include:

```text
HorseCharacter::GetNormalizedSpeed
HorseCharacter::GetNormalizedSpeedInput
HorseCharacter::TryStartSprinting
HorseCharacter::SetSprintingTrue
```

In-game validation captured a real horse instance with the expected native
stamina data:

```text
StaminaRecoveryPercentageRate      15
StaminaTotalRecoveryPercentageRate 40
StaminaRecoveryCooldown             2
StaminaSprintPercentageRate        25
CurrentStamina / MaxStamina        62 / 62
```

Horse Sprint Duration is therefore considered validated. The feature changes
the native sprint stamina drain rate rather than enlarging the stamina pool.

```text
0x   = vanilla drain / vanilla duration
5x   = default, approximately 5x sprint duration
20x  = maximum
```

V0.28 changes only the user-facing range/default for this validated feature:
default `5.00x`, maximum `20.00x`. The native HorseCharacter detection path
from V0.27 is retained unchanged.


## V0.29 - Horse Speed / Sprint Speed native movement correction

V0.29 keeps the validated V0.27 native `HorseCharacter` resolver and the
validated V0.28 sprint-duration path, but fixes the remaining horse speed
primitives.

The retail UE4 reflection table proves the exact movement fields:

```text
UMayhemHorseCharacterMovementComponent / UCharacterMovementComponent

MaxWalkSpeed            +0x1DC
MaxWalkSpeedCrouched    +0x1E0
MaxSwimSpeed            +0x1E4
MaxFlySpeed             +0x1E8
MaxCustomMovementSpeed  +0x1EC
MaxAcceleration         +0x1F0
MinAnalogWalkSpeed      +0x1F4
BrakingFrictionFactor   +0x1F8
BrakingFriction         +0x1FC

UMayhemHorseCharacterMovementComponent
SprintingMaxSpeed       +0x760
```

Previous horse candidates incorrectly wrote some of these neighboring fields.
V0.29 also removes the invalid `movement+0x190 == horse` acceptance test:
`HorseCharacter::GetNormalizedSpeed` already resolves the native movement
component through its own virtual getter at vtable slot `+0x5F8`.

Horse Speed now targets the real movement component's `MaxWalkSpeed` and
`MaxAcceleration`; Horse Sprint Speed targets that same movement component's
`SprintingMaxSpeed`.


## V0.30 - Graphics Adapter Engine.ini control

V0.30 adds a small system setting for selecting Unreal Engine's graphics
adapter index.

The overlay exposes values `0` through `4` with `0` as the default.
The selected value is persisted in `DarksidersGenesisMod.ini` and written to:

```text
%LOCALAPPDATA%\THQ Nordic\Darksiders Genesis\Saved\Config\WindowsNoEditor\Engine.ini
```

The mod writes exactly:

```ini
[SystemSettings]
r.GraphicsAdapter=N
```

If `[SystemSettings]` or `r.GraphicsAdapter` does not exist, it is created.
If the key already exists, only its value is updated through the Win32 INI API.
Other Engine.ini settings are preserved.

A game restart is required for adapter selection to affect device creation.


## V0.31 - Horse UObject lifetime safety hotfix

V0.29 validated the native horse speed, sprint-speed and stamina primitives, but
the first long level-load test exposed an important lifetime bug inherited by
V0.30.

The horse subsystem cached raw `HorseCharacter` and horse movement pointers and
re-applied tunings every rendered frame. During seamless travel or level reload,
UE4 can destroy those UObjects while the cached addresses remain readable and may
later be recycled for unrelated allocations. Continuing to write horse offsets
through such stale pointers can corrupt unrelated game state.

The reported failure presented as:
- a malformed/default-looking reticle after loading;
- corrupted HUD/menu visual elements;
- eventual `0xC0000005` write access violation inside the game's reference-
  counted object destruction path.

V0.31 changes the ownership rule:

```text
Native HorseCharacter callback active
    -> horse pointer is considered live
    -> resolve current horse movement
    -> apply Horse Speed / Sprint Speed / Sprint Duration

Outside a native HorseCharacter callback
    -> no horse-memory writes
```

Additional safety:
- `Tick()` performs no cached-pointer writes;
- the generic movement hook performs no horse writes;
- shutdown never restores values through cached raw UObject pointers;
- the horse movement pointer is re-resolved on every native horse capture;
- a changed movement pointer resets cached baselines as a new horse generation;
- generic movement exclusion accepts a cached horse movement only for a short
  window after a live native HorseCharacter callback.

The validated V0.30 Graphics Adapter Engine.ini control is retained unchanged.


## V0.32 - Reticle / Alt-Tab isolation

V0.31 removes the horse stale-pointer write path, but a separate visual issue
remains: immediately after loading a level the reticle can appear as a broken
cross, while a simple Alt-Tab restores the correct reticle.

The V0.31 session log proves the malformed reticle appears before the first
fully validated horse capture, so this visual issue is treated separately from
the horse lifetime crash.

V0.32 isolates the D3D11 overlay path:

- ImGui is no longer initialized on the first game Present;
- no overlay RTV is created during normal gameplay;
- the game window is not subclassed until the user opens the mod menu;
- the complete overlay backend is created lazily on the first menu-key press;
- game-window foreground transitions are logged;
- every process-owned DXGI ResizeBuffers call is logged before and after.

Test policy:

1. Start the game and do not open the mod overlay.
2. Load a save and inspect the reticle.
3. If the broken cross is present, Alt-Tab once.
4. Preserve the per-session log.

This determines whether the fix comes from DXGI ResizeBuffers, focus activation,
or merely avoiding early ImGui/RTV/WndProc initialization.


## V0.33 - Cursor-layer reticle fix

The V0.32 focus trace isolates the broken-cross issue from both the horse system
and the ImGui overlay:

- the malformed cross can appear before ImGui is initialized;
- Alt-Tab repairs it through a foreground transition;
- no DXGI `ResizeBuffers` occurs during the fixing Alt-Tab;
- the reticle/cross remains visible when `ui.HideHud` is forced, proving it is
  handled by the cursor layer rather than the ordinary HUD layer.

Retail executable audit identifies:

```text
UAirshipUIManager::IsCursorVisible
RVA 0x715800
```

The native function ultimately reads the PlayerController mouse-cursor visible
state.

V0.33 therefore:
- hooks `UAirshipUIManager::IsCursorVisible`;
- forces cursor visibility false while mod Hide HUD is active;
- schedules a cursor refresh when a newly validated player instance appears;
- posts `WM_SETCURSOR` to the real game window several times after load;
- logs the active `HCURSOR`, visibility and position around focus transitions
  and forced refreshes.

This is intended to reproduce the cursor-reset part of Alt-Tab without actually
changing application focus.


## V0.34 - Reticle focus-cycle workaround (validated manually in-game)

V0.33 is **rejected as an effective reticle fix**. Its scheduled WM_SETCURSOR
messages were successfully posted after a new player was found, but did not
repair the cross-shaped reticle. The user's V0.33 log also shows that a genuine
Alt-Tab involves a different OS cursor handle after focus comes back, and no
DXGI ResizeBuffers call was observed. The trace contained an F4 action but no
F1 Hide HUD toggle, so Hide HUD cursor suppression is not yet proven either way.

V0.34 removes automatic WM_SETCURSOR refreshes that did nothing. It introduces
one manually triggered reticle-repair action:

- `F5 = ReticleFocusTest` in the included INI;
- the action is also selectable from the remappable Hotkeys tab;
- the test does **not** Alt-Tab, steal foreground or move the mouse;
- it posts one paired simulated deactivate/activate message sequence to the
  game's window: `WM_ACTIVATEAPP`, `WM_ACTIVATE`, then `WM_SETFOCUS`
  and `WM_SETCURSOR`;
- logs each posting result and cursor handle, then captures cursor state
  500 ms afterward;
- no ongoing timer, native horse writes, or per-frame cursor replacement.

**In-game validation (2026-10-08):** The user confirms that V0.34's F5
focus-cycle action fixes the malformed reticle after loading. The confirmed
result is the **manual F5 workaround**, not an automatic fix on every load.
The separate V0.33 Hide HUD cursor visibility behavior has not yet received a
dedicated F1 test; do not describe it as validated.

**Usage:** With the malformed cross visible and the mod overlay CLOSED,
press F5 once. If using an older mod INI, set `F5=ReticleFocusTest` under
`[Hotkeys]` or remap a key to `Reticle Focus Test` in the overlay.
This action can temporarily change the game's perceived input focus; a real
Alt-Tab may restore the normal state if focus behaves unexpectedly.

**Safety:** V0.31 horse lifetime ownership rules remain untouched; V0.32 lazy
ImGui creation remains. The V0.29 binary release is kept as a rollback option.


## V0.35 - camera investigation restarted from V0.34

Source: validated V0.34 commit 32f7827cc2968596a34dadc6466730d4280ff94b.
None of the V0.35A/B/C or V0.36 camera changes are inherited.

The camera is NOT modified by this test build. Two individually validated
read-only native-method hooks record whether UCameraComponent::GetCameraView
(RVA 0x16F9790) and USpringArmComponent::UpdateDesiredArmLocation
(RVA 0x6F57B0) are exercised in actual gameplay. The hooks call the original
implementation unchanged. They only sample a live FOV output / current arm
distance and log the first 12 calls then one of every 5000. The correct
retail EXE hash and instruction prologues must match before installation.
No native UObject pointers survive callbacks; no camera setting or rotation
is changed. Existing FOV is OFF/locked, and no fake zoom setting is added.

Test on foot with War and Strife, then mounted if possible, while leaving
the mod overlay closed. Press F5 only if the reticle needs repairing.
Quit the game and provide the non-cumulative DarksidersGenesisMod.log.
Look for CameraTrace V0.35 READY and callback call counts.
Successful compilation does NOT prove that either native path controls
the rendered gameplay view. No promotion to main or public release until
an in-game test confirms behavior. Further zoom/FOV/angle work must use
verified live gameplay callbacks.


## V0.36 - independent Hide Reticle and visible camera controls (test)

This build continues directly on main, without creating a development branch.
Hide Reticle (default OFF; Hotkey F6 by default) is separate from Toggle HUD
(F1) and preserves F5's validated manual reticle focus repair. The existing
native UAirshipUIManager::IsCursorVisible hook now obeys Hide Reticle too.
A second, opt-in Windows SetCursor suppression path acts only when the game
is the foreground window, Hide Reticle is ON, and the mod overlay is closed.
This targets the malformed Windows cursor cross which may survive ui.HideHud.
The OS cursor is restored on exit from that suppression state. The in-game
behavior is NOT yet validated. The original reticle bug is not claimed fixed.

Camera is now a dedicated enabled tab, with FOV override (40 to 140,
default OFF/90), Zoom (+ closer / - farther, -75 to +200 percent, default 0),
and Camera Pitch Offset (-35 to +35 degrees, default 0). All settings are
persisted to the INI and published to atomics. Native, verified-method hooks
modify only a live FMinimalViewInfo output (FOV and pitch) and temporarily
scale the live SpringArm TargetArmLength in its own callback, restoring that
UObject field immediately afterward. No camera UObject pointer is cached
across frames. Hook readiness, callback counters and observed native/applied
values are shown in Camera and logged once per session with sampling.

Caveat: native GetCameraView and UpdateDesiredArmLocation are known code
addresses but their use by the actual gameplay camera remains unproven.
This is an EXPERIMENTAL candidate, not a claim that FOV/zoom/pitch work.
If no calls are observed, the next step is to locate the actual camera path
from the game's native call graph. Test normal gameplay, Strife, and mounted
camera if possible, and supply DarksidersGenesisMod.log.

Defaults remain vanilla for Hide Reticle, FOV, Zoom and Pitch. The config
revision does not change, to avoid resetting existing player preferences.
Older INI files read the new settings using safe defaults when missing.
The validated horse lifetime ownership, lazy ImGui overlay, skip logos and
F5 reticle recovery remain unchanged. Artifacts contain files at the ZIP root.

## V0.37 - Camera height + configurable arrow keys (TEST, 2026-10-08)

Based directly on the working V0.36 main commit 634f89dc.
**User in-game status:** FOV, Zoom and Camera Pitch are working.
A camera-height setting is now provided: CameraHeightOffset from -1500 to
+1500 world units, default zero (vanilla). It changes only the transient
FMinimalViewInfo.Location.Z output at +0x08 in the existing live camera
callback. It does not cache or write camera UObjects.

New camera keybinding actions are configured under [CameraHotkeys] and can
be rebound in the Camera tab. Defaults:
- Up: raise camera by 50 units; Down: lower camera by 50
- Left: Zoom Out by 10 percent; Right: Zoom In by 10 percent
- PitchDown, PitchUp: unbound; optional increments of 5 degrees

Keys repeat when held (initial delay 290 ms, repeat every 90 ms), only
while the game is foreground and mod overlay is closed. Duplicate bindings
are unbound in other camera actions. Capturing Escape cancels; menu key and
F1-F12 are reserved. Assigning None from the Camera UI is supported.
Existing INIs without [CameraHotkeys] use those defaults. Camera values and
hotkeys are saved via the normal debounce/persistence mechanism.

**Known Zoom issue (deferred):** Zoom changes camera arm distance without
updating UE4 depth-of-field focal distance or blur/postprocess parameters,
so a zoomed image can become very blurry. Do NOT regress the working zoom
just to compensate; native DOF correction is a separate future task.

**Reticle status corrected:** earlier V0.34 F5 focus pulse was prematurely
described as a validated repair. The user confirms the cross-shaped reticle
still occurs in V0.36. Thus F5 and the V0.36 Hide Reticle mechanism are
experimental, not a reliable bug fix; neither is promoted as solved.

One build number per iteration; changes committed on main; legacy stable
release v0.29 remains untouched. Per-launch non-cumulative log retained.
Test V0.37 in gameplay before promoting features to release.

## V0.38 - horizontal camera yaw, reticle UI and Alt-Tab mitigation (TEST)

Base: main V0.37 commit 1108b307a71886a17be6187fba0a4b8a690b04a8.
User log V0.36: Window focus trace switches foreground 1 to 0 around
23:17:10, with no observed return before logging stops; not proof
the mod caused lost focus. V0.37 log: Hide Reticle ON checked in menu,
but OS pointer suppression begins only on overlay close. F6 on/off
and Win32 SetCursor interceptions are proven by log.

V0.38:
- Native FMinimalViewInfo::Rotation.Yaw (+0x10) adjusted in transient
  GetCameraView output only; CameraYawDegrees -180..+180, default 0,
  configured with a live slider, number entry, default button.
- Bindable action YawLeft and YawRight, default NumPad 4 and NumPad 6
  with Num Lock enabled, 5 degrees per step. Prior bindings preserved.
- Hide Reticle menu explains gameplay-only behavior and includes
  Apply Reticle Setting and Return to Game to close the overlay.
  F6 remains the toggle; this does not repair the underlying malformed cross.
- Focus mitigation: restore cursor even when losing foreground
  (V0.37 erroneously only restored when foreground); release pointer
  clip on that suppression exit, do not block WM_ACTIVATEAPP,
  WM_ACTIVATE, WM_SETFOCUS, WM_KILLFOCUS, WM_MOUSEACTIVATE or Alt
  system key messages, and log WndProc focus messages.
  Only release cursor clipping from an active overlay while the game
  has foreground. In-game Alt-Tab still requires validation.
- No changes to validated FOV, Zoom, Pitch, Height or horse protections.
  Zoom postprocess Depth of Field blur is a tracked future task.
- No release until user in-game confirmation, sequential build
  V0.38 on main, artifacts ZIP root, session-only logs.

Validation: test yaw +/-30 degrees, NumPad4/6, custom bindings,
Hide Reticle checkbox and apply button, F6 with overlay closed,
Alt-Tab with overlay both open and closed and Hide Reticle both states.

## V0.39 - Startup loader diagnostic (TEST)

The user reported that the *direct-download* V0.38 did not show either
the overlay or any hooks. V0.36 and V0.37 previously loaded and functioned
on the same PC. The existing uploaded logs predate this V0.38 failure.
Static inspection of the V0.37/V0.38 Windows build artifacts found both
ZIPs valid with all four root-level files, same x64 PE architecture and
matching imported DLL families. No confirmed root cause yet.

V0.39 changes ONLY startup observability (not the camera or gameplay):
- The DXGI proxy writes DarksidersGenesisLoader.log into the same folder
  as dxgi.dll; this log is independent of DarksidersGenesisMod.asi.
- The first loader log truncates the previous session. It lists the
  proxy DLL path, ASI scan, each ASI LoadLibrary result and GetLastError,
  and the real System32 dxgi.dll load result. No cached pointers, no
  new hooks, and no changes to camera, reticle, horse or HUD behavior.
- Core startup version string updated to V0.39 so an old/cached ASI is
  immediately recognizable in DarksidersGenesisMod.log.
- A missing loader log means the game did not invoke the proxy's
  DXGI factory export, the proxy was not placed/loaded correctly, or
  initialization happened earlier than logging. In that case, confirm
  dxgi.dll is next to the actual DarksidersGenesis-Win64-Shipping.exe.
- If loader log exists but ASI log does not, look for the precise
  LoadLibrary result/error; also check game EXE hash and quarantine.
- If both log files exist with version 0.39, check native hook readiness.
- Treat V0.37 as verified fallback, not V0.38. No stable release.

The other unresolved issues remain: unreliable malformed reticle/F5,
potential Alt-Tab focus recovery, zoom-induced DOF blur, and in-game
validation of horizontal yaw. V0.39 makes no claims to fix these.

## 2026-10-09 correction: V0.38 startup failure was antivirus quarantine

The user confirmed that V0.38 **does load and work**. The preceding
"no overlay / no hooks" report was caused by Windows Defender flagging
and quarantining the mod, **not** by a confirmed loader or code defect.
This correction supersedes the original startup-failure hypothesis
documented in the V0.39 diagnostic entry above. Keep that historical
entry for traceability, but do not treat it as a current failure.

V0.38 is reinstated as a functioning test build. This does NOT by itself
validate every V0.38 feature (yaw camera rotation, Alt-Tab recovery, or
persistent reticle fix); these retain their separate test statuses.
V0.39 remains an optional loader diagnostic, not a mandatory patch.

Installation guidance: check Microsoft Defender Protection History if
the ASI or DLL disappears. Verify the downloaded file comes from the
project's GitHub repository before deciding how to handle a warning;
do not advise disabling antivirus globally or bulk-excluding folders.

## V0.40 - Diagnose cross-shaped reticle source after save load (TEST)

V0.38 user test confirms Hide Reticle ON/OFF fixed, while cursor still
looks like a cross immediately after loading a save. Log shows native
UAirshipUIManager::IsCursorVisible changing 0->1 after loading.
F5 synthetic focus pulse is unreliable and not a permanent fix.

V0.40 provides a reversible test mode separate from validated HideReticle:
[Features] CrossCursorTestMode=0 (default) = unmodified behavior.
1 = suppress only the Win32 SetCursor output while leaving native Unreal
cursor visibility untouched.
2 = suppress only the native Unreal cursor while leaving Win32 cursor
output untouched. Existing Hide Reticle independently hides both layers.
The ImGui overlay keeps its clickable mouse pointer. The user's menu
can apply changes and return to gameplay. Settings survive restart.
No additional game hooks and no cached UObject writes.

The SetCursor hook now logs changed requested OS cursor handles with a
bounded budget even if HideReticle is OFF. Native UI cursor visibility
transitions are also logged on state changes. This evidence distinguishes
the source of the cross from the legitimate reticle.

TEST: after loading the same save and reproducing cross, keep Hide Reticle
OFF. Try cross mode 1, then mode 2, returning to game each time. Tell
whether the cross disappears and whether normal reticle remains visible.
Restore 0 for default. This build is a diagnostic/possible mitigation,
not an already validated automatic reticle repair.

V0.38 functionality and camera, horse, focus, F6 kept unchanged;
V0.39 loader diagnostics retained. Build V0.40 from main, no dev
branches or automatic public release; permanent download ZIP on GitHub
since temporary artifact links have failed for this user. Log is
noncumulative per launch.

## V0.41 - Auto F5 reticle correction after save loading (test)

Actual V0.40 log: the test mode stayed CrossCursorTestMode=0, so the two
separate source-filter modes 1/2 were not exercised; do not consider
those experiments disproven. The game's normal load shows native UI
cursor visibility 0->1 after scene load. User's V0.38 test establishes
that pressing F5 or actual Alt-Tab fixes the cross; Hide Reticle ON/OFF
also works and must remain independent.

New [Features] AutoReticleRefreshOnLoad=1 defaults ON, with a gameplay
overlay checkbox. On a native UI cursor 0->1 load transition, arm a
2500ms delayed, one-shot replay of the existing F5 activate/deactivate
state machine. Run only on game Present with a validated local player,
foreground nonminimized window and closed overlay. No synthetic mouse,
SetForegroundWindow, UObject writes, cached game pointers or new hooks.
Only one automatic cycle per 20s, cancel pending on manual F5 or real
Alt-Tab, expire after 30s if gameplay is not ready. Log all arms,
dispatches and cancellations. Disable via INI if focus changes undesirably.

This is a candidate automatic workaround, not a validated fix until
user tests in game. V0.40 is preserved for rollback; gameplay settings,
cursor source diagnostic tests, camera yaw, zoom, height, horse, HUD,
F1/F6 and skip intro remain unchanged. The build ZIP keeps four files at
root, noncumulative logs, no public release before user validation.

## V0.42 - Cleanup and READ-ONLY reticle root-cause instrumentation (TEST)

**User result:** V0.41 automatic F5 replay did not fix the cross and is
an undesirable workaround. User asked to remove leftover useless experiments
and investigate the actual mechanism causing the cross after save-load.

Audit of code vs V0.38/V0.40:
- V0.41 AutoReticleRefreshOnLoad and its delayed focus pulse scheduler,
  state, INI, overlay toggle, config and runtime plumbing: REMOVED.
- V0.40 CrossCursorTestMode 1/2, which were never exercised in supplied
  logs and would only mask symptoms: REMOVED.
- Old INI keys AutoReticleRefreshOnLoad/CrossCursorTestMode are deleted on
  next normal config save. Existing INI revision stays unchanged.
- Preserve VALIDATED Hide Reticle ON/OFF including F6, the native cursor
  visibility hook and Win32 SetCursor suppression only when Hide Reticle ON.
- Preserve manual F5 (user confirms it clears the cross) but NEVER trigger
  it automatically. It is an opt-in A/B control for the diagnostic.
- Preserve Alt-Tab focus handling, lazy ImGui, verified camera/yaw/zoom,
  horse pointer safety, HUD, intro skip, movement and all other features.
- Legacy ThirdPerson / other inactive config keys remain for backward
  compatibility: do not silently remove user-requested saved settings.
- Native cursor visibility remains read-only except existing Hide Reticle.
  With Hide Reticle OFF the diagnostics NEVER change SetCursor's output.

NEW READ-ONLY DIAGNOSTIC:
- Sample GetCursorInfo every ~350ms while rendering. On change (cursor
  handle, showing flags, game focus), log desktop cursor handle, flags,
  foreground, mouse position, GetIconInfo hotspot and bitmap dimensions,
  clipping and last Win32 requested handle. Release icon bitmaps immediately.
- Existing SetCursor hook, on first <=40 requested-handle transitions, logs
  calling thread, return address and game exe relative caller RVA. These
  pinpoint which code path requested the cursor after loading.
- Correlate with native UAirshipUIManager::IsCursorVisible state changes.
- Snapshot before manual F5 and after the paired cycle, and across
  real Alt-Tab transitions; no keypress or focus generated by the probe.

TEST PROCEDURE: keep HideReticle OFF and never open overlay until after the
cross appears. Load save, observe cross for several seconds; then do one
real Alt-Tab out-and-back, see whether cross disappears; optionally press
manual F5, then exit and send noncumulative DarksidersGenesisMod.log.
Ideally also compare whether the cross appears with all mod binaries
temporarily absent, to distinguish a native game bug from mod interaction.
Do not attribute the root cause before comparing code RVA/state transitions.
No public release until validated. ZIP files at root.

## V0.43 - Skip Logos attachment bypass implicated in malformed cursor (TEST)

User discovered cross appears after save load when Skip Logos is enabled.
The earlier V0.19C patch at RVA 0x260244 jumps to 0x260257, avoiding
SStartupScreens attachment to MoviePlayer. This bypass was initially
validated for faster boot and independent Skip Intro cinematic behavior,
not for the lifecycle of Slate input/focus/cursor in later levels.
It is a STRONG causal lead, not yet proof of which cleanup call is missing.

This build DOES NOT introduce another cursor workaround or speculate a
replacement low-level patch. It preserves the V0.42 removal of automatic
F5 and abandoned cursor mode 1/2 experiments, and passive cursor traces.
It preserves working Hide Reticle ON/OFF and F6, camera, HUD, horse, etc.

Safe-by-default, controlled A/B:
- Fresh INI SkipLogos=0; SkipIntroVideos remains independent and ON.
- Bump config revision 2101 -> 2102. Migrates existing INI with ONLY
  SkipLogos forced OFF; other settings preserved.
- CRITICAL: earliest proxy checks config revision BEFORE the ASI loads;
  old revisions cannot enable the attachment bypass on that first boot.
- Explicit SkipLogos=1 after migration can still activate old V0.19C
  for A/B if needed, with warning in GUI and loader/ASI log.
- SkipLogos=0 leaves vanilla MoviePlayer attachment intact.
- Changing either option affects startup on NEXT game launch only.
- No new public release until tested.

Controlled test: SkipLogos OFF, restart game, load same save, check
cross without F5 or Alt-Tab. If cross absent, association confirmed;
optional second run SkipLogos ON verifies regression with otherwise
identical setup. Next engineering step is disassemble StartupScreens'
attach, playback completion and destruction paths to implement a
true native lifecycle-preserving logo skip without touching game assets.
Do not claim a permanent working Skip Logos replacement yet.

### StartupScreens root-cause audit (after V0.43)

The full audit and exact known 19-byte setup sequence are in
[docs/STARTUPSCREENS_AUDIT.md](docs/STARTUPSCREENS_AUDIT.md).
The failed logo bypass at `0x260244` skips both a call to
`0x1608B90` (probable GetMoviePlayer) and the virtual method call at
`[vtable+0x20]` (probable loading-screen setup) with `[rbp-0x78]`.
Those identities must be checked with the exact retail executable.
**User verified SkipLogos OFF removes the cross-shaped cursor.**
Retain V0.43 safe default; avoid workarounds. The game EXE is not
present among files currently accessible, so no replacement binary
patch is being claimed or published.

## V0.44 - Probe MoviePaths with the real retail EXE (TEST)

USER PROVIDED the exact 62,113,280-byte retail binary, whose SHA-256
matches 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54.
Disassembly of RVA 0x25FE40..0x260257 confirms:
- settings UObject array at +0x38 contains `StartupMovies`;
- each entry in that TArray is a 16-byte UE FString;
- the compiled startup code copies it into `FLoadingScreenAttributes`
  at [rbp-0x68], count [rbp-0x60], and passes attributes at
  [rbp-0x78] to a virtual MoviePlayer method;
- the getter call at 0x260244 resolves to 0x1608B90 and virtual
  call at 0x260253 uses [vtable+0x20]. With the original 19 bytes
  present, the native call sequence remains intact.

V0.44 has a **strictly opt-in, one-shot diagnostic probe**, not a new
logo skip. Fresh test INI [Diagnostics] StartupMovieProbe=1. Remove
or set 0 to return to fully unmodified native startup (V0.43 behavior).
It uses an exact-byte-verified 19-byte trampoline at RVA 0x260244,
and replays all original calls byte-for-byte before 0x260257. The
pre-call C++ observer reads MoviePaths TArray entries without writing
to them, bounding all lengths and page accesses with VirtualQuery.
It logs names, count, and minimum loading-screen time in
DarksidersGenesisLoader.log. Diagnostic is disabled when legacy
SkipLogos=1 because that legacy code is already proven to cause the
cursor regression. An in-game attempt to enable SkipLogos while probe
is installed fails closed until a restart with probe disabled.

***Do not call this a Skip Logos fix.*** No logo suppression occurs.
The purpose is to identify the exact runtime playlist names, allowing
a later targeted plan that preserves native MoviePlayer attachment.
The default distribution enables the probe for a one-time test, but
the proxy defaults it OFF for older INIs that lack the key. Hide
Reticle ON/OFF, F6, camera, horses, HUD, intro skip, and all other
features remain unchanged. If the probe causes a crash, disable
StartupMovieProbe=0 and restore V0.43; do not ship as stable release.
No synthetic F5, focus or Win32 cursor manipulation is introduced.
Log is per-run (loader also resets per-run).

## V0.45 - ABORT unsafe StartupMovieProbe trampoline; restore safe baseline

**User-reported crash** with V0.44. Supplied loader log shows that
`StartupMovieProbe V0.44: ... trampoline ACTIVE` was written, while no
`StartupMovieProbe ... item` or `call=` entry appeared. ASI log reaches
early core init but ends before normal shutdown. This strongly implicates
the new mid-function trampoline; no crash dump or exception address
was supplied, so precise failing instruction is **not proven**.

Root engineering mistake: V0.44 diagnostic was described as read-only
because it did not modify `FLoadingScreenAttributes`, but it patched
**19 bytes of executable instructions** and redirected execution to
a manually built x64 thunk. That is intrusive and unsafe without
unwind registration, thorough calling-convention tests and execution
validation. Do not reintroduce it as an "observation-only" technique.

**V0.45 removes the entire V0.44 trampoline and observer** from the
DXGI proxy and restores exactly the proven V0.43 proxy source.
New INI omits `[Diagnostics] StartupMovieProbe`. If an old INI still
contains `StartupMovieProbe=1`, the V0.45 proxy deliberately ignores
that retired key, so an old INI cannot reactivate the probe. Keep
`SkipLogos=0` as the safe startup default. The historic bypass at
`0x260244` remains *explicitly opt-in* and not fixed; turning it ON
can reintroduce the cross-shaped cursor as previously confirmed.
`SkipIntroVideos` remains independent.

This is **safety rollback, not a repaired Skip Logos**. Reticle/F6,
camera, HUD, horses and all other gameplay code are left unchanged.
No automatic F5, synthetic focus or cursor masking reintroduced.
The live exe remains unmodified by the probe with SkipLogos OFF.

Next investigation route: inspect `StartupScreens` defaults and the
actual movie/config resources offline (for example the game's
`DefaultGame.ini`, `StartupScreens` settings, or the original game
`.pak` assets). Prefer content-/descriptor-level, logo-only changes
without bypassing `MoviePlayer::SetupLoadingScreen`; any runtime hook
requires separate safety audit. No public stable release from this
test until user validates V0.45 startup.

## V0.45 runtime validation from user logs (2026-10-09)

The user supplied both Loader and ASI logs for the V0.45 test session
(start 13:51:40, clean shutdown at 13:52:42):
- DXGI proxy logs "V0.43 SAFE: native MoviePlayer startup attachment preserved",
  and loads the ASI successfully.
- ASI logs "0.45-no-trampoline-safe-startup" with supported exact EXE
  SHA-256 and Skip Logos patched=0 enabled=0.
- D3D11, Camera, Horse, Reticle, HUD, Movement, Recovery and Damage
  hooks report ready; player detected; in-game combat and native
  cursor transitions are recorded.
- Ends with "Shutdown: complete", no crash recorded in these logs.
- The user did not explicitly describe the visual appearance of the
  reticle in this V0.45 session: do not claim visual revalidation
  from native-cursor visibility alone.
- In contrast, V0.44 opt-in early trampoline showed StartupMovieProbe
  ACTIVE and caused a startup crash. The implementation is removed
  in V0.45: never resurrect it.

Next engineering step: target the logos selectively while preserving
SStartupScreens and the native MoviePlayer attachment/lifecycle.
No automatic F5, synthetic focus, disabled startup module, globally
emptied startup movies or unsafe mid-function trampoline.
Public stable release remains unchanged.

### Native StartupScreens continuation, verified logo identities

The exact game EXE and public PC-specific guides support further
analysis recorded in [docs/STARTUPSCREENS_AUDIT.md](docs/STARTUPSCREENS_AUDIT.md):
logo files are `THQ_LogoBasic.mp4` and `AS_LogoBasic.mp4`;
`CG_Intro_LowVi.mp4` is separate. Unlike a config-file rename,
the desired mod must remain ASI-only. The file stems are absent from
the matching EXE; only reflected settings/array fields are present.
The actual runtime movie list is unknown without inspecting the
game's cooked config/resources. V0.45 is unchanged and safe with
`SkipLogos=0`. No speculative patch is permitted.

## V0.46 - Experimental exact-name first-two StartupMovies filter (TEST)

Goal: bring back Skip Logos without touching the native MoviePlayer setup
and without altering game assets. Continue directly from V0.45 stable
safe startup. V0.44's crashing 19-byte MoviePlayer wrapper remains gone.

SOURCE: The user-supplied retail executable was verified against
62,113,280 bytes and SHA256
9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54.
RVA 0x25FF31 contains exactly:
44 8B 76 08 (mov r14d,[rsi+8], array count)
48 8B 36    (mov rsi,[rsi], array descriptor pointer).
The game's unmodified 19 bytes at RVA 0x260244 are independently
validated and NEVER changed by this version.

V0.46 uses an early 7-byte exact-signature redirect at RVA 0x25FF31.
A nearby executable island replays those original two instructions,
saves flags and all volatile GP registers, allocates the Win64 32-byte
shadow space and calls a bounded validator. The validator reads at most
3 names with VirtualQuery guards and logs them to the Loader log.
Only if first two FString descriptors name THQ_LogoBasic and AS_LogoBasic
in either order, count is 3..64 and all relevant buffers are readable
does the island increase RSI by 32 (skip two 16-byte descriptors) and
decrease R14D by 2. This preserves the third and subsequent entries.
In all other circumstances the ORIGINAL full playlist is passed on.
The island restores original registers/flags and uses a RIP-indirect
absolute jump back at RVA 0x25FF38, not clobbering RAX.

The following native code deep-copies the resulting descriptors and
calls the ORIGINAL SetupLoadingScreen vtable method. Unlike the
V0.19C failed attachment bypass, this retains the MoviePlayer/Slate
startup path and original cleanup ownership. It is nonetheless an
experimental early executable detour; crash-free behavior is NOT
proven. On crash use V0.45 or SkipLogos=0 (fresh startup required).

Fresh TEST ZIP includes SkipLogos=1 to exercise the new mechanism.
Existing INIs are preserved and V0.45 users must explicitly change
SkipLogos=0 to 1 to test it. Missing/old INIs do not automatically
enable it. The old V0.19C attachment bypass is removed entirely from
the proxy; it cannot be invoked by this build. Skip Intro remains
controlled by g.PlayIntroCinematicOnBoot and unaffected by code changes.

If the named logo files are NOT the first two native entries, the
validator logs FAIL OPEN and the logos still show. This is intentional
rather than guessing. Future builds may expand to name-matching in
arbitrary positions after observing real runtime ordering.

TEST GATE: startup with new proxy, log messages in
DarksidersGenesisLoader.log, logos skipped or not, warning display,
intro with SkipIntroVideos OFF, in-game cursor after save load,
Alt-Tab and overlay; do not publish a public tagged release before
user validates. Files at ZIP root. No new auto F5/focus simulations.

## V0.47 - Unified compact log, with ALL gameplay features unchanged (TEST)

User requested **one** log containing essentials, instead of two logs.
This revision modifies only diagnostic output and its filenames.
New per-session canonical filename: `DarksidersGenesisMod.log`.

- DXGI proxy creates/truncates the canonical logfile on the first
  loader event. It logs only early Skip Logos decisions, relevant
  prefix-filter matches/mismatches, ASI load result and errors.
- The proxy deletes the obsolete `DarksidersGenesisLoader.log` if
  present, to avoid confusing next-run stale results.
- Proxy exports `DGUnifiedLogActive` so the ASI can append to the
  existing log rather than erasing early events in `ResetLogFile`.
  If the proxy is unavailable, ASI retains its independent log reset.
- ASI suppresses old, high-volume cursor snapshots, repeated focus,
  camera trace frames, rejected movement-candidate dumps and raw
  repeated HorseCharacter capture traces. It keeps hooks' READY/FAILED,
  exact supported game fingerprint, player capture, setting changes,
  overlay actions, single first combat samples and clean shutdown.
- These filters affect **messages only**. The camera/horse/reticle
  hooks and event semantics are untouched. No new focus workaround.
- New version label V0.47; INI remains unchanged. Current V0.46
  experimental logo-prefix filter is preserved verbatim, not reworked.
- IMPORTANT: user's uploaded logs at 15:03 were still from V0.45,
  so they do not validate V0.46's experimental Skip Logos.
- Keep public release unchanged pending user tests of V0.47.

## V0.48 - Preserve old Loader.log files (requested correction)

V0.47 introduced a single log but unnecessarily deleted the existing
`DarksidersGenesisLoader.log` on process attach. **V0.48 removes that
DeleteFileW operation and its related stale-file path entirely.**
From now on the mod does not CREATE, WRITE, RESET or DELETE
`DarksidersGenesisLoader.log`. An existing old loader log is preserved
byte-for-byte on disk. It is historical data, not an active logfile.

All early proxy and ASI events still go to the single per-session
`DarksidersGenesisMod.log`; the proxy truncates only that file once
per game launch and the ASI appends without discarding early events.
Compact filtering, settings, hooks and the V0.46 experimental selective
Skip Logos implementation remain unchanged. Build uses the existing
root-level four-file ZIP layout; no public tagged release until tested.

## V0.49: user log proves runtime StartupMovies count=2, not 3

V0.47 runtime session on Oct 9 at 15:13 (NOT a V0.48 binary)
shows that the early RVA 0x25FF31 hook executed as intended without a
crash. Log from the existing unified Mod.log:
- "Skip Logos V0.46: exact-name prefix filter requested"
- "exact 7-byte startup copy hook installed"
- ASI reports 0.47-unified-compact-log.
- At native call: "StartupMovies count=2 validPtr=1"
- V0.46 incorrectly logs "FAIL OPEN, unexpected movie count or array",
  because count < 3 is hard-rejected BEFORE logging names.
- Session shuts down normally.

Critical constraint: V0.19B already tried copying a zero-count
StartupMovies array. That experiment removed the independently
controlled intro as well as logos. A naive V0.49 count>=2 change
would repeat that regression, so it is specifically NOT done.

V0.49 extends the existing non-mutating native diagnostic to
allow count=2 for *name reading* while still refusing to empty
the movie array. It prints the two FString entries and whether
they EXACTLY identify THQ_LogoBasic and AS_LogoBasic. If there
are only two entries, even an exact match returns false so the
original native video array and MoviePlayer path remain intact.
The original >2 filter behavior is unchanged. Result is a
focused investigation build, NOT a complete Skip Logos fix.
It preserves V0.48 single Mod.log with compact output, never
creates/deletes/touches any old Loader.log, and contains the
same cumulative gameplay features. When names are confirmed,
research a logo-skip mechanism that keeps the native
MoviePlayer and intro lifecycle (not count-zero, skipping
attachment, or early-function RET patch).

Public release remains unchanged. User should install both
dxgi.dll and ASI from the V0.49 test archive (four files at ZIP
root), launch with SkipLogos=1, then provide only
DarksidersGenesisMod.log.

## V0.50: safe startup and single-log recovery following V0.49 silent run

The user's report after the V0.49 diagnostic is: "il ny a plus de log."
No new runtime log or crash dump was supplied. This alone does NOT
establish whether the proxy was loaded, the ASI was quarantined,
the wrong binary was copied, log-file permissions failed, or the
experimental RVA 0x25FF31 startup hook failed before logging.
Do not claim a specific root cause.

The V0.49 source had a journaling reliability bug: g_loaderLogInitialized
was set TRUE before checking whether CreateFileW succeeded. The ASI
then trusted that boolean when deciding whether to truncate a prior
log. V0.50 changes the flag only AFTER successful CreateFileW and
adds an ASI check that the canonical logfile exists. This avoids
silent log-init success on failed file I/O.

More importantly, the V0.46-V0.49 early handwritten assembly
trampoline is REMOVED entirely from src/dxgi_proxy.cpp. DllMain
does NOTHING except record its HMODULE and call
DisableThreadLibraryCalls. No GetPrivateProfileIntW, no early
VirtualProtect, no early trampoline, no manual executable patch.
All early proxy diagnostics are written outside DllMain on first
DXGI factory/ASI loading, to a single
DarksidersGenesisMod.log (at the dxgi.dll/ASI install directory).

Skip Logos is TEMPORARILY UNAVAILABLE in V0.50; calls to the proxy's
DGSetSkipLogosEnabled return false for enabled=1, log a clear
warning, and leave the engine's MoviePlayer/startup screens native.
An existing user INI with SkipLogos=1 will not re-enable the hook.
The fresh packaged INI has SkipLogos=0. V0.49's observed count=2
remains in docs as evidence; the *last actual working video-skipping
fix is not established*.

All other ASI hooks, camera, horses, HUD, reticle and skip intro
remain unchanged. One compact Mod.log per launch (proxy truncates,
ASI appends); an existing DarksidersGenesisLoader.log is NEVER
created/opened/deleted/modified. This is a crash/log recovery TEST,
not a completed logo fix. Do not create a public release until
the user confirms in-game behavior. If the file is still absent,
diagnose ASI/proxy load/permissions/AV rather than patching
MoviePlayer at random. Four ZIP entries remain at root.

## V0.51 - Undo speculative V0.50 rollback; keep proven log improvements

The user clarified that their earlier report "no log" was a
wrong-file-location mistake. Their subsequently supplied V0.49
single-session log from 2026-10-09 15:39:20 to 15:39:57
proves **both proxy and ASI loaded correctly**:
- 0.49-two-movie-names-diagnostic startup marker.
- StartupMovies count=2.
- entry[0] 'THQ_LogoBasic'.
- entry[1] 'AS_LogoBasic'.
- TWO_LOGOS_CONFIRMED; no removal because earlier count-zero
  patch inadvertently removed the intro cinematic too.
- Healthy gameplay runtime, camera input, player capture and
  normal mod shutdown.

Therefore the *suspected* missing-log failure behind V0.50
is NOT an observed failure of V0.49. Revert ONLY V0.50's
forced disable/removal of the startup movie diagnostic
and restore the exact V0.49 native probe/hook implementation.
Do NOT modify the way it filters, shifts or handles a two-item
playlist. At count=2 it is an observer only, NOT a working
Skip Logos fix, and must be described that way.

Keep these useful improvements from V0.50:
- successful CreateFileW required before setting
  g_loaderLogInitialized.
- ASI checks unified Mod.log file existence before trusting
  proxy readiness.
- One compact per-session DarksidersGenesisMod.log.
- NEVER create, overwrite, or delete old
  DarksidersGenesisLoader.log files.
- No change to existing gameplay, camera, horse, HUD or reticle.

V0.51 is an exact experimental V0.49 startup diagnostic plus
hardened logging, built with SkipLogos=1 in the fresh test INI.
Existing user's INI remains honored; when off, early hook
is not installed. No public release until test validation.

Next development target: safely skip exactly the two confirmed
logo entries WITHOUT zeroing the entire StartupMovies list
or bypassing native MoviePlayer initialization. Avoid
guessing that list count 3+ or modifying the intro toggle.

## V0.52 - Two native startup logos suppressed by substitution (needs game validation)

User's Oct 9 15:50 V0.51 log confirms exact original UE playlist:
count=2, \`THQ_LogoBasic\` and \`AS_LogoBasic\` (in that order).
The previous experiment did not remove either entry.
V0.52 moves from read-only observation to actively omitting the two
logo videos with an explicitly constrained method:
- Leave the native \`UStartupScreensSettings\` and game-owned
  \`StartupMovies\` source array wholly unmodified.
- At exactly the same signature-validated RVA 0x25FF31 early
  copy point proven to run correctly by V0.49 and V0.51, return
  a static two-element FString descriptor array with names
  \`DG_Skipped_THQ_Logo\` and \`DG_Skipped_AS_Logo\`,
  pointing at immutable, process-lifetime UTF-16 strings with
  correct capacity and null-terminated sizes.
- The native \`StartupMovies\` count is left at **2**. The
  original engine-owned deep-copy loop copies these descriptors
  and the original \`GetMoviePlayer()->SetupLoadingScreen\` call
  runs untouched, unlike rejected count-zero V0.19B and
  MoviePlayer-attach bypass V0.19C.
- A mismatch of game fingerprint, instruction bytes, array
  pointer/count or exact logo names yields NO modification.
- For a future unexpected count >2 with the two logos at the
  start, return \`movies+2\` with new count \`count-2\`, keeping
  all other video entries.
- The original x64 island now uses a pointer-returning helper
  and emits \`test rax,rax; je +13; mov rsi,rax;
  cmp r14d,2; je +4; sub r14d,2\`. Old GP register/flag saves,
  shadow space and native continuation are unchanged.
- No V0.44 movie attach hook, synthetic F5, cursor rebind,
  MP4 file modifications, or alternate overlay behavior.
- A nonexistent movie path may cause black frames or a
  media-backend fallback; only the user's in-game test can
  establish timing/intro/focus behavior. It is not described
  as already validated.

V0.52 has a **normal version name** and \`downloads/DarksidersGenesis_V0.52.zip\`;
no "experimental" or "TEST" suffix per user request.
The zip retains exactly four files at root, cumulative features,
unified noncumulative \`DarksidersGenesisMod.log\`, and NEVER touches
\`DarksidersGenesisLoader.log\`. New INI keeps \`SkipLogos=1\`.
Do not tag a public release before user validates in-game:
1. Logos actually absent after fresh boot.
2. Warning screen / loading transition, and intro when
   \`SkipIntroVideos=0\`, remain functional.
3. Cursor/cross after loading a save, F6 reticle/overlay input.
4. No crash, no black screen that hangs indefinitely.
If any issue, set \`SkipLogos=0\` and restart, or restore stable V0.45.

## V0.53 - Freeze V0.52 Skip Logos + Skip Intro; compact UI

The user's V0.52 runtime log (2026-10-09 16:53) confirms:
- the early verified movie-list hook installed and matched
  the exact TWO entries: THQ_LogoBasic and AS_LogoBasic;
- two nonexistent names substituted with nonzero count=2;
- independent native intro CVar applied with SkipIntroVideos=1;
- player identity, reticle hooks, camera, horse hooks, damage
  and movement initialized; normal shutdown.
In-game USER FEEDBACK: reticle appears OK; Skip Intro and Skip Logos
appear to work. The user explicitly asks to FREEZE their
implementations. Therefore V0.53 does not change:
src/dxgi_proxy.cpp, src/SkipLogosFeature.cpp, native Skip Intro
implementation or native reticle handling. Preserve exactly V0.52.

V0.53 normalizes SkipLogos=1 and SkipIntroVideos=1 across
new install INI, C++ Store member defaults, Reset Defaults,
fallback reads, and RuntimeSettings init. ConfigRevision=2103,
with explicit migration for older config revisions:
- Old pre-2102 bypass configurations retain historical safety OFF
  migration (do not reenable the unsafe legacy bypass).
- Existing revision 2102 settings retain user choices, including
  deliberate SkipLogos=0 or SkipIntroVideos=0; updating to
  revision 2103 only synchronizes saved metadata.
- Fresh installs and the Reset Defaults button select both ON.

Overlay: beside each numeric slider's Reset button, show ONE
dimmed LIVE/WAIT indicator, with a hover-only tooltip carrying
technical hook details. Keep rich Combat, Horse, Camera and Recovery
diagnostics accessible through collapsed Debug tree nodes.
System has concise READY/restart status. Native values are not
fabricated and no runtime feature implementation changed.

NEXT FEATURE (not implemented in V0.53): new separate
SkipWarning option to remove exactly the two startup notices
"play with controller" and "autosave warning". They are not
part of StartupMovies (which has exactly the two logo names)
and must NOT be removed by the name-substitution hook. The
startup screen definition array (noted previously at
UStartupScreensSettings +0x50, stride 0x40) is a separate
candidate requiring correct native timing and ownership audit.
Neither DisableStartupScreens nor bypass SetupLoadingScreen is
acceptable since these previously disrupted intro/cursor.
Do not expose a checkbox that promises functionality before
the independent warning suppression is implemented.
Once implemented, choose a user-controlled INI default
separately from the two frozen video skips.

Normal cumulative archive, four root-level files, new V0.53
label, no "TEST" suffix. Unified per-session Mod.log; old
Loader.log untouched. No public release/tag without user request.

## V0.54 Third Person, HUD and hotkey cleanup
Uses user-provided F1 HUD / F2 Movement / F3 Recovery / F4 Reticle and camera controls, all defaults copied to the new INI. Removed Reticle Focus Test action and all synthetic focus pulses. HUD Hidden is the only display toggle, with F1 still supported. Third Person is now an opt-in native camera view transform (OFF by default), using original springarm distance to estimate pivot and apply transient shoulder-height view. May require in-game tuning. Skip Warning will use an independent guarded StartupScreenDef filter, not the movie list. No changes to validated Hide Reticle or Skip Intro logic.

## V0.54 Skip Warning independent attempt and Third Person

User log for V0.53 confirms logos are matched/substituted, the
native intro CVar is set, camera hooks, HUD, reticle, movement and
horse hooks enabled; user INI supplies F1 ToggleHUD, F4 ToggleReticle,
F5-F12 None and camera keys with Home/End + PageUp/PageDown.

The V0.54 Skip Warning checkbox controls a SEPARATE guarded filter
for the startup-screen definition array (settings +0x50; each 0x40
bytes in original disassembly, unrelated to StartupMovies at +0x38).
The known working early 7-byte startup hook passes the original
movie TArray field pointer through volatile r8 as the third argument
to the existing C++ name filter. Before the native MoviePlayer
Setup call, if native definition count is EXACTLY 2 and pointers,
capacity, regions and write permissions are valid, set only
StartupScreenDef.Num=0. Existing actual THQ+AS name replacement and
movie count=2 stay the same. No external files touched. On unexpected
count/layout, warnings remain and a fail-open line is logged.
This is a gameplay *attempt*, not yet validation that controller and
autosave notices are actually controlled by these definitions. An
existing INI can set SkipWarning=0 for the next boot.

Third Person: optional, OFF in the user's provided INI. Native view
and arm hooks already existed; new transient camera output computes
a pivot from original view/pitch/yaw and spring-arm length, then
repositions the view with a shallow -12-degree pitch and a 0.25x-3x
distance multiplier. This may not exactly follow character facing,
and needs real camera test. Switch OFF to restore native framing.

Removed ReticleFocusTest, its synthetic focus pulse, and stale F5
description. HUD Hidden is the only overlay HUD toggle; F1 action
remains. No change to validated reticle suppression/Skip Intro hooks.
User defaults copied into INI. New normal V0.54 build/archive ZIP
has all four files at root; no public tagged release yet.

## V0.55 (9 October 2026): Third Person distance controls, DOF, hotkeys, Skip Warning

- Third Person Distance now has a manual numeric input, a 1.00x Default button, 0.25x–3.00x limits, and finite-value validation. F5 toggles Third Person by default; INI ConfigRevision 2105 migrates a V0.54 F5=None to this new default without changing other user bindings.
- Every working binary feature toggle is selectable in the F1–F12 Hotkeys list, including HUD, Reticle, Movement, Recovery, Skip Intro, Skip Logos, Skip Warning, Third Person, FOV, Pistol/Melee Damage, Jump, Glide, Horse Speed/Sprint Speed/Sprint Duration, Hotstreak, eight camera directions, and Camera Reset.
- Depth of Field: while Third Person or manual Zoom displaces the camera, the mod temporarily sets the verified native UE4 console variable r.DepthOfFieldQuality to 0, restoring its captured native value when both modes are off (and at normal shutdown). This avoids incorrect blur but disables DOF while modified; it is not a physical focal-distance recalculation. The CVar registration signature and storage were confirmed against the supplied exact game EXE. No Engine.ini write.
- Skip Warning V0.54 did not work because its array adjustment ran after the native screen-definition loop. V0.55 moves the intercept to a distinct exact-byte-validated native sequence at RVA 0x25FEBF, replaying the original instructions and skipping that loop only if exactly two 0x40-byte screen definitions exist. Neither source array nor MoviePlayer is modified. Outcome needs in-game validation. Disable via SkipWarning=0 and restart to isolate.
- Skip Logos name substitution, Skip Intro CVar, reticle, horse and gameplay internals remain unchanged. ZIP at root contains dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md. Only DarksidersGenesisMod.log is active per run; no touching the old Loader.log. No public release until validated.

## V0.56 (9 Oct 2026) - Third Person pose, mouse/right-stick orbit, warning native span audit

- User validated V0.55 DOF: native quality 2 -> 0 when Third Person enabled, 0 -> 2 when disabled. Keep V0.55 DOF unchanged.
- Expose previously fixed Third Person pitch -12 degrees, vertical offset +60 Unreal units and existing distance multiplier in Camera tab; reset controls and INI persistence. Preserve manual camera height too.
- CameraMouseGamepad=0 by default, optional right-stick XInput (dead-zone) and RAWINPUT mouse with WM_MOUSEMOVE fallback. No game input consumption, no cursor synthesis. Accumulated orbit offsets are session-only and reset when disabled/unfocused/menu open; sensitivity and stick speed persist. The WndProc observer is installed on demand without initializing ImGui. In-game camera control requires testing.
- Skip Warning V0.55 observed its native loop but failed count=2 guard. V0.56 records the actual byte span and preserves original RFLAGS/RAX/RDX; one/two 0x40-byte definition counts are allowed to bypass, all other counts fail open. Still experimental until warning pages disappear in-game. No original movie array, intro, reticle or native UObject writes.
- ConfigRevision=2106 migrates user settings without changing existing hotkeys; 4 files at ZIP root; no tagged release until validated.

## V0.57 (9 October 2026) - Isolate original aiming from Third Person camera

User feedback: V0.56 Third Person controls work very well. The Depth of Field compensation remains validated. Skip Warning still DOES NOT WORK, and is deliberately unchanged in this build (pending separate diagnosis). The native aiming visual effect remains for future work; it is NOT hidden in V0.57.

When Third Person is active, gameplay focused and ImGui closed, the camera input observer now consumes native WM_MOUSEMOVE and mouse WM_INPUT notifications after capturing the movement for our orbit camera. This does not intercept mouse buttons, clicks, scrolling, keyboard input, focus or Alt-Tab. The observer attaches on Third Person activation regardless of whether camera mouse/gamepad input is enabled; it is not a global Windows input blocker. All events are passed through again when Third Person is off, menu open, or focus lost. Native GetCursorPos and direct UE player aim logic are NOT hooked, so an absolute-position aiming path may still rotate the player; do not claim complete native aim suppression without in-game test.

A separately scoped MinHook filter masks only XInput X/Y right-thumbstick values exposed to the game while Third Person is on. Left stick, buttons, triggers and other inputs remain intact. The mod's orbit camera polls the unfiltered original XInput trampoline. Tries xinput1_4, xinput1_3, xinput9_1_0, xinputuap and GetStateEx ordinal 100. If the game reads controller input through Steam Input/HID instead, this method may not catch it; aim filter counts in the log identify whether the XInput gate was exercised.

Both filters fail open outside the exact validated game and are disabled when the overlay is open or game unfocused. Counters are printed only when first exercised, at 10,000 messages, and on orderly shutdown. V0.56 camera pose, DOF, skip logos/intro, reticle, horse and gameplay mechanics are left unchanged. Install test ZIP with 4 root-level files. No public stable release without in-game validation.

## V0.58 (9 October 2026) - controller TPS combat axis test (UNVALIDATED)

User confirmed V0.57 filtering via log: mouse native move messages suppressed 9,777, right stick native XInput reads filtered 26,909. This proves the game actually polls XInput 1.3/1.4, and is a safer integration point than writing actor rotations or movement-component flags without validated native offsets. Skip Warning still FAILS: loader span is 0xC0 (three 0x40-byte definitions), so V0.56's 1/2 definition gate does not skip; **no changes to Skip Warning** in this build.

User requests a real controller TPS: move camera with right stick while firing/throwing, aim in the facing direction, and move backward/sideways with the left stick. The existing native right-stick filter now synthesizes the engine's own directional aim input during RT (fire / throw) and RB (secondary fire). Desired direction uses the validated camera's transient applied yaw relative to vanilla native yaw: RX=sin(delta)*32767, RY=cos(delta)*32767. Left stick is rotated by the same camera delta in the native game's input frame (X'=LX*cos(delta)+LY*sin(delta), Y'=LY*cos(delta)-LX*sin(delta)). Input magnitude is preserved, avoiding direct player actor writes. Controller 0 only; LB ability menu and all other controls remain vanilla. Outside active fire/throw, V0.57 right-stick isolation remains. A camera calibration yaw adjustment -180..180 degrees is exposed for game-specific aim mapping if axes differ.

New [Features] TPSControllerCombatAim=1 and TPSControllerStrafe=1, [Values] TPSAimYawOffsetDegrees=0, ConfigRevision=2107, all active ONLY with ThirdPerson ON + foreground gameplay and exact supported game. The user can independently disable either experimental mapping in Camera. Applies to gamepad only; no mouse aim change. No assumptions about static player pointers or native yaw offsets are introduced. V0.56 pose, validated DOF, reticle, HUD, horses, intro and skip logos remain as they were.

WARNING / test limitation: this is an XInput-level prototype, **not** a proven native SetActorRotation or movement-component orientation-to-movement implementation. Native game action code must actually rotate Strife from the synthesized right-stick aim, and movement must retain lateral facing. If game auto-orients the pawn to moving direction, a follow-up *native actor/movement rotation hook* is required after exact executable disassembly; do not claim true TPS strafe yet. Test camera facing while shooting with RT, RB, throwing held item, left-stick strafing while RT held, LB radial selection, Third Person OFF and F5 and Alt-Tab. New log TPS V0.58 records first 1/120 firing samples, strafe samples and end totals. Request game observations, rather than more memory writes based on guesses. Workflow artifact is delivered via GitHub Actions artifacts by user preference, not chat sandbox.

## V0.59 (9 October 2026): exact executable audit, absolute-world yaw & live facing probe (EXPERIMENTAL)

User supplied the exact exe bytes `DarksidersGenesis-Win64-Shipping(1).exe` (62,113,280 B, SHA-256 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54). Native PE .text/RDATA reflection audit confirms `GetNormalizedAimRotation` native generated dispatchers and actual functions at RVA `0x638240` and `0x570260`. Both implementations read `RootComponent` from Actor `+0x158`, a cached component FRotator at root `+0x1F0..0x1F8`, and actor aim state at `+0x900/+0x904`. These getters calculate angle differences. They are **not** safe, verified SetActorRotation entrypoints and must not be used as setters. `GetPlayerLookRotation` is present in native registration, but its owner/callers require further audit. No arbitrary actor rotation writes introduced.

V0.59 collects READ-ONLY local actor worldYaw from root+0x1F4 inside the already validated live player movement GetMaxSpeed callback (in-game validity predicate from jump/double-jump/glide). It checks allocated memory with VirtualQuery, finite angle and clips into -180..180; retains only float+tick. Zero cached UObject pointer derefs from the D3D11 Present or XInput hooks. Log first, 120th, 10000th player world yaw reading. Aim logs snapshot `actorYaw` and `actorFresh` if sample is within 200ms.

V0.58 used `appliedCameraYaw-nativeCameraYaw` as a WORLD aim target; this changes dramatically when the native camera reorients. V0.59 **separates** world-space desired aim and relative camera movement: combat right-stick synthetic target is now `appliedCameraYaw+TPSAimYawOffsetDegrees` (absolute world yaw), fed as sin/cos at 72% magnitude, rate capped at 180deg/s from last aim command; initial heading seeds from latest validated actor yaw when available. Relative applied-native yaw is retained only to transform camera-relative LEFT stick input. No writes to native player orientation or camera UObject. This is a controlled attempt with additional runtime diagnostics, NOT proven to achieve the requested TPS facing/strafe. Third Person OFF or menu/focus loss reverts to game native inputs. Existing V0.57 Win32/XInput input gates, validated DOF/horse/reticle etc preserved.

Test: third-person camera enabled, RT shooting still camera vs while moving sideways, RB secondary, grenade throw, standing turn, release RT, F5 OFF. Examine `TPS V0.59 combat sample`, `TPS V0.59 live actor facing` and outcome; if actorYaw remains stale or orientation misaligns, next implementation must hook authoritative combat facing or native movement orientation and verify with a video. Skip Warning remains *nonfunctional*, span 0xC0 three startup entries, unchanged for this version.

## V0.60 (9 October 2026): left stick safety rollback + TPS aim paced by camera (TEST)

User rejected V0.59: pushing LEFT STICK UP can make player travel RIGHT, and camera and actor turning are not well synchronized. V0.59 log (20261009-200302) verifies player yaw read 14,855 times. First RT sample: camera=-74.0, native=-45, command=-94.2, actor=-97.1, error camera-actor = +23.1 degrees. Sample #120: camera=+2.2, command=+2.2, actor=+0.2, error +2.0 deg. Therefore actor rotation is updated and converges; remaining early discrepancy + unpredictable left-stick behavior must be isolated.

**Cause / mitigation:** V0.58/0.59 rotated BOTH left stick axes with a per-frame camera-relative delta despite the game having its own movement-to-heading mapping. This can effectively rotate "forward" into a "rightward" analog input, especially at 90-degree camera orbit. V0.60 disables `TPSControllerStrafe` by default, including a **targeted migration from revision 2107 to 2108**. Native left stick remains byte-for-byte unchanged. The old transformation is retained ONLY when user explicitly enables EXPERIMENTAL left-stick rotation in Camera, and then ONLY during RT/RB attack so normal locomotion is unaffected. The goal is to verify whether the native combat facing itself already yields correct forward/strafe movement when the native right stick is driven by TPS camera heading.

**Aim pacing:** absolute camera world yaw continues V0.59, but cap command step to the configured `CameraStickSpeed` (30..180 deg/s, default 135 deg/s), rather than fixed 180 deg/s faster than camera. Synthetic native aim-stick length reduced from 23592 (72%) to 18022 (55%), still well above gamepad deadzone. Existing actor-yaw probe, boot/HUD/reticle/horse/DOF/skip logos and intro remain unchanged. Logs now include left stick raw passthrough first/120/10000, camera world yaw vs live actor yaw, and final counts including moving while firing.

**LIMITATION:** this patch removes a provably wrong extra transform but does not guarantee a true native character-facing lock or strafing animation. If native gameplay reorients actor toward walk direction during firing, the next fix must hook the authoritative in-game control rotation/movement orientation, based on disassembly with exact EXE, rather than another synthetic stick transformation. Test priority: ThirdPerson ON, rightstick orbit, RT fire while leftstick UP, then LEFT/RIGHT/BACK, then release RT and move. Leave EXPERIMENTAL rotation OFF. A full game exit supplies totals and early yaw samples. Skip Warning stays nonfunctional (`0xC0` span, three definitions) and unchanged. This is V0.60 TEST, not release.

## V0.61 (9 October 2026): native TPS combat facing, camera pivot stabilization and aim ground FX test

V0.60 user result: strafe while firing still absent (moving camera can make it look like strafing); vanilla visual aiming effect and aiming camera lateral soft offset disturb TPS. Log: 3420 combat polls, 7010 native left stick passthrough, user explicitly enabled experimental remap 868 times, first RT cameraYaw=-45 vs actorYaw=+44.4 (gap -89.4). The previous XInput axis-rotation approach cannot independently control native character facing.

Exact supplied game EXE reflective disassembly: `bOrientRotationToMovement` UCharacterMovementComponent native generated setter at RVA `0x1CF0300` has bytes `80 89 40 02 00 00 10 C3`: OR bit 0x10 at movement+0x240. V0.61 in the *validated live local-player GetMaxSpeed hook* clears only this bit during ThirdPerson RT/RB firing, then restores bit if previously owned once fire stops/TP off/overlay appears; never dereferences stale cached components. [Features] `TPSLockCombatFacing=1`, opt-out; default left-stick XInput passthrough unchanged. Does not guarantee true TPS until user tests.

[Features] `TPSLockCombatCameraPivot=1` stabilizes the native springarm camera target during RT/RB, using last idle camera-to-live-player XY relative offset, refreshed only from live local-player ComponentToWorld.Translation (root+0x1D0..1D8), with 250ms actor freshness, 12s baseline, world plausibility checks, and reset on actor identity generation. Does not freeze camera in the world when the player moves; only avoids native top-down aim target side-drift. OFF returns V0.60 transient camera positioning.

[Features] `TPSHideGroundAimFx=0` optional EXPERIMENTAL native effect hook at RVA `0x6DDAD0` (`HideGroundTargetingEffect(bool)` verified bytes). Forces hide=true ONLY when this function is called during ThirdPerson. May hide ground grenade/item aim effect. **NOT PROVEN to affect Strife gun laser or generic vanilla aiming FX.** No indiscriminate FX disabling. Hook doesn't retain effect actor pointer; native effects may only restore when native visibility call happens. Existing F4 reticle remains independent. UI exposes separately all three options and preserves cumulative features.

INI revision 2109 migrates previously enabled experimental left-stick reorientation back OFF to prevent double transforms. Skip Warning known to fail on three 0x40 definitions (span 0xC0), no change. V0.61 TEST pending user feedback with native-facing/restore, pivot compensation and ground aim hook log lines; no official Release.

## V0.62 (9 October 2026): full-time TPS facing test (unvalidated)

User request: "il faut tester d'avoir le personnage toujours dos à la camera". V0.61 activates native `bOrientRotationToMovement` bit clearing only while RT/RB held. Log previously showed 25 locks/25 restores and large yaw error despite gamepad synthetic combat aim. V0.62 tests keeping Strife's **native facing directed by camera all the time**, whether idle, moving forward/backward/sideways or firing.

New `TPSAlwaysFaceCamera=1` (ConfigRevision 2110), with an independent overlay checkbox in Camera. ON only in ThirdPerson with `CameraMouseGamepad=1`, focus on exact supported executable, closed ImGui, controller index 0, and fresh validated live player sample <200ms. The XInput native aim path is now synthesized continuously (not only during RT/RB), using existing camera absolute yaw, rate-limited to configured camera stick speed, and 55%-magnitude native right stick. The camera samples the actual original controller right stick through XInput trampoline, unaffected by this. The authoritative movement `bOrientRotationToMovement` bit (+0x240, mask 0x10) is cleared in the *validated live player GetMaxSpeed callback* while recent (220ms) camera-facing aim pulses exist, allowing left stick to strafe without reorienting to movement. Only owned flag is restored on the next valid native callback after TPS OFF, option OFF, game focus/overlay loss or inactivity; never restore through stale cached UObject pointer. While locked, if the game reasserts the bit, it is cleared again and separately counted. Left stick remains **unchanged**; old experimental left-stick remap stays OFF.

New logs `TPS V0.62: always face sample` include cameraYaw, real actorYaw, signed yaw error, commanded yaw, left axes, firing state; final command/reassert totals. In-game test must check idle rotation, moving in all directions, shooting, LB wheel, F5 OFF, old effect indicators and camera pivot. This is still a synthetic native aim route, NOT verified direct actor rotation. It may cause the native vanilla aim visual outside shooting, especially until that visual is identified properly, and may not perfectly lock actor orientation depending on native aim code. Fail-open if no active validated player/controller/camera; in-game test result essential before declaring functional. Existing ThirdPerson pose, native visual ground FX option, camera pivot stabilization, FOV, DOF, horse, combat damage, HUD, reticle, intro skip and logos unchanged. Skip Warning still does NOT WORK and is unchanged.

## V0.63 (10 October 2026): rejected always-face reverted; camera pose defaults & native facing cone

User rejected V0.62 Always Face Camera: it simulates Ctrl-like permanent aim until dodge and restricts some movement. Log showed `alwaysFaceCommands=15855` even when RT=RB=0. All idle synthetic right-stick input is removed; TPS combat synthetic aiming is active only with RT/RB and `TPSControllerCombatAim=1` (default ON), while existing V0.57 idle input isolation stays. Experimental left-stick rotation was observed 3542 times in the user's log; implementation, UI and INI option are removed completely. Native LX/LY are always unchanged.

New [Features] `TPSCameraFacingGuard=1` and [Values] `TPSFacingToleranceDegrees=90` (20..180): in the exact validated live-player movement callback, compare actual player root yaw against final camera yaw. Inside this 90-degree cone, temporarily disable `bOrientRotationToMovement` bit +0x240/0x10 to reduce automatic movement-oriented turning without forced aiming. Outside the cone restore the native bit and allow vanilla orientation; combat lock from V0.61 still applies while RT/RB. **This does not directly rotate the character** and is experimental; only full game testing can validate strafing and dodge. Log counts cone IN/OUT, owned lock/restores.

New Third Person Distance range 0.0..3.0, default 0.50 (0 radial arm distance now genuinely allowed by camera calculation, old clamp min 120 removed). Vertical offset default +180 (was +60). Overlay Default controls and F-key Camera Reset updated. ConfigRevision 2111 migration updates only unchanged prior defaults 1.0/60.0 and preserves customized values; removes old deprecated INI keys.

Hide Vanilla Ground Aiming Effect test was nonfunctional (native hook installed yet zero calls). Remove that misleading option and hook while continuing future search for the actual visual effect path. Other camera pivot stabilization, DOF, horses, HUD, reticle, skip intros/logos unchanged. Skip Warning still fails span 0xC0 and remains deferred. Build is V0.63 TEST pending validation.


## V0.64 (10 Oct 2026): TPS player-follow camera, zone camera baseline, independent TPS FOV, input separation (TEST)

User findings after V0.63: region triggers override native camera location/angles/zoom; some global camera sliders appear not to work while ThirdPerson is active; mouse reticle aim is grounded in top-down world-plane projection and may jump between front/back at a horizontal projection boundary; right stick originally rotates player; need player-anchored camera and separate TPS FOV. These are distinct systems. **Do not claim a fix for mouse weapon aim** until the game's mouse-target world projection, shot origin and weapon traces have been identified. Former V0.62 Always Face rejected (simulates held Ctrl). Never reintroduce permanent native aim commands.

**[Features] TPSFOV=1** and **[Values] TPSFOVDegrees=90.0** (40..140) are independent from the existing global FOV override. When ThirdPerson=1 AND TPSFOV=1, TP FOV wins regardless of global FOV. Otherwise the global FOV setting works exactly as before, or native FOV if global also OFF. Observable `TPS V0.64: independent FOV` logs. The ordinary FOV override is still usable outside TP.

**[Features] TPSFollowPlayer=1**: in the validated local player's native GetMaxSpeed callback, previously existing read-only root ComponentToWorld.Translation (X/Y/Z) + update tick + generation are published. Camera V0.64 uses that fresh (max 250ms) actor XYZ to construct FMinimalViewInfo.Location: pivot = player XYZ; output camera = pivot - cameraForward * TPSDistance + TPSVerticalOffset and global CameraHeightOffset. Rotation is the TPS view pitch and yaw. It does **not** install SetViewTargetWithBlend, reparent UCameraComponent, set camera UObject transforms or preserve stale player pointers. This is an effectively player-attached *transient camera view*, with fallback to V0.63 camera reconstruction if no validated live player. Guarding native output proximity (XY under 15000 UE units, Z under 7000) avoids overriding distant cinematic camera views. The camera's native collision/SpringArm trace is not guaranteed to continue as before, because final rendered view location is reconstructed, so test collisions/walls.

**[Features] TPSLockZoneCamera=1** (effective only when TPSFollowPlayer=1): freeze a baseline native camera yaw + SpringArm distance at first validated nearby native gameplay view per player generation. Derive TPS yaw from that baseline plus existing cameraYawDegrees and player orbitYaw; derive TP distance from captured native arm length * ThirdPersonDistanceMultiplier. Region-triggered native yaw/distance/target-pivot changes then no longer drive the TPS rendered view. Resets on ThirdPerson OFF or player generation changes. Does NOT patch the game's camera zones, triggers, assets, camera volumes or cinematic state; only replaces the final transient gameplay camera output when proximity/player validation passes. If TPSFollowPlayer=0, old native-derived camera algorithm and old TPSLockCombatCameraPivot are retained. With follow enabled, TPSLockCombatCameraPivot is redundant; UI explains these interactions. Generic CameraPitchDegrees gets overridden by View pitch##TP. Native SpringArm Zoom primarily affects vanilla/fallback camera, NOT attached zone-locked TP; these old controls are NOT missing code, but superseded by TPS mode.

**[Features] TPSSuppressNativeRightStick=1**: independently toggle the V0.57 original XInput filtering. When ThirdPerson and overlay closed and player 0, original controller right stick is blocked from native character-yaw interpretation outside RT/RB; our camera still reads RAW XInput via native original trampoline for orbit. OFF lets native right stick also rotate character; selection wheel LB, co-op, unfocused/TP OFF remain vanilla. Actual RT/RB combat aiming when TPSControllerCombatAim=1 continues its own native synthetic aim behavior independently.

**[Features] TPSSuppressVanillaMouseAim=1**: independently toggle V0.57 Win32 WM_MOUSEMOVE/WM_INPUT suppression; our mouse orbit can still read original relative mouse deltas, but this does *not* neutralize all game mouse aiming paths, absolute cursor world projection or targeting. Mouse and gamepad aim remain fundamentally different, and visual "horizontal line" / target front/back switch is unresolved. Treat mouse control as an open native target-projection task, not as a patch victory.

**[Values] TPSCombatAimTurnRate=180.0** (30..360), now separate from CameraStickSpeed (camera orbit) because character aiming and camera rotation are different systems; only native synthetic RT/RB aim uses this rate limiter. No forced aim outside actual RT/RB. Reticle suppression F4 remains separate.

**Config revision fix:** SaveNow historically writes literal ConfigRevision=2106 despite current kConfigRevision increasing to 2111, causing migration on future loads. V0.64 writes current revision 2112 using swprintf_s, preserving hotkeys and all calibrated existing settings. New flags default to 1 as indicated and new values default to 90 FOV / 180 aim turn. TP distance 0..3 default 0.5 and TP vertical offset default +180 retained.

V0.64 test tasks: activate F5 TPS; toggle FPS FOV against global FOV; walk/rotate camera and confirm player remains framing center; cross one known camera-changing zone and verify angle/distance stable; walk close to wall and check collisions; test right stick native-suppression on/off while camera orbit remains; test mouse aiming separately and log what remains broken; open/close overlay, co-op, mounts, cutscenes, Alt-Tab; TPS OFF returns original. The local player camera-follow is conditional and not a true camera UObject attachment, so this test is **not** a validated final fix. Existing horse/DOF/HUD/reticle/logo/intro code retained. Skip Warning still nonfunctional (0xC0 native span count 3), not changed.


## V0.65 (10 October 2026): simplified foot-centered TPS camera TEST

**User goal:** replace the collection of competing Third Person camera tweaks with a centered, player-linked camera, suppress native camera-zone changes except cutscenes, keep the native right stick from rotating Strife, and place the reticle at the screen center. Previous V0.64 gameplay NOT validated.

**Camera:** exact game executable only. After game GetCameraView, use an independent transient FMinimalViewInfo pose computed from the real local-player root XYZ. Pivot XY = player root X/Y. Pivot Z = rootZ - [Values] TPSFootAnchorOffset (default 88 Unreal units), then + ThirdPersonHeightOffset (default 180). This foot correction is **approximate**, since exact capsule half-height has not been validated; editable 0..200 in overlay. The orbit reference yaw is the actual player yaw captured when TPS camera mode first locks, **not** the top-down native camera yaw. Right stick/mouse orbit yaw/pitch act relative to that initial direction. Native SpringArm distance is sampled once per player generation; camera-zone yaw, XY aim drift and zoom changes should no longer alter the attached TPS view. Character rotation is NOT forcibly tied to camera orbit.

**Cutscene fail-open:** native view is returned without any TPS/FOV/reticle transformation if player data is stale >300 ms, native camera is farther than 6500 units XY or 7500 Z from player, or native pitch is outside [-89,-15] degrees. This is only a gameplay/cutscene **heuristic**, not direct cinematic-state detection. A near-player cinematic using a similar top-down pitch can still be mistaken for gameplay. Persistent UCameraComponent/SpringArm state is not written. Collision with walls is NOT automatically guaranteed for our final override.

**Center reticle:** [Features] TPSFixedCenterReticle=1 draws lightweight ImGui crosshair at exact viewport center while TPS gameplay camera is active and the game is foreground. Existing native UI cursor-visibility hook and SetCursor hook blank the vanilla cursor while this display is active. No mouse-warp or button intercept is used. This is a *visual* fix only: actual mouse screen-to-world targeting and bullet direction still use vanilla game aim rules and need separate analysis. Draw initialization stays lazy: triggered only by opened menu OR active fixed TPS reticle. Cinematics and TPS OFF hide the crosshair; independent existing Hide Reticle / Hide HUD still take priority.

**Inputs:** Controller's original right stick is filtered from native player rotation via existing TPSSuppressNativeRightStick=1 while idle, but is separately read unfiltered for camera orbit. TPSControllerCombatAim=1 remains enabled while RT/RB firing, which can intentionally steer native shot aim and the player while attacking; no permanent virtual Ctrl aim as rejected V0.62. CameraMouseGamepad defaults ON (migrates existing revision <=2112). Superseded experimental movement-facing cone and native soft-aim-pivot correction now default OFF and are removed from primary TPS UI, retained as INI-only compatibility diagnostics.

**ConfigRevision=2113.** Preserve existing FOV TPS override, other mod features, horse, HUD/reticle, intro/logo skips, hotkeys. Skip Warning is still broken and unchanged. Test: idle/walk/dodge player screen center, rotate camera, adjust foot offset, zoom/distance/height, move through camera zone, approach walls, launch near/far cutscene and return, mouse/controller input, overlay, Alt-Tab, TPS OFF. Test-build only, not official Release. Versioned permanent ZIP + rolling latest test prerelease.


## V0.66 (10 October 2026) TEST: camera eligibility repair & native walking strafe

USER BUG REPORT V0.65: "la camera ne change pas / avec la manette les pas de coter ne marche pas". User's V0.65 log `DarksidersGenesisMod(20261010-185350).log` proves the exact-supported EXE and hooks READY, local player movement validated, 8,062 actor-yaw samples, but `TPS V0.65: native camera returned to game (cinematic/invalid player) n=1 near=0 pitch=-45.0`, and NO recorded foot camera attached / independent TPS FOV line. The V0.65 gameplay guard refused native top-down camera because of its tight 6500 XY / 7500 Z proximity, so camera remained vanilla. Also 374 original left-stick samples, only one native strafe lock/restoration while actually firing, meaning no persistent movement-facing lock for passive strafing. Mouse input filtered 5,345, right-stick filtered 76. Do not call these fixes verified until tested.

### Camera eligibility repair
V0.66 checks the native playable top-down camera signature: fresh exact live player world coordinates + native pitch [-89,-15] + either proximity within 6500 XY/7500Z OR ordinary game native pitch [-55,-35] and FOV [60,85]. This **rescues the player's valid gameplay camera even if native view-to-player distance is larger than 6500**. At the first few rescued or rejected frames, logs now print native camera and player XYZ, XY and Z gaps, FOV and pitch. This is a diagnostic fallback and **not a proven native cutscene detector**: a cutscene sharing identical camera pitch/FOV may be mistaken for gameplay. Conversely unusual game camera angles still restore vanilla. No persistent camera UObject changes. Also, if no SpringArm value is captured yet, use a one-time 900-UU base arm for TPS so missing arm telemetry can no longer silently block all TPS changes; captured native arm is retained when available. Keeps V0.65 player-root-to-foot pivot and independent TPS FOV.

V0.65 reset `tpsViewGameplay=false` at start of every GetCameraView, including secondary non-game views. V0.66 only clears it when TPS is disabled, then uses the existing fresh successful view timestamp (<250ms in Present) for crosshair visibility; avoids other view samples cancelling the valid game view in the same frame.

### Controller strafe test
New [Features] `TPSStrafeLock=1` (ON by default) and overlay toggle `Keep player facing while strafing (TEST)`. In the validated live local-player GetMaxSpeed callback, if Third Person is ON, the game is focused, ImGui menu closed, and MovementMode is Walking (1) or NavWalking (2), clear only **bOrientRotationToMovement** bit +0x240 mask 0x10 so left-stick movement cannot automatically reorient player to walk direction. Restored when TPS OFF, option OFF, overlay/focus loss or movement mode changes on next valid callback, without writing stale UObject pointers. Unlike rejected V0.62 mode, DO NOT inject persistent CTRL or virtual right-stick aiming; continue original left-stick passthrough and original right-stick camera-only input outside RT/RB. Actual firing still retains its separate TPSControllerCombatAim option. Log strafeLockTicks and native flags plus actorYaw/cameraYaw.

This suppresses only one native turning mechanism, not necessarily every animator/ability/pawn-rotation path. Confirm real backward/sideway movement with controller **without firing**, during shooting, after dodges and when switching to non-TPS mode; report failures instead of assuming success. Old V0.65 fixed crosshair and foot correction are kept and remain display-only. Avoid claiming weapon aim follows the crosshair; mouse aim world-ray remains outstanding.

Build version V0.66, ConfigRevision=2114 (backward compatible, old INI defaults preserved), ZIP files at root, rolling latest TEST prerelease and versioned downloads V0.66; official Release unchanged and still needs user validation. Skip Warning still non-functional.


## V0.67 (10 October 2026): correct native root component world translation (TEST)

**User V0.66 result:** camera goes to a remote point on map and does not follow the moving player. User log `DarksidersGenesisMod(20261010-190828).log` shows explicit native view (31838,-6410,3024), reported actor world location **(0,0,0)**, synthetic TPS foot view (-389,966,313), repeated with 32,477-unit planar gap. This definitively rules out a mere spring-arm zoom, FOV or player yaw issue. Mod was attaching to wrong coordinates.

**EXE-backed root cause proof:** user-supplied exact shipping executable SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. Native assembly at RVA **0x669728** loads RootComponent from `Actor+0x158`, and at RVA **0x66976C** executes `movups xmm1, XMMWORD PTR [rax+0x1A0]` and extracts X/Y/Z scalar via `movss` + `shufps`; this is a grounded read of world translation. Prior V0.64-0.66 code incorrectly assumed world translation at `root+0x1D0/+0x1D4/+0x1D8`, which silently returned 0/0/0. V0.67 reads `root+0x1A0,+0x1A4,+0x1A8`, only inside the existing validated live-player GetMaxSpeed callback and with page/finite-value guards; no saved UObject pointer writes. Logs first, 120, 1000, 10000 `TPS V0.67: live actor transform` with real world coordinates; TPS camera uses these coordinates directly, so movement can actually carry the camera. Player yaw getter remains unchanged (root+0x1F4).

V0.66 workaround that accepted a far-away native view solely because pitch was -45 and FOV 70 is now **removed**; synthetic TPS only writes when fresh validated player coordinates are within 6500 XY and 7500 Z of native view and pitch in [-89,-15]. This protects arbitrary remote/cinematic viewpoints and prevents accidental teleporting. Still a heuristic, not explicit cutscene detection. No angle/height/zoom/FOV changes, no changes to native right stick, TPSReticle/face while strafing logic, mouse input or all other already present mod features. Strafe lock was active 4,908 native movement callbacks in V0.66 but user has not validated genuine strafing; do not describe it as resolved. V0.67's sole runtime objective is correct live-player world coordinates and stable player-following TPS camera. The old F4 reticle and Skip Warning (still ineffective) are unchanged.

This is experimental V0.67 TEST with ConfigRevision unchanged (2114), validated Windows Actions build required, versioned downloads archive plus rolling prerelease latest TEST, no official release upgrade before in-game validation.


## V0.68 (10 October 2026): TPS input simplification + rebindable L3 recenter TEST

User feedback on V0.67 (supplied `DarksidersGenesisMod(20261010-193310).log`): camera now correctly follows actor root real XYZ; controller native right stick is suppressed except while shooting because `TPSControllerCombatAim` **injects artificial right-stick axes during RT/RB**, undoing suppression. User explicitly rejects Ignore Zone Camera Changes option, fixed center drawn crosshair, Keep Player Facing While Strafing, Native Strafe While Shooting, and other obsolete facing-cone/pivot/aim calibration. Mouse aim remains projected onto **flat native top-down world plane**, so a shot at a wall can go to vanilla mouse reticle location rather than TPS camera forward. This requires a native shot target/trace direction detour not available in current hooks. Do not describe firing as fixed by recentering the UI or hiding a cursor.

V0.68 implements:
- Removed TPSControllerCombatAim synthetic native right stick on ALL RT/RB. XInput right stick filtering now **always** clears right-axis data on controller #1 during active TPS even when shooting, while our orbit polls original raw XInput through existing trampoline. Native LB wheel and co-op #2 continue vanilla. Mouse and right stick camera orbit remain unchanged. **Shots still use native game targeting** and are NOT forced onto camera forward; native projectile target hook remains TOP PRIORITY.
- Retired V0.65 fixed ImGui central crosshair completely. Restore lazy ImGui only when Insert opens, no extra D3D overlay render frame on TPS gameplay. Hide OS/UI cursor automatically during valid active TPS gameplay while preserving F4 Hide Reticle for vanilla outside TPS; do not draw any replacement reticle. Actual native HUD reticle element may still require further removal research.
- Removed all user-facing obsolete settings and cleaned them on INI SaveNow: `TPSFixedCenterReticle`, `TPSLockZoneCamera` (its switch only, player-camera reference stays naturally stable), `TPSControllerCombatAim`, `TPSLockCombatFacing`, `TPSStrafeLock`, `TPSCameraFacingGuard`, `TPSLockCombatCameraPivot`, `TPSCombatAimTurnRate`, `TPSAimYawOffsetDegrees`, `TPSFacingToleranceDegrees`. Legacy native facing-bit override now **only restores** any owned bit on a live validated player, no longer forces "strafe". No injected aim/CTRL. FOV and stable TPS camera pose preserved.
- Rebindable controller button via `[Controls] TPSRecenterButtonMask=64` (default `XINPUT_GAMEPAD_LEFT_THUMB`, L3). Overlay camera control Combo offers L3, R3, LB, RB, A and D-pad directions. On a rising edge from unmodified controller #1, `camera_trace::RecenterOnPlayer()` computes `orbitYaw = currentActorYaw - capturedBaseYaw - manualCameraYaw`, resets orbitPitch to 0, without moving the player, modifying game camera components, or changing TPS distance. Requires a fresh player transform and camera baseline. Button gameplay function remains native (the rebind may overlap gameplay action). Supports hot-rebinding by overlay; changes persisted to INI.
- [Meta] `ConfigRevision=2115`: migrate saved INI safely, delete retired keys and preserve existing player settings/hotkeys; no other validated mod features changed.

Test: TPS ON (F5), orbit with right stick while RT/RB firing and idle, verify character is never turned by right stick; L3 click recenter behind current facing, verify rebind via Insert; game remains functional in LB wheel and no forced strafing; vanilla on TPS OFF. Mouse shot-world-plane issue intentionally tracked and uncorrected; log to identify native aim computation separately. Preserve official release V0.29, build V0.68 as TEST, archive with 4 root files, rolling test prerelease + permanent versioned ZIP.


## V0.69 (10 October 2026) TEST: mouse passthrough and controller fire gate ONLY

User report V0.68: when `Block vanilla right-stick player rotation` is disabled, holding RT/RB should **stop the mod's gamepad camera orbit** so the native game can use its right stick without both systems rotating; when block is enabled, user reports inconsistent results and requests a fix. `Isolate native flat mouse aiming movement` appears broken and player fires at arbitrary locations. Preserve all other features unchanged.

**Implemented only these two input fixes:**
- Disabled + fire RT or RB (controller #1): suppress OUR **right-stick orbit input only** for the duration of fire, using the unfiltered XInput state already read by `UpdateOrbitInput`. Native right stick passes through as before (the original XInput callback never clears it when the checkbox is OFF). Mouse orbit and other controls are unchanged.
- Enabled: unchanged semantic behavior of isolating native right stick while our mod camera still reads original unfiltered right stick, *also during RT/RB* (V0.68 already had this intent). Added independent logging `native right-stick filtered DURING FIRE`, `rightStickReadsFiltered`, `whileFiring` counts to verify the XInput route actually reached the native filter; if user still observes character rotation, investigate alternate Steam Input/HID/native aim paths rather than assume success.
- Removed the misleading `Isolate native flat mouse aiming movement` checkbox, its default/INI/runtime setting and the suppression of WM_MOUSEMOVE/WM_INPUT in TPS. The old interception moved the mod camera but froze the game's flat-plane world aim target. New behavior observes raw mouse deltas for TPS camera orbit while forwarding all native mouse events normally, returning to vanilla targeting (which may still differ from TPS camera-forward ray). The obsolete `TPSSuppressVanillaMouseAim` key is purged from existing user INIs on migration. **Native 3D projectile targeting remains unresolved.**

Version `0.69-tps-shoot-stick-gate-native-mouse`, config revision 2116. No modifications to camera attachment/geometry, L3 recenter, FOV, movement speed, strafing, combat hooks, horse, HUD toggle, other features, or official validated release. Build is **TEST**, not user validated. Reproduce with RT/RB and Block ON then OFF, check resulting game character rotation, 3D camera position and shot direction, native mouse ability to aim, and collect fresh log to diagnose alternative input if ON still fails.


## V0.70 (10 October 2026) TEST: independent TPS camera devices, horse pose, right-stick investigation

User's V0.69 log (22:01:48-22:04:14) shows valid foot TPS camera transform, two successful L3 recenter presses, and XInput right stick **filtered 3,741 reads, 1,136 of them during shooting**. Yet the user reports native character rotation still happens. This proves filtering is invoked on at least one controller API, NOT that all game/Steam Input/UE paths obey it. Do **not** claim it solved until game testing. There is no forced native actor yaw write. Instrument an on-foot right-stick-only controlled yaw probe when left stick is idle and the right-stick filter is on; emit limited diagnostic only if actor yaw changes >8 degrees in 650 ms. The follow-up log distinguishes an alternative rotation path from a nonfunctional filter and preserves co-op inputs.

**Independent TPS camera input controls:** previous `Control camera with mouse / controller right stick` becomes two unrelated overlay toggles:
- `Control camera with mouse` -> [Features] CameraMouseOrbit=1;
- `Control camera with controller right stick` -> [Features] CameraControllerOrbit=1.
Each retains its own speed slider and can be independently disabled. The old combined `CameraMouseGamepad` key is read as migration default for both and removed on SaveNow; the runtime legacy `cameraOrbitInputEnabled` remains derived as logical OR only for camera orbit/recenter math. The separate **Block vanilla right-stick player rotation** option is preserved unchanged, including the V0.69 shooting gate. If both are disabled the camera stays at the last orbit angle, with existing TPS follow still available.

**Horse TPS:** The prior V0.67-0.69 synthetic TPS rejects camera when the on-foot `UMayhemCharacterMovementComponent::GetMaxSpeed` callback stops on mount. The log clearly shows `HorseFeature: HORSE MOVEMENT READY` at 22:02:26 followed by `TPS camera REJECT actorValid=0 fresh=0` at 22:02:26. V0.70 adds a read-only `ACharacter::RootComponent` world pose sample `Actor+0x158` -> component translation `+0x1A0,+0x1A4,+0x1A8`, yaw `+0x1F4` inside the **already working live native HorseCharacter callbacks**, never dereferences a saved horse UObject from Present or camera. First accept only if horse is within 2800 XY and 1100 Z of a recently observed local player (<15 s); subsequent updates only for same live horse pointer seen within 2 s. Camera selects horse pose only when on-foot callback is stale (>300 ms), horse pose is fresh (<750 ms), camera proximity and native pitch checks pass, and it returns to foot pose when player callback resumes. Logs `LIVE horse camera pose` and `camera pose source HORSE/FOOT`. Uses same existing TPS distance, height, FOV, recenter, angle, no mount-speed change. This is **experimental**: horse ACharacter root offsets and callback cadence to be verified by log; unusual cinematics still use proximity/pitch heuristic. Do not use cached horse pointers for writes.

Other features strictly unchanged: horse stamina/sprint speed code, camera positioning on foot, existing FOV, HUD, L3 bindings, damage, intro, etc. Revision 2117; deliver Windows build V0.70 TEST through fixed rolling prerelease and versioned backup ZIP, files directly at archive root. Official validated release unchanged.


## V0.71 (10 October 2026) TEST: independent TPS strafe mouse/keyboard vs controller

Requested after V0.70: re-enable experimental TPS strafing with **two separate options**, one for mouse/keyboard and one for controller. Do not affect validated TPS camera or horse following, independently toggleable orbit controls, L3 recenter, XInput right-stick filtering or input during shooting.

[Features] `TPSMouseKeyboardStrafe=0` (off by default): only when Third Person is active, exact supported EXE, foreground game, overlay closed, native on-foot **Walking / NavWalking** movement mode and a sampled WASD or arrow direction key is pressed, suppress native `UCharacterMovementComponent.bOrientRotationToMovement` (byte `movement+0x240`, mask `0x10`) through existing validated local-player GetMaxSpeed callback. This attempts to retain facing while moving sideways/backward, with normal keys untouched. Mouse rotation and camera orbit remain independently controlled.

[Features] `TPSControllerStrafe=0` (off by default): identical native flag action when controller #1 left analog magnitude exceeds native XInput deadzone, sampled through original unfiltered XInput API in Present. Controller right-stick camera and its native suppression remain unchanged, and LB radial menu is excluded.

These options are **experimental**, because V0.66's older global strafe experiment failed user gameplay validation; no forced actor yaw/rotation write, no virtual CTRL/aim, no input remapping. Both start OFF to preserve tested V0.70 movement. Per-frame input source samples are atomic, stale after 160ms, and only handled in the *live, exact local player movement callback*, never by dereferencing cached UObjects. The mod only clears the native orientation flag if it was ON; on stop, lost focus, TPS OFF, non-walking movement, or toggle OFF it restores only the bit it owns in the next valid movement callback. When a movement component pointer changes, no stale pointer dereference is attempted. Bounded logging: `TPS V0.71: strafe native lock`, `RESTORE`, totals `keyboardLocks`, `padLocks`, `nativeRestores`. This is not proof of effective side-stepping until in-game test.

ConfigRevision 2118. Split and persist toggles via overlay; no changes to FOV, camera, horse sprint/stamina, player speed, HUD, other valid mod features. Official release unchanged. Windows x64 test build with ZIP root-level 4 files, rolling TEST prerelease and permanent numbered ZIP.


## V0.72 (10 October 2026): V0.71 controller strafe regression rollback + narrowly scoped synthetic mouse hypothesis TEST

User feedback on V0.71: "grosse regression strafe ne marche pas pour la manette et loption pour desactiver la rotation ne marche plus". Latest supplied log at 22:41:28–22:43:41 corroborates:
- `TPS V0.71: strafe native lock n=1 keyboard=0 pad=1 mode=1`, restored n=1 at end; **1607 active callbacks, padLocks=1, keyboardLocks=0**. The one native `UCharacterMovementComponent.bOrientRotationToMovement` bit intervention does NOT produce true strafing. Clearing it is insufficient to govern Strife yaw; don't describe it as a fix.
- Right-stick XInput filters fired **811 reads, including 143 during RT/RB**, yet native player yaw changed `88.9°, 130.3°, 73.2°` with LEFT stick idle and RIGHT active. This conclusively indicates a residual rotation path, not a missing XInput filter. Could be UE controller/aim rotation or software mapping gamepad stick to Windows mouse; the log alone cannot distinguish. No unverified actor rotation setters.

**Safe regression recovery:** restore V0.70 `UpdateTPSFacingNative` restore-only implementation, remove V0.71 input-source sampling and all newly added on-foot native flag clearing; leave known camera, mounting, FOV, controller/gamepad orbit splitting, L3, game features untouched. Previously requested individual `TPSMouseKeyboardStrafe` / `TPSControllerStrafe` options remain visible but disabled in overlay and force OFF on ConfigRevision 2119 migration. They are NOT functional until a verified native player yaw/aim hook is identified. Avoid silently retaining a misleading functioning toggle.

**Targeted experimental alternate input test:** preserve V0.69 genuine native mouse passthrough. While TPS + foreground + overlay CLOSED + `TPSSuppressNativeRightStick=1` and RAW controller right stick moved within 130 ms, block *only* WM_MOUSEMOVE game window messages when no actual RAW physical mouse input has been seen in the previous 130 ms. This may catch Steam Input virtual mouse mapping in addition to filtered XInput. **Never** block WM_INPUT from the real mouse, never block global mouse messages, never change XInput left stick/triggers/buttons or native ability wheel. Log the exact count `TPS V0.72: controller-associated WM_MOUSEMOVE blocked` and totals. If residual yaw remains with count zero or nonzero, prioritize auditing native Unreal rotation authority rather than changing sensitivity or unrelated camera offsets. Physical mouse movement concurrent with controller right stick may still be briefly suppressed, so treat as opt-in-equivalent experimental behavior gated by existing right-stick-block checkbox, not stable release.

V0.72 is a TEST only, no in-game verification, and does not promise verified strafing or full native right-stick rotation disable. ZIP at root includes loader, ASI, INI, README, versioned download and rolling TEST release; official validated release unchanged.

## V0.73 (10 October 2026) TEST: native character movement rotation path (NOT VALIDATED)

User supplied exact retail executable `DarksidersGenesis-Win64-Shipping(2).exe` (62,113,280 bytes, SHA-256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`) and the V0.72 log `DarksidersGenesisMod(20261010-210017).log`. V0.72 filtered 4,848 original controller right-stick samples (1,975 during fire), but **no** controller-associated `WM_MOUSEMOVE` messages, disproving the specific mouse-suppression route in that run. The older rotation-flag-only V0.71 is rejected; do NOT reintroduce it or rotate XInput left stick (rejected V0.59).

**New native disassembly**: the game's movement-rotation block at `RVA 0x5B96BF` calls UE4's FRotator interpolator at `RVA 0x8A1700`, passing native current desired rotation at `MovementComponent+0x22C` and a transient target rotator. The native result is written back into the movement component at `RVA 0x5B96C4..0x5B96D3`. The preceding block chooses game-native look vs movement input (`RVA 0x671A40` / `0x671C10`). This is a DIFFERENT authority from the rejected `bOrientRotationToMovement` bit.

V0.73 hooks only the validated shared interpolation primitive, and changes target yaw **only when invoked from this exact game movement callsite**, the current rotation pointer corresponds to the same fresh validated local-player movement component, TPS gameplay view is fresh, window focused, overlay closed, and `TPSControllerStrafe=1` was enabled explicitly in the overlay. Target yaw = the current actual **rendered TPS camera yaw**, while native interp, movement speed, left/right analog bytes, native button inputs and every unrelated call remain original. No permanent native movement flag or fake Ctrl/aim state. Optional Controller Strafe stays OFF by default. Keyboard strafe remains disabled until independently analyzed. If the engine applies another later yaw authority, this scoped interception will not be enough; do not claim full strafe until tested in-game.

**Required test**: TPS ON F5, enable `Controller strafe: face TPS camera while moving (TEST)` in Insert overlay; with right stick held rotating the camera, walk LEFT/RIGHT/BACK with the left stick, stop and repeat during RT/RB. Confirm forward/back movement remains correct, animations/dodge/ability wheel/sprint intact, camera orbit and horse normal, then disable option and TPS OFF to verify exact vanilla behavior. Log should show `TPS V0.73: native physics desired yaw override` and final `nativePhysicsCalls/yawOverrides`. A counter confirms interception ONLY, not functional validation. Official stable Release unchanged. Four root-level ZIP files, experimental branch archival build, log overwritten each boot.


## V0.74 (11 October 2026): four independent while-firing TPS controls (EXPERIMENTAL)

User rejected the two V0.73 TPS strafe controls. Supplied V0.73 log confirms exact EXE and successful native hook installation but **only 7 matching physics callbacks and 0 yaw overrides**, so no claim that player strafe worked.

Replace retired `TPSMouseKeyboardStrafe` and `TPSControllerStrafe` with four disabled-by-default `[Features]` keys: `TPSMouseStrafeWhileFiring`, `TPSMouseRotateWhileFiring`, `TPSControllerStrafeWhileFiring`, `TPSControllerRotateWhileFiring`. Old keys are removed from INIs during save; config revision 2120, independent checkboxes under **Camera / TPS camera controls**, labeled Mouse/keyboard and Controller/camera stick.

- **Strafe while firing:** test holding the native character facing recorded when the corresponding source starts firing, without rotating movement inputs. Left-stick/WASD remain vanilla.
- **Rotate player while firing:** test setting native desired physics yaw to the current actual TPS camera yaw while the corresponding source is firing. When both enabled for one source, Rotate takes precedence; controller wins if both sources are firing simultaneously.
- Mouse shooting: primary mouse button held. Controller shooting: RT or RB as observed in the original game's live XInput reads, with a 180ms freshness check. Only local validated on-foot movement component, exact supported EXE, focus, menu closed, fresh rendered TPS view, native interpolator callsite `0x5B96BF`; no flags, direct actor writes, fake aim key, or analog axis transformation.
- The UE native physics rotation candidate remains unvalidated: **the V0.73 log showed only 7 relevant calls**. V0.74 adds first-ten call-path diagnostics `TPS V0.74: physics trace` and explicit firing/movement/fresh-camera/overrides totals to tell whether the source really participates during combat. A zero override is a failed hypothesis, NOT a fixed feature.

Everything outside this four-control path and its INI/UI/log plumbing is intentionally untouched. This is a separate branch TEST ZIP, four files at ZIP root, no automatic official release promotion. Test TPS shooting with mouse and controller separately, lateral and backward movement while firing, camera orbit and co-op safety; compare screenshot and log.

## V0.75 (11 October 2026) TEST: native player look-input path, right-stick rotation block and firing camera facing

User feedback on V0.74: Block vanilla right stick player rotation regressed, strafe while shooting failed. Uploaded log **proves native physics interpolator had zero invocations during entire session** (`physics=0, yawOverrides=0`), although the original XInput read filter was active `rightStickReadsFiltered=5791, whileFiring=1013` and player's yaw changed 99.7 degrees under right stick only. This means the earlier `RInterp` hook is not a workable control surface for this session. Do not treat a successful installation as a functional fix.

**New exact EXE disassembly:** `UMayhemCharacter::GetPlayerLookInput` native method at **RVA 0x671A40** reads game character +0xD24/+0xD28 2D world look vector, converts to native vector output. `GetPlayerLookRotation` at **RVA 0x671B00** calls the look-input getter, converts into a rotation and falls back to root native rotation if look input zero. Both are real C++ native methods; verified separate concrete 8/10-byte entry signatures from the exact known 62,113,280-byte executable, SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. Hooks are only active for freshest locally controlled on-foot player identity, valid gameplay TPS, game focus and closed menu, and all tests are opt-in except requested mouse firing and existing native right-stick block toggle.

**Controls:** remove V0.74's `TPSMouseStrafeWhileFiring` and `TPSMouseRotateWhileFiring` UI, config and runtime fields. Mouse/keyboard automatically sets the game's **look return vector** to the currently rendered TPS world-camera heading `(cos yaw, sin yaw, 0)` *while left mouse button is held*; native look-rotation return follows the camera as well. This does not transform WASD.

Keep the existing **Block vanilla right-stick player rotation** option, ON by default. In addition to existing live XInput right-axis filtering, the new native look-input hook zeroes the game's look-vector return when a live unfiltered right-stick movement was detected and physical RAWINPUT mouse has not recently been observed; look-rotation getter holds most recent validated actor yaw in the same condition. This is a **hypothesis about a secondary controller/engine path**, not a validated elimination of every yaw writer.

Controller gets only two independent opt-in settings: **Strafe while firing (player movement)** (`TPSControllerStrafeWhileFiring`) and **Rotate while firing (camera movement)** (`TPSControllerRotateWhileFiring`). Strafe mode drives *native look direction only* to TPS camera world forward during RT/RB, leaves left stick player movement untouched. Rotate mode independently enables the mod's raw-XInput TPS camera orbit during RT/RB; when that option is OFF, right-stick TPS camera orbit is suspended during firing, whether or not the vanilla right-stick PLAYER block is enabled. The player block and camera-orbit option are separate switches. Both remain OFF by default. Game-native behavior outside TPS, during mount, when window unfocused and when overlay open is preserved.

A simple in-game test is mandatory to determine if native look callbacks are actually used: log `TPS V0.75: native game look input READY`, `native gamepad look input neutralized`, `game-native LOOK VECTOR -> camera during fire` and final `nativeLookInput/nativeLookRotation/suppressedLook/mouseLookAim/padLookAim` counts. Check controller idle/right-stick rotation, camera orbit during RT/RB, true sideways/backward movement while firing, and mouse firing facing direction. If counts are zero, do not claim a fix; revisit game controller rotation authority. No other features modified. Build archive has exactly 4 files at ZIP root, experimental branch, no automatic official release promotion.


## V0.76 (11 October 2026) Experimental controller hold controls and camera defaults

**V0.75 code is frozen as a user-accepted baseline**: see [V0.76 freeze/changelog](docs/V0_76_CHANGELOG_AND_FREEZE.md). This update only adds control integration and camera options around the validated V0.75 look hooks. Tested Windows x64 build required; **no feature is declared user-validated until an in-game test**.

**New optional controller functions enabled by default** (TPS context, local controller #0):
- Hold **R3** for a temporary vanilla camera view, resume TPS on release, without changing the persistent F5 TPS state. Rebind in overlay (`TPSSuspendButtonMask`).
- Long press **L3** (650ms) to toggle TPS ON/OFF, independently rebindable (`TPSToggleHoldButtonMask`, `TPSToggleHoldMs`); short L3 still recenters if using the same recenter binding.
- Hold **R3 grenade**: translate physical press and release into two distinct native R3 key pulses to equip during hold and stow on release. This is a pulse-based *experimental* recreation of game's toggle, not a direct equipment state patch; test R3 and character switching carefully.
- Hide native **ground-targeting aim FX** in TPS, signature checked old RVA `0x6DDAD0`, not guaranteed to hide every gun aim effect.
- Stabilize native **horse pose camera** source during shooting once the mounted camera source has been positively sampled, to reduce aiming-related camera jumps; not a complete horse-specific firing fix.

**Camera defaults based on user-provided V0.75 INI:** TPS FOV 110, distance 0.200, pitch -12°, vertical offset +300, foot origin 0, global yaw +5°, global height -300, controller stick speed 180°/s, mouse sensitivity 0.12. Existing user INI camera values are preserved. New installs retain Third Person OFF until F5/L3 because activating TPS automatically was not requested. Camera vertical offset now -1000..+1000, foot origin correction -500..+500. User provided the same -1000 value for both vertical bounds; treating the upper one as +1000 prevents a non-adjustable slider. `Rotate on foot while firing` is labeled to reflect the user's observed forward shooting; the actual V0.75 native look-hook behavior has not been changed.

**Known issue:** sporadic second-character throwable weapon flash needs a separate native event trace to identify cause, not a blind manipulation of inventory/animations. Ground-targeting FX hook may not hide all aiming visuals. Stable V0.75 ZIP and release unaffected. V0.76 ZIP contains four files at archive root.


## V0.77 (11 October 2026) | validated V0.76 preserved

The user validated all V0.76 controls, with one remaining visual issue: the native `HideGroundTargetingEffect` checkbox does not hide the on-screen aiming effect. V0.77 preserves the V0.75/V0.76 movement, right-stick block, firing look, grenade, long L3, mounted camera, HUD and other validated systems.

- `[Controls] TPSSuspendDelayMs=300` is a new configurable **0–3000ms** start delay for the physical R3 temporary vanilla-view hold. Short R3 presses under threshold do NOT swap the camera; holding beyond the delay temporarily shows vanilla, releasing immediately returns to TPS. It is independent of L3 long-press.
- **New-install and Reset Defaults values** now mirror the user's Oct 11 V0.76 INI: TPS on, controller firing Strafe/Rotate both on, TPS FOV 110, distance .20, pitch -12°, TPS vertical offset +120, foot 0, global yaw 0, global height 0, mouse sensitivity .12, controller speed 180, L3 toggle hold 500ms. Other features stay as previously validated and **existing per-user settings are never overwritten by migration**.
- **Aim effect diagnostic:** legacy native hide-ground-targeting hook is actually installed, but V0.76 had `groundAimHidden=0`. V0.77 counts *all* native calls, already-hidden calls, forced hides separately to verify whether the specific native ability participates. Fix obsolete INI SaveNow line that incorrectly removed `TPSHideGroundAimFx` after saving. Not yet a proven removal of Strife's normal aiming visual; do NOT modify validated camera, aim or combat hooks on an unverified hypothesis.

See [V0.77 scoped changelog and native aiming-FX audit](docs/V0_77_HOLD_DELAY_AND_AIM_FX_AUDIT.md) and [V0.76 protected baseline](docs/V0_76_CHANGELOG_AND_FREEZE.md). V0.77 test ZIP has four files at ZIP root; no promotion to the official stable release pending user tests.


## V0.78 (2026-10-11) | TPS OFF default + confirmed Strife aiming effect target

**User clarification (video):** the unwanted turquoise aiming visual comprises **both** (1) the straight laser-like world-space line originating near Strife's dual pistols and (2) the cyan semicircular arc near the character. These are **not** the cursor, the HUD reticle or the grenade ground-targeting effect. The V0.77 shutdown log confirms `groundFxCalls=0`, so the previously installed native `HideGroundTargetingEffect` hook is not used for the requested blue visuals. Actual asset/effect producer remains unresolved. No undocumented risky render/combat detours have been added; the old UI checkbox must not be represented as a proven fix for these visuals.

**Requested default:** `ThirdPerson=0` in the new-install INI; matching config Reset Defaults / read fallback OFF. An existing user's persisted `ThirdPerson=1` is left alone. Enable TPS with F5 or long L3. All V0.77 values and validated behavior are retained. [Detailed video and executable audit](docs/V0_78_STRIFE_AIM_LINE_ARC_AUDIT.md).

**Status:** safe packaging/default correction, aiming line + arc visual hide still under investigation. Tested ZIP is a standalone development build and not promoted to stable without user acceptance.


## V0.83 (2026-10-11) | Clear overlay and live status, horse summoning research

User evidence: **Hide HUD does not remove Strife's cyan aiming line and semicircle**. They are outside the global HUD-hide operation. Do not modify Hide HUD just to target them.

The mod overlay now explains each gameplay control plainly. Real observed events are shown for movement recovery, outgoing combat and Hotstreak gains, horse callbacks, plus camera frames/attachments; low-level offsets, pointers and EXE hashes move under optional Advanced sections. Status labels distinguish "hook ready" from activity seen during actual gameplay.

**Horse Everywhere (experimental research)** is available as an ON/OFF choice and as `[Features] HorseEverywhere=0` (default OFF). Native server-spawn-request diagnostics are signature-guarded and pass-through. **The restriction bypass is NOT functional yet**, and the overlay states so: exact no-summon-zone condition hasn't been identified; forcing an unverified spawn path would risk game breakage. Try a summon in an allowed area and a restricted area and send the log; compare `Horse Everywhere V0.83 probe totals`. The user-validated horse speed and sprint code is unchanged.

This branch descends from protected **V0.78**, not the failed V0.80 beam-hide or unvalidated V0.82 UI gating tests. All existing TPS, controller, horse stamina, combat, startup, cursor and HUD code preserved. See [V0.83 technical notes](docs/V0_83_HORSE_EVERYWHERE_RESEARCH_AND_UI.md).

## V0.84 (2026-10-11) | HUD blue aim-indicator modes and R3 grenade hold repair

- New **Gameplay > HUD and aiming effects** selector: `Hide Everywhere` (0), `Default Only` (1), `TPS Only` (2, **default**), stored as `[Features] AimIndicatorHideMode=2`. The old experimental checkbox is removed from Camera; all other camera controls stay unchanged. **The actual turquoise world-space line and semicircle are not yet confirmed hidden**: the currently available exact native FX hook had zero calls in users' Strife logs, and a correct shader/component is still needed. The UI reports real observed FX-hook calls rather than falsely implying a successful visual fix.
- **R3 grenade long hold fix candidate**: native game-facing R3 now stays pressed for the entire physical hold (previous test sent only a 65ms pulse). A separate 200ms stow-toggle press follows a 100ms release gap. All other controller bits, camera rotation, L3 and firing callbacks are unchanged. New `TPS V0.84 GRENADE` logs report physical transitions and forwarded stow pulses; equip/stow still requires user gameplay testing.
- **Horse Everywhere status unchanged: NOT YET FUNCTIONAL.** The previous log confirms `serverSpawn=0`; the previously added native server probe cannot bypass a zone check upstream of it. Horse options default OFF for Everywhere and previous horse movement/stamina hooks stay intact. We need to locate the real game/Blueprint summon permission before enabling a working bypass.
- Based on V0.83 with prior user-validated gameplay kept; main untouched. See [V0.84 technical audit and test](docs/V0_84_AIM_MODES_R3_GRENADE_HORSE_AUDIT.md).

## V0.85 (2026-10-11) | Mounted TPS camera stabilization while firing (TEST)

The user confirmed **V0.84 R3 grenade equip/stow is fixed and WORKING**, so the entire grenade/XInput, L3/R3, native firing and combat code is frozen unchanged. User reports *Stabilize mounted TPS camera while firing* previously produced no apparent difference.

Code audit confirmed that the old switch was a narrow foot-pose/horse-pose priority latch, **not an actual camera smoothing feature**. When horse pose was already naturally selected, toggling it made no visual difference. It could also invalidate an otherwise usable foot camera frame when the horse sample was stale. The user's log repeatedly shows `camera pose source = HORSE/FOOT` and live mount callbacks, but had no stabilization-specific counters.

V0.85 makes the checkbox observable and useful during actual mounted shooting: when the game reports active fire and a real fresh horse pose is selected, it applies **150ms vertical-only camera pivot smoothing**, retains fresh horse pose against transient foot samples, and preserves the foot pose when the horse sample is stale. All XY tracking, camera orbit, weapon aim, controller state, mount movement and damage remain native/unchanged. The checkbox OFF path is unsmoothed. Its overlay now reports real requested/selected/smoothed frame counts and last vertical correction. **User gameplay validation is still required.**

[Technical scope and test procedure](docs/V0_85_MOUNTED_CAMERA_FIRING_STABILIZATION_AUDIT.md). Build on V0.84; do not promote the old V0.82 targeting UI suppression nor claim the blue aim line/arc is fixed.

## V0.86 (2026-10-11) | Two mounted TPS shooting modes, compact full gamepad binding

Mounted TPS aim has two exclusive modes that override on-foot rotation/strafe/camera-orbit switches:
- `TPSMountedAimMode=0` (default): **Camera Forward**. You can control the TPS camera while mounted and firing; the game's native look getters receive camera-facing aim. Camera orbit remains active even if on-foot orbit switches are off.
- `TPSMountedAimMode=1`: **Vanilla Aim**. While firing on a recognized horse, TPS camera orbit pauses and the native game handles right-stick/look input. Camera control returns when you stop shooting. This is an experimental game-native aim path, still requires actual gameplay verification.

The V0.85 mount height smoothing is removed (it was not the requested mounted camera behavior). The native mount/source pose remains guarded by validated callback freshness.

TPS controller hotkey UI has **one button selector per action**, each with **OFF**: hold/release grenade (now rebindable), temporary vanilla view, long press to toggle TPS and recenter. Supports every standard XInput gameplay button plus LT/RT. Defaults R3/R3/L3/L3 kept and V0.84 validated grenade hold/stow timing unchanged. Renumbered `ConfigRevision=2126` and existing custom configurations migrate, disabled legacy shortcuts become OFF. Choosing a trigger or a button already used by the game may override its native function; **the Xbox Guide button is not available via standard XInput**.

The user identified native horse summon controls as keyboard **H** or gamepad **LB+RB**. V0.86 passively logs these user inputs against native `SERVER_SpawnHorseMount` call counts. **Horse Everywhere still does NOT bypass forbidden-area restrictions.** The log should clarify whether the request is stopped upstream.

[Technical V0.86 notes](docs/V0_86_MOUNTED_AIM_AND_GAMEPAD_BINDINGS.md). GitHub main remains untouched; V0.84 validated grenade baseline preserved.

## V0.87 (2026-10-11) | Mounted TPS aiming consistency / native Ctrl experiment

**Vanilla Aim:** the validated firing behavior is unchanged: while mounted and shooting the game keeps native aim and the TPS camera orbit is paused. When **not firing**, the right stick now remains camera-only, not simultaneously rotating the game aim. Exact game test is required.

**Camera Forward:** while mounted and firing, the mod now experiments with initiating the game's native **Ctrl aim action** by posting a matched Ctrl down/up keyboard event to the **game window only**, scoped to mounted TPS fire. No system-wide SendInput or persistent keyboard state changes. The existing native look-vector-to-camera system remains. **Experimental:** game engine might ignore posted keys, so judge the shot direction in gameplay; diagnostics `TPS V0.87 CTRL AIM` prove only message submission.

**Unresolved intentionally:** keyboard/mouse TPS camera can sometimes lock against angles and refuse a full 360-degree turn. This is recorded but left completely untouched until mounted/controller functionality is coherent.

**Blue line + arc:** a new **pass-through only** Unreal targeting UI-component event probe checks `UMayhemTargetingUIComponent` and `UMayhemTargetUiPositionComponent`, with event counters in the HUD overlay and shutdown log. It blocks nothing. The blue line and semicircle are **not yet proven hidden**; the previous ground-FX hook is confirmed 0 calls.

**Horse Everywhere:** native H / LB+RB summon input now logs before and 1800 ms after, together with server spawn delta and **live horse callbacks captured using the existing validated HorseFeature**. This is diagnostic only, not a bypass of restricted areas.

The validated R3 grenade hold/release, customizable bindings, horse speed/stamina, visual HUD, TPS orbit-angle code and all unrelated feature hooks are preserved. Do not promote to stable release until gameplay testing. Technical details: [V0.87 audit](docs/V0_87_MOUNTED_CTRL_AIM_VISUAL_HORSE_AUDIT.md).

## V0.88 (2026-10-11) | Horse Everywhere ON by default and separate area-ban diagnostics

**Frozen / validated:** Mounted TPS **Vanilla Aim** now works to the user's satisfaction. **Do not touch** the native aim/camera behavior, R3 grenade hold/stow or gamepad bindings.

**Camera Forward still experimental and NOT FIXED.** V0.87 posted eight Ctrl key down/up pairs but the player reports bullets continue along a cached/fixed original character direction. V0.88 removes these ineffective synthetic key messages, preserves existing camera controls and logs the native look-vector yaw compared with TPS camera yaw during mounted shots. This is not a projectile trajectory measurement. Independent mod *Klassik 3rd person cam* (Nexus mod #2, author aaaaabbbbccc) claims working **camera-facing bullet raycasts**; this is a focused lead for auditing the actual projectile trace in the exact supported EXE, not an imported or validated patch.

**Horse Everywhere defaults ON** in new packaged INI, reset defaults, fallback and the first migration to ConfigRevision 2127. `[Features] HorseEverywhere=1`. An explicit OFF saved afterwards remains OFF. **No forbidden-zone bypass exists yet:** current ON state only authorizes research/telemetry. Two different restriction mechanisms are now tracked separately: (1) inputs which cannot summon, and (2) *banned mounted volumes* that remove an already summoned horse. Uses existing native HorseCharacter callback timestamps/identity, live mounted-camera recognition, real H/LB+RB input timing, server spawn deltas, and HORSE -> FOOT transitions to build falsifiable logs without writes or forced spawns.

New log tags `Horse Everywhere V0.88 AREA INPUT`, `AREA RESULT`, `MOUNT EXIT` and `TPS V0.88 FORWARD RAYCAST AUDIT`. Previous target UI and native ground-FX probes both saw zero events; blue line and arc remain unresolved. Keyboard/mouse TPS 360-degree rotation glitch is recorded and intentionally deferred.

[Complete V0.88 investigation and strict regression freeze](docs/V0_88_HORSE_ZONES_FORWARD_RAYCAST_AUDIT.md). Main branch unchanged.

## V0.89 (2026-10-11) | Scoped native horse-region gate experiment (TEST ONLY)

A separate experimental V0.87 build (not the public V0.88 lineage) revealed verified native `IsInHorseRegion` and `IsInHorseDesummonRegion` methods, with **49,923 / 30,177** observed calls on the user's supported executable. That session had Horse Everywhere OFF and therefore **no override**. User's following genuine V0.88 log showed the server spawn callback can occur BEFORE input logging by ~7 ms; earlier `serverDelta=0` on that successful mount was misleading. A later `HORSE -> FOOT` event with no mount key for 7.9s suggests (does not prove) a *forced-despawn banned region*.

V0.89 keeps **Horse Everywhere default ON** and adds signature-checked native region getter return tests. The original region getters ALWAYS execute first; **only** when the last locally validated player pressed physical **H or LB+RB within 1,600 ms** will the mod return allowed=true or desummonRegion=false. Outside this user-initiated summon window the game receives unchanged results. **This is not yet tested to lift either region restriction**, and it does NOT persistently prevent a horse from disappearing in a horse-banned volume. Set OFF in overlay to restore native behavior immediately.

New telemetry `Horse Everywhere V0.89 REGION/DESUMMON` plus `serverJustBeforeInput` avoids the chronological false-negative and separates region checks, native spawn calls and actual horse identity changes.

**Mounted Vanilla Aim is fully frozen and unchanged**, as are the user-validated R3 grenade timing and all controller/camera/horse stats. Camera Forward remains unresolved and now **only adds a passive log** of whether native `GetPlayerLookInput` sees the live horse actor. The native projectile raycast is not hooked. Blue line/arc and keyboard/mouse 360-degree yaw problem remain deferred.

[Technical V0.89 details](docs/V0_89_SCOPED_HORSE_REGION_TEST.md). Branch `main` is untouched; this is an experimental build, not an official stable release.

 
## V0.90 (2026-10-11) | Strife shader aiming indicators + mounted Horse Everywhere retention (EXPERIMENTAL)
 
**Two user-identified pixel shaders** are now intercepted directly by the ASI through early D3D11 hooks (ShaderToggler bytecode CRC32s `1212955419` and `2294978555`). The existing **Blue aiming line and semicircle** mode selector controls when their matching draw calls are hidden: Hide Everywhere, Default Only or TPS Only (default). No separate ReShade/dxgi.dll is required. The effect is *not yet visually validated*, and other visuals sharing the same shaders could also disappear. Overlay shows matched shaders and suppressed draw count. Native shooting/reticle/HUD targeting remain unchanged.
 
**Horse Everywhere** retains the V0.89 summon-key permission for 1.6 seconds, then *also* maintains the local player's region permission while the live mounted TPS camera, native HorseCharacter callback and matching horse identity remain fresh. It drops back to vanilla instantly when those are not confirmed, on foot or when Horse Everywhere is OFF. This targets the reported zone-ban horse disappearance after approximately 1.6–1.9 seconds; other mission-script horse despawns may still occur. It does not spawn a horse forcibly or touch stale UObject pointers. For safety, live mounted retention currently applies only with TPS active.
 
**Frozen unchanged:** mounted Vanilla Aim (user validated), R3 grenade hold/stow, existing controller bindings, strafe and aiming controls, on-foot TPS, horse speed/sprint/stamina, camera FOV/offsets, HUD/reticle, intro, overlay and INI. Camera Forward true mounted projectile raycast remains an independent open issue. See [V0.90 scoped technical notes](docs/V0_90_SHADER_AIM_HORSE_RETENTION.md). Development remains on `debug`; `main` stable and `archive` historical.


## V0.91 (2026-10-11) | Horse Everywhere with the normal camera (EXPERIMENTAL)

**V0.90 shader visibility CONFIRMED BY USER:** both Strife aiming visual indicators (cyan line and arc) disappear, with no noticeable interference with other scene/HUD elements. The D3D11 CRC32 match and shader draw suppression code and existing Hide Everywhere / Default Only / TPS Only selector are **frozen unchanged** in V0.91. There is no verified safe Unreal CPU-side effect callsite corresponding to these GPU shaders; attempting to guess one would endanger a now working function.

**Horse Everywhere now handles native view as well as TPS:** physical H / LB+RB summon checks are retained, as is the validated V0.90 live-mounted TPS override. A supplementary camera-independent native horse lifetime scope joins the game-local player, a genuine spawn server callback and a freshly created live native HorseCharacter identity. The native callback must remain fresh while riding, and a return to validated on-foot movement disables the retention. No cached UObject dereferences and no game memory writes in the region hook. Normal camera retention is **experimental**, not yet confirmed; separate forced mission despawn logic might still exist.

**Mounted Camera Forward:** remains an outstanding issue and is not claimed fixed. In the latest V0.90 log, its activation is recorded, but the projectile trace hook is absent and the mounted-native look getter audit never receives a sample. The proper correction requires verified native projectile/raycast direction, not shader draw call tracing or another controller-axis injection. Native mounted **Vanilla Aim stays untouched**.

Previous cumulative versions are preserved at `archive/downloads/` (V0.89 and shader-validated V0.90). Future experimental builds are on `debug` only; `main` remains stable. See [V0.91 technical note](docs/V0_91_NATIVE_CAMERA_HORSE_RETENTION.md).


## V0.92 (2026-10-11) | Rebindable keyboard TPS, paired horse pad buttons, mounted Camera Forward input test

**Protected user-validated cumulative V0.91 features**: Horse Everywhere works in banned zones in normal camera AND TPS; user-validated V0.90 GPU shader removal cleanly hides two aiming indicators. No code changes to HorseFeature, ShaderAimFeature or mounted **Vanilla Aim**.

**Rebindable horse summon/dismount**: the two controller buttons are individually selectable in the Horse overlay and must be **held simultaneously**. The defaults remain LB+RB. Select a separate keyboard summon key (default H), and use it with or without TPS. The game still owns spawning/dismounting; the mod translates the remapped inputs back to its existing native controls.

**TPS keyboard shortcuts**: In Camera > TPS/Horse keyboard bindings, customize Horse (H), long-press TPS toggle (OFF), short recenter (OFF), hold temporary native view (OFF) and hold/release grenade/gear (OFF). OFF defaults prevent overriding existing controls. They use the same action and timing settings as their controller equivalents. Keyboard grenade with a gamepad uses the same original native R3 pulse; without a controller the mod tries native E Use Gear, which is not yet verified equivalent to R3 in every situation. Keybinds persist in `[TPSKeyboardHotkeys]` and `[Controls]` in the INI, including `HorsePadButtonA=256` and `HorsePadButtonB=512`.

**Camera Forward (EXPERIMENTAL, not yet user-validated)**: only when firing mounted in TPS Camera Forward mode, the game's native right-stick input is emulated straight UP while the physical right stick continues to move the independent TPS camera. This is a simpler native gamepad aiming attempt, not an actual mounted projectile raycast correction. Vanilla Aim and foot aiming remain unchanged. Switch to validated Vanilla Aim if Camera Forward still shoots off-axis.

Detailed technical notes: [V0.92 rebinds + input](docs/V0_92_REBINDABLE_TPS_HORSE_FORWARD_INPUT.md). Stable `main` untouched; V0.91 backup stored in `archive/downloads`; test builds on `debug`.


## V0.93 (2026-10-11) | TPS controls cleanup, unified Camera Forward / Vanilla Aim and single-tap horse recovery (TEST)

**Protected validated baseline:** V0.91 Horse Everywhere banned-zone permissions (both normal camera and TPS), V0.90 exact two-shader aim indicator removal, mounted Vanilla Aim, horse movement/sprint/stamina, all earlier controller R3 grenade hold/release, HUD, FOV, graphics adapter, skip intros and overlays. No changes to `HorseFeature.cpp` or `ShaderAimFeature.cpp`. `main` remains untouched.

**Horse Gameplay page:** a compact one-line description, native server/region/desummon metrics under a small collapsed information/debug tree. Keyboard Horse summon/dismount (default H) now sits beside two individually selectable simultaneously held controller buttons (default LB + RB) on the same page. Existing keyboard and gamepad bindings persist.

**Banned area first-key ghost toggle (experimental):** if a REAL physical horse summon in a gated region made no server request and no live horse appears after 1.8 seconds, and was not a mounted player's voluntary dismount, issue at most ONE short native H recovery tap. This may avoid the old need to press twice; not guaranteed until tested. It never enters an endless respawn loop.

**TPS aim modes on foot and horseback:**
- `Camera Forward`: the independent TPS camera continues to read raw mouse/stick orbit, and the game receives virtual right-stick straight UP when RT/RB fires. The mounted effect is promising per user test; **on-foot extension is new and experimental**. The analog experiment does not create a virtual XInput device for mouse-only players.
- `Vanilla Aim`: at idle free TPS camera; during firing, independent orbit pauses and the native gamepad and look getters handle aiming. Mounted implementation is user-validated, on-foot use is now experimental.
- Keep existing strafe-while-firing movement behavior and all INI settings. The redundant on-foot camera rotate checkbox is hidden from the UI; one TPS aiming selector replaces the separate confusing choices.

**Keyboard/controller side by side:** Camera > TPS controls groups keyboard rebinding and controller button selection in four rows by action: TPS long toggle, recenter, temporary native camera and grenade equip/stow. Shared press-duration settings are adjacent to their actions. Horse binding lives only under Gameplay. No defaults of old user mappings are overwritten.

The V0.92 cumulative ZIP is preserved in the `archive` branch. Technical detail and caveats in [V0.93 notebook](docs/V0_93_UNIFIED_TPS_AIM_CONTROLS.md).


## V0.94 (2026-10-11) | Native radial wheels and simpler TPS input (EXPERIMENTAL)

**User INI is now the source of all defaults:** TPS ON, distance 0.150, pitch 0, height 100, TPS FOV 110, native camera mouse/stick, camera sensitivity 0.120 and stick speed 180, and all supplied combat, horse, hotkey and intro preferences. `ConfigRevision=2128` preserves existing per-user values while retiring obsolete input toggles. The reference `config/DarksidersGenesisMod.ini` reflects the provided file except for deliberately removed obsolete fields.

**Radial menu priority in TPS:** when LB or a native keyboard equipment/ammunition/ability wheel command (E, G, Tab, Left Alt) is held, the game receives its original right-stick position and the **native player look vector/rotation**; our TPS camera stays active, while independent stick camera orbit yields to radial selection. The wheel also overrides any shared R3/E temporary-vanilla camera shortcut to prevent an unexpected switch back to native view. This is the new test focus, not yet user validated.

**Simplified TPS aim:** previously separate `Block vanilla right stick`, `Strafe while firing` and obsolete `Rotate on foot while firing` settings have been retired as checkboxes, runtime flags and INI fields. Their genuinely required native input isolation and firing look-vector correction are automatic TPS mechanics, except while using native wheels or native Vanilla Aim during firing. They never provided an independent real strafing movement mechanism. The single TPS aim selector still controls both foot and mounted combat: **Camera Forward** emulates right-stick UP while firing with independent free camera; **Vanilla Aim** pauses independent camera while firing and passes native aim through. Left-stick movement remains owned by the game.

**Native keyboard grenade:** the mod's duplicate keyboard grenade shortcut and artificial E release pulse are removed; the game already owns E Use Gear. User-validated native gamepad R3 hold/release pulse is retained. Existing shared E `TemporaryView` binding from the provided INI stays recorded, but cannot take the camera away while native gear wheel E is active. Choose a different key to request temporary vanilla view independently.

**Horse keyboard banned-zone fix:** the old extra H press after 1800ms is gone. A real H event now marks native Horse Everywhere scope before delivery to the game WndProc, while a two-button XInput chord marks that scope before the native game polls LB+RB. Both use the same previously user-validated scoped horse-region bypass; no extra mount command or scripted horse spawn is introduced. **Still needs gameplay test** to establish first-key success.

The shader aim suppression and native HorseFeature code remain unchanged. All work is on experimental `debug`; stable `main` untouched; V0.93 is backed up on `archive`. Full technical notes: [V0.94 radial/Input audit](docs/V0_94_NATIVE_RADIAL_TPS_CLEAN_INPUT.md).


## V0.95 (2026-10-11) | Shared keyboard E, native gamepad wheels, TPS mouse aiming and horse first-press reliability (TEST)

**Restored shared native E + temporary view:** `TemporaryView=E` is deliberately dual-purpose. Holding E invokes the game's normal equipment/gear UI AND, once the configured hold delay has elapsed, enables temporary vanilla view. V0.94 incorrectly blocked this behavior merely because E opened a native wheel; that suppression is removed. The R3 controller shortcut can likewise retain its existing grenade / temporary-view sharing. An unrelated LB radial menu remains protected from accidentally triggering temporary view.

**Right-stick selection in wheels:** the original physical LB XInput snapshot now bypasses all TPS R3 and aiming modifications **before** those modifiers run, retaining native right-stick item selection. The native look getters already pass wheel vectors through; CameraTrace also keeps a previously accepted TPS view while an actual radial wheel is open if the native wheel camera momentarily changes angle, with fresh actor and proximity safety guards. This requires user in-game validation for equipment, ammo and abilities.

**TPS mouse attempt:** experimental `TPSMouseCenterAim=1` (new checkbox in Camera > TPS, reversible) centers the native game mouse target only during physical left-click firing on foot in Camera Forward. Real RAW mouse deltas continue to orbit the independent camera; synthesized center WM_MOUSEMOVE is excluded from mod orbit. Does not touch the validated gamepad path, horse-mounted aiming, Vanilla Aim, or underlying weapon raycast. Disable the option if it introduces cursor drift or worsens targeting; it is not yet validated.

**Horse Everywhere first press:** game-native IsInHorseRegion is now eligible to return allowed for a verified, recently observed local on-foot character even BEFORE the horse input arrives. This addresses earlier region checks that could make the first H/LB+RB press a no-op. The actual V0.91 riding/desummon handling, HorseCharacter lifetime-safe hooks, speeds and stamina remain as before. This pre-grant is experimental; no forced actor spawn or extra synthetic tap.

**Selective horse summon/lock HUD icon:** requested but **NOT yet implemented**; there is no verified individual UMG widget, texture or pixel-shader ID for this on-screen prompt. We will not obscure the global HUD or hide a guessed shader at the cost of user-validated visuals. A separate visual isolation capture is required for safe masking.

**Requested shortcut defaults:** `ToggleTPS=Z` and `RecenterTPS=Z` for both short and long hold. A previous unmodified V/V pair migrates automatically to Z/Z on ConfigRevision 2129; other personalized shortcuts stay intact. Native E and original controller pad bindings preserved. The full V0.94 cumulative ZIP was saved in the `archive` branch before these changes; `main` remains unchanged.

[Full V0.95 engineering notebook](docs/V0_95_WHEELS_SHARED_E_MOUSE_HORSE.md). V0.95 is an experimental `debug` build pending visual/user verification.


## V0.96 (2026-10-12) | Separate native horse restrictions, native mouse firing audit, exact user defaults (TEST)

**Horse Everywhere is still ON by default and retains the user-validated V0.95 behavior when both new checkboxes stay enabled.** `HorseAllowRestrictedSummon=1` selectively enables the native `IsInHorseRegion` allowance for the verified local player, including V0.95 pre-grant. `HorsePreventForcedDesummon=1` independently controls the native `IsInHorseDesummonRegion` override with the previously validated mounted/summon scoped permission. Master `HorseEverywhere=0` disables both. Switches are in **Gameplay > Horse**, giving three test choices (first only, second only, or both). Whether a *padlock icon* corresponds to either native predicate is **not yet demonstrated**; native gating and on-screen UI art should not be conflated. No modification to the two verified V0.90 shaders or any blanket HUD masking.

**Mouse aiming while firing (reversible experiment):** audit of the uploaded V0.95 log found **58** repeated native target cursor moves, plus **784** `GetPlayerLookInput` mouse-fire vector overwrites with TPS camera yaw. These two simultaneous steering sources may fight the game's native mouse-plane aim. With `TPSMouseCenterAim=1` the game now retains its own mouse look-input and look-rotation during on-foot Camera Forward fire, and recentering the native cursor happens **once per new mouse-button press**, not on every Present frame. The camera retains physical RAW mouse orbit. A bounded existing-view exception also prevents an already valid TPS camera from dropping to native merely because a firing frame temporarily fails the historical `nativePitch <= -15` admission gate. `TPSMouseCenterAim=0` restores the earlier TPS yaw-input path for A/B testing. The actual native projectile raycast has NOT been hooked and a complete vertical-aim repair is not claimed.

**Exact new fresh-install defaults copied from the supplied V0.95 INI into both ASI source/runtime initializers and shipped INI:** TPS starts **OFF** (`ThirdPerson=0`), global camera height **50**, TPS distance **0.150**, pitch **-12**, TPS height **0**, TPS FOV **110**, mouse sensitivity **0.120**, stick speed **180**, all previously configured damage/movement/horse speed/stamina, Z/Z shared TPS shortcut, E shared gear + temporary view and controller hotkeys preserved. Two new horse flags are ON. `ConfigRevision=2130` migrates missing settings without overwriting individual existing user choices.

V0.95 is backed up in the `archive` branch; only experimental `debug` changes were made and stable `main` remains intact. [V0.96 native gate and mouse engineering notes](docs/V0_96_HORSE_REGION_SWITCHES_MOUSE_AIM_AUDIT.md).

**Test order:** verify Horse Everywhere BOTH ON (must behave exactly as V0.95); then disable one native restriction at a time and compare where the padlock/no-icon behavior occurs. Test mouse shots with `TPSMouseCenterAim=1` then 0 and capture the per-test log. E shared gear+camera and controller Camera Forward should remain unchanged.


## V0.97 (2026-10-12) | Horse areas, RAW-only mouse TPS, automatic cutscene camera and radial input (TEST)

**Horse Everywhere** is now one four-way dropdown in Gameplay: **Off / In all area / Restricted only / Banned zone only**. Default `In all area` retains exactly the already working V0.95 pregranted region and scoped desummon protection. The targeted modes are *experimental classifications*, not confirmed visual zone types: restricted means the original native desummon-region getter returns FALSE, banned means TRUE. The prior two separately ineffective checkboxes are retired. The locked HUD icon versus the no-icon case is still not mapped to these native predicates. The menu preserves your exact labels while explaining this limitation. `HorseAreaMode=1` in the INI (0..3).

**On-foot TPS mouse restarted:** delete the remaining `SetCursorPos` warps and V0.75/V0.96 synthetic mouse-angle look rewrites from the actual on-foot physical mouse-fire path. Native mouse target/shot is unchanged; independent TPS camera movement uses only real RAW mouse deltas with a rare genuine WM_MOUSEMOVE fallback. Physical left mouse fire **never freezes the TPS mouse camera**, even when the gamepad-oriented Vanilla Aim lock is selected. Mounted/gamepad XInput aim behavior is untouched. Previous `TPSMouseCenterAim=1` is retained for INI compatibility but now means **rebuilt RAW mouse camera enabled**. OFF disables the additional mouse camera orbit, not the native mouse aiming. This is not a native projectile-raycast TPS retargeting fix; vertical 3D shot alignment remains open.

**Automatic cutscene handling:** new default-ON `TPSAutoCutscene=1` option under Camera → TPS. Native GetCameraView actor/pose validity must remain non-gameplay for 500ms after a valid TPS view before we mark an experimental cinematic suspension. While detected, TPS input filtering and orbit pause and the game displays its original camera. TPS immediately resumes when a valid local gameplay view returns, preserving on/off selection and orbit basis. OFF preserves the old V0.96 heuristic; this is camera-shape detection, not authoritative UE Sequencer events.

**Native equipment/ammunition/capacity wheel:** complete original gamepad right-stick X/Y is now forwarded not just on native LB but also on a **physically held R3**, with no simultaneous TPS stick orbit. The validated R3 grenade press-release behavior and shared E native gear / temporary camera remain unchanged. New `TPS V0.97 WHEEL` telemetry helps verify whether the real wheel gets the physical right-stick vector. **The other wheels remain unverified until tested in-game**; do not promote this as a universal repair.

User-selected ASI/INI defaults unchanged: third-person OFF, distance .150, TPS pitch -12, TPS height 0, global camera height 50, FOV 110, mouse .12, stick 180, short/long Z/Z, keyboard H horse and E dual function, pad LB+RB and R3 as configured. ConfigRevision **2131**. Previous V0.96 ZIP is archived; only experimental `debug` was edited. [Full native audits / rollback notes](docs/V0_97_HORSE_AREA_RAW_MOUSE_CINEMATICS_RADIAL.md).


## V0.98 (2026-10-12) | User-validated horse/equipment input, Restricted only default, wheel-camera cleanup

**Validated by user:** V0.97 Horse Everywhere works perfectly. R3/native equipment wheel is FUNCTIONAL again using the right analog stick. These fixes, plus LB wheel selection, original horse summon/mount lifetime guards, camera/gamepad aim, shared E equipment + temporary view, and Z/Z TPS controls are **frozen**. A native aerial-camera transition while opening other radial wheels is ACCEPTED and now explicitly **deferred**. Do not keep layering experimental wheel-camera overrides.

**New authoritative default:** `HorseAreaMode=2` or **Restricted only**, for fresh `DarksidersGenesisMod.ini`, ASI C++ ResetDefaults, runtime initialization and missing INI key fallback. Four menu choices remain Off, In all area, Restricted only, Banned zone only. Existing INI files with an explicit horse area mode retain their chosen value rather than having it forcibly overwritten; a fresh V0.98 INI will select Restricted only. Config revision **2132**. No modification to validated native horse permission and desummon callback implementation.

**Horse summon icon removal request:** no horse-summon-icon hide feature exists in active V0.97 source or DLL configuration. The game's **own options** control that icon, so the planned mod-side feature is now closed as **not needed**. Active overlay text no longer suggests a speculative padlock/icon mapping. Do NOT conflate this icon with working Hide HUD, Hide Reticle or the two shader-signature targeting indicators; these must remain untouched.

**Removed inactive/unsuccessful code:** the V0.95 camera radial view-continuity exception `radialGameplay` and its log branch are removed from `CameraTraceFeature.cpp`. V0.97's successful *input side* remains entirely intact: native LB/R3 right-stick packet pass-through and held-wheel flags that prevent competing TPS camera orbit. Accept vanilla aerial camera during wheels until a separate dedicated investigation is requested.

**Unchanged/unvalidated:** new V0.97 RAW mouse TPS experiment and automatic cutscene suspension remain as-is, pending explicit results. [V0.98 engineering/validation notes](docs/V0_98_RESTRICTED_HORSE_WHEEL_CLEANUP.md). V0.97 cumulative ZIP archived, stable `main` unchanged, experimental `debug` only.


## V0.99 (2026-10-12) | Native TPS camera-cutscene classification and read-only physical mouse audit (TEST)

**Stable game functions remain frozen:** user-validated Horse Everywhere (Restricted only by default), physically held R3 equipment radial wheel native right-stick selection, LB wheels, native gamepad aiming/mounted camera, H/LB+RB horse, shared E equipment + temporary view, Z/Z controls and the V0.90 two exact Strife target-indicator shader hashes. Accept aerial camera during wheels. **No code edits to HorseFeature.cpp, XInput FilterNativeXInput, native wheel input, shader filtering or User INI defaults**.

**Automatic cutscene refinement:** earlier V0.97 log shows false cinematic transitions with missing/stale actor samples and an otherwise normal native gameplay camera pitch. V0.99 does not interpret a missing actor alone as cinematic evidence: the original camera must show distinctly non-gameplay pitch **or** a verified live actor far away; evidence must remain consistent for 500ms. Native LB/R3 wheel selection is excluded. Detected genuine native-view transitions still suspend TPS and reattach when a valid gameplay view returns; option `TPSAutoCutscene=0` preserves the no-extra-suspension behavior. There is no official Unreal Sequencer state hook; this remains a conservative heuristic.

**TPS mouse diagnosis, with no input/shot rewrites:** V0.97 RAW-only mouse camera remains intact. Removed one unused historical cursor-warp counter. Added low-volume, bounded diagnostics for physical RAW WM_INPUT packet/motion count (including actual LMB fire), WM_MOUSEMOVE fallback movement, TPS orbit applied/native yaw while firing, and the game's untouched native look vector versus camera yaw. The one-session log now prints `TPS V0.99 RAW INPUT`, `FALLBACK INPUT`, `FIRE ORBIT`, `MOUSE AIM AUDIT` and shutdown totals. This is NOT a claimed 3D projectile/raycast aiming fix; the native projectile trace source is still unknown.

**Test:** walk/rotate mouse without fire, then shoot and rotate while holding mouse left-click; compare native/RAW camera behavior in the new log. Verify a genuine cinematic and loading transition. Recheck R3 equipment wheel and horse restricted areas for regressions. Existing ConfigRevision **2132**, existing user INI unchanged. [V0.99 technical log and rationale](docs/V0_99_TPS_MOUSE_CINEMATIC_NATIVE_AUDIT.md). Previous cumulative V0.98 archived on `archive`; protected `main` unchanged. 


## V1.00 (2026-10-12) | Restore V0.98 mouse camera & equipment-wheel baseline, isolated mouse forward-fire (TEST)

**Latest user correction:** the newest attached log is actually **V0.98** (`0.98-restricted-horse-default-equipment-wheel-validated-cleanup`) and the user confirms both **on-foot mouse TPS camera rotation *without* firing** and the **equipment wheel** work in V0.98. R3 **USES** the selected equipment; it is *not the equipment wheel's opener*. The previous `TPS V0.97 WHEEL: R3` diagnostic label is semantically wrong and is not proof of wheel activation. V0.99 is a rejected experiment.

**Exact camera rollback:** `src/CameraTraceFeature.cpp` restored as a whole from the successfully compiled user-tested V0.98 commit `c5fe4d0`, to avoid guessing which V0.99 cinematic camera-shape branch might have impacted wheel selection. This also restores the V0.98 false-positive cinematic detector behavior on temporary missing actor samples. The separate cinematic feature remains experimental and needs a later isolated repair, *not bundled with this mouse experiment*.

**Mouse firing experiment ONLY:** `TPSMouseForwardFire=1` (default) makes the native *look-vector getter* return `(cos(TPS camera yaw), sin(TPS camera yaw))` only when physically holding left mouse button on foot, without a simultaneously firing gamepad, with a verified fresh local TPS view and no wheel. `=0` bypasses the experiment, returning native vanilla look, exactly as in V0.98. UI under Camera → TPS controls and aiming. This **mimics forward aim from the camera** but does NOT create an XInput packet, hook the actual weapon raycast or change the physical stick/gamepad controller. The camera rotation when not firing, all validated controller XInput, R3 use/stow, LB selection, shared E, Horse Everywhere Restricted default, shader aim visibility and all other prior features remain untouched. New config revision **2133**. Log prefix `TPS V1.00 MOUSE_FORWARD` includes applied yaw counts and bypass reasons.

**New Gameplay feature roadmap, NOT yet active:** three independent multiplier settings requested for **Wrath (Courroux)**, **Ultimate Form** and **Synergy (Synergie)**. The existing Hotstreak `AddJuice` hook at `0x660260` is a separate verified mechanic and cannot safely be repurposed as evidence for these gauges. Find each native owner, verify local character, apply bounded charging multipliers, then add their distinct ON/OFF + sliders to the Gameplay UI. No decorative non-functional controls.

**Skip Warnings remains open:** current early `dxgi_proxy.cpp` warning-list trampoline checks ONE/TWO native 0x40-byte definitions; the newest V0.98 user log shows a THREE-definition span `0xC0` with `seen=1 skipped=0`. Next native data audit must identify the specific warning record(s) before changing the loop gate, so unrelated startup screens and validated skip logos/intro remain safe. [Full V1.00 source comparison / roadmap](docs/V1_00_MOUSE_FORWARD_AND_GAMEPLAY_ROADMAP.md). Canonical [CURRENT_STATE](docs/CURRENT_STATE.md) updated. Previous V0.99 ZIP archived; branch `main` untouched.
