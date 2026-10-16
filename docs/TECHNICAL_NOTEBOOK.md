## V1.45 — construite, contrôlée et installée : coop / potions TEST (16 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation en jeu encore requise.** Base solo V1.43, potions V1.44 conservées ; source `332cbc973c521910522df990df98e0867066726e`, archive debug `b68341ee7d1aa3f44d2dbfce6f560c4bbf09afba`. [Compilation Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38890283190) **SUCCESS, 17/17 tests**, dont le résolveur natif de propriétaires et le vrai module de caméra/commandes P2 isolé. Analyse MSVC : mêmes huit avertissements historiques que V1.44, aucun nouvel avertissement dans les modules coop. Contrôle en lecture seule sur la session coop : les quatre composants Follow/Split reconnaissent leurs deux propriétaires distincts.

[ZIP cumulatif V1.45 — téléchargement direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.45_LOCAL_COOP_OWNERSHIP_TEST.zip), **820459 octets**, SHA256 `D08CA7A180B12AC6F6A61CE1A1E6E8543C7E6A4D76581B657E32F6CD496B6FA7`. Exactement ASI, dxgi.dll, INI et README à la racine ; deux binaires AMD64, identité embarquée1.45,115clés INI et hashes CI/dépôt/téléchargement direct vérifiés. **108 anciennes archives inchangées**, main2bc3122 / archive10e0f22 et release publiquev1.43 vérifiées intactes.

Installation après fermeture effective du jeu, avec sauvegarde complète des quatre fichiers et du log : `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v145-20261016-125910`. ASI/dxgi remplacés et hashes de destination conformes ; **INI personnel préservé octet pour octet**, SHA256 `5766A97774293C8C30F14D0855EAEC4A4B1138C899548A0088DC9A0E3BA9D0D7` avant/après. Le README installé est actualisé avec ce reçu ; le ZIP immuable conserve son README de préparation. Aucun lancement automatique.

Le suivi, l'orbite et la visée P2 utilisent désormais leur propre chaîne joueur/contrôleur/caméra ; les raccourcis existants sont conservés. Les ajouts potions comprennent remplissage, recharge1–100 et Unlimited, assignables dans Hotkeys sans réaffecter les touches existantes. RawInput, Alt-Tab, gains/jauges, munitions, filtres shader et Skip Warning restent identiques ; les raccords de propriété caméra/cheval/manette sont décrits dans l'audit. **Le comportement coop, les potions et le bug de visée native après dash sont à tester. Action Recovery reste indisponible** : la durée supplémentaire se situe dans l'animation Level3 du troisième dash de Strife, sans point sûr de fin anticipée encore démontré.

**Test prioritaire :** éloigner les deux personnages, tourner chaque caméra avec sa propre manette, puis faire réapparaître chaque personnage et vérifier qu'il garde sa vue. Contrôler ensuite roues, vue native + visée + dash, cinématique et potions ON/OFF. [Preuves, limites, TODO et rollback](V1_45_LOCAL_COOP_OWNERSHIP.md). Les préparations ci-dessous sont des enregistrements historiques.

## V1.45 — caméras coop attribuées à leurs joueurs (source de TEST)

Base solo V1.43 validée hors bugs signalés ; ajouts potions V1.44 conservés **CONSTRUITS / CONTRÔLÉS, non encore testés en jeu ni installés**. V1.44 source248db7b, CI38885637165 **SUCCESS,15/15 tests** ; ZIP812291octets SHA256 `7704EE2A7005F149002467826FEB650E7F9579EEF80EDA6C7D3FFA6E950B838F`, quatre fichiers à la racine, binaires AMD64 et téléchargement direct contrôlés. Archive V1.44 conservée immuable ; la release publique V1.43 reste en place.

La capture coop du16octobre prouve deux LocalPlayers, deux contrôleurs et deux caméras `SplitscreenCamera` appartenant chacun à leur personnage. Le mod utilisait pourtant une référence globale remplacée par le dernier personnage observé. V1.45 prépare une identité P1 fixée par `GameInstance.LocalPlayers[0]`, une vue P2 avec suivi/orbite/pause/visée séparés et sa propre manette native `ControllerId`. Les réglages et boutons reconfigurables existants sont repris ; aucune nouvelle touche imposée. L'ancien hook natif de masquage persistant des effets de visée est retiré ; le filtre shader validé reste identique. **Le bug visuel/de direction après dash en vue native reste à retester, sans cause démontrée à ce stade.**

La troisième capture dash identifie `Strife_Dash_Level3_Montage` : durée observée environ0,83s contre0,40s pour les deux premiers dashs, avec une fin de déplacement faible après environ0,5s. Les positions seules ne mesurent pas l'instant d'appui du quatrième dash et ne prouvent pas un point sûr de fin anticipée. **Action Recovery reste indisponible, aucune nouvelle intervention dans l'animation.** [Preuves natives, changement, limites, tests et rollback](V1_45_LOCAL_COOP_OWNERSHIP.md).

**Source préparée / EXPÉRIMENTAL. Construction V1.45, paquet, installation et validation utilisateur pas encore annoncés.** Objectif : chaque vue et commande de caméra suivent leur joueur avant/après réapparition ; aucun vol de caméra entre P1 et P2. RawInput, Alt-Tab, tir souris solo, potions, munitions, jauges, roues et Skip Warning conservés hors raccords de propriété décrits dans l'audit.

## V1.44 — potions natives TEST et capture Strife (16 octobre 2026)

Base V1.43 debug `5fecde0`. **Sources préparées / EXPÉRIMENTAL, construction et installation à vérifier.** Ajout des commandes **Fill health potions**, **Potion recharge rate** (1–100×, défaut2 désactivé), **Unlimited potions** (OFF), chacune assignable dans Hotkeys → F1-F12. Remplissage et maintien du stock par l'ajout/plafond natifs ; recharge par conversion du surplus de soin ; soin et cooldown conservés. Trois nouvelles clés, 112 valeurs et raccourcis antérieurs inchangés. Entrées/visée/caméra/cheval/roues/jauges/munitions/warnings préservés ; aucun nouveau résultat en jeu revendiqué.

Recovery reste indisponible : deux captures en lecture seule,22 puis17 instances natives de dash, seconde confirmée avec Strife. Délai global0 ; mode de mouvement(6,5) maintenu jusqu'à la fin, aucune fin de fondu exploitable observée. Pas de retour de l'ancien hook sans effet ; prochaine recherche limitée à la fin du déplacement réel et à la libération native du dash, coffres reportés. V1.43 publique,107 archives et main/archive protégés. [Preuves natives, configuration, tests, TODO et rollback](V1_44_NATIVE_POTIONS_AND_DASH_AUDIT.md).

## V1.43 publiée et installée — défauts utilisateur (16 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / PUBLIÉ / INSTALLÉ.** Base V1.42 debug `45571dd354e316fcb4f1fa255741844c5dd09d54`. Source `902861eb1ff34c14dc5ded092581b360767da817` ; [Actions Windows x64 38880517949](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38880517949) **SUCCESS, 13/13 tests**. ASI, INI, génération, runtime, resets et boutons Default partagent les **112 valeurs** du profil utilisateur. Seuls les défauts cheval changent : **distance 0,25× / vertical +40** ; pied 0,15/0, ultime 0,30/+80, pitch −12°, FOV TPS 110°. Les 110 autres valeurs, Inser et les raccourcis sont conservés.

[Release publique v1.43](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases/tag/v1.43) depuis **debug**, non-prerelease ; tag et target_commitish vérifiés sur le source construit. [Téléchargement direct du ZIP cumulatif](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases/download/v1.43/DarksidersGenesis_V1.43_USER_DEFAULTS_RELEASE.zip), **807377 octets**, SHA256 `A680334FC4E634AC24AF92DF59A00BEFCCA8B4716EEABDDCB08BCE1C64FAF990`. Quatre fichiers à la racine, deux binaires PE AMD64, identité V1.43 et parité INI vérifiés. Archive Git, asset public, checksum et CI identiques. Publication de l'archive au commit `4f6115ff9a2cf30bb3523e6b7544c6a7bc645a6f` ; **106 anciennes archives, main et archive préservés**. Analyse MSVC exécutée, mêmes huit avertissements déjà examinés, sans nouveau gain de performance ou validation en jeu revendiqué.

Installation jeu fermé avec sauvegarde des quatre fichiers et du log : `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v143-20261016-113324`. Binaires installés conformes ; **INI personnel conservé byte pour byte**, SHA256 avant=après `5766A97774293C8C30F14D0855EAEC4A4B1138C899548A0088DC9A0E3BA9D0D7`. README installé actualisé avec ce reçu ; le ZIP immuable conserve le README de préparation du source. Aucun jeu lancé automatiquement.

**Action Recovery reste indisponible ; prochaine investigation : dash uniquement, coffres reportés.** Aucun hook recovery réintroduit. Fonctions natives caméra/visée/souris/manette/cheval/roues/jauges/munitions/warnings/updater identiques à V1.42 ; validations déjà acquises conservées, sans nouvelle preuve en jeu pour V1.43. Test suivant : Default cheval 0,25/+40 et Updates V1.43 ; puis capture synchronisée du dash et identification de sa libération native avant un essai fonctionnel. [Profil, preuves, TODO et rollback](V1_43_USER_DEFAULTS_RELEASE.md).

## V1.42 construite, contrôlée et installée — caméra cheval et version updater (16 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL en attente de test en jeu.** Base V1.41 debug `04ca27e`. Source `bf02da378a8890c07581045019ac396d0a008dc3` ; [CI Windows x64 38875417060](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38875417060) **SUCCESS,13/13 tests réussis**, dont vraie persistance/héritage/indépendance des vues pied/cheval/ultime et parité des **112 clés**. Analyse MSVC exécutée, mêmes huit avertissements déjà examinés en V1.41. ZIP publié par Actions au commit `64c4e93c2ee02b5a891237b6facb5306bdcc35e8`.

Camera → Third Person → **Horseback TPS camera**, sous l'ultime : **Distance (Horseback)** et **Vertical offset (Horseback)** avec saisie manuelle, curseur et Default. Les anciens INI héritent de leur vue à pied pour démarrer ; valeurs ensuite indépendantes après sauvegarde. Défauts neufs0.15/0 et reset TPS inclus. Pose montée native existante conservée ; trois sélections de valeurs ajoutées, orbite/visée/commandes/roues/gauges/munitions/warnings préservés. **Updates affiche désormais la version compilée**, supprimant le texte V1.22 figé signalé dans la capture ; V1.42 attendue au prochain lancement. La comparaison de version déjà corrigée reste identique.

[Télécharger le ZIP cumulatif V1.42 TEST](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.42_HORSE_CAMERA_SETTINGS_TEST.zip), **806041 octets**, SHA256 `3C47E465DAF03D7C9AE8B6767045A1444CCA03C0A37D67BC15AE30B89F76E2FC`. Exactement quatre entrées à la racine, deux binaires PE AMD64 et identité ASI1.42 vérifiés. Archive Git/téléchargement direct/CI identiques ; **105 archives précédentes**, main et archive vérifiés inchangés.

Installée après fermeture du jeu, sauvegarde complète `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v142-20261016-104624` ; hashes des binaires installés conformes. **INI personnel conservé byte pour byte**, SHA256 avant=après `CD18319DD0468D35E7E0BCDA3A7EEEE9C76F8B7689131C0CA9AD75E313518AE9`. Les deux clés manquantes sont héritées en mémoire et seront écrites par le prochain enregistrement des réglages ; les110 anciens réglages et les changements personnels récents restent conservés. README installé actualisé avec ce reçu ; ZIP immuable conserve son README de préparation. Aucun jeu lancé automatiquement. [Détails, tests et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_42_HORSE_CAMERA_SETTINGS.md).

**Action Recovery reste indisponible.** La relecture de l'ancienne capture montre18 observations/12s, délai global déjà0, quelques états MONTAGE et aucun DASH/INTERACT identifié : manque de synchronisation avec l'attente ressentie. Prochaine étape : capture pendant les deux actions puis identification de leur verrou/libération native, avant un autre essai fonctionnel. [Preuves, piste et protocole](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ACTION_RECOVERY_NEXT_INVESTIGATION.md). Test de V1.42 : réglages cheval distincts, monter/descendre, ultime, tir/roues, puis vérifier Running mod dans Updates ; aucune nouvelle validation en jeu revendiquée.

## V1.42 - préparation historique (reçu ci-dessus)

Base livrée V1.41 debug `04ca27e`. **Sources préparées / EXPÉRIMENTAL**, construction et installation à vérifier. Camera → Third Person → **Horseback TPS camera**, sous la forme ultime : **Distance (Horseback)** et **Vertical offset (Horseback)**, avec curseur, saisie manuelle et Default. Deux valeurs indépendantes persistées ; anciens INI : héritage de leurs paramètres à pied. Défauts neufs0.15/0, reset TPS étendu ; les110 valeurs préexistantes et Inser conservés.

La caméra utilise la pose montée déjà reconnue, avec priorité à la forme ultime si active. Trois sélections de valeurs ajoutées ; provenance de pose, orbite, pitch/FOV partagés, visée/RawInput/XInput, cheval, roues, jauges/munitions et Skip Warning inchangés. Tests de configuration étendus aux trois profils et parité112 clés. Aucun nouveau succès en jeu annoncé. Jeu ouvert pendant la préparation ; installation avec sauvegarde après fermeture. [Détails, critères de test, compatibilité et rollback](V1_42_HORSE_CAMERA_SETTINGS.md).

Correction du retour utilisateur sur la capture Updates : l'affichage « Running mod: V1.22 » était encore écrit en dur malgré la comparaison corrigée en V1.41. Le libellé et le préfixe du journal suivent maintenant BuildInfo ; ils doivent afficher la version compilée V1.42. Ce point manqué dans l'audit est documenté ; logique d'installation et chemins de contrôle inchangés.

## V1.41 construite, contrôlée et installée — 16 octobre 2026

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL en attente du test utilisateur.** Base livrée V1.40 `e230b53` ; source V1.41 `d1bae62d3f191f8efb0a2c4b51a50667920ca08a`. [GitHub Actions Windows x64 38843203433](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38843203433) **SUCCESS**, job116787762522, **13/13 CTest réussis** et analyse native MSVC `/analyze` exécutée. Huit avertissements examinés et détaillés dans l'audit, pas une prétendue absence d'avertissements. Test isolé C# local : zéro échec. Archive publiée au commit `0b42a1942c25819fc50152e151263920fb808067`.

[ZIP cumulatif V1.41 TEST](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.41_AUDIT_CLEANUP_INSERT_TEST.zip), **803900 octets**, SHA-256 `0BEB4EAFC2C1212EC56EB5CC4FEA32F4F8ECB360CC0D2A0F0B1F82AF2DEDB67B`. Exactement quatre fichiers à la racine, deux modules PE AMD64, identité ASI1.41, ancien recovery actif absent, anciens hooks de gains retirés. Le téléchargement direct et l'archive Git ont le même hash que la CI. Les **103 archives antérieures** et main/archive sont inchangées. Updater autonome V1.1 séparé, [CI38843203439 réussie](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38843203439), deux entrées à la racine / PE AMD64 vérifiés ; ancien V1.0 préservé.

**Inser est rétabli** dans les valeurs par défaut et dans l'installation personnelle. Les **109 autres valeurs** de l'INI restent identiques ; hors unique remplacement MenuKey, octets conservés. Sauvegarde complète avant installation : `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v141-20261016-050412`. INI avant `A351972C25730520AC248854F0032C8A659FA94434F2C9F0F8111CCAD772EB8D`, après `FA634ED0793339FC6AE34E72C08D47A86447CCB8300295EE9056C8BFDECF097E` ; hashes des binaires installés conformes au ZIP. Installation jeu fermé, sans lancement automatique. README installé actualisé avec ce reçu ; le ZIP immuable contient le README de préparation du source.

Nettoyage : **802 lignes natives nettes en moins**, trois anciennes installations de hooks supprimées, sondes obsolètes retirées et lecture du yaw réduite à une seule valeur dans deux callbacks fréquents. Erreurs de sauvegarde et de version updater corrigées ; commandes reconfigurables publiées par atomiques. Calculs validés des munitions/gains et fonctions acquises préservés. **Action Recovery retiré car sans effet**, toujours à réétudier après dash/coffre. Aucun gain de FPS chiffré ni nouveau succès en jeu annoncé. Test suivant : Inser, munitions/gauges, TPS pied/cheval, sticks, roues/pause, Alt-Tab, cinéma et forme ultime. [Audit détaillé, limites restantes, TODO et rollback](V1_41_FULL_AUDIT_CLEANUP.md).

## V1.41 - préparation historique de l’audit (reçu ci-dessus)

Base V1.40 debug `e230b53`. **Sources préparées / EXPÉRIMENTAL**, construction et livraison à vérifier. Audit de source, logs, configuration, callbacks natifs, updater et workflows : anciens hooks/sondes sans bénéfice retirés ; essai Action Recovery abandonné après zéro effet mesuré, clairement indiqué indisponible. Correction de la fausse réussite de sauvegarde INI, de la comparaison updater restée V1.22 et de son historique périmé après une installation manuelle. Huit commandes reconfigurables publiées par atomiques ; règles XInput/sticks/tir conservées. Lecture du yaw seule dans deux callbacks fréquents au lieu de copier toute la télémétrie.

**Menu par défaut : Inser**, dans ASI, INI, génération et boutons Default. Les 109 autres valeurs du profil V1.40 restent identiques. Calculs validés munitions/gains/remplissages, RawInput, Alt-Tab, cheval, roues, caméra et présentation Skip Warning conservés ; aucune nouvelle validation en jeu annoncée. Anciennes archives et main/archive préservées. [Audit complet : erreurs, code retiré, risques restants, contrôles, TODO et rollback](V1_41_FULL_AUDIT_CLEANUP.md).

## V1.40 built, checked and installed - user defaults / ammo validated (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE.** Base V1.39 debug474bbfa. User **VALIDATES ammunition behavior**, while **Action Recovery remains unresolved** (last test12132 ticks,8927 local montage observations,zero shortened tails/finishes). This defaults release adds no native recovery intervention and claims no recovery repair. Source86f773cabf0c2a2523c36ffb83b4127538a3e4f6; [Windows x64 CI38807877488](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38807877488) **SUCCESS,11/11 CTest suites passed**. The new real config test compares all **110 shipped INI keys** with compiled missing-INI generation and reset output, checks runtime settings and preservation of explicit older overrides. Archive publication f88880fe8a618b3ad5070170d26613d41d16b07c.

[Download cumulative V1.40 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.40_USER_DEFAULTS_AMMO_VALIDATED_TEST.zip), **811138 bytes**, SHA256 `662BAD16053DA496AF6495AFCBDA306B5EC57AA9493FFE429E17BB0279FBD77A`. Exact four root files, PE AMD64 binaries, embedded ASI1.40 identity and full packaged INI values equal to the saved user profile. CI/repository/direct-download hashes match; all **102 previous archives**, main2bc3122 and archive10e0f22 verified unchanged.

