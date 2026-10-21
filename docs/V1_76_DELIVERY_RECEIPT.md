# V1.76 — reçu de livraison, 21 octobre 2026

21 octobre 2026. **CONSTRUITE / CONTRÔLÉE / NON INSTALLÉE PAR L'AGENT ; nouveauté EXPÉRIMENTALE, non validée en jeu.** Base validée V1.73 dans les essais rapportés, suite cumulative de V1.75 dont les nouveautés attendent le test utilisateur. V1.75 a été retrouvée installée sur disque ; ne pas transformer cela en validation de ses fonctions.

Nouveau choix OFF par main pour Strife normal à pied : modèle masqué, bras au repos dans les poses de tir par copie de rendu, tirs/sockets natifs conservés. Ranger les armes et rester immobile brièvement pour enregistrer la pose. Mêlée, actions inconnues, cheval et ultime gardent leur présentation native. Effets propres à une main filtrés ; effets communs connus conservés avec une arme visible et retirés avec les deux OFF. **OFF universel de tous les effets/formes toujours ouvert**, six capacités encore en adaptation.

[CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39614279357) : **46/46 suites, zéro avertissement MSVC**. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/026664c3e06a85b3714af5591ff3bef135ff2cb1/downloads/DarksidersGenesis_V1.76_FOOT_PISTOL_OFF_TEST.zip), 850967 octets, SHA256 `9FF084D8FBF542EDC0D65D78F06A9DC3D06FC363D2EB5BFEBE72CE93ED6B2A27`. Quatre fichiers racine/CRC/AMD64, 157 clés INI et 55 ID Hotkeys conservés, 140 anciennes archives inchangées. Installation manuelle ultérieure ; aucun fichier du jeu remplacé par l'agent. [Critères et limites](V1_76_FOOT_PISTOL_OFF_TEST.md).

Tester plus tard dans le même lot : OFF droit/gauche/deux puis Native, tirs normaux/Hotstreak et dash, mêlée/roues/formes/P2 ; visée sans tir et réception lourde V1.75. [Recherche des capacités](V1_76_WRATH_ADAPTATION_RESEARCH.md). HUD général en surveillance, autres FX P2 reportés, notification de mise à jour à confirmer.

## Construction et intégrité

Source `2ead453b0d53f440eb0d7f7cdd6aae6be192e49c`, archive `026664c3e06a85b3714af5591ff3bef135ff2cb1`. [CI](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39614279357), 46/46 suites ; suites PistolVisual, DashCombat, MountedHolsters, ManualAim, HeavyLanding, UserDefaultsConfig et les autres contrôles passent. Compilateur/analyse MSVC sans avertissement ; avis de dépréciation des outils CMake/MinHook et Node distincts.

Deux passages préalables ont échoué exclusivement dans le nouveau test : déclaration de pointeur corrigée, puis lecture avant écriture différée corrigée par SaveNow. Aucun ZIP de ces passages n'a été publié. Les tests du rendu/visibilité/effets étaient déjà passés avant le second échec de vérification INI. Le passage complet final fait référence ; simulation native et intégrité ne prouvent pas le rendu dans le jeu.

[ZIP public immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/026664c3e06a85b3714af5591ff3bef135ff2cb1/downloads/DarksidersGenesis_V1.76_FOOT_PISTOL_OFF_TEST.zip), **850967 octets**, SHA256 `9FF084D8FBF542EDC0D65D78F06A9DC3D06FC363D2EB5BFEBE72CE93ED6B2A27`. HTTP 200/hash, CRC, deux binaires AMD64 et exactement quatre fichiers racine vérifiés :

| Fichier | Octets | SHA256 |
| --- | ---: | --- |
| DarksidersGenesisMod.asi | 1504256 | `A67F85F1678BC2130B212AB327C43CECCD7855342CF98149EEFF21BD2D1D1F02` |
| DarksidersGenesisMod.ini | 4303 | `B6608512171FC90632A408A89AA1EB2FF264ECDC027DA8DBD665F8713965B16E` |
| dxgi.dll | 155136 | `CB85E2D51DB907B98A3E1D1825E1AB25276E5A1997BCEDAF0EAA46C2288E8AAB` |
| README.md | 16385 | `10AF5C8D023484D193F81C3E22F52633CD8178118252D8B29EB6A0134A669114` |

Les 157 clés fournies et leurs valeurs antérieures sont identiques à V1.75 ; 55 ID Hotkeys inchangés. Native (0) reste fourni pour chaque main, visée manette OFF/OFF et HeavyLandingRecovery=0. Aucun INI personnel remplacé. 140 anciennes archives inchangées, 141 ZIP au total. main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` et tag v1.43 `902861eb1ff34c14dc5ded092581b360767da817` conservés. Douze modules de V1.75, dont visée, caméra, ressources, sauts, grappin et nouveautés V1.75, identiques dans Git ; seuls les raccords de présentation explicitement décrits changent.

Le README du ZIP décrit la préparation avant CI ; le présent reçu complète son statut sans réécrire l'archive. ASI retrouvée installée : V1.75 SHA256 `0F41EA56889E553494DFAE77F392A62CD4C8B933F4C3293DE08E9CE5D6AECD56`. L'agent n'a remplacé aucun fichier du jeu, PAK, sauvegarde ou INI ; aucun lancement du jeu. Le relevé ponctuel a rencontré GAME_NOT_RUNNING, sans relance. L'utilisateur installera V1.76 manuellement après sa session.

## Essai ultérieur et retour arrière

1. Strife à pied : ranger, attendre « OFF ready », comparer OFF droit/gauche/deux puis Native. Contrôler modèle, bras, autre main, mêlée et changement de personnage ; répéter en native/TPS et P2 si Strife y est joué.
2. Standard et munitions spéciales normal/Hotstreak : direction, tirs, dégâts et effets conservés selon le périmètre indiqué. Déplacement + visée + tir + dash ; roues, équipement maintenu, coffre et rangement.
3. Cheval/ultime, pause/cinématique et Alt-Tab : leurs poses restent natives ; retour à pied et Native sans modèle manquant. Les effets partagés ne sont pas séparés avec un pistolet visible.
4. Dans ce même lot, visée seule un/deux boutons et réception lourde V1.75 OFF/ON, Strife/War, native/TPS. Aucun de ces tests n'est réclamé immédiatement ni supposé fait.

Rollback : chaque main sur Native ; Heavy landing movement recovery OFF et deux commandes Aim without firing OFF si nécessaire. Retour complet avec les binaires V1.73 sauvegardés et l'INI personnel conservé ; mode 3 inconnu aux versions antérieures revient à Native. [Détails et limites](V1_76_FOOT_PISTOL_OFF_TEST.md).

Suite : adaptation des six capacités via lancement, composants et montages compatibles ; compléter OFF pour formes et effets partagés. HUD en surveillance, autres FX P2 reportés. Les candidats restent expérimentaux jusqu'au retour utilisateur.
