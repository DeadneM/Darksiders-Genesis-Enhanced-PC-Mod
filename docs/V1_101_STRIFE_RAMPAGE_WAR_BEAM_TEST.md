# V1.101 - lot Rampage Strife et rayon War

**CONSTRUITE / CONTROLEE : [58/58 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40191183951), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/417e6047dd3857850e6f55dd82cbdb83c0470a20/downloads/DarksidersGenesis_V1.101_STRIFE_RAMPAGE_WAR_BEAM_TEST.zip) - [Recu](V1_101_DELIVERY_RECEIPT.md). Corrections War et Rampage Strife EXPERIMENTALES, NON VALIDEES EN JEU ; base V1.98 validee.**

EXPERIMENTAL, NON VALIDE EN JEU. Base validee V1.98, source cumulative V1.100. [Contrat de gameplay, preuves et essais](V1_100_STRIFE_RAMPAGE_WAR_BEAM_TEST.md).

V1.100 a ete construite et controlee : [58/58 suites Windows](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40177287968), zero erreur, un avertissement MSVC C6386 sur g_cast[c.index]. Le contrat Owner bornait deja l'index, mais cette garantie n'etait pas locale a l'ecriture du tableau. Cette archive reste intacte et intermediaire ; aucun succes en jeu annonce.

V1.101 introduit PlayerCast(index), qui rend uniquement l'adresse explicite P1 ou P2, sinon nullptr. Tous les anciens acces variables au tableau sont retires ; les usages verifient le pointeur. Les essais ajoutent les index -1 et 2 avec refus sans lecture/ecriture d'etat, ni lancement ou fin de pouvoir. Aucun pragma, suppression d'avertissement ou assouplissement de l'analyse. Les gestes et mecanismes de gameplay V1.100 restent identiques.

Objectif du lot : World Ender War tire devant et garde sa pose sans repetition dans les deux vues ; Rampage Strife se deplace, respecte l'obstacle, termine et rend le controle deux fois. Le mouvement, les degats et les sorties du nouveau transfert ne sont pas prouves par CI. Le corps garde sa locomotion Strife ; aucune animation Rampage dediee n'est annoncee. Les limites et tests du contrat V1.100 s'appliquent integralement. P2 reporte.

165 anciennes archives, main/archive/v1.43 et tous les acquis preserves. 168 cles INI, 55 ID Hotkeys et defauts inchanges. Installation manuelle jeu ferme, INI/PAK/sauvegardes gardes. Repli valide V1.98 ; V1.99 et V1.100 restent experimentales. Prochaine etape : retour groupe avant promotion.
