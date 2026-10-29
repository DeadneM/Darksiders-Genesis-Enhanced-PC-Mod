# V1.131 - aide TPS globale ou par munition

29 octobre 2026. CONSTRUIT / CONTROLE ; NOUVELLE AIDE NON VALIDEE EN JEU. Base V1.130 explicitement validee et installee. Aucun jeu lance ni compilation locale ; installation ensuite demandee et verifiee, INI personnel conserve. main reste V1.124 ; aucun changement main/archive/v1.43.

Source `5d241d0e45166a927d6278d93bb5d65631403b35`, archive `73c494fb4acdd7d33125427dd2b084b921a434bc`. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40947674176) : **68/68 suites**, zero avertissement/erreur MSVC. Nouvelle suite AimAssist : progression3D, limite6degres, obstacle proche, valeurs invalides, ancien INI et mode/sept valeurs/raccourcis. ProjectileSpawn : aide positive pour les sept classes, P2, arme de faisceau et exclusion du secondaire, entree du solveur Lava, WorldEnder/vue native/pause preserves. Autres suites V1.130 conservees. Les fixtures ne prouvent pas le ressenti dans le jeu.

## Reglage et limites

Camera -> TPS controls and aiming -> Added TPS aim assist, groupe ferme initialement. Mode Global ou Per ammunition ;0..100%, saisie et Default. Les valeurs globales et les sept valeurs restent memorisees en changeant de mode. Standard/Charge/Graviton/Lava/Beam/Nature/Static, meme valeur normale/Hotstreak.0 par defaut retrouve V1.130 ; c'est un pourcentage de correction ajoutee, pas une proportion vanilla.

Un seul calcul commun de cible3D avant les consommateurs existants. Utilise la cible choisie par le moteur, index/serie/racine verifies ; cone avant6degres, priorite a une collision plus proche et verification de visibilite. Pas de scan d'acteurs, nouveau hook ou ecriture d'angle/corps/input ; garde la physique, les coefficients de vitesse/gravite, les dommages et portees natifs. Lava passe ce point au solveur natif vitesse/pitch ; mode3D OFF garde sa hauteur/arc natif. Statique conserve le contact physique et les faisceaux secondaires restent exclus.0 n'effectue aucun nouveau travail de cible/visibilite.

Si le moteur ne fournit pas de cible native, ou si l'arme du faisceau est ambigue, aide refusee ; pas d'attribution devinee. Disponibilite a cheval/coop/certains Hotstreak et effets positifs restent a valider. World Ender/autres pouvoirs exclus. Camera, RawInput/XInput, raccourcis, apparences/pose UI V1.130 et autres fonctions conservees. Neuf nouvelles cles,227 au total ;218 anciennes valeurs et55ID Hotkeys preserves. [Audit et limites](evidence/V1_131_STATIC_REVIEW.json), [validation utilisateur V1.130](evidence/V1_130_USER_VALIDATION.json), [contrat](V1_131_TPS_AIM_ASSIST_TEST.md).

## Livraison et essai

[ZIP cumulatif public](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/73c494fb4acdd7d33125427dd2b084b921a434bc/downloads/DarksidersGenesis_V1.131_TPS_AIM_ASSIST_TEST.zip), 2426222 octets, SHA256 `17E4F72CA4B0E61AA200893AC9AA12F601C3D1E0BC0A74F3C35D50F3BA73D07D`. Quatre racines ASI/DXGI/INI/README, methode0, CRC valide, binaires AMD64 ; INI/README compares a la source compilee.195 anciennes archives Git identiques,196 au total ; updater debug telecharge les memes octets. Defender ZIP/contenu sans menace, protections actives, aucun reglage/exclusion change. [CI](evidence/V1_131_CI_CHECK.json), [ZIP](evidence/V1_131_PACKAGE_CHECK.json), [updater](evidence/V1_131_UPDATER_CHECK.json), [Defender](evidence/V1_131_DEFENDER_CHECK.json).

Garder l'INI personnel. Essayer Global0/25/50/100 sur une cible juste a cote du reticule, puis Per ammunition avec un type0 et un type75. Tester les sept types et Hotstreak, surtout Lava/Charge/Graviton et les rebonds Static ; ennemi en bord d'ecran/derriere obstacle non attire. Verifier rappel des valeurs, retour0, vue native et main vide en pause/roues. V1.131 ensuite installee sur demande ; aucune nouvelle validation en jeu supposee. Retour0 ou rollback V1.130 possible.

## Installation locale demandee ensuite

V1.131 installee sur demande explicite, jeu ferme. Sauvegarde V1.130 `before-V1.131-20261029-212759-8dd7d05e`, anciens ASI/DXGI/README/INI/journal verifies avant copie ; trois fichiers remplaces depuis le ZIP controle. Verification independante des hashes, AMD64 et identite1.131, INI byte-identique, EXE original identique et PAK non ecrits (inventaire noms/tailles/dates UTC identique). Jeu non lance ; nouvelle aide toujours a valider. Ancien INI conserve : nouvelles valeurs absentes chargent0 par defaut et sont ajoutees a la prochaine sauvegarde du menu. [Preuve](evidence/V1_131_INSTALL_CHECK.json).
