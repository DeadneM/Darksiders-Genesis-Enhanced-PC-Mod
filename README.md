## V1.92 — prototype Caltrops vers War au sol

24 octobre 2026. **EXPÉRIMENTAL NON TESTÉ EN JEU ; compilation et contrôles V1.92 à consigner après CI.** Base expressément validée V1.87 pour les cas rapportés, acquis antérieurs conservés ; base cumulative V1.91 `d974c18`, dont le diagnostic Geyser **n’a pas été testé**. L’utilisateur autorise la poursuite des autres sujets sans pouvoir lancer le jeu : cet accord ne constitue pas une validation fictive. Geyser sur Strife reste NON CORRIGÉ ; P2 reste reporté.

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

## V1.79 test — World Ender natif, bras OFF, épées et saut monté

**V1.79 CONSTRUITE / CONTRÔLÉE : [48/48 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39683569838), zéro avertissement MSVC. Nouveaux comportements EXPÉRIMENTAUX, non validés en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/7f579ed3f068ff1cba6f25a7a6ba680ba5759517/downloads/DarksidersGenesis_V1.79_NATIVE_WORLDENDER_BLADES_MOUNTED_JUMP_TEST.zip) · [Reçu, empreintes et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_79_DELIVERY_RECEIPT.md). Les 143 anciennes archives et les 163 anciens réglages sont conservés. Installation manuelle, garder son INI.

**Lot EXPÉRIMENTAL, base cumulative V1.78, debug seulement.** Bras gauche/droit OFF traités sur la même base, côtés de holsters inversés, choix séparé pour masquer les épées de Strife. World Ender retrouve son axe natif : la correction des faisceaux ordinaires ne doit plus le diriger à l'opposé des canons selon la caméra. Nouvelle case **Jump on horseback**, OFF par défaut, utilisant l'action de saut réaffectable du jeu et le mouvement natif des montures. La visibilité de World Ender à caméra proche et les nouvelles animations restent à confirmer en jeu.

Voir [objectif, preuves, limites et liste des essais](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_79_NATIVE_WORLDENDER_BLADES_MOUNTED_JUMP_TEST.md). Compilation, contrôles et empreintes seront consignés dans le reçu de livraison ; ils ne constituent pas une validation en jeu. Installation **manuelle**, conserver son INI. Les groupes fermés par défaut, Never, maintien OFF, menus/manette et les réglages existants sont conservés. Coop différée, capacités étrangères, réception lourde et suivi vertical des exécutions restent ouverts.

---

## V1.78 test — Darksiders Genesis Enhanced PC Mod

**CONSTRUITE / CONTRÔLÉE : [47/47 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39669799213), zéro avertissement MSVC. Nouveautés EXPÉRIMENTALES, non validées en jeu.** Base cumulative V1.77, avec les retours utilisateur du 21 octobre : visée sans tir, réglages et updater validés dans les essais rapportés ; OFF avec pistolet droit visible validé à pied. Réception lourde incertaine, coop reportée. [Objectif, preuves, limites et essais](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_78_PISTOL_OFF_LIFECYCLE_CLEANUP_TEST.md).

Lot : choix OFF indépendant du cache de pose, maintien pendant les roues et au retour de l’ultime ; masquage des modèles en main et rangés à cheval ; candidat pour reposer aussi l’épaule droite lorsque seul le pistolet gauche est visible ; masquage du holster à pied du côté OFF dans la seule copie de rendu. Projectiles, sockets et animations de gameplay conservés. Les bras et holsters en cuir du modèle monté restent natifs : aucune validation visuelle supplémentaire n’est supposée.

Le menu propose désormais **Never (always visible)** pour le trait/arc, et ses groupes démarrent repliés. Les choix déjà ouverts restent ouverts pendant la session. Tous les réglages fournis et les raccourcis sont conservés. Nettoyage issu des deux audits : **167 lignes nettes de code inutilisé retirées**, sans gain de FPS ou de taille binaire revendiqué.

**World Ender reste ouvert en TPS.** Le rayon et son effet natifs sont créés et infligent des dégâts ; l’utilisateur voit le rayon en vue du dessus seulement. Le masquage trait/arc n’explique pas le défaut. Aucun hook de rayon ajouté sur cette hypothèse. Les six capacités entre personnages, la réception lourde et le suivi vertical pendant les exécutions restent dans la file active.

[ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/08a5a2ace26e80ea8950a31996ca7fed6650a8ad/downloads/DarksidersGenesis_V1.78_PISTOL_OFF_LIFECYCLE_CLEANUP_TEST.zip) · [Reçu et intégrité](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_78_DELIVERY_RECEIPT.md). Installation **manuelle**, conserver l’INI personnel. Aucun fichier du jeu, PAK ou sauvegarde remplacé par l’agent. Les sections suivantes conservent l’historique de leurs propres versions.

---

## V1.77 test — Darksiders Genesis Enhanced PC Mod

[Audit de maintenance comparé](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/AUDIT_DOUBLE_NETTOYAGE_V1_77.md) : patch de nettoyage préparé pour le prochain lot, non appliqué ; V1.77 reste inchangée pendant les essais.

Lot cumulatif debug **CONSTRUIT / CONTRÔLÉ : 47/47 suites Windows x64, zéro avertissement MSVC**. Nouveau panneau Gameplay → **Wrath ability slots** : trois choix indépendants par personnage, parmi ses trois capacités natives. Tous les choix fournis valent Native. Le jeu conserve lancement, déverrouillage, coût et animations. **EXPÉRIMENTAL, non validé en jeu.**

**L'échange libre des six capacités reste à terminer.** Les pouvoirs étrangers nécessitent l'adaptation des animations et composants. Les indications de roue peuvent encore décrire l'emplacement initial. Candidats visée sans tir/réception lourde V1.75 et OFF à pied V1.76 repris sans modification, toujours à tester. Base validée V1.73 dans les essais rapportés.

[Télécharger le ZIP V1.77 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b5da4721104b0b5a08911bf8cc51af88a4559f6f/downloads/DarksidersGenesis_V1.77_NATIVE_WRATH_SLOTS_TEST.zip) · [Reçu et intégrité](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_77_DELIVERY_RECEIPT.md) · [Essais groupés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_77_NATIVE_WRATH_SLOTS_TEST.md). Installation manuelle ultérieure, garder l'INI personnel et ses raccourcis ; les six nouvelles clés absentes restent Native. Aucun fichier du jeu remplacé par l'agent. Les sections suivantes décrivent les versions précédentes et leurs statuts historiques.

---

## V1.76 test — Darksiders Genesis Enhanced PC Mod

Lot cumulatif debug, **CONSTRUIT / CONTRÔLÉ : 46/46 suites Windows x64, zéro avertissement MSVC**. Nouvelle option **OFF (foot, experimental)** par pistolet de Strife dans Gameplay → Appearance. **Non validée en jeu.** Reprend la visée manette sans tir et le Recovery de réception lourde V1.75, encore à tester. Base validée V1.73 dans les essais rapportés.

Pour OFF à pied : ranger les armes puis rester immobile brièvement ; le menu indique quand la pose est prête. Le candidat masque le pistolet choisi et repose son bras pendant la visée, avec tirs actifs. Retour Native réversible. Mêlée, cheval et ultime restent natifs. Les effets connus propres à une main sont filtrés ; certains effets communs restent avec un pistolet visible, puis sont retirés avec les deux OFF. **OFF universel des effets/formes encore ouvert**, ainsi que l'échange libre des six capacités.

[Télécharger le ZIP V1.76 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/026664c3e06a85b3714af5591ff3bef135ff2cb1/downloads/DarksidersGenesis_V1.76_FOOT_PISTOL_OFF_TEST.zip) · [Reçu et intégrité](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_76_DELIVERY_RECEIPT.md) · [Essais groupés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_76_FOOT_PISTOL_OFF_TEST.md). Installation manuelle ultérieure : conserver l'INI personnel et ses raccourcis. Aucun fichier du jeu remplacé par l'agent. Les sections suivantes décrivent les bases précédentes et leurs statuts historiques.

---

## V1.75 test — Darksiders Genesis Enhanced PC Mod

Lot cumulatif **debug** sur la base V1.73 validée dans les essais rapportés : **visée sans tir à la manette** avec un ou deux boutons configurables, et candidat **Heavy landing movement recovery** pour reprendre le déplacement après une réception lourde automatique. Version **CONSTRUITE / CONTRÔLÉE**, 45/45 suites Windows x64, zéro avertissement MSVC. Les deux nouveautés sont **EXPÉRIMENTALES**, OFF par défaut et non validées en jeu. [ZIP vérifié, intégrité et reçu de livraison](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_75_DELIVERY_RECEIPT.md). [Objectif et essais groupés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_75_PAD_AIM_HEAVY_LANDING_TEST.md).

