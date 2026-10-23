# V1.91 — trace Geyser corrigée, gameplay encore non fonctionnel

**V1.91 CONSTRUITE / CONTRÔLÉE : [53/53 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40016586657), zéro erreur et zéro avertissement MSVC. DIAGNOSTIC seulement ; Geyser sur Strife NON CORRIGÉ, non validé en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/0be129010adbb857e0d3de6bbcdd32b3d7022c1b/downloads/DarksidersGenesis_V1.91_GEYSER_DISPATCH_DIAGNOSTIC_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_91_DELIVERY_RECEIPT.md).

Base validée V1.87 `c20319f93413f849a41b10fdfd96f7842a172ac6` pour les cas rapportés ; cumul V1.90 archive `cc3932ad985ba779d47f60b3f2865e01d5970239`. Geyser sur Strife reste REJETÉ après V1.89. Objectif : obtenir une trace effective du même SpawnBlast sur War natif puis Strife. Réussite du diagnostic : paire entrée/sortie sur chaque personnage, ou limite d’observation explicitement constatée ; aucun succès de gameplay supposé.

Second contrôle V1.90 : filtre War=0 et fixture reprenant cette erreur ; 53 tests passaient mais la comparaison War réelle aurait été exclue. V1.91 corrige **War=2**, cohérent avec le chemin natif Map de WrathSlotFeature (Strife=1/War=2), ajoute le rejet War=0 et vérifie le cache hors thread. Aucun masquage d’avertissement ; archive V1.90 conservée et documentée comme intermédiaire non retenue. Le montage Geyser n’est pas modifié. Nouvelle trace à retirer après diagnostic.

## Résultat utilisateur et preuves de comparaison

- Strife : pose figée pendant le cast, aucun dégât ni consommation constatés par l’utilisateur. Native fonctionne ; P2 plus tard.
- Deux captures en lecture seule : montage compatible de Strife avançant normalement, environ 2,37 s ; Geyser seul actif, pas de pouvoir concurrent interceptant la recherche du dernier pouvoir.
- Comparaison War natif puis Strife : notification native CallAbilityFunction/SpawnBlast présente dans les deux files vers 0,488 s ; les échantillons répétés de file ne sont pas comptés comme autant d’appels.
- Les deux slots d’animation sont TPS ; la suppression de notify par l’instance de Strife retourne false. Le getter canonique du composant de capacités correspond au champ suivi. Les quatre classes d’attaque sont des Blueprints de données dérivés de MayhemBlast ; aucune restriction explicite à War trouvée dans ce chemin.
- Ces relevés **ne prouvent pas l’exécution de SpawnBlast ni l’apparition du blast**. Scène relevée : Level_00/Néant, où l’absence de coût n’est pas une preuve suffisante de défaut. Pas de correctif de Courroux inventé.

## Changement isolé

Le hook ProcessEvent existant garde son ordre observation UI avant/appel natif/observation UI après. Une enveloppe dans ForeignWrath transmet exactement une fois les quatre arguments natifs. Filtre sur thread de jeu, classe Geyser vivante, fonction exacte SpawnBlast appartenant à cette classe, instance non CDO, propriétaire local réciproque et à pied. War et Strife acceptés pour comparaison ; aucun filtre de touche.

La fonction est recherchée dans les enfants de sa classe, jamais par une adresse d’instance persistée. Le contrat UClass/UField est déjà utilisé par l’audit de réflexion ; le jeu fermé ne permet pas de revérifier cette nouvelle recherche sur une instance actuelle. Le message TRACE ready indiquera sa résolution réelle dans le build.

Budget : huit appels Geyser par session, quatre événements imbriqués de blast par appel. Contexte propre au thread et restauré à la sortie, y compris exception native. Identités, propriétaire/instigateur et compte global d’objets lus seulement ; pas de décompte d’objets assimilé à une preuve d’attaque. Les événements de blast sont comptés, **pas les acteurs uniques**. Leur absence ne prouve pas qu’aucun acteur n’a été créé : un acteur de données peut n’émettre aucun événement intercepté. Un événement présent ne prouve pas les dégâts.

Le montage V1.89 et son résultat rejeté ne changent pas. Aucun timer/tick ni nouvelle interception, aucun Finish additionnel, aucune écriture de mouvement, coût, ressources, dégâts, CDO ou asset source. Ancien cast Stoneskin strictement conservé. Diagnostic temporaire à retirer une fois la cause établie. Les capacités natives restent le repli conseillé ; le transfert Geyser n’est pas une option fonctionnelle annoncée.

## Contrôles et essai groupé

Suite ForeignWrath étendue : transmission exacte de quatre arguments et appel unique, appels imbriqués et budget, scope/exception, comparaison War, classes/threads/propriétaires inéligibles, cache de fonction exact, absence de mutation des trois objets. Les contrôles de montage, politiques, INI et autres fonctions restent requis dans les 53 suites.

Après installation manuelle du ZIP, conserver son INI. Préparer Strife avec un emplacement Blade Geyser, puis faire une activation de Geyser avec War sur Native et une avec Strife, de préférence à pied en mission et hors menus. Revenir à Native ; transmettre le journal. Si les lignes TRACE ready/DISPATCH manquent, ne pas multiplier les activations. Aucun test P2 demandé.

Rollback : remettre Native, ou restaurer l’ASI V1.87 validé en conservant l’INI personnel. Aucun remplacement automatique des fichiers chargés et aucune relance. 155 archives antérieures conservées ; nouveau ZIP à quatre fichiers racine (ASI, dxgi.dll, INI, README), debug seulement. main/archive/v1.43 restent intacts. Prochaine étape : lire cette trace puis corriger seulement le maillon démontré, ou revoir l’approche si ce passage reste non observable ; ne pas empiler un autre remplacement d’animation.