Saved profile now defines new installs, reset and Default buttons: user's menuVK_C0, debug update checks, ammo reductionON/divisor2, Unlimited togglesOFF, gains4/4/4, Ultimate camera0.30/+80, F1-F9 gauge/Unlimited actions and F12 HUD. Existing explicit personal INI values remain authoritative; legacy missing-Ultimate keys still inherit their normal camera. All native ammo/recovery/input/camera/horse/gauge/warning/updater implementations are identical to V1.39; only defaults, descriptive status and build identity changed. The user-selected defaults are incorporated; **no new in-game claim is made for V1.40 or Action Recovery**.

Installed with game closed, complete backup `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v140-20261015-231525`, matching installed binary hashes and **personal INI preserved byte for byte**, SHA256 `A351972C25730520AC248854F0032C8A659FA94434F2C9F0F8111CCAD772EB8D` before/after. Installed README refreshed with this receipt; immutable ZIP retains its source-preparation README. No automatic game launch. Next: retain the validated ammunition path and investigate the real recovery lifecycle/eligibility before another intervention. [Defaults, runtime evidence, compatibility, rollback and TODO](V1_40_USER_DEFAULTS.md).

## V1.40 source-preparation record (historical, superseded above)

Base V1.39 debug474bbfa. User **VALIDATES ammo**, reports Action Recovery still ineffective, and explicitly chooses the current installed INI as the new ASI/INI defaults. Saved profile is promoted: menuVK_C0, debug update channel, ammo reductionON/divisor2, gains4/4/4, Ultimate camera0.30/+80 and current F1-F12 bindings. Store/runtime/reset/Default buttons are synchronized; explicit existing INI overrides and legacy camera-key inheritance are preserved. Ammo/camera/input/gauge/native recovery implementations are unchanged. V1.39 recovery log has12132 ticks/8927 local observations but **zero applied tails/finishes**; UI now labels it unresolved. **Source prepared / EXPERIMENTAL; build/package/install not yet claimed.** [Profile, evidence, acceptance and TODO](V1_40_USER_DEFAULTS.md).

## V1.39 built, checked and installed - ammo / native animation recovery TEST (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL.** Base V1.38 debug e39153e, with Unlimited Wrath and Skip Warning now explicitly **VALIDATED by user**, preserving earlier gauge/TPS validations. Source 1a1684a82d861cc5cc3f066879ff7dcf4d787791; [Windows x64 CI38802359996](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38802359996) **SUCCESS, 10/10 CTest suites passed**, including real ammo INI/hotkey save/reload, fractional cost/Unlimited policy and recovery delay resets. Numbered ZIP publication ee59029851ae9549556418f089d6d75b2339ed75.

[Download cumulative V1.39 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.39_AMMO_ACTION_RECOVERY_TEST.zip), **809940 bytes**, SHA256 `4BE85D59CC89B1D11D6AE4341B78B9221520CE1F95DC691FDF5447F76A2F9441`. Exact four root entries, both binaries PE AMD64, embedded ASI1.39/new-feature identities, former forced-MOVE ready path absent, default ammo controls OFF/divisor2 and all older INI defaults retained. CI/repository/direct-download ZIP hashes match. All **101 earlier archives**, main2bc3122 and archive10e0f22 checked unchanged.

Installed with game closed after the user's closure reply, binary destination hashes verified. Complete prior installation/log backup: `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v139-20261015-225133`. **Current personal INI preserved byte for byte**, SHA256 `50274EA27343151CE44A38374F4C585163D96BC06EFA64D42FA69DC7A60AD51C` before and after; no personal camera value, gauge setting or shortcut replaced. Installed README refreshed with this delivery receipt; the immutable ZIP retains its pre-CI cumulative README. No game was launched automatically and **no V1.39 in-game functionality is claimed validated**.

Gameplay -> Combat/resource controls -> **Ammunition (test)**: optional **Reduce ammo consumption** divisor0-10 (0/1 normal,2 half,10 one tenth), independent **Unlimited ammo**, each assignable in Hotkeys -> F1-F12. Existing assignments retained. Player movement -> **Action Recovery** retains its checkbox/action/delay, now testing normal native completion at natural animation blend-out instead of overriding MOVE. Full XInput/RawInput/camera aim/horse/gauge/warning implementations protected against the V1.38 base. **Next test:** finite special ammo OFF/2/10/Unlimited ON then OFF; dash and chest with Recovery OFF/ON(delay0), retained loot, proper locomotion/attacks and no sliding. Runtime counts are evidence of hook activity/completion, not measured user responsiveness. [Native evidence, remaining limits, acceptance and rollback](V1_39_AMMO_ACTION_RECOVERY.md).

## V1.39 source-preparation record (historical, superseded above)

Base **V1.38 debug e39153e**; user now validates Unlimited Wrath and Skip Warning, extending earlier gauge/TPS validations. Adds saved optional ammo reduction0-10 (0/1 native,2 half,10 one tenth), Unlimited ammo and two independent Hotkeys actions; no existing binding/default is reassigned. Replaces the ineffective forced-MOVE/AWAITING_FINISH recovery route with normal native completion of natural montage blend-out tails. Read-only V1.38 samples show MoveInterruptDelay0 and direct RUNNING-to-FINISHED transitions; exact native blend/finish paths were audited before implementation. Main animation/input path stays native until blend-out; this remains **EXPERIMENTAL**, including loot/locomotion/actual timing and real ammo spending. Windows build/package/install not yet claimed. [Evidence, acceptance, limits, rollback and cumulative TODO](V1_39_AMMO_ACTION_RECOVERY.md).

## V1.38 follow-up - Wrath OFF / Void hub (2026-10-15)

User reports free abilities at zero with Unlimited Wrath OFF, then confirms **Strife in the Void hub**. That hub normally permits free Wrath ability use independently of the mod. Log confirms OFF20:06:13, manual0% with matching readback20:07:01, gauge stays0 until explicit all-ON20:27:39; shutdown has **zero Wrath hold overrides** and one later authorized top-up. **Void behavior explained; no gameplay fix or mission validation claimed.** Current V1.38 binaries/configuration retained; no new build. Next: test the same Wrath ability in an actual mission with Unlimited OFF, then ON/OFF. [Evidence, limits and next test](V1_38_WRATH_OFF_CHECK.md).

## V1.38 built, checked and installed - Unlimited / Ultimate recenter / Skip Warning TEST (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL.** Base V1.37 debug985b0ff; all three natural gauge gain multipliers now **VALIDATED by user**, in addition to prior manual refills and ordinary TPS. Source 729f747770b9e3790775fe580c94357a92f446ae (initial feature commit cb9f130, final warning-counter diagnostic adjustment); [Windows x64 CI38780065798](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38780065798) SUCCESS, all **7/7 CTest** suites passed: stick roles, manual request policy, persisted gauge hotkeys/Unlimited/SkipWarning settings, Ultimate camera INI/runtime, natural reward policy, Unlimited identity/cap policy and exact named warning-page selection. Archive publication f57e9eeaa453b699c8e96a536efd5f2cd4ba59c9.

[Download cumulative V1.38 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.38_UNLIMITED_RECENTER_WARNINGS_TEST.zip), 800204 bytes, SHA256 `DEB367E0A8260B8CF0DC1E2F99974FF16ABBD11487431C2C0C72DF30814C46D0`. Exact four root entries, PE AMD64 binaries, embedded ASI1.38 identity, SkipWarning=1 / Unlimited defaults OFF and direct-download/CI/repository hashes independently checked. Installed with game closed and matching binary hashes. Backup: `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v138-20261015-194914`. Personal INI preserved except the explicitly requested **Features.SkipWarning=1**: exactly one byte changed, previous SHA256 `E43F791EB19ADF3DC289A8459B482B97672066DA9329DDB2146AD737D8D27E45`, new `1AC57E025B9A70EABC6E33B9820C38E70ACE045E1CF4FB5E44FD05178C33486F`; all personal camera values, multipliers and F1 assignments retained. Installed README refreshed with this receipt; immutable ZIP retains source-preparation README. main/archive and every previous archive baseline checked unchanged. Preliminary first CI ZIP retained in commit8829e789 and locally under ../audit-v131; final ZIP uses the corrected counter build.

Gameplay -> Resource recharge -> **Unlimited gauges (test)**: three independent saved checkboxes. Hotkeys -> F1-F12: **Toggle Unlimited Wrath / Ultimate / Synergy / all gauges**, no existing/default assignment changed. Unlimited has priority over manual target percentages, respects actual native maxima, and is OFF by default. Fresh verified form yaw supplies recenter; no input-path changes. **Skip Warnings (test) is ON as requested**, using the new exact-named presentation timer/fade path; rejected asset-load-loop bypass remains OFF. New features remain EXPERIMENTAL: no in-game Unlimited, recenter or warning-skip success is claimed. Test cold startup/notices/reticle/wheels, each Unlimited + all toggle then OFF/native spending, both Ultimate forms/recenter/exit. [Evidence, limitations, rollback and cumulative TODO](V1_38_UNLIMITED_RECENTER_WARNINGS.md).

## V1.38 source-preparation record (historical, superseded above)

Base V1.37 debug985b0ff; **ALL three natural gauge gain multipliers now VALIDATED by user**, alongside prior manual refills and ordinary TPS. Adds three saved Unlimited checkboxes (default OFF) plus independent/all-gauges F1-F12 actions without changing bindings. Fresh active-form yaw repairs recenter's stale normal-pose selection. New named save/controller presentation experiment advances native timers/fades/cleanup; **SkipWarning defaults ON at explicit user request**, while the rejected asset-loading-loop bypass stays OFF. No new gauge hooks; full XInput/RawInput/native aim/horse/legacy startup proxy retained. **EXPERIMENTAL, source prepared; Windows build/install pending; no gameplay success claimed.** [Evidence, acceptance, rollback and cumulative TODO](V1_38_UNLIMITED_RECENTER_WARNINGS.md).

## V1.37 built, checked and installed - Ultimate camera / Strife / native rewards TEST (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL.** Cumulative base V1.36 debug6e1e5fe, preserving validated manual refills/ordinary TPS. Source9ae0f59fe4067f1385fb98000121d41b380ddac2 (gameplay sourcef8c9858, Windows test-name repair only afterward). [Windows x64 CI38758958418](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38758958418) SUCCESS; all five CTest suites passed: stick roles, gauge request policy, saved hotkeys, independent Ultimate camera INI/runtime and positive resource-gain policy. Archive publication44074e32ab2d47aa7a0206bab3d6b8ec1ab5fd41.

[Download cumulative V1.37 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.37_ULTIMATE_CAMERA_AIM_GAUGES_TEST.zip), 793735 bytes, SHA256 `59FA3FB722D608789E79EE700B205A30826A5929C6AE0BFB67AAFEF7CD051857`. Independently verified exact four root entries, PE AMD64 binaries, ASI1.37 identity and direct download/CI/repository hash agreement. Installed with game closed and matching binary hashes; V1.36 backup `../audit-v131/install-backup-before-v137-20261015-171955/`, personal INI preserved exactly SHA256 `D4CCBFD4D2C2BCA1058E66716B870C104F7787A6492A23A69EA7EEF299D56738`. Installed README refreshed; immutable ZIP retains source-preparation README. main/archive and every older ZIP unchanged.

Camera -> Third Person -> below existing TPS values: **Distance (Ultimate form)** and **Vertical offset (Ultimate form)**, with manual entry/Default, independently saved. Missing keys inherit current personal normal distance/height. Strife-only verified form ownership enables the existing camera-forward firing paths; War remains native. New experimental natural-gain multiplier paths target actual Wrath pickups and Ultimate combat rewards, excluding manual refill, spends, potions, upgrades and other actors. Existing Synergy math/manual queue, normal-player XInput body, relative RawInput, horse, wheels and hotkeys retained. **Framing, Strife form attack-helper coverage and natural multiplier effects require user gameplay tests.** Skip Warning still OFF/nonfunctional: native stage identified as asset loading; guarded native asset-path resolution added for subsequent presentation work. [Evidence, limitations, tests, TODO and rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_37_ULTIMATE_CAMERA_AIM_GAUGES.md).

## V1.37 source-preparation record (historical, superseded above)

Base V1.36 debug6e1e5fe; validated manual refill and accepted ordinary TPS retained. Latest log: four typed Ultimate entry/exit pairs,3592 valid poses, only3 missing-actor samples; four combined F1 refills applied. Adds **Distance (Ultimate form)** and **Vertical offset (Ultimate form)** below existing TPS values, independently saved; missing keys inherit personal normal camera. Strife-only verified native owner/type bridge enables existing camera-forward pad/look and native3D mouse target paths; War stays native. New exact-caller local-authority setter hooks test real Wrath pickup and Ultimate combat gains, excluding manual refill, spends, potions and upgrades; validated Synergy unchanged. **EXPERIMENTAL, source prepared; Windows build/install pending.** Skip Warning stays OFF: corrected old stage as asset loading, adds bounded native asset-path resolution for next presentation investigation. [Evidence, limits, rollback and TODO](V1_37_ULTIMATE_CAMERA_AIM_GAUGES.md).

## V1.36 built, checked and installed - gauge hotkeys / Ultimate pose TEST (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE. V1.35 manual fillers VALIDATED by user; new hotkey gameplay and Ultimate correction EXPERIMENTAL.** Base debug56fbedb, accepted ordinary V1.34 TPS with known Ultimate regression. Source44b6d12d1507e74926c54fe414ba93e46aec46eb; [Windows x64 CI38727065139](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38727065139) SUCCESS. CTest TpsStickRoles, GaugeRequests and GaugeHotkeys all passed (3/3), including actual INI save/reload of four new actions. Archive publication4d4ecd62c1b2c5c95fbab2f1cd5d61ff4f8bcf00.

[Download cumulative V1.36 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.36_GAUGE_HOTKEYS_ULTIMATE_POSE_TEST.zip), 788051 bytes, SHA256 `9538DC2BA73A1A649FA5653F51A63A2882C8B2ACC4B8C2BCBB4B5C56BBE55944`. Exact four root files, both binaries PE AMD64, ASI1.36 identity and direct-download hash verified against CI/repository. Installed with game closed, backup `../audit-v131/install-backup-before-v136-20261015-132719/`, matching binary hashes; personal INI retained exactly SHA256 `82AF15917398293EFA674C28EF064E15F85FF4D646B60ECBD11763AC3AD69D68`. Installed README refreshed with this receipt; immutable ZIP retains source-preparation README. All older archives and main/archive unchanged.

Hotkeys -> F1-F12: choose **Fill Wrath (100%)**, **Fill Ultimate (100%)**, **Fill Synergy (100%)**, or **Fill all three gauges (100%)**, then Save. No default or existing assignment changed. Same validated manual native refill queue; hotkeys are100% one-shot requests, the menu slider remains independent. New logs confirm7 manual requests/readbacks,2125 missing normal actor samples and ZERO native cinematic-mode events; exact Ultimate timing was absent. Native local form pose now supplies TPS follow and its native input passes through while transformed; no new hook. This is a bounded experimental correction, not an in-game success claim. Test four chosen hotkeys then Ultimate entry/movement/attacks/exit2-3 times; recheck ordinary foot/horse, wheels, pause, scene, both stick layouts and temporary native view. Natural Wrath/Ultimate gain multipliers remain unresolved. [Evidence, limitations, rollback and TODO](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_36_GAUGE_HOTKEYS_ULTIMATE_POSE.md).

## V1.36 source-preparation record (historical, superseded above)

**V1.35 manual fillers now VALIDATED by user for all three gauges.** Base debug56fbedb / accepted ordinary V1.34 TPS; Ultimate camera/control regression remains. Four100% refill actions added to Hotkeys -> F1-F12, independently and all together, with no changed assignments or INI defaults. Same validated native request path. New log: seven requests/applied readbacks,2125 missing actor samples, ZERO native cinematic-mode events; Ultimate timing was not marked. Native audit identifies the distinct form and adds a strictly typed local-pawn/owned-form pose, with native form input passthrough and no new hooks. **Ultimate follow/controls remain EXPERIMENTAL; source prepared, Windows build/install pending.** Existing normal XInput body, RawInput, horse, wheels, natural Synergy and manual refill engine retained. [Evidence, limits, rollback and acceptance](V1_36_GAUGE_HOTKEYS_ULTIMATE_POSE.md).

## V1.35 built, checked and installed - manual gauge levels (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL, gameplay confirmation pending.** Base V1.34 debug b94de95 (ordinary play accepted, known Ultimate camera/control regression). Source 25f2d94e1ab62a56e9f18f62333bb353f7582498; [Windows x64 CI38717620159](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38717620159) SUCCESS, both C++ tests TpsStickRoles and GaugeRequests passed (2/2). Publication commit 93adad6cbb44182a02ac0474ba874f8baa2ce3ec.

[Download cumulative V1.35 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.35_MANUAL_GAUGE_REFILL_TEST.zip), 784427 bytes, SHA256 `6CBAE44DF0FE7C8B226211170C8464D7DE1EB4A81A077349D3B241245E0136BD`. Exact four root files, PE AMD64 binaries, embedded ASI1.35 identity and direct download hash independently verified. Installed with game closed and matching binary destination hashes. V1.34 backup: `../audit-v131/install-backup-before-v135-20261015-120713/`; personal INI unchanged SHA256 `82AF15917398293EFA674C28EF064E15F85FF4D646B60ECBD11763AC3AD69D68`. Installed README refreshed with this receipt; immutable ZIP retains the source-preparation README.

Test on foot: open mod menu -> Gameplay -> Combat and resource gauges -> Resource recharge -> Target level100% -> Set Ultimate. Close menu and activate Ultimate normally. Set Wrath/Set Synergy and the0-100% slider are available for their respective tests. These are explicit one-shot native level requests, not repaired natural gain multipliers or automatic activation/unlock. Typed local ownership/authority, native maxima, freshness, actor/generation cancellation and expiry guard requests. **No in-game refill or transformed-camera success has been claimed.** Camera/input/natural Synergy/horse/config remain the V1.34 base. Next: verify actual gauges/activation and collect the Ultimate transition log before changing camera ownership. main/archive and every old ZIP verified unchanged. [Native evidence and remaining work](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_35_MANUAL_GAUGE_REFILL.md).

## V1.35 source-preparation record (historical, superseded above)

Base V1.34 debug b94de95: ordinary play accepted by user, **Ultimate overhead-camera/control regression newly reported**. Priority is explicit manual refill before camera repair. V1.35 adds queued one-shot 0-100% native level controls for Wrath, Ultimate/Chaos and Synergy, typed live-local ownership, native caps, readback logs, actor/generation cancellation and expiry. DebugRefill commands were proved shipping no-ops; actual native bounded setters are used. No camera/input/horse/natural Synergy/config changes; no auto refill, unlock or activation. Natural Wrath/Ultimate gain multipliers remain unverified. **EXPERIMENTAL, source prepared; Windows build/install pending.** [Evidence, limits and test](V1_35_MANUAL_GAUGE_REFILL.md).

## V1.34 verified Windows build and local installation - EXPERIMENTAL

**CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL, not user validated.** Base V1.33 debug 2291e80; source d362076f357306db1d45d727ae21622236bf5f6d. Windows x64 [CI38639339105](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38639339105) SUCCESS, including actual C++ stick-role matrix (native swap ON/OFF, physical camera Left/Right, movement retained during aim injection/filtering, buttons/triggers retained). Archive publication commit 77bb0b43e99032dc36a331a3f516bd2ae1f6305e.

[Download cumulative V1.34 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.34_NATIVE_ACTIONS_STICK_ROLES_TEST.zip), 777132 bytes, SHA256 `8D91A23547D30EA335A8E0FD69B27A9CA1E8189183A136438A12179483B8A561`. Independently verified four root files, both binaries PE AMD64, embedded ASI 1.34 identity and direct URL download hash. Installed with game closed; ASI/loader destination hashes matched; personal INI stayed exactly AB2F95CD2AE49AD1706E1E9A420FEEDB1E416D25FCA157E83897554EF2C3E095. Prior V1.33 backup: `../audit-v131/install-backup-before-v134-20261014-232427/`. Installed README is refreshed with this post-CI receipt; the immutable ZIP contains the pre-CI cumulative source README.

Complete TPS camera/opposite movement roles and native swap compensation; native pause now owns copied camera freeze; mouse native target requests continue through release. Existing wheel selection guards retained; passive native radial visibility is diagnostic only. **Secondary impacts, post-fire turn, native swap read in-game and LB/keyboard wheel-follow behavior still require user tests; automatic support for every in-game wheel/fire rebind is NOT claimed.** Next: test those cases FOOT/HORSE, all three wheels including rebound controls, pause/scene/temporary native view. Protected feature modules/config defaults and every earlier ZIP are unchanged; main/archive heads verified unchanged. [Full evidence and acceptance](V1_34_NATIVE_ACTIONS_STICK_ROLES.md).

## V1.34 source-preparation record (historical, superseded above)

Base debug 2291e80 / V1.33. User validates wheel sticks; incomplete stick exchange, wheel-control camera freeze, sideways secondary and post-fire turn remain reported. V1.34 maps camera/opposite movement roles consistently and reads the native swap setting, freezes copied view only for real native pause (or genuinely missing wheel pose), and lets native mouse target requests retain camera-center targeting after button release. UI option describes full role exchange. No new button-specific binding. Legacy configurable wheel-selection guards are preserved while native radial visibility is observed passively; full native action/rebind coverage is NOT solved or claimed. Native scene lifecycle, R3 helper, RawInput/Alt-Tab, horse, Synergy, config defaults and previous archives are preserved. **EXPERIMENTAL, source prepared; compile/install pending.** [Evidence, rollback and test criteria](V1_34_NATIVE_ACTIONS_STICK_ROLES.md).

## V1.33 verified Windows delivery and installation, 2026-10-14

CI38633252781 SUCCESS; source52ee7b4, numbered archive5e60284. Independent four-root-file ZIP/PE AMD64/build identity/direct URL verification; ZIP774986 bytes SHA256 EFF76B7514FFB69F957ABB7FE9688C5122F29C7F2EC917E4ED32B157940D040E. Normal authorized installation succeeded with game closed, binary destination hashes matched; INI AB2F95CD2AE49AD1706E1E9A420FEEDB1E416D25FCA157E83897554EF2C3E095 unchanged; old installation backup ../audit-v131/install-backup-before-v133-20261014-223512/. Installed README updated post-CI. Earlier review quota failure resolved through same approval route. main/archive and old archives unchanged. CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL; test equipment-wheel selection and center-camera mouse shots foot/horse before claiming gameplay validation. [Full delivery evidence](V1_33_CAMERA_CENTER_UI_PAD.md).

## V1.33 source change after V1.32 user test (historical pre-CI stage)

User validates retained TPS in three wheels and detection of the tested scene; equipment wheel selection regresses. Latest actual log proves native mouse target path FOOT1657/HORSE779, hit2349/miss87, native cinema19:42:33.669-19:42:39.653. V1.33 keeps the successful camera/lifecycle module identical. Adds tpsUiViewHeld to native aim suppression and live-look-actor gates: native UI receives original stick and look results; no direct XInput filter body changes. Mouse ray now uses camera center yaw/pitch, no cursor/aspect/FOV steering. [Full evidence, contracts and acceptance](V1_33_CAMERA_CENTER_UI_PAD.md).

256 flag combinations and 25 camera angles checked against source expressions; no local C++ compiler found. NOT BUILT / NOT INSTALLED / no gameplay fix claim. Workflow names future V1.33 four-file ZIP; no such artifact exists. Publication pending automatic review availability after prior push review failed due usage limit; cannot bypass that gate. V1.32 is last verified binary; preserve all archives and personal INI. Actual V1.32 installation is proven by user's new log, not by the earlier stopped installer.

## V1.32 Windows delivery verified, 2026-10-14

CI run 38608331116 SUCCESS, source cf613bb, numbered archive debug commit d842a55. ZIP 775037 bytes, exactly ASI/INI/dxgi.dll/README at root, PE AMD64 verified; direct raw download matched SHA256 C0EE1089862D61D37BC355724BA3DB2872346F520828BDFE4F56B709D2EA7124. CONSTRUIT / CONTROLE / EXPERIMENTAL, no in-game success claim. Local old installation and personal INI backed up; first install stopped before copies because game process was still running. Await closure and destination hashes. [Full delivery and native audit](V1_32_NATIVE_TARGET_CAMERA_OWNERSHIP.md). main/archive unchanged.

## V1.32 2026-10-14: one native mouse target path and explicit camera ownership

After user rejection of V1.31 mounted aim/cinematics, implement NativeTpsFeature: exact guarded game mouse helpers 0x5719A0/0x571E50, scoped deprojection only at 0x571EB8 return and matching controller, strict local physical mouse provider, original native collision and positive-range no-hit target. 2D consumers derive direction from the same 3D target; old mouse getter-yaw edits retired. Further disassembly proves BOTH helpers obtain mouse pixels: 0x1ACAA70 wraps GetMousePosition; previous device-discriminator hypothesis retracted. Native muzzle/projectile consumer and mounted coverage require actual runtime validation.

ForceCinematicMode native lifecycle 0x648B20 mirrored pass-through, IsGamePaused 0x188D940 read on live camera context, same-camera/generation copied TPS pose held in known UI. Explicit temporary shortcut priority retained per user choice; no INI/default change. Native module replaces old geometric cinematic suspension while unknown views still fail open. [Full ABI/evidence, scopes, risks, rollback and success criterion](V1_32_NATIVE_TARGET_CAMERA_OWNERSHIP.md). Windows CI/package pending; EXPERIMENTAL, not gameplay validated. XInput/RT/RB/R3, horse/gauges/shaders/RawInput source preserved.

## 2026-10-14: exact EXE targeting / independent native camera return paths

User V1.31 failure report is authoritative for gameplay: mouse aim and tested cinematic remain unresolved. Exact installed EXE now hash-verified; historical unavailable-EXE blocker superseded. New native mouse helper 0x5719A0 deprojects through 0x187F0B0, optionally uses a collision target, otherwise intersects with normal (0,0,1), permits negative intersection parameter, normalizes XY. Native helper 0x571E50 deprojects another screen input and traces 100000 units via 0x171E560. Callers around 0x57B940 / 0x57C130 dispatch by a virtual input-source value; mouse/gamepad ownership and real weapon coverage require runtime proof. This is actual native evidence beyond the obsolete 2D look getter; not yet a projectile fix. [Full log findings, RVAs, limits, next steps and acceptance](V1_31_NATIVE_TARGETING_LIFECYCLE_AUDIT.md).

Latest log: zero cinematic ENTER; 31ms SHORT_CUT and 15/16ms horse transitions; native camera return also occurs from immediate stale-pose rejection or explicit shared E/R3 temporary view, independently of cinematic classification. Preserve fail-open native view until native UI/sequence ownership is known. Documentation-only, debug only, no build/version/default/input change; prior ZIPs retained. Existing V1.31 CI rechecked success; package integrity checked separately from runtime. No additional experimental hook installed.

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

## V1.26, 2026-10-14: persistent RawInput re-arm versus fixed 8 retries

V1.25 user log confirms `WM_INPUT relative packets=46193`, fallback=324, 8522 physical cursor warps avoided, with only 6 fallback edge recenters. Process mouse registration was **removed repeatedly** during the session: 8/8 recovery attempts succeeded, the total cap was exhausted, the final registration state was `0` (missing), thus the next removal would not be repaired. V1.26 replaces the global eight-attempt limit by a **2,000 ms minimum re-arm interval**, with counters `throttled`, `recoveredAfterOldCap`. Still only re-arm when no registered native mouse owner exists, in foreground live TPS; no registration is stolen and no cursor hide is reintroduced. Resource/boot/cinematic investigation: Synergy 35/35 positive gains boosted and positive native caller RVA `0x67EADC`; Wrath candidate +2 x3 without any native AddWrath; Ultimate native AddChaos zero. Warning records stable active IDs `0x1C99E`, `0x1CB7E`; 3rd `0x1CB6C` inactive; are **not proven visible screen identifiers**. Camera transition is not native LevelSequence identity and R3 fallback remains. Full audit `docs/V1_26_PERSISTENT_RAWINPUT_AND_AUDIT.md`; pending Windows CI and user test.

## V1.25 CI and permanent ZIP publication verified (2026-10-14)

GitHub Actions [run 38542601698](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38542601698): **SUCCESS**, including Windows x64 CMake configuration, Release compilation, exact-four-root-files archive inspection, upload and permanent `debug` push. [Permanent V1.25 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.25_RELATIVE_RAW_RECOVERY_TEST.zip): **756,018 bytes**, Git blob SHA-1 `16dc0637d7ac86c130d8d2d7055af02b79bed801`. V1.24 size 753,948 bytes and V1.23 size 752,055 bytes remain untouched. No modifications to `main` (2bc3122c1ae07b7333798462f85c1d9fd3a739f7) or `archive` (10e0f220b85022bc9269e0cb9390a811a3b1cf89). CI success verifies build and packaging only, **not** real in-game relative-input continuity or a working warning skip. Validation remains pending.

## V1.25 RawInput recovery, gauge callers, warning candidate identities, camera transition audit

User V1.24 feedback: mouse primary/secondary fire WORK, but infinite camera rotation/regain at screen edge broke again. RawInput packets fell 17,154 (V1.23) to 67, WM_MOUSEMOVE fallback 1,213 to 2,353; edge warp 37 to 49; zero warp failures. Never assume the game registration remains after startup. The old GetRegisteredRawInputDevices(NULL) also treated the expected ERROR_INSUFFICIENT_BUFFER size query as fatal. V1.25 conservatively polls the registration each ~1.3 sec, preserves any native/other HWND owner, re-arms only when zero mouse registrations exist (max 8), logs RAW_OWNER, RAW_REARM, RAW_STALL. No edge warp while RawInput is fresh; V1.23 warping remains **fallback only**, not fully removed yet. User V1.24 mouse fire bindings preserved. Additional read-only Synergy (+Wrath/Chaos if invoked) native callers, warning candidate index-like fields and camera transition classification. Full evidence and safety rules: docs/V1_25_RAW_RELATIVE_AND_AUDITS.md. Status EXPERIMENTAL until gameplay test; main/archive frozen.

## V1.24 Windows x64 build and publication verified (2026-10-14)

GitHub Actions [run 38537645671](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38537645671) completed **SUCCESS**: configure, Release compile, four-root-file ZIP verification, artifact upload, and permanent `debug` archive publication. [Download V1.24 cumulative TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.24_TPS_MOUSE_FIRE_AUDIT_TEST.zip): **753,948 bytes**, Git blob SHA-1 `4048d7eaa1d23ef35b4560ed737065ebfc96c937`. Archived V1.23 ZIP remains present: 752,055 bytes, SHA-1 `567ad7f7854f515ae4f3e091dd87e4a5c65dc013`. The workflow checked exactly `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, and `README.md` at ZIP root. `main` and `archive` branch heads are unchanged. **Windows compilation is not in-game verification**: native secondary shot direction and alternate button remapping remain EXPERIMENTAL awaiting the user's test.

## V1.24 code and log audit 2026-10-14

User V1.23 feedback confirms smooth unlimited mouse orbit via 37 edge recaptures, zero errors. Synergy 27 positive gains / 27 boosted, Wrath +9 read-only candidate (4+1+4), AddWrath=0 and AddChaos=0, cinematic heuristic enters 2/exits 1 (identity still unverified). V1.24 scopes primary/secondary Win32 mouse button mapping to focused live TPS and adds both to the local player native Camera Forward look vector/rotation hook; leaves validated pad/XInput/radial/R3 unchanged. Main and archive remain frozen. New INI revision 2140 stores MousePrimaryButton VK1 and MouseSecondaryButton VK2 by default, with overlay selection. Source research adds positive local Synergy timestamp read-only for Wrath correlation and logs early StartupScreens 8-byte raw payload samples; no resource writes or startup bypass. **This is experimental until CI and in-game firing test.** Full note: docs/V1_24_TPS_FIRE_GAUGES_WARNINGS.md.


## Windows x64 CI and published archive verified (2026-10-14)

GitHub Actions [run 38534464159](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38534464159): **SUCCESS** for configure, Release build, four-root-file packaging, artifact upload and permanent `debug` ZIP publication. [V1.23 permanent ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.23_TPS_EDGE_RECAPTURE_TEST.zip): 752055 bytes, Git blob SHA-1 `567ad7f7854f515ae4f3e091dd87e4a5c65dc013`. The older V1.22 remains exactly 750900 bytes and Git blob SHA-1 `07acd04899dc160d8fbb03a452ce9d362804b9d9`. Windows compilation does not establish gameplay success. `main` and `archive` branch tips unchanged. User test still pending.

---
# V1.23 - TPS relative mouse edge recapture (2026-10-14)

Branch `debug`, base V1.22. EXPERIMENTAL, game validation pending. User reports V1.22 functions all work except mouse yaw stops at window edge. V1.22 log: RAWINPUT registered=1, 687 physical packets, 77 at border, fallback=1354, mouse camera frames=1140, registrationErrors=0.

**Targeted change:** Retain V1.20 RawInput and original input passthrough; if real recent mouse movement (<180ms) reaches 10px of client edge, reposition OS cursor to client center with >=350ms cooldown. The resulting artificial WM_MOUSEMOVE is excluded from mod camera deltas. Only focused, fresh TPS gameplay; no overlay, equipment wheel, TPS temporary vanilla/cutscene view, or Alt-Tab. This is an explicit limited exception to the former no-pointer-warp policy, NOT a reticle hide function. Zero SetCursor / game UI visibility hooks; F4=None. No changes to gauges, intro, warnings, cinematics, controller, horse, updater, menu or default INI.

**Verify:** rotate TPS several full circles both directions at each edge, ensure no camera snap; note any visible pointer recenter. Test Insert menu, game wheel, controller R3, horse, Alt-Tab and cinematic transitions for zero recenter. Log includes `TPS V1.23 EDGE_RECAPTURE` counters. If native cursor/menu regresses, revert to preserved V1.22. Compile Windows x64, package exactly dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md at ZIP root. Next V1.24 after user feedback.

## V1.22 2026-10-13 - retire reticle hiding, preserve relative TPS mouse

User reports V1.21 cross cursor and broken mouse rotation after Hide Reticle. Native `SetCursor(NULL)` and `UAirshipUIManager::IsCursorVisible` detours existed even with manual HideReticle OFF due TPS-auto policy. **Both hook implementations and installations are now deleted**, along with cursor blank/restoration/probe, menu, config and F4 action (defaults F4=None). Native game cursor remains untouched by ASI except normal ImGui overlay pointer. Retire old INI keys on explicit save only. Gameplay independent options and all other defaults unmodified.

V1.22 Windows x64 Actions run 38461117435 SUCCESS; 4-root ZIP on debug; V1.21 original archive SHA preserved. Cutscene native-view heuristic 3 enters / 2 exits / 376 invalid-actor samples, identity unproven. Wrath/Ultimate Add callbacks remain absent; Synergy V1.20 59/59 gain boosts confirmed. Skip Warnings prepared flags 1,1,0 but `drawVerified=0`, risky bypass remains disabled. [V1.22 full audit](V1_22_REMOVE_RETICLE_FEATURE.md).

## V1.21 single reticle control + automatic menu-only TPS policy

Remove conflicting controls after user's V1.20 session (global Hide Reticle ON/OFF + TPS pointer OFF/ON); preserve manual F4 Hide Reticle in Camera > General instead of Gameplay/HUD. Code-managed TPS pointer suppression only during active game view, not overlay or radial menu. Deprecated ShowTPSMousePointer removed from Store and runtime and new INI; explicit save deletes stale key. No unrelated default changes, camera/mouse code unchanged. See [V1.21 details](V1_21_RETICLE_MENU_ONLY.md).

## V1.20 raw relative mouse and cutscene/gauge study (2026-10-13)

V1.19 TPS mouse stops at physical screen edge because WM_MOUSEMOVE coordinates stop; V1.20 conditionally registers mouse RawInput on existing game HWND if no game mouse registration, preserving legacy/RawInput native dispatch and LMB Camera Forward, no cursor warp. Logs true raw mouse vs HID and movement at edge. Existing native camera cutscene heuristic unchanged; bounded ENTER/EXIT and rejection metadata for later review. Wrath candidate increments, decreases and recent native local-damage correlation are read-only; no new gain hooks or data mutations. Synergy user-validated, current defaults and in-game updater retained. [V1.20 test](V1_20_RELATIVE_MOUSE_CUTSCENE_GAUGES.md).

## V1.19 mouse TPS unified Camera Forward, user defaults ONLY (2026-10-13)

No formerly rejected V1.17 cursor hack. Default ThirdPerson=1, ShowTPSMousePointer=1 in Store, runtime and INI, with TPS-only runtime pointer guard. Mouse control needs no UI option: native mouse RAWINPUT or WM_MOUSEMOVE fallback (not swallowed; no synthetic input) feeds existing cameraOrbit yaw/pitch, and LMB native look gets pad-equivalent TPS Camera Forward vector/rotation. Real shooting path remains to test. Skip Warnings control visually restored but disabled as unsafe, with native setup preserved. User's last 1.18 log includes no gauge positive events and mouse 413/10 calls. Cross bug MUST be retested with ShowTPSMousePointer=1, not hidden. [V1.19 note](V1_19_TPS_MOUSE_FORWARD.md).

## V1.18 canonical V1.15 rollback after V1.17 TPS regression (2026-10-13)

No recycled rejected mouse/cursor patches: V1.15 source restored for TPS/reticle, config, runtime, DXGI, overlay, updater. User rejects V1.16 and V1.17. V1.18 includes only passive mouse message class counts, Wrath candidate classification correlated with charged input and prepared-warning-record flags. No camera movement, no additional Synergy multiplier changes, no startup loop short-circuits. Existing user INI is preserved and may require manual rollback of TPS/pointer values. Skip Logos remains selectable as in V1.15; controlled cold boot OFF/ON requested. [Full recovery and tests](V1_18_CLEAN_RECOVERY_RESEARCH.md).

## V1.17 targeted stock OS cross cursor repair (2026-10-13)

Despite preserved MoviePlayer attach and safe V0.52 exact two-logo substitution, user V1.16 reports cross cursor regression. V1.16 first launches TPS=1 and ShowTPSMousePointer=1 as defaults, so previously hidden native OS cursor could be exposed. New V1.17 stock IDC_CROSS -> IDC_ARROW substitution is gated to focused live TPS gameplay, no overlay/HideReticle/wheel/cutscene, and bounded diagnostics mark actual source. Not a global crosshair/UI override and not proof this caused bug. No startup changes. Details [V1.17](V1_17_CROSS_CURSOR_REPAIR.md).

## V1.16 (2026-10-13): defaults and rejected startup patch comparison

New factory/shipped/fallback TPS=1 and ShowTPSMousePointer=1; UI label now says ON by default. Existing user saved INIs preserved by updater. See [V1.16 comparison](V1_16_TPS_DEFAULTS_STARTUP_COMPARISON.md) for 0.19A startup-module shortcut (also bypassed intro), V0.19B zero movies (also bypassed intro), V0.19C 19-byte MoviePlayer attachment skip (cross cursor regression), V0.44 unsafe observer crash, and current V0.52 targeted logo path preservation. Skip Warning quarantined: 3x0x40 definitions read-only, no justified evidence that bypassing initialization loop suppresses two actual warning frames. New mouse camera orbit remains optional experimental future build: previous V0.98 RAW-based camera worked while not shooting, but mouse fire was unreliable. No startup or input hook modified in V1.16.

## V1.15 bounded resource gain audit (2026-10-13)

The V1.14U logfile shows `SynergyLocal=317 positive=113` and additive `observed=317 local=317 boosted=113`. This is consistent: 113 positive calls were multiplied, 204 did not produce positive native additive gain. First two additive calls had zero factor B; no lost gains proven. Current detour multiplies factor C in native A*B*C, i.e., amount received, not object or absolute gauge; remains unchanged. To determine collectible gem vs enemy kill path, add read-only positive/zero/negative/invalid factor sampling and marker for additive-origin vs direct `SetCurrentSynergy`. Wrath/Chaos hook calls stay zero and need further tracing. Strict build numbering V1.15 onward: V1.16, V1.17, no letters / repeated numbering. [Details](V1_15_RESOURCE_GAINS_AUDIT.md).

## V1.14U UI-fix: TPS option grouping + About factory reset modal (2026-10-13)

User reports Show Mouse Pointer in TPS is currently in Gameplay/HUD, but belongs in Camera/Third Person. Relocated checkbox and explanation to Camera > Third Person camera after the enable toggle. Key `[Features] ShowTPSMousePointer=0` remains OFF by default; TPS suppression and Hide Reticle priority unchanged.

**Confirmed code fault**: `ImGui::OpenPopup("Reset INI to Factory Defaults?")` occurred inside `BeginTabItem("About")` but `BeginPopupModal(...)` was outside `EndTabBar()`, so ImGui ID stacks differed and modal never appeared. The uploaded V1.14U log (18:39:38–18:50:41) contains four `INI saved` events but NO `INI factory reset` nor Reset action despite user click, consistent with the modal never being reached. The fix puts BeginPopupModal within the same About tab before EndTabItem, with a persistent SUCCESS/FAILED/CANCELLED message immediately below button plus LAST ACTION; explicit log entries for button/confirm/result/cancel. Existing full factory reset method and dated .bak backup logic preserved.

Other log highlights (not changed): Synergy 317 observed/113 boosted; Wrath/Chaos Add 0; TPS forward foot 2337/horse 0. LB+Y native 32, writes 0. Mod x64 hooks successfully loaded. User still needs to validate in game.

This is cumulative V1.14U with embedded updater unchanged; new ZIP name DarksidersGenesis_V1.14_ZZ_TPS_RESET_FEEDBACK_TEST.zip sorts after earlier V1.14 Z archive so updater identifies it as latest branch ZIP, without overwriting earlier test builds.

## V1.14U ASI updater integrated (2026-10-13)

Experimental updates interface inside ASI: Beta=0 main / Beta=1 debug, Check for Updates on background WinHTTP, Install on Game Exit and Restore Previous with dialogs. Post-exit helper compiled with x64 csc, embedded as RCDATA resource 101. After user confirms, helper extracted to LOCALAPPDATA and launched detached to wait for process exit; checks supported EXE and GitHub package blob, verifies four root ZIP files, preserves personal INI, backs up and restores binaries. No fifth root ZIP file. Windows Actions [38430137501](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38430137501) passed; archive `DarksidersGenesis_V1.14_Z_ASI_UPDATER_TEST.zip` 745505 bytes published. In-game behavior not yet validated. [Full note](V1_14U_INMENU_UPDATER.md).

## V1.14 TPS cursor opt-in, Info reset consolidation (2026-10-13)

Only new cursor behavior is the `ShowTPSMousePointer=0` config toggle. Both native and Win32 TPS automatic cursor hiding check the inverse toggle; Hide Reticle retains priority. The menu offers a checkbox under HUD/aiming effects, OFF by default. V1.13 reset duplicates were identical entry points to one method; consolidated into About and kept per-control Defaults. User V1.13 log: Synergy 122/60 positive boosts, Wrath/Chaos hooks zero, candidate 4 rises/2 falls; 610 TPS foot forward injections, no horse or charged-shot test. Unexpected BaseJuice diagnostic and ScaleType integer warrant separate investigation. See [V1.14 note](V1_14_TPS_POINTER_INFO_RESET.md).

# Darksiders Genesis Enhanced PC Mod - Technical Notebook

This file preserves the complete historical README / development notebook that
accumulated through the V0.1 -> V0.16 experimental cycle.

It is intentionally archival. For installation and current feature status, use
the repository root `README.md`.

---

# Darksiders Genesis Enhanced PC Mod

> Dedicated repository for the ASI/DXGI gameplay-enhancement mod.
>
> The separate pre-order mount unlock project remains in
> `DeadneM/Darksiders-Genesis-Preorder-Mounts-PC`.

This file is the **authoritative development notebook** for the experimental
Darksiders Genesis ASI mod. It is intentionally cumulative so development can
be resumed later without reconstructing decisions from chat history.

> Public `main` currently tracks the validated **V0.13B ASI base**.
> **V0.14A is rejected** because it failed to identify the mounted horse.
> **V0.14B is rejected for horse-speed control** because user testing produced
> no perceptible horse acceleration.
> **V0.14C is rejected as a runtime test** because its IsHorseActive resolver
> matched zero functions at startup, so none of the direct horse movement code
> ever executed. V0.14D removes that resolver dependency while keeping the
> still-unvalidated direct MaxWalkSpeed / MaxAcceleration experiment.

---

## Project goal

Build a small, robust Win64 ASI mod for **Darksiders Genesis** with:

- a local `dxgi.dll` ASI loader;
- an in-game overlay inspired by the validated Q Protocol UI model;
- a **rebindable menu key** to open/close the overlay, default **Insert**;
- full mouse interaction while the overlay is visible;
- F1-F12 fixed hotkey slots whose actions are reconfigurable;
- one INI as the only configuration source;
- fail-open behavior if the renderer/overlay cannot initialize;
- no game-file edits for ASI functionality.

The README must continue to record:

- each build;
- exact technical changes;
- test results;
- validated/rejected bases;
- known issues;
- executable compatibility information;
- current TODO;
- next investigation.

---

## Target executable audit

File supplied for the first ASI pass:

```text
DarksidersGenesis-Win64-Shipping.exe
Size:    62,113,280 bytes
SHA-256: 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54
Machine: x86-64 / PE32+
```

Relevant imports confirmed directly from the executable:

```text
dxgi.dll
  CreateDXGIFactory
  CreateDXGIFactory1

