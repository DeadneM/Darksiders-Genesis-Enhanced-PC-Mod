# V1.102 - Stoneskin, police et nettoyage du Rampage rejete

**CONSTRUITE / CONTROLEE : [57/57 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40194346246), zero erreur ou avertissement MSVC. [ZIP cumulatif immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/00581c2daeb87a192b99d2bfbac75ea1b36557c8/downloads/DarksidersGenesis_V1.102_STONESKIN_MENU_FONT_TEST.zip). Nouveautes EXPERIMENTALES, NON VALIDEES EN JEU.**

Base retenue : acquis V1.98 et World Ender sur War V1.100, confirme par l'utilisateur. Geyser/Clone/Caltrops restent valides. Source cumulative `042efcc61588de9415e43b81164fb32b93d449dd`. Retour V1.101 : Rampage Strife rejete (icone disparue, comportement anormal) ; aucun succes declare pour cette experience.

- Rampage Strife : fichiers du module, factory de mouvement et reflexion supprimes, ainsi que leurs tests specifiques. La valeur Strife=5 revient a Native, meme si un ancien INI la contient. Rampage War reste natif.
- Refus d'un transfert indisponible : classe/icone et debloquage d'origine conserves, affordable=false. Native 0x664DC1/0x664DC8/0x664DCF verifie ces sorties avant le dispatch ; aucun autre pouvoir lance par repli. Le rendu du HUD reste a confirmer en jeu.
- Stoneskin : choix 6 pour les deux personnages et trois emplacements. War natif ; Strife utilise sa protection/cout/duree natifs avec un geste propre et un montage compatible. Materiau de pierre War, onde de choc de montage et effets d'arme non copies. Stinger accepte reste natif. Protection, variantes et expiration sur ce nouveau build demandent un essai.
- Police : About → Menu appearance, 10–32 px, saisie manuelle et Default 13 px. Scope menu/popups restaure l'echelle ImGui hors menu. Persistance, bornes/NaN, ancien INI et vrai contexte ImGui controles. Aucun rendu en jeu presume.
- 169 cles INI dont une seule nouvelle ([Overlay] FontSize) ; 168 anciennes valeurs et 55 ID Hotkeys conserves. Les modules World Ender, Clone, Caltrops, visee/camera et Stinger sont compares a la base, sans modification ; le prefixe accepte des montages est identique apres retrait du seul bloc Rampage.
- 166 anciennes archives comparees par SHA256, 167 ZIP au total. main/archive/v1.43 verifies intacts ; debug uniquement.
- Source compilee `be2cfe17cd6d0e88819cc7daf474440da1a202bb` ; commit archive `00581c2daeb87a192b99d2bfbac75ea1b36557c8`.
- ZIP : 973932 octets, SHA256 `586BDEFE15B059E5C47DD7A5999F818D8C2E2EF36D8EB115002FFA8CAF3690B7`. Quatre fichiers racine, CRC valide, ASI/dxgi AMD64, README/INI compares au commit compile. Lien public HTTP 200, octets identiques verifies.

Installation manuelle jeu ferme, conserver son INI/raccourcis/PAK/sauvegardes. Aucun lancement ou remplacement de fichier du jeu par l'agent. [Contrat, limites et essais groupes](V1_102_STONESKIN_MENU_FONT_TEST.md).

Essais : police 18/24 et reouverture ; Stoneskin sur les deux personnages (cout, protection/expiration, commandes, retour Native) ; icone du slot Strife avec ancienne selection Rampage ; bref controle des quatre transferts valides. P2 reporte. La suite se fera sur ce retour groupe ; Rampage et reception lourde restent ouverts.
