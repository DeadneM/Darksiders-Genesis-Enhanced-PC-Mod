## V1.103 - reception lourde par interruption native du mouvement

**CONSTRUITE / CONTROLEE : [57/57 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40199153318), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/74cb774b480f1cfb5cd2ed6e8d8a15ccd0fd39ec/downloads/DarksidersGenesis_V1.103_HEAVY_LANDING_NATIVE_INTERRUPT_TEST.zip) - [Recu](V1_103_DELIVERY_RECEIPT.md). Nouvelle reception lourde EXPERIMENTALE, NON VALIDEE EN JEU ; base V1.102 validee pour Stoneskin et police.**

**EXPERIMENTALE, NON VALIDEE EN JEU ; compilation et livraison controlees ; essai en jeu requis.** Base V1.102 : Stoneskin et taille de police explicitement VALIDES par l'utilisateur ; Geyser/Clone/Caltrops V1.98 et World Ender War V1.100 conserves. Rampage Strife V1.100/V1.101 reste rejete et retire.

La reception lourde apres une chute utilise maintenant sa vraie fenetre native d'interruption MOVE, ouverte apres l'impact (0,15 s et sol continu 50 ms). Le moteur decide de la fin du pouvoir, du blend de l'animation et de la reprise lorsque le joueur se deplace. L'ancien correctif qui ecrivait AllowPlayerMovement et le bit d'action MOVE est retire. Pas de fin forcee, simulation de touche, changement des dashs ou ecriture de vitesse. La case existante reste OFF par defaut ; les raccourcis reaffectes suivent l'action native.

169 cles INI, 55 ID Hotkeys et anciens reglages inchanges ; 167 anciennes archives preservees ; debug uniquement. Installation manuelle jeu ferme en conservant son INI/raccourcis/PAK/sauvegardes. [Contrat, preuves, limites et essais](V1_103_HEAVY_LANDING_NATIVE_INTERRUPT_TEST.md). Le transfert Rampage demande encore un releve du refus exact ; aucun candidat aveugle reactive. P2 reste a tester plus tard.

Essai groupe : Strife puis War, meme chute en vue native et TPS, case OFF puis ON ; maintenir le deplacement a la reception et comparer la reprise, refaire sans bouger (animation/impact natifs), puis verifier dash et attaques. Remettre OFF si le rendu ou les commandes regressent.

---

## Retour utilisateur V1.102 - Stoneskin et police valides

25 octobre 2026 : « stoneskin valide / taille police ok ». **Stoneskin et taille de police VALIDES par l'utilisateur dans ses essais.** Conserver ces chemins et leurs limites documentees ; aucune nouvelle mesure de degats, variante ou validation P2 inventee. World Ender War V1.100 et Geyser/Clone/Caltrops V1.98 restent retenus. Base de reprise : V1.102 avec ces acquis. Rampage Strife V1.100/V1.101 reste rejete et retire ; reception lourde toujours ouverte. Le refus d'icone indisponible n'a pas de validation visuelle distincte rapportee. Pas de nouveau build pour cette consignation.

---

## V1.102 - Stoneskin pour les deux personnages et police du menu

**CONSTRUITE / CONTROLEE : [57/57 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40194346246), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/00581c2daeb87a192b99d2bfbac75ea1b36557c8/downloads/DarksidersGenesis_V1.102_STONESKIN_MENU_FONT_TEST.zip) - [Recu](V1_102_DELIVERY_RECEIPT.md). Stoneskin Strife et police EXPERIMENTAUX, NON VALIDES EN JEU ; acquis V1.98 et World Ender War V1.100 valides conserves.**

**EXPERIMENTALE ; compilation et livraison controlees ; test utilisateur requis.** Base retenue : acquis V1.98 et World Ender sur War V1.100, explicitement confirme par l'utilisateur. Geyser/Clone/Caltrops toujours valides. Rampage sur Strife V1.100/V1.101 REJETE : icone de capacite disparue et comportement anormal. Ce chemin, ses montages, sa reflexion et ses tests specifiques sont retires de l'ASI ; une ancienne selection Strife=5 revient a Native. Rampage propre de War conserve.

Stoneskin est selectionnable dans chacun des trois emplacements des deux personnages : chemin natif sur War ; sur Strife, statut/cout/duree natifs et geste compatible avec son propre squelette. Transfert Strife EXPERIMENTAL et partiel : materiau de pierre War, onde de choc du montage et effets d'arme non copies. Stinger reste natif, sans reattache ou correction visuelle.

About → Menu appearance : taille de police du menu de 10 a 32 px, saisie exacte et Default 13 px, enregistree dans [Overlay] FontSize. Curseur et fenetres du mod gardent leur fonctionnement ; autres overlays et jeu hors du scope de cette taille. INI personnel conserve, cle absente = taille d'origine.

Refus d'un transfert indisponible : le resolveur garde la classe/icone et le debloquage natifs du slot, puis bloque seulement son utilisation ; il ne renvoie plus une classe vide. Une capacite encore verrouillee reste verrouillee. Ce changement traite un defaut confirme du code, sans attribuer une cause runtime non capturee a Rampage.

169 cles INI (une ajout), 55 ID Hotkeys et anciens reglages preserves ; 166 archives precedentes intactes, debug seulement. Installation manuelle jeu ferme, garder INI/raccourcis/PAK/sauvegardes. P2 reporte. [Contrat, preuves, essais et repli](V1_102_STONESKIN_MENU_FONT_TEST.md).

Test groupe : police et reouverture du menu ; Stoneskin Strife/War (cout, protection et expiration, fin du cast/retour Native) ; controle de l'icone apres ancienne selection Rampage et bref controle des quatre transferts valides. Aucun nouveau resultat en jeu annonce.

---

## V1.101 - lot Rampage Strife / rayon War, controle des joueurs explicite

**CONSTRUITE / CONTROLEE : [58/58 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40191183951), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/417e6047dd3857850e6f55dd82cbdb83c0470a20/downloads/DarksidersGenesis_V1.101_STRIFE_RAMPAGE_WAR_BEAM_TEST.zip) - [Recu](V1_101_DELIVERY_RECEIPT.md). Corrections War et Rampage Strife EXPERIMENTALES, NON VALIDEES EN JEU ; base V1.98 validee.**

**EXPERIMENTALE, NON VALIDEE EN JEU ; compilation et livraison controlees ; validation en jeu requise.** Base validee V1.98. Cumul des candidats V1.99/V1.100 : direction et pose de World Ender War, mouvement natif et fins de Rampage Strife. V1.100 a passe 58/58 suites mais l'analyse MSVC signale C6386 sur l'index du tableau d'etats ; son ZIP reste une archive intermediaire.

V1.101 remplace tous les acces variables a ce tableau par un resolveur borne explicite P1/P2, avec refus des indices invalides et tests -1/2. Aucune suppression d'avertissement. Les deux candidats de gameplay restent identiques : rayon dans le repere propre de War, geste unique puis pose maintenue ; Rampage en mouvement natif sur une piste non evaluee par le corps de Strife. Pas d'animation Rampage dediee annoncee.

Geyser/Clone/Caltrops valides, Stinger natif, cameras, tirs, manettes, roues et autres acquis conserves. 168 cles INI et 55 ID Hotkeys inchanges ; 165 archives precedentes a conserver, debug uniquement. Installation manuelle jeu ferme en gardant INI/raccourcis/PAK/sauvegardes. P2 differe. [Contrat, essais et repli](V1_101_STRIFE_RAMPAGE_WAR_BEAM_TEST.md).

Essai groupe : War World Ender dans les deux vues (direction, pose, fin et relance) ; Strife Rampage en espace libre puis face a un obstacle (mouvement, degats, fin/reprise et relance) ; retour Native et controle rapide des trois transferts deja valides. Ces comportements restent a approuver en jeu.

---

## V1.100 - Rampage Strife et correction du rayon War

**EXPERIMENTALE, NON VALIDEE EN JEU. Compilation Windows et livraison a verifier.** Base validee V1.98 ; cumul V1.99 avec retour utilisateur partiel : World Ender fonctionne sur War, mais tire a 90 degres a droite dans les deux vues et repete son geste. Ces deux defauts restent ouverts jusqu'au prochain essai.

Ce lot remplace la source artistique VFX_Aura par le repere de tir du composant prive, aligne sur la capsule de War. Son geste se joue une fois, maintient sa pose pendant le rayon, puis revient a son idle ; duree, callbacks, cout et annulation natifs conserves. Aucun changement de camera ni correctif general des tirs.

Rampage sur Strife revient par un chemin distinct de V1.96 : mouvements portes par les animations, boucle, collisions et phases de fin natifs dans un montage prive. La piste utilise un emplacement absent du graphe de Strife, verifie a chaque lancement : son corps garde ses propres poses de locomotion, sans recevoir les os de War. **Pas d'animation Rampage dediee a Strife annoncee ; mouvement, degats et sortie restent a tester.** Contrats refuses = abandon natif du pouvoir et transfert indisponible pour la session, sans timer de secours.

Geyser/Clone/Caltrops valides, Stoneskin/Stinger natifs, controles, tirs et cameras conserves. INI de 168 cles et 55 ID Hotkeys inchanges ; 164 anciennes archives a conserver, debug seulement. Installation manuelle jeu ferme, garder son INI, ses raccourcis, PAK et sauvegardes. P2 differe. [Preuves, essais et rollback](V1_100_STRIFE_RAMPAGE_WAR_BEAM_TEST.md).

Test groupe : World Ender sur War dans les deux vues, orientation, pose maintenue et sortie ; Rampage sur Strife en espace libre puis face a un obstacle, fin et relance ; retour Native et comparaison des pouvoirs propres. Aucun resultat CI ne remplace ces essais en jeu.

---

## V1.99 - World Ender sur War, composant natif propre

**CONSTRUITE / CONTROLEE : [56/56 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40174981083), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/030d79dc867acc19431c3b710b5b9eb62ce60818/downloads/DarksidersGenesis_V1.99_WAR_WORLD_ENDER_NATIVE_COMPONENT_TEST.zip) - [Recu](V1_99_DELIVERY_RECEIPT.md). World Ender War EXPERIMENTAL, NON VALIDE EN JEU ; base V1.98 validee.**

**Base V1.98 VALIDEE par l'utilisateur le 25 octobre 2026** : gestes Geyser sur Strife, Clone/Caltrops sur War approuves ; Stinger natif deja accepte et cloture. Ce nouveau candidat conserve ces acquis. **V1.99 EXPERIMENTALE, NON TESTEE EN JEU ; compilation et livraison controlees, voir le recu ci-dessus.**

L'ancien transfert World Ender cherchait une propriete `Chaoseater` absente de War. Le nouveau chemin utilise son vrai modele `CharacterMesh0`, le point natif `VFX_Aura` et un composant de rayon prive lui appartenant, enregistre dans son monde et attache a sa capsule par le moteur. Montage prive avec geste War, deux callbacks StartBeam/DestroyBeam uniques aux temps natifs et etats d'annulation conserves. Pas de nouveau hook, timer, touche imposee ni modification d'un asset partage. [Contrat, preuves, essais et rollback](V1_99_WAR_WORLD_ENDER_NATIVE_COMPONENT_TEST.md).

Rampage sur Strife reste indisponible : sa boucle utilise le mouvement porte par les animations de War, absent des clips Strife et de la pose idle rejetee. Aucune reactivation de cet ancien adaptateur. Pouvoirs propres natifs, tirs/cameras/controleurs/roues et acquis conserves. INI de 168 cles, 55 ID Hotkeys et valeurs par defaut inchanges ; 163 anciennes archives intactes, debug uniquement. Installation manuelle en conservant son INI, ses raccourcis, son PAK et ses sauvegardes. P2 reporte.

Test groupe : War a pied en mission, World Ender dans un emplacement, verifier rayon et degats en vue native puis TPS, fin du pouvoir, reprise du mouvement et second lancement ; remettre Native et comparer Strife. Verifier rapidement les trois transferts valides. Le rendu, l'orientation, les sons et les variantes du rayon sur War restent a approuver en jeu.

---

## V1.98 - Gestes propres au receveur et cristal Stinger identifie

### Retour utilisateur du 25 octobre 2026 - Stinger cloture

Sur V1.98, l'utilisateur confirme que le cristal du coeur Stinger est dans le dos, bien attache au personnage, et accepte ce rendu comme le comportement vanilla retrouve. **Apparence Stinger VALIDEE par l'utilisateur ; demande de deplacement/orientation vers l'avant CLOTUREE.** Conserver cet effet natif, sans nouvelle adaptation, reattache ou decalage. Le releve de rotations est arrete. Cette validation concerne uniquement Stinger/Stoneskin, pas les gestes Geyser/Clone/Caltrops V1.98, qui restent experimentaux et a tester. Aucun code ASI, INI, raccourci, PAK ou fichier du jeu modifie ; aucun nouveau build.

**CONSTRUITE / CONTROLEE : [55/55 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40135148178), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9f21be738b703e1cc6f40c5422531e4130b57486/downloads/DarksidersGenesis_V1.98_RECEIVER_CASTING_GESTURES_TEST.zip) - [Recu](V1_98_DELIVERY_RECEIPT.md). Rendu des gestes EXPERIMENTAL, non valide en jeu.**

Lot EXPERIMENTAL, validation en jeu encore requise. Base cumulative V1.97 ; acquis utilisateur : effets/degats Geyser et Caltrops V1.95, effet Clone V1.96. V1.97 reste un repli construit/controle, pas une version declaree validee par l'utilisateur.

Geyser sur Strife utilise un geste de canalisation Strife ; Clone/Caltrops sur War un geste War. Squelettes propres, fichiers d'animation natifs inchanges, sons/pas seuls dans ces gestes. Durees des casts, callbacks et effets retenus conserves ; retour a la pose precedente si le geste charge est indisponible. Aucun nouveau hook, chargement force ou timer. Rampage sur Strife et World Ender sur War restent retires ; pouvoirs propres natifs conserves.

Cristal Stoneskin : capture en lecture seule identifie P_Stinger_CreatureCore_StoneSkin du coeur Stinger, attache a CollisionCylinder, 97 unites au-dessus de la racine du modele. L'utilisateur confirme sa disparition en retirant ce coeur ; les effets Stoneskin ordinaires restent presents. Source visuelle CONFIRMEE ; aucune preuve de deplacement cause par le mod, aucun correctif global d'effets ajoute. [Preuves, objectif, essais et rollback](V1_98_RECEIVER_CASTING_GESTURES_TEST.md).

Installation manuelle jeu ferme, conserver son INI/raccourcis/PAK/sauvegardes. 168 cles et 55 ID Hotkeys inchanges ; 162 archives anterieures a conserver ; debug seulement, P2 differe. Le recu de livraison donne les resultats Windows/ZIP lorsqu'ils sont disponibles. Les entrees suivantes sont historiques.

---

## V1.97 - Repli apres rejet de V1.96

V1.96 REJETEE en jeu : Rampage sur Strife bloque le personnage ; World Ender sur War echoue ; Clone fonctionne sans animation correcte. Caltrops/Geyser : effets et degats confirmes, animations toujours incorrectes. **V1.97 CONSTRUITE / CONTROLEE : [55/55 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40123319154), zero avertissement/erreur MSVC ; repli NON VALIDE EN JEU.** [ZIP cumulatif immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/81b4e41b4d0b9a2d74eee45ba6f63bcbe986c217/downloads/DarksidersGenesis_V1.97_FOREIGN_POWERS_SAFE_SUBSET_TEST.zip) - [Recu](V1_97_DELIVERY_RECEIPT.md). Base : V1.95 et uniquement les effets retenus de Clone V1.96.

Le code des deux transferts rejetes est retire, pas seulement masque dans le menu. Les anciennes selections Rampage sur Strife et World Ender sur War reviennent a Native. Leurs pouvoirs d'origine restent disponibles sur leur propre personnage. Clone/Caltrops sur War et Geyser sur Strife conserves. Aucun changement des tirs, cameras, controles, effets valides ou defauts INI.

Stoneskin reste natif sur War ; aucune adaptation de ce pouvoir. Cristal toujours signale, uniquement pendant Stoneskin : NON CORRIGE. Le PAK d'origine contient un effet de pointes de pierre conditionne par un coeur ; piste seulement, aucun composant observe en jeu et aucune attribution de cause. Animations : les poses idle ne sont pas une solution finale ; il faut reprendre le transfert d'animation et son lifecycle avant de reintroduire les pouvoirs rejetes.

Installation manuelle avec INI/raccourcis/PAK/sauvegardes conserves. 168 cles, 55 ID Hotkeys, 161 anciennes archives ; debug uniquement, P2 reporte. [Etat, preuves et essais](V1_97_RETAINED_FOREIGN_SUBSET_TEST.md). Les entrees suivantes sont historiques.

---

## V1.96 - lot des capacites et retrait de notre Stoneskin

24 octobre 2026. **CONSTRUITE / CONTROLEE : [55/55 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40119582099), zero avertissement/erreur MSVC. Nouveaux transferts EXPERIMENTAUX, non valides en jeu.** [ZIP cumulatif immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/fbb17b46f293e9c64fb59ebfa7ae5d4cb92bc923/downloads/DarksidersGenesis_V1.96_FOREIGN_POWER_BATCH_TEST.zip) - [Recu](V1_96_DELIVERY_RECEIPT.md). Base cumulative V1.95. Caltrops sur War et Geyser sur Strife : effets et degats confirmes par l'utilisateur ; animations incorrectes. Ces chemins sont conserves.

Le meme lot ajoute Clone et World Ender sur War, Rampage et ses quatre suites sur Strife. Composants propres au lanceur et montages prives compatibles. Poses idle : animations, mouvement de Rampage et nouveaux effets/degats non valides en jeu.

Notre adaptation de Stoneskin et son choix explicite sont supprimes. Ancien choix 6 = Native ; Stoneskin original de War conserve via Native. Comparer le cristal apres redemarrage complet, sa cause reste inconnue. Trace Geyser temporaire retiree. Aucun nouveau hook ou timer de gameplay.

Installation manuelle avec INI, raccourcis, PAK et sauvegardes conserves. 168 cles, 55 ID Hotkeys et 160 anciennes archives preserves. Debug exclusivement ; P2 reporte. [Contrat et essais](V1_96_FOREIGN_POWER_BATCH_TEST.md). Les entrees suivantes sont historiques.

---

## V1.95 — Geyser sur Strife, candidat final et identité du ZIP centralisée

24 octobre 2026. **CONSTRUITE / CONTRÔLÉE : [54/54 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40114886605), zéro erreur et zéro avertissement MSVC. Geyser sur Strife EXPÉRIMENTAL, non validé en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/027dae183a714754e66263e4f0f3bc0622498c42/downloads/DarksidersGenesis_V1.95_STRIFE_GEYSER_PRESENTATION_TEST.zip) · [Reçu](V1_95_DELIVERY_RECEIPT.md). Base validée V1.87 dans les cas rapportés ; cumul V1.93 et correction de présentation V1.94. Celle-ci a passé 54/54 suites sans avertissement MSVC, mais son archive publiée porte encore le suffixe Caltrops : elle reste immuable et intermédiaire. V1.95 conserve exactement son code de jeu et fait lire le nom du ZIP depuis BuildInfo, avec vérification de sa version avant compilation.

Geyser sur Strife : son de lancement, lames/nova et variantes à leurs temps natifs ; un seul SpawnBlast. Racine propre de Strife dans des notifications privées, sans effets de son épée ni voix de War. **Pose Strife_Idle encore utilisée, dégâts et rendu non validés en jeu.** [Contrat et test ciblé](V1_95_STRIFE_GEYSER_PRESENTATION_TEST.md). Pas de nouveau hook/minuterie, ni modification des autres capacités ou des acquis.

Installation manuelle, garder son INI/raccourcis/PAK/sauvegardes. 168 clés, 55 ID Hotkeys et 159 anciennes archives intacts ; main/archive/v1.43 préservés, P2 reporté. Les sections suivantes sont historiques.

---

## V1.94 — présentation native de Geyser sur Strife

24 octobre 2026. **V1.94 INTERMÉDIAIRE CONSERVÉE : 54/54 suites et zéro avertissement MSVC, mais suffixe d’archive resté sur Caltrops. Remplacée par V1.95 avec le même code de jeu et un nom de ZIP centralisé. Aucun résultat en jeu supposé.** Base validée V1.87 dans les cas rapportés ; cumul V1.93 et diagnostic réel du 24 octobre. Le dernier essai Strife reste rejeté : pose figée sans effet. Le journal prouve cependant l’appel de SpawnBlast et un objet d’attaque appartenant au bon personnage ; les dégâts ne sont pas mesurés.

Correction ciblée : le montage privé conserve maintenant le son de lancement, les lames/nova et leurs variantes avec leurs temps natifs, ainsi qu’un seul SpawnBlast. Les points Bone_War_Root deviennent la racine de Strife dans des notifications privées ; voix, balayages et effets de son épée ne sont pas transférés. Le jeu conserve allocation, notifications et fin du montage. **La pose reste Strife_Idle : aucune nouvelle animation de lancement annoncée, ni dégâts prétendus réparés.** [Contrat, limites, test et rollback](V1_94_STRIFE_GEYSER_PRESENTATION_TEST.md).

Installation manuelle avec INI/raccourcis/PAK/sauvegardes personnels conservés. Caltrops sur War V1.93 reste expérimental ; Stoneskin, capacités propres, caméras/tirs, XInput/roues et autres acquis inchangés. P2 reporté. 168 clés, 55 ID Hotkeys et 158 anciennes archives préservés ; debug exclusivement. Les sections suivantes sont historiques.

---

## Diagnostic Geyser V1.93 — 24 octobre 2026

Essai comparatif reçu : **Strife reste figé sans effet ; transfert non validé**. Le journal prouve pourtant l’appel réel de SpawnBlast et des événements d’un objet d’attaque appartenant au bon personnage, avec War puis Strife. Le code et une nouvelle extraction vérifiée du PAK montrent que le montage privé omet les sons et effets natifs et utilise une pose de repos. **Défaut de présentation identifié ; dégâts non mesurés, aucun nouveau correctif/build annoncé.** [Résultat, limites et correction ciblée](GEYSER_COMPARISON_2026_10_24.md). Les sections suivantes sont historiques ; V1.93 reste le dernier ZIP.

---

## V1.93 — Caltrops sur War au sol, candidat final hors jeu

24 octobre 2026. **V1.93 CONSTRUITE / CONTRÔLÉE : [54/54 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40058379301), zéro erreur et zéro avertissement MSVC. Caltrops sur War EXPÉRIMENTAL, NON TESTÉ EN JEU.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/85f4d9a893ddfe85e6aef2799e339b2b7d567102/downloads/DarksidersGenesis_V1.93_WAR_CALTROPS_GROUND_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_93_DELIVERY_RECEIPT.md). Base validée V1.87 dans les cas rapportés ; cumul V1.91 et prototype V1.92, dont le code corrigé est `dac4d9d`. Poursuite autorisée sans pouvoir lancer le jeu : **aucune validation fictive**. Geyser reste NON CORRIGÉ et son diagnostic V1.91 inchangé ; P2 reporté.

**Caltrops sur War au sol**, dans chacun des trois emplacements existants : pose War compatible, spawner natif demandé au démarrage depuis son bassin et avec son propre propriétaire. Pas de nouveau hook/minuterie de gameplay, bouton physique, composant partagé ou CDO modifié. Réentrance limitée à l’instance exacte ; arrêt du seul transfert si le contrat de cast échoue. Le lancer aérien et l’animation Strife ne sont pas transférés. Effet, dégâts, coût en mission, fin/annulation et cœurs **non testés en jeu**. [Contrat et essais différés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_93_WAR_CALTROPS_GROUND_TEST.md).

V1.92 a été archivée depuis 6161055 pendant son remplacement ; ses 54 suites passaient, mais le ZIP n’inclut pas la correction dac4d9d. Il reste immuable et n’est pas le candidat final. V1.93 ajoute aussi une garde CI : refuser la publication si les sources compilées ont changé sur debug. 168 clés et défauts, 55 ID Hotkeys et 157 anciennes archives conservés ; main/archive/v1.43 intacts. Installation manuelle, aucun jeu lancé ni fichier installé ; tests groupés plus tard.

Clone/World Ender vers War : composants de réception et nettoyage à couvrir ; Rampage vers Strife : quatre fins Land/Slam à adapter. Ils restent indisponibles et en étude, sans fonctionnalité prétendue. Les sections suivantes sont historiques.

---

## V1.92 — prototype Caltrops vers War au sol

24 octobre 2026. **V1.92 INTERMÉDIAIRE ARCHIVÉE, remplacée par V1.93 : 54 suites passées sur les sources 6161055, mais ce ZIP précède la correction dac4d9d de réentrance par instance. Son run a été annulé trop tard, après publication. Aucun résultat de jeu supposé ; archive conservée sans modification.** Base expressément validée V1.87 pour les cas rapportés, acquis antérieurs conservés ; base cumulative V1.91 `d974c18`, dont le diagnostic Geyser **n’a pas été testé**. L’utilisateur autorise la poursuite des autres sujets sans pouvoir lancer le jeu : cet accord ne constitue pas une validation fictive. Geyser sur Strife reste NON CORRIGÉ ; P2 reste reporté.

Gameplay → Wrath ability slots : **Caltrops sur War**, indépendamment dans ses trois emplacements. Prototype **au sol seulement** : pose War_Idle compatible pendant la durée native du cast, création demandée au démarrage par la fonction native Caltrops depuis **Bone_War_Pelvis**, avec le propriétaire War courant. Aucun nouveau hook, minuterie de gameplay, composant partagé, modification de CDO ou commande physique ajoutée. Coût et déblocage de l’emplacement restent gérés par le résolveur natif. Cette variante n’est pas le lancer animé de Strife ; effet/dégâts/sons, cœurs, fin et annulation restent **à vérifier en jeu**. [Contrat, limites et essais différés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_92_WAR_CALTROPS_GROUND_TEST.md).

Installation manuelle, garder son INI/raccourcis/PAK/sauvegardes. 168 clés et défauts, 55 ID Hotkeys et 156 anciennes archives préservés ; main/archive/v1.43 intacts. Code des acquis, cast Stoneskin, montage et trace Geyser V1.91 conservés. Clone et World Ender vers War exigent encore leurs propres composants de réception ; Rampage vers Strife exige sa chaîne de déplacement/fin. Ces trois transferts restent indisponibles, sans case inerte annoncée comme fonctionnelle. Essais groupés plus tard ; aucune relance ni capture demandée maintenant. Les sections suivantes sont historiques.

---

## V1.91 — diagnostic Geyser avec identité native War correcte

23 octobre 2026. **V1.91 CONSTRUITE / CONTRÔLÉE : [53/53 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40016586657), zéro erreur et zéro avertissement MSVC. DIAGNOSTIC seulement ; Geyser sur Strife NON CORRIGÉ, non validé en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/0be129010adbb857e0d3de6bbcdd32b3d7022c1b/downloads/DarksidersGenesis_V1.91_GEYSER_DISPATCH_DIAGNOSTIC_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_91_DELIVERY_RECEIPT.md). Base expressément validée V1.87 (réticule, bras War et Léviathan, avec acquis antérieurs conservés). Base cumulative V1.90 archivée `cc3932a`. Geyser V1.89 sur Strife **REJETÉ** par l’utilisateur ; retour Native et pouvoirs propres fonctionnels. P2 reporté.

V1.90 a passé 53 suites sans avertissement MSVC, mais le second audit a relevé un type War erroné dans le filtre **et dans son test synthétique**. Archive conservée, intermédiaire non retenue. V1.91 utilise War=2 comme le chemin natif WrathSlotFeature, rejette l’ancien type 0 et impose aussi le thread de jeu pour découvrir la fonction. **Aucun changement du pouvoir Geyser ni promesse de réparation.** La trace est limitée à huit appels et quatre événements imbriqués par appel ; les quatre arguments et l’appel natif restent transmis une seule fois, sans nouvel hook/minuterie. [Contrat et essai](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_91_GEYSER_DISPATCH_DIAGNOSTIC_TEST.md).

Installation manuelle, conserver son INI/raccourcis/PAK/sauvegardes. 168 clés et défauts, 55 ID Hotkeys, 155 anciennes archives et branches main/archive/v1.43 préservés. Test unique : préparer un emplacement Strife sur Geyser, une activation War Native puis Strife, à pied et hors menus ; revenir à Native et conserver le journal. Objectif : confirmer l’entrée dans la fonction et l’existence éventuelle de l’attaque, pas annoncer des dégâts réparés. Pas d’essai P2 demandé. Ensuite : corriger le maillon démontré et retirer la trace temporaire ; les quatre autres transferts restent ouverts. Les sections suivantes sont historiques.

---

## V1.90 — diagnostic ciblé Geyser, échec V1.89 confirmé

23 octobre 2026. **V1.90 INTERMÉDIAIRE NON RETENUE pour le diagnostic comparatif : [53/53 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40015555280), zéro erreur et zéro avertissement MSVC, mais filtre War erroné (type 0 au lieu du type natif 2) découvert au second audit. Remplacement V1.91 préparé ; Geyser sur Strife NON CORRIGÉ.** [Archive immuable conservée](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/cc3932ad985ba779d47f60b3f2865e01d5970239/downloads/DarksidersGenesis_V1.90_GEYSER_DISPATCH_DIAGNOSTIC_TEST.zip) · [Reçu et limite](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_90_DELIVERY_RECEIPT.md). Base expressément validée : V1.87 pour réticule, bras War et Léviathan, avec les acquis antérieurs conservés. Base cumulative V1.89 `6921fb7`. **Transfert Geyser V1.89 REJETÉ en jeu** : Strife reste en pose de repos, aucune attaque visible ; retour Native et pouvoirs propres confirmés fonctionnels par l’utilisateur. **P2 reporté**. Aucun nouveau correctif de capacité prétendu.