d3d11.dll
d3d9.dll
```

The executable therefore provides a clean path for:

```text
game
  -> local dxgi.dll proxy
  -> real Windows System32\dxgi.dll
  -> DarksidersGenesisMod.asi
  -> D3D11 Present / ResizeBuffers hook
  -> Dear ImGui overlay
```

UE4 / MoviePlayer strings confirmed in the executable include:

```text
StartupMovies
WindowsMoviePlayer
MoviePlayer
CreateMoviePlayer
GetMoviePlayer()->SetupLoadingScreenFromIni
DefaultGameMoviePlayer.cpp
```

This gives the Skip Intro feature a real engine-native investigation path.

---

## V0.3A — Native Toggle HUD

**Status: TEST CANDIDATE**

V0.3A is cumulative from V0.2A:

- validated V0.1 DXGI / D3D11 / ImGui overlay foundation;
- rebindable overlay menu key;
- first real gameplay feature: native Toggle HUD.

### HUD binary audit

The supplied target executable contains the game-specific console variable:

```text
ui.HideHud
>0 Hides all HUD completely.
Force HUD to be hidden.
<=0: default HUD behavior
1: HUD always hidden
```

Static audit of the validated executable:

```text
ui.HideHud UTF-16 string RVA : 0x26F3F78
registration name xref RVA    : 0x000E779A
CVar data-slot RVA            : 0x03803748
native boolean getter RVA     : 0x0063AB50
```

Getter body:

```asm
mov rax, [rip + ui.HideHud_data_slot]
cmp dword ptr [rax], 0
setne al
ret
```

### V0.3A implementation

The ASI **does not overwrite the CVar value**.

At runtime it:

1. finds the UTF-16 `ui.HideHud` name inside `.rdata`;
2. finds the unique RIP-relative registration reference in `.text`;
3. follows the validated registration layout to the CVar data slot;
4. locates the unique native boolean getter using that exact data slot;
5. hooks only the getter with MinHook.

Return policy:

```text
nativeHidden || modHidden
```

This means the mod can request HUD hiding without cancelling a native game
request to hide the HUD.

Runtime behavior:

- F1 defaults to Toggle HUD;
- first press: HUD hidden;
- next press: HUD visible again;
- overlay -> Features -> HUD Hidden can also change the runtime state;
- runtime HUD state starts visible on every launch;
- `[Features] ToggleHUD=1` enables the feature by default;
- if resolution fails, Toggle HUD becomes unavailable while the rest of the
  ASI stays fail-open.

### V0.2A — Rebindable menu key / safe rollback

**Status: TEST CANDIDATE**

V0.2 keeps the validated V0.1 renderer/input foundation and makes the overlay
open/close key fully rebindable at runtime.

Behavior:

- default remains `Insert`;
- `General -> Rebind Menu Key` waits for the next keyboard key;
- `Esc` cancels capture;
- the new key is saved immediately to `[Overlay] MenuKey`;
- Save / Reload / Reset Defaults all understand the menu binding;
- Reset Defaults restores `Insert`;
- arbitrary keyboard keys are persisted as a readable token when known, or
  `VK_XX` for less common virtual keys;
- the captured key is debounced so the same press does not instantly close the menu.

### V0.1 — Overlay foundation

**Status: VALIDATED IN GAME**

### First successful Windows build

GitHub Actions run:

```text
37231616133
```

Build result: **PASS**

Artifacts:

```text
dxgi.dll
  SHA-256 513262d7212e4c4fe907ab83e8794704b8970dc2b1e97137b152fafd3abb16f6

DarksidersGenesisMod.asi
  SHA-256 ec9b951727edecbeef73c984068107c06d183d078370b412889bad402f73e1e5

DarksidersGenesis_ASI_V0.1_TEST.zip
  SHA-256 7cf4fb36e1e72a1ad759a97a9ffd418757bd9dd0d5795b09a6c4627e6d7ced1d
```

Both compiled binaries were independently checked as **PE32+ x86-64 DLLs**.

### Build-system notes

The first CI pass exposed two build-environment details which are now documented:

- GitHub `windows-latest` moved to the Windows 2025 / Visual Studio 2026 image,
  so the workflow now uses the `Visual Studio 18 2026` CMake generator.
- MinHook's Windows x64 output uses its configuration postfix
  (`minhook.x64.lib`); CMake target resolution was hardened so the ASI links
  the actual MinHook target instead of guessing a library filename.

The source itself then compiled and linked successfully.

### Purpose

V0.1 is deliberately a foundation build.

It validates:

1. DXGI proxy loading;
2. automatic ASI discovery/loading;
3. D3D11 swap-chain discovery;
4. Present + ResizeBuffers hooks;
5. Dear ImGui initialization on the real game swap chain;
6. validated menu toggle;
7. mouse input/cursor access;
8. suppression of gameplay F1-F12 input while the menu is open;
9. F1-F12 action mapping;
10. INI Save / Reload / Reset Defaults;
11. clean logging to `DarksidersGenesisMod.log`.

V0.1 does **not** pretend that gameplay features are already implemented.
Every requested feature is enabled by default in configuration, but is clearly
shown as **Hook pending** until a native implementation is identified and tested.

### Overlay layout

Tabs:

```text
General
Features
Hotkeys
About
```

Hotkey architecture follows Q Protocol's clean model:

```text
F1-F12 fixed key
      -> selected Action
      -> shared feature implementation
