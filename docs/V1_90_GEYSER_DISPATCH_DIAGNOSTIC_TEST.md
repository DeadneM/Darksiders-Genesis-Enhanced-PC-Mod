# V1.90 — Geyser : trace d’exécution, sans correctif de gameplay

**V1.90 INTERMÉDIAIRE NON RETENUE pour le diagnostic comparatif : [53/53 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40015555280), zéro erreur et zéro avertissement MSVC, mais filtre War erroné (type 0 au lieu du type natif 2) découvert au second audit. Remplacement V1.91 préparé ; Geyser sur Strife NON CORRIGÉ.** [Archive immuable conservée](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/cc3932ad985ba779d47f60b3f2865e01d5970239/downloads/DarksidersGenesis_V1.90_GEYSER_DISPATCH_DIAGNOSTIC_TEST.zip) · [Reçu et limite](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_90_DELIVERY_RECEIPT.md).

Base validée V1.87 `c20319f93413f849a41b10fdfd96f7842a172ac6` pour les cas rapportés ; cumul V1.89 `6921fb7a19dc93bff331f937299d5fbe0f020a2e`. Geyser V1.89 rejeté pour le transfert vers Strife, pas de validation globale ni P2 déduite. Objectif unique : distinguer notification mise en file, appel Blueprint effectif et activité d’un acteur d’attaque. Réussite du diagnostic : journal avec une paire entrée/sortie de SpawnBlast pour War et Strife, ou absence de cette paire identifiée comme limite à poursuivre. **Cela ne vaut pas réparation du pouvoir.**

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

Rollback : remettre Native, ou restaurer l’ASI V1.87 validé en conservant l’INI personnel. Aucun remplacement automatique des fichiers chargés et aucune relance. 154 archives antérieures conservées ; nouveau ZIP à quatre fichiers racine (ASI, dxgi.dll, INI, README), debug seulement. main/archive/v1.43 restent intacts. Prochaine étape : lire cette trace puis corriger seulement le maillon démontré, ou revoir l’approche si ce passage reste non observable ; ne pas empiler un autre remplacement d’animation.