La comparaison native War/Strife montre le montage actif et la notification d’attaque présente dans les deux files d’animation. Elle ne prouve pas l’entrée dans SpawnBlast ni la création de l’attaque. V1.90 ajoute uniquement une trace limitée à huit appels sur le passage d’événements déjà utilisé : entrée/sortie de Geyser et événements imbriqués de ses acteurs d’attaque. Appels natifs et quatre arguments transmis intégralement ; aucun nouvel hook, timer, coût, dégâts ou délai modifié. Une absence d’événement imbriqué ne prouve pas une absence d’acteur. [Contrat et essai](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_90_GEYSER_DISPATCH_DIAGNOSTIC_TEST.md).

Installation **manuelle**, conserver son INI/raccourcis/PAK/sauvegardes. 168 clés et leurs défauts, 55 ID Hotkeys, 154 anciennes archives et branches main/archive/v1.43 préservés. Après installation : régler un emplacement Strife sur Geyser, faire une activation avec War sur Native puis une avec Strife, hors menus ; revenir à Native et conserver le journal. Aucun essai coop demandé. Suite : analyser cette exécution réelle avant toute nouvelle modification de Geyser, puis supprimer la trace temporaire ; Rampage vers Strife et les trois pouvoirs Strife vers War restent ouverts. Les sections suivantes sont historiques.

---

## V1.89 — candidat Geyser finalisé

23 octobre 2026. **V1.89 CONSTRUITE / CONTRÔLÉE : [53/53 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40009032079), zéro erreur et zéro avertissement MSVC. Geyser sur Strife EXPÉRIMENTAL, non validé en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/8507ca9ffb9fbc24a62ea6d13146245ae13c335e/downloads/DarksidersGenesis_V1.89_STRIFE_GEYSER_CLEAN_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_89_DELIVERY_RECEIPT.md). Base expressément validée : V1.87 (réticule, bras War et Léviathan dans les essais rapportés), code des acquis conservé. Base cumulative : V1.88 `4309438`, prototype Geyser encore expérimental, 53 tests passés mais avertissement C6386 sur son marqueur joueur ; archive préservée. Accès au marqueur désormais centralisé, uniquement P1/P2, sans index dynamique ni avertissement masqué. Aucun changement de calcul du pouvoir. [Objectif, limites et tests](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_89_STRIFE_GEYSER_CLEAN_TEST.md).

Gameplay → Wrath ability slots : **Blade Geyser sur Strife**, trois emplacements indépendants, attaque déclenchée par sa notification native. Pose de repos compatible ; **enchaînement rising slash de War non transféré**. Coût/déblocage/variantes et fin natifs, aucun nouvel hook ni minuterie. Tester apparition, dégâts, coût, fin, retour Native puis Geyser propre de War ; P2 séparément. Protection Stoneskin conservée, apparence native à identifier d’après la précision utilisateur. Rampage → Strife et les trois pouvoirs Strife → War restent ouverts. Installation manuelle, INI/PAK/sauvegardes personnels conservés ; 168 clés, 55 ID Hotkeys et 153 archives antérieures intacts. Autres FX P2 reportés, réception lourde incertaine, HUD général non reproduit.

---

## V1.88 — Geyser natif sur Strife

23 octobre 2026. **V1.88 intermédiaire CONSTRUITE / CONTRÔLÉE : [53/53 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40008159794), zéro erreur, un avertissement d’analyse MSVC C6386 restant sur le nettoyage du marqueur compagnon. Archive conservée, remplacée à la livraison par V1.89 après correction de cet accès. Geyser sur Strife EXPÉRIMENTAL, non validé en jeu.** [Archive immuable vérifiée](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/4309438f8a87d95ef33ab14fe0a4bdc0f4dda245/downloads/DarksidersGenesis_V1.88_STRIFE_GEYSER_TRANSFER_TEST.zip). Base cumulative V1.87 `c20319f`. **Correctif du réticule V1.87 maintenant VALIDÉ par l’utilisateur dans son essai**, comme le bras War et Léviathan ; code de ces acquis inchangé. Branche **debug** seulement ; installation manuelle, conserver son INI/raccourcis/PAK/sauvegardes. Aucun résultat coop supplémentaire supposé.

Gameplay → Wrath ability slots : ajout expérimental **Blade Geyser sur Strife**, indépendamment dans chacun des trois emplacements. Déclenchement `SpawnBlast` natif à environ 0,488 s, progression/coût/variantes et fin gérés par le jeu ; copie temporaire du montage natif avec une pose de repos compatible. Pas de minuterie ni nouvel hook. **L’enchaînement de mêlée rising slash propre à War n’est pas transféré.** Emplacement inactif si ses assets ne sont pas prêts ; échec du contrat de montage : arrêt natif du cast puis blocage de ce seul transfert jusqu’au redémarrage. [Objectif, limites, preuves, essais et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_88_STRIFE_GEYSER_TRANSFER_TEST.md).

Protection Stoneskin V1.83 conservée. Précision utilisateur : il n’est plus certain qu’un cristal était présent avant ; **aspect natif à identifier**, sans régression visuelle affirmée ni correctif visuel inventé. Rampage vers Strife et Clone/World Ender/Caltrops vers War restent ouverts. 168 clés INI et leurs défauts, 55 ID Hotkeys, commandes souris/manette, caméras, tirs, roues, cheval, effets validés et 152 archives antérieures préservés. Réception lourde incertaine, autres FX P2 reportés, HUD général non reproduit. Les sections suivantes sont historiques.

---

## V1.87 — correction du crash de curseur V1.86

23 octobre 2026. **V1.87 CONSTRUITE / CONTRÔLÉE : [52/52 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39998309541), zéro erreur et zéro avertissement MSVC. Défaut de retour curseur corrigé et contrôlé ; reproduction en jeu encore à confirmer.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/64b612f3341a1abebf3dc2961de1580a73904089/downloads/DarksidersGenesis_V1.87_CURSOR_ENUM_ABI_FIX_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_87_DELIVERY_RECEIPT.md). Base cumulative V1.86 `7763e99`. **Interception du réticule V1.86 REJETÉE pour crash**, remplacée par le contrat de retour entier 32 bits natif. Retour utilisateur du 23 octobre : **bras de War et correction Léviathan VALIDÉS dans les essais rapportés**, code conservé. Le correctif du réticule V1.87 reste à valider en jeu ; aucune validation de la version entière ni de la coop déduite. Acquis validés V1.85 (effets des lames), V1.82 (suivi d'exécution) et V1.81 (bras OFF) conservés, branche debug seulement, installation manuelle et INI/raccourcis personnels gardés.

Le minidump montre un index de curseur `0xEDFB8D08` au lieu de `8`. Cet index, sa conversion signée et le calcul du tableau correspondent exactement à l'adresse de lecture invalide. Le champ natif est un octet, **la fonction renvoie un enum entier 32 bits dans EAX** ; V1.86 déclarait à tort le hook et son trampoline avec un retour octet. Les bits restants étaient indéfinis, y compris en renvoyant le résultat natif dans un menu. Contrat corrigé sans nouvel hook ni modification d'entrée. [Preuves, limites, essais et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_87_CURSOR_ENUM_ABI_FIX_TEST.md).

Les anciens tests consommaient eux aussi un octet et ne couvraient pas ce contrat. Désormais, toutes les politiques de curseur passent par un consommateur 32 bits compilé dans un fichier distinct, avec contrôle de type, enums natifs et absence de troncature. INI identique (168 clés), 55 ID Hotkeys et 151 anciennes archives conservés. Tester les quatre modes, puis les pages/roues et leurs transitions ; le reste du lot V1.86 est inclus. Cristal Stoneskin et cinq autres transferts restent ouverts. Les sections suivantes sont historiques.

---

## V1.86 — bras de War, réticule et régression Léviathan

23 octobre 2026. **V1.86 CONSTRUITE / CONTRÔLÉE : [52/52 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39995845270), zéro erreur et zéro avertissement MSVC. Nouveaux choix visuels et retour Léviathan EXPÉRIMENTAUX, à confirmer en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9c04d2514c82a6dad6961ea05686975cb3dde6d9/downloads/DarksidersGenesis_V1.86_WAR_ARM_RETICLE_LEVIATHAN_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_86_DELIVERY_RECEIPT.md). Base cumulative V1.85 `46aff86`. Les effets des épées masquées V1.85 sont maintenant **VALIDÉS par l'utilisateur dans son essai** ; leur code reste inchangé, comme les bras OFF et le suivi TPS des exécutions déjà validés. Branche **debug** seulement, installation manuelle, conserver son INI et ses raccourcis.

Gameplay → Appearance : **War arm appearance — Native / Basic / Augmented**, aspect seulement sans donner le Gantelet ; les armures qui remplacent les éléments enfants gardent leur politique native. **Mouse reticle — Vanilla / Hide in TPS / Hide in normal view / Hide in both views** : seul le réticule de jeu est masqué, les pages, roues, pause et leurs transitions conservent le curseur natif. Deux clés supplémentaires, Native/Vanilla par défaut ; les 166 anciennes valeurs et 55 ID Hotkeys sont conservés.

Léviathan : le journal et les octets du jeu démontrent que la vérification de signature s'effectuait après l'interception de SpawnEmitterAttached par les effets de lame V1.84/V1.85. Initialisation déplacée avant cette interception, garde conservée et effets de mêlée inchangés. Retour des éclairs à confirmer en jeu avec les lames masquées. [Objectif, preuves, limites, essais et rollback V1.86](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_86_WAR_ARM_RETICLE_LEVIATHAN_TEST.md).

Stoneskin : protection sur Strife jugée plausible par l'utilisateur, sans mesure complète ; cristal incorrect sur **Strife et War**, dans les deux vues et même avec les emplacements War Native. **Défaut ouvert**, aucune correction visuelle prétendue. Le transfert partiel V1.83 est inchangé. Geyser/Rampage vers Strife et Clone/World Ender/Caltrops vers War restent à adapter ; les cinq transferts ne sont pas livrés. Autres FX P2 reportés, réception lourde incertaine, HUD général non reproduit. Les sections suivantes sont historiques.

---

## V1.85 — lot cumulatif effets des épées et prototype Stoneskin

22 octobre 2026. **V1.85 CONSTRUITE / CONTRÔLÉE : [51/51 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39845333890), zéro erreur et zéro avertissement MSVC. Nouveaux effets et protection Stoneskin EXPÉRIMENTAUX, non validés en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/6f05855e2c3ba54b904d78116bc9562895ff997a/downloads/DarksidersGenesis_V1.85_COMPACT_BARE_BLADE_FX_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_85_DELIVERY_RECEIPT.md). Installation manuelle, garder son INI et ses raccourcis. Base cumulative V1.84 `6e27cf7`. Acquis V1.81 et suivi TPS des exécutions V1.82 validés dans les essais rapportés, conservés sans modification. Branche debug seulement ; installation manuelle, conserver son INI et ses raccourcis.

Correction des effets de mêlée attachés de V1.84 conservée : arcs compacts depuis les mains des côtés masqués, sans changer dégâts/portée. V1.84 avait passé 51 tests avec un avertissement C4324 de padding ; la structure privée est réordonnée, 128 octets contrôlés à la compilation, sans masquer l'avertissement ni changer les calculs. [Contrats, limites, essais et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_85_COMPACT_BARE_BLADE_FX_TEST.md).

Prototype V1.83 inclus : protection Stoneskin vers Strife, encore partielle et non validée en jeu. Onde de choc de lancement, sons et apparence de pierre non transférés ; mélange complet des six capacités toujours ouvert. 166 réglages INI, 55 ID Hotkeys et 149 anciennes archives conservés. Tester les effets des lames masquées puis, séparément, la protection et le retour Native. Les sections suivantes sont historiques.

---

## V1.84 — correction du chemin des effets de mêlée attachés

22 octobre 2026. **V1.84 intermédiaire CONSTRUITE / CONTRÔLÉE : [51/51 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39844030589), zéro erreur, un avertissement MSVC C4324 de padding de Plan. Nouvelle compilation V1.85 prévue après correction de disposition, sans masquer l'avertissement. Effets EXPÉRIMENTAUX, non validés en jeu.** [Archive conservée](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/c58304fd20466e173e0960ca09e47b399455aa7a/downloads/DarksidersGenesis_V1.84_ATTACHED_BARE_BLADE_FX_TEST.zip) · [Reçu](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_84_DELIVERY_RECEIPT.md). Installation manuelle. Base cumulative V1.83 `81d61dd`. Acquis validés V1.81 conservés ; le suivi TPS des exécutions V1.82 est maintenant **VALIDÉ par l'utilisateur dans son essai**, code inchangé. Le correctif V1.82 des effets d'épées cachées est inefficace dans son essai et remplacé pour les chemins manquants. Branche debug seulement.

Cause démontrée par lecture seule : les huit notifications ciblées des premiers coups/attaques aériennes utilisent SpawnEmitterAttached sur Bone_Strife_Root ; V1.82 les excluait. Elles passent maintenant par le même contexte de propriétaire et de côté que les effets détachés. Position de la main exprimée dans le repère racine natif, taille ×0,4 ; parent, socket, rotation, destruction et pooling conservés. Variante du troisième coup `_03_b` ajoutée d'après ses notifications chargées exactes. Dégâts, portée, poses, tirs et caméra inchangés. Les arcs communs avec une seule lame visible restent natifs. [Contrats, limites, essais et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_84_ATTACHED_BARE_BLADE_FX_TEST.md).

Le prototype V1.83 de **protection Stoneskin vers Strife** est inclus sans modification ; il reste partiel et non validé en jeu : onde de choc, sons et apparence de pierre de War non transférés. Le mélange complet des six capacités reste ouvert. Installation manuelle, conserver son INI et ses raccourcis ; 148 anciennes archives préservées.

---

## V1.83 — premier transfert de pouvoir : Stoneskin sur Strife

22 octobre 2026. **V1.83 CONSTRUITE / CONTRÔLÉE : [51/51 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39841420585), zéro erreur et zéro avertissement MSVC. Protection Stoneskin sur Strife EXPÉRIMENTALE, partielle et non validée en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/aa9a096a232dca76de9456387a7565b69de8925d/downloads/DarksidersGenesis_V1.83_STRIFE_STONESKIN_TRANSFER_TEST.zip) · [Reçu et limites](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_83_DELIVERY_RECEIPT.md). Installation manuelle, conserver son INI. Base cumulative V1.82 `6c916ff`, actuellement en essai utilisateur ; dernière base expressément validée V1.81 dans les cas rapportés. Branche **debug** seulement, installation manuelle.

Chacun des trois emplacements de Strife peut recevoir Stoneskin. L'emplacement d'origine doit être débloqué ; coût, statut de protection, variantes de cœurs et fin restent natifs. Un montage temporaire compatible utilise le repos de Strife pour la durée du cast de War. Aucun déblocage de progression War ni modification des assets/CDO, entrées ou ressources de l'autre joueur. Le reste du mélange des six capacités reste ouvert. [Objectif, contrats, limites, essais et rollback V1.83](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_83_STRIFE_STONESKIN_TRANSFER_TEST.md).

INI et raccourcis inchangés, Native par défaut dans les six choix. Les pouvoirs propres de chaque personnage et tout le lot V1.82 sont conservés. Aucun résultat du nouvel adaptateur en jeu supposé ; les nouvelles fonctions de V1.82 attendent toujours le retour utilisateur. Réception lourde encore incertaine, autres FX P2 reportés, HUD général non reproduit.

**Limite du prototype : seule la protection native et son coût sont transférés.** L'animation compatible de repos ne porte pas les notifications de War : onde de choc de lancement, sons, particules et apparence de pierre ne sont pas encore transférés. Les variantes de cœurs et la protection réelle nécessitent un essai ; ne pas attendre une aura visible du statut de base. Le mélange complet des six capacités reste ouvert.

Retour utilisateur V1.82 du 22 octobre : **suivi TPS des exécutions VALIDÉ dans l'essai rapporté ; effets des épées cachées inefficaces**, dossier rouvert. Lecture seule : huit notifications ciblées des deux premiers coups et des attaques aériennes sont attachées à Bone_Strife_Root, chemin exclu par V1.82 ; les six notifications du troisième coup sont détachées. Le contrat des mains et des assets est confirmé, le passage réel des callbacks reste à vérifier.

---

## V1.82 — effets des lames masquées et suivi vertical des exécutions

22 octobre 2026. Base cumulative V1.81 `adb5ae8`, **VALIDÉE par l'utilisateur pour les trois essais rapportés** (bras OFF animés, mêlée/pouvoirs, transitions). Branche **debug** seulement ; aucun résultat coop supplémentaire supposé. Le code des bras OFF, modèles/holsters, cheval/sons et visée reste conservé.

**V1.82 CONSTRUITE / CONTRÔLÉE : [50/50 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39834733781), zéro erreur et zéro avertissement MSVC. Nouveaux effets et suivi vertical EXPÉRIMENTAUX, non validés en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/03a45347f8484ca4af9fa17cf5b49d3e66062cf9/downloads/DarksidersGenesis_V1.82_BARE_BLADE_EXECUTION_CAMERA_TEST.zip) · [Reçu, empreintes et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_82_DELIVERY_RECEIPT.md). Installation manuelle, conserver son INI et ses raccourcis. Les effets de lame identifiables du côté masqué partent de sa main avec une taille de 40 %, sans changer dégâts ou portée. Les effets communs restent natifs avec une seule épée visible. Nouveau suivi TPS de la montée animée du bassin pendant une exécution native, indépendant par joueur et sans modifier la pose servant à la visée. [Objectif, preuves, limites, essais et rollback V1.82](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_82_BARE_BLADE_EXECUTION_CAMERA_TEST.md).

La capture n'a relevé que des exécutions au sol, confirmé par l'utilisateur : elles servent de référence de cadrage et **ne valident pas le suivi en hauteur**. Les six capacités étrangères restent en étude : contrat du montage dynamique pour Stoneskin précisé, aucun adaptateur incomplet livré. Réception lourde encore incertaine ; autres FX P2 différés, HUD général non reproduit. Installation manuelle, conserver son INI personnel et ses raccourcis.

---

## V1.81 — candidat d'animation désarmée pour les bras OFF

22 octobre 2026, base cumulative V1.80 `3533b3d`, branche **debug** seulement. Le cheval de V1.80 est **VALIDÉ par l'utilisateur dans son essai** et reste inchangé. Le bras OFF figé est rejeté : la demande est une vraie animation désarmée, tout en conservant explicitement la mêlée et les pouvoirs.

**V1.81 CONSTRUITE / CONTRÔLÉE : [48/48 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39803846206), zéro erreur et zéro avertissement MSVC. Bras animés EXPÉRIMENTAUX, non validés en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/36d31209abfbf4c0fea6b5afd64184855f1f964b/downloads/DarksidersGenesis_V1.81_NATIVE_UNARMED_ANIMATION_TEST.zip) · [Reçu et empreintes](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_81_DELIVERY_RECEIPT.md). Installation manuelle, garder son INI et ses raccourcis. Repos/marche/course natifs échantillonnés dans une pose privée, appliquée au seul bras dont le pistolet est OFF. Ancienne capture immobile supprimée. Aucun nouveau réglage ; INI et raccourcis conservés. [Objectif, preuves, limites, essais et rollback V1.81](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_81_NATIVE_UNARMED_ANIMATION_TEST.md). Installation manuelle. Six capacités étrangères, réception lourde et suivi vertical des exécutions restent ouverts ; autres FX P2 différés, HUD général en surveillance.

---

## V1.80 — saut validé, sons montés et candidat bras OFF

22 octobre 2026, branche **debug**, base cumulative V1.79 `5e2221b`. L'utilisateur confirme le saut à cheval et les points 1 à 5 de ses essais, **avec une exception explicite : les bras sans pistolet restent incorrects des deux côtés**. Les choix d'apparence, holsters, épées et la correction native de World Ender sont retenus dans les essais rapportés ; pas de validation coop supposée.

**V1.80 CONSTRUITE / CONTRÔLÉE : [48/48 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39793144590), zéro avertissement MSVC. Sons et nouveau rendu des bras EXPÉRIMENTAUX, à confirmer en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/8124f3e683d16dafbabf6bc610c7f2998a2dce4f/downloads/DarksidersGenesis_V1.80_MOUNTED_SOUNDS_PACKET_ARMS_TEST.zip) · [Reçu, empreintes et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_80_DELIVERY_RECEIPT.md). Installation manuelle, conserver son INI et ses raccourcis.

Lot construit **EXPÉRIMENTAL** : case Jump on horseback déplacée dans **Gameplay → Horse**, son de départ/atterrissage via les SoundCue du jeu, choix Native jump (varied) / Horse snort (varied) / Off ; nouvelle correction de rendu des bras utilisant le même paquet, avec conservation du repos compatible après cheval/ultime. Le saut validé, les fonctions de visée et les raccourcis sont conservés. [Objectif, preuves, limites et essais V1.80](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_80_MOUNTED_SOUNDS_PACKET_ARMS_TEST.md).

Les six capacités entre personnages restent ouvertes : le sélecteur actuel conserve les trois pouvoirs de chaque famille. Aucun pouvoir étranger lancé sur un squelette ou un composant incompatible. [Contrats restant à adapter](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_80_FOREIGN_WRATH_CONTRACTS.md). Installation manuelle, garder l'INI personnel. Les sections suivantes restent historiques.

---

## V1.79 — candidat du lot apparence / World Ender / saut monté

**V1.79 CONSTRUITE / CONTRÔLÉE : [48/48 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39683569838), zéro avertissement MSVC. Nouveaux comportements EXPÉRIMENTAUX, non validés en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/7f579ed3f068ff1cba6f25a7a6ba680ba5759517/downloads/DarksidersGenesis_V1.79_NATIVE_WORLDENDER_BLADES_MOUNTED_JUMP_TEST.zip) · [Reçu, empreintes et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_79_DELIVERY_RECEIPT.md). Les 143 anciennes archives et les 163 anciens réglages sont conservés. Installation manuelle, garder son INI.

Base debug `5d4883e` / V1.78. Retours : maintien OFF et interface jugés bons ; bras encore incorrects, côtés des holsters inversés ; coop différée. Nouveautés expérimentales et critères : [V1.79](V1_79_NATIVE_WORLDENDER_BLADES_MOUNTED_JUMP_TEST.md).

Deux branches OFF clavicule comprise sur Spine2 ; holsters rendus inversés ; Native/masquage gauche/droite/deux pour les épées exactes de Strife ; exemption de World Ender dans le hook des faisceaux ordinaires, par provenance du projectile, sans changement de caméra ni matériau ; saut monté OFF par défaut par action JUMP et DoJump natifs. Capture VM_READ de 2 378 échantillons sans erreur ; le rayon existe et son PSC est visible/actif. L'utilisateur confirme qu'il est visible caméra reculée et que sa direction indépendante de l'animation est incorrecte. Rendu proche encore à vérifier, ne pas annoncer réparé. Contrôles CI et ZIP dans le reçu après livraison. Aucune installation ni relance par l'agent.

---

## V1.78 — lot OFF / interface / nettoyage construit et contrôlé

21 octobre 2026. **CONSTRUIT / CONTRÔLÉ : [47/47 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39669799213), zéro avertissement MSVC ; nouveaux comportements EXPÉRIMENTAUX, non validés en jeu.** Base cumulative V1.77, retours utilisateur intégrés, branche debug seulement. [Détail et essais](V1_78_PISTOL_OFF_LIFECYCLE_CLEANUP_TEST.md).

Validations rapportées sur V1.77 : visée sans tir, sauvegarde des réglages, updater ; OFF gauche avec seul le pistolet droit visible à pied ; autres pouvoirs de Strife et pouvoirs de War semblent fonctionner dans les essais rapportés. HUD général non reproduit. Heavy landing reste incertain, coop attendra. Ne pas étendre ces validations à toutes les combinaisons de réaffectation.

Construit : persistance visuelle OFF dans les roues, après ultime, et masquage des quatre modèles montés par côté ; bras droit OFF incluant son épaule dans la copie de rendu, branche gauche validée conservée ; holsters à pied masqués par leurs deux os vérifiés. Never ajouté au trait/arc, groupes du menu fermés par défaut. Patch revu des deux audits appliqué : 167 lignes nettes retirées dans 12 fichiers, appels natifs préservés. Nettoyage intégré au build contrôlé. [ZIP immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/08a5a2ace26e80ea8950a31996ca7fed6650a8ad/downloads/DarksidersGenesis_V1.78_PISTOL_OFF_LIFECYCLE_CLEANUP_TEST.zip) · [Reçu, empreintes et rollback](V1_78_DELIVERY_RECEIPT.md).

World Ender : deux activations enregistrées en lecture seule, 2 932 échantillons sans erreur ; référence du rayon présente durant environ 2,8 s, puis destruction native. Une lecture ponctuelle après sa fin avait été présentée à tort comme absence de création ; conclusion corrigée après bilan complet. Deuxième capture : 2 864 échantillons, aucune erreur ; acteur de rayon, template `P_Strife_worldender_scale1` et `MayhemBeamEffect` présents. L’utilisateur confirme dégâts et visibilité uniquement en vue normale ; Default Only en TPS ne le rétablit pas. Défaut de présentation TPS encore ouvert, aucune correction prétendue. Inspection privée de trois effets / 1 096 exports, matériaux et montage, sans changement d’asset.

Ajout demandé pour plus tard : caméra TPS ne suivant pas assez la hauteur pendant les exécutions, action hors champ. Les choix étrangers parmi les six capacités restent à adapter. Autres FX P2 toujours reportés. Aucune installation ou relance par l’agent ; INI, PAK et sauvegardes conservés. Les sections suivantes sont historiques.

---

## V1.77 — construite et contrôlée, réaffectation native du Courroux

Complément du 21 octobre : [double audit des leftovers](AUDIT_DOUBLE_NETTOYAGE_V1_77.md), deux méthodes confrontées. Proposition de nettoyage **167 lignes nettes / 12 fichiers**, patch séparé non appliqué et non compilé. Les sources actives et le ZIP V1.77 restent inchangés pendant les essais utilisateur. Intégrer au prochain lot cohérent et repasser les contrôles ; l'échange des six capacités et OFF universel restent ouverts.

21 octobre 2026. **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE, non validée en jeu.** Poursuite autorisée sans attendre l'essai du lot ; aucune nouvelle validation utilisateur supposée. Base cumulative V1.76, base explicitement validée V1.73 dans les essais rapportés. V1.75 toujours retrouvée installée sur disque ; aucun fichier du jeu remplacé par l'agent. Installation manuelle ultérieure.

Trois choix indépendants pour Strife et trois pour War, pour l'instant **parmi les trois pouvoirs de chaque personnage**. Nouveau panneau Gameplay → Wrath ability slots ; Native partout par défaut. Une seule interception du lookup natif avec déverrouillage et coût, sans service par frame ni cache d'objets. Le choix libre parmi **les six capacités reste ouvert** : adaptation des animations, notifications et composants nécessaire. Les indications de roue peuvent encore décrire l'emplacement initial. Visée sans tir, réception lourde V1.75 et OFF à pied V1.76 conservés, leurs essais en jeu toujours attendus.

[CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39632822621) : **47/47 suites, zéro avertissement MSVC**. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b5da4721104b0b5a08911bf8cc51af88a4559f6f/downloads/DarksidersGenesis_V1.77_NATIVE_WRATH_SLOTS_TEST.zip), 856818 octets, SHA256 `47507C0A5901B8C2E44DFBC16EFB5964A6523C35759EE2FBF5D0C9625CEE15C1`. Quatre fichiers racine, CRC/AMD64, 163 clés INI dont six nouvelles Native, 157 anciens réglages et 55 ID Hotkeys conservés, 141 anciennes archives inchangées. Quinze modules de caméra, visée, dash, ressources, holsters et OFF inchangés par rapport à V1.76 ; main/archive/v1.43 conservés. [Reçu, preuves et rollback](V1_77_DELIVERY_RECEIPT.md) · [Essais groupés](V1_77_NATIVE_WRATH_SLOTS_TEST.md).

La recherche étrangère progresse séparément : 40/40 fonctions Blueprint des six pouvoirs entièrement décodées, dépendances du clone et du rayon précisées ; aucune capacité étrangère lancée. Suite : adaptation entre personnages, OFF universel encore ouvert. HUD général en surveillance ; autres FX P2 reportés ; notification updater à confirmer. Les sections suivantes sont historiques.

---

## V1.76 — construite et contrôlée, candidat OFF à pied

21 octobre 2026. **CONSTRUITE / CONTRÔLÉE / NON INSTALLÉE PAR L'AGENT ; nouveauté EXPÉRIMENTALE, non validée en jeu.** Base validée V1.73 dans les essais rapportés, suite cumulative de V1.75 dont les nouveautés attendent le test utilisateur. V1.75 a été retrouvée installée sur disque ; ne pas transformer cela en validation de ses fonctions.

Nouveau choix OFF par main pour Strife normal à pied : modèle masqué, bras au repos dans les poses de tir par copie de rendu, tirs/sockets natifs conservés. Ranger les armes et rester immobile brièvement pour enregistrer la pose. Mêlée, actions inconnues, cheval et ultime gardent leur présentation native. Effets propres à une main filtrés ; effets communs connus conservés avec une arme visible et retirés avec les deux OFF. **OFF universel de tous les effets/formes toujours ouvert**, six capacités encore en adaptation.

[CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39614279357) : **46/46 suites, zéro avertissement MSVC**. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/026664c3e06a85b3714af5591ff3bef135ff2cb1/downloads/DarksidersGenesis_V1.76_FOOT_PISTOL_OFF_TEST.zip), 850967 octets, SHA256 `9FF084D8FBF542EDC0D65D78F06A9DC3D06FC363D2EB5BFEBE72CE93ED6B2A27`. Quatre fichiers racine/CRC/AMD64, 157 clés INI et 55 ID Hotkeys conservés, 140 anciennes archives inchangées. Installation manuelle ultérieure ; aucun fichier du jeu remplacé par l'agent. [Reçu, intégrité, essais et rollback](V1_76_DELIVERY_RECEIPT.md).

