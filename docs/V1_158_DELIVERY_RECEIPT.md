# V1.158 — livraison cumulative et installation

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ, EXPÉRIMENTAL en jeu.** Base cumulative V1.157 ; main V1.153 inchangée. Clics du menu acceptés, actions favorables avec réserve. GPU accepté, coop différée. Les correctifs précédents écharpe/cheval n'ont pas été déclarés validés après le retour négatif.

[ZIP cumulatif vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/39acc77193470acf40fc0f6e8fd253fbcdca24f0/downloads/DarksidersGenesis_V1.158_MOUNT_CLOTH_PAD_ORBIT_TEST.zip) · [GitHub Windows, 82/82 tests](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/42132284456) · [Périmètre, preuves et limites](V1_158_MOUNT_CLOTH_PAD_REVIEW.md).

## Changements

- Tissu Stoneskin : sources natives A et Molten A avec émission, sans substitution de Base_Color ; corps minéral et restitution inchangés. Setter de texture et sources B retirés, deux matériaux et cinq ressources réellement demandées. Les trois anciens marqueurs de texture/B sont absents du binaire vérifié. Le rendu reste à juger dans le jeu.
- Cheval chargé avec le niveau : disponibilité réessayée brièvement, capacité réservée distinguée du nombre d'éléments, passifs déjà actifs laissés intacts sans bloquer les autres. Le Pawn réellement possédé est vérifié par classe et liens réciproques, sans exiger un ancien personnage distinct. Une seule activation des passifs manquants, pas de force-active permanent.
- Menu : orbite au joystick configuré maintenue pour P1/P2, édition souris sans rotation ni recentrage du curseur. Les boutons du menu et leurs relâchements restent réservés ; tir, XInput, roues et affectations conservés.

## Contrôles et intégrité

Source `5f1591a85630ea0af128b4bd5c25996072bd5e3a` ; archive `39acc77193470acf40fc0f6e8fd253fbcdca24f0`. ZIP 2,430,702 octets, SHA256 `202670EBA556CED21056A4A16D36C7C38F2AA0DEA8AE584C14DF2443B6F55862`. Deux binaires AMD64 à la racine, aucun INI/README ; CRC et lien public HTTP 200 vérifiés. Updater : V1.158 sélectionnée. 222 anciennes archives conservées, 223 au total ; 248 valeurs et 55 identifiants Hotkeys inchangés. Main/archive/tag v1.43 inchangés.

82/82 tests Windows GitHub, aucun avertissement C++ détecté. Les dépréciations externes CMake/MinHook et Node restent consignées dans le relevé CI. Aucun build local. Defender 1.463.140.0 : aucune menace sur le ZIP et les deux binaires, protections actives et inchangées. Deux relectures du périmètre par le même agent : contrats/usages puis init/restauration et comparaison des fichiers ; aucune autre suppression spéculative.

[CI](evidence/V1_158_CI_CHECK.json) · [Paquet/updater](evidence/V1_158_PACKAGE_CHECK.json) · [Defender](evidence/V1_158_DEFENDER_CHECK.json) · [Deux passes et retrait des marqueurs](evidence/V1_158_SCOPE_AUDIT.json).

## Installation et rollback

Dans la continuité de la demande d'installation précédente, jeu constaté fermé, sauvegarde et verrous contrôlés. Installation le 6 novembre 2026 à 00 h 57 Europe/Paris des seuls ASI et dxgi.dll, empreintes identiques au ZIP. Sauvegarde V1.157 vérifiée : `before-V1.158-20261106-005712-25747b` dans audit-v131/install-backups. INI personnel strictement identique (`277B9B187D43EB23090C18CD7DF3E86C617554095C8BFA03A18AACD3136299C1`) ; raccourcis, Engine.ini, exécutable et PAK inchangés. Aucun jeu lancé par l'agent. [Preuve d'installation](evidence/V1_158_INSTALL_CHECK.json).

Retour arrière : restaurer les deux anciens binaires sauvegardés, sans remplacer l'INI. Les sources synchronisées et les deux changements locaux préexistants restent intacts.

## Test groupé restant

1. Menu ouvert : tourner au stick caméra choisi et continuer à cliquer/éditer à la souris sans mouvement parasite ; fermeture et Alt-Tab normaux.
2. Charger directement le niveau déjà à cheval : vérifier flammes/crinière/queue sans descendre/rappeler. Le scénario antérieur a été réutilisé, aucune nouvelle capture de l'ancienne version n'est demandée. La cause initiale exacte de l'inactivité n'est pas annoncée démontrée.
3. Strife Stoneskin : écharpe complète et feu présent, puis restitution après expiration ; armure actuelle puis une autre, avec variante Molten si équipée. Gestes/durée et autres pouvoirs inchangés.

Ni les fixtures ni les compteurs ne prouvent le rendu, le coût FPS ou la coop réelle. GPU déjà validé, coop à la fin. Suite : exploiter le retour sur ces trois changements avant de modifier d'autres effets.
