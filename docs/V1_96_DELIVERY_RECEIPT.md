# V1.96 - Livraison du lot des capacites

CONSTRUITE / CONTROLEE : [55/55 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40119582099), zero avertissement ou erreur MSVC. [ZIP cumulatif immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/fbb17b46f293e9c64fb59ebfa7ae5d4cb92bc923/downloads/DarksidersGenesis_V1.96_FOREIGN_POWER_BATCH_TEST.zip). Les trois nouveaux transferts sont EXPERIMENTAUX ; aucun resultat en jeu suppose.

- Base cumulative : V1.95 / 5b35a2be7f6a53d51f9b953f9d14a8bab93f14fc. Caltrops sur War et Geyser sur Strife : effets/degats confirmes par l'utilisateur ; animations incorrectes et conservees.
- Ajouts : Clone et World Ender sur War, Rampage et quatre suites sur Strife. Composants propres au lanceur, montages prives, chronologie native et adaptation du SpawnSocket de Rampage sur la racine de Strife. Poses idle, charge et rendu/degats encore a tester.
- Retrait : toute adaptation Stoneskin et son choix explicite. Ancien choix 6 = Native ; War garde son pouvoir original par Native. Trace DispatchEvent Geyser temporaire supprimee.
- INI 168 cles et defauts, 55 ID Hotkeys, controles, tirs et cameras inchanges. 160 anciennes archives conservees, 161 ZIP au total. main/archive/v1.43 verifies intacts ; P2 differe.
- Source compilee : `a1d9f2f2bd9c1181f491a1748f39705f7a2f31a9`. Commit archive : `fbb17b46f293e9c64fb59ebfa7ae5d4cb92bc923`.
- ZIP : 954372 octets ; SHA256 `A126802D7A3467E1CC26B5DBCB0503ACDB9B4EFBCD15705520DA5F05E7D050D3`. Quatre fichiers racine, CRC valide, ASI/dxgi AMD64, README/INI compares aux sources et lien public HTTP 200 verifie.

Installation manuelle jeu ferme en gardant INI, raccourcis, PAK et sauvegardes. Aucun jeu lance, aucune installation ni ecriture dans la memoire du jeu. Retour arriere : V1.95.

Tests groupes : War Native pour Stoneskin/cristal apres redemarrage ; Clone et rayon sur War ; Rampage bref/maintenu sur Strife ; effets/degats, cout en mission, fin et reprise du controle ; puis Caltrops/Geyser et retour Native. [Limites et contrat](V1_96_FOREIGN_POWER_BATCH_TEST.md).

L'origine du cristal n'est pas etablie. Les composants nouvellement enregistres et les transferts ne sont pas valides en jeu. Les poses compatibles ne remplacent pas un retargeting d'animation. Une premiere execution CI a rejete une fixture Geyser incomplete ; corrections du nom None et de la classe generee uniquement dans la fixture. Aucune archive de cette execution publiee.