```

No duplicate hotkey-specific gameplay code should ever be added.

### Authoritative default hotkeys

| Key | Action |
|---|---|
| F1 | Toggle HUD |
| F2 | Movement Speed |
| F3 | Action Recovery |
| F4 | Skip Intro Videos |
| F5 | Third Person |
| F6-F12 | None |

### Authoritative default feature policy

All requested options are **enabled by default**:

```ini
ToggleHUD=1
MovementSpeed=1
ActionRecovery=1
SkipIntroVideos=1
ThirdPerson=1
```

Initial tuning placeholders stored in the INI:

```ini
MovementSpeedMultiplier=1.150
ActionRecoveryMultiplier=2.000
```

These values are intentionally inert until their native hooks are implemented.

---

## Current TODO

### 1. Toggle HUD

Goal:

- toggle the actual in-game HUD without disabling unrelated rendering;
- preserve menus/overlay;
- no permanent asset edits.

Status: **implemented in V0.3A through native `ui.HideHud` getter hook; awaiting in-game validation**.

### 2. Movement Speed

Goal:

- adjustable movement speed;
- user-facing multiplier in overlay/INI;
- avoid globally changing game time;
- do not affect enemies/NPCs;
- do not consume the separate horse-speed roadmap item.

Default option state: **Enabled**.

Initial stored test value: **1.15x**.

Status: **V0.4A rejected; corrected implementation in V0.5B awaiting validation**.

Implementation notes:

V0.4A originally resolved `AMayhemCharacter::GetMaxSpeed`, but user testing
showed no movement-speed effect. That helper only queries the movement component
and is not the physics virtual consumed by CharacterMovement.

V0.5B corrects the target:

- `UMayhemCharacterMovementComponent` reflected object size: `0x850`;
- base `UCharacterMovementComponent::GetMaxSpeed` virtual slot: `+0x3D0`;
- Mayhem override RVA in the audited executable: `0x56FBE0`;
- the signature is unique in `.text`;
- the hook checks `CharacterOwner` at `+0x190`;
- applies only when `APawn::IsLocallyControlled` is true;
- applies only in `Walking` / `NavWalking` movement modes.

### 3. Action Recovery Speed

Observed behavior:

After some actions there is a short interval where the character is visibly
finished but cannot move yet.

Goal:

- shorten/remove only this post-action movement lock;
- do not indiscriminately speed up all animations;
- expose a tuning control if the engine path supports a scalar.

Default option state: **Enabled**.

Initial stored test value: **2.00x**.

Status: **implemented in V0.5A; awaiting in-game validation**.

Native audit result:

- owner: `UMayhemPlayerAbilityComponent`;
- reflected field: `MoveInterruptDelaySec`;
- field offset: `+0x110`;
- runtime elapsed timer used by the same gate: `+0x114`;
- `ECharacterActions::MOVE = 0x1D`;
- native gate RVA in the audited executable: `0x5B3150`;
- signature match count: exactly **1**.

The native logic explicitly rejects `MOVE` while:

```text
MoveInterruptDelaySec > elapsed timer
```

V0.5A therefore scales only this delay during the native check.

### 4. Skip Intro Cinematic

Goal:

- skip the game's opening story cinematic through the ASI;
- do not delete/rename original game files;
- leave normal loading/movie systems intact.

Implementation:

```text
g.PlayIntroCinematicOnBoot = 0
```

Default option state: **Enabled**.

Status: **VALIDATED IN GAME in V0.13B**. This skips the game's opening
cinematic, but it does **not** skip the developer/publisher startup logos.

### 4B. Skip Logos

Goal:

- skip startup company/developer/publisher logos;
- arrive directly at the title / **Press Any Key** screen;
- keep this independent from Skip Intro Cinematic;
- do not rename or delete original game files.

Known movie targets from the retail content layout:

```text
ProjectMayhem/Content/Movies/THQ_LogoBasic.mp4
ProjectMayhem/Content/Movies/AS_LogoBasic.mp4
```

The existing `g.PlayIntroCinematicOnBoot` control does not affect these two
startup movies. The ASI therefore needs a separate MoviePlayer/file-selection
path that suppresses only these logo movies while leaving normal cutscenes and
loading movies intact.

Status: **pending dedicated startup-logo hook**.

### 5. Third Person

Goal:

- switch gameplay to a usable third-person camera;
- preserve aiming/collision/control behavior as much as possible;
- ideally allow runtime toggle.

Default option state: **Enabled**.

Status: **pending camera/player-controller audit**.

### 6. Pistol Damage

Goal:

- increase pistol damage;
- ideally expose a multiplier in the overlay/INI;
- avoid affecting unrelated weapon classes.

Status: **pending weapon-damage audit**.

### 7. Melee Damage

Goal:

- increase melee damage;
- keep the modifier separate from firearm damage;
- ideally expose a multiplier in the overlay/INI.

Status: **pending melee-damage audit**.

### 8. Jump Height

Goal:

- increase jump height;
- expose an adjustable multiplier if the native movement path allows it;
- preserve reliable landing and collision behavior.

Status: **pending character-movement audit**.

### 9. Horse Speed / Sprint

Goal:

- increase normal horse movement speed;
- increase horse sprint speed;
- keep normal speed and sprint tunable independently if possible.

Status: **pending mount movement/sprint audit**.

### 9B. Horse Dash / Sprint Duration

Goal:

- increase how long the horse dash/sprint can remain active;
- keep duration separate from horse speed;
- expose an adjustable duration multiplier if the native mount ability supports it;
- preserve stamina/cooldown behavior unless explicitly tuned by a later feature.

Status: **pending horse dash/sprint ability audit**.

### 10. FOV

Goal:

- expose an adjustable gameplay FOV;
- keep it runtime-configurable from the overlay/INI;
- preserve camera transitions and special camera states.

Status: **pending camera/FOV audit**.

### 10B. Character Zoom / Camera Distance

Goal:

- allow runtime zoom in and zoom out around the controlled character;
- expose camera distance separately from FOV;
- preserve combat, aiming and special camera transitions;
- provide slider and manual numeric input when the native camera path is identified.

Status: **pending camera/player-controller audit**.

### 10C. Camera Angle

Goal:

- allow runtime adjustment of the gameplay camera angle;
- target pitch/elevation first, with yaw/orbit adjustment if the native camera path safely supports it;
- keep angle control independent from FOV and character zoom;
- preserve scripted and special camera states.

Status: **pending camera/player-controller audit**.

### 11. Hotstreak Charge

Goal:

- increase Hotstreak charge/gain rate;
- expose a configurable multiplier if the underlying system supports it;
- avoid changing unrelated resource/ability charge systems.

Status: **pending Hotstreak system audit**.

---

## Design rules

1. **Fail open.** Overlay failure must not stop the game from running.
2. **No fake success.** UI state and real gameplay-hook state are separate.
3. **INI is authoritative.** No hidden secondary config database.
4. **One feature implementation.** Hotkeys and overlay call the same primitive.
5. **No executable replacement.** ASI development should not require shipping a
   modified game EXE.
6. **Keep public validated work safe.** The existing pre-order mounts release is
   untouched while this branch is experimental.
7. **Journal every build here.** Never rely on chat history as the only source.
8. **Per-session logging.** `DarksidersGenesisMod.log` must be truncated at
   every game launch so it contains only the current session. Do not use a
   cumulative append-across-launches log.

---

## Test checklist for V0.1

Install next to the game executable:

```text
dxgi.dll
DarksidersGenesisMod.asi
DarksidersGenesisMod.ini
README.md
```

Then verify:

- [x] Game boots normally.
- [ ] `DarksidersGenesisMod.log` is created.
- [ ] Log says D3D11 hooks were installed.
- [ ] Log says ImGui overlay is READY.
- [x] Insert opens the overlay.
- [x] Insert closes the overlay.
- [x] Mouse moves and clicks correctly inside the overlay.
- [ ] Game does not react to menu clicks.
- [x] F1-F12 can be reassigned from Hotkeys.
- [ ] Save persists settings after restart.
- [ ] Reload restores INI values.
- [ ] Reset Defaults restores F1-F5 mapping and all feature defaults.
- [ ] Alt-Tab does not break the overlay.
- [ ] Resolution/fullscreen changes do not break the overlay.
- [ ] Game exits normally.

---

## Build history

### V0.1

- New DXGI proxy loader.
- Loads `*.asi` from the game executable directory.
- Forwards the game's confirmed `CreateDXGIFactory` and
  `CreateDXGIFactory1` calls to the real System32 DXGI.
- Includes compatibility forwarders for common newer DXGI exports.
- New D3D11 overlay core using MinHook + Dear ImGui.
- Insert menu toggle.
- Mouse/keyboard capture while open.
- F1-F12 Q Protocol-style action map.
- INI persistence.
- Requested features all enabled by default.
- Gameplay hooks deliberately left pending for the first foundation test.
- Startup movie strings audited and recorded.

**Compilation:** PASS.

**In-game validation:** PASS — user confirmed V0.1 foundation works perfectly.

**First successful build run:** `37231616133`.

**Canonical V0.1-test binary hashes before README-only rebuild:**

```text
dxgi.dll                  513262d7212e4c4fe907ab83e8794704b8970dc2b1e97137b152fafd3abb16f6
DarksidersGenesisMod.asi  ec9b951727edecbeef73c984068107c06d183d078370b412889bad402f73e1e5
```


### V0.2

- V0.1 becomes the validated overlay foundation.
- Adds runtime menu-key rebinding from the General tab.
- Default key remains Insert.
- Escape cancels a pending key capture.
- New binding is written immediately to `[Overlay] MenuKey`.
- INI parser accepts named common keys, F1-F24, letters/numbers and `VK_XX` fallback tokens.
- Reset Defaults restores Insert.
- Menu title shows the current open/close key instead of hard-coding Insert.
- Captured key is debounced to prevent instant menu closure on the capture press.
- Gameplay features remain enabled by default and continue to be developed independently.

**Validation:** awaiting V0.2 in-game test.


### V0.3A

- Cumulative from V0.2A.
- Added binary audit for the game-specific `ui.HideHud` CVar.
- Added semantic resolver anchored to the UTF-16 CVar name.
- No raw fixed RVA is used as the runtime resolver.
- Added native getter hook instead of writing the CVar.
- F1 now toggles runtime HUD hidden/visible state.
- Overlay Features tab exposes current HUD Hidden runtime state.
- Native game HUD-hide state is preserved with `nativeHidden || modHidden`.
- Reset Defaults restores the mod HUD runtime state to visible.
- Menu-key rebinding from V0.2A is preserved.
- Remaining Movement Speed / Action Recovery / Skip Intro / Third Person hooks
  are still pending.

**Validation:** awaiting first V0.3A in-game test.


## V0.4A — On-foot Movement Speed

**Status: TEST CANDIDATE**

Cumulative from V0.3A.

### Binary audit

The game exposes the relevant Mayhem functions through UE4 reflection:

```text
GetMaxSpeed
GetDefaultMaxSpeed
GetMayhemMovementComponent
AddSpeedLimitOverride
StopOverridingMaxSpeed
ESpeedModType::ADD_VALUE
ESpeedModType::ADD_MOD
ESpeedModType::MULTIPLY_MOD
```

For the audited executable, the `GetMaxSpeed` Blueprint exec wrapper calls the
native helper at RVA `0x56FBC0`. That helper retrieves the Mayhem movement
component from character offset `+0xA20` and dispatches its virtual max-speed
query.

Direct gameplay callers of this helper were also found outside the reflection
wrapper, confirming that it participates in real movement calculations.

### Player-only filtering

The feature must not speed enemies or NPCs.

The hook therefore applies the multiplier only when the character's inherited
`APawn::IsLocallyControlled` virtual returns true.

The audited UE4 wrapper dispatches this virtual through vtable offset:

```text
0x680
```

### Horse exclusion

Horse speed is a separate TODO and must remain independently tunable.

`AMayhemPlayerCharacter::IsHorseActive` was audited and checks:

```asm
cmp qword ptr [rcx + 0xE70], 0
setne al
```

V0.4A therefore leaves `GetMaxSpeed` unchanged while a horse mount is active.

### Runtime behavior

- Movement Speed is enabled by default.
- Default multiplier: **1.15x**.
- F2 toggles Movement Speed ON/OFF.
- Overlay slider range: **1.00x to 2.50x**.
- Slider changes save immediately to the INI.
- No global TimeScale modification.
- No enemy/NPC speed modification.
- No horse-speed modification.
- Resolver failure is fail-open and leaves vanilla movement untouched.

**Validation:** **REJECTED**. User confirmed Movement Speed produced no effect.

Root cause found during V0.5B audit: V0.4A hooked the character-side query helper
instead of the actual CharacterMovement physics virtual.


## V0.5A — Action Recovery / MOVE interrupt delay

**Status: TEST CANDIDATE**

Cumulative from V0.4A.

### What was found

The delay reported by the user is represented directly in
`UMayhemPlayerAbilityComponent` as:

```text
MoveInterruptDelaySec
```

The generated reflection data confirms:

```text
UMayhemPlayerAbilityComponent object size: 0x118
MoveInterruptDelaySec offset:              0x110
runtime elapsed timer offset:              0x114
```

A unique native action-gate function was then identified at audited RVA:

```text
0x5B3150
```

Its opening logic is equivalent to:

```text
if action == ECharacterActions::MOVE (0x1D):
    if MoveInterruptDelaySec > elapsed:
        reject movement
```

This is the exact post-action movement lock targeted by the feature.

### V0.5A implementation

The mod does **not** speed up animations and does **not** change global
TimeScale.

For a MOVE action only:

```text
effectiveDelay = native MoveInterruptDelaySec / ActionRecoveryMultiplier
```

The ASI temporarily substitutes that effective value only while the original
native action-gate function executes, then immediately restores the object's
original value.

This preserves every other native condition checked by the game.

### Runtime behavior

- Action Recovery is enabled by default.
- Default multiplier: **2.00x**.
- F3 toggles Action Recovery ON/OFF.
- Overlay slider range: **1.00x to 5.00x**.
- Slider changes are saved immediately.
- 2.00x means the MOVE lock lasts half as long.
- 5.00x means the MOVE lock lasts one fifth as long.
- No animation-speed change.
- No attack-speed change.
- No global TimeScale change.
- No permanent write to `MoveInterruptDelaySec`.
- Signature mismatch is fail-open and leaves vanilla behavior untouched.

### Additional input fix

V0.5A also fixes a hotkey-state issue inherited by Movement Speed:

- F2 can now re-enable Movement Speed after it has been toggled OFF.
- F3 can likewise toggle Action Recovery both OFF and back ON.
- Toggle HUD still respects its separate feature-enable checkbox.

**Compilation:** PASS.

**GitHub Actions run:** `37235986567`.

Compiled binary hashes:

```text
dxgi.dll
be75f8f2a12b1e44482ae5ffa76708e788eaddd64fdd591fff621b89a1e7277b

DarksidersGenesisMod.asi
10ee522195dd391da435249cdc9d4c1388de41c685db845a5bee7f1d8b7a6002
```

**Validation:** **REJECTED**. User testing confirmed that this implementation did not shorten the post-dodge recovery lock.


## V0.5B — Movement Speed physics-virtual fix

**Status: TEST CANDIDATE**

Cumulative from V0.5A.

### Why V0.4A failed

V0.4A hooked the character-side helper used to *query* movement speed. User
testing confirmed that changing its return value did not alter actual movement.

Binary re-audit of the movement-component vtable showed the real path:

```text
UCharacterMovementComponent::GetMaxSpeed
vtable slot: +0x3D0
```

The Mayhem movement component overrides this slot with:

```text
UMayhemCharacterMovementComponent::GetMaxSpeed
audited RVA: 0x56FBE0
class size:  0x850
```

This function is the layer that combines:

- native movement-mode speed;
- Mayhem speed limits;
- ADD_VALUE modifiers;
- ADD_MOD modifiers;
- MULTIPLY_MOD modifiers.

That is the correct place to scale the value seen by movement physics.

### V0.5B policy

The hook multiplies the native return only when:

```text
CharacterOwner != null
APawn::IsLocallyControlled == true
MovementMode == Walking or NavWalking
MovementSpeed feature == enabled
```

So it does not globally modify movement components, enemies, falling, swimming,
flying or custom movement modes.

Default remains:

```ini
MovementSpeedMultiplier=1.150
```

For validation, test **2.00x** first so the effect is unmistakable.

### Regression note

Action Recovery from V0.5A is preserved unchanged.

**Validation:** awaiting V0.5B in-game test.


### Recovery audit note — MoveInterruptDelaySec

The previous V0.5A interpretation of `MoveInterruptDelaySec` as the player's
post-dodge movement lock is rejected.

Current hypothesis:

- it may instead participate in hit reaction / stun / interruption recovery;
- it remains a valid field to investigate for a future **stun / hit recovery**
  feature;
- it must not be reused for dodge recovery without runtime proof.

Dodge recovery investigation is now focused on the dedicated Mayhem dash ability
state and its movement-input lock instead.


## Active investigation — Dodge recovery

User clarified the target behavior:

> after a dodge animation appears finished, the player can remain unable to
> move for a short tail window.

This is now treated as **dash/dodge recovery**, not generic movement recovery.

Current native leads:

- `UMayhemPlayerDashAbility`
- `UMayhemPlayerGroundDashAbility`
- `UMayhemPlayerDashStartAbility`
- `MinMontageRate`
- `MaxMontageRate`
- `MontageRateIncreaseDuration`
- player movement-input lock observed at `AMayhemPlayerCharacter + 0xAAC`
- dash entry sets that lock;
- dash finalization clears that lock.

The next implementation must target the dash-state recovery/cancel window or
early release of that movement-input lock. It must **not** reuse
`MoveInterruptDelaySec` unless runtime evidence proves relevance.

### MoveInterruptDelaySec reclassification

`MoveInterruptDelaySec` remains an interesting field, but is now tracked as a
possible **hit / stun / interruption recovery** control rather than dodge
recovery.

### Horse roadmap addition

A separate TODO exists for **Horse Dash / Sprint Duration**, distinct from horse
speed, so mount sprint duration can be tuned independently.


## V0.6A — Self-calibrating Dodge Recovery

**Status: REJECTED**

User testing reported no perceptible change. The `Player + 0xAAC` dash flag is therefore not accepted as the actual post-dodge movement blocker.

V0.5A `MoveInterruptDelaySec` recovery is no longer installed.

V0.6A targets the dash ability's own movement-input lock:

```text
AMayhemPlayerCharacter + 0xAAC
```

Native audit:

```text
GroundDash start RVA   0x5B0440
GroundDash tick RVA    0x5B72B0
GroundDash finish RVA  0x59BF80
lock set site          0x5B047A
lock clear site        0x59BFBF
```

The runtime resolver does not blindly use those RVAs. It finds unique start and
finish signatures, then derives the tick function from their shared vtable
relationship.

### Behavior

- first observed dodge is left completely vanilla;
- the ASI measures its native dash-lock lifetime;
- later dodges release only the movement-input lock slightly before native
  finalization;
- dash animation, dash velocity, collision and ability finish remain native;
- default early release: **100 ms**;
- overlay range: **0 to 300 ms**;
- F3 toggles Dodge Recovery;
- the overlay shows the learned native dash-lock duration;
- signature/vtable mismatch is fail-open.

INI:

```ini
[Values]
DodgeEarlyUnlockMs=100.000
```

This is intentionally a targeted test. If 100 ms is too early or too late, the
value can be tuned directly from the overlay without rebuilding the ASI.


## V0.6B — InputSuppressWindow diagnostic bypass

**Status: TEST CANDIDATE**

V0.6A produced no perceptible change, so the dash-local flag at
`AMayhemPlayerCharacter + 0xAAC` is now treated as an accompanying dash-state
flag rather than the proven movement blocker.

A deeper binary audit found a dedicated animation-notify state:

```text
UAnimNotify_InputSuppressWindow
Display name: "Suppress Player Input Window"
```

Its vtable resolves semantically from that display-name string.

Audited native methods:

```text
GetNotifyName : RVA 0x5D1230
NotifyBegin   : RVA 0x5D8860
NotifyEnd     : RVA 0x5D8F70
```

`NotifyBegin` resolves the player character and calls:

```text
AMayhemPlayerCharacter::SetInputSuppressed(reason, true)
```

`NotifyEnd` calls:

```text
AMayhemPlayerCharacter::SetInputSuppressed(reason, false)
```

The player keeps suppression reasons in a counted table at approximately
`Player + 0xC20`, so this system can suppress input independently of the
dash-local `+0xAAC` flag.

### Diagnostic policy

When Dodge Recovery is ON, V0.6B bypasses both
`UAnimNotify_InputSuppressWindow::NotifyBegin` and matching `NotifyEnd`
completely.

This is intentionally aggressive and diagnostic.

Goal:

- if dodge recovery suddenly changes, the real blocker has been identified;
- if nothing changes, InputSuppressWindow is eliminated as the cause and the
  next audit moves to the ability/action state machine.

A bypass counter is maintained so toggling the feature does not unbalance the
game's native suppression counters.

The log prints every bypassed Begin/End. If the feature has no visible effect,
those lines tell us whether the dodge montage actually uses this notify state.

**Do not treat V0.6B as a final implementation.**


## V0.7A — Common Ability MOVE Recovery

**Status: TEST CANDIDATE**

The latest user report clarified that the dead movement tail is **not dodge-specific**:
many player actions can finish visually, then leave the character fixed for roughly
half a second before movement resumes.

### What the V0.6 logs proved

V0.6A did not actually install in the tested executable because its dash resolver
failed closed:

```text
Dodge recovery: start/finish signature mismatch start=1 finish=2
Dodge recovery: resolver failed; feature remains fail-open
```

V0.6B did install its `UAnimNotify_InputSuppressWindow` diagnostic hooks, but no
Begin/End bypass events were recorded during the supplied test session. That
system therefore does not explain the common recovery tail observed across many
actions.

### Common ability system audit

The native ability base exposes:

```text
EnableInterrupt
IsInterruptEnabled
ProcessInterrupt
AbilityInterruptsFlags
AllowedActionsFlags
```

The interrupt enum includes:

```text
EAbilityInterrupt::NONE
EAbilityInterrupt::MOVE
EAbilityInterrupt::JUMP
...
```

The audited native `IsInterruptEnabled` function reads the ability interrupt
bitset directly from the ability object:

```text
bitset storage: +0xB0
ability state:  +0xD8
elapsed time:   +0xDC
```

The ability lifecycle enum is:

```text
0 INITIALIZING
1 STARTING
2 RUNNING
3 SUSPENDED
4 AWAITING_FINISH
5 FINISHED
6 FINALIZED
```

### V0.7A policy

V0.7A hooks the common native `IsInterruptEnabled` path.

For `EAbilityInterrupt::MOVE` only:

- native MOVE permission is always preserved;
- STARTING and RUNNING remain untouched;
- movement is **not** allowed to cancel an action early;
- if native MOVE is still disabled while the ability is in
  `AWAITING_FINISH`, the mod returns true.

This specifically targets the dead post-action tail without turning movement
into a universal animation cancel.

### Runtime diagnostics

The overlay shows:

- total MOVE interrupt queries;
- queries where native code blocked MOVE;
- queries forced by V0.7A;
- last observed ability state;
- last elapsed ability time.

The log records state transitions and the first forced MOVE events.

If the user still feels no difference:

- `Forced > 0` means the common interrupt path is active but another movement
  gate remains downstream;
- `Forced = 0` means the observed dead tail occurs before/after
  `AWAITING_FINISH`, and the logged state transition tells us where to move next.


## V0.8A — AllowedActions MOVE diagnostic

**Status: TEST CANDIDATE**

### Why V0.7A is rejected

The user reported no effect, and the supplied V0.7A log is decisive:

- the `IsInterruptEnabled` hook resolved and installed successfully;
- no `MOVE query`, `Native blocked` or `FORCE MOVE` events were emitted
  during the test session.

Therefore the observed post-action movement lock does **not** flow through
`EAbilityInterrupt::MOVE` in the tested paths.

### New target: IsActionEnabled

The reflected `IsActionEnabled` wrapper calls a tiny native bit-test helper at
audited RVA:

```text
0x5A6800
```

Its native code checks the ability's action bitset:

```text
ability + 0x80
```

This is distinct from `IsInterruptEnabled`, which reads from `ability + 0xB0`.

The previously audited generic movement gate independently established:

```text
ECharacterActions::MOVE = 0x1D
```

### Local-player filtering

The ability reflection data exposes `Instigator` at:

```text
ability + 0x48
```

V0.8A records the locally controlled player pointer from the already installed
CharacterMovement hook, then applies the diagnostic only when:

```text
ability->Instigator == local player
```

Enemies and NPCs therefore keep native action permissions.

### Diagnostic policy

When Action Recovery is ON and the local player's ability receives:

```text
IsActionEnabled(MOVE)
```

V0.8A preserves native `true`, but overrides native `false` to `true`.

This is intentionally aggressive. The purpose is to prove or reject
`AllowedActionsFlags` as the common movement lock.

Telemetry records:

- all MOVE action queries;
- local-player MOVE queries;
- local queries where native code blocked MOVE;
- forced MOVE results;
- ability state and elapsed time at the last local query.

If this finally removes the dead tail but allows movement too early during some
actions, the path is proven and the next build can restrict the override by
state/time using the captured telemetry.

If counters remain at zero, `AllowedActionsFlags` is also eliminated.


## V0.8B — Safe Action Tail + Unified Gameplay UI

**Status: TEST CANDIDATE**

### V0.8A user result

V0.8A finally produced a visible gameplay change, which is important evidence
that the `AllowedActionsFlags / IsActionEnabled(ECharacterActions::MOVE)` path
is relevant.

However the V0.8A diagnostic was intentionally aggressive and forced MOVE
whenever the local player's active ability returned false.

The user observed a concrete regression after opening a chest:

- the character could move;
- the interaction/character animation remained locked;
- the result was movement without a valid locomotion animation.

This proves that MOVE must **not** be forced while an interaction ability is
still in its normal `RUNNING` phase.

### V0.8B recovery policy

V0.8B keeps the same proven `IsActionEnabled(MOVE)` path, but changes the
policy:

```text
INITIALIZING     native
STARTING         native
RUNNING          native
SUSPENDED        native
AWAITING_FINISH  mod may release MOVE
FINISHED         native
FINALIZED        native
```

Only the `AWAITING_FINISH` dead tail is shortened.

A new user setting controls how long to wait after first observing the tail:

```ini
ActionRecoveryDelayMs=0.000
```

Overlay range:

```text
0 ms -> 500 ms
```

Default `0 ms` means release MOVE immediately once the ability has already
entered `AWAITING_FINISH`.

This is designed specifically to avoid the chest/interactions regression seen
with V0.8A while still attacking the common post-action freeze.

### Overlay redesign

The unused `General` tab has been removed.

Tabs are now:

```text
Gameplay
Hotkeys
About
```

Menu-key rebinding and configuration buttons moved to `Hotkeys`.

The Gameplay page is grouped into:

```text
Player
Combat
Horse
Camera
System
```

### Unified option model

Where a numeric tuning value makes sense, every gameplay feature now follows:

```text
[checkbox] Feature
           [value slider]
