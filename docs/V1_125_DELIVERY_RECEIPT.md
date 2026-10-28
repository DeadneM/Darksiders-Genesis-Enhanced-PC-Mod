# V1.125 - consolidation controlee apres promotion V1.124

28 octobre 2026. **CONSTRUIT / CONTROLE ; NETTOYAGE NON VALIDE EN JEU.** Base V1.124 validee et explicitement promue sur main au commit `e2e519f7ea84d4b773fd7a31611779ae68308696`, sans recompilation ni remplacement du ZIP teste. archive et v1.43 preserves. Suite uniquement debug. [Preuve de promotion](V1_124_MAIN_PROMOTION.md).

Source `c7f8c29fe7398149b848fcbe17b7ef16d6735009`, archive `2dd91999a23acbed02dd5c2b4b2a7aedd6f0a6ac`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40665878336) : **65/65 suites**, zero MSVC warnings/errors. Aucun build, lancement ou installation locale.

## Changement retenu et conservation

Deux passes confrontees (graphe/references, puis producteurs/consommateurs) sur145fichiers/52cpp/24020lignes. Aucun module entier abandonne prouve. Copie/champ de telemetrie cheval sans lecteur retires ; heartbeat producteur et vrais consommateurs conserves. Clone sur War : trois tableaux d'une entree et deux tables de noms deviennent scalaires ; boucle d'un tour/branches Strife non appelables remplacees par le meme contrat War. Six appels natifs/son gardent leur ordre et leurs arguments normalises. Les gardes faibles/proprietaires et le repli natif restent.

Deux lignes physiques nettes de production en moins ; simplification de structures sur des lignes conservees. Aucun gain FPS promis. Tests existants conserves ; cas Strife natif et incoherences acteur/animation/os ajoutes au meme test. [Deux audits et limites](V1_125_DOUBLE_AUDIT.md).

Geyser/ForeignMontage, Stoneskin/materiaux/effets, Caltrops et son appel unique valide, Rampage, World Ender, visee/souris/manette/camera, HeavyLanding et les autres fonctions sont inchanges.212valeurs INI et55ID Hotkeys conserves ; INI personnel a garder. Cette version n'ajoute aucun hook, appel natif, ecriture de mouvement ou changement de timings.

## Recovery et apparences : conclusions, pas de correction inventee

EXE exact et27assets natifs controles hors jeu. Le delai global unique n'est pas demontre : montage, fondu, permissions, demandes/interruption, timers dash et fin de capacite sont distincts. MOVE reception lourde commence a0,833373s/0,813425s dans les assets ; les seuils0,668465/0,402459 concernent d'autres actions. Description historique corrigee, code HeavyLanding valide inchange. Coffres Strife2,7333s/War2,1667s, fondus0,1s ; ces temps d'asset ne prouvent pas la duree du temps mort ressenti. [Carte complete et prochaine mesure](V1_125_RECOVERY_TIMING_MAP.md).

L'utilisateur rapporte des apparences sans effet/peu interessantes. Onze des43groupes sont Native/Hidden seulement ; la fabrique actuelle ne couvre que certaines nouvelles particules et exige l'asset cible charge. Les autres composants/modeles/materiaux restent a traiter selon cas precis. Pas de reparation universelle ni suppression de reglages revendiquee. Rampage accepte dans son retour, coop differee ; effets P2 semblaient bons et HUD general plus reproduit.

## Livraison et test

[ZIP cumulatif immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/2dd91999a23acbed02dd5c2b4b2a7aedd6f0a6ac/downloads/DarksidersGenesis_V1.125_CONSOLIDATION_RECOVERY_MAP_TEST.zip), 2355221octets, SHA256 `4C07060ADB4762F71C439896837F2B73BF15C07A1D497AF0203D0FF837B3636A`. Quatre racines exactes, methode0/CRC et ASI/DXGI AMD64 controles, README/INI conformes a la source compilee. Les189anciens blobs ZIP sont conserves,190archives au total. Updater debug selectionne V1.125 et telecharge les memes octets. main reste sur V1.124 ; pas de nouvelle release creee.

Defender ZIP et dossier extrait : exit0/aucune menace, protections actives avant/apres, pas d'exclusion ni changement de reglage. Ce controle n'est pas une certification universelle.

Test utile du nettoyage : Clone sur War deux fois (effet/sons, cout, reprise marche/saut), Clone natif Strife, puis cheval/roue rapide. Les autres pouvoirs et profils camera restent ceux acceptes. Recovery general et apparences ne sont pas annonces corriges ; coop plus tard. Rollback : meme ZIP V1.124 sur main, garder l'INI personnel. Aucun fichier du jeu remplace par cette livraison.

Preuves : [CI](evidence/V1_125_CI_CHECK.json), [ZIP](evidence/V1_125_PACKAGE_CHECK.json), [updater](evidence/V1_125_UPDATER_CHECK.json), [Defender](evidence/V1_125_DEFENDER_CHECK.json), [confrontation](evidence/V1_125_CLEANUP_COMPARISON.json), [assets Recovery](evidence/V1_125_RECOVERY_ASSET_MAP.json).