Tester plus tard dans le même lot : OFF droit/gauche/deux puis Native, tirs normaux/Hotstreak et dash, mêlée/roues/formes/P2 ; visée sans tir et réception lourde V1.75. [Recherche des capacités](V1_76_WRATH_ADAPTATION_RESEARCH.md). HUD général en surveillance, autres FX P2 reportés, notification de mise à jour à confirmer. Les sections suivantes sont historiques.

---

## V1.76 — candidat OFF par pistolet à pied, préparation

Base validée : V1.73 dans les essais rapportés. V1.75 est construite/contrôlée mais ses deux nouveautés restent en attente du test utilisateur ; elles sont reprises cumulativement sans validation inventée. L'utilisateur demande de poursuivre et testera le lot plus tard ; installation manuelle, aucune fermeture ou relance à demander.

**PRÉPARÉ / EXPÉRIMENTAL, pas encore construit ni validé en jeu.** Nouveau choix OFF par main dans Appearance, Strife normal à pied, utilisant une pose rangée native et une copie de rendu. Aucun changement des poses des sockets, du calcul de projectile, des actions, du mouvement ou des munitions. Modèles gauche/droite existants, holsters et fonctions V1.75 conservés. Les deux choix fournis restent Native ; aucun nouveau raccourci ou clé INI.

Limites explicites : cheval/ultime restent natifs ; effets de bouche individualisés filtrés par main, effets communs connus retirés seulement avec les deux OFF. Ce n'est donc pas encore le OFF universel de tous les effets/formes. Les six capacités ne sont pas échangeables dans ce build ; adaptation des montages et composants toujours ouverte. [Objectif, preuves, tests et rollback](V1_76_FOOT_PISTOL_OFF_TEST.md). CI et paquet à compléter après résultats réels.

---

## V1.75 — construite et contrôlée, installation manuelle

21 octobre 2026. **CONSTRUITE / CONTRÔLÉE / NON INSTALLÉE ; nouvelles fonctions EXPÉRIMENTALES, non validées en jeu.** Base V1.73 validée dans les essais rapportés, canal debug. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39537334409) : 45/45 suites, zéro avertissement MSVC. Visée seule manette avec un/deux boutons et nouveau candidat de déplacement après réception lourde automatique ; les deux sont OFF par défaut. Aucun ancien Recovery du dash réactivé.

[ZIP cumulatif public vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/a816cc3db3c446fac407b93dc881058c19e7e43d/downloads/DarksidersGenesis_V1.75_PAD_AIM_HEAVY_LANDING_TEST.zip) : 836203 octets, SHA256 `B6D1F0CE0D3ACE5B5CF27A3C75373778707CC84EA9FA41764B4036B5ABB6EE08`. Quatre fichiers racine/CRC/AMD64 ; 156 valeurs INI antérieures, 55 ID Hotkeys et 139 anciennes archives conservés. main/archive/v1.43 inchangés. Installation manuelle demandée ; V1.73 reste installée, aucun fichier du jeu remplacé.

À tester ensemble : maintien/relâchement de visée native/TPS, personnages et P2 ; réception lourde OFF/ON après haute chute (Strife puis War) ; contrôle court dash/tirs spéciaux/roues/rangement. Efficacité Recovery encore inconnue, War_HeavyLand non observé dans la capture. [Reçu, intégrité, essais et rollback](V1_75_DELIVERY_RECEIPT.md).

**Suite :** adaptation des six capacités, OFF complet par pistolet. Ni sélecteur inerte ni OFF partiel livré. HUD en surveillance, autres FX P2 reportés, notification de mise à jour à confirmer. [Preuves](V1_75_WRATH_HEAVY_LANDING_PISTOL_RESEARCH.md). Les sections suivantes sont historiques ; leurs statuts avant construction ne remplacent pas ce reçu.

---

## V1.75 — lot visée manette et réception lourde, préparation

Base V1.73 validée dans les essais rapportés, visée V1.74 construite/contrôlée non installée. Nouveau candidat **Heavy landing movement recovery**, OFF par défaut : classes HeavyLand exactes, deux verrous natifs de mouvement après impact, sans hook, Finish, timer, touche ou changement de dash. L'utilisateur confirme qu'il s'agit de la réception lourde automatique après chute, pas de l'attaque plongeante. Capture7480échantillons, trois rejets, six capacités natives et trois HeavyLand Strife ; aucun War_HeavyLand relevé. Nouvelle visée et nouveau candidat **PRÉPARÉS / EXPÉRIMENTAUX**, efficacité en jeu non validée. [Objectif, limites, essais et rollback](V1_75_PAD_AIM_HEAVY_LANDING_TEST.md).

Les capacités ne sont pas encore échangeables : les deux lookups ne reconnaissent que leurs propres trois IDs, les montages utilisent deux squelettes et des composants diffèrent. OFF complet par pistolet non livré ; frontière de copie de rendu identifiée sans modification. Ces sujets restent ouverts. [Preuves](V1_75_WRATH_HEAVY_LANDING_PISTOL_RESEARCH.md). HUD en surveillance, autres FX P2 reportés.

Installation **manuelle par l'utilisateur à la fin de sa session**. Aucun nouveau fichier du jeu, INI, PAK ou sauvegarde remplacé par l'agent. Anciennes archives et branches protégées conservées ; ne pas demander de fermer le jeu. CI et paquet encore à vérifier à cette étape.

---

## V1.74 — construite et contrôlée, installation manuelle reportée au prochain lot

Base cumulative V1.73, ses trois essais rapportés validés. Nouvelle visée manette sans tir : **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE**, pas validée en jeu. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39498950576) : **44/44 suites, zéro avertissement MSVC**. Source `61d9d4b01405ceb45ae802c348cce6dcfaf6a053`, archive `53e2b1645f83e2449eea98d01d3f19bc39ab921f`. [ZIP cumulatif vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/53e2b1645f83e2449eea98d01d3f19bc39ab921f/downloads/DarksidersGenesis_V1.74_NATIVE_PAD_AIM_TEST.zip), 831912 octets, SHA256 `578DC4AC279472C28556F7B930AFCB2FB6C4E6D53D84E4E8B91616A7AC29AA6B`. Quatre fichiers racine, CRC, AMD64, 156 clés fournies, 55 ID Hotkeys conservés ; les 154 anciens réglages gardent exactement leurs valeurs. Les 138 anciennes archives, main/archive, tag v1.43 et fichier de reprise préexistant sont inchangés.

Le README du ZIP contient la préparation V1.74 avant CI sous le titre historique de sa base V1.73 ; le présent reçu et la version compilée V1.74 font référence. L'archive publiée reste immuable. Une tentative d'installation s'est arrêtée dès le contrôle GAME_RUNNING, **avant sauvegarde ou remplacement**. L'annonce précédente de fermeture déduite d'une lecture sandbox était erronée : le contrôle final avec les permissions d'installation a vu le processus actif. Aucun fichier du jeu remplacé. L'utilisateur précise ensuite qu'il installera **manuellement à la fin de sa session** et demande de regrouper davantage de travail ; ne plus demander sa fermeture ou installer automatiquement ce lot.

Suite : capacités et réception lourde, puis OFF pistolet selon preuves. Choix demandé : trois emplacements par personnage, Native ou n'importe laquelle des six capacités. Chaque Blueprint GetWrathAbilityById ne reconnaît que ses propres trois ID ; les six montages utilisent deux squelettes distincts. Le remplacement de FName seul serait inopérant. Ne pas livrer six options présentées fonctionnelles avant adaptation et contrôles des animations/composants. [Recherche du prochain lot](V1_75_WRATH_HEAVY_LANDING_PISTOL_RESEARCH.md).

Capture VM_READ achevée : **7 480 échantillons, trois lectures incohérentes rejetées**, six capacités natives des deux personnages relevées. Trois HeavyLand de Strife au sol, position verticale stable, durées observées d'environ 0,9–1,2 s ; aucun HeavyLand de War capturé. Rampage Slam et AirDash Land de War sont distincts. Clarification utilisateur : Recovery vise **la réception lourde automatique après chute**, pas l'attaque plongeante. Pas encore de nouveau correctif Recovery construit ou testé.

---

## 20 octobre 2026 — V1.74, source manette de visée native

Action identifiée dans l'enum vivant : AIM31 / AIM_RELEASE32, distincte de RANGED_ATTACK_ONE18 / TWO20. Interface personnage +8D0 : vtable2708780, dispatch681D50, query669D30, gestionnaire acteur664A20. Dispatch respecte les gardes natives ; query lit l'état d'action. Aucun appel ni écriture par la capture externe VM_READ. Nouveau module préparé : signatures, possession réciproque pawn/controller, rôle3, identité faible et callback de jeu ; deux leases par joueur, sources natives composées, annulation respectée, relâchement rejeté différé, échantillon périmé500ms, pas de boucle d'acquisition ni ancien acteur rappelé.

Les deux bindings Controls/AimWithoutFirePadButtonA/B sont optionnels, OFF, sans révision du profil ou ID Hotkey. Lecture physique originale seulement ; état de direction TPS séparé du tir. Tests préparés sur dispatch/query substitués, cycle de maintien, composition, annulations, joueurs, profils et vrai getter coop. [Périmètre et critères V1.74](V1_74_NATIVE_PAD_AIM_TEST.md). Pas encore de réussite de compilation/CI/jeu annoncée.

Demande capacités : choix libre parmi les six pour chaque emplacement des deux personnages, confirmé par l'utilisateur. Inspection native avant toute interception ; conserver coût, lifecycle, commandes reconfigurables, compatibilité personnage et rollback. Ne pas confondre classe de tir, capacité de Courroux et animation, ni copier arbitrairement un pointeur entre personnages.

---

## HUD non reproduit et poses de Strife relevées — 20 octobre 2026

Retour utilisateur après refus d'arène verrouillée : **« hud toujours visible »**. Défaut placé en surveillance ; aucune correction dédiée ni attribution à un ancien changement. Captures avant/après V1.73 complètes, zéro erreur : pile UI vide, HUD final bShowHUD vrai et nouvelle racine visible/opacité1. L'identité de la racine a changé ; ne pas affirmer une conservation du même widget.

Essai des bras effectué par l'utilisateur : repos, visée seule, Standard et Charge Shot. Capture externe VM_READ terminée : **2 939 échantillons retenus, 30 lectures refusées par les contrôles de cohérence**, aucun changement du jeu. AnimInstance reliée au Strife possédé ; hiérarchie de80os, états natifs de visée et classes de tir relevés. Source active de pose identifiée ; les tableaux Cached* sont vides dans cette session. Le chemin natif des sockets de tir consulte aussi les poses : modifier directement les os ne préserverait pas leur origine. OFF reste ouvert ; recherche d'une séparation visuelle, sans patch direct des poses ou de montage. [Preuves et limites](AUDIT_PISTOL_OFF_HUD_2026_10_20.md).

V1.73 et ses acquis restent en place. Aucun nouvel ASI/ZIP, aucune écriture de configuration, PAK ou sauvegarde. [File restante](ACTIVE_QUEUE.md) : OFF, puis ground slam ; autres FX P2 reportés et Auto check update à confirmer visuellement.

---

## Retour utilisateur V1.73 — trois essais confirmés, 20 octobre 2026

Réponse «1. marche /2. marche /3. marche /prêt» : conservation des montures au chargement, réglages DOF et contrôle visée/tir/dash **VALIDÉS dans les essais rapportés**, sans extrapolation à des cas non décrits. Le jeu est lancé par l'utilisateur. Capture HUD initiale en lecture seule : parcours complet de307380objets,9853objets UI retenus, zéro erreur, racine générale Visibility4/opacité1. Aucun correctif HUD ni résultat après refus encore annoncé ; reproduction demandée.

[File restante remise à jour](ACTIVE_QUEUE.md) : OFF complet par pistolet, HUD général intermittent, puis ground slam. Autres icônes/FX P2 reportés ; Auto check update déjà codé, retour visuel encore attendu. Ancienne file archivée intégralement, aucun historique perdu. Aucun changement de binaire, INI, PAK ou sauvegarde.

---

## Suite après V1.73 — audit OFF / HUD, 20 octobre 2026

**AUDIT / OBSERVATION PRÉPARÉE**, aucun changement ASI ni nouveau build. Inspection privée de 18 assets / 121 exports : 14 montages utilisent Shoot_Right, y compris des montages qui font tirer les deux pistolets ; le cheval utilise Blend. Le slot ne désigne donc pas une main. Arrêt/filtrage d'un slot par main écarté ; conserver les événements de projectile et le correctif dash validé. Les effets spéciaux sur sockets communs et la pose partagée restent à résoudre.

HUD général intermittent : ancienne comparaison corrigée pour exclure les octets voisins et les fonctions. Aucun état fautif global démontré ; nouvelle observation avant/après à largeur exacte et propriétaires recontrôlés préparée hors ASI. Jeu fermé lors du contrôle, arrêt sans lancement ni capture. Préparation utilisateur en attente. [Preuves, piste écartée et prochaine étape](AUDIT_PISTOL_OFF_HUD_2026_10_20.md). V1.73 reste installée ; aucune nouvelle validation en jeu. Ground slam après le reste, autres FX P2 reportés.

---

## V1.73 — CONSTRUITE / CONTRÔLÉE / INSTALLÉE, nouveaux comportements EXPÉRIMENTAUX

20 octobre 2026. Base cumulative V1.72 contrôlée, retours V1.71 pris en compte. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39469924007) : **43/43 suites, zéro avertissement MSVC ASI/DXGI**. La suite propre au candidat Recovery supprimé est retirée ; compatibilité testée dans LegacyRecovery. Avis CMake/MinHook et Node des outils distincts. Source `65f43520c99eda56cf0b63acbb37b7e4d1e83050`, archive `516db3d8cd898e9d544c571a2ba73fdfa38d070b`. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/516db3d8cd898e9d544c571a2ba73fdfa38d070b/downloads/DarksidersGenesis_V1.73_MOUNT_DOF_CLEANUP_TEST.zip) : **826993 octets**, SHA256 `1D768FD2F9E178AE969685B23DB91220BEBCDCC71F5BC8DB2FADB98F3A84AF38`. HTTP 200/hash, CRC, AMD64 et quatre fichiers racine contrôlés. **154 réglages fournis, 55 identifiants de raccourci conservés, 137 anciennes archives inchangées** ; main/archive et v1.43 préservés.

Installation locale contrôlée après fermeture du jeu par l'utilisateur et sauvegarde : ASI/DXGI/README vérifiés, INI personnel et raccourcis conservés, aucun PAK ni sauvegarde modifiés. Aucun lancement par l'agent. Le README du ZIP décrit la préparation avant CI ; ce reçu complète son statut. **Nouveau résultat en jeu NON VALIDÉ.**

Lot : maintien des montures bonus déjà choisies pendant le chargement, y compris disponibilité désactivée, comme les armures ; distance DOF native/TPS jusqu'à **5 000 m** ; retrait du diagnostic périodique des skins et du Recovery dash sans effet. Le modèle de cheval de base reste sélectionnable et son choix n'est jamais remplacé automatiquement. Le filtre natif reste complet pour les autres skins/DLC. Les valeurs par défaut et toutes les fonctions validées restent conservées.

Retour utilisateur : Recovery V1.71 sans effet dans l'essai, **candidat REJETÉ**. Code, politique, état, appel de permission MOVE, checkbox, réglage runtime et clé de l'INI fourni supprimés. Le module de dash revient au code V1.70, vérifié à l'identique pour le comportement ; seule la phrase périmée du bilan a changé. Les anciens ID 3 et 54 et leurs tokens restent réservés/inertes pour conserver les affectations personnelles, exclus des nouvelles assignations ; aucune lecture/écriture de l'ancienne clé de fonctionnalité. La prochaine recherche Recovery commencera par le **ground slam**, après le reste de la file, comme demandé.

Preuves du défaut de monture : Despair existe dans le joueur, le SaveGame actif et le disque avant chargement ; après, seules les armures restent au joueur/SaveGame alors que le disque contient encore Despair. Filtre natif 6A2460 de retrait des lignes DLC identifié. Le candidat conserve seulement les deux choix bonus réellement retirés, via SetSkin sur cette liste. Classes, appel, signatures, types et tailles bornés contrôlés. Aucun choix imposé, droit DLC modifié, patch global de table ou écriture directe de fichier. [Audit natif](V1_72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.md).

Essais groupés :

1. Choisir Despair et Rampage, sauvegarde normale, recharger puis redémarrer ; vérifier sélection ET modèle. Décocher Bonus mount skins et recharger : bonus équipé conservé avec disponibilité du menu native. Choisir le cheval de base et recharger : il reste de base.
2. DOF Native/TPS : 20 / 1 250,5 / 5 000 m, quantité 0 / 0,6 / 2 ; saisie et rechargement, arène → mission/hub, chapitre 8 et transformations. Le dernier rendu V1.71 semble bon dans l'essai utilisateur ; nouvelles distances à valider.
3. Vérification courte des acquis : déplacement + visée + tir spécial + dash, native/TPS. Pas de nouvel essai Recovery demandé.

Suite : OFF complet par main des pistolets (modèle, effets, bras ; projectiles conservés), HUD général intermittent après refus d'arène verrouillée s'il réapparaît, puis étude du ground slam. Autres FX P2 reportés explicitement. V1.72 est préservée, mais n'a pas été installée : V1.73 intègre le dernier retour avant installation. [Critères et rollback](V1_73_MOUNT_DOF_CLEANUP_TEST.md). Les états historiques suivants ne remplacent pas ce reçu.

---

## V1.72 — CONSTRUITE / CONTRÔLÉE, nouveaux comportements EXPÉRIMENTAUX

20 octobre 2026. Base cumulative V1.71, acquis validés préservés. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39468587155) : **44/44 suites, zéro avertissement MSVC ASI/DXGI**. Avis CMake/MinHook et Node des outils distincts. Source `2973184516b1b035d47e80f800b0833af6411d37`, archive `9dc0eef9afdb237fea1953fea2df7c9632fdbbee`. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9dc0eef9afdb237fea1953fea2df7c9632fdbbee/downloads/DarksidersGenesis_V1.72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.zip) : **829605 octets**, SHA256 `A2D87C852B5CBB836B57ECC5EECF3ABCC8FC33B4D59CA829D72BE2A7F78190D2`. HTTP 200/hash, CRC, AMD64 et exactement quatre fichiers racine contrôlés. **155 clés et 55 actions conservées ; 136 anciennes archives inchangées.** Main/archive et v1.43 préservés.

Installation locale prête mais non effectuée : le jeu est encore ouvert. Demande de fermeture en attente ; aucun fichier du jeu remplacé par cette livraison à ce stade. INI, raccourcis, PAK et sauvegardes conservés. Aucun lancement par l'agent. Le README du ZIP décrit la préparation avant CI ; ce reçu complète son statut. **Nouveau résultat en jeu NON VALIDÉ.**

Lot : maintien des deux choix de monture bonus au chargement et distance DOF native/TPS jusqu'à **5 000 m**, sans changer les valeurs par défaut. L'option de skins contrôle seulement leur disponibilité dans le menu : un bonus déjà équipé reste conservé même OFF, comme les armures. Choisir le modèle de base le conserve normalement aux chargements suivants, sans réimposition du bonus. Le filtre natif reste complet pour les autres skins et droits DLC. L'ancien diagnostic périodique V1.71 et son appel par image sont retirés ; un seul hook de chargement ciblé, pas d'empilement de correctifs de personnage.

Preuves : sur la session V1.71, Despair existe dans le joueur, le SaveGame actif et le disque avant rechargement ; après, les deux armures restent et Despair manque au joueur/SaveGame, alors que le disque le contient encore. Filtre natif 6A2460 identifié : retrait des lignes marquées DLC absent. Réinsertion des seuls choix bonus retirés via SetSkin natif, dans cet appel. Classes, appel, signatures, types et listes bornés contrôlés ; pas d'écriture directe de sauvegarde ni de droits DLC. Les simulations vérifient notamment OFF conservant le choix mais pas sa disponibilité, modèle de base/non-bonus non forcé, autre choix natif prioritaire et absence de doublon. [Audit, limites et rollback](V1_72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.md).

Essais groupés :

1. Choisir Despair et Rampage, laisser la sauvegarde normale, recharger puis redémarrer ; vérifier sélection et modèle. Refaire case décochée : bonus équipé conservé, disponibilité du menu native. Choisir ensuite le cheval de base et recharger : il doit rester de base.
2. DOF : Native/TPS, saisie 20 / 1 250,5 / 5 000 m et quantité 0 / 0,6 / 2 ; redémarrage, arène → mission/hub, chapitre 8, angles, pied/cheval/ultime et changements de personnage. Dernier retour V1.71 : flou « ça a l'air ok », limité à cet essai.
3. Dash movement recovery V1.71 OFF/ON, Strife troisième/quatrième dash et War, déplacement/visée/tir, native/TPS ; toujours OFF par défaut, bénéfice sans nouveau verdict. Rejeter en cas de glissement ou régression.

OFF complet des pistolets et HUD général intermittent après refus d'arène verrouillée restent ouverts ; autres effets P2 reportés. La première compilation V1.72 a été annulée pendant la construction après la précision utilisateur sur OFF ; elle n'a publié aucune archive. La CI ci-dessus inclut cette précision. Les états historiques ci-dessous ne remplacent pas ce reçu.

---

## V1.71 — CONSTRUITE / CONTRÔLÉE / INSTALLÉE, lot EXPÉRIMENTAL

20 octobre2026. Base V1.70, acquis validés conservés. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39454685096) : **44/44 suites, zéro avertissement MSVC ASI/DXGI**. Source `4ec834595bcf3aca9809006862019459b29f4257`, archive `82a5c678985c5ed397466a37f6ef28fc56ce1bb1`. [ZIP immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/82a5c678985c5ed397466a37f6ef28fc56ce1bb1/downloads/DarksidersGenesis_V1.71_DASH_MOVE_BATCH_TEST.zip) :826278octets, SHA256 `ABE9F9E24C7BBD53E315D12B142E74EE82B74B9FCD608DE924A05E3148310A82` ; HTTP200/hash, CRC, AMD64 et quatre fichiers racine contrôlés. **155 clés,55 actions,135 anciennes archives inchangées** ; main/archive/v1.43 conservés.

Installation locale avec sauvegarde, jeu fermé : ASI/DXGI/README vérifiés, INI et raccourcis personnels conservés, aucun PAK ni sauvegarde modifiés. Aucun lancement. Le README du ZIP décrit la préparation avant CI ; ce reçu complète son état. **Le nouveau rendu et la reprise du déplacement sont NON VALIDÉS en jeu.**

Lot cumulé : réglages DOF par vue et région native V1.70 ; candidat Dash movement recovery OFF par défaut, sans nouveau hook. Il attend une décélération observée terminée, une vélocité quasi nulle ET une position réelle stable sur deux observations, puis permet une seule demande native MOVE réelle par activation. L’ancienne fin forcée Recovery n’est pas réintroduite. Nouvelle action Hotkeys54, sans affectation ; anciens ID/bindings inchangés. Le simulateur du callback vérifie P1/P2 et native/TPS, refus pendant déplacement réel malgré vélocité nulle, pause et implémentation inconnue, absence de fin forcée ou d’écriture d’acteur/mouvement. Ces contrôles ne prouvent pas le bénéfice en jeu. [Objectif, preuve, essais regroupés et rollback](V1_71_NATIVE_DASH_MOVE_BATCH_TEST.md).

Les nouvelles observations de sélection des chevaux sont un **diagnostic borné**, sans choix forcé ni écriture save. Copie privée de6sauvegardes conservée pour comparaison, sources inchangées. Le dernier relevé des deux parties actives contient les armures sans les entrées bonus de monture ; il confirme la perte, pas sa cause. **Chevaux au chargement et OFF complet pistolet restent OUVERTS.** Relecture privée de167fonctions Blueprint, tailles de script toutes concordantes ; slots Shoot_Left/Right distincts mais visée partagée au niveau Spine1. Aucun OFF partiel ni manipulation de montage présentée comme terminée.

### Essais à faire ensemble quand l’utilisateur sera prêt

1. Rendu automatique : arène → hub/mission, chapitre8, angles, changements de personnage, pied/cheval/ultime.
2. DOF : groupes Native/TPS différents ; Disable puis Custom distance0/5/20m et quantité0/0,6/2 ; saisie, pause/roues, cinématique, coop, redémarrage.
3. Dash : nouveau candidat OFF/ON, Strife trois dashs puis quatrième, War, déplacement maintenu puis nouvel appui, visée et tir spécial, native/TPS et coop. Rejeter le candidat si aucun bénéfice net, glissement, premier dash répété ou régression de visée/tir ; OFF arrête les prochaines demandes.
4. Skins : choisir les deux bonus avec option cochée, laisser une sauvegarde habituelle, recharger puis redémarrer. Les observations de listes et les fichiers privés permettront ensuite de comparer le point de perte ; réparation non annoncée.

HUD général après refus d’arène verrouillée intermittent ; autres sprites/FX P2 reportés. Pas de test immédiat supplémentaire demandé. Les préparations historiques ci-dessous ne remplacent pas ce reçu.

---

## Préparation V1.71 — lot cumulé, 20 octobre2026

Demande utilisateur : poursuivre et regrouper les essais. Base V1.70 construite/installée, rendu encore expérimental ; acquis validés conservés. Nouveau candidat **Dash movement recovery** OFF par défaut : reprise par la cause MOVE native après décélération observée et arrêt, sans fin d’ability forcée. Observation des deux listes de skins chevaux, bornée et sans écritures. OFF complet pistolet : slots gauche/droite identifiés, couche de visée partagée ; pas encore livré. **PRÉPARÉ / EXPÉRIMENTAL**, aucune validation de jeu supplémentaire. [Preuves, critère, essais groupés](V1_71_NATIVE_DASH_MOVE_BATCH_TEST.md).

## V1.70 — CONSTRUITE / CONTRÔLÉE / INSTALLÉE, rendu EXPÉRIMENTAL

20 octobre 2026. Base V1.69 et acquis préservés. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39443047647) : **43/43 suites, zéro avertissement MSVC ASI/DXGI**. Avis d’outillage CMake/MinHook et Node distincts. Source `3ed44ddf0aba240e2b85a96fa2f8d9cdec231939`, archive `e7a9a18b166480b296780191e22ed85e81bfbc57`. [ZIP immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/e7a9a18b166480b296780191e22ed85e81bfbc57/downloads/DarksidersGenesis_V1.70_PER_VIEW_DOF_TEST.zip) : **822447 octets**, SHA256 `2FF56D6E2995985F462EA2D85CEED1A245A39C7D5588D18A0D3450C1B06DE6CE` ; HTTP200/hash, CRC, AMD64 et exactement quatre fichiers racine contrôlés. **154 clés,54 actions,134 anciennes archives inchangées** ; main/archive/v1.43 préservés. Aucun ancien défaut déclaré validé par la CI.

**Installée avec sauvegarde, jeu fermé : ASI/DXGI/README contrôlés ; INI personnel, raccourcis, sauvegardes et PAK conservés. Aucun lancement.** Le README contenu dans le ZIP décrit la préparation avant la CI ; ce reçu confirme les contrôles effectués après publication.

Correction du flou proche TPS : sans région déjà surchargée par la caméra, conserver celle du niveau ; la capture V1.69 montrait3000 unités remplacées par environ55 après l’arène. Deux profils **Native view / TPS view** : Disable, Custom distance0–200m et quantité0–5% absolue ; tous OFF par défaut, valeurs inactives10m/0,6%. Même disposition que les autres valeurs (curseur/saisie/Default). Pas de nouveau hook. Cinématiques ignorées par les profils. **Rendu NON VALIDÉ en jeu.** [Preuves, critères, essais et rollback](V1_70_PER_VIEW_DOF_TEST.md).

**Skins chevaux : bug OUVERT**, option Bonus mount skins toujours cochée au chargement, choix bonus à refaire ; armures semblent rester. La tentative de lecture supplémentaire a trouvé le jeu fermé et s’est arrêtée sans lancement. Copie native des sélections identifiée, point de perte non démontré ; aucun patch save/DLC ou sélection forcée. **HUD général après refus d’arène verrouillée : intermittent, non reproduit dans l’essai actuel**, pas de correction annoncée. OFF complet pistolet et nouvelle approche Recovery ouverts ; autres sprites/effets P2 reportés selon l’utilisateur.

### Essais V1.70

1. Laisser Disable et Custom OFF dans les deux groupes : arène puis hub/mission, chapitre8, plusieurs angles, changement de personnage ; le décor ne doit plus devenir flou à deux mètres.
2. En TPS : Disable ON/OFF, puis Custom distance0/5/20m et quantité0/0,6/2. Vérifier curseur et saisie, intensité, distance et retour au rendu du niveau.
3. Donner des valeurs différentes aux deux vues : TPS/native temporaire, pied/cheval/ultime, pause/roues, cinématique et coop. Recharger pour vérifier la persistance des réglages ; sélection des chevaux toujours en diagnostic.

---

## Reprise V1.70 — rendu par vue, 20 octobre 2026

Base cumulative V1.69 ; Léviathan et menu validés par l’utilisateur. Le flou V1.69 a un nouveau contre-exemple après l’arène : région volume3000 remplacée par environ55 dans la caméra TPS, capture439 relevés sans erreur. V1.70 préserve la région du volume en mode automatique et ajoute deux profils DOF indépendants Native/TPS (Disable, Custom distance/intensité), tous OFF par défaut. **PRÉPARÉ / EXPÉRIMENTAL**, compilation/paquet/rendu à vérifier. [Critères, preuves, essais et rollback](V1_70_PER_VIEW_DOF_TEST.md).

