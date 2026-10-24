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

## V1.75 — construite et contrôlée, installation manuelle

21 octobre 2026. **CONSTRUITE / CONTRÔLÉE / NON INSTALLÉE ; nouvelles fonctions EXPÉRIMENTALES, non validées en jeu.** Base V1.73 validée dans les essais rapportés, canal debug. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39537334409) : 45/45 suites, zéro avertissement MSVC. Visée seule manette avec un/deux boutons et nouveau candidat de déplacement après réception lourde automatique ; les deux sont OFF par défaut. Aucun ancien Recovery du dash réactivé.

[ZIP cumulatif public vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/a816cc3db3c446fac407b93dc881058c19e7e43d/downloads/DarksidersGenesis_V1.75_PAD_AIM_HEAVY_LANDING_TEST.zip) : 836203 octets, SHA256 `B6D1F0CE0D3ACE5B5CF27A3C75373778707CC84EA9FA41764B4036B5ABB6EE08`. Quatre fichiers racine/CRC/AMD64 ; 156 valeurs INI antérieures, 55 ID Hotkeys et 139 anciennes archives conservés. main/archive/v1.43 inchangés. Installation manuelle demandée ; V1.73 reste installée, aucun fichier du jeu remplacé.

À tester ensemble : maintien/relâchement de visée native/TPS, personnages et P2 ; réception lourde OFF/ON après haute chute (Strife puis War) ; contrôle court dash/tirs spéciaux/roues/rangement. Efficacité Recovery encore inconnue, War_HeavyLand non observé dans la capture. [Reçu, intégrité, essais et rollback](V1_75_DELIVERY_RECEIPT.md).

**Suite :** adaptation des six capacités, OFF complet par pistolet. Ni sélecteur inerte ni OFF partiel livré. HUD en surveillance, autres FX P2 reportés, notification de mise à jour à confirmer. [Preuves](V1_75_WRATH_HEAVY_LANDING_PISTOL_RESEARCH.md). Les sections suivantes sont historiques ; leurs statuts avant construction ne remplacent pas ce reçu.

---

## V1.74 — construite et contrôlée, installation manuelle reportée au prochain lot

Base cumulative V1.73, ses trois essais rapportés validés. Nouvelle visée manette sans tir : **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE**, pas validée en jeu. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39498950576) : **44/44 suites, zéro avertissement MSVC**. Source `61d9d4b01405ceb45ae802c348cce6dcfaf6a053`, archive `53e2b1645f83e2449eea98d01d3f19bc39ab921f`. [ZIP cumulatif vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/53e2b1645f83e2449eea98d01d3f19bc39ab921f/downloads/DarksidersGenesis_V1.74_NATIVE_PAD_AIM_TEST.zip), 831912 octets, SHA256 `578DC4AC279472C28556F7B930AFCB2FB6C4E6D53D84E4E8B91616A7AC29AA6B`. Quatre fichiers racine, CRC, AMD64, 156 clés fournies, 55 ID Hotkeys conservés ; les 154 anciens réglages gardent exactement leurs valeurs. Les 138 anciennes archives, main/archive, tag v1.43 et fichier de reprise préexistant sont inchangés.

Le README du ZIP contient la préparation V1.74 avant CI sous le titre historique de sa base V1.73 ; le présent reçu et la version compilée V1.74 font référence. L'archive publiée reste immuable. Une tentative d'installation s'est arrêtée dès le contrôle GAME_RUNNING, **avant sauvegarde ou remplacement**. L'annonce précédente de fermeture déduite d'une lecture sandbox était erronée : le contrôle final avec les permissions d'installation a vu le processus actif. Aucun fichier du jeu remplacé. L'utilisateur précise ensuite qu'il installera **manuellement à la fin de sa session** et demande de regrouper davantage de travail ; ne plus demander sa fermeture ou installer automatiquement ce lot.

Suite : capacités et réception lourde, puis OFF pistolet selon preuves. Choix demandé : trois emplacements par personnage, Native ou n'importe laquelle des six capacités. Chaque Blueprint GetWrathAbilityById ne reconnaît que ses propres trois ID ; les six montages utilisent deux squelettes distincts. Le remplacement de FName seul serait inopérant. Ne pas livrer six options présentées fonctionnelles avant adaptation et contrôles des animations/composants. [Recherche du prochain lot](V1_75_WRATH_HEAVY_LANDING_PISTOL_RESEARCH.md).