```

Persistent options now exist for:

```text
Movement Speed
Action Recovery
Jump Height
Pistol Damage
Melee Damage
Hotstreak Charge
Horse Speed
Horse Sprint Speed
Horse Sprint Duration
FOV
Third Person camera distance
```

Toggle-only features remain toggle-only where a scalar has no meaningful
semantics:

```text
Toggle HUD
Skip Intro Videos
```

Pending features are clearly labelled `Pending hook`; their values are saved
now so future runtime implementations do not require another menu redesign.

### Reference-PAK-driven defaults

The new UI names follow the concrete systems recovered from the supplied
reference PAKs:

- Jump Height -> player Blueprint movement defaults;
- Horse Speed -> `MaxWalkSpeed`;
- Horse Sprint Duration -> `StaminaSprintPercentageRate`;
- Pistol Damage -> projectile `Damage`;
- Hotstreak Charge -> projectile `BaseJuice` candidate.

These pending controls are configuration/UI infrastructure only until their
runtime hooks are validated.


## V0.9A — Jump / Glide / Horse Sprint Duration Hooks

**Status: TEST CANDIDATE**

V0.9A starts converting the reference-PAK findings into real runtime hooks.

### Reflection offsets recovered from the executable

The UE4 generated property parameter tables in the audited executable expose the
actual runtime offsets:

```text
UCharacterMovementComponent
  JumpZVelocity                                      +0x1A0

UMayhemPlayerCharacterMovementComponent
  DoubleJumpZVelocity                                +0x85C
  GlideDurationSeconds                               +0x86C

AMayhemHorseCharacter
  StaminaRecoveryPercentageRate                     +0x910
  StaminaTotalRecoveryPercentageRate                +0x914
  StaminaSprintPercentageRate                       +0x918
```

These offsets are derived from the generated reflection metadata for the target
executable, not guessed from community SDK layouts.

### Runtime application model

The already-installed
`UMayhemCharacterMovementComponent::GetMaxSpeed` hook is used as a safe
game-thread heartbeat.

For each local movement component, V0.9A captures the original values once and
then applies or restores the selected settings.

This means disabling an option restores the value that the game instance
originally had instead of hardcoding one character's defaults.

### Jump Height

New active control:

```text
Jump Height [ON/OFF]
0.50x .. 3.00x
```

Targets:

```text
JumpZVelocity
DoubleJumpZVelocity
```

Because ballistic jump height is approximately proportional to vertical
velocity squared when gravity is unchanged, V0.9A uses:

```text
velocity multiplier = sqrt(height multiplier)
```

This keeps the user-facing value closer to an actual height multiplier.

### Glide / Flight Duration

New option:

```text
Glide / Flight Duration [ON/OFF]
0.50x .. 5.00x
```

Target:

```text
GlideDurationSeconds +0x86C
```

The original per-character duration is captured at runtime, so Strife and War
can retain different vanilla durations while receiving the same multiplier.

Default:

```ini
GlideDuration=1
GlideDurationMultiplier=1.500
```

### Horse Sprint Duration

The reference horse PAK proved that sprint duration is directly controlled by
stamina drain:

```text
StaminaSprintPercentageRate
```

The example mod changes the observed vanilla value:

```text
25.0 -> 0.0
```

V0.9A implements a tunable duration multiplier without forcing infinite sprint:

```text
effective stamina drain = original drain / duration multiplier
```

Examples for an original drain of 25:

```text
1.00x -> 25.0
2.00x -> 12.5
5.00x -> 5.0
10.0x -> 2.5
```

Disabling the option restores the captured original drain.

### UI state

The Gameplay page now contains active runtime controls for:

- Jump Height
- Glide / Flight Duration
- Horse Sprint Duration

Other roadmap controls remain visible and persistent but are still marked
`Pending hook` until their runtime implementation lands.

**Validation:** awaiting in-game V0.9A test.


## V0.9B — Crash fix / Jump + Glide only

**Status: TEST CANDIDATE**

V0.9A is **REJECTED** because it crashes during level loading.

Crash evidence from the supplied UE4 crash package:

```text
Crashed thread: GameThread
Module frame: DarksidersGenesisMod + 0x3676
```

Disassembly of the exact V0.9A ASI maps RVA `0x3676` to:

```asm
movss xmm0, dword ptr [rbx + 0x918]
```

That instruction is the V0.9A read of:

```text
AMayhemHorseCharacter::StaminaSprintPercentageRate
```

inside `FindOrCaptureHorseState`.

The unsafe assumption was:

```text
AMayhemPlayerCharacter + 0xE70
    == stable live AMayhemHorseCharacter pointer
```

The crash proves that assumption is false in at least some loading/runtime
states.

### V0.9B correction

The entire direct horse-pointer tuning path is removed.

V0.9B keeps active only:

```text
Jump Height
Glide / Flight Duration
```

Horse Sprint Duration remains visible/configurable but is marked pending until a
safe horse-instance resolver is established from an actual horse component /
ability path.

No read or write to `horse + 0x918` occurs in V0.9B.

**Validation:** **VALIDATED IN GAME.** User confirmed V0.9B works correctly after removing the unsafe horse pointer path.


### V0.9B validation result

User validation:

```text
"nickel"
```

Confirmed:

- level loading no longer crashes;
- the V0.9A horse pointer crash path is removed;
- Jump Height remains usable;
- Glide / Flight Duration remains usable.

V0.9B therefore becomes the new validated base for the movement/glide branch.

Horse Sprint Duration remains pending until a safe horse-instance resolver is
found.


## V0.10A — Hotstreak Charge / AddJuice Hook

**Status: TEST CANDIDATE**

V0.10A starts the combat-hook phase with the safest target recovered from the
reference PAK audit.

### Native target

The executable exposes the Hotstreak API:

```text
AddJuice
GetCurrentJuice
GetCurrentJuiceRatio
GetMaxJuice
RemoveJuice
TryActivateHotStreak
TryConsumeJuice
```

The generated `AddJuice` exec wrapper resolves to the native function at:

```text
RVA 0x660260
```

The native body has a unique signature in the audited executable.

### Runtime policy

V0.10A hooks:

```text
AddJuice(void* component, float Amount)
```

Only positive gains are eligible.

The native function immediately reads its player-owner pointer from:

```text
component + 0xE8
```

V0.10A compares that pointer to the already captured locally controlled player
and only multiplies the gain when they match.

No writes are made to arbitrary UObject/Blueprint fields.

Formula:

```text
effective gain = native gain * HotstreakChargeMultiplier
```

Loss/consumption paths remain native.

### UI telemetry

The existing `Hotstreak Charge` checkbox/value is now active.

The overlay displays:

```text
Boost calls
Last native gain
Last boosted gain
```

This lets the user confirm immediately whether gameplay is flowing through the
hook.

Default:

```ini
HotstreakCharge=1
HotstreakChargeMultiplier=2.000
```

### Pistol Damage

Pistol Damage remains pending in V0.10A.

The reference PAK proves the projectile Blueprint `Damage` property is the
correct gameplay target, but it is Blueprint-defined rather than a simple
native reflected field in the executable. A safe projectile/damage-call filter
will be audited separately rather than guessing an object offset.

**Validation:** **REJECTED FOR HORSE SPEED IN GAME.**

User feedback:

```text
The horses do not seem to accelerate.
```

Regardless of whether the safer V0.14B mount telemetry resolves correctly,
multiplying the horse `GetMaxSpeed` return is not accepted as a working speed
control. V0.14C therefore stops using that return value as the gameplay lever
and moves to the direct movement properties proven by the reference Horse PAK.


## V0.10B — Expanded tuning ranges

Requested maximum values:

```text
Jump Height        5.00x
Glide Duration    10.00x
Hotstreak Charge  25.00x
```

The runtime clamps were updated to match the UI limits, so the higher values are
not cosmetic-only.

Development continues on the same V0.10 combat branch.


## V0.11A — Pistol Damage / DamageRecord Hook

**Status: TEST CANDIDATE**

The executable reflection metadata confirms:

```text
FMayhemDamageEventRecord
  Damage          +0x08
  ScaleType       +0x0C
  ElementTypes    +0x10
  DamageSourceTags +0x18
  HotStreak       +0x28
    BaseJuice     +0x00
```

The native projectile base also stores:

```text
DamageEventRecord +0x88
HotStreak         +0x1B0
```

The Blueprint library `DoDamageToActor` native function resolves uniquely at:

```text
RVA 0x667300
```

### Filter

The user-supplied DualPistols reference PAKs consistently modify both:

```text
Damage
BaseJuice
```

on Strife gun projectiles.

V0.11A therefore treats a positive, sane `BaseJuice` value in the outgoing
`FMayhemDamageEventRecord` as the gun/projectile discriminator.

### Mutation policy

The hook:

1. reads native Damage and BaseJuice;
2. if BaseJuice > 0, multiplies Damage by the user value;
3. calls native `DoDamageToActor`;
4. restores the original Damage immediately.

No Blueprint/CDO or projectile instance is permanently mutated.

### Overlay telemetry

Pistol Damage now shows:

```text
Boost calls
Last native damage
Last boosted damage
Last BaseJuice
```

This will quickly confirm whether the expected DualPistols records are flowing
through the hook.

**Validation:** awaiting in-game test.


## V0.11B — Manual Numeric Input + Zero-Minimum Policy

**Status: TEST CANDIDATE**

User-requested UI/value policy:

- every tunable numeric option now exposes both a slider and an explicit manual
  input field;
- multiplier-style values use **0** as their minimum;
- future multiplier-style options should also default to a minimum of 0 unless
  zero is semantically invalid.

New requested defaults/ranges:

```text
Movement Speed
  default 1.50x
  min 0.00x

Jump Height
  default 1.25x
  min 0.00x
  max 5.00x

Glide / Flight Duration
  default 10.00x
  min 0.00x
  max 100.00x

Pistol Damage
  min 0.00x
  V0.11A hook rejected by user: no gameplay effect

Melee Damage
  min 0.00x

Hotstreak Charge
  min 0.00x
  max 25.00x

Horse Speed
  min 0.00x

Horse Sprint Speed
  min 0.00x

Horse Sprint Duration
  min 0.00x
```

Third-person distance is also treated as a multiplier and now allows 0. FOV
remains an absolute camera angle and keeps its safe nonzero range.

### Pistol Damage rollback

V0.11A `DoDamageToActor + BaseJuice` filtering is **REJECTED** because user
testing produced no pistol-damage change.

The hook is no longer installed in V0.11B. The menu entry remains available as
a pending feature while a better projectile/damage path is audited.


## V0.12A — Melee Damage / GetBaseDamage Diagnostic

**Status: TEST CANDIDATE**

The executable exposes a generated Blueprint exec wrapper for:

```text
GetBaseDamage
```

Audited wrapper:

```text
RVA 0x770D90
virtual slot +0x928
```

The wrapper calls the character virtual, receives the native float in XMM0 and
writes it to the Blueprint result pointer.

### V0.12A policy

The hook:

1. runs the native wrapper first;
2. only accepts the locally controlled player character;
3. reads the returned BaseDamage float;
4. applies `MeleeDamageMultiplier`;
5. writes only the Blueprint result value.

No character stats or UObject fields are permanently modified.

This is intentionally diagnostic because `BaseDamage` may feed more than one
player attack family. The overlay telemetry shows:

```text
Boost calls
Last native BaseDamage
Last boosted BaseDamage
```

Testing should compare melee attacks against pistol/ranged attacks. If melee
damage changes while projectile damage does not, the path is suitable for the
feature. If unrelated player damage scales too, the next build will filter the
callsite/ability rather than keeping the broad BaseDamage result override.

### Cumulative UI/value policy retained

V0.12A includes V0.11B:

- visible manual numeric entry beside every slider;
- multiplier minima at 0;
- Movement Speed default 1.50x;
- Jump Height default 1.25x, 0..5x;
- Glide / Flight Duration default 10x, 0..100x;
- Hotstreak Charge 0..25x;
- Pistol Damage V0.11A rejected and not installed.

**Validation:** awaiting in-game test.


## V0.12B — Melee 100x Diagnostic

**Manual numeric UI: VALIDATED.**

User confirmed the slider + manual-entry interface is good and should remain the
standard numeric-control pattern.

### Melee Damage status

V0.12A is **not rejected**.

User feedback indicates the setting appears to have some effect, but a 10x
setting does not produce an obviously decisive one-shot result. This is
consistent with `GetBaseDamage` potentially being only one term in the final
damage formula.

Current classification:

```text
Melee Damage / GetBaseDamage
PROMISING / PARTIALLY OBSERVED
not yet validated
not rejected
```

### V0.12B diagnostic range

Melee Damage maximum is raised to:

```text
100.00x
```

The runtime clamp is also 100x, so this is not UI-only.

Test interpretation:

- if 100x produces an unmistakable damage jump / one-shots, the
  `GetBaseDamage` path is confirmed as materially contributing to combat;
- if 100x still has only a modest effect, `GetBaseDamage` is not the final
  applied-damage control and the next audit should move downstream into the
  final damage-event scaling path.

The existing overlay telemetry remains:

```text
Boost calls
Last base damage -> boosted base damage
```

This lets the runtime call path be distinguished from the gameplay effect.


## V0.13A — Final Player Outgoing Damage Hook

**Status: TEST CANDIDATE**

### V0.12B result

V0.12B `GetBaseDamage` is **REJECTED**.

User testing at **100x** still did not produce an unmistakable damage increase.
Therefore `GetBaseDamage` is not the correct final applied-damage control for
the requested Melee Damage option.

### Native final outgoing-damage stage

The executable contains the player outgoing-damage filter at:

```text
RVA 0x668BE0
```

The function directly mutates:

```text
FMayhemDamageEventRecord::Damage +0x08
```

and natively applies:

```text
d.PlayerOutgoingDamageMultiplier
```

before continuing through outgoing-damage filters/status effects.

This is the lowest confirmed player-outgoing stage found so far and is
downstream of `GetBaseDamage`.

### V0.13A policy

V0.13A calls the complete native outgoing-damage filter first, then applies the
user multiplier to the resulting final outgoing Damage value.

The hook is local-player-only.

Current diagnostic classification:

```text
BaseJuice > 0
  -> Pistol Damage

BaseJuice == 0
  -> Melee Damage diagnostic
```

The pistol discriminator comes directly from the supplied Strife projectile
reference PAKs, where gun projectiles carry positive `BaseJuice`.

The zero-juice branch is intentionally still diagnostic: it may also include
some non-pistol abilities. Runtime telemetry logs `ScaleType` and damage-tag
count so the filter can be tightened after testing.

### Runtime telemetry

Overlay/logs now report:

```text
Pistol:
  event count
  final native damage -> boosted damage
  BaseJuice

Melee diagnostic:
  event count
  final native damage -> boosted damage
  ScaleType
  DamageSourceTags count
```

### Range

Both final-damage multipliers can reach 100x in this diagnostic build.

If a 100x value still has no visible effect, the problem is no longer an
upstream stat/filter issue and the next audit must move to the target-side
health subtraction path.

**Validation:** awaiting in-game test.


## V0.13B — Final Outgoing Damage + Functional Skip Intro

**Status: TEST CANDIDATE**

V0.13B combines the new final outgoing-damage pipeline with a real native
Skip Intro implementation in one cumulative build.

### Functional Skip Intro

The executable contains the native console variable:

```text
g.PlayIntroCinematicOnBoot
default = 1
```

Its registration and boot-time use were audited directly in the target EXE.

Registration xref:

```text
RVA 0x000E831D
```

Native boot read:

```text
RVA 0x0063C465

mov rax,[g.PlayIntroCinematicOnBoot_data]
cmp dword ptr [rax],0
je  skip_intro_path
```

The resolver does not hardcode the runtime address. It:

1. finds the UTF-16 CVar name;
2. resolves the unique registration LEA;
3. derives the UE4 CVar data-slot store;
4. validates the unique boot-time `cmp [CVarData],0` use;
5. waits briefly for UE4 static CVar initialization;
6. captures the vanilla value;
7. applies:
   - Skip Intro ON -> `0`
   - Skip Intro OFF -> captured vanilla value.

The control is applied **before D3D11 probe initialization** so the default
enabled state can take effect as early as possible during startup.

The Present hook also reapplies the selected state so overlay/hotkey changes are
kept synchronized. Turning Skip Intro on/off after boot naturally affects the
next startup rather than retroactively cancelling an already-started cinematic.

### UI / hotkey

`Skip Intro Videos` is no longer marked Pending.

The overlay shows:

```text
Native g.PlayIntroCinematicOnBoot control
Native CVar now
Vanilla captured value
```

F4 continues to be the default hotkey and now toggles the real native control.

### Damage

V0.13B also contains the V0.13A final player outgoing-damage hook:

```text
RVA 0x668BE0
```

Current diagnostic split:

```text
BaseJuice > 0  -> Pistol Damage
BaseJuice == 0 -> Melee Damage diagnostic
```

Both are applied after the game's native outgoing-damage filtering.

**Validation:** awaiting in-game test for both final damage and boot intro skip.


## V0.13B validation result

**VALIDATED IN GAME.**

User feedback:

```text
"oui nickel"
```

Validated cumulative behavior:

- functional native Skip Intro control through `g.PlayIntroCinematicOnBoot`;
- final player outgoing-damage hook is accepted as the new damage base;
- V0.13B remains cumulative with the previously validated manual numeric input,
  Jump Height, Glide / Flight Duration, Movement Speed, HUD and safe Action
  Recovery behavior.

V0.13B becomes the new canonical development base.

The next implementation phase targets the horse movement/stamina feature set,
using a safe horse component / ability path rather than the rejected
`Player + 0xE70` pointer assumption from V0.9A.


## V0.14A — Safe Horse Runtime

**Status: REJECTED**

V0.14A reintroduces the horse feature set after the V0.9A crash, but no longer
trusts `Player + 0xE70` as a horse object by itself.

### Structural validation chain

The previously audited native function:

```text
AMayhemPlayerCharacter::IsHorseActive
cmp qword ptr [rcx + 0xE70], 0
setne al
ret
```

is now hooked and used only as the GameThread heartbeat.

The pointer at `Player + 0xE70` is treated as a **candidate**, never as proof.

Before V0.14A reads or writes any horse gameplay property it requires:

```text
local player
  -> dynamically discovered CharacterMovement member offset