Skins chevaux : **ouverts**, retour modèle natif au chargement avec Bonus mount skins coché, sélection à refaire. Armures semblent conservées. HUD général après refus d’arène verrouillée : ancien défaut confirmé, non reproduit dans l’essai actuel ; aucune correction annoncée. Aucun patch de sélection/save ou HUD aveugle.

## Précision utilisateur — HUD général après refus d’une arène verrouillée

20 octobre 2026, après livraison V1.69. Le problème touche **le HUD général**. Déclencheur : cliquer une arène encore verrouillée ; un message indique que le joueur ne l’a pas débloquée, le menu devient transparent, puis le HUD général manque. L’ancienne abréviation « HUD d’arène » ne désignait pas le bon élément ; ne pas limiter la recherche au widget UI_ArenaHUD.

Les données cuites déjà relevées montrent StartMission puis l’animation outro dans le chemin de sélection ; cette animation possède deux pistes RenderOpacity. Le corps natif 585010 présente aussi une branche d’échec après le test 5841F0, avant le chemin de préparation de mission. Cela concorde avec le refus et la transparence décrits, **sans établir quelle branche a été exécutée dans la reproduction ni la cause finale**. Piste : transition de fermeture et restitution du mode UI/HUD après refus. Les captures précédentes (hide natif/mod=0, racine visible, opacité 1, IsInViewport vrai) restent pertinentes.

Prochain relevé ciblé : HUD général, page de sélection, message de refus, pile du gestionnaire UI et état de l’animation de fermeture, avant/après le clic. Critère de réussite : après refus, l’interface retrouve son état correct et le HUD général revient en reprenant le jeu, sans bascule manuelle du HUD. **Correction ouverte ; aucun nouveau patch ni build dans cette clarification.** ASI installé V1.69, INI, raccourcis et archives inchangés.

---

## V1.69 — CONSTRUITE / CONTRÔLÉE, corrections visuelles EXPÉRIMENTALES

20 octobre 2026. [GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39412347126) : **43/43 suites, zéro avertissement MSVC ASI/DXGI**. Avis CMake/MinHook et Node des outils distincts. Source `02aac6e0a04ad3fec7d2e7dfcc01425a0d08318f`, archive `28f923c0c3336c75d7b7ec76232fc22bc56a13b0`. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/28f923c0c3336c75d7b7ec76232fc22bc56a13b0/downloads/DarksidersGenesis_V1.69_NATIVE_VISUAL_FOCUS_TEST.zip) : **818621 octets**, SHA256 `C5197C120DD59FB607BF2B33AF56B15EC9D020DA33C4DB01A7E82D3591CFE514`. Téléchargement public HTTP 200, hash identique à la CI, CRC, AMD64, exactement quatre fichiers racine, marqueurs, **146 clés et 54 actions** contrôlés. **133 anciennes archives et trois documents historiques inchangés** ; main/archive et v1.43 conservés.

Base V1.68, acquis V1.66/V1.67 conservés. Lot : attache native Léviathan, méthode DOF du niveau et protection de la profondeur du personnage en TPS, sections Appearance / Weapon sheathing repliables. Ancien forçage CircleDOF retiré, aucun nouveau hook ou réglage. Nouveau rendu **NON VALIDÉ en jeu**. Installation locale contrôlée, jeu fermé et sauvegarde préalable : ASI/DXGI/README vérifiés, INI personnel et absence actuelle du PAK de skins conservés. Aucun lancement par l’agent. [Preuves, critères, essais groupés et rollback](V1_69_NATIVE_VISUAL_FOCUS_TEST.md).

### Skins : autonomie ASI établie dans le dernier essai

Le journal moteur de la session V1.68 du 20 octobre, ouverte à 12:47:13 et fermée à 12:57:56, n’enregistre que le montage de ProjectMayhem-WindowsNoEditor.pak. Le PAK PreOrder est absent du dossier. Avec le retour utilisateur sur la sélection des chevaux/armures et le retour natif OFF, cela confirme l’essai **sans le PAK cosmétique**. Maintien de l’apparence après redémarrage encore à tester. Aucun PAK déplacé/supprimé par l’agent. Cette preuve complète les réserves des paragraphes historiques suivants.

HUD après arène verrouillée, OFF complet pistolet et nouvelle approche Recovery restent ouverts. Les autres FX P2 restent reportés ; Auto check attend sa confirmation visuelle. Les paragraphes de préparation ci-dessous sont historiques, pas des contrôles encore en attente.

---

## V1.69 — préparation depuis les retours V1.68

20 octobre 2026. Durée ultime infinie **VALIDÉE par l'utilisateur** ; changement de personnage et netteté du héros au chapitre 8 semblent bons. Décors trop nets en TPS ; Léviathan fonctionne mais trop bas. Skins chevaux/armures et retour natif OFF confirmés ; maintien après redémarrage et autonomie sans PAK non certifiés.

Lot : attache Léviathan sur la racine native (écart mesh −97 prouvé par VM_READ), méthode DOF du niveau et protection de la profondeur du personnage, sections Appearance / Weapon sheathing repliables. **PRÉPARÉ / EXPÉRIMENTAL**, construction Windows et nouveau rendu à contrôler. [Preuves, limites, critères, essais et rollback V1.69](V1_69_NATIVE_VISUAL_FOCUS_TEST.md). HUD d'arène, OFF complet pistolet et nouvelle approche Recovery ouverts ; autres FX P2 reportés.

---

## V1.68 — CONSTRUITE / CONTRÔLÉE, nouveaux comportements EXPÉRIMENTAUX

20 octobre 2026. [CI Windows x64 réussie](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39404000199) : **43/43 suites**, aucun avertissement MSVC ASI/DXGI ; avis CMake/MinHook et Node des outils distincts. Source `eac8da5d1e67e6c3afd3e50d3f83c7acf569d7a8`, archive `b1a7110048071987facf6f28af0475b87c1a272b`. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b1a7110048071987facf6f28af0475b87c1a272b/downloads/DarksidersGenesis_V1.68_VISUALS_ULTIMATE_TEST.zip) : **817200 octets**, SHA256 `110C3E851C966FD26E87FE31450475553E2985A7158643430923117B2A486960`. Téléchargement public HTTP 200, hash identique à la CI, CRC, AMD64, quatre fichiers racine, marqueurs, 146 clés et 54 identifiants Hotkeys contrôlés. **132 anciennes archives inchangées.** Main/archive et v1.43 conservés.

Base : V1.66 validée pour sauts/holsters + choix des pistolets V1.67 validés. Lot : vrai IsSkinUnlocked à la place du mauvais IsSkinEquipped, focus du même point utilisé pour émettre la géométrie TPS pendant le changement, case **Infinite Ultimate duration**, visuel seul **Leviathan OFF/1/2/3**. Nouveaux résultats en jeu **NON VALIDÉS**. Aucun lancement ni remplacement des fichiers du jeu par l’agent à ce stade ; INI/raccourcis et PAK personnels préservés. [Objectifs, preuves, limites, tests groupés et rollback](V1_68_VISUALS_ULTIMATE_TEST.md). Les paragraphes « préparation » suivants sont historiques.

### HUD : reproduction capturée, correction encore ouverte

L’utilisateur a préparé un HUD visible puis a reproduit la disparition après sélection d’arène verrouillée. Capture du 20 octobre : PID 12880, **V1.67** chargée (ASI SHA256 `ECDE0F74B69B8A3558669A45422BB7D05C5ADDC2135EC4D9DA127D2FCF164286`). 8907 échantillons en cinq minutes, zéro erreur de lecture : `ui.HideHud` natif jeu/rendu reste **0/0**, qualité DOF **2/2**. Le booléen interne du mod `g_hudHidden`, identifié dans le getter compilé V1.67 et lu après signature du module, vaut également **0** ; aucun événement Toggle HUD dans le journal. Cela exclut les deux masquages de ce getter lors de ce relevé, sans prouver une cause extérieure à tout le reste du mod.

Deux instantanés avant/après, sans écriture ni appel natif : acteur HUD et racine générale présents, `bShowHUD` vrai, visibilité racine 4, opacité 1. Les pointeurs et propriétés ne doivent être comparés que sur leur taille réelle : les 16 octets de contexte capturés contiennent parfois le champ voisin ; les UFunction ne sont pas des propriétés d’instance. Le vrai test natif IsInViewport a ensuite été retracé (wrapper 15190A0, consommateur 14B41D0) et reproduit uniquement par lecture : pointeurs +208/+210 présents, compteur partagé +8 = 1, donc résultat vrai. Un simple retrait du viewport n’explique pas cette panne. L’arbre détaillé après panne est conservé en privé pour remonter les états d’affichage/bindings et couches Slate. **Cause et correction non établies ; aucune réparation HUD revendiquée dans le ZIP V1.68.** Les autres éléments possiblement manquants restent un signalement incertain. OFF complet des pistolets et autres tâches de la file restent ouverts.

---

## V1.68 — préparation expérimentale ; erreur de skins V1.67 corrigée à la source

20 octobre 2026. Base retenue : V1.66 (sauts/holsters validés), plus les sélecteurs gauche/droite de pistolet V1.67 **VALIDÉS par l’utilisateur**. Montures V1.67 **en échec** sur une nouvelle partie avec le PAK actif ; autres skins non testés. Le mauvais branchement IsSkinEquipped est retiré au profit du vrai IsSkinUnlocked, avec contrôle de l’association native nom/wrapper/appel. Les déclarations anciennes de provenance du rapport V1.67 sont corrigées dans [le rapport V1.68](V1_68_VISUALS_ULTIMATE_TEST.md).

Lot préparé : correction des skins, continuité de mise au point au changement de personnage, case durée ultime infinie (sans remplissage), sélecteur visuel Léviathan OFF/1/2/3. Trois clés nouvelles opt-in, 146 clés au total, 54 identifiants Hotkeys (anciens IDs conservés). **Construction Windows et vérification du ZIP encore à effectuer ; nouveaux résultats en jeu NON VALIDÉS.** Aucun lancement, installation, remplacement du PAK ou changement d’INI personnel.

Le flou signalé concerne plusieurs niveaux. Relevé : qualité DOF 2/2 conservée ; mode lentille TPS modifiant le rendu général, trou de compensation d’environ 0,64 s au changement observé. HUD absent après sélection d’arène verrouillée : **ouvert, aucune correction revendiquée**. Autres éléments HUD manquants : information incertaine. OFF complet des pistolets (modèle/effets/bras, projectiles conservés) encore ouvert. Recovery et autres effets P2 reportés restent séparés. [Essais groupés, preuves et rollback](V1_68_VISUALS_ULTIMATE_TEST.md).

---

## V1.67 — CONSTRUITE / CONTRÔLÉE, apparences EXPÉRIMENTALES

19 octobre 2026. Base V1.66 validée pour sauts/holsters, conservée. Choix du pistolet natif gauche/droit par main de Strife ; deux cases distinctes pour les montures bonus et les six apparences de progression. **Nouveaux comportements NON VALIDÉS dans le jeu.** [Inventaire, preuves, essais et rollback](V1_67_PISTOL_CHOICES_AND_SKINS.md).

[CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39253436601) : **42/42 suites, zéro avertissement MSVC ASI/DXGI** ; avis de dépréciation MinHook/CMake et Node distincts. Source `4a4bd1bee5126ee9a21cf78fe90048f6c509776d`, archive `c760123057e66f2b3d7053b7acf3441de2cee44e`. [ZIP cumulatif vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/c760123057e66f2b3d7053b7acf3441de2cee44e/downloads/DarksidersGenesis_V1.67_PISTOL_CHOICES_SKINS_TEST.zip) : 809061 octets, SHA256 `8E37AE2BBBAD9465BEFE2BC75F2566B74311EB0908E01682D1D6CD02EF3B9591` ; CRC, AMD64, quatre fichiers à la racine, marqueurs, 143 clés/53 actions, téléchargement HTTP 200 et signature identique contrôlés. **131 anciennes archives inchangées.** Configuration personnelle et PAK non modifiés, aucun lancement ni installation par l’agent. Main/archive et tag stable conservés.

Essais : combinaisons de pistolets à pied/cheval, tir/rangement et Native ; **fermer et rouvrir la page Skins après un changement d’option**, vérifier les deux groupes puis OFF. Le PAK déjà installé garde ses deux montures disponibles même avec la case ASI OFF ; l’autonomie ASI exige un essai sans ce seul PAK, déplacé réversiblement après fermeture du jeu. Choisir une apparence possédée avant de désactiver son groupe. Recovery et autres effets P2 restent hors du lot. Les préparations suivantes sont historiques ; elles ne signifient plus que la construction est en attente.

---

## V1.67 — choix des pistolets et skins, préparation expérimentale

19 octobre 2026. Base retenue V1.66 : sauts et holsters validés. Demande clarifiée : choisir le modèle du pistolet droit ou gauche dans chaque main, pas importer un modèle supplémentaire. Deux listes indépendantes préparées, variantes Stowed correspondantes à cheval, restauration par identité native. Une interception de disponibilité cosmétique, ciblée sur huit lignes exactes, propose séparément deux montures bonus et six apparences de progression ; aucun grant ni contrôle global de DLC remplacé. PAK de montures local analysé : deux tables, deux booléens par ligne bonus modifiés, aucun modèle ajouté. [Inventaire complet, preuves, limites et essais](V1_67_PISTOL_CHOICES_AND_SKINS.md).

**PRÉPARÉ / EXPÉRIMENTAL : construction Windows et contrôle du ZIP à effectuer, aucune validation en jeu de V1.67.** Quatre clés nouvelles Native/Native/OFF/OFF, 143 clés et 53 actions ; toutes les valeurs antérieures conservées. Sources sauts/visée/caméras/commandes/rangement/dash/ressources/grappin inchangées. PAK et INI personnels non modifiés, aucune relance/installation. Recovery et autres effets P2 hors de ce lot. Les reçus antérieurs restent historiques.

---

## V1.66 — sauts et holsters VALIDÉS par l’utilisateur

19 octobre 2026. Retour explicite : « pour les sauts c'est bon bravo », « pour les holsters c'est bon bravo », puis « donc tout a lair bon/ ». **Sauts et holsters VALIDÉS dans l’essai rapporté ; V1.66 devient la base de travail retenue sur debug.** Les trois délais de rangement et la durée ultime étaient déjà validés en V1.65. Aucun nombre de sauts, mode, personnage, cas coop ou délai mesuré n’est inventé à partir de ce retour général.

Mise à jour documentaire uniquement : code runtime, INI, raccourcis, binaires et ZIP V1.66 conservés. Pas de nouvelle build ni de promotion de main/archive. Action Recovery et modèles alternatifs de pistolets restent ouverts ; autres icônes/effets P2 toujours différés sur demande ; Auto check update sans confirmation visuelle distincte. [État courant](CURRENT_STATE.md) · [File active](ACTIVE_QUEUE.md).

Les critères et mentions « à tester » ci-dessous décrivent la préparation historique et sont remplacés, pour le statut des sauts et holsters, par ce retour utilisateur.

---

## Après V1.66 — Recovery natif, modèles et séparation de l’historique

19 octobre 2026. L’utilisateur teste V1.66 et demande de poursuivre le reste. **Préparation uniquement : pas de changement runtime/config, pas de build ni de nouveau relevé de jeu.** [Nouveaux faits et limites](NEXT_RECOVERY_AND_MODELS.md) : MOVE action 29/interruption 1, JUMP action 30/interruption 2 ; séparation masques d’action/interruption/requête ; raison native de fin sur un octet ; pipeline d’interruption et notification de fenêtre identifié, bénéfice sûr encore non établi. Capture externe de la fenêtre préparée sans attachement. L’ancien Recovery reste retiré.

Inventaire privé élargi : index PAK de 86641 entrées vérifié, 119 assets retenus ; quatre géométries de pistolet main/rangement seulement, autres variantes classées matériaux/textures ou corps complet. Aucun sélecteur de modèle alternatif livré. Autres sprites/effets P2 toujours différés.

README, état courant et roadmap raccourcis et rendus actuels ; anciennes versions intégrales préservées avec signatures dans [docs/history](history/DOCUMENT_HISTORY_MANIFEST.json). [File active](ACTIVE_QUEUE.md) pour la reprise. Les reçus, hypothèses et préparations suivants restent l’historique daté, sans annuler le statut courant.

---

## V1.66 — construite et contrôlée, nouveau cycle de saut expérimental

Base **V1.65** : holsters montés, trois délais de rangement et durée ultime **VALIDÉS par l’utilisateur** ; double saut non validé (deux remontées sans animation puis blocage). V1.66 remplace la substitution de compteur par un réarmement natif conjoint publié à l’animation et anticipe la recherche des modèles montés à pied. Aucun nouveau hook ni réglage. **CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL : les sauts animés et la réduction du délai de placement restent à vérifier en jeu.**

[Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39217494013) : **41/41 suites, zéro avertissement MSVC ASI/DXGI**. Source `f01f70d563dc4e3716464718fb6642bbdd8f9702`, archive `12061a956b9257284c8b1779503cf058b75fab1f`. [ZIP cumulatif V1.66 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/12061a956b9257284c8b1779503cf058b75fab1f/downloads/DarksidersGenesis_V1.66_NATIVE_JUMP_CYCLE_TEST.zip) : 934413 octets, SHA256 `CC15584E210717E4F173967BE47024CA949983FD24870B03190191936888F67F`. CRC, AMD64, quatre fichiers racine, marqueurs, lien public HTTP 200 et signature identique contrôlés. **139 réglages et 53 actions inchangés ; 130 anciens ZIP préservés**. Main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` et v1.43 `902861eb1ff34c14dc5ded092581b360767da817` vérifiés intacts. Les avis de dépréciation CMake de MinHook et Node des actions de transfert sont distincts des avertissements du code compilé.

**Essais groupés :** Always dix nouveaux sauts **avec animation**, After gliding un saut par nouveau plané, OFF/atterrissage, TPS/vue native ; délai de première montée après quelques secondes à pied, puis tir/rangement/descente. [Procédure, preuves, limites et rollback](V1_66_NATIVE_JUMP_CYCLE_TEST.md). Acquis de visée, dash, focus, grappin, munitions, ultime, rangement et commandes conservés ; effets/icônes P2 toujours reportés. Aucun lancement ni installation par l’agent, INI personnel intact. Si les sauts restent bloqués ou sans animation, le candidat est à rejeter et l’option à remettre OFF ; compilation et fixtures ne constituent pas une validation du rendu. Les préparations ci-dessous sont historiques.

---

## V1.66 — nouveau cycle de saut à tester, acquis V1.65 conservés

**Retour V1.65 :** modèles de holster montés **VALIDÉS** avec un délai de placement ; trois délais de rangement **VALIDÉS** ; durée ultime **VALIDÉE** après activation de sa case. **Double saut NON VALIDÉ** : deux remontées, sans animation, puis blocage. Ces retours remplacent les mentions « à tester » V1.65 ci-dessous.

**Candidat V1.66 EXPÉRIMENTAL, construction à effectuer :** retrait de la substitution du compteur dans DoJump ; réarmement conjoint des deux compteurs via leurs setters natifs au passage de lecture d’animation, après observation du double saut et relâchement de l’action native. Le moteur reçoit le vrai compteur disponible puis publie à nouveau deux au saut réussi. Aucun reset de nœud d’animation, compteur UE, vitesse ou touche forcé. Recherche bornée des modèles de holster anticipée à pied pour réduire l’attente à la montée ; placement validé conservé. [Preuves, limites, tests et rollback](V1_66_NATIVE_JUMP_CYCLE_TEST.md).

**À tester ensemble :** Always pendant au moins dix appuis, animation à chaque nouveau saut ; After gliding un saut par nouveau plané ; OFF, atterrissage et comparaison TPS/vue native ; premier montage après quelques secondes à pied, tir/rangement/descente. Effets et icônes P2 restent reportés à la demande de l’utilisateur. Aucune relance ni installation par l’agent, INI personnel intact. Historique antérieur conservé ci-dessous.

---

## V1.65 — construite et contrôlée, essais groupés à faire

**CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL — pas encore VALIDÉ par l'utilisateur.** Base cumulative V1.64 ; dernier jeu observé V1.63. Le lot regroupe correction du cycle saut/plané, essai réversible des modèles natifs de holster à cheval (nouvelle case OFF par défaut), trois délais de rangement et durée ultime 0–10 inclus depuis V1.64. Les acquis validés sont conservés.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39208468459) **SUCCESS : 41/41 suites, 0 avertissement MSVC ASI/DXGI**. Source `fdf7a8607e282d8da1b569c7b13828f41f0bdbe0`, archive `310c19b918f2b682add6e514f09bf2ba6fdf59a4`. [ZIP cumulatif V1.65 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/310c19b918f2b682add6e514f09bf2ba6fdf59a4/downloads/DarksidersGenesis_V1.65_AIR_JUMP_HOLSTERS_TEST.zip) : 931063 octets, SHA256 `7CEC19D21D94826FE7002A07849A250BA32EF31D5F1880456DF11973F0178CE6`. CRC, AMD64, quatre fichiers racine, marqueurs, **139 clés / 53 actions**, HTTP 200 et **129 anciens ZIP inchangés** vérifiés. Les 138 valeurs précédentes du profil fourni sont conservées ; seule MountedPistolHolsters=0 est ajoutée. Main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` et tag v1.43 `902861eb1ff34c14dc5ded092581b360767da817` revérifiés inchangés.

**Tests dans une même version :** (1) sauts OFF/Always au moins dix appuis/After gliding un saut par nouveau plané, vue native et TPS ; (2) Mounted pistol holsters ON/OFF, orientation puis tir/rangement/montée-descente ; (3) trois délais distincts et raccourcis de rangement ; (4) Ultimate decrease rate 0.5/1/2 puis 0, Unlimited OFF/ON. Auto-check V1.63 encore sans retour. [Détails, preuves et rollback](V1_65_AIR_JUMP_HOLSTERS_TEST.md).

La capture de l'ancienne version identifie le crédit effacé en mode plané et les modèles de holster ; elle **ne valide pas** les nouveaux comportements. Ajustement visuel des Stowed et continuité d'Always encore à confirmer en jeu. Les autres icônes et effets P2 sont laissés en attente à la demande explicite de l'utilisateur ; aucune modification de ces couches. Autres modèles 3D et nouvelle Action Recovery restent ouverts. Aucun lancement/installation par l'agent, aucune écriture de l'INI personnel. Blocs de préparation ci-dessous conservés comme historique.

---

## V1.65 — lot cumulatif sauts, holsters, rangement et ultime

**CANDIDATE EXPÉRIMENTALE — nouveaux comportements à valider en jeu.** Base cumulative V1.64, acquis validés conservés. Les trois délais de rangement (lames Strife / pistolets Strife / épée War) et Ultimate decrease rate 0–10 restent inclus ; l'utilisateur joue encore en V1.63 lors du relevé du 19 octobre. Aucun retour ne valide implicitement V1.64/V1.65.

**Sauts :** le plané utilise le mode natif Custom, pas Falling ; l'ancienne observation effaçait donc son crédit. Le candidat conserve ce crédit et, sur la fonction native de demande de saut, termine un plané éligible avec StopGliding avant de laisser la machine d'actions native décider du saut. Always ne dépend plus d'une observation caméra préalable. L'impulsion reste native, avec extension/restauration du seul délai pendant un saut supplémentaire tardif. Aucun compteur, vitesse, animation ou bouton physique n'est forcé. OFF reste natif. La capture confirme le défaut d'observation ; elle ne valide pas encore la correction ni la continuité d'Always.

**Mounted pistol holsters (experimental)** : nouvelle case dans Weapon sheathing, **OFF par défaut**. Essai limité aux deux composants de holster de Despair : modèles natifs Stowed à la place des modèles de main, déjà chargés, via SetStaticMesh natif ; restauration sur OFF/sortie du cheval et protection contre réutilisation d'objet. Aucun masquage, rotation, socket, projectile ou arme en main modifié. L'ajustement visuel est une hypothèse étayée par les géométries natives différentes, à vérifier avant toute validation.

**Essais groupés :** sauts Off / Always (10 appuis) / After gliding (un nouveau saut par plané), TPS et vue native ; holsters ON/OFF puis tir/rangement/montée-descente ; trois délais de rangement distincts et raccourcis communs ; durée ultime 0.5/1/2, puis 0, Unlimited OFF/ON. [Procédure, preuves, rollback et état de construction](V1_65_AIR_JUMP_HOLSTERS_TEST.md).

Icônes et effets de tir P2 laissés en attente à la demande de l'utilisateur. Autres modèles 3D et nouvelle Action Recovery encore ouverts. Anciennes archives, INI personnel et raccourcis préservés ; aucune installation ni relance par l'agent. Les blocs plus anciens ci-dessous décrivent leurs versions respectives.

---

## V1.64 — construite et contrôlée, trois délais et durée ultime à tester

Historique CI du lot : premier passage `39204933279` refusé par l'assertion de test HotstreakConfig restée à 52 actions après ajout de l'action 52. Assertion mise à jour pour conserver explicitement les anciens identifiants et vérifier Count=53, commit `9c10e8f819624bf03d7bcd677cdd62677a1c755d` ; aucun ZIP publié par le passage refusé. Le passage complet indiqué ci-dessous a ensuite compilé et exécuté les 40 suites.

**CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL — nouveaux réglages non encore VALIDÉS en jeu.** Base cumulative V1.63 ; grappin validé par l'utilisateur dans son essai, avec rangement manuel, netteté TPS, Standard limité et suppression son/effets parasites du dash conservés. Les raccourcis de rangement continuent de viser toutes les armes. Trois délais et choix Never indépendants : **lames Strife / pistolets Strife / épée War**. Ancien réglage personnel conservé pour pistolets et hérité par War ; lames Strife à 0 s / Never OFF par défaut.

**Ultimate decrease rate** : case facultative, OFF par défaut ; valeur proposée **×2**, curseur/saisie 0–10 et action Hotkeys ajoutée sans attribution automatique. Sous 1 raccourcit, 1 natif, au-dessus prolonge ; 0 termine au prochain tick positif de décharge native. Unlimited conserve sa priorité. Les gains et remplissages existants sont inchangés. La nouvelle interception est limitée au débit actif natif calculé, avec retour float et gardes vérifiés.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39205401503) **SUCCESS : 40/40 suites, 0 avertissement MSVC ASI/DXGI**. Source `9c10e8f819624bf03d7bcd677cdd62677a1c755d`, archive `5c649d478b165e79941f3f0e392f22190cef52dc`. [ZIP cumulatif V1.64 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/5c649d478b165e79941f3f0e392f22190cef52dc/downloads/DarksidersGenesis_V1.64_WEAPON_TIMERS_ULTIMATE_TEST.zip) : 921460 octets, SHA256 `64E708E1C24B043E468D9CBC0D47C4A9EADDD1764302E3EF556F72F094CDDDF1`. CRC, AMD64, quatre fichiers racine, marqueurs, 138 clés / 53 actions, HTTP 200, 128 anciens ZIP et 132 anciennes valeurs du profil fourni contrôlés inchangés. Les éventuels avis de dépréciation d'outils restent distincts des avertissements du code ASI.

**À tester :** choisir trois délais différents ; laisser les lames Strife à 0 s/Never OFF pour vérifier leur disparition native, puis Never séparés, OFF global et raccourcis communs en solo/P1/P2. Ultimate Unlimited OFF : 1 → 2 → 0.5, puis 0, fin de forme et retour caméra, War/Strife/P2 ; Unlimited ON doit rester prioritaire, OFF du réglage rend la décharge native. Aucun lancement ni installation par l'agent ; INI personnel non remplacé. [Périmètre et limites](V1_64_WEAPON_TIMERS_ULTIMATE_TEST.md).

**Double saut toujours ouvert et inchangé :** le test en mouvement n'a pas été capturé. Premier relevé inexploitable ; second avec 7649 lignes au sol et 124 lectures échouées en fin de session, puis processus plus disponible. Ces relevés ne démontrent aucune cause ni réparation. Auto-check V1.63 reste sans retour utilisateur. Holsters montés, autres sprites P2, modèles alternatifs et refonte Action Recovery encore en file. Notes de préparation ci-dessous historiques.

---

## V1.64 en préparation — 19 octobre 2026

**Grappin V1.63 validé par l'utilisateur dans son essai.** Nouvelle demande de rangement : conserver tous les raccourcis et séparer lames de Strife, pistolets de Strife et épée de War ; lames natives par défaut. Code et migration préparés, avec réglage de durée ultime 0–10, ×2 proposé, OFF par défaut. **CONSTRUCTION À EFFECTUER / EXPÉRIMENTAL**, aucun résultat en jeu revendiqué. Double saut encore ouvert : la séquence n'a pas été capturée ; pas de changement de son code. Aucun lancement/installation ni écriture de l'INI utilisateur par l'agent. [Périmètre, preuves et tests V1.64](V1_64_WEAPON_TIMERS_ULTIMATE_TEST.md). Les instructions d'attente et états antérieurs ci-dessous sont supersédés par le retour reçu et la demande de continuer.

---

## Retour utilisateur du 19 octobre — attente des essais V1.63

**VALIDÉS dans les essais rapportés :** raccourcis de rangement, netteté TPS, Standard limité et disparition du son/des effets du tir pendant le dash. **NON VALIDÉ : double saut** ; seul Always on autorise trois sauts supplémentaires, puis bloque. Grappin V1.63 et auto-check : retour encore attendu. La liste a été préparée avant le dernier message V1.63 ; aucune validation de cette build n'est déduite.

**À la file :** Ultimate decrease rate 0–10, durée réduite sous 1, native à 1, allongée au-dessus, **×2 par défaut** ; distinct du gain de jauge. **Attendre le retour V1.63 avant toute nouvelle modification de code/build.** [Retour exact et consigne de reprise](USER_FEEDBACK_2026_10_19.md). Les états antérieurs ci-dessous sont historiques lorsqu'ils diffèrent de ce retour.

