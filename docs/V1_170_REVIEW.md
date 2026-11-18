# V1.170 — lot cumulatif : arbre des cœurs et options d’image

Base validée : V1.168. V1.169 intermédiaire construite (85/85 tests), conservée intacte ; ses deux filtres restent à valider en jeu. La comparaison demandée du menu est arrivée après sa construction : V1.170 regroupe ce travail et le correctif d’affichage. Statut avant CI : PRÉPARÉ / EXPÉRIMENTAL. Main/release v1.168 intacts, aucun remplacement des binaires actuellement chargés par le jeu.

## Preuve du défaut des niveaux

Deux relevés du même processus V1.168, arbre cassé puis rétabli après retour/réouverture : 205 widgets comparés, zéro erreur de lecture. 30 CoreTreeNodeMinor conservent leurs données de niveau mais leurs ActivePip passent de Hidden à Visible/Collapsed. Une différence de focus du panneau d’informations accompagne la navigation ; elle n’est pas assimilée à un changement des niveaux possédés. Les relevés externes n’ont fait aucun appel moteur ni écriture mémoire.

Le Blueprint initialise les nœuds par Refresh → initializeData/populateSlot → UpdateSlottedVisual. Le défaut observé est un état visuel initial non rafraîchi ; sa cause exacte dans le cycle natif/mod n’est pas encore entièrement établie. Le correctif expérimental réutilise RefreshNodes, la fonction native déjà employée par la page, après l’échauffement initial du widget. Il ne force pas directement la visibilité des images et ne change aucun niveau, objet équipé ou donnée de sauvegarde.

## Correctif ciblé

Gameplay → System and startup → **Creature core level display fix**, ON par défaut dans cette beta ; OFF laisse le cycle natif.

L’observateur ProcessEvent existant est réutilisé, sans nouveau hook. Seule la classe CharacterPage_CreatureCoresFullscreen_C du package exact est acceptée. Les pointeurs de classe/fonctions et le widget ont des identités faibles contrôlées ; le lecteur natif commun crée une référence faible ordinaire si nécessaire pour les contrats. Les paramètres sont vérifiés : RefreshNodes = 0 octet ; le Tick original garde ses 60 octets et ses arguments inchangés.

Après deux Tick natifs terminés, RefreshNodes est invoquée **une seule fois par instance**, sur le thread du jeu, par le dispatch original. L’état est consommé avant l’appel pour éviter les réentrances. Cache borné à quatre pages, sans éviction d’objet vivant. L’objet détruit, une signature inconnue, une classe/fonction indisponible ou une option OFF ne provoquent aucun appel. Pas de scans globaux d’objets par frame, pas de timer ni touche imposée ; classification mise en cache. Aucun patch de script/cursor VM, de PAK ou de shader.

[Comparaison et contrat runtime](evidence/V1_170_CORE_UI_REVIEW.json). Les dumps de mémoire restent privés et locaux. Le rendu après correction n’a pas encore été validé en jeu.

## Options d’image cumulées

Camera → Image effects : **Disable motion blur** et **Disable chromatic aberration**, toutes deux OFF par défaut, indépendantes de TAA/DOF. Elles s’appliquent globalement, y compris aux deux vues locales et aux cinématiques, et restaurent les valeurs natives capturées en revenant à OFF. [Contrats et limites V1.169](V1_169_REVIEW.md).

255 anciennes valeurs conservées ; trois nouvelles clés en tout depuis V1.168 : deux filtres OFF et CoreLevelDisplayFix ON. Profil de référence : 258 clés, 55 actions Hotkeys inchangées. Aucun INI fourni/remplacé dans le ZIP. Correction de streaming SourceControl toujours non incluse : SoulBarrier est un orbe/cache d’âmes destructible de décor, dont le script appelle réellement SourceControlHelpers.IsEnabled ; supprimer cet appel à l’aveugle est écarté. Aucun gain de FPS promis.

## Tests après construction

1. Au premier accès à l’arbre après lancement/chargement, vérifier les indicateurs de niveaux sans revenir au menu précédent. Tester Strife puis War ; un bref passage d’initialisation d’environ deux ticks est possible.
2. Changer la sélection, équiper/déséquiper normalement un cœur, fermer/rouvrir : niveaux et progression doivent rester cohérents, navigation et sauvegarde normales. Si le correctif gêne, le désactiver dans System and startup. Le journal signale le rafraîchissement unique.
3. Native/TPS : comparer Disable motion blur ON/OFF pendant une rotation rapide de caméra ; vérifier le retour du rendu initial.
4. Comparer Disable chromatic aberration ON/OFF dans une scène avec franges colorées aux bords ; une scène sans effet natif peut montrer peu de différence. Tester les deux cases ensemble, TAA activé puis désactivé.
5. Un chargement et une cinématique : réglages mémorisés, rendu cohérent, caméra/HUD habituels conservés. La coop peut être vérifiée avec les deux vues si disponible.

Rollback : désactiver le correctif et les nouveaux filtres ; les deux binaires V1.168 validés restent disponibles. Ne pas installer V1.169 séparément pour ce lot : V1.170 est cumulative.
