# V1.103 - reception lourde, interruption native MOVE

**CONSTRUITE / CONTROLEE : [57/57 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40199153318), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/74cb774b480f1cfb5cd2ed6e8d8a15ccd0fd39ec/downloads/DarksidersGenesis_V1.103_HEAVY_LANDING_NATIVE_INTERRUPT_TEST.zip) - [Recu](V1_103_DELIVERY_RECEIPT.md). Nouvelle reception lourde EXPERIMENTALE, NON VALIDEE EN JEU ; base V1.102 validee pour Stoneskin et police.**

25 octobre 2026. Base debug V1.102 validee par l'utilisateur pour Stoneskin et police. Acquis Geyser/Clone/Caltrops V1.98 et World Ender War V1.100 conserves. Nouvelle recuperation EXPERIMENTALE, non validee en jeu.

## Objectif et critere de reussite

Raccourcir l'immobilisation apres la reception lourde automatique d'une chute, uniquement quand le joueur demande de se deplacer. Sur Strife et War, vue native/TPS, une meme chute doit permettre une reprise plus rapide avec ON, conserver l'impact, finir proprement et permettre une seconde reception. Sans deplacement, conserver l'animation. La reception d'attaque plongeante et les dashs sont hors de ce changement.

## Comparaison des deux chemins

L'ancien module modifiait AllowPlayerMovement (+0x161) et AllowedActionsFlags MOVE (+0x80, bit29) a chaque frame. Ce sont des permissions de marche, pas la fin du verrou d'animation. Le clip Strife_Jump_Land_Heavy active bEnableRootMotion ; il contient une fenetre AnimNotify_EnableAbilityInterrupt MOVE a 0,668465 s. War_Jump_Land_Heavy_Montage ouvre la meme fenetre a 0,402459 s. Durees d'origine : 1,633333 et 1,6 s. Les effets d'impact du montage se trouvent a 0,016657 et 0,052577 s ; les derniers sons initiaux Strife identifies sont a 0,127431 s.

Le nouveau module ouvre seulement la demande native d'interruption MOVE a partir de 0,15 s d'ability, apres 50 ms continus au sol Walking. Il appelle la meme fonction que NotifyBegin : RVA 0x5A9D60, ABI void(ability, uint8 cause, bool enabled). MOVE est EAbilityInterrupt=1, distinct de l'action MOVE=29. Signature du prologue, appel du NotifyBegin et instruction de mise a jour du bit controles. L'EXE audite a SHA256 9F4702024DF5EEA1D51DF7745B0AD1EA95B97009982F73DDC1218C53DFF33D54.

Preuve native : NotifyBegin 0x5A85A9 appelle 0x5A9D60. Celle-ci met a jour le masque temporaire +0x120. Le tick 0x5B4739 compare ce masque a AbilityInterruptsFlags +0xB0 puis appelle le setter virtuel +0x2A0 (0x599310) ; celui-ci met a jour les actions et leur evaluation native. La demande de deplacement reste donc une fonction du jeu reaffectable, aucun bouton impose. Le mod n'appelle pas Finish et ne change ni position/temps de montage, vitesse, vecteur de deplacement ni etat de visee.

Contrat borne : deux joueurs locaux, autorite, controller/pawn reciproques, personnage vivant a pied, classe HeavyLand exacte, movement component propre, ability active sans fin en attente, helper MayhemMontagePlayer et montage/package originaux de chaque personnage, RateScale 1. Toute difference refuse l'experience. Les autres pouvoirs, dashs, knockdowns, swaps et transformations restent exclus.

Avant la fenetre native d'origine, OFF/perte de contexte restaure uniquement la fenetre ajoutee, par les deux API natives ; les autres bits ne sont pas ecrases. Une fenetre deja ouverte par le jeu n'est jamais revendiquee. Apres le temps natif d'origine, son MOVE appartient au jeu : le marqueur est oublie. Identites index/serie verifiees avant restauration, puis de nouveau apres le premier appel natif ; aucune restauration hors game thread pendant le shutdown.

Limite : les temps compares sont ceux de l'ability, avec le montage original/rate natif 1 et les points extraits des assets. La synchronisation effective, le blend, les sons et les transitions du moteur demandent un essai en jeu ; les fixtures ne les prouvent pas. Aucun resultat P2 annonce.

## Double revue et Rampage

Premiere passe : EXE exact, enum MOVE=1, appel de NotifyBegin, masques demandes/effectifs, reconciliation native et marqueurs des deux montages verifies hors ligne. Deuxieme passe : code/fixtures verifies pour sol/impact, proprietaires, package/rate, OFF, pause/roues/cinema, instance recyclee, autres fenetres et fin native. Les deux passent par le meme point natif ; l'ancienne ecriture de permissions de marche a ete retiree, pas cumulee avec ce candidat.

Rampage Strife : packages, squelette Strife_Skeleton, cinq slots du graphe et FName DefaultSlot sont corrects dans les preuves consultees. L'initialiseur 0x1D7590 confirme DefaultSlot a base+0x3A32340. Aucune erreur de ces constantes n'a ete identifiee ; leur etat lors du refus ancien reste inconnu ; aucune preuve ne designe encore la condition precise de Prepare qui echouait. Le log V1.101 consulte contient READY mais aucun CAST. Il serait injustifie de relivrer le meme montage War sur un slot non evalue. Le code rejete reste absent ; une capture de refus ciblee est preparee hors ASI, sans ecriture ni lancement du jeu. Jeu ferme lors du preflight courant, pas de nouvelle preuve runtime.

## Conservation et essais

Stoneskin, police, Stinger, Geyser, Clone, Caltrops, rayon War, tirs, camera TPS/native, controles XInput/souris, roues, horse, holsters et autres acquis compares au commit de base. Aucun changement de config/defauts, 169 cles et 55 ID Hotkeys. 167 anciennes archives et main/archive/v1.43 intacts. La case Heavy landing movement recovery existante reste OFF, sa description precise maintenant l'interruption native.

1. Installation manuelle jeu ferme, garder l'INI et ses raccourcis. Player movement → Heavy landing movement recovery.
2. Avec Strife, meme chute assez haute, maintenir le deplacement a la reception : comparer OFF/ON en vue native puis TPS. Verifier impact, reprise, absence de glissement/blocage, puis seconde chute.
3. Refaire sans bouger : la reception et ses effets doivent se jouer normalement. Verifier ensuite dash, visee et attaque.
4. Refaire avec War. Retour OFF pour comparer et si anomalie. Les autres pouvoirs valides n'ont pas besoin d'un nouveau test exhaustif.

Repli V1.102 avec la case OFF ; INI/PAK/sauvegardes conserves. Aucun remplacement de fichiers du jeu ou lancement par l'agent. Suite : retour groupe sur cette reception, refus Rampage cible, puis essais P2 differes et FX P2 reportes a la demande de l'utilisateur.