Capture VM_READ achevée : **7 480 échantillons, trois lectures incohérentes rejetées**, six capacités natives des deux personnages relevées. Trois HeavyLand de Strife au sol, position verticale stable, durées observées d'environ 0,9–1,2 s ; aucun HeavyLand de War capturé. Rampage Slam et AirDash Land de War sont distincts. Clarification utilisateur : Recovery vise **la réception lourde automatique après chute**, pas l'attaque plongeante. Pas encore de nouveau correctif Recovery construit ou testé.

---

# Ajouts actifs — V1.74 en préparation

- **Visée sans tir manette : EN COURS.** Deux entrées, maintien, Native AIM/AIM_RELEASE ; OFF par défaut. Fonction à construire et tester sur base V1.73.
- **Capacités de Courroux : RECHERCHE.** Trois choix indépendants par personnage ; Native ou l'une des six capacités, choix libre confirmé. Dépendances de classes/animations/propriétaire à vérifier avant implémentation.

La file historique ci-dessous est conservée ; OFF pistolet et ground slam restent ouverts, autres FX P2 reportés.

---

# File active — 20 octobre 2026, après le retour V1.73

Branche **debug** uniquement. V1.73 est construite, contrôlée et installée. L'utilisateur répond **« 1. marche / 2. marche / 3. marche / prêt »** à la suite des trois essais V1.73 : conservation du choix des montures au chargement, réglages DOF et contrôle visée/tir/dash. Ces essais sont **VALIDÉS par l'utilisateur** ; aucun cas supplémentaire non décrit n'est supposé testé.

## Travail fonctionnel restant

| Sujet | État et résultat attendu |
| --- | --- |
| OFF indépendant pour chaque pistolet | **OUVERT.** Retirer modèle, effets de cette arme et bras tendu ; conserver projectiles et gameplay. Les choix de modèles gauche/droite et les holsters sont validés. Le nom Shoot_Right ne désigne pas la main : certains montages font tirer les deux armes. Aucun OFF partiel livré. |
| Nouvelle approche Action Recovery | **À REPRENDRE APRÈS LE RESTE.** Commencer par le ground slam, conformément à la décision utilisateur. Le candidat dash sans bénéfice est retiré. Les anciennes demandes dash/coffres restent des pistes ultérieures, sans correctif actif ni résultat annoncé. |

## Reporté ou seulement à confirmer

| Sujet | Décision |
| --- | --- |
| HUD général après refus d'arène verrouillée | **SURVEILLANCE / NON REPRODUIT en V1.73.** L'utilisateur confirme HUD toujours visible. Relevés avant/après complets, sans erreur ; nouvelle racine visible/opacité1, bShowHUD vrai, pile UI vide. Aucun correctif dédié, cause historique inconnue. Reprendre seulement si le défaut revient. |
| Autres icônes au-dessus des personnages et effets de tir plats/mal orientés en P2 | **REPORTÉS expressément par l'utilisateur.** Conserver la correction validée des indications d'interaction ; identifier les autres éléments avant d'intervenir. |
| Autres éléments HUD supposés manquants | Signalement incertain, lié à l'étude HUD ; aucun élément précisément identifié à restaurer. |
| Auto check update | Fonction déjà codée : vérification au démarrage du canal choisi, notification temporaire et rappel jaune. **Retour visuel spécifique encore absent** ; reste un essai, pas une fonction à recoder d'office. |

## Acquis à préserver

Caméra TPS/native, souris/manette et réaffectations ; profils pied/cheval/ultime et coop ; visée/tir/dash ; ressources, munitions/Hotstreak, vie/potions ; grappin, sauts/plané/sprint ; rangement et trois délais ; modèles des pistolets et holsters ; skins bonus et six variantes de progression ; durée ultime/infinie ; Léviathan ; flou et indications d'interaction ; skip warning et mises à jour. Les validations historiques restent bornées aux essais rapportés.

Le nettoyage du code, le retrait des diagnostics devenus inutiles, la cohérence du menu et la documentation accompagnent chaque correction. Ils ne doivent pas réintroduire les candidats rejetés ni modifier les raccourcis personnels.

Préserver INI, raccourcis, PAK, sauvegardes, main/archive et tous les anciens ZIP. Regrouper les changements cohérents ; aucune nouvelle build de diagnostic seule. [Audit OFF/HUD](AUDIT_PISTOL_OFF_HUD_2026_10_20.md) · [État courant](CURRENT_STATE.md) · [Historique intégral de la précédente file](history/ACTIVE_QUEUE_THROUGH_AUDIT_2026_10_20.md).