---

## V1.63 — construite et contrôlée, crash grappin corrigé dans le code, essai en jeu requis

**CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL — pas encore VALIDÉ en jeu par l'utilisateur.** Base cumulative V1.62 ; correction des quatre arguments de l'initialisation du grappin (OFF compris) et de l'identification du propriétaire entrant. Auto check update facultatif dans Updates, OFF par défaut, canal choisi au démarrage, notification passive de 8 secondes visibles et rappel jaune au-dessus des onglets. Aucun nouveau hook ni changement des commandes/tirs/dash/caméras validés.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39174543962) **SUCCESS : 39/39 suites, 0 avertissement MSVC ASI/DXGI**. Les avertissements d'outils éventuels sont distincts du code compilé. Source `9c8d51d618f48ef0e3f1d9a2ef8a8578d71ba054`, archive `9d1fbbe737a6f872fd556ab44b23fd0eb6778eaa`. [ZIP cumulatif V1.63 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9d1fbbe737a6f872fd556ab44b23fd0eb6778eaa/downloads/DarksidersGenesis_V1.63_GRAPPLE_INIT_UPDATE_TEST.zip) : 918086 octets, SHA256 `6BE3D04FAD6469A8152F0F144EC040BAFBDA03229EEA41BA292D5A6601F02991`. CRC, AMD64, quatre fichiers racine ASI/INI/DXGI/README, marqueurs du correctif et de l'auto-check, 132 clés/52 actions, HTTP 200 et 127 anciens ZIP inchangés contrôlés.

**Priorité d'essai :** sur V1.63, grappin OFF puis ON ×2, viser/lancer/annuler avec Strife et War, puis P2. Ne plus utiliser V1.62 comme référence sûre pour le grappin : son interception erronée est active même avec la portée OFF. Les tests de transmission et le dump expliquent le défaut ; ils ne prouvent pas l'absence de tout autre crash en jeu. Une seule build candidate fournie, archives antérieures conservées. Aucun lancement du jeu, installation ou changement de l'INI personnel par l'agent.

**Essais restants depuis V1.60 :** raccourcis clavier/manette de rangement ; focus du chapitre 8 à pied/cheval/ultime ; Standard limité/remplissage/réduction ; double saut trois modes ; son/flash/animation Standard pendant déplacement+visée+tir+dash ; auto-check et notice. [Matrice cumulative](TESTS_DEPUIS_V1_60.md). Le reste de la file demeure ouvert : holsters montés, autres sprites P2, modèles alternatifs et refonte Action Recovery. Notes de préparation ci-dessous conservées comme historique.

---

## V1.63 — correctif du crash grappin et notifications de mise à jour (19 octobre 2026)

**SOURCE EXPÉRIMENTALE — construction et contrôles Windows à effectuer.** Base cumulative V1.62 `550df8a3ed697f9a696b57503e5dc1c4cf49440d`. Le rapport utilisateur invalide la sûreté du chemin grappin V1.62, y compris option OFF. Ne plus présenter OFF comme un contournement de ce crash ; V1.61/V1.60 retirent ces interceptions. Toutes les archives sont conservées pour l'historique, sans recommandation de rejouer le crash.

**Objectif et réussite :** préserver les quatre arguments de l'initialisation native du grappin, avec portée OFF/native puis ON étendue sans crash ; ajouter un contrôle facultatif au démarrage, une notification passive temporaire et un rappel jaune limité au canal sélectionné. Critères en jeu non encore satisfaits par les seuls tests de code.

**Cause démontrée au niveau du code et cohérente avec le dump :** les deux wrappers V1.62 déclaraient un seul argument. La fonction de base `5A3510` consomme RCX=capacité, RDX=propriétaire, R8=source, R9=liste empruntée de références partagées. Le dump fourni échoue à `58AB85`, `inc dword ptr [rcx+8]`, lors de la copie de cette liste via `5A35CC → 5A371E → 674E62 → 5A4967 → 5A3C57`, après deux cadres de l'ASI. Exception `C0000005`, écriture vers une adresse du module jeu, durée 62 secondes. Le journal du mod actuel provient d'une relance postérieure et n'est pas assimilé à celui du crash. ASI installée identique au paquet V1.62 par SHA256 ; module/timestamp du dump compatibles. Aucun dump, chemin personnel ni identifiant de compte publié.

**Correction :** les deux wrappers et leur trampoline transmettent tous les arguments inchangés, ON/OFF et hors scope. Le propriétaire utilisé pour filtrer l'option vient du paramètre entrant : les champs natifs de propriétaire ne sont remplis qu'au cours de l'initialisation. Une garde supplémentaire vérifie le prologue de `5A3510`. Les trois distances restent groupées sous un seul facteur, seules les valeurs de portée sont temporairement ajustées/restaurées. Aucun nouveau hook ajouté. Le défaut venait du contrat d'appel, pas d'une valeur de pistolet confondue avec le grappin.

**Auto check update :** case dans Updates, OFF par défaut pour conserver les anciennes préférences ; clé `[Updater] AutoCheckUpdates=0`. Si ON, une seule vérification GitHub au lancement sur le canal configuré, hors du rendu ; l'activation de la case vérifie aussi immédiatement. Le service existant refuse une deuxième opération concurrente. Un résultat strictement plus récent affiche une notification de 8 secondes visibles, même menu fermé, puis un rappel jaune au-dessus des onglets. Le temps hors focus n'épuise pas la notification ; elle ne prend ni focus ni input, ne force pas le curseur, et aucun frame ImGui supplémentaire n'est maintenu après son expiration quand le menu est fermé. Résultat courant, antérieur ou échec réseau : aucun avis de mise à jour. Changement de canal : résultat invalidé ; choix indisponible pendant une opération. Vérification manuelle toujours disponible ; installation/rollback inchangés et explicitement confirmés, INI conservé. L'avertissement de downgrade cite maintenant la version compilée, plus V1.14.

**Contrôles préparés :** transmission effective des quatre arguments et consommation de la liste de références par le consommateur simulé, exactement un appel par chemin ON/OFF et exclusion ; portée/restauration et propriétaire initialement absent ; défauts/INI personnel/reload/reset ; durée et Alt-Tab ; vrais flags de fenêtre Dear ImGui sans input/focus. Les tests V1.62 utilisaient la même signature erronée que le wrapper ; ils ne couvraient pas ce contrat et sont corrigés. Gardes comparées au binaire exact. 132 clés actives / 52 actions ; une seule nouvelle clé, aucun raccourci assigné. 127 anciens ZIP contrôlés identiques avant construction. Onze sources des fonctions validées et des autres ajouts V1.62 comparées et conservées.

**À essayer dans V1.63, après construction :** grappin OFF et facteur 1, visée puis lancement/annulation, Strife/War, puis ON ×2, cible/traction/relais, P2. Arrêter au premier crash et garder le rapport ; traversées/tyroliennes restent non démontrées. Auto-check OFF puis ON au prochain lancement, canal courant/antérieur et erreur réseau ; une future version plus récente doit donner toast puis bannière, sans prise de contrôle. La fixture teste cet avis sans ajouter un faux ZIP public. Vérifier aussi les essais cumulatifs ci-dessous.

**Reste ouvert :** raccourcis V1.60, focus V1.61, Standard limité et sauts V1.62 encore à valider par l'utilisateur ; présentation des tirs pendant dash, holsters montés, autres sprites P2, modèles alternatifs et refonte Action Recovery. Aucun lancement/installation ni écriture de l'INI personnel par l'agent pour cette préparation.

[Liste cumulative des essais depuis V1.60](TESTS_DEPUIS_V1_60.md).

---

## V1.62 — lot construit et contrôlé, disponible sur debug (19 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL — nouvelles fonctions non validées en jeu.** Base cumulative V1.61, acquis précédemment validés conservés. Lot de trois ajouts : **Standard limité** (capacité 1–10000, 200 par défaut), **portée du grappin** (acquisition/relais/traction ensemble) et **double saut Off / After gliding / Always on**. Les trois ajouts sont **OFF par défaut**, avec actions Hotkeys ajoutées sans renuméroter les anciennes. After gliding = un double saut réarmé par chaque nouveau plané, pas des sauts répétés sans replaner. Le grappin applique le réglage à la prochaine initialisation de l’action ; traversées/tyroliennes à vérifier.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39173208014) **SUCCESS : 38/38 suites, zéro avertissement MSVC ASI/DXGI**. Avis de dépréciation CMake/Node des outils conservés, sans les présenter comme des avertissements du code ASI. Source construite `b5adc3b24929cebbcb74531518055d7eaf1ed946`, commit d’archive `ade9270a83d516dea999e159ada6c73ab73152ae`. [ZIP cumulatif V1.62 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/ade9270a83d516dea999e159ada6c73ab73152ae/downloads/DarksidersGenesis_V1.62_NATIVE_GAMEPLAY_BATCH_TEST.zip) : **914339 octets**, SHA256 `69B509F2AA1A034EFAA5231F501CFFB67BA0B484FA9604BF36112C2276BE2295`. CRC, quatre fichiers racine, architecture AMD64, marqueurs, 131 clés/52 actions, téléchargement public HTTP 200 et **126 anciens ZIP inchangés** contrôlés. Les 126 anciennes valeurs du profil fourni sont inchangées ; 12 sources critiques de visée/caméra/dash/roues/cheval/rangement/HUD/focus comparées à la base et identiques.

Disponible dans l’updater **debug** à sa prochaine vérification. **Aucune installation par l’agent**, aucun lancement du jeu, aucune écriture de l’INI personnel. Le jeu encore ouvert charge la V1.60 constatée par hash ; aucune installation V1.61/V1.62 n’est revendiquée. La capture externe du grappin a terminé ses 3813 échantillons sans erreur et fermé son handle VM_READ. Les contrôles de construction ne prouvent pas la portée ni le saut ni les visuels en jeu.

**Essai commun :** Standard capacité 10 avec réduction/Unlimited OFF → épuisement et refus à zéro → Fill → ÷2/Unlimited → OFF ; grappin ×1 puis ×2, reprendre sa visée entre réglages, cible lointaine/traction/relais/annulation et P2 ; double saut Off, Always avec quatre sauts, After gliding avec un seul saut réarmé par chaque nouveau plané puis atterrissage ; vérifier chaque joueur, changement de personnage et caméra native/TPS. Contrôles courts du tir, roues et rangement validés, puis focus V1.61 ON/OFF aux angles flous. [Matrice détaillée et limites](V1_62_NATIVE_GAMEPLAY_BATCH_TEST.md).

**Suite ouverte :** résultat du filtre de présentation du dash, focus V1.61 et raccourcis V1.60 à confirmer ; holsters montés, autres sprites P2, modèles compatibles et refonte d’Action Recovery encore non livrés. Rollback : trois options OFF pour le comportement natif, ou archives V1.61/V1.60 conservées pour retirer les nouvelles interceptions, INI personnel conservé. Le reste de la file n’est pas déclaré réparé. Détails de préparation datés ci-dessous ; ce reçu fait foi pour la construction/livraison.

---

## V1.62 — lot gameplay natif (19 octobre 2026)

**SOURCE EXPÉRIMENTALE — compilation et validation en jeu à effectuer.** Base cumulative V1.61 `7aad312d3fbde4330360493a21607cd3cab309b4`. Acquis validés V1.58/V1.59 conservés : prompts P1/P2, valeurs sous 1, Zoom In/Clear, rangement natif ; reprise AIM V1.56 conservée. Focus V1.61, raccourcis V1.60 et filtre de présentation du dash restent à valider, sans nouveau changement de ces chemins.

**Objectif du lot :** ajouter ensemble trois contrôles de gameplay demandés en réutilisant les fonctions natives, avec OFF natif et actions Hotkeys, sans modifier les commandes de saut, les sticks, les tirs ou la physique. Réussite : réserve Standard finie et rechargeable qui bloque à zéro ; acquisition/relais/traction du grappin étendus ensemble ; double saut Off / après chaque plané / toujours, avec conditions d’action natives et isolation P1/P2. Un lot commun de vérifications remplace les petites builds successives.

- **Limited Standard ammunition**, OFF par défaut. Capacité exacte 1–10000 (200 par défaut), curseur/saisie/Default. Retour du maximum natif remplacé seulement pour les proxies Standard possédés par les joueurs locaux, initialement infinis. Les champs de capacité, les CDO et tables de données ne sont jamais écrits. Réserve initialement remplie à l’activation ; baisse du plafond borne le stock, hausse sans remplissage gratuit. OFF rend immédiatement le maximum infini et restaure le compteur à -1 lors de la prochaine observation gameplay. Réduction, Unlimited et remplissage existants partagés. Les deux réserves natives de type Standard (DualPistols et StandardShot) sont traitées individuellement ; WorldEnder/type 4 reste natif. **Hotstreak conserve son propre coût natif**, comme les autres munitions ; aucune promesse de consommation pendant cette forme.
- **Grapple range**, OFF par défaut, facteur 1–10, 2 par défaut avec saisie manuelle. Les trois valeurs natives vérifiées sont multipliées ensemble à l’initialisation des capacités locales à pied : acquisition 1800, relais 1500, distance de proxy de traction 1200 dans les Blueprints relevés. Les rayons sont copiés par le moteur dans ses finders, la distance sert au calcul initial du point de traction ; les champs de la capacité sont restaurés au retour de l’appel. Reprendre la visée/l’action après un changement : aucun recalcul d’une traction déjà lancée. Angles, classes de cibles, obstacles, vitesse/détachement et contraintes de traversée restent natifs. **L’allongement de tous les parcours/tyroliennes n’est pas démontré** ; essai nécessaire.
- **Infinite double jump**, Off par défaut ; **After gliding** donne un double saut supplémentaire après chaque nouveau plané (répéter saut→plané→saut), consommé seulement si l’action native réussit ; **Always on** permet de répéter les doubles sauts sans plané. Le retour du compteur est plafonné à 1 uniquement à sa lecture précise dans l’action de saut, lorsque le compteur réel vaut 2 et le personnage local est en chute ordinaire. Aucun compteur réel, vitesse, impulsion, animation ou input n’est écrit par le mod ; le jeu exécute encore son action et ses restrictions. Atterrissage, changement de mode/propriétaire et transformation invalident les droits du mode après plané. Ce n’est pas le retour d’Action Recovery.

Trois actions ajoutées à la fin de Hotkeys : `FiniteStandardAmmo` (49), `GrappleRange` (50), `InfiniteDoubleJump` (51, cycle Off→After gliding→Always on). Identifiants 0–48, raccourcis actuels et INI personnels conservés. Cinq nouvelles clés, **131 clés actives / 52 identifiants**. Aucun raccourci assigné automatiquement. Aucun changement des valeurs utilisateur précédentes.

**Preuves avant interception :** capture externe VM_READ de War et Strife utilisant le grappin : **3813 échantillons, zéro erreur**, propriétaires locaux et deux classes natives confirmés ; inventaire des réserves Standard/special/WorldEnder. Binaire exact inchangé. Initialisations de grappin `5A3C40`/`5A3A10`, copies du finder `6AD9D0`, consommateur d’ammo `69839B–6983C6`, getter de maximum `6A90D0`, action de saut `69FC70`, lecture `69FCB0`/retour `69FCB5` et getter `56D400` étudiés. Les six signatures ajoutées sont comparées au binaire local ; échec d’une garde laisse la fonction native. Cinq interceptions natives supplémentaires (maximum ammo, deux initialisations de grappin, action/getter de saut), aucune interception d’input.

**Contrôles préparés :** vrais wrappers de fonctions avec consommateurs natifs simulés, OFF, frontières/valeurs invalides, restauration des octets du grappin, comptage inchangé hors action, crédits de plané, refus natif, P1/P2 et identité ; profil/Hotkeys/defaults. Les fixtures ne sont pas une validation en jeu. Aucune écriture mémoire par les diagnostics, aucun lancement du jeu par l’agent, aucune installation à ce stade.

**À tester ensemble :**

1. Standard : capacité 10, réduction et Unlimited OFF, épuiser puis vérifier le refus à zéro ; Fill, réduction ÷2, Unlimited ; plafond plus petit/plus grand puis OFF. Vérifier les deux slots et la coop, sans épuiser les munitions spéciales pour ce test. Récompenses de munitions et persistance après changement de personnage/chargement restent à contrôler.
2. Grappin : facteur 1 puis 2, sortir/rentrer sa visée après modification ; cible/point précédemment hors portée, relais et traction d’ennemi. Annulation, obstacle et traversée/tyrolienne ; War, Strife et P2. OFF puis nouvelle action retrouve la portée native. Arrêter si une cible inaccessible coince le personnage.
3. Double saut : Off natif ; Always, au moins quatre sauts successifs ; After gliding, pas de troisième saut sans plané, un nouveau saut après plané, un seul avant le plané suivant. Atterrissage, mort, changement de personnage, cheval/ultime, P1/P2 et caméra native/TPS. Un appui de saut reste une action native ; aucun maintien automatique promis.
4. Contrôles courts des acquis : tirs Lava/Charge/faisceaux, reprise AIM après dash, roues et manettes P2 ; rangement et raccourcis V1.60 ; focus V1.61 ON/OFF aux angles flous, toutes formes. Le mouvement+visée+tir+dash reste un point de validation du filtre existant, pas une réparation annoncée par ce lot.

**File restante :** présentation Standard pendant dash sans retour conclusif ; flou V1.61 et raccourcis V1.60 en attente de validation ; holsters montés sans correction démontrée ; autres sprites P2 à identifier ; modèles 3D sans alternative compatible démontrée ; nouvelle conception Action Recovery à repartir d’un verrou natif précis. Aucun de ces sujets n’est déclaré clos. Rollback : options nouvelles OFF ; pour retirer aussi leurs interceptions, archive V1.61 ou V1.60 conservée, avec INI personnel.

Références privées conservées dans `audit-v131/v162-live-gameplay.json`, `v162-live-grapple-actions.jsonl`, `v162-grapple-methods.txt`, `v162-native-names.txt`, `v162-jump-fields.txt`, `v162-source-checks.json`. Aucun asset du jeu distribué.

---

## V1.61 — construite et contrôlée, mise au point TPS à tester (19 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL — essai visuel à effectuer.** Base cumulative V1.60 `1d746e9`, installée par l’utilisateur via updater et contrôlée. Clear et rangement V1.59 validés ; nouveaux raccourcis V1.60 et tir pendant dash sans nouvelle validation utilisateur. Cette build est limitée au flou signalé sous certains angles en TPS à pied, cheval et ultime.

**Objectif / réussite :** garder le personnage net avec la caméra TPS proche, tout en conservant un véritable effet de profondeur de champ. Nouveau « TPS lens focus (experimental) » dans Camera > TPS, ON par défaut (`Features/TPSLensFocus=1`). OFF conserve exactement la correction de distance V1.60 et le mode de flou du niveau, sans changer angle, FOV, distance ni visée. La différence d’apparence et de coût du rendu reste à mesurer en jeu.

**Preuves / correction de l’audit précédent :** capture externe VM_READ, 1470 échantillons, plusieurs angles stables. Distance de focus ≈ profondeur du pivot (par exemple 329.06515 contre 329.06514) ; le modèle couvre une plage plus large, variable avec l’angle, par exemple 144.73–497.44. Cela écarte une simple absence de notre correction de distance sur ces vues, sans prouver seul le résultat GPU final. Le volume chargé `LVL09_PostProcessVolume_Dungeon` a méthode 1 Gaussian, near/far blur 0.6, foyer 1000, région 3400 et poids 1 ; borné et activé.

L’ancienne conclusion « bits d’override de foyer/région désactivés » était **fausse**. Les octets bruts sont +0x16=0F et +0x17=FF : masks 08 et 04 actifs ; méthode +0x18=07, mask 02 actif aussi. Ce nouveau décodage corrige les conclusions historiques V1.58, sans réécrire leur état daté. Le premier inventaire au changement de monde était prématuré et filtrait le monde racine ; une seconde lecture après streaming trouve le volume de `Level_09_Base_Dungeon`. Les réglages stockés d’une caméra ne suffisent toujours pas à certifier le mélange final.

**Changement ciblé :** dans la sortie de vue TPS déjà possédée, la candidate ajoute le mode natif CircleDOF=2 et son bit d’override +0x18 mask 02, avec la distance déjà calculée. Le niveau peut ainsi garder ses réglages natifs dans les vues d’origine ; la vue TPS utilise une lentille focalisée sur le personnage. Aucune désactivation globale du DOF, aucun patch de volume/UObject, aucun nouveau hook. Qualité, ouverture, sensor width, paramètres atmosphériques, régions et tailles stockées conservés. Les valeurs de régions peuvent être ignorées par le mode circulaire : le moteur les remet lui-même à zéro dans son rendu pour méthode 2 ; ce n’est pas un effacement par le mod. Effet, intensité et performance visuels restent à valider.

**Garde native :** propriété Method offset 23, table enum `2EBD618=2`, setter `74CEB0`, consommateur du mélange `1B30380` et branche de rendu `1B22A3B` vérifiés avant activation. Le bit 04 voisin n’est pas le bit de méthode : le test reproduit ce piège et vérifie 02. Si une garde manque, chemin V1.60 conservé. Même politique P1/P2, cheval/ultime et maintien de vue ; les règles existantes de propriétaire, poids 1 et sortie gameplay restent requises. Une vue native fraîche reste intacte.

**Contrôles effectués :** 34 suites Windows, nouveaux cas du vrai helper de focus ON/OFF, bits de rendu, mode inconnu, poids partiel, isolation P1/P2, préservation octet par octet hors focus/méthode, profil et publication runtime. 126 clés, 49 actions, 125 anciens ZIP contrôlés inchangés. Sources de tir/dash/inputs/ressources/rangement/HUD inchangées. Construction et fixtures ne valident pas l’image du jeu.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39170957103) **SUCCESS : 34/34 suites, zéro avertissement MSVC ASI/DXGI**. Avis de dépréciation des outils conservés. Source construite `daedcb9a62ed114da089c9304ec6e4254debc91a`, archive immuable `39d04389e23e148d0839186f597c4a9e99f53bec`. [ZIP cumulatif V1.61 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/39d04389e23e148d0839186f597c4a9e99f53bec/downloads/DarksidersGenesis_V1.61_TPS_LENS_FOCUS_TEST.zip) : 905379 octets, SHA256 `4AC4C242453832121E9F38295A55D60A1FF541FF75D0C5F4DFC413C55DC3335B`. CRC, quatre fichiers racine, AMD64, marqueurs, 126 clés/49 actions, 125 anciens ZIP inchangés et téléchargement public HTTP 200 contrôlés. Aucun test automatisé ne valide le rendu en jeu. Disponible sur le canal debug de l’updater ; V1.60 reste la dernière installation effectivement constatée à ce reçu. Aucun remplacement forcé des fichiers chargés, aucun INI modifié par l’agent.

**À tester :** même endroit/angles du chapitre 8 avec TPS lens focus ON puis OFF, à pied, cheval et ultime ; autre chapitre et coop ; vue native temporaire et vraie cinématique gardent leur effet ; regard sur la netteté du personnage, le flou d’arrière-plan et la fluidité. Raccourcis V1.60 à contrôler séparément. Rollback : V1.60 ou OFF pour revenir à sa politique de focus. Aucun autre sujet de la file n’est déclaré corrigé.

Références techniques : [DOF UE4, Epic](https://dev.epicgames.com/documentation/en-us/unreal-engine/1.11---depth-of-field?application_version=4.27). Les noms/layouts de cette version du jeu sont vérifiés dans son binaire ; la documentation générale ne prouve pas sa version ni son rendu final. Captures privées : v161-angle-summary.json, v161-live-postprocess-values.json, v161-dof-enum-refs.txt et v161-final-dof-normalization.txt ; aucun asset privé livré.

---

## V1.60 — construite, contrôlée et installée via updater (18 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — nouveaux raccourcis à valider en jeu.** Base cumulative V1.59 `099bd1ec1a90c54b7a143a586fdf80a1079fa96a`. Retour utilisateur : **Zoom In/Clear et rangement des armes VALIDÉS**. Le résultat du filtre de tir pendant dash n’a pas encore été confirmé ; ce code est conservé sans changement.

**Objectif / réussite :** remplacer le bouton « Sheathe weapons now » par des raccourcis directement dans la section Weapon sheathing, en complément des Hotkeys F1–F12. Un raccourci clavier reconfigurable (Change/Clear/Default) et deux emplacements manette : deux OFF désactivent, un OFF laisse l’autre bouton utilisable seul, deux boutons actifs forment une combinaison. Les gâchettes LT/RT sont proposées. Tout est OFF par défaut ; anciens INI, raccourcis et actions conservés. Aucune touche physique imposée.

Clavier et Hotkeys gardent la portée existante sur les joueurs locaux admissibles. Les demandes simultanées de rangement sont regroupées une seule fois par joueur avant la mise en file native, y compris une même F-touche assignée à la fois dans le menu et Hotkeys. Chaque manette est associée au ControllerId local publié et ne commande que son joueur ; propriétaire ambigu, périphérique déconnecté, identité expirée ou changement de personnage restent sans action. Les lectures physiques utilisent le chemin existant non filtré, sans changer les boutons/sticks transmis au jeu. Le jeu conserve ses raccourcis natifs : choisir une combinaison libre pour éviter de lancer aussi une action native.

Appui unique, aucune répétition tant que le raccourci est maintenu. Configuration, branchement ou changement de joueur pendant un appui ne créent pas d’action ; appuis dans l’overlay ou hors focus consommés. L’action continue d’attendre au plus cinq secondes l’état natif inactif autorisé. Délais automatiques, jamais automatique, fonctions natives Clear, restrictions pied/cheval/Ultimate et sécurité d’action V1.59 préservés. Aucun nouveau hook, aucune rotation/visée/animation modifiée ; aucune ancienne option validée réinitialisée.

**Contrôles effectués :** 34 suites Windows, dont nouveau test de combinaisons/OFF/appui/focus/identités P1/P2, fixture réelle du rangement avec ciblage P2 et requêtes en double, aller-retour INI/migration et vrai Dear ImGui pour les quinze lignes clavier. 125 clés actives, 49 identifiants d’actions ; 124 ZIP précédents contrôlés inchangés. Ces contrôles ne constituent pas une validation en jeu. Rollback : V1.59 en conservant l’INI personnel.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39155920524) **SUCCESS : 34/34 suites, zéro avertissement MSVC ASI/DXGI** ; avis de dépréciation des outils conservés. Source construite `8f8d4c9828f5baa35334c281cceeb76d34b9bf6d`, archive immuable `a2553fce1c81e0f00f600947420f50cc1b40c9b1`. [ZIP cumulatif V1.60 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/a2553fce1c81e0f00f600947420f50cc1b40c9b1/downloads/DarksidersGenesis_V1.60_SHEATH_SHORTCUTS_TEST.zip) : 902453 octets, SHA256 `7D2500EE0FBDB3CDBBCB5461ED36FAB838EDED284078343EFB86D7694E6E358E`. CRC, quatre fichiers racine, AMD64, métadonnées, 125 clés INI/49 actions, 124 anciens ZIP inchangés et téléchargement HTTP 200 contrôlés. Installation par l’utilisateur via updater contrôlée ; sauvegarde V1.59 vérifiée. Aucun lancement automatique ni validation en jeu des nouveaux raccourcis.

Installation effectuée par l’utilisateur via l’updater intégré, puis contrôlée en lecture seule : ASI/DXGI et README identiques au ZIP V1.60 vérifié. Sauvegarde V1.59 de l’updater `20261018-212312-503-67c688` et tous les fichiers de son manifeste contrôlés par hash. Aucune réinstallation ni écriture dans le jeu par l’agent. INI personnel actuel relevé sans modification, SHA256 `1B92F88B5C0E26D408E2619D43BC395126F9A51931A915634FC28DB4C43F8BB2` ; aucune prétention de comparaison avant/après l’installation faite par l’utilisateur. Aucun lancement du jeu par l’agent. Les nouveaux raccourcis restent à valider en jeu. Le README installé est celui du paquet immuable ; le dépôt porte ce reçu actualisé.

**À tester :** (1) clavier assigné directement dans Weapon sheathing, Clear puis Reload ; (2) manette avec entrée 1 seule, entrée 2 seule, puis deux boutons ; (3) maintenir, relâcher et réappuyer ; (4) même F-touche via menu et Hotkeys, un seul rangement ; (5) chaque manette n’agit que sur son joueur en coop ; (6) menu ouvert/Alt-Tab/rebranchement avec bouton tenu, aucun rangement différé ; contrôle court du délai automatique/jamais automatique déjà validé. Le critère de réussite reste à confirmer par l’utilisateur.

**Suite distincte :** flou, holsters montés, autres sprites P2, Standard finies, portée du grappin et modèles alternatifs restent ouverts. Cette build ne prétend pas les inclure. La présente demande de raccourcis est isolée pour préserver les fonctions validées.

---

## V1.59 — construite, contrôlée et installée (18 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation en jeu nécessaire.** Base V1.58 `ca74560cdfa5201a9d67c5222afb0d980b39aa88`, avec Hotstreak sous 1 et prompts P1/P2 validés par l’utilisateur et conservés. La candidate corrige le conflit d’identifiants ImGui en-tête/ligne, retire la liste des armes du filtre de notifications de dash et ajoute le rangement natif demandé : délai 0–300 s ou jamais automatique, bouton et action « Sheathe weapons » assignable dans Hotkeys. **Contrôle automatique OFF par défaut** ; aucune touche de l’utilisateur modifiée.