candidate horse at player + 0xE70
  -> CharacterMovement at the same member offset
horse movement + 0x190
  -> CharacterOwner == candidate horse
horse stamina fields
  -> finite and sane
horse movement vtable + 0x3D0
  -> GetMaxSpeed target inside executable .text
```

If any check fails the horse feature remains fail-open and vanilla.

### Dynamic CharacterMovement member discovery

The already validated player movement hook knows both:

```text
player CharacterOwner
player MovementComponent
```

V0.14A scans aligned pointer members on the local player for that exact
MovementComponent pointer and accepts the offset only when it occurs uniquely.

The same discovered offset is then required to work on the candidate horse.

This removes the need to guess the ACharacter CharacterMovement member offset.

### Horse Speed

The validated horse movement component provides its actual virtual
`GetMaxSpeed` target from vtable slot:

```text
+0x3D0
```

V0.14A dynamically hooks that horse-specific function when needed.

If the horse happens to share the same Mayhem GetMaxSpeed implementation as the
player, the existing movement hook handles the validated horse path without a
second hook.

The lowest positive native horse max speed observed is captured as the normal
baseline.

```text
native <= baseline * 1.08 -> Horse Speed
native >  baseline * 1.08 -> Horse Sprint Speed
```

This classification is diagnostic and telemetry is shown in the overlay.

### Horse Sprint Duration

After the actor/movement structural proof, the reflected horse properties are
used:

```text
StaminaRecoveryPercentageRate       +0x910
StaminaTotalRecoveryPercentageRate  +0x914
StaminaSprintPercentageRate         +0x918
```

Only `StaminaSprintPercentageRate` is modified.

Formula:

```text
effective drain = original drain / duration multiplier
```

A value of `0x` is treated as effectively no sustainable sprint and applies a
very high drain rather than dividing by zero.

Unmounting restores the captured original sprint-drain value when the object is
still safely writable.

### Overlay telemetry

The Horse section now reports:

- whether a structurally validated mount exists;
- discovered CharacterMovement member offset;
- native -> effective horse speed;
- NORMAL / SPRINT classification;
- captured normal-speed baseline;
- native -> effective sprint stamina drain;
- validation success / rejection counters.

**Validation:** **REJECTED IN GAME.**

User feedback:

```text
V0.14A did not find the mount even while the player was mounted.
```

The native `IsHorseActive` heartbeat itself is still useful, but the
V0.14A structural chain was too restrictive. In particular, it assumed that
the object reachable through `Player + 0xE70` could expose its
`CharacterMovement` pointer at the exact same member offset dynamically
discovered on the player character.

That assumption is now rejected. `Player + 0xE70` must not be treated as the
horse actor or as the basis for horse gameplay reads/writes.


## V0.14B - Movement-owner horse detection

**Status: REJECTED FOR HORSE SPEED**

V0.14B starts from V0.14A but removes the failed mount-identification chain.

### Detection policy

`AMayhemPlayerCharacter::IsHorseActive` is retained only as a native
**mounted-state signal** for the locally controlled player.

V0.14B no longer **assumes** that `Player + 0xE70` is a ready-to-use
`AMayhemHorseCharacter`.

The native pointer is now treated only as an **opaque candidate**. While mounted,
the mod safely scans aligned pointer members of that candidate and accepts a
movement component only when:

```text
candidate member pointer
  -> CharacterOwner +0x190 points back to the same candidate
  -> MovementMode is sane
  -> GetMaxSpeed slot +0x3D0 points inside executable .text
candidate actor
  -> horse stamina fields +0x910/+0x914/+0x918 are readable and sane
```

This specifically removes the failed V0.14A assumption that the horse movement
pointer must exist at the exact same member offset as the player movement
pointer.

A second fallback path remains active: while the native mounted state is true,
the validated player `GetMaxSpeed` hook observes live non-player movement
owners and can adopt one as the horse after the same structural checks.

Only after structural proof does the mod:

- capture the horse runtime state;
- enable Horse Speed / Horse Sprint Speed classification;
- apply Horse Sprint Duration through the captured native stamina drain;
- install a dedicated horse GetMaxSpeed hook only if the mount does not share
  the already hooked player movement target.

This keeps V0.14B fail-open if neither discovery path can prove the mount.

### Diagnostics

Horse validation failures now keep a reason code and only log when the reason
changes, avoiding per-frame log spam.

### Log policy change

Starting with V0.14B, `DarksidersGenesisMod.log` is truncated once at startup.
The log therefore contains **only the current game session** and is no longer
cumulative across launches.

### Camera roadmap additions

Added to the TODO list:

- **Character Zoom / Camera Distance**
- **Camera Angle**

These remain separate from FOV and Third Person so each camera behavior can be
tuned independently.

**Validation:** awaiting in-game test.


## V0.14C - Direct Horse Movement Properties

**Status: TEST CANDIDATE**

V0.14C keeps the safer horse discovery work from V0.14B but changes the actual
speed-control mechanism.

### Why V0.14B is not the speed base

V0.14B attempted to multiply the result of the horse movement
`GetMaxSpeed` virtual. In-game testing did not produce perceptible horse
acceleration.

The reference Horse PAK provides a much stronger gameplay target because it
changes the horse CharacterMovement defaults directly:

```text
MaxWalkSpeed      1300.0 -> 1500.0
MaxAcceleration    600.0 -> 700.0
```

V0.14C therefore treats the GetMaxSpeed hook as telemetry only.

### Runtime offsets under test

The target executable already gave three exact CharacterMovement anchors:

```text
CharacterOwner  +0x190
JumpZVelocity   +0x1A0
MovementMode    +0x1B0
```

These align with the UE4 CharacterMovement property sequence used by this
engine build. V0.14C derives the following **test candidates**:

```text
MaxWalkSpeed    +0x1D4
MaxAcceleration +0x1E8
```

These are not blindly written. The horse candidate must first pass all V0.14B
structural validation and the live values must also fall inside horse-specific
ranges consistent with the reference PAK:

```text
MaxWalkSpeed    1000 .. 2000
MaxAcceleration  250 .. 1500
```

This intentionally rejects the known local-player movement defaults such as
the much higher player MaxAcceleration.

### Application model

Once the horse movement component is validated, V0.14C captures its native
values once and applies:

```text
effective MaxWalkSpeed    = native MaxWalkSpeed    * HorseSpeedMultiplier
effective MaxAcceleration = native MaxAcceleration * HorseSpeedMultiplier
```

Disabling Horse Speed or unmounting restores the captured native values.

The direct values are re-applied while mounted so runtime overlay changes take
effect without requiring a remount.

### Sprint separation

**Horse Sprint Speed is not claimed as implemented by V0.14C.**

The reference assets indicate that sprint has its own ability/runtime
`RunSpeed` path. Until that path is resolved, Horse Sprint Speed remains
pending instead of pretending that GetMaxSpeed scaling works.

Horse Sprint Duration remains a separate stamina-drain feature through
`StaminaSprintPercentageRate`.

### New telemetry

The Horse overlay reports:

- native and applied `MaxWalkSpeed`;
- native and applied `MaxAcceleration`;
- native mounted signal;
- horse/player movement member offsets;
- GetMaxSpeed telemetry only;
- validation/rejection counters and last rejection reason.

Additional V0.14C rejection reasons:

```text
9  direct movement properties unreadable
10 direct movement values outside horse-specific range
11 MaxWalkSpeed / MaxAcceleration not writable
```

**Validation:** **REJECTED AS A RUNTIME TEST.**

User feedback:

```text
Horse speed still does not change.
```

The supplied V0.14C log explains why this result does **not** invalidate the
direct MaxWalkSpeed / MaxAcceleration path:

```text
Horse runtime: IsHorseActive signature match count=0
Horse runtime: IsHorseActive resolver failed; horse features remain fail-open
Horse runtime unavailable; horse features remain fail-open
```

No later `mounted`, `VALIDATED`, `MaxWalkSpeed`, or horse rejection-reason
telemetry appears. Therefore V0.14C never reached horse discovery or any direct
movement write.

The same log also exposed excessive repeated diagnostics:

```text
Horse runtime: CharacterMovement member discovery ambiguous matches=2
```

This repeated thousands of times and is treated as a logging bug.




## V0.14D - Horse Heartbeat Resolver Fix

**Status: TEST CANDIDATE**

V0.14D fixes the root cause proven by the V0.14C runtime log.

### Remove the failed IsHorseActive byte hook

The audited native logic is still:

```text
AMayhemPlayerCharacter::IsHorseActive
    Player + 0xE70 != nullptr
```

V0.14A-C tried to locate and hook a tiny compiled helper implementing that
check. In the actual V0.14C session the resolver returned zero signature
matches, disabling all horse features before gameplay.

V0.14D no longer installs or requires that hook.

Instead, the already validated
`UMayhemCharacterMovementComponent::GetMaxSpeed` hook acts as a GameThread
heartbeat. From the last known locally controlled player it safely reads:

```text
Player + 0xE70
```

and derives the same mounted state directly:

```text
null     -> unmounted
non-null -> mounted candidate present
```

The pointer remains an opaque candidate only. It is never trusted as a horse
actor until the full structural validation chain passes.

### Discovery sequence

While mounted, V0.14D:

1. reads the opaque candidate from `Player+0xE70`;
2. scans that candidate for a unique movement-like member;
3. requires `movement+0x190 -> CharacterOwner == candidate`;
4. validates MovementMode and executable GetMaxSpeed vtable target;
5. validates horse stamina fields;
6. validates the direct movement property candidates;
7. only then captures and modifies the horse.

The V0.14B movement-owner fallback remains available as a second path.

### Direct speed experiment retained

Because V0.14C never executed the horse code, these candidates remain
**unvalidated rather than rejected**:

```text
MaxWalkSpeed    +0x1D4
MaxAcceleration +0x1E8
```

If V0.14D reaches validation, the overlay/log should finally expose the native
and applied values and provide a real in-game test of this path.

### Logging cleanup

The log remains truncated at every game launch.

V0.14D additionally suppresses per-frame diagnostic floods:

- ambiguous CharacterMovement member count is logged only when the count changes;
- rejected non-player movement captures have a small per-session log budget;
- horse rejection reasons still log only when the reason changes.

This keeps the current-session log useful instead of producing tens of
thousands of duplicate lines.

**Validation:** awaiting in-game test.


## V0.14E - UI Defaults + Jump Height 20x + Flat Artifact

**Status: TEST CANDIDATE**

V0.14E is cumulative from V0.14D.

### Jump Height

The previous overlay/runtime ceiling of 5x is raised to:

```text
0.00x .. 20.00x
```

The runtime clamp is raised at the same time, so values above 5x are effective
rather than UI-only.

The default remains:

```text
JumpHeightMultiplier=1.250
```

### Per-value Default buttons

Every currently tunable numeric control now exposes a `Default` button next
to its slider/manual field.

Defaults restored by those buttons are:

```text
Movement Speed         1.50x
Action Recovery        0 ms
Jump Height            1.25x
Glide / Flight         10.00x
Pistol Damage          2.00x
Melee Damage           2.00x
Hotstreak Charge       2.00x
Horse Speed            1.25x
Horse Sprint Speed     1.25x
Horse Sprint Duration  2.00x
FOV                    90 deg
Third Person Distance  1.00x
```

Pressing `Default` updates the in-memory value and saves it to the INI.

### Packaging fix

V0.14D's GitHub Actions artifact contained a ZIP file inside the GitHub artifact
ZIP.

V0.14E removes the inner `Compress-Archive` package from the artifact path.
The GitHub artifact now contains the four distributable files directly:

```text
dxgi.dll
DarksidersGenesisMod.asi
DarksidersGenesisMod.ini
README.md
```

Downloading the Actions artifact therefore produces one ZIP with the usable
files at its root, not a ZIP containing another ZIP.

**Validation:** awaiting in-game test.


## V0.14F - Safe Horse Diagnostics

**Status: TEST CANDIDATE**

V0.14F is cumulative from V0.14E and keeps:

- Jump Height up to 20x;
- per-value `Default` buttons;
- flat GitHub artifact packaging;
- all previously validated non-horse hooks.

### Root cause found in the V0.14E runtime log

The V0.14E session reached the mounted-state heartbeat successfully, but then
the broad mounted-time fallback treated every non-local movement owner entering
the shared `GetMaxSpeed` hook as a possible horse.

The resulting log contained roughly **301k horse rejection lines in one
session**, alternating primarily between:

```text
reason=7  no unique horse movement member found
reason=3  horse stamina fields outside sane range
```

No `Horse runtime: VALIDATED` or `MaxWalkSpeed` line appeared, so the horse
speed write path was never reached. The failure was discovery/logging pressure,
not a proven failure of the direct movement-property idea.

### V0.14F safety changes

1. Removes the broad non-local movement-owner horse fallback.
2. Uses only the opaque object reached from `Player+0xE70` as the discovery
   root.
3. Throttles the expensive horse discovery scan to **one attempt per second**.
4. Rate-limits horse rejection logging to **one line per second maximum**, even
   when rejection reasons alternate.
5. Keeps all horse writes disabled until the candidate passes the complete
   structural validation chain.
6. Adds bounded diagnostics at mount time:
   - opaque candidate pointer;
   - raw stamina triplet at +0x910/+0x914/+0x918;
   - number of movement-like members;
   - first four candidate member offsets.

This candidate is intentionally diagnostic-first. Stability of the already
validated player/combat/UI features has priority over forcing horse values.

**Validation:** **STABILITY VALIDATED IN GAME.**

User feedback:

```text
Tout est redevenu normal.
```

Validated in V0.14F:

- general game/mod stability restored;
- no V0.14E-style horse log storm;
- existing validated non-horse features remain usable;
- Jump Height 20x UI/runtime ceiling retained;
- per-value Default buttons retained;
- flat artifact packaging retained.

Horse speed / sprint behavior is **not yet validated**. The horse path remains
diagnostic-first and must not be promoted to canonical functionality until the
runtime structure is identified and tested successfully.


## V0.16A - Clean Shared-Hook Horse Test

**Status: TEST CANDIDATE**

V0.15A / B / C horse and Skip Logos experiments are not used as a runtime
base for this build.

V0.16A restarts from the V0.14F stability-validated branch and imports only the
clean source split introduced later.

The supplied V0.15C runtime log revealed that the already validated
`UMayhemCharacterMovementComponent::GetMaxSpeed` hook sees several non-player
movement components in normal gameplay. Those components showed `JumpZ=600`
while the local player movement components showed `JumpZ=900`.

That evidence makes the extra horse hooks, actor scans, Player+0xE70 path and
vtable ancestry resolver unnecessary.

### Runtime model

The existing shared GetMaxSpeed hook now does exactly this:

```text
GetMaxSpeed(movement)
  -> read CharacterOwner
  -> determine whether owner is local player
  -> HorseFeature observes non-local component
  -> normal validated player path continues unchanged
```

HorseFeature installs **no hook**.

For at most eight unique non-player movement components it records one compact
fingerprint:

```text
native GetMaxSpeed
MovementMode
JumpZVelocity
MaxWalkSpeed
MaxAcceleration
StaminaRecoveryPercentageRate
StaminaTotalRecoveryPercentageRate
StaminaSprintPercentageRate
```

A component is automatically accepted only when it matches the reference Horse
PAK signature:

```text
MaxWalkSpeed              ~= 1300
MaxAcceleration           ~= 600
StaminaRecovery           ~= 15
StaminaTotalRecovery      ~= 40
StaminaSprintDrain        ~= 25
```

Only after that exact signature is proven does the feature write:

```text
MaxWalkSpeed
MaxAcceleration
StaminaSprintPercentageRate
```

No candidate rejection spam is emitted.

Horse Sprint Speed remains pending its independent `RunSpeed` primitive.

### Skip Logos

Skip Logos is intentionally **not modified in V0.16A**. The previous file-hook,
command-line and literal-string query approaches are rejected. A new Skip Logos
attempt will not be shipped until a native target is proven from the retail
executable or startup configuration.

**Validation:** awaiting in-game test.


## Horse Direction Reset - Reference PAK Becomes Authoritative

The user correctly pointed back to the supplied working horse reference mod.
This changes the implementation priority.

Reference:

```text
ZZZ-Horse_P.pak
SHA-256 3719ac840e1d0d7f137c9322580a3abe58cc5cf93d4b1ea97352c66490d3a920
```

The reference PAK contains direct edits to the horse Blueprint defaults and
therefore provides a stronger source of truth than indirect movement-hook
experiments.

### Proven vanilla -> mod horse deltas

```text
GallopSpawnSpeedThreshold           300.0 -> 400.0
StaminaRecoveryPercentageRate        15.0 -> 100.0
StaminaTotalRecoveryPercentageRate   40.0 -> 100.0
StaminaSprintPercentageRate          25.0 -> 0.0

MaxWalkSpeed                       1300.0 -> 1500.0
MaxAcceleration                     600.0 -> 700.0
BrakingFrictionFactor                 1.0 -> 2.0
```

### New implementation rule

Future horse work must prefer direct control of the same Blueprint/CDO
properties proven by the reference PAK:

```text
MayhemHorseCharacter_Blueprint
  MaxWalkSpeed
  MaxAcceleration
  BrakingFrictionFactor
  GallopSpawnSpeedThreshold
  StaminaRecoveryPercentageRate
  StaminaTotalRecoveryPercentageRate
  StaminaSprintPercentageRate