Gameplay → **Aim without firing** : un champ OFF laisse l'autre bouton fonctionner seul ; deux entrées actives nécessitent leur maintien ensemble ; OFF/OFF désactive la fonction. Le maintien appelle l'action native, sans simuler Ctrl ou ajouter de tir. En TPS, il suit la caméra du joueur ; chaque manette concerne son propre joueur. Les affectations natives restent actives. Relâcher après menu, changement de personnage ou reconnexion avant de viser à nouveau.

Gameplay → Player movement → **Heavy landing movement recovery (experimental)** : ouvre les deux verrous de déplacement après l'impact d'une chute assez haute, sans forcer l'arrêt de l'animation. Ce candidat vise seulement les HeavyLand normaux de Strife/War ; dash, attaques, Rampage, transformations et changements de personnage restent conservés. L'efficacité réelle et les effets doivent être vérifiés en jeu ; trois réceptions Strife observées, pas de War_HeavyLand dans la capture.

**L'échange libre des six capacités et le OFF complet par pistolet restent en étude et ne sont pas annoncés fonctionnels.** Les dépendances de squelettes, composants et rendu sont documentées. [File restante](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ACTIVE_QUEUE.md). Installation manuelle par l'utilisateur à la fin de sa session, INI personnel et raccourcis à conserver. Aucun fichier du jeu remplacé pendant la préparation.

---

### Acquis de la base V1.73

Canal **debug**, lot cumulatif : maintien de Despair/Rampage au chargement, distance DOF native/TPS jusqu'à **5 000 m**, nettoyage du Recovery du dash signalé sans effet. **CONSTRUIT / CONTRÔLÉ / INSTALLÉ** ; trois essais V1.73 confirmés par l’utilisateur : 43/43 suites Windows x64, zéro avertissement MSVC ; installation avec sauvegarde et INI personnel conservé. Chargement des montures, réglages DOF et contrôle visée/tir/dash confirmés dans les essais V1.73 rapportés. [ZIP immuable contrôlé](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/516db3d8cd898e9d544c571a2ba73fdfa38d070b/downloads/DarksidersGenesis_V1.73_MOUNT_DOF_CLEANUP_TEST.zip). [Critères et essais groupés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_73_MOUNT_DOF_CLEANUP_TEST.md).

La case Bonus mount skins règle la disponibilité du menu ; le bonus déjà équipé reste conservé même OFF, comme les armures. Le modèle de base reste sélectionnable et son choix n'est jamais remplacé automatiquement. Chargement des montures, réglages DOF et contrôle visée/tir/dash confirmés dans les essais V1.73 rapportés. Le flou V1.71 semble bon dans le dernier retour utilisateur ; les réglages DOF V1.73 sont également confirmés dans les essais rapportés.

Le candidat Dash movement recovery V1.71 est **REJETÉ dans l'essai utilisateur et retiré**, avec son état, sa checkbox et son réglage runtime. Les corrections validées de visée/tir pendant le dash sont conservées. Les anciens raccourcis Recovery restent inertes sans décaler les autres. V1.75 ajoute un nouveau candidat distinct pour la réception lourde automatique après chute, OFF par défaut et encore expérimental.

Archive vérifiée : `DarksidersGenesis_V1.73_MOUNT_DOF_CLEANUP_TEST.zip`, quatre fichiers racine. OFF complet des pistolets reste ouvert ; modèles gauche/droite validés conservés.

Suite en cours : [audit OFF des pistolets et HUD général](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/AUDIT_PISTOL_OFF_HUD_2026_10_20.md). Les montages de tir ne séparent pas les mains par leur nom de slot ; aucun OFF partiel ajouté. Essai V1.73 après refus d’arène : HUD toujours visible, relevés avant/après effectués ; défaut historique placé en surveillance.

### Installation et mise à jour

Placer les quatre fichiers à côté de `DarksidersGenesis-Win64-Shipping.exe`, dans `ProjectMayhem/Binaries/Win64`. Sauvegarder les fichiers déjà présents. Pour mettre à jour une installation existante, **conserver l’INI personnel et ses raccourcis** ; les réglages manquants prennent leur valeur par défaut compilée. Fermer le jeu avant de remplacer les fichiers chargés.

Le menu s’ouvre avec **Inser** dans le profil fourni ; cette touche est reconfigurable. Onglets : **Gameplay**, **Camera**, **Hotkeys**, **Updates**, **About**. Les valeurs numériques disposent des contrôles indiqués dans le menu ; utiliser les descriptions de chaque option pour son unité et le sens de son facteur.