**Action Recovery reste supprimé**, sans retour de sa fin anticipée du dash. La reprise de visée et le filtrage des projectiles/sons/effets imposés par les animations sont indépendants de cette ancienne expérience. Le filtre reçoit maintenant toute arme native valide, réciproquement possédée et inscrite dans les slots de tir 2/3 ; plus d’ajout cas par cas Beam/Lava/Charge. Le cas Standard, les demandes mixtes et les objets non typés conservent leur chemin. Aucun nouveau hook, aucune modification de la physique/fin du dash, de la balistique, des axes ou du cycle d’attaque. Son résultat en jeu reste à vérifier ; le mouvement corporel du montage reste natif.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39152805753) **SUCCESS : 33/33 suites, zéro avertissement MSVC ASI/DXGI**. Test réel du Dear ImGui épinglé, vrai service de rangement simulé/configuration et régressions des notifications avec Beam et une sous-classe native non énumérée. Source construite `f066899b6148c5c787ab25f01f26b98b86629cb7` ; archive immuable `7d5dbbe4ca87de3b6febc01c73288f3b41c96dfa`. Les avis de dépréciation des outils restent visibles. Aucun test automatisé ne constitue un essai dans le jeu.

[ZIP cumulatif V1.59 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/7d5dbbe4ca87de3b6febc01c73288f3b41c96dfa/downloads/DarksidersGenesis_V1.59_BINDINGS_NATIVE_SHEATH_TEST.zip) : **898,472 octets**, SHA256 `82F2185050C098FAAE1B0A42A4371455390D2AFE38852965D0B1310FA57CCD52`. CRC, quatre fichiers racine, AMD64, version, 122 clés INI et 49 identifiants d’actions contrôlés ; téléchargement public HTTP 200 et **123 anciens ZIP inchangés** vérifiés. La nouvelle action 48 est ajoutée sans renuméroter les précédentes. INI personnel conservé à l’installation ; rollback V1.58.

**Préservé :** code HUD et facteurs V1.58 validés, reprise AIM, tirs/cible/caméra/mise au point, XInput/RawInput, roues, ressources et comportements hors périmètre. L’option de rangement n’écrit que les deux seuils natifs du personnage normal ; compteurs, flags et meshes restent natifs. L’action manuelle appelle les deux fonctions Clear natives seulement en état inactif autorisé ; pied vivant, hors pause/cinématique, sans AIM/tir/capacité active. Cheval/Ultimate restent natifs. Strife observé : 4 s ; War Blueprint : 5 s, essai War/P2 à effectuer.

**File ouverte :** flou chapitre 8/transformations, autres sprites partagés P2, Standard finies, portée du grappin, holsters montés et modèles 3D alternatifs. Les quatre meshes main/rangés ont été distingués des textures et matériaux ; aucune nouvelle géométrie compatible n’est démontrée. Le relevé monté voit les composants du cheval : pistolets en main masqués, holsters visibles sur Bone_Strife_Holster_R/L, transforms relatifs nuls et meshes Strife_Gun_Mesh_R/L. Cela exclut une simple double visibilité locale des composants montés, sans démontrer encore la bonne pose ni le modèle attendu. Les composants du personnage normal sont distincts ; leurs seuls flags locaux ne prouvent pas leur rendu effectif. Aucun correctif spéculatif ni asset privé n’est livré pour ces sujets.

Installation après fermeture du jeu et contrôle de 278 processus visibles. Sauvegarde V1.58 `install-backup-before-v159-20261018-201943`, manifeste d’intégrité et `Restore-Previous.ps1` conservés ; syntaxe de restauration contrôlée, script non exécuté. **INI personnel inchangé octet pour octet** : `D99169453FEB41296867AA66868480D5730C3FAC5AB36648F1E993E704582487` ; raccourcis conservés. ASI/DXGI installés identiques au paquet vérifié. Aucun lancement du jeu par l’agent, aucune validation de gameplay. README installé actualisé avec ce reçu ; ZIP immuable.

**À tester :** Clear Zoom In TPS actif puis Reload ; tir + déplacement + visée + dash avec Standard/Beam/Charge/autre munition, normal/Hotstreak et P1/P2 ; visée après dash et roues ; rangement OFF, délai 0.5/2/10 s, jamais automatique et action manuelle via Hotkeys sur Strife/War, y compris retour de cheval/Ultimate. Contrôle court HUD P2 et Hotstreak 0.5. [Audit et limites](V1_59_BINDINGS_NATIVE_SHEATH_TEST.md). Les statuts de préparation ci-dessous sont historiques.


## Suite de la file après installation V1.59

L’utilisateur précise attendre une version regroupant les correctifs et futures fonctions. La V1.59 installée est une étape intermédiaire, pas la totalité de cette file. Aucun des sujets ci-dessous n’est déclaré corrigé par la construction actuelle. Les essais V1.59 restent nécessaires avant de retenir ses expériences dans une nouvelle base.

Grappin : analyse hors jeu des Blueprints natifs et des structs imbriqués, parsing complet des propriétés de quinze assets GhostHook. La visée générique contient SearchRadius=1800 pour TargetingProperties, 1500 pour LinkedBeamTargetingProperties, avec angles distincts. La traction a ProxyTargetMaxDistance=1200, ProxyTargetDetachDistance=50 et ProxyTargetReelSpeed=8000. Métadonnées natives : LinkedBeamTargetingProperties à +220 et ProxyTargetMaxDistance à +240 dans leurs classes respectives ; ne pas appliquer ces offsets à une classe inconnue. Ces limites ont des fonctions différentes ; aucun multiplicateur global ou changement d’angle/vitesse n’est livré. Prochaine preuve : consommateurs natifs, classes/propriétaires effectivement actifs et retour OFF avant d’exposer une option de portée. Références privées : v160-ghost-defaults.json, v160-ghost-targeting.json, v160-ghost-property-metadata.json.

Munitions normales finies : le getter natif 6A90D0 calcule la capacité depuis le proxy +350 et le facteur du propriétaire +46C, avec minimum -1. Le setter autoritaire 697E70 limite le compte courant +34C à ce maximum et conserve le sentinelle -1. Cela confirme qu’une jauge affichée seule ne suffirait pas. Il reste à identifier sans ambiguïté le proxy Standard, son initialisation, ses consommateurs de coût et la restauration native OFF ; aucune écriture de capacité ni nouvelle réserve n’est livrée. Les réductions/remplissages validés restent inchangés. Références privées : v160-native-ammo-max.txt, v160-native-ammo-set.txt.

Holsters : le relevé en jeu et les modèles prescrits par le Blueprint Despair concordent ; pose finale/os/sockets et apparence encore à comparer. Modèles alternatifs : quatre géométries main/rangées distinguées des textures, sans nouvel asset alternatif compatible retenu. Flou et autres sprites P2 : toujours ouverts ; aucun effet global supprimé ni parent masqué.
---

## V1.59 — raccourcis, notifications de dash et rangement natif (18 octobre 2026)

**SOURCE DE TEST / EXPÉRIMENTAL — construction et validation en jeu à effectuer.** Base cumulative V1.58 `ca74560cdfa5201a9d67c5222afb0d980b39aa88`, retenue partiellement : l’utilisateur valide Hotstreak sous 1 et les prompts d’interaction P1/P2. Clear de Zoom In rejeté avec capture d’un conflit ImGui. Le retour sur le test 4 est précisé : tirs Standard pendant le dash avec une autre munition équipée, pas une nouvelle régression démontrée de la reprise AIM.

**Objectif / réussite :** Clear indépendant de l’en-tête et persistant après Reload ; supprimer la liste de noms de munitions du filtre existant sans modifier timing, visée ou tirs hors dash ; fournir le contrôle natif de rangement demandé. Rangement automatique modifié OFF par défaut, action manuelle assignable dans Hotkeys. Critère en jeu : délai choisi/jamais automatique et rangement manuel en état inactif, sans interrompre visée/attaque, P1/P2 et Strife/War à tester.

Le conflit vient de `TableHeadersRow` qui pousse l’index de colonne : l’en-tête Clear (3) et la ligne Zoom In (3) utilisaient le même identifiant. Une portée nommée commune distingue les lignes de tous les tableaux clavier. Nouveau test avec le vrai Dear ImGui épinglé, qui reproduit l’ancienne collision puis vérifie les identifiants des en-têtes et boutons ; aucun avertissement masqué.

**Action Recovery reste retiré.** Le filtre des notifications de tir de dash est un correctif distinct. La capture en lecture seule voit trois Dash Fire Level1, visée/tir natifs tenus et physique/cleanup attendus. L’arme principale est Beam Shot, absente de l’ancienne liste. La candidate retire cette liste : seuls les emplacements natifs de tir 2/3, le type MayhemWeapon, l’inscription courante et le propriétaire réciproque définissent une arme équipée admissible. Standard conserve son cas explicite ; demandes mixtes/objets non typés, autres emplacements, propriétaire/vue inconnus restent natifs. Le même prédicat alimente les filtres existants de projectile, son et flash/douilles ; pas de nouveau hook, animation/fin/physique/input inchangés. **Correction encore à valider**, sans promettre un tir spécial continu pendant le dash ni un changement du mouvement corporel du montage.

**Rangement des armes :** seuils natifs CombatIdleDurationSec `CD0` et RangedIdleDurationSec `CD4` identifiés par métadonnées et consommateur natif `68D923–68D9C3`. ClearCombatIdle `663AC0`, ClearRangedIdle `663DC0`, slots `BB8/BD0` de la vtable native `2707940`, vérifiés en lecture seule sur Strife. Blueprint War : délai combat 5 s ; Strife : délai ranged 4 s, transition à environ 4.05 s confirmée par la capture. OFF restaure les deux valeurs propres au personnage. ON propose jamais automatique (seuil float maximal, seulement comparé par le timer natif) ou délai 0–300 s, saisie manuelle et fractions. Les compteurs écoulés ne sont jamais écrits ; animations, meshes et états d’attaque restent natifs. Mode pied seulement ; cheval/Ultimate restaurent les seuils du personnage normal. Cache de référence par identité UObject, aucune éviction d’objet vivant modifié. Aucun hook supplémentaire : service natif de joueur existant. L’action manuelle attend jusqu’à 5 s un état vivant, à pied, hors pause/cinématique, sans capacité active ni AIM/tir natif tenu, puis appelle les deux fonctions natives une fois. Anciennes actions conservées, nouvelle action 48, aucune touche imposée.

**Préservé :** HUD et facteurs V1.58 validés, reprise AIM V1.56, balistique/cible, caméra coop/mise au point, XInput/RawInput, roues, ressources, sauvegardes et raccourcis. 122 clés INI actives et 49 identifiants d’actions ; 123 anciennes archives à conserver. 33 suites prévues : UI réelle, vraie fonction de rangement simulée/configuration et régressions du filtre sans liste d’armes. Construction/fixtures ne valident pas le jeu.

**File d’attente poursuivie :** index PAK vérifié, 29 références privées extraites pour War, cheval, Standard et GhostHook/modèles. Quatre meshes Strife trouvés : deux en main et deux rangés ; les textures/matériaux ne sont pas des modèles alternatifs. L’asset Standard référence sa capacité de visée sans réserve finie explicite. Le composant GhostHook ne fournit pas à lui seul une portée unique. Grappin, Standard finies, holsters montés et choix de modèles restent ouverts jusqu’à preuve de leur chaîne native ; aucune fausse option ni modification d’asset livrée. Flou chapitre 8, notamment transformations, toujours ouvert. Prochain relevé : attachements du cavalier et propriétaire des pistolets.

**À tester :** (1) Clear Zoom In avec TPS actif puis Reload ; (2) Standard/Beam/Charge et une autre munition, déplacement + visée + tir + dash, normal/Hotstreak et P1/P2 ; (3) contrôle court de visée après dash et roues ; (4) rangement OFF puis délai 0.5/2/10 s et jamais automatique, action manuelle via Hotkeys, Strife/War, pause/attaque/cheval/Ultimate ; (5) HUD P2 et Hotstreak 0.5 restent bons. En cas de régression, revenir à la sauvegarde V1.58 en conservant l’INI personnel.

---

## V1.58 — construite, contrôlée et installée (18 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation en jeu à effectuer.** Base cumulative V1.57 `1eb7d24b7496bdc9b6be19972da16e2431d4013b`, avec reprise de visée après dash V1.56 validée par l’utilisateur et conservée. Facteurs sous 1 réellement appliqués pour ammo/Hotstreak/potions ; taille du prompt d’interaction réglable (0.65 par défaut) et propriétaire caméra P2 corrigé dans la fonction native ; suppression de l’expérience Action Recovery de fin anticipée. Les raccourcis de caméra native, dont Zoom In/Clear, deviennent éditables même en TPS ; ils s’exécutent toujours uniquement en vue native.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39149165341) **SUCCESS : 31/31 suites, zéro avertissement MSVC ASI/DXGI**. Source construite `29a584316e83c02d37f3470534b5f7da1acdc14a`, archive immuable `fa71b5a125a0cb9de5e88470af25743d065bc49f`. Le premier passage a correctement bloqué l’emballage sur une différence de format INI `0.65`/`0.650` ; l’INI livré a été corrigé pour correspondre aux valeurs écrites par le mod, sans assouplir le contrôle. Avis de dépréciation des outils conservés.

[ZIP cumulatif V1.58 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/fa71b5a125a0cb9de5e88470af25743d065bc49f/downloads/DarksidersGenesis_V1.58_RESOURCE_HUD_CLEANUP_TEST.zip) : **892,043 octets**, SHA256 `2C1A81498C2FADE85C404584A6125CDDEE740E3DE76192A7CC56311C5005C112`. CRC, quatre fichiers racine, AMD64, version, 119 clés INI/48 identifiants d’actions et téléchargement public HTTP 200 contrôlés ; **122 anciens ZIP inchangés**. Le nouveau test HUD exerce le vrai hook simulé, l’orientation P1/P2, les identités réutilisées, les tailles animées et leur restauration. Le retrait Recovery conserve les anciennes clés personnelles et l’identifiant 3 inactif, sans renumérotation.

**Toujours ouverts :** déplacement + visée + tir + dash, avec toutes les munitions et les deux vues ; flou chapitre 8, notamment cheval/Ultimate ; autres sprites P2 hors InteractHUD. Aucun changement de balistique/cible, XInput/RawInput, roues ou compensation de mise au point. La lecture seule confirme le bon focus dans les deux caches natifs actuels et précédents P1/P2, notamment le cheval ; aucun rendu final corrigé n’est revendiqué. Le volume natif `LVL09_PostProcessVolume_Dungeon` est chargé dans `Level_09_Base_Dungeon`, activé, poids 1, borné ; ses bits d’override de distance et région sont désactivés malgré les valeurs stockées. Ce relevé ne démontre donc pas qu’il impose directement le flou observé ni pourquoi ce chapitre serait le seul touché.

Installation après confirmation utilisateur et vérification de 272 processus visibles, jeu fermé. Sauvegarde V1.57 `install-backup-before-v158-20261018-192726`, manifeste d’intégrité et `Restore-Previous.ps1` conservés ; syntaxe de restauration contrôlée, script non exécuté. **INI personnel inchangé octet pour octet** : `FA9E7A196CB348C9DEF962E2A62A93A4DA91042D21D067D091E232B295E3AB38` ; raccourcis conservés. ASI/DXGI installés identiques au paquet vérifié. Aucun lancement du jeu par l’agent, aucune validation de gameplay. README installé actualisé avec ce reçu ; le ZIP reste immuable.

**Essais :** (1) Clear de Zoom In avec TPS actif, UNBOUND puis Reload/reprise du jeu ; (2) Hotstreak `0.5`/`1`/`2`, ammo finies et recharge potions `0.5`, Unlimited OFF/ON ; (3) levier coop, rotations indépendantes P1/P2 et tailles `0.5`/`1`/OFF ; (4) déplacement + visée + dash sans tir en native/TPS, puis contrôle court des tirs validés et roues P2. Rollback V1.57 ; INI personnel à conserver lors de l’installation. [Audit et limites](V1_58_RESOURCE_HUD_CLEANUP_TEST.md). Les statuts de préparation ci-dessous sont historiques.

---

## V1.58 — facteurs, interactions HUD et retrait de Recovery (18 octobre 2026)

**SOURCE DE TEST / EXPÉRIMENTAL — construction et validation en jeu à effectuer.** Base cumulative V1.57 `1eb7d24b7496bdc9b6be19972da16e2431d4013b`. La reprise de visée maintenue après dash, validée par l’utilisateur en V1.56, est conservée. L’utilisateur autorise la suite des fonctions pendant que **déplacement + visée + tir + dash reste ouvert avec toutes les munitions**, dans les deux vues. La V1.57 n’est pas déclarée entièrement validée.

- Facteurs entre 0 et 1 : Hotstreak et consommation de munitions divisent leurs coûts par la valeur ; `0.5` double donc la baisse/consommation et raccourcit la réserve. Recharge de potions `0.5` = moitié du rythme natif. `0` garde le comportement natif pour ces trois options ; Unlimited reste prioritaire. Les autres multiplicateurs déjà compatibles ne sont pas réécrits.
- HUD d’interaction : taille réglable avec saisie manuelle, `0.65` par défaut, OFF/`1` = taille native. Correction expérimentale de l’orientation P2 par son vrai LocalPlayer, avant la fonction native d’orientation. Seulement les composants InteractHUD réciproquement possédés par les deux personnages ; aucune correction générale des sprites partagés.
- Action Recovery : option et fin anticipée du dash retirées, après le retour utilisateur sur leur incohérence. Le timing du dash redevient natif. La reprise AIM indépendante et les filtres de tir existants restent conservés. L’ancien identifiant Hotkey reste réservé et inactif ; les clés personnelles anciennes sont ignorées sans être supprimées.
- Zoom In / Clear : la zone des raccourcis était désactivée avec les paramètres de caméra native lorsque TPS était actif. Les raccourcis deviennent éditables dans les deux modes ; leur exécution reste limitée à la vue native. Aucun changement des touches de l’utilisateur.

**Flou chapitre 8 encore ouvert, notamment cheval et forme ultime.** Le relevé coop en lecture seule confirme P1 Havoc et P2 Rampage, poids caméra 1, bit de distance `08` actif et distances correctes (643.06 / 551.21 unités). La mise au point suit déjà la position/rotation réelle de chaque vue TPS et les changements de distance. Les autres paramètres du rendu restent à identifier ; une profondeur propre aux grands modèles ou un mélange post-process restent des hypothèses. Aucune cause particulière au chapitre 8 ni correction visuelle de ces formes n’est démontrée. Vue native/cinématiques conservent leur profondeur de champ d’origine ; le zoom manuel natif garde son repli antérieur.

**Préservé :** calcul des tirs/cible, caméra coop et compensation de mise au point V1.57, XInput, RawInput, roues, Ultimate/cheval, ressources hors facteurs cités, sauvegardes et raccourcis. Aucun asset du jeu modifié. 119 clés INI actives, 48 identifiants d’actions, 122 anciennes archives à conserver. Les nouvelles clés HUD prennent leurs valeurs par défaut si absentes de l’INI personnel.

**Objectif de la build :** facteurs sous 1 réellement appliqués, petits prompts lisibles et orientés vers leur propre vue P2, suppression de l’expérience Recovery incohérente sans perdre la visée après dash. **Réussite :** essais utilisateur P1/P2 conformes, aucune régression des tirs/roues/visée, taille restaurable et raccourci effacé persistant. Contrôles prévus : 31 suites Windows, source native, ZIP AMD64 à quatre fichiers racine et intégrité des anciennes archives. Un contrôle automatisé ne valide pas le gameplay.

**À tester :** (1) TPS actif : Clear de Zoom In, affichage UNBOUND puis Reload/reprise du jeu ; (2) Hotstreak `0.5`, `1`, `2`, puis Unlimited OFF/ON ; (3) munitions finies et recharge de potions `0.5`/`1`/`2` ; (4) levier en coop, rotation indépendante des caméras P1/P2, taille `0.5`/`1`/OFF ; (5) déplacement + visée + dash sans tir dans les deux vues, puis contrôle court Charge/Lava/faisceaux/roues P2. Le tir pendant dash et le flou restent des défauts connus, pas des corrections à valider dans cette build. Rollback : paquet/sauvegarde V1.57 avec INI personnel préservé.

---

## V1.57 — construite, contrôlée et installée (18 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation utilisateur nécessaire.** Base cumulative V1.56 ; déplacement + visée + dash validé par l’utilisateur et conservé. Deux corrections ciblées : bit natif d’activation de la distance de mise au point TPS corrigé (`08`, l’ancien `10` activait l’ouverture) ; filtre du son, des flashs et des douilles Standard du Dash Fire avec munitions spéciales. **Le mouvement corporel du montage reste natif**, pour préserver la charge et la reprise des capacités.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39133116240) **SUCCESS : 30/30 suites, zéro avertissement MSVC ASI/DXGI**. Les avis CMake/Node concernent les outils et ne sont pas masqués. Source construite `773480ba71feab1fcf439ff3473380f6b157df02`, archive immuable `78acf8005d429994c54b6d681d6444eb976b5b70`. Les fixtures exercent les vrais filtres avec des objets natifs simulés et reproduisent le bit lu par le mélange du rendu ; elles ne valident pas la présentation dans le jeu.

[ZIP cumulatif V1.57 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/78acf8005d429994c54b6d681d6444eb976b5b70/downloads/DarksidersGenesis_V1.57_FOCUS_DASH_PRESENTATION_TEST.zip) : **888,752 octets**, SHA256 `FB7A9D57F609C60FCE66E2C4F6CD33AB2E770BB98BC2909D5D5CAEE9E7B336F6`. Quatre fichiers racine, CRC, AMD64, métadonnées de version, 119 clés/48 actions, téléchargement public HTTP 200 et **121 anciens ZIP inchangés** contrôlés. Aucun changement d’INI/raccourci ; corps de reprise AIM, Tick/fin/physique Recovery, calcul des tirs, XInput/RawInput et fonctions hors périmètre conservés.

Installation jeu réellement fermé après confirmation utilisateur et contrôle de 274 processus visibles. Sauvegarde V1.56 `install-backup-before-v157-20261018-155347`, avec manifeste et `Restore-Previous.ps1` (syntaxe contrôlée, non exécuté). **INI personnel identique avant/après** : `8105BB2676132C3D51A1F53EF0BE8F972A56EA05DF727AB3831D64B516EC0A6A` ; raccourcis conservés. ASI/DXGI installés identiques au paquet vérifié. Aucun lancement du jeu par l’agent ; aucun résultat visuel V1.57 encore validé. Le README du ZIP garde sa préparation historique ; l’archive est immuable.

**À tester :** (1) chapitre 8 immobile, angles/distances TPS : personnage net avec profondeur de champ conservée ; (2) déplacement + visée + tir spécial + dash, quatre directions et Hotstreak : absence du son/flash/douilles Standard, reprise du tir spécial intacte ; (3) contrôle court de la visée après dash validée, Standard/Charge/Lava/faisceaux et roues P2 ; (4) chaque vue coop, retour natif/cinématique. Si visée ou charge régressent, revenir à la sauvegarde V1.56. [Audit et limites](V1_57_FOCUS_DASH_PRESENTATION_TEST.md). Les statuts de préparation ci-dessous sont historiques.

---

## V1.57 — mise au point TPS et manifestations du tir de dash (18 octobre 2026)

**SOURCE DE TEST / EXPÉRIMENTAL — construction et validation en jeu à effectuer.** Base cumulative V1.56 ; **déplacement + visée maintenue + dash VALIDÉ par l’utilisateur**, dans son essai du 18 octobre. Cette reprise native de visée et le timing/physique Recovery sont conservés. Restent rejetés : personnage flou au chapitre 8 ; son et manifestation du tir Standard pendant déplacement + visée + tir spécial + dash, malgré les projectiles déjà retirés.

Le relevé du chapitre 8 démontre une distance de mise au point correcte dans la caméra, mais le mauvais bit d’activation V1.56 : `0x10` pilote l’ouverture, tandis que le mélange natif lit `0x08` pour la distance. V1.57 corrige ce bit et vérifie aussi la branche qui le consomme ; effet de profondeur, qualité et autres paramètres conservés. L’ancien test contrôlait notre écriture, pas son interprétation native : il reçoit un scénario reproduisant le consommateur du rendu.

Les quatre séquences natives Dash Fire possèdent des notifications séparées pour le son Standard, les flashs et les douilles. Deux interceptions natives vérifiées écartent seulement ces manifestations lorsque le même prédicat que le filtre de projectiles reconnaît un dash local Strife à pied avec tir spécial tenu. Sons de dash/effort/pas, autres effets, tirs Standard ou mixtes/inconnus, autres personnages et modes restent natifs. **Le mouvement corporel du montage Dash Fire est conservé**, pour ne pas changer la suspension/reprise des capacités et casser la charge ; son éventuel recul résiduel n’est pas annoncé comme corrigé. Aucun remplacement de munition ni de capacité.

INI, raccourcis, balistique/cible Lava/Charge/faisceaux, XInput/RawInput, ressources, coop et paramètres caméra inchangés. Aucun asset de jeu modifié ; aucun correctif lié à une touche. Tests prévus : frontières des vrais filtres avec consommateur natif simulé, directions et P1/P2, régression du bit de mise au point, suites cumulatives. **Pas encore de nouveau ZIP ni d’installation ; le jeu ouvert est uniquement lu.**

**Essais prioritaires :** (1) chapitre 8 immobile puis plusieurs angles/distances TPS, personnage net et effet de profondeur présent ; (2) déplacement + visée + tir spécial + dash, quatre directions, normal/Hotstreak : absence de son/flash/douilles Standard ; (3) contrôle court de la visée après dash validée, tir Standard, tir spécial après dash, Charge/Lava/faisceaux et roues P2 ; (4) retour natif/cinématique et chaque vue coop. Au moindre recul de l’acquis AIM ou de la charge, revenir à V1.56. Les demandes HUD/armes/grappin restent dans la [suite prévue](ROADMAP.md).

---

## V1.56 — construite, contrôlée ; installée (18 octobre 2026)

**Demandes suivantes enregistrées le 18 octobre 2026 :** [HUD/interactions et sprites P2, rangement manuel/automatique des armes, munitions normales limitées, portée du grappin, modèles des pistolets et holsters à cheval](ROADMAP.md). Elles passent **après les réparations actuelles** ; V1.56 reste expérimentale en attente du retour utilisateur. Mise à jour documentaire uniquement, sans changement d'ASI, d'INI ni d'installation.

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation utilisateur nécessaire.** Base cumulative V1.55, avec les tirs précédemment validés conservés. La nouvelle capture AIM tenu confirme le défaut de reprise V1.55. Deux corrections sont livrées : service dans le passage de caméra qui subsiste après le dash ; accès au composant permanent DualPistolsBP, vérifié dans le personnage et son équipement, au lieu du contexte d'arme vide. La physique/timing de Recovery et le filtre de projectiles V1.55 restent identiques.

La profondeur de champ TPS est maintenant conservée : seule la distance de mise au point de chaque sortie P1/P2 suit le personnage. Qualité, ouverture et autres paramètres natifs restent actifs. **Aspect visuel à tester**, notamment chapitre 8, distances/angles et transitions cinématiques. **Son/animation du tir de dash encore ouverts** ; aucune substitution de variante susceptible d'altérer la charge spéciale n'est livrée.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39127789158) **SUCCESS : 30/30 suites, zéro avertissement MSVC ASI/DXGI**. Les avis CMake/Node concernent les outils et ne sont pas masqués. Source construite `e574452a645e82fd9f64198036a72f923f818e83`, archive immuable `5eb9a1c53df13df886c639439b21669759752620`. Le test natif simulé atteint bien une demande sur les pistolets enregistrés sans capacité/getter/contexte actif, avec TPS/Recovery OFF/ON et isolation P2 ; ce contrôle ne constitue pas un essai dans le jeu.

[ZIP cumulatif V1.56 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/5eb9a1c53df13df886c639439b21669759752620/downloads/DarksidersGenesis_V1.56_NATIVE_DASH_FOCUS_TEST.zip) : **885,040 octets**, SHA256 `14F95747114D77D7845693745A6CBE89F7EBE6C8DE25DAD3AE6350D789C73FB2`. Quatre fichiers racine, CRC, AMD64, version/nom d'archive, 119 clés/48 actions, téléchargement public HTTP 200 et **120 anciens ZIP inchangés** contrôlés. Aucun changement d'INI ou de raccourci. Code de balistique/cible, XInput, RawInput et ressources conservé ; aucun nouveau hook, recherche globale d'objets ou filtre audio dans l'ASI.

Installation jeu réellement fermé, 290 processus visibles contrôlés, sauvegarde `install-backup-before-v156-20261018-151919`, avec manifeste et restauration V1.55. **INI personnel identique avant/après** : `C117459A4641B5488551663FD7FC104C35E08064DA7C3967BC6D272FA5E6EA91`. Aucun lancement du jeu par l'agent ; pas de validation de gameplay V1.56.

