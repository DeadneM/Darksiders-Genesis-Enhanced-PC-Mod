# V1.136 — native TPS target pipeline

31 octobre 2026. **CONSTRUITE / CONTRÔLÉE ; EXPÉRIMENTALE, À VALIDER EN JEU.** Base cumulative V1.135, dont le recentrage/cache reste contesté. Les autres acquis validés sont conservés. Objectif unique : remplacer les corrections ajoutées par une chaîne native cohérente entre recherche sous le réticule, sélection et consommation de la cible. Critère : Statique n'est plus attirée vers un ennemi latéral au détriment de la caisse/pot visé, sans raccourcir sa portée ou casser ses chaînes ; référence OFF/100 avant réglage.

## Remplacement de l'approche précédente

- Suppression de la copie persistante d'acteur/point, du délai250ms, du repli sur le finder commun et de la classification d'arme destinée au faisceau libre. Le point retenu par n'importe quel finder n'est plus imposé aux tirs suivants.
- Le sélecteur6C3520 reçoit désormais sa direction d'origine, ainsi que son propriétaire et son prédicat d'origine. ÀOFF/100, aucune écriture angulaire. L'option globale/par munition ne fait que moduler temporairement les deux angles natifs, puis les restaure. Rayon, poids, dégâts et données de chaînes ne sont pas modifiés.
- Le helper souris2D5719A0 est de nouveau exécuté au lieu d'être systématiquement remplacé par la trace projectile3D. Sa recherche native sous le curseur et ses vérifications de collision restent à lui ; seule sa déprojection utilise le rayon TPS existant, dans une portée synchrone liée au même contrôleur. Les sites571A77 et1AC95E6 sont ajoutés au garde de déprojection existant. Pas de nouvelles coordonnées souris ou de touche simulée.
- Si le plan natif ne fournit pas une direction finie vers l'avant (notamment rayon horizontal), le repli3D vers l'avant déjà validé est conservé. Pas de retour aux tirs arrière ou au sol liés aux anciennes intersections de plan.
- Pour les sorties projectile/capacité/Lava, le seul point assisté admis est celui que le calcul natif vient réellement de consommer dans CET appel : observation de66A620 depuis59D3BD, sans appel moteur supplémentaire de l'ASI. La portée se ferme dès le retour original. Aucune cible ne survit vers un autre tir, joueur, frame ou branche libre. La correction tardive des tirs chargés et la paire vitesse/angle Lava restent nécessaires et conservées.
- Statique garde son affectation native de cible et ses chaînes. Le filtre617B90 de V1.129/130 n'est pas réintroduit. Une branche libre ne peut plus emprunter le point ennemi d'une autre action.

## Pourquoi ce changement vise le défaut ancien

L'enquête avant le premier retour V1.126 a établi que le correctif V1.53 ne couvrait que la direction libre du faisceau, alors que son verrouillage provenait d'une recherche distincte. V1.136 rétablit la recherche native sous le réticule dans le même cadre TPS et respecte la consommation native de chaque cible. Elle ne tente pas de neutraliser le défaut par une portée réduite ou un nouveau filtre de contact. La responsabilité exacte dans le symptôme historique n'est toujours pas prouvée par un essai comparatif.

## Périmètre et limites honnêtes

Le réglage angulaire reste limité à Strife normal à pied P1/P2. Le chemin souris restauré est protégé par le fournisseur TPS déjà existant ; manette, vue native et contexte refusé restent sur leur chemin original. Aucun changement XInput/RT/RB/R3, RawInput/Alt-Tab, caméra, roue, pause, modèle/bras/holster, pouvoir transféré ou ressource. Les limites d'acquisition natives peuvent toujours produire un accrochage natif : cette build ne promet pas une absence totale d'aide à100.

Le relevé du point natif ajoute un hook à signature sur un résolveur pur déjà identifié, au lieu d'appeler ce résolveur sur un acteur mémorisé. Pas de scan, de nouvelle allocation d'identité, de mutation de cible ni de modification de composant. Le moteur continue à appeler son résolveur une fois ; l'ASI observe seulement la sortie pendant l'appel original vérifié. Les gardes des sites de déprojection et l'unwind de portée sont testés en fixture, pas en moteur réel.

Les228clés INI et55Hotkeys restent identiques. Aucun INI personnel ni fichier du jeu remplacé par l'agent. Compilation Windows x64 via GitHub Actions uniquement ; nouvelles archives numérotées conservées. main/archive protégées.

## Vérification et essai demandé

Fixtures ciblées : recherche2D native prioritaire, même rayon dans sa recherche imbriquée, arguments inchangés, repli horizontal/arrière, autre contrôleur/site hors portée, nettoyage sur exception ; sélecteur appelé une fois avec sa direction d'origine et angles restaurés ; point consommé uniquement par son appel natif, imbrication et tir suivant sans héritage ; sorties réelles projectile/release/Lava et faisceau libre. Ces contrôles ne prouvent pas le comportement final en jeu.

Installation manuelle ou updater debug en gardant son INI. Dans la même scène TPS souris, commencer caseOFF puisON100 : pot/caisse proche devant un ennemi, puis ennemi latéral et éloigné. Tourner la caméra pendant Statique ; vérifier portée ressentie et chaînes, normal et Hotstreak. Comparer ensuite50/100/150, puis Standard/Charge/Lava. Vérifier vue native et manette sans changer leurs réglages ; coop en dernier. Quitter normalement pour les bilans CURSOR_QUERY/NATIVE_ASSIST. Rollback complet : ZIP V1.135 conservé avec son INI. Ne pas annoncer le bug corrigé avant ce retour.

Livraison vérifiée : [70/70 suites, zéro avertissement MSVC](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41230908006), [ZIP cumulatif](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/0f990303aa719ef42d341c85cc1fd0c5c28acb26/downloads/DarksidersGenesis_V1.136_NATIVE_TPS_TARGET_PIPELINE_TEST.zip), [reçu et limites](V1_136_DELIVERY_RECEIPT.md). Aucun essai en jeu ni installation par l’agent.