Dans **Updates**, choisir le canal voulu, puis vérifier les mises à jour. Le changement de canal seul n’installe rien. L’installation à la fermeture et la restauration utilisent le système de sauvegarde intégré. **Auto check update**, facultatif et désactivé par défaut, vérifie au démarrage le canal choisi ; une mise à jour trouvée produit un avis temporaire puis un rappel jaune dans le menu. Son comportement en jeu attend encore un retour explicite.

### Fonctions et état

| Groupe | Contenu | État à retenir |
| --- | --- | --- |
| Caméra et commandes | TPS/native, souris relative, manette, échange des sticks, FOV coop, profils pied/cheval/ultime, cinématiques et roues | Chemins acquis conservés ; réglages et commandes reconfigurables |
| Visée et dash | Visée souris/manette, trajectoires, reprise AIM et retrait des tirs/sons/effets Standard parasites pendant le dash | Corrections confirmées dans les essais utilisateur, conservées |
| Netteté et interaction HUD | Focus TPS, taille et orientation des indications d’interaction P1/P2 | Validés dans les essais rapportés ; autres icônes/effets P2 reportés |
| Ressources | Gains, remplissages et Unlimited ; munitions, Hotstreak, vie, potions, ultime ; Standard limité | Fonctions validées conservées ; facteurs et descriptions propres à chaque réglage |
| Mouvement et grappin | Réglages de déplacement/saut/plané/cheval, sprint, portée native du grappin | Acquis conservés ; portée validée dans l’essai utilisateur |
| Rangement | Délais séparés lames Strife / pistolets Strife / épée War ; action commune avec raccourcis clavier et manette | Délais validés V1.65 ; holsters validés dans l’essai V1.66 |
| Sauts supplémentaires | OFF / After gliding / Always on | Sauts validés dans l’essai utilisateur V1.66 |
| Recovery | Ancien candidat dash retiré ; nouveau Heavy landing movement recovery pour réception lourde automatique | OFF par défaut, expérimental V1.75 ; efficacité à confirmer en jeu |
| Modèles 3D des pistolets | Choix indépendant du modèle droit ou gauche par main de Strife | Choix gauche/droite validés ; OFF complet encore ouvert |
| Skins | Deux montures bonus ; six variantes de progression dans deux groupes séparés | Sélection et OFF confirmés V1.68 ; sélection sans PAK confirmée ; maintien au chargement V1.73 confirmé dans les essais rapportés |

[État courant](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/CURRENT_STATE.md) et [file active](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ACTIVE_QUEUE.md) font référence pour les éléments restant à tester ou réparer. Une validation utilisateur porte sur l’essai rapporté, sans supposer tous les personnages, chapitres et états coop vérifiés.

### Apparences et durée ultime

Les choix **Native / Right pistol model / Left pistol model** par main de Strife sont conservés. Les holsters montés utilisent la variante rangée correspondante. Le futur OFF complet (modèle, effets et bras, avec projectiles conservés) n’est pas livré dans cette version.

**Bonus mount skins** et **Progression skins** restent indépendants et OFF par défaut. V1.67 interceptait à tort « équipé » ; V1.68 remplace ce branchement par le vrai « débloqué ». Aucun droit DLC, récompense ou bonus de gameplay accordé par le mod. Le jeu garde la sélection native ; fermer et rouvrir la page Skins après un changement. L’essai V1.68 de chevaux/armures et le retour natif OFF sont confirmés sans PAK cosmétique par le journal moteur. Les armures restent dans le dernier essai. Le défaut de chargement des montures a été localisé dans le filtre DLC natif ; V1.72 conserve les deux choix bonus déjà présents même lorsque leur disponibilité dans le menu est désactivée, comportement confirmé dans les essais V1.73 rapportés. Un PAK éventuellement réinstallé peut maintenir des montures disponibles avec l’option ASI OFF.

**Infinite Ultimate duration** bloque seulement le débit de la transformation déjà active, au niveau courant de jauge. Il ne remplit rien et ne rend pas l’activation gratuite. Décocher reprend le multiplicateur de durée si sa case est active, sinon le débit natif. Nouvelle action assignable dans Hotkeys, non attribuée par défaut. **Unlimited Ultimate** conserve son rôle de remplissage.

**Override Leviathan lightning**, décoché par défaut, rend le comportement natif. Une fois coché : **OFF / Effect 1 / Effect 2 / Effect 3**. Visuel seulement, sans accorder le cœur ou son bonus. Si le visuel choisi est indisponible, le menu l’indique et conserve l’effet natif. Pied/cheval/ultime et P2 à tester, ainsi que le retour natif sans doublon.

