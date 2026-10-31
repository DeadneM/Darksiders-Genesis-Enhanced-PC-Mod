# V1.136 — chaîne native de ciblage TPS

31 octobre 2026. **CONSTRUITE / CONTRÔLÉE ; EXPÉRIMENTALE, À VALIDER EN JEU.** Base cumulative V1.135 ; remplace les derniers ajouts de ciblage, conserve les autres acquis. Aucun build local ni lancement ; installation locale ensuite demandée et vérifiée ci-dessous, INI personnel conservé.

Source `aa87712d800d4492423c623e77b6227707378350` ; archive `0f990303aa719ef42d341c85cc1fd0c5c28acb26`. [GitHub Actions Windows x64 Release / MSVC analyse](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41230908006) : **70/70 suites**, zéro avertissement/erreur MSVC. Avis de dépréciation des outils externes CMake/MinHook et Node Actions distincts, consignés dans la preuve CI. Les fixtures vérifient le retour de la recherche native sous le réticule, les scopes et arguments, la direction d'origine du sélecteur, la restauration des angles, la consommation synchrone du point et les vrais chemins projectile/release/Lava/faisceau libre. Ce ne sont pas des essais d'impacts ou de portée en moteur réel.

[ZIP public immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/0f990303aa719ef42d341c85cc1fd0c5c28acb26/downloads/DarksidersGenesis_V1.136_NATIVE_TPS_TARGET_PIPELINE_TEST.zip) : 2422129 octets, SHA256 `9F4DC29FD3F62A63224643A9F57BECFCC513E274228AE138F57AB52E0996BA93`. Quatre fichiers racine ASI/DXGI/INI/README, méthode0/CRC et binaires AMD64 vérifiés. INI/README comparés à la source compilée.200anciennes archives Git conservées,201au total. main/archive et tagv1.43 inchangés. L'updater debug sélectionne et télécharge les mêmes octets. ZIP et fichiers extraits contrôlés par Defender sans menace ; protections actives, aucun réglage/exclusion modifié.

## Remplacement effectué

Le helper souris2D natif retrouve sa recherche sous le curseur et ses filtres, sous le même rayon TPS pour sa déprojection directe et imbriquée. Le repli3D vers l'avant conserve les cas où un plan horizontal ne donne aucune direction utilisable. Le sélecteur garde ses arguments/direction d'origine. Le cache d'acteur/point250ms, son repli commun et la classification d'arme du faisceau libre sont supprimés. Les points assistés ne sont observés que pendant leur consommation réelle par l'appel natif courant, puis la portée se ferme immédiatement. Un faisceau libre ne reprend plus le point d'une autre action.

Le filtre de contact617B90 n'est pas remis. Les constantes de portée/rayon/poids/dégâts/chaînes ne sont pas ajustées. Le curseur global/par munition module seulement les angles natifs, avec OFF/100 sans écriture angulaire. Il peut toujours subsister un accrochage légitime du moteur. Le comportement complet et la cause historique unique restent non validés en jeu.

18fichiers comparés inchangés, dont caméra/commandes/RawInput/coop/modèles/holsters/capacités/config, et9corps de fonctions de propriété/physique conservés.228clés INI,55Hotkeys inchangés. Les fichiers actuellement installés ne sont pas remplacés par cette livraison. [Deux passes](evidence/V1_136_STATIC_REVIEW.json).

## Essai prioritaire

Garder l'INI. TPS souris, Strife à pied : caseOFF puisON100, caisse/pot proche avec ennemi latéral/lointain, tourner la caméra en maintenant Statique. Vérifier portée et chaînes, normal et Hotstreak. Comparer ensuite50/100/150 ; Standard/Charge/Lava puis vue native/manette, coop en dernier. Fermer normalement pour les bilans CURSOR_QUERY/NATIVE_ASSIST. Une correction observée dans les fixtures doit encore être confirmée par l'utilisateur.

Réglage angulaire : normal Strife à pied P1/P2 ; monture/ultime gardent leur acquisition native. Aucun input réaffecté. Rollback : ZIP V1.135 conservé, INI personnel. [Objectif et périmètre](V1_136_NATIVE_TPS_TARGET_PIPELINE_TEST.md), [CI](evidence/V1_136_CI_CHECK.json), [ZIP](evidence/V1_136_PACKAGE_CHECK.json), [updater](evidence/V1_136_UPDATER_CHECK.json), [scan](evidence/V1_136_DEFENDER_CHECK.json).

## Installation locale demandée le 31 octobre 2026

V1.136 installée depuis le ZIP contrôlé, jeu fermé, après sauvegarde V1.135 `before-V1.136-20261031-195714-d9cfabb3`. ASI/DXGI/README remplacés puis relus indépendamment ; anciennes copies ASI/DXGI/README/INI/journal sauvegardées et vérifiées. INI personnel identique octet pour octet et raccourcis conservés, EXE original identique ; aucun PAK écrit (noms/tailles/dates UTC inchangés). Jeu non lancé ; correction toujours expérimentale jusqu'au retour utilisateur. [Preuve](evidence/V1_136_INSTALL_CHECK.json).
