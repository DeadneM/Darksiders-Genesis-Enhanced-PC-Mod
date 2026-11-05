# V1.157 — livraison cumulative et installation

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ, EXPÉRIMENTAL en jeu.** Base main V1.153 acceptée conservée ; cumulative debug V1.156 reprise, avec ses effets natifs du cheval et son écharpe Stoneskin. Correction des clics souris du menu, maintien du lifecycle dash/stomp et suppression du blocage global des F1–F12 hors saisie/capture. Le retour négatif V1.155 est pris en compte, sans annoncer sa continuité comme validée.

[ZIP vérifié, deux binaires à la racine](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/eef98d95f7eb24389b9990db69b2178a60293ef8/downloads/DarksidersGenesis_V1.157_MENU_INPUT_LIFECYCLE_TEST.zip) · [Compilation Windows et 82/82 tests](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/42123214553) · [Périmètre et limites](V1_157_MENU_INPUT_REVIEW.md) · [Cheval/écharpe et causes observées](V1_156_NATIVE_MOUNT_SCARF_REVIEW.md).

Source `6b382f3d410826a13f189b2eabcccf7dfe58a0a4`, archive `eef98d95f7eb24389b9990db69b2178a60293ef8`. ZIP 2,433,774 octets ; SHA256 `A8AC7E494E22D26AEF74CD764054BAC8751B3981B39F561CA99C2E81502EFAA8`. 221 anciennes archives inchangées, 222 au total. Les 248 valeurs INI et les 55 identifiants Hotkeys restent conservés. Main `563ae9ca8c2647689af8464d320544afe55b1894`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` et tag v1.43 inchangés. Aucune compilation locale.

## Vérification et installation

82/82 tests GitHub ; aucun avertissement C++ détecté. Les avertissements de dépréciation CMake/MinHook et des actions Node sont conservés dans la preuve CI, sans être présentés comme des avertissements de l'ASI. ZIP CRC, en-têtes AMD64, identités incorporées, lien public HTTP 200 et sélection V1.157 par l'updater contrôlés. Defender 1.463.140.0 : aucune menace sur ZIP et binaires extraits ; protections actives et inchangées.

L'utilisateur a demandé l'installation. Le jeu a été constaté fermé avant la transaction et les verrous vérifiés ; aucune nouvelle confirmation nécessaire. Installation terminée le 5 novembre 2026 à 23 h 46 (Europe/Paris), des seuls DarksidersGenesisMod.asi et dxgi.dll. Empreintes installées identiques au paquet. Sauvegarde vérifiée : `before-V1.157-20261105-234608-bdd650` sous le dossier local audit-v131/install-backups. Les binaires précédents correspondent à V1.155. Aucun lancement du jeu par l'agent.

INI personnel strictement identique avant/après, SHA256 `277B9B187D43EB23090C18CD7DF3E86C617554095C8BFA03A18AACD3136299C1` ; raccourcis conservés. Engine.ini, exécutable original et inventaire des PAK inchangés. Aucun INI ni README dans le ZIP. Les sources synchronisées sources/ et les changements locaux préexistants sont laissés intacts.

[CI](evidence/V1_157_CI_CHECK.json) · [Paquet/updater](evidence/V1_157_PACKAGE_CHECK.json) · [Antivirus](evidence/V1_157_DEFENDER_CHECK.json) · [Installation](evidence/V1_157_INSTALL_CHECK.json) · [Deux relectures et fichiers conservés](evidence/V1_157_SCOPE_AUDIT.json).

## Essai groupé restant

1. Menu : clics répétés sur cases/sliders, modifications prises en compte sans le fermer ; F1–F12 configurées hors saisie, puis réaffectation/saisie sans activation parasite. Un appui commencé avant ouverture doit se relâcher normalement ; fermer et Alt-Tab sans clic/tir coincé.
2. Dash/stomp : comportement tap/hold conservé lorsqu'une action native arrive pendant l'affichage de l'éditeur ; aucun mouvement provoqué par les seuls clics d'édition.
3. Premier cheval du chapitre 1 sans rappel : flammes/crinière/queue, puis descente/rappel et menus. La cause initiale de l'inactivité n'est pas démontrée ; la reprise cible l'état entièrement inactif relevé.
4. Strife Stoneskin : écharpe entière et feu pendant le pouvoir, rendu d'origine après expiration ; refaire avec une autre armure. Les couleurs et la découpe restent à vérifier en jeu.

GPU après fermeture/redémarrage VALIDÉ par l'utilisateur ; ne pas le remettre dans la liste à retester. Coop/P2 à la fin, comme convenu. Aucune mesure FPS ni validation visuelle de ces nouveaux correctifs n'est annoncée. Retour arrière : restaurer les deux binaires sauvegardés sans remplacer l'INI personnel. Suite : analyser le retour groupé, puis les essais coop différés.