**TPS character focus** garde la méthode DOF du niveau. En TPS, la couche Gaussian proche est désactivée et la région nette protège la profondeur du modèle ; flou lointain, qualité et ouverture restent natifs. Clé INI `TPSLensFocus` conservée. OFF garde la correction de distance seule. V1.69 remplace le forçage CircleDOF qui rendait les décors trop nets. V1.70 conserve aussi la région nette du niveau tant que la caméra ne la surcharge pas déjà. Dans Camera → Depth of field, Native view et TPS view ont chacun Disable et Custom. Custom remplace explicitement le DOF par Gaussian ; distance0–5000m, quantité0–5% absolue (pas un facteur), saisie manuelle et Default. Distance10m et quantité0,6% sont inactives tant que Custom est OFF. Disable est prioritaire et garde les valeurs mémorisées. Les réglages DOF sont confirmés dans les essais V1.73 rapportés.

Léviathan s'attache à la racine comme le natif ; l'ancien rattachement au modèle de Strife était 97 unités plus bas. Hauteur et rangement du menu validés par l’utilisateur dans l’essai V1.69. **Appearance** et **Weapon sheathing** sont repliables et distinctes de **Player movement** ; réglages et raccourcis conservés.

La disparition du HUD après refus d’arène n’est pas reproduite dans le dernier essai V1.73 ; surveillance, sans correction dédiée ni réaffichage global forcé. OFF complet pistolet et nouvelle approche Recovery ouverts. [Essais et rollback V1.69](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_69_NATIVE_VISUAL_FOCUS_TEST.md).

### Raccourcis du profil fourni

Toutes les affectations suivantes sont reconfigurables et l’INI personnel reste prioritaire.

| Touches | Actions fournies |
| --- | --- |
| F1–F4 | Remplir toutes les jauges / Ultime / Synergie / Courroux |
| F5–F8 | Unlimited toutes les jauges / Ultime / Synergie / Courroux |
| F9 | Unlimited munitions |
| F10–F11 | Non attribuées |
| F12 | Afficher/masquer le HUD |

Les commandes de caméra native et de TPS ont leurs sections dédiées ; elles ne remplacent pas la configuration rebindable du jeu. Le rangement dispose aussi de raccourcis directement dans Gameplay, en plus des actions Hotkeys. **Les raccourcis de rangement dédiés sont non attribués par défaut.**

### Validation V1.66 et retour arrière

Retour explicite du 19 octobre 2026 : « pour les sauts c'est bon bravo », « pour les holsters c'est bon bravo », puis « donc tout a lair bon/ ». Ces deux sujets sont clos dans l’essai rapporté ; V1.66 devient la base de travail retenue. Ce retour ne décrit pas une matrice exhaustive de modes, personnages ou situations coop et ne confirme pas séparément Auto check update.

Le code validé est conservé. En cas de nouvelle régression, noter le contexte et garder le journal ; le système de restauration et les anciennes archives permettent un retour arrière en préservant l’INI personnel. Le [ZIP V1.66 conservé](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/12061a956b9257284c8b1779503cf058b75fab1f/downloads/DarksidersGenesis_V1.66_NATIVE_JUMP_CYCLE_TEST.zip) permet de revenir à cette base en conservant l’INI personnel.

### Sources, construction et historique

Source C++20/CMake, Windows x64 ; dépendances MinHook et Dear ImGui fixées par le projet. Le workflow [.github/workflows/build-v090-debug.yml](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/.github/workflows/build-v090-debug.yml) configure, compile, teste et préserve une nouvelle archive numérotée sur `debug`. Une archive publiée n’est jamais remplacée par des octets différents. La release stable n’est pas promue par une simple réussite de CI.

[Carnet technique cumulatif](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/TECHNICAL_NOTEBOOK.md) · [Prochaines étapes](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ROADMAP.md) · [Audit Recovery et modèles](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/NEXT_RECOVERY_AND_MODELS.md) · [Règles du projet](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/DEVELOPMENT_RULES.md).

Le README actuel sépare les informations actives des anciennes préparations. **L’ancien README intégral est préservé, octet pour octet, dans [l’historique jusqu’à V1.66](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/history/README.md)**. Les anciennes affirmations de statut et anciens raccourcis de cet historique ne décrivent pas la version actuelle. Les anciens ZIP restent inchangés.