**Essais prioritaires :** (1) déplacement + visée + dash sans relâchement, native/TPS, Recovery OFF/ON ; (2) Static/Charge/Graviton normal et Hotstreak pendant/après dash, Standard et P2/roues ; (3) chapitre 8 avec effet de profondeur conservé, plusieurs distances/angles, retour temporaire/cinématique et coop ; (4) contrôle court Lava/Charge/faisceaux validés. [Audit, limites et critères d'arrêt](V1_56_NATIVE_DASH_FOCUS_TEST.md). Les statuts de préparation ci-dessous sont historiques.

---

## V1.56 — reprise de visée native et mise au point TPS (18 octobre 2026)

**SOURCE DE TEST / EXPÉRIMENTAL — construction et validation en jeu à effectuer.** Base cumulative V1.55 `0b1d21a0d35740774a5f870ebab0f1a3919e379e`, avec les tirs précédemment validés conservés. Retour V1.55 : déplacement + visée + dash toujours défectueux ; projectiles Standard forcés retirés mais animation/son encore présents ; tir standard/P2/roues « ça a l'air OK » ; suppression du flou confirmée, conservation de l'effet demandée.

La nouvelle capture en lecture seule voit **trois Dash Aim** : AIM reste tenu et le vecteur de visée reste non nul, tandis que les capacités deviennent vides pendant plusieurs secondes. Deux défauts V1.55 sont corrigés dans la source : la reprise dépendait d'un getter appelé par la capacité disparue ; son accès à l'arme utilisait une pile de contexte vide après le dash. V1.56 utilise le passage existant de caméra du joueur, dans les deux vues, et le vrai composant `DualPistolsBP`, vérifié à la fois dans le personnage et son équipement. Toujours une seule demande native, avec les gardes de propriétaire/sol/vie/AIM/pause/cinématique ; aucun axe, touche ou bit d'action forcé. Recovery ON/OFF conserve la même physique et le même nettoyage.

En TPS, la qualité de profondeur de champ reste native : seule la distance de mise au point de la vue émise suit la profondeur du personnage. Méthode, ouverture, tailles de flou, transitions, poids et autres effets sont conservés. P1/P2 ont chacun leur sortie ; pauses/roues gardent le dernier plan et les cinématiques/vue native ne reçoivent pas cette compensation. Un poids caméra partiel/inconnu reste natif ; l'ancien zoom manuel et le repli de compatibilité conservent leur règle V1.54. **L'aspect visuel du nouveau plan n'est pas encore validé.**

**Toujours ouvert : son et animation du tir Standard pendant le dash spécial.** Le filtre de projectiles V1.55 est conservé. La piste de sélection d'un Dash Aim à la place du Dash Fire a été écartée avant construction : elle change aussi la suspension/reprise des capacités spéciales, et pourrait dégrader le tir chargé. Aucun nouveau hook de variante, remplacement d'asset ou filtre audio général n'est livré.

Nettoyage : ancien rappel de reprise dans le getter et ancien accès à l'arme de contexte retirés ; aucune couche de correction concurrente. Balistique/cible, XInput, RawInput, coop, raccourcis, ressources et configuration conservés. 119 clés INI, 48 actions Hotkeys et 120 anciennes archives à préserver. Suite de 30 tests prévue, dont un scénario sur le vrai code de reprise avec pile de contexte vide et sans appel au getter, et un contrôle des seuls octets modifiés pour la mise au point. Ces tests ne remplacent pas un essai dans le jeu.

**Essais :** (1) déplacement + visée + dash, vue native/TPS, Recovery OFF/ON, sans relâcher la visée ; (2) tir Static/Charge normal et Hotstreak pendant/après dash, puis Standard et P2/roues ; (3) chapitre 8 immobile/mouvement, plusieurs distances et angles TPS : personnage net, effet de profondeur encore visible ; (4) retour temporaire/vue native/cinématique et chaque vue coop ; (5) contrôle court des tirs Lava, Charge et faisceaux déjà validés. [Audit, limites et critères d'arrêt](V1_56_NATIVE_DASH_FOCUS_TEST.md). Les statuts ci-dessous sont historiques.

---

## V1.55 — construite, contrôlée et installée (18 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation utilisateur nécessaire.** Base cumulative V1.54 `6bea05ec6347ef4e0a15e34e7c2b6bc8a23491b2`, avec les tirs V1.53 validés conservés. V1.54 n'avait pas été installée : V1.55 remplace directement l'installation V1.53 et apporte aussi son correctif de profondeur de champ côté rendu.

Le candidat redémarre le cycle natif de visée une seule fois après un dash lorsque la liste des capacités est vide et l'action AIM reste tenue. Il écarte aussi les projectiles Standard imposés par les quatre animations Dash Fire lorsque seules des demandes de munitions spéciales reconnues sont tenues. **Les tirs spéciaux reprennent après le dash selon leur cycle natif ; aucun tir spécial continu pendant toute l'animation n'est promis.** Vue du dessus/TPS et Recovery ON/OFF partagent la règle, sans correction d'une touche ou d'un axe. Les demandes mixtes Standard + spécial et les cas inconnus restent natifs.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39125021680) **SUCCESS : 29/29 suites, zéro avertissement MSVC ASI/DXGI**. Les avis de dépréciation CMake/Node restent ceux des outils ; aucune suppression d'avertissement ajoutée. Source construite `7f99563a26975f07950ef47584d5a2bc0e23dccf`, archive immuable `eb5cd3efea4d0872a1169a7101ee0eadf6402f4a`.

[ZIP cumulatif V1.55 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/eb5cd3efea4d0872a1169a7101ee0eadf6402f4a/downloads/DarksidersGenesis_V1.55_NATIVE_DASH_COMBAT_RESUME_TEST.zip) : **879 258 octets**, SHA256 `68E196A46ED5316140EC3F0EFCE3198AD776D5A0114B8ED5C401409208189FC3`. Quatre fichiers racine, CRC, AMD64, version, 119 clés/48 actions, retrait de l'ancien diagnostic des axes et téléchargement public HTTP 200 contrôlés. ASI `D740A6275FC4234EB60E83E52D835E735C0E422FA9646E68AEF02813CC0404A6`, DXGI `0845A1045D53A696A6DEEEFFBB79A263A6F18D7634DA61EEA67954AFEF2A9FA0`.

Installation jeu fermé après vérification de 299 processus visibles dans le contexte Windows complet. Sauvegarde locale V1.53 `audit-v131/install-backup-before-v155-20261018-135757`, manifeste d'intégrité et `Restore-Previous.ps1` fourni ; syntaxe de restauration contrôlée, script non exécuté. **INI personnel conservé octet pour octet**, avant/après `6BCD4FA9A40D92ED4380767C89137B11145DE1FACEB4C3D7DD50F71833B4F407`. Aucun lancement du jeu par l'agent. Le README installé reçoit ce reçu ; le README de préparation du ZIP reste immuable avec l'archive.

70 autres fichiers source, le timing/fin/protection en combat de Recovery, le code de transformation XInput restant, RawInput, balistique/cible commune, caméra/DOF, roues, coop, ressources et Hotkeys conservés. **119 anciens ZIP inchangés**, main/archive, tag et release v1.43 préservés. Le relevé des axes physiques V1.53 est retiré ; les deux nouveaux compteurs sont bornés et indiquent des demandes/exclusions, pas une réussite de gameplay.

**Essais prioritaires :** (1) déplacement + visée + dash en vue du dessus puis TPS, Recovery OFF/ON ; (2) Static Shot puis Charge Shot pendant/après le dash, normal/Hotstreak, absence de balle Standard ajoutée ; (3) tir Standard pendant dash inchangé ; (4) P2 et roues ; (5) chapitre 8 immobile et bascule TPS/vue native/cinématique ; (6) contrôle court Lava, Charge Shot, faisceaux et obstacles. [Audit, limites, critères d'arrêt et six essais](V1_55_NATIVE_DASH_COMBAT_RESUME_TEST.md). Aucun de ces résultats n'est encore validé par ce build. Les anciens statuts ci-dessous sont historiques.

---


## V1.55 — reprise native de visée et tirs forcés du dash (18 octobre 2026)

**SOURCE DE TEST / EXPÉRIMENTAL — construction et validation en jeu à effectuer.** Base cumulative V1.54 `6bea05ec6347ef4e0a15e34e7c2b6bc8a23491b2`, avec les tirs V1.53 validés et le correctif DOF V1.54 conservés. Le retour confirme le tir normal pendant le dash en vue du dessus **et en TPS**. La capture Static Shot montre trois dashs Aim et deux dashs Fire : après les deux premiers, plus aucune capacité active malgré un vecteur de visée encore non nul ; pendant les dashs Fire, les capacités de visée existantes sont suspendues puis reprennent.

V1.55 ajoute une demande native de reprise de visée **une seule fois par dash**, uniquement pour le Strife local à pied, au sol, sans capacité active, avec l'action native AIM encore tenue. La fonction native du composant de pistolets reprend son cycle habituel ; aucune touche, inversion de stick, rotation ou bit d'entrée n'est forcé. Le retour de la fonction n'est pas une validation visuelle. Les deux vues et Recovery ON/OFF partagent cette règle.

Les quatre séquences natives Dash Fire contiennent des notifications de projectile. Le cas avant impose explicitement la classe Standard : le correctif écarte ce tir forcé pendant un dash local lorsque les demandes de tir tenues concernent des munitions spéciales reconnues. **Les tirs spéciaux conservent leur pause/reprise native après le dash ; aucune nouvelle variante balistique n'est fabriquée pendant l'animation.** Les tirs standards, tirs hors dash, ennemis, cheval/Ultime et cas inconnus restent natifs. Un maintien simultané Standard + spécial reste natif faute de correspondance démontrée entre les deux notifications de pistolet et les deux sélections de munition.

Nettoyage : retrait du diagnostic des axes physiques ajouté pour l'audit V1.53 ; un suivi de cycle court remplace ses compteurs et les journaux sont bornés. Les traitements XInput/RawInput et les raccourcis sont inchangés. Recovery garde sa physique, sa fin native et sa protection en combat. Les autres fonctions validées, la cible/balistique et l'INI de 119 clés/48 actions sont conservés. Aucun nouvel interrupteur, aucune modification des anciennes archives ou des branches main/archive.

**Réussite à vérifier :** garder la visée après déplacement + visée + dash ; ne plus produire de balle Standard à la place de Static Shot pendant Dash Fire ; reprise normale du tir spécial après le dash. Comparer vue du dessus/TPS et Recovery OFF/ON, puis P2. Contrôler aussi le flou du chapitre 8, Lava/Charge Shot, faisceaux et roues. 29 suites prévues en CI ; compilation, paquet et installation ne prouvent pas ces résultats en jeu. [Audit et protocole](V1_55_NATIVE_DASH_COMBAT_RESUME_TEST.md). Les statuts plus anciens ci-dessous sont historiques.

---


## V1.54 — construite et contrôlée, installation en attente du jeu fermé (18 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL — PAS ENCORE INSTALLÉ NI VALIDÉ EN JEU.** Base V1.53 conservée pour les tirs déjà validés. Le correctif de profondeur de champ atteint désormais les copies jeu **et rendu** ; leur restauration conserve les deux valeurs d'origine et donne priorité à la suspension cinématique existante. La lecture du jeu ouvert en V1.53 a confirmé **jeu=0, rendu=2** : l'ancien journal « DOF supprimée » ne prouvait donc pas une suppression effective dans le rendu. Le test visuel du chapitre 8 reste nécessaire.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39110618829) **SUCCESS : 28/28 suites, zéro avertissement MSVC ASI/DXGI**. Les avis de dépréciation des outils CMake/Node restent distincts ; aucune suppression d'avertissement ajoutée. Source `56a7c6cfc6f39574a7292bd58d6076330610ee60`, archive immuable `ca87039de126c7c395d0359ed83b069917c90fe0`.

[ZIP cumulatif V1.54 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/ca87039de126c7c395d0359ed83b069917c90fe0/downloads/DarksidersGenesis_V1.54_CAMERA_DOF_RENDER_FIX_TEST.zip) : **873 459 octets**, SHA256 `1E2579128C11AEBCF0D2EAE21A23BF6771ADB9DFC2A4D7C626F20756900151E0`. Quatre fichiers racine, CRC, AMD64, version, 119 clés/48 actions et téléchargement public HTTP 200 contrôlés. **118 anciennes archives inchangées** ; main/archive, tag et release v1.43 conservés. 72 autres fichiers source et toutes les fonctions hors DOF du module principal inchangés. INI/raccourcis sans modification.

Le jeu est ouvert : **aucun fichier installé remplacé**, aucune écriture dans sa mémoire ni lancement par l'agent. La première interrogation non privilégiée ne voyait pas ses processus ; vérification Windows corrigée en dehors de ce contexte restreint, puis observation `VM_READ` seulement. Ne pas déduire « jeu fermé » d'une liste de processus vide dans un contexte sans visibilité suffisante. Installer après fermeture réelle, sauvegarde et contrôle de l'INI personnel.

**Dash toujours ouvert :** déplacement + visée + dash perd la réponse de visée ; avec tir, une balle normale peut remplacer la munition équipée, Recovery ON comme OFF. Nouveau fait statique : les deux notifications de tir de `Strife_Aim_Dash_Fire_F` imposent `ProjectileOverride = MayhemProjectile_Standard_Blueprint_C` ; les séquences B/L/R référencent également cette classe et restent à contrôler au point d'appel. Cela fournit une piste native concrète, sans prouver encore la provenance de chaque projectile observé ni un comportement du jeu sans ASI. Les variantes spéciales ont leur propre cycle de tir : ne pas remplacer arbitrairement une classe de projectile sans ce cycle. Aucune réparation risquée de munition/visée n'est embarquée dans V1.54 ; capture externe prête pour identifier la transition réelle.

**À tester après installation :** chapitre 8 immobile/mouvement, bascules TPS/vue native, retour temporaire et cinématique ; contrôle court Lava/Charge Shot, faisceaux, collisions devant/derrière et roues coop. [Audit, limites et suite](V1_54_CAMERA_DOF_RENDER_FIX_TEST.md). Les anciens statuts ci-dessous sont historiques.

---


## V1.54 — profondeur de champ réellement lue par le rendu (18 octobre 2026)

**SOURCE DE TEST / EXPÉRIMENTAL — construction et validation en jeu à effectuer.** Base V1.53 `997433c2e05170f7bf41f65118c137c691101930`. Retour utilisateur : le flou excessif du chapitre 8 reste visible même immobile. L'ancien correctif V0.55 ne modifiait que la copie jeu de `r.DepthOfFieldQuality` ; le rendu lit la copie suivante, à `data+4`. V1.54 corrige ce défaut démontré dans l'exécutable supporté, sans nouveau hook, shader, modification d'Engine.ini ni réglage persistant du jeu.

Les deux copies ont maintenant leur sauvegarde propre et sont supprimées pendant la caméra déplacée, puis restaurées à la vue native. Une valeur native devenue différente de notre zéro est conservée au retour. La suspension cinématique existante restaure les effets natifs ; la classification pause/roues/cinématiques ne change pas. Le réglage manuel de zoom reste indépendant en gameplay. **Le chapitre 8 n'est pas encore validé visuellement : un autre effet du niveau peut contribuer au flou.**

Les deux bugs de **déplacement + visée + dash**, avec ou sans tir et Recovery ON/OFF, restent **OUVERTS**. L'analyse des scripts natifs confirme le choix de trois variantes de dash et leurs transitions propres. `69FBD0` est `DoAimAbility`, pas un calcul de projectile : aucune substitution de munition fondée sur cette mauvaise interprétation n'est ajoutée. La prochaine preuve attendue est la capacité réellement active avant/après le défaut ; capture externe en lecture seule, sans code de diagnostic supplémentaire dans l'ASI. Pas de correctif d'une touche ou d'un stick.

**Conservé :** tirs Lava/Charge Shot et corrections faisceaux/obstruction validés dans les cas testés ; toutes les autres fonctions, valeurs par défaut, 119 clés INI et 48 actions Hotkeys. Aucun changement XInput/RawInput, P2, roues, ressources, balistique ou Recovery. Anciens ZIP et branches main/archive préservés.

**Objectif de cette build isolée :** supprimer la profondeur de champ résiduelle en TPS au chapitre 8, et retrouver les effets natifs hors caméra modifiée. **Tests :** comparer TPS/vue native immobile puis en mouvement ; basculer plusieurs fois ; utiliser le retour temporaire et une cinématique ; contrôle court tirs et roues. Les bugs de dash ne sont pas annoncés corrigés. [Audit, limites et suite](V1_54_CAMERA_DOF_RENDER_FIX_TEST.md). Les paragraphes plus anciens ci-dessous sont historiques.

---


## V1.53 — validations utilisateur et deux bugs de dash ouverts (18 octobre 2026)

**VALIDÉ PAR L'UTILISATEUR :** les points 1 et 2 du dernier protocole sont corrigés : Static Shot/Beam Shot ne partent plus à gauche dans les cas testés ; caméra contre le décor sans inversion du tir, avec obstacle devant toujours bloquant. Conserver ces corrections, ainsi que Lava et le tir chargé déjà validés. Le retour ne détaille pas toutes les variantes ON/OFF, normal/Hotstreak ou P2 ; aucune validation générale de ces combinaisons n'est ajoutée.

**Toujours défectueux, avec Action Recovery ON comme OFF :**

1. Déplacement + visée + tir + dash : des munitions normales sortent à la place de la munition choisie.
2. Déplacement + visée + dash : la commande de visée ne répond plus. Le stick mentionné décrit la reproduction ; le correctif doit concerner la fonction de visée et sa transition d'action, indépendamment des touches, périphériques et interversions de sticks.

La protection de dash en combat V1.53 n'est donc **pas une réparation validée** de ces défauts. Leur reproduction OFF ne permet pas d'attribuer le problème uniquement au raccourcissement actif de Recovery ; elle ne démontre pas non plus que tous les hooks/états du mod sont innocents ni qu'il s'agit d'un bug du jeu sans mod.

Le journal V1.53 commencé à 09:39:46 confirme des essais P1 en vue native : 48 observations après dash avec Recovery ON et 48 avec OFF, avec axes physiques et états d'action encore présents dans plusieurs échantillons. Cela ne prouve pas que la bonne capacité de visée/tir reste active. Douze lignes BEAM_FORWARD attestent aussi l'exécution du chemin corrigé ; **la validation visuelle vient du retour utilisateur**, pas de ces compteurs. Capture privée effectuée sans modifier le jeu ouvert.

**Prochaine étape :** auditer le choix des variantes natives Dash/Aim/Fire, la capacité de munition sélectionnée, les actions en attente et leur reprise à la fin du dash. Rechercher le point commun de transition avant tout nouveau hook. Corriger les fonctions et leurs propriétaires, sans filtrage d'une touche ou d'un stick particulier ; réévaluer la protection Recovery existante avant d'empiler une autre réparation. Réception lourde et progression du triple dash restent des sujets séparés.

V1.53 reste la base conservée avec ces deux exceptions ouvertes. **Mise à jour documentaire uniquement : aucun changement d'ASI, d'INI, de raccourci ou de ZIP, aucune nouvelle build/install ; main/archive préservés.** Source construite `a240350a0ebeb996a84821f9e39db881c9b909ef`, archive immuable `dfe76d3439537e1afafefd0623e04e5704438038`. Les listes de tests et statuts de préparation plus anciens ci-dessous sont historiques.

---

## V1.53 — construite, contrôlée et installée (18 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — corrections en jeu à tester.** Base V1.52 ; **Lava et tir chargé validés par l'utilisateur et conservés**, cas simple tir + dash rétabli. V1.53 traite le consommateur natif des faisceaux libres, les cibles de caméra situées derrière le départ du tir, et la protection des dashs de combat contre une fin anticipée Recovery. La description de l'option explique cette limite : **pendant une action native de visée ou de tir, le jeu conserve le dash complet**. La disparition des deux bugs de dash reste à démontrer ; aucune substitution forcée de munition ni réactivation forcée d'AIM/MOVE.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39105711994) **SUCCESS : 28/28 suites, zéro avertissement MSVC ASI/DXGI**. Source `a240350a0ebeb996a84821f9e39db881c9b909ef`, archive `dfe76d3439537e1afafefd0623e04e5704438038`. La première exécution a été annulée pour inclure le descriptif de Recovery avant livraison ; aucune archive de cet essai interrompu publiée/installée. Les avis de dépréciation des outils CMake/Node restent distincts de l'analyse du code.

[ZIP cumulatif V1.53 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/dfe76d3439537e1afafefd0623e04e5704438038/downloads/DarksidersGenesis_V1.53_BEAM_FORWARD_OBSTRUCTION_DASH_GUARD_TEST.zip) : **870 636 octets**, SHA256 `3218102DF626F11ECB21E51AC2AF402980535F94768F3CF18D401D176A9AF64A`. Quatre fichiers racine, CRC, AMD64, version, 119 clés et 48 IDs contrôlés ; téléchargement public HTTP 200 identique à la CI/dépôt. ASI `DF497BB1000D072000575C0A6C9C5BF70979829941D170033421BA68EBDEBFEE`, DXGI `DE99816FB5DF1A0AA1FDF919C8EAAA761BC742BB26526E816022C66033D23128`. **117 anciennes archives inchangées**, main/archive, tag et release v1.43 conservés.

Installée jeu fermé, sauvegarde V1.52 `audit-v131/install-backup-before-v153-20261018-085925`, restauration fournie et syntaxe vérifiée, non exécutée. **INI personnel conservé octet pour octet**, SHA256 avant/après `6BCD4FA9A40D92ED4380767C89137B11145DE1FACEB4C3D7DD50F71833B4F407`. Aucun lancement du jeu par l'agent. Le README installé reçoit ce reçu ; celui du ZIP reste immuable. 67 autres fichiers source inchangés ; seul le texte explicatif Recovery change dans le menu. Le calcul Lava reste identique.

**À tester :** Static Shot/Beam Shot ON/OFF et P1/P2 ; caméra contre un mur derrière puis mur devant ; déplacement + visée + dash en vue native, Recovery OFF/ON ; munition spéciale pendant dash ; contrôle court Lava/Charge Shot et roues coop. Les branches de socket reconnu et de ciblage/chaîne des faisceaux restent natives ; leur couverture n'est pas validée. [Audit, critères, limites et cinq essais](V1_53_BEAM_OBSTRUCTION_DASH_GUARD_TEST.md). Les paragraphes de préparation ci-dessous sont historiques.

---

## V1.53 — faisceaux, obstacle derrière la caméra et protection du dash en combat

## Base, retour utilisateur et objectif

Base debug V1.52 `8ec540a0542af4a2e77938b932f3e7c8531bbb3b`. Le 18 octobre, l'utilisateur **valide Lava et le tir chargé**. Tir et dash en vue du dessus refonctionnent dans le cas simple. Quatre défauts restent signalés : Static Shot et Beam Shot à gauche ; perte de visée après déplacement + visée + dash ; balles normales pendant dash avec Recovery et une autre munition équipée ; tir retourné vers la caméra quand elle touche le décor. La validation ne couvre pas globalement toutes les munitions/variantes/P2. INI, valeurs par défaut et raccourcis à préserver.

Objectif : corriger la direction du faisceau à son véritable consommateur et empêcher une cible de caméra derrière le départ du tir ; protéger la transition native du dash en combat. Critères : faisceaux vers la cible sans décalage fixe, caméra contre le décor sans tir arrière, munition et visée conservées au dash. **SOURCE DE TEST / EXPÉRIMENTAL**, compilation, installation et comportement réel à vérifier avant de les annoncer.

[Audit, limites et cinq essais](V1_53_BEAM_OBSTRUCTION_DASH_GUARD_TEST.md). 119 cles / 48 IDs Hotkeys conserves.

---

## V1.52 — base de tir commune et nettoyage vérifiés, installée (17 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — tirs et dash à valider en jeu.** Base V1.51 debug, dont le changement d'angle caméra avec 3D et le retour du défaut Charge Shot/Hotstreak avec OFF sont rejetés. Les acquis validés de V1.50 sont conservés. **ON/OFF partagent le même socle de direction et la même pose caméra.** La case ne retire plus la correction de direction au relâchement ; ON ajoute seulement la visée verticale. Réglage indépendant **« Shot vertical angle »**, saisie manuelle de −45° à +45°, défaut **+12°** pour la vue −12° habituelle. Il agit sur la trajectoire réelle, sans changer la caméra.

L'ancien hook de rotation placé trop tôt est retiré : une fonction native pouvait ensuite reprendre le yaw du personnage. La correction intervient désormais après cette substitution. Les tirs ordinaires utilisent l'argument natif `SpawnLocation`, au lieu d'une référence approximative ; aucun décalage horizontal fixe ajouté. Les origines de toutes les variantes de capacité restent à vérifier. Les trois interceptions couvrent les signatures natives distinctes pistolet/capacité/Lava ; elles utilisent le même résolveur de cible/direction. Le helper V1.51 qui changeait la caméra avec 3D est supprimé.

**[CI Windows x64 réussie](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39070045234) : 27/27 suites, zéro avertissement MSVC sur l'ASI/DXGI**, sans désactivation d'alerte ni hausse des seuils d'analyse. Les huit avertissements historiques sont traités. Les outils externes CMake/Node des Actions émettent encore des avis de dépréciation ; ce n'est pas une compilation entièrement silencieuse. Source ASI/DXGI `6b081c58497a177d8e8af60dd3320c56e3606b31`, archive `fd4fe3f8cd213e81fb7fbf998c62b595cbf3cd8d`. Zéro alerte ne prouve ni l'absence de toute dette de code ni une correction en jeu.

[ZIP cumulatif V1.52 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/fd4fe3f8cd213e81fb7fbf998c62b595cbf3cd8d/downloads/DarksidersGenesis_V1.52_COMMON_AIM_INPUT_CLEANUP_TEST.zip) : **866 972 octets**, SHA256 `9B72998C9CD15883CE16AE0412332000449D8587D61675DE0D169BC10BAB58EB`. Quatre fichiers racine, CRC, PE AMD64, identité ASI, 119 clés et 48 IDs Hotkeys contrôlés ; public/dépôt/CI concordants. Les 118 anciennes valeurs sont conservées. Les 116 anciennes archives, main/archive et v1.43 restent protégés.

Installée jeu fermé après sauvegarde `audit-v131/install-backup-before-v152-20261017-231827`. ASI `ADEB44C256D5D90BAA744F26FE8AE534C2527BC06BAF1820A8792B09EB109162`, DXGI `F7AF999350B298D251B5428FA88AA2A42F2A46CD33D1BCE115A1880194E2B800`. **INI personnel conservé octet pour octet**, SHA256 avant/après `059085DAC051F980A6F9735057D9A741AA0AB70BA177E2D9069DD75162D4E2D9`. La clé d'angle absente prend +12° au chargement sans modifier l'INI à l'installation. Aucun lancement du jeu. README installé actualisé séparément ; ZIP numéroté immuable.

Entrées : vue TPS récente obligatoire pour le getter de regard, lecture caméra XInput brute protégée et une seule transformation par lecture du jeu, même en cas d'appels imbriqués. **Le bug du stick après dash reste à tester**, comme la progression du triple dash et la réception lourde ; aucune nouvelle fin d'animation forcée. Le journal complet V1.51 confirme 4142 recalculs Lava sans rejet et 20 demandes Recovery, sans preuve d'impacts corrects. Ses 96 échantillons après dash ont saturé le budget avec l'option TPS ON ; cela ne permet aucune conclusion sur les essais ultérieurs ni sur la vue effectivement affichée. Les nouveaux budgets sont séparés entre les options TPS/Recovery. [Preuves, limites, six essais et rollback](V1_52_COMMON_AIM_INPUT_CLEANUP_TEST.md).

---

## V1.51 — construite, contrôlée et installée (17 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — alignement des tirs à valider en jeu.** Base V1.50 debug partiellement acceptée : roues/équipement/changement de personnage validés par l'utilisateur ; Charge Shot/Graviton/Lava rejetés pour leur direction trop basse. Recovery produit un effet observé, mais la progression du triple dash, la réception lourde et les sticks après dash restent ouverts. Cinématiques à vérifier ; coop/autres fonctions paraissent bonnes, sans validation générale.

La case **« 3D aiming (experimental) »**, activée par défaut, sépare l'inclinaison de cadrage de la direction rendue : après **recentrage**, la vue est horizontale, la caméra garde sa position en hauteur et l'orbite permet de viser haut/bas. OFF rend le regard simple Camera Forward, les trajectoires/hauteurs natives et la pose caméra précédente. Le contrôle des modèles de composants Lava est corrigé ; les futurs rejets donnent leur motif. Les sticks après dash reçoivent une observation bornée en lecture seule, sans nouveau hook. Aucun nouveau correctif de réception n'est annoncé.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39065973807) **SUCCESS, 26/26 suites**. Source ASI/DXGI `8f226dc5ece66b3890f7764a08081c9e224cfd3e`, archive `4d064c6852bd67b34ce99c25f41f8641bd499b26`. **Huit avertissements statiques historiques**, contre douze en V1.50 : ceux liés à l'identité et aux compteurs de tirs ont disparu. Ces contrôles ne prouvent pas les impacts réels ni les réceptions.

[ZIP cumulatif V1.51 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/4d064c6852bd67b34ce99c25f41f8641bd499b26/downloads/DarksidersGenesis_V1.51_OPTIONAL_3D_AIM_LEVEL_VIEW_TEST.zip) : **864 771 octets**, SHA256 `82F22328A8190C1AA71954F183606BC2607160420E0304492E5E9003EBC1706D`. Quatre fichiers racine, CRC, PE AMD64, identité ASI, 118 clés actives et 48 IDs Hotkeys contrôlés ; téléchargement public, dépôt et empreinte CI concordants. Les 117 anciennes valeurs sont conservées. Les cinq options retirées restent absentes de l'ASI.

Installée jeu fermé avec sauvegarde : `audit-v131/install-backup-before-v151-20261017-222107`. ASI `99C97E052EE343C0DF6426660D47886FF8FC299906FBBA887676475E7D010B37`, DXGI `65D64F4DB70395E408EC430A5E2C3A4D90FB30D223BAEBBD074BEA97F281F127`. **INI personnel conservé octet pour octet**, SHA256 avant/après `D0E1491510DB41C6ED6CD53875B3F30CD8E88D56AF02343040955465A91D983E`. `TPS3DAim` absent de cet INI prend ON sans le modifier lors de l'installation. Aucun lancement du jeu. README installé actualisé séparément ; ZIP numéroté immuable.

Les **115 anciennes archives**, main/archive, tag et release v1.43 sont préservés. Le journal confirme 0 recalcul Lava réussi/1623 rejets et 139+7 demandes Recovery en V1.50 ; la correction de garde CDO est fondée sur le code, pas sur un motif de rejet absent du vieux log. [Preuves, limites, huit essais prioritaires et rollback](V1_51_OPTIONAL_3D_AIM_LEVEL_VIEW_TEST.md).

---

## V1.50 — construite, contrôlée et installée (17 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation en jeu à faire.** Base V1.49 debug et acquis V1.48 conservés hors réparations ouvertes. Source ASI/DXGI `832f2d17425f6a79dcebe490ce2f9d61a8c8c8af`, archive `b492f2b96b84bbf20216d2940d2811da6d1b6336`. [GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39047758200) **SUCCESS, 26/26 suites**. Les tests couvrent les vrais modules avec frontières natives simulées ; ils ne mesurent ni impacts réels ni gain de contrôle après dash.

[ZIP cumulatif V1.50 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b492f2b96b84bbf20216d2940d2811da6d1b6336/downloads/DarksidersGenesis_V1.50_AIM_RADIAL_RECOVERY_CLEANUP_TEST.zip) : **861 128 octets**, SHA256 `57727E9CA19241AC46A4BD08A8E9EA861AB2E539F7026C44FEF1CDA782152E04`. Quatre fichiers racine, CRC, deux PE AMD64, identité ASI 1.50, 117 clés et 48 IDs Hotkeys contrôlés ; téléchargements public/dépôt/CI concordants. Les cinq réglages retirés en V1.49 restent absents de l'ASI. Aucun succès de gameplay annoncé.

Installé jeu fermé après sauvegarde des quatre fichiers et du journal : `audit-v131/install-backup-before-v150-20261017-181241`. ASI `504AC3EBE1ADA0835FCAAE248DBE40D15EB72E0099BFD0E7D81781745FCAF0B6`, DXGI `E28046DCA57B685EA0680FAB2F061CF676E16F99071CAAEFB1C00C9DC11086C5`. **INI personnel conservé octet pour octet**, SHA256 avant/après `D0E1491510DB41C6ED6CD53875B3F30CD8E88D56AF02343040955465A91D983E`. Aucun lancement du jeu. Le README installé est actualisé avec ce reçu séparément ; le README du ZIP reste immuable.

Nettoyage et essais : garde Recovery Actor/composant réparée d'après 1 203 observations historiques, pistolets/Finish et Lava reliés à la cible 3D par joueur, roues protégées par état natif possédé, garde de blend cinématique, caches liés aux serials UE4, relâchements souris et sauvegardes updater. **La visée après dash en vue du dessus reste partiellement traitée pour l'affichage ; l'orientation native n'est pas déclarée réparée.** La parallaxe de près et l'ensemble des variantes natives/Blueprints restent à éprouver. [Audit complet](V1_49_COMPLETE_SOURCE_AUDIT.md) et [liste des 20 essais / limites / rollback](V1_50_NATIVE_AIM_RADIAL_RECOVERY_TEST.md).

L'analyse produit **12 avertissements MSVC : huit historiques et quatre nouveaux** (nom local masquant une identité, plus deux avertissements d'indexation indirecte des compteurs). Les gardes P1/P2 existent avant l'accès ; les rendre plus explicites et renommer la variable reste une dette de nettoyage, sans prétendre à un compilateur sans avertissement. Les limites d'arrêt/threads/INI transactionnel et de visibilité shader par viewport restent documentées. Pas de gain de FPS promis.

**113 anciens ZIP vérifiés aux octets d'origine**, main/archive et tag/release v1.43 préservés. L'ancienne CI indépendante de l'updater a réécrit V1.1 lors du premier push ; son ZIP d'origine a été rétabli puis contrôlé. Le workflow versionne désormais la correction séparément : updater V1.2, archive `c524bb0`, [CI SUCCESS](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39047926649). Aucun nouveau tag/release stable. La V1.50 reste le seul nouveau mod installé.


## V1.50 — visée 3D, roues natives, Recovery et nettoyage (préparation)

Base V1.49 debug, acquis V1.48 conservés hors réparations ouvertes. **SOURCE PRÉPARÉE / EXPÉRIMENTAL — compilation Windows et validation utilisateur requises.** Correction d'une garde qui empêchait Action Recovery de passer (Instigator Actor confondu avec composant, confirmé par 1 203 observations historiques), visée vers la collision 3D propre à chaque joueur dans les retours pistolet/Finish, et recalcul couplé rotation/vitesse de Lava avec ses formules natives. Roues attribuées à P1/P2 par transitions natives plutôt que touches ; blend caméra expiré pris en compte. Aucun succès d'impact ni gain de récupération annoncé avant essai.

Nettoyage des variables/API sans effet, caches de valeurs d'origine liés aux serials UE4, protection des relâchements souris, restauration de l'état graphique et sauvegardes updater vérifiées. **117 clés actives, 48 IDs et valeurs personnelles conservés.** Les acquis jauges, refills, paramètres caméra, RawInput, bindings et fonctionnement ordinaire restent le socle. La visée après dash en vue du dessus reste ouverte pour l'orientation ; le masquage graphique a une correction bornée. Pas de promesse de FPS ni d'absence absolue de dettes.

[Preuves, limites, retour arrière et liste complète des 20 essais](V1_50_NATIVE_AIM_RADIAL_RECOVERY_TEST.md). [Audit complet du source V1.49](V1_49_COMPLETE_SOURCE_AUDIT.md). Les reçus précédents ci-dessous restent historiques.

## V1.49 — construite, contrôlée et installée : Action Recovery dash (17 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — effet en jeu à valider par l'utilisateur.** Base V1.48 acceptée hors exceptions conservée. Source finale `8a73298f6ececb30d03483857bde921a3fadf614`, archive debug `2408c3740392987a5c1a7c3826b578ec2a619b9a`. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38998878024) **SUCCESS, 24/24 tests**. Le vrai module Recovery est testé avec frontières natives simulées ; cela ne mesure pas le délai réel de contrôle dans le jeu. Huit avertissements MSVC historiques, aucun nouveau dans Recovery. La première CI a compilé mais échoué sur deux tests de configuration ; la ligne de sauvegarde orpheline détectée a été retirée avant tout paquet/install, puis les 24 tests ont passé.

**Action Recovery (dash)** retrouve sa case et son ancien raccourci (ID 3), avec délai de 0 à 300 ms, défaut 0. La physique originale s'exécute entièrement ; après une décélération positive observée puis épuisée, demande unique de fin normale native, avec restauration par le jeu de son mouvement/état temporaire. Variantes inconnues, Horse/Ultimate, actions de coffre, pause/cinématique et activations non observées restent natives. OFF rend le comportement natif. **Le gain de récupération et l'absence de glissement restent à confirmer en jeu ; coffres différés.**

Nettoyage contrôlé dans source et binaire : cinq réglages sans effet supprimés, atomiques écrites sans lecture et compteur passif retirés, commentaires erronés corrigés. **117 clés actives et leurs valeurs, 48 identifiants Hotkeys conservés** ; 57 autres fichiers source et 13 fonctions centrales de visée/RawInput/entrées comparés. Les cinq noms de réglages retirés sont absents de l'ASI. Les anciennes lignes d'un INI personnel sont laissées intactes et ne sont plus consommées ; aucun gain de FPS ni propreté absolue de tout le dépôt n'est revendiqué. Roues coop, Charge Shot/Graviton/Lava et visée native après dash restent les réparations ouvertes séparées.

[ZIP cumulatif V1.49 — téléchargement direct vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/2408c3740392987a5c1a7c3826b578ec2a619b9a/downloads/DarksidersGenesis_V1.49_DASH_RECOVERY_CLEANUP_TEST.zip), **848 881 octets**, SHA256 `06434519009CB31D36FC71B47DEABE4E72593BD31FE873A5D0808C0303CF1007`. Exactement ASI/dxgi.dll/INI/README à la racine, CRC et binaires AMD64, identité 1.49 et 117 clés contrôlés ; empreintes CI/dépôt/téléchargement public identiques. **112 anciens ZIP conservés** (110 mod + 2 updater), branches main/archive et tag v1.43 protégés ; pas de nouvelle release publique. Le README immuable du ZIP est le reçu de préparation ; le dépôt et le README installé portent le présent reçu.

Installation après contrôle des processus : jeu fermé. Sauvegarde complète des quatre fichiers et du log : `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v149-20261017-053229`. ASI/dxgi installés conformes au ZIP ; **INI personnel et raccourcis conservés octet pour octet**, SHA256 avant/après `D0E1491510DB41C6ED6CD53875B3F30CD8E88D56AF02343040955465A91D983E`. Aucun lancement automatique ni écriture externe de mémoire. Le README installé est actualisé séparément après ce reçu.

**Test ciblé :** en mission, Strife à pied, comparer Action Recovery OFF puis ON avec délai 0 : trois dashs enchaînés puis quatrième, mouvement maintenu et attaque à la sortie ; vérifier distance, glissement, reprise des commandes et absence d'invulnérabilité persistante. Puis War/P2 et modes natif/TPS. Retour arrière : OFF, ou binaires/README V1.48 sauvegardés en gardant l'INI personnel. [Preuves, limites, nettoyage, critères et rollback](V1_49_DASH_RECOVERY_CLEANUP.md). Les préparations ci-dessous sont historiques.

## 17 octobre 2026 — V1.49 : phase native de décélération et fin de dash

Nouvelle preuve : `5B0F40` initialise simultanément les timers224/228 ; `5B93F0` décrémente le premier et applique le facteur `max(remaining/duration,0)`. Les979observations historiques de dash au vtable27D31B8 ont ce virtuel de déplacement et le cleanup59C110. Le chemin `59B080 → 59B2D0 → virtuel2A8/59C110` gère fin, montage, état temporaire et restauration native du mouvement. Il est réutilisé après le Tick physique original, seulement après observation d'une phase positive puis épuisée, état local RUNNING, propriétaire/composant actifs réciproques et variante prouvée. Un seul hook, six signatures exactes ; zéro initial/vieux timer/activation remplacée ou expirée passent sans fin anticipée. Les captures anciennes ne prouvent pas encore la durée du gain en jeu.

Source V1.49 préparée : case/délai et Hotkey ActionRecovery ID3 réactivés sur ce module, coffre reporté. Retrait de cinq configurations sans consommateur, deux atomiques écrites sans lecture et compteur passif de chord ; la garde comportementale de forme ultime conserve TPSChargeChordDown.117clés actives, valeurs et48IDs conservés ; aucun effacement des lignes personnelles inconnues.57autres fichiers source inchangés. Les commentaires erronés ne sont plus utilisés comme preuves. Deux nouveaux tests du vrai module et de ConfigStore,24suites attendues. **EXPÉRIMENTAL, construction/validation utilisateur non encore annoncées.** [Preuves, risques, critères et rollback](V1_49_DASH_RECOVERY_CLEANUP.md).

## 16 octobre 2026 — complément Graviton, pitch et cible 3D

L'utilisateur ajoute **Graviton** et observe un tir déjà descendant au recentrage TPS par défaut. **PROUVÉ :** `ThirdPersonPitchDegrees=-12`, `pitch=basePitch+orbitPitch` P1/P2, recentrage `orbitPitch=0`, fournisseur projectile recopiant le pitch rendu quand ses gardes passent. Le test existant impose cette copie de -12 ; il ne teste pas l'alignement réel depuis la bouche. L'inventaire historique V1.46 expose Graviton au slot `+8E8 → 6A9F20`, et les callbacks de classe prouvent sa chaîne GravityShot → ChargeShot → DualPistols. Ses assets ont aussi Aim/Charge/Finish et des références à la capacité projectile native commune. Donc il faut examiner le départ de chaque phase, pas déduire une trace de tir de la seule présence du retour partagé.

Inclinaison mathématique -12° : composante verticale -0,208 avant gravité. **INFÉRÉ :** compatible avec la pente signalée. Contribution exacte du pitch, du point cible, de la bouche et du cache de capacité **INCONNUE en jeu** ; aucune nouvelle capture Graviton. Vitesse et gravité effectives ne sont pas déduites d'un nom d'asset ou d'un override absent. Cible retenue : point 3D du bon réticule, direction depuis la vraie bouche, solveur natif pour les arcs ; conserver haut/bas et paramètres personnels. Aucun défaut mis à zéro, aucune soustraction aveugle de 12°, aucun hook nouveau dans ce complément documentaire. [Audit et critères actualisés](V1_48_OPEN_REPAIRS_AUDIT.md). Les cinq exceptions de l'entrée précédente deviennent six avec Graviton ; les autres acquis restent acceptés.

## 16 octobre 2026 — audit V1.48 : roues coop et trajectoires natives

**Décision :** le reste du mod est accepté par l'utilisateur pour l'instant ; exceptions maintenues : roue d'équipement P1/P2, Charge Shot manette surtout Hotstreak/recul, Lava normal en cloche manette P1, visée native après dash, Action Recovery dash (coffre reporté). Les anciens tests généraux en attente sont historiques.

**Constats prouvés :** `RadialActive()` n'alimente que les logs ; les protections P1/P2 utilisent encore le binding de roue, pas un état HUD réellement ouvert et attribué à son joueur. Les références faibles `widget+218/+220` sont LocalPlayer/monde, pas contrôleur/monde ; le contexte propriétaire et la réciprocité ont été lus. Le getter natif de sélection `7983B0` retourne `widget+298`, initialisé à -1 ; index et visibilité Slate ne suffisent pas à prouver une ouverture. Charge Shot Finish utilise la classe projectile dont le retour identifié `59D280` peut obtenir une direction par `672760`, avec un chemin qui contourne le getter Look corrigé ; Lava retourne séparément sa rotation balistique en cache par `7BBF20`. Les conditions actuelles fondées sur le maintien du tir ne suivent pas toute la fin de charge. Cause exacte pendant les tirs signalés non capturée.

**Limites :** journal V1.48 avec événements P1/P2 mais sans identité d'action/impact ; inventaire ultérieur avec un seul contrôleur actif ; 974 lectures passives avec War/Tremor, aucun tir Charge Shot/Lava ou dash. Le champ caméra exploratoire mal aligné est exclu. `5B93F0` est du déplacement de dash, pas une fin d'action ; aucun déblocage ne doit être fondé sur cette confusion. Action Recovery n'a plus de consommateur gameplay, ses clés restent de compatibilité. Les deux atomiques de chord chargé écrits sans lecture et des commentaires trompeurs sont signalés pour nettoyage ciblé ; la fonction de détection du chord possède encore une garde utilisée en forme ultime.

[Audit complet, preuves natives, limites, objectifs et TODO](V1_48_OPEN_REPAIRS_AUDIT.md). **Documentation seulement**, aucun hook/source/INI/build modifié. Base à conserver : V1.48 acceptée hors exceptions. Les essais suivants isoleront roue par joueur, Charge Shot, Lava, visée native puis Recovery ; pas de réouverture des acquis sans nouvelle régression.

## V1.48 — retour utilisateur du 16 octobre 2026

**VALIDÉ PAR L'UTILISATEUR, pour les fonctions explicitement testées :** réduction de la baisse Hotstreak avec saisie manuelle ; Unlimited et remplissage Hotstreak ; remplissage des munitions ; les quatre nouvelles actions Hotkeys. Ces acquis sont à conserver. Ce retour ne précise pas de test P2/coop et ne valide pas globalement tous les correctifs cumulatifs V1.46–48.

**Lava normal en cloche, manette P1 : toujours défectueux.** Retour utilisateur : la trajectoire semble dépendre du viseur et de l'angle de caméra ; caméra horizontale, le projectile arrive presque aux pieds. Le symptôme est rapporté en jeu ; le lien causal précis avec le point3D/caméra reste à mesurer. Priorité à l'audit de la cible3D et du calcul balistique natif, sans correction arbitraire de yaw. Le faisceau Hotstreak n'est pas signalé. Visée native après dash et Action Recovery restent deux sujets distincts non résolus.

Les contrôles encore attendus portent sur le menu uniforme, l'angle solo Strife/War, la propriété des caméras coop/cheval/ultime et leurs réglages, les commandes P2, les premiers tirs, le retour de cinématique et les options gameplay P2/Unlimited santé-vol plané-sprint. [Liste actuelle et priorités](V1_48_HOTSTREAK_REFILLS.md). Aucune nouvelle build ni modification de gameplay dans ce reçu ; INI personnel, binaires installés et archives conservés.

## V1.48 — construite, contrôlée et installée : Hotstreak / refills (16 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation utilisateur en jeu requise.** Base cumulative V1.47 construite mais non installée, acquis validés V1.43 et suivants conservés. Source `b7b90d29314ce00229a0737004426cbb0375b28d`, archive debug `550b7784b4f1341521bc8f7113250e5177207232`. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38964259848) **SUCCESS,22/22 tests** ; huit avertissements historiques, aucun nouveau. Les tests des vrais services P1/P2 couvrent les nouveaux refills, le diviseur, Unlimited ON/OFF, les resets et les générations. Cela ne vaut pas validation en jeu.

Ajouts : **Reduce Hotstreak decrease**0–10 avec saisie manuelle/Default, **Unlimited Hotstreak**, **Fill Hotstreak**, **Fill ammunition**, quatre nouvelles actions Hotkeys appendues44–47 sans remplacer les anciens bindings. Réduction et Unlimited Hotstreak OFF par défaut ;0/1=natif,10=un dixième. Le module natif conserve délais/reset/activation et le plafond du Hotstreak ; les refills font une seule demande fraîche par joueur et contrôlent la relecture. Les119anciennes clés/valeurs et44actions, le calcul validé de consommation des munitions,27fichiers protégés et13fonctions centrales de visée/RawInput/XInput ont été comparés et conservés. V1.47 cumulative : présentation commune de toutes valeurs, échange Strife/War basé sur les types natifs1/2 et commandes d'équipement/TPS P2 avec états distincts, encore à vérifier visuellement.

[ZIP cumulatif V1.48 — téléchargement direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.48_HOTSTREAK_REFILLS_TEST.zip), **843312 octets**, SHA256 `6EDA1124513CC88903F8F0DFCC611BE7624024D831CD7FAB5E1043A5F6E1CE90`. Exactement ASI/dxgi.dll/INI/README à la racine, CRC et binaires AMD64, identité1.48 et122clés contrôlés ; empreintes CI/dépôt/téléchargement public identiques. **111 anciennes archives intactes**, main`2bc3122`, archive`10e0f22`, tag/release publiquev1.43`902861e`/id414771221 vérifiés inchangés. Aucune nouvelle release publique créée. Le README du ZIP immuable reste celui de la préparation.

Installation après vérification élevée des processus : le jeu était complètement fermé. Sauvegarde des quatre fichiers et du log : `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v148-20261016-224804`. ASI et dxgi installés conformes au ZIP ; **INI personnel et raccourcis préservés octet pour octet**, SHA256 avant/après `89647F8748749404EC06725B4EEC792BBC3CBA68B03C1639A9E7CBEDFF988A95`. Les nouvelles clés manquantes utilisent leurs défauts compilés OFF sans remplacer le fichier personnel. Le README installé est ensuite actualisé avec ce reçu. Aucun lancement automatique ni écriture externe de mémoire.

**Lava : l'utilisateur confirme le projectile normal en cloche, manette P1.** Son calcul natif balistique et sa rotation en cache ont été identifiés, mais **aucun correctif Lava n'est annoncé** ; la prochaine capture doit comparer cible3D, cache, entrée au relâchement et caméra. Le faisceau Hotstreak n'est pas signalé. **Visée native après dash NON RÉSOLUE ; Action Recovery INDISPONIBLE.** Test prioritaire : nouveaux refills/Hotkeys, diviseurs1/2/10 puis Unlimited ON/OFF en mission ; ensuite menu/échanges solo/équipement P2 et récapitulatif V1.46–47. [Preuves, limites, critères et rollback](V1_48_HOTSTREAK_REFILLS.md).

## V1.48 — Hotstreak et remplissage des munitions (source de TEST)

Base debug V1.47 construite, non installée/non validée en jeu ; les acquis validés antérieurs restent protégés. **SOURCE PRÉPARÉE / EXPÉRIMENTAL : compilation V1.48, paquet, installation et test en jeu restent à vérifier.** Ajouts : réduction de la baisse Hotstreak avec diviseur0–10, Unlimited Hotstreak, Fill Hotstreak, Fill ammunition ; quatre actions ajoutées à la fin de Hotkeys, sans remplacer les anciennes. Les deux nouvelles options sont OFF par défaut ;0/1=normal,10=un dixième. Menu curseur/saisie/Default commun. Trois nouvelles clés,122au total ;48IDs d'actions, anciens44inchangés.

Le Hotstreak utilise les fonctions natives de retrait/remplissage et leur plafond ; décroissance/coûts sont séparés des resets. Les remplissages sont exécutés une seule fois par joueur valide sur le thread du jeu, avec annulation après respawn/expiration et contrôle de relecture. P1/P2 ont leurs propres contextes. Le chemin de gains validé n'est pas changé.

**Lava, manette P1 : audit natif avancé, correction NON VALIDÉE et non annoncée.** La capacité Lava normale calcule et conserve sa propre rotation balistique ; elle ne passe pas par le retour commun des pistolets que corrige V1.46. La variante Hotstreak est un faisceau distinct. Les preuves et la prochaine capture sont documentées ; aucun nouveau hook de visée ni rotation du personnage n'est ajouté dans cette build. Visée native après dash NON RÉSOLUE ; Action Recovery INDISPONIBLE. [Preuves, objectifs, tests et rollback](V1_48_HOTSTREAK_REFILLS.md).

## V1.47 — compilation réussie, contrôles locaux du paquet (non installée)

Source `c23fb2e`, archive debug `876dae662aba20cf17e0361966437bad04844fa5`. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38940318540) **SUCCESS,21/21 tests**, huit avertissements historiques et aucun nouveau. ZIP **836866 octets**, SHA256 `61820C1D0AEDC4071C5624BC7653A43EA453E3906D9BDF3BB5EE0536F71CBA05`, quatre fichiers à la racine, CRC, binaires AMD64, version1.47, INI119clés et concordance du hash CI/dépôt contrôlés. **110 anciennes archives conservées.** La première tentative de compilation a échoué sur deux erreurs de types/const, corrigées avant ce succès. Ni installation ni validation utilisateur V1.47 annoncée.

Le téléchargement direct de contrôle a d'abord été refusé par le contrôle automatique d'autorisation : sa limite d'utilisation empêchait la revue, sans avis de danger. Après rétablissement de ce service, le même téléchargement soumis à revue a réussi : SHA256 public/dépôt/CI concordants `61820C1D0AEDC4071C5624BC7653A43EA453E3906D9BDF3BB5EE0536F71CBA05`. **Contrôle direct désormais effectué.** V1.47 reste le point de retour construit pour V1.48, non installé ; son README de préparation reste immuable dans l'archive.

## V1.47 — menu unifié et commandes TPS P2 (source de TEST)

Base V1.46 debug `ab65e8c`, acquis validés conservés ; **SOURCE PRÉPARÉE / EXPÉRIMENTAL, compilation et installation à vérifier**. Le retour utilisateur rejette encore le saut d'angle Strife/War et signale des commandes P2 manquantes. Capture en lecture seule : Strife=1, War=2 ; le filtre V1.46 rejetait War et l'ancienne caméra de blend pouvait consommer la nouvelle génération. Correction des deux causes sans écrire la rotation du personnage. Les tests emploient désormais les vraies valeurs natives.

Tous les réglages numériques partagent libellé/curseur/saisie/Default et une largeur adaptée à la fenêtre ; munitions et Unlimited reprennent la présentation commune. P2 réutilise la politique de maintien/stow de P1 avec un état privé, ainsi que les bindings configurés pour appui court/long TPS, vue temporaire et chord cheval. TPS ON/OFF reste une option partagée ; les vues et recentrages restent privés. **119clés,44actions, défauts et anciens raccourcis inchangés.** Tir projectile V1.46 conservé ; un lien avec les tirs latéraux n'est pas démontré. Visée native après dash NON RÉSOLUE ; Action Recovery INDISPONIBLE. [Preuves, tests à ne pas oublier, limites, TODO et rollback](V1_47_MENU_TPS_INPUT.md).

## V1.46 — construite, contrôlée et installée : coop / ressources / FOV (16 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation utilisateur en jeu requise.** Base debug V1.45 `1b35ad7` (ses bugs signalés restent la référence de comparaison), acquis solo V1.43 et potions V1.44 conservés. Source finale `baf7d3fb1e6b1d4635a81630a8d3262bd3ebd506`, archive debug `4808373a44b47ee382f84a2fd5a02ff622456632`. [Compilation Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38933030844) **SUCCESS, 21/21 tests**, incluant les vrais modules caméra P2, propriété cheval/ultime, ressources P1/P2, budget de vol plané/sprint, santé et rotation de départ du projectile. Les tests isolés remplacent seulement leurs frontières natives ; ils ne prouvent pas le résultat visuel. Analyse MSVC : huit avertissements historiques, aucun ajouté dans les nouveaux modules. La première tentative a échoué sur un nom du test cheval ; corrigée avant cette compilation et sans installation intermédiaire.

**Camera → Third Person → FOV coop** : réglage distinct du FOV solo, commun aux deux vues, curseur et saisie40–140°, Default110°. L'INI existant sans cette clé hérite du FOV TPS personnel. TPS FOV reste l'activation commune ; aspect ratio et viewport natifs conservés. Autres ajouts : saisie manuelle du diviseur de munitions0–10 ; **Fill health / Unlimited health / Unlimited glide / Unlimited horse sprint duration**, avec quatre nouvelles actions Hotkeys, Unlimited OFF par défaut. Les corrections expérimentales portent sur les propriétaires des caméras cheval/ultime, les options P2, l'angle conservé lors du changement Strife/War, le ViewTarget natif pour les scènes et la rotation de base des tirs Strife Camera Forward à la manette. Les huit fonctions principales RawInput/visée/axes/roues vérifiées restent identiques à V1.45.

[ZIP cumulatif V1.46 — téléchargement direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.46_COOP_RESOURCES_CAMERA_TEST.zip), **832350 octets**, SHA256 `89B4A051142E37213257C2843B4A229BF6D3BA47E524C2CE2CB2B9CF6173A745`. Exactement ASI/dxgi.dll/INI/README à la racine, binaires AMD64, version embarquée1.46,119clés et115 anciens défauts inchangés ; hashes CI/dépôt/téléchargement direct concordants. **109 anciennes archives intactes**, main2bc3122, archive10e0f22 et release publiquev1.43 id414771221 contrôlés inchangés. Le README du ZIP immuable est celui de la préparation ; le présent reçu décrit la livraison effective.

Installée après réponse « JEUX FERMER » et vérification effective des processus, avec sauvegarde des quatre fichiers et du log : `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v146-20261016-183856`. ASI/dxgi de destination conformes au ZIP ; **INI personnel préservé octet pour octet**, SHA256 `FD8C182119BAACC318B03BC77831D57E5662924C229D7A7A0E9258333A3F3FFC` avant/après, raccourcis conservés. Le README installé est actualisé avec ce reçu. Aucun lancement automatique, aucune écriture externe de mémoire ni changement des sauvegardes du jeu.

**Test prioritaire :** régler le FOV coop et vérifier les deux vues, puis revenir en solo ; tester séparément cheval et ultimes P1/P2, premier tir primaire/secondaire, options/remplissages P2 et ON/OFF des nouveaux Unlimited, roues et retour de scène. **Visée native après dash : NON RÉSOLUE. Action Recovery : indisponible.** Trajectoire réelle, scènes gardant une caméra attachée, santé face aux morts de script et armes d'ultime hors classe de pistolets restent des limites explicites. [Preuves, critères de réussite, TODO et rollback](V1_46_COOP_RESOURCES_CAMERA.md). Les entrées ci-dessous sont historiques.

## V1.46 — coop, caméra et ressources (source de TEST)

Base V1.45 debug `1b35ad7` / source `332cbc9`, issue de la base solo V1.43 et des potions V1.44. **SOURCE PRÉPARÉE / EXPÉRIMENTAL : compilation, paquet et installation restent à vérifier.** Les retours V1.45 rejettent encore la visée native après dash, la caméra montée/ultime en coop, plusieurs options P2 et certains déclenchements cinéma ; aucune validation globale V1.45 n'est revendiquée.

V1.46 reconnaît les caméras appartenant au cheval/à la forme active, utilise la pose du bon joueur et route gains, remplissages, Unlimited et coûts de munitions par deux contextes distincts. Le vrai `ViewTarget`/`PendingViewTarget` remplace le simple verrou « mode cinématique » ; une caméra de jeu prouvée n'est plus rejetée à cause de son angle/distance. L'angle TPS reste choisi lors d'un échange Strife/War dans le même monde. Une nouvelle correction vise le départ du projectile des pistolets de Strife au moment du tir, en mode Camera Forward à la manette ; souris, visée native, équipement et boutons gardent leurs chemins.

Ajouts demandés : **FOV coop TPS indépendant du solo**, commun aux deux vues, réglable dans Camera → Third Person (curseur, saisie et Default). Une ancienne configuration hérite du FOV TPS personnel ; aucun aspect ratio ni viewport modifié. Saisie manuelle du diviseur de munitions 0–10 ; **Fill health**, **Unlimited health**, **Unlimited glide**, **Unlimited horse sprint duration**, avec quatre nouvelles actions assignables dans Hotkeys, sans réaffecter les anciennes. Les trois nouveaux Unlimited sont **OFF** par défaut. La santé utilise le setter/plafond natifs des joueurs vivants ; le vol plané conserve son budget de durée tout en exécutant la physique et les sorties natives ; le cheval cesse de consommer son endurance de sprint et conserve récupération/conditions de départ. UI des jauges et potions montre aussi P2.

**Visée native après dash : NON RÉSOLUE. Action Recovery : toujours indisponible.** Les observations cheval/ultime prouvent les propriétaires, pas la réussite visuelle de V1.46. Les nouvelles fonctions et la trajectoire réelle exigent un test utilisateur. [Preuves, limites, objectif, tests et rollback](V1_46_COOP_RESOURCES_CAMERA.md). Les archives antérieures et les branches main/archive restent protégées ; aucune release publique V1.46 demandée.

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