```

The V0.14/V0.15 experimental paths remain useful historical diagnostics but are
no longer the preferred architecture:

- Player+0xE70 horse-pointer interpretation;
- actor/member scans;
- secondary/base GetMaxSpeed hooks;
- vtable ancestry resolver.

Horse Sprint Duration continues to map to stamina drain:

```text
effective drain = vanilla StaminaSprintPercentageRate / duration multiplier
```

Example with vanilla drain 25:

```text
1.0x -> 25.0
2.0x -> 12.5
5.0x -> 5.0
10x  -> 2.5
infinite -> 0.0
```

Horse Sprint Speed remains separate and must be resolved from the sprint ability
data rather than conflated with stamina duration.

### Skip Logos

Skip Logos remains paused after the rejected V0.15A/B/C methods. No new Skip
Logos candidate should be shipped until a native retail target is proven.


### V0.16A local-control correction

The first V0.16A draft passed a boolean derived from
`APawn::IsLocallyControlled` into HorseFeature and skipped those candidates.

That was incorrect for mounts: a horse controlled by the local player can
itself be locally controlled.

The final V0.16A ordering is therefore:

```text
shared GetMaxSpeed hook
  -> HorseFeature observes every movement component
  -> exact reference-PAK property signature decides horse identity
  -> APawn::IsLocallyControlled is evaluated only afterwards
  -> normal player movement tuning path continues unchanged
```

Horse identification is now independent from pawn control state.


## V0.19 - Skip Logos root cause and validated fix

The final root cause was not the generic UE4 startup-movie path. The retail
executable contains a ProjectMayhem-specific plugin:

```text
ProjectMayhem/Plugins/StartupScreens/
```

with `SStartupScreens`, `StartupScreensModule`,
`UStartupScreensSettings`, `StartupScreenDef` and `StartupMovies`.

Three plugin-level candidates were tested in game:

```text
V0.19A  StartupModule RET @ 0x25FE40
        Logos skipped
        Intro also skipped with Skip Intro OFF
        -> rejected, too broad

V0.19B  force StartupMovies copied count to 0 @ 0x25FF31
        Logos skipped
        Intro also skipped with Skip Intro OFF
        Warning message remains
        -> rejected, too broad

V0.19C  bypass MoviePlayer attachment block
        0x260244 -> 0x260257
        Logos skipped
        Skip Intro OFF still shows warning + intro cinematic
        Skip Intro ON remains independent
        -> VALIDATED
```

A slightly longer black transition is visible with Skip Intro OFF, but was
reported as minor/non-problematic.

Final rule for Skip Logos:

```text
Do not disable StartupScreens globally.
Do not empty its whole movie playlist.
Bypass only SStartupScreens attachment to MoviePlayer.
```

V0.19D freezes this exact V0.19C behavior as the validated baseline.


## V0.27-V0.28 - Native HorseCharacter resolver succeeds

V0.27 abandons all indirect horse identification paths and hooks native
`HorseCharacter` functions directly. This produced the first confirmed in-game
horse pointer and exact native stamina values.

Observed validated horse state:

```text
recovery     = 15
totalRecovery= 40
cooldown     = 2
sprintRate   = 25
stamina      = 62 / 62
```

Validated runtime fields used by Horse Sprint Duration:

```text
HorseCharacter +0x918 = StaminaSprintPercentageRate
HorseCharacter +0x9C0 = CurrentStamina
HorseCharacter +0x9C4 = MaxStamina
```

The duration feature preserves the stamina pool and scales effective duration by
reducing `StaminaSprintPercentageRate`:

```text
multiplier <= 0 -> native sprintRate
multiplier > 0  -> native sprintRate / multiplier
```

V0.28 user-facing policy:

```text
minimum 0x   = vanilla
default 5x
maximum 20x
```


## V0.29 - Correct horse movement ownership and field offsets

The V0.27 log proves that native HorseCharacter capture is correct, but also
shows `movementReady=0`. Re-audit of `HorseCharacter::GetNormalizedSpeed`
and the UE4 reflection tables identifies two independent causes:

1. The virtual call at HorseCharacter vtable `+0x5F8` already returns the
   horse movement component. Rejecting that pointer with
   `movement+0x190 == horse` was invalid.
2. Earlier candidates used neighboring UCharacterMovementComponent offsets.

Exact reflected movement layout:

```text
MaxWalkSpeed            0x1DC
MaxWalkSpeedCrouched    0x1E0
MaxSwimSpeed            0x1E4
MaxFlySpeed             0x1E8
MaxCustomMovementSpeed  0x1EC
MaxAcceleration         0x1F0
MinAnalogWalkSpeed      0x1F4
BrakingFrictionFactor   0x1F8
BrakingFriction         0x1FC
```

`SprintingMaxSpeed +0x760` belongs to
`UMayhemHorseCharacterMovementComponent`, not to `AMayhemHorseCharacter`.
Native disassembly corroborates this: movement code reads its owner horse's
`bSprinting +0x8D0` and then reads `this+0x760` when sprinting.

V0.29 therefore:
- trusts the HorseCharacter native movement getter;
- writes Horse Speed to movement `MaxWalkSpeed / MaxAcceleration`;
- writes Horse Sprint Speed to movement `SprintingMaxSpeed`;
- preserves V0.28 Sprint Duration on HorseCharacter stamina drain.


## V0.31 - Crash audit: stale Horse UObject writes

A user crash dump after a level load showed:

```text
Exception: 0xC0000005 write access violation
Fault RVA: DarksidersGenesis-Win64-Shipping.exe +0xCC0FBC
Faulting instruction: lock xadd dword ptr [rbx+8], eax
Corrupt ref-count pointer base: rbx = 0x1E3000010
Attempted write: 0x1E3000018
```

The fault occurs in the game's reference-counted destruction path, not inside
the ASI module. Combined with pre-crash reticle/HUD corruption, this is consistent
with earlier memory corruption.

Source audit found the unsafe producer in HorseFeature: raw `horse` and
`movement` pointers were retained indefinitely and dereferenced/written from
`Tick()` every frame, including after seamless travel.

V0.31 invariant:

```text
A raw HorseCharacter pointer may be dereferenced for tuning only while one of
the hooked native HorseCharacter functions is currently executing with that
pointer as its live this-object.
```

No asynchronous/per-frame restore or write through cached horse pointers is
permitted. This rule should be preserved in future camera/mount work as well.


## V0.32-V0.34 - Malformed reticle after load, focus-cycle solution

- V0.31's live-only native HorseCharacter writes eliminated the known unsafe
  cached-UObject write pattern. This rule is immutable for later builds.
- V0.32 deferred ImGui/RTV/WndProc initialization until first overlay open.
  The reticle issue reproduced without ImGui initialization, and a real
  Alt-Tab repaired it without a D3D11 ResizeBuffers call.
- V0.33 hooked `UAirshipUIManager::IsCursorVisible` and posted WM_SETCURSOR
  three times following player detection. The user reported **no visual fix**;
  the log confirms that the messages were posted, so that strategy is rejected.
- V0.34, branched from V0.33, removes the ineffective automatic refresh loop
  and adds the remappable `ReticleFocusTest` action (F5 in the included INI).
  One press queues a synthetic WM_ACTIVATEAPP/WM_ACTIVATE deactivation, then
  reactivation with WM_SETFOCUS and WM_SETCURSOR; cursor state is logged.
- **User validation, 2026-10-08:** "parfait ca marche" after testing V0.34.
  Accept the **manual F5 reticle correction** as working. This does NOT prove
  automatic correction on level load and does NOT separately validate hiding
  the cursor with F1 Hide HUD.
- The Win32 message pulse is a workaround, not a new engine-level cursor or
  reticle asset hook. Keep an explicit hotkey and preserve the safe rollback
  path for focus anomalies. No cursor refresh writes to HorseFeature memory.
- `main` should contain V0.30 GraphicsAdapter, V0.31 horse lifetime safety,
  V0.32 lazy overlay and V0.34 manually validated cursor focus pulse.


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

## StartupScreens audit after V0.43 confirmation

`README.md` links a detailed engineering audit at
`docs/STARTUPSCREENS_AUDIT.md` documenting the existing native call
sequence, the rejected v0.19A/B/C methods, relevant Unreal MoviePlayer
APIs, the cross-cursor A/B result, and the exact remaining static
reverse-engineering tasks. No hook or runtime patch changes.

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

## V0.55: native pre-warning hook and camera-distance DOF compensation

1. Direct disassembly of the exact 62,113,280-byte original EXE at RVA 0x25FEB7..0x25FEEA proves that the StartupScreenDef array (+0x50, 0x40 stride) is iterated BEFORE StartupMovies are copied at 0x25FF31. The V0.54 late change was necessarily ineffective for this loop.
2. V0.55 changes the warning mechanism: a new separate signed 7-byte detour at RVA 0x25FEBF (48 C1 E7 06 48 03 FB), using a nearby executable island that replays SHL/ADD. It saves/restores RAX and, only with boot SkipWarning=1 and an observed native end pointer exactly 0x80 bytes beyond start (2x0x40 definitions), changes RDI to RBX so the native loop at 0x25FED0 does not iterate. No heap calls, UObject writes, source array count mutation, MoviePlayer bypass, synthetic focus, or game asset renaming. Separate booleans track whether the native hook was executed and its count guard matched. Fail open on signature/count mismatch; user visual confirmation pending.
3. Manual native EXE disassembly of r.DepthOfFieldQuality registration verifies name LEA at RVA 0x12259D, the final value-slot MOV at RVA 0x1225C5, and value pointer slot RVA 0x381DEA8. The ASI verifies both 7-byte signatures and exact retail EXE identity. While Third Person or camera Zoom is active, it sets the runtime native CVar to 0, and restores the captured 0–4 native quality when the transforms turn off or on normal mod shutdown. This intentionally avoids wrong focal blur by temporarily disabling DOF; no genuine FPostProcessSettings focal recalculation is yet implemented.
4. F5 Third Person becomes default, camera distance numeric input + Default are implemented; all functional feature toggles and eight camera actions plus Reset are available in F1–F12, no ReticleFocusTest. INI revision 2105 migrates only old F5=None while preserving user choices, and all ZIP files stay at root.

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


## V0.58 live test: video + log 2026-10-09 (REJECT TPS combat mapping)

User-provided 38.95 s 2560x1440/59.94fps gameplay video and matching 21:25:09-21:27:11 mod log have been examined. The V0.58 dual-stick TPS prototype is **not validated**. In video, Strife repeatedly faces sideways or toward camera, not reliably back to the camera when shooting; movement + shooting produces visibly incoherent aim direction. Camera/character yaw outruns or diverges. User specifically requests shooter/thrower facing away from camera, actor turn with camera without overshoot, and left-stick backward/sideways strafing.

Log: exact supported EXE SHA9f470202...; V0.58 TPS strafe sample #1 at 21:26:27.673: yawDelta=48.8°, leftRaw=(1364,24142), mapped=(19074,14862). Combat aim #1 at 21:26:28.339: RT=255, RB=0, yawDelta=33.8°, native RX/RY=(18216,27237). Combat aim #120 at 21:26:30.358: RT=255, yawDelta=170.5°, RX/RY=(5436,-32313). Shutdown totals: combatAim=1022, strafeRemap=1059, nativeFireOrThrowPolls=1022, native mouse blocks=628 and right-stick filtered reads=2672. The delta changed about 136.7° in ~2 s. This *does not by itself prove* positive feedback because user may have moved camera, but shows the aim target is not stable with native camera yaw. Static 32767-full-scale synthetic stick also makes the player snap independently of actual camera rotation.

Root-cause architecture: V0.58 samples `GetCameraView` telemetry (`appliedYaw-nativeYaw`), rotates an XInput full-scale virtual right stick only during RT/RB, and rotates left stick by the same instantaneous difference. It does NOT control native Actor/Controller facing, bOrientRotationToMovement, input-to-world conversion, or native weapon/grenade target selection. Camera yaw is merely changed in *transient* `FMinimalViewInfo`; actor yaw and weapon yaw remain uncontrolled. Stronger/faster synthetic stick, arbitrary offset calibration, and static actor pointer writes would not be a justified repair. V0.57 native input gating and camera/DOF are intact.

**Next prerequisite:** audit native player-facing and movement controller using exact `DarksidersGenesis-Win64-Shipping.exe` (62,113,280 B / SHA-256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`), not currently available as mountable raw bytes. Need identify native rotation/aim/throw and movement APIs, their exact signatures and validation against EXE, so new TPS logic can lock player heading to actual camera-forward only when shooting/throwing; preserve proper strafing and disable immediately when Third Person OFF. Preserve V0.57 baseline and reversibility; V0.58 should remain test-only. User INI helpful for exact camera settings.

Skip Warning remains separate/nonfunctional: native definition span 0xC0 indicates three 0x40-size entries and existing gate only bypasses counts 1/2; last log seen=1 skipped=0. Defer as user requested. No V0.59 compiled yet; do not imply that these notes are a new build.

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


## V0.61 follow-up (2026-10-09): runtime evidence from user log

User attached `DarksidersGenesisMod(20261009-203809).log` and gameplay video `ProjectMayhem   2026-10-09 22-37-10.mp4`. The log was read fully. Video visual inspection could not be completed this turn because container/Python tools returned ClientError repeatedly; do **not** claim to have visually validated its contents.

Session 22:35:09–22:37:50:
- V0.61 ASI loaded under exact validated executable (size 62,113,280, SHA-256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`).
- Native ground aim FX hook READY; native strafe lock triggered 25 times and restored 25 times; no observed leaked flags in logged callbacks, but this is not proof of correct strafing animations or actor tracking.
- `TPS V0.61 totals: nativeStrafeLocked=25 restored=25 groundFXHidden=0` (could mean checkbox OFF or no matching native display callback; the log does **not** distinguish them).
- `combatAim=3061 experimentalStrafeRemap=0 ... liveActorYaw=24067 leftStickPassthrough=2766 movingFire=1670`. Experimental leftstick remapping remained OFF as intended.
- At 22:36:17.631: cameraYaw=-46.2°, actorYaw=-135°, actorGap=+88.8°, commandYaw=-132.8°; at 22:36:21.640: cameraYaw=-66.4°, actorYaw=-134.9°, gap=+68.5°, commandYaw=-66.4°. These demonstrate that native facing bit toggle is **not sufficient to guarantee Strife faces away from camera**. Desired command yaw may match camera yaw while actor yaw diverges; a native aim/control rotation integration is still needed.
- NO `TPS V0.61: native aim camera drift locked` line in log. Possibilities include runtime camera-pivot checkbox disabled, stale/unusable actor location, missing idle baseline when attack starts, or hook branch not reached. The log does not identify which. Instrument checkbox state and camera pivot gating conditions before making another speculative camera transform.
- Skip Warning still no effect: native warning span=0xC0/3 definitions, seen=1 skipped=0. Out of scope of present TPS test.

Next evidence-driven build priorities:
1. Add one-shot config snapshot and non-spam camera pivot gate telemetry (actor position validity/freshness, aiming state, baseline existence and pivot change) to determine why stabilization is not observed. Do not silently claim "fixed".
2. Add calls-seen and forced-hidden counters for the native `HideGroundTargetingEffect` function separately; inspect whether generic Strife aiming FX uses a different UFUNCTION instead of claiming this hook affects it.
3. Find the native authority controlling actor yaw/aim target and movement facing in the exact executable. Existing `bOrientRotationToMovement` bit test is proven to execute but **not** proven to implement strafe. Focus on actor yaw vs camera yaw and control/aim updates; avoid synthetic full-scale stick.
4. Re-examine the supplied gameplay video when local media tool access is available. In its absence, do not infer observed visual effects.
5. Preserve all validated mod subsystems (V0.55 DOF, V0.57 aim isolation, horse, HUD, reticle) and treat V0.61 as experimental. No V0.62 created or tested in this evidence-only update.

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

# V1.12 - Resource status and responsive controls - 2026-10-13

## Base and goal
Cumulative debug base: 0a13e93 (V1.11 compiled TEST), retaining previously validated functions. V1.11 as a whole has not been user validated. No main/archive writes. Archive remains at 10e0f220; main at 2bc3122c.
Goal: readable common resource controls and truthful functional status.
Acceptance: Windows x64 compilation, exactly four ZIP root files, no changes to native hooks/configuration, then user checks compact overlay and gameplay.

## Changes and preservation
Common tuner status now occupies a separate line instead of fixed x=520.
Below 560px available width, the multiplier slider and exact-value/default controls occupy separate rows.
Wrath and Ultimate explicitly say no confirmed gameplay multiplier effect. Hook installed does not mean gain path works. Synergy retains validated status only when its hook is available.
Build identity corrected: V1.11 still logged V1.10. New identity is 1.12-resource-status-responsive-ui-test.
INI, shortcuts, native LB+Y, RT/RB injection, R3, horse, cameras, HUD, Hotstreak and Synergy hooks unchanged. Existing light debug and startup timeline preserved.

## Native audit and limits
Wrath player+0xEA0 -> component+0x3A0 remains read-only candidate: historical V1.09 14 positive / 9 negative changes; AddWrath and AddChaos zero observed callbacks.
No executable or fresh game log available in this workspace to establish the true gain authority. Do not multiply sampled deltas: spend/refill/maximum changes and local ownership must first be distinguished.
Boot order reported by user: THQ -> Airship -> save warning -> controller warning -> intro.
StartupScreens flags 1/1/0 do not identify the drawing callback. SkipWarning remains OFF; native initialization preserved. Presentation/duration suppression remains unresolved.

## Build / test / rollback
GitHub Windows CI builds the cumulative ASI and DXGI loader with pinned dependencies, then checks the four root entries:
dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md.
Status before CI: EXPERIMENTAL, not yet CONSTRUIT. CI result and ZIP integrity must be checked separately.
Test: Insert, reduce window width, enable the three charge controls, enter x1/x2 then Default; verify key tables, Escape capture cancellation, INI save/reload. Compare Synergy gains at x1/x2; Wrath/Ultimate remain unverified. Check horse, RT/RB, LB+Y, LB wheel, R3 and cold boot/cursor.
Rollback: reinstall preserved V1.11 ZIP with backup of personal INI.

## Cumulative TODO / next step
- User validation of V1.12 layout and non-regression.
- Obtain exact supported EXE and fresh gain/spend log; trace actual Wrath/Chaos writes rather than repeat inactive Add hooks.
- Identify warning presentation/duration authority; preserve initialization before implementing skip.
- TPS mouse remains deferred.

## Verified delivery - 2026-10-13
CONSTRUIT / CONTROLE / EXPERIMENTAL; no user gameplay validation.
Source commit db97a0a3ea9a45821103b67266afd79662c1aa14.
Windows CI run 38417365147: success (configure, Release build, package, upload, numbered ZIP publication).
https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38417365147
ZIP publication commit be47d5a3f7f71f35f730784cb5bf6644231b44ce on debug.
Downloaded permanent raw URL successfully; root entries checked:
DarksidersGenesisMod.asi 893952 bytes; dxgi.dll 153600 bytes; DarksidersGenesisMod.ini 2484 bytes; README.md 200430 bytes.
Both binaries checked for MZ/PE headers and AMD64 machine 0x8664.
SHA256: DBE658AC84009ADA84D6A42701AD96F3706A0A6CA5522D732D210E244756548B.
main 2bc3122c1ae07b7333798462f85c1d9fd3a739f7 and archive 10e0f220b85022bc9269e0cb9390a811a3b1cf89 unchanged after build.
README bundled with build describes pre-CI status; this verified delivery entry supersedes that pending-build status.
No fresh runtime tests; Wrath/Ultimate multipliers and Skip Warnings remain unresolved as documented.
