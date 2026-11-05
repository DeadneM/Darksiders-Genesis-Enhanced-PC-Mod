# V1.154 — choix des effets natifs de poussière de dash

Le lot est maintenant **CONSTRUIT et CONTRÔLÉ** : 81/81 tests Windows GitHub, aucun avertissement de compilation détecté. Le rendu reste **EXPÉRIMENTAL en jeu**. [ZIP et reçu final](V1_154_DELIVERY_RECEIPT.md).

Base : V1.153 acceptée, promue sur main au commit `563ae9ca8c2647689af8464d320544afe55b1894` avec le ZIP existant. Objectif de cette bêta debug : sélectionner les rendus de poussière déjà présents dans les notifications natives, sans changer le dash validé. Réussite attendue : le choix est visible pour Strife et War dans les deux vues, Native retrouve les surfaces d'origine, Hidden masque ces poussières, et l'ancien masquage reste effectif.

## Apparence et compatibilité

Appearance → Effect appearances → Other effects → **Ground dash dust (both heroes)** : Native, Dry dust, Coins, Metal sparks, Sand, Snow, Stone debris, Water, Wood debris, Hidden. Une seule nouvelle clé : Values.DashSurfaceAppearance, défaut 0 (Native).

Les huit exports ParticleSystem et leurs packages restent ceux de la bibliothèque existante. Leurs usages dans les tables m_SurfaceOverrides ont été vérifiés pour les deux héros : les noms Strife ne signifient pas que War ne les emploie pas. [Inventaire et preuves natives](NATIVE_APPEARANCE_RESEARCH_2026_11_05.md).

La nouvelle famille est ajoutée à la fin : index 45, rôle 100 ; tous les anciens numéros de familles et cibles sont conservés. Seuls les huit assets de poussière sont affectés au nouveau rôle, sans changer leur index ou package. Les 460 assets restent les mêmes. Aucun fichier de particules du jeu n'est distribué.

L'ancien Other Strife effects = Hidden garde la priorité, y compris pour la poussière utilisée par War. L'interface indique de remettre ce réglage sur Native pour voir les nouveaux choix. La visibilité des composants déjà masqués est restaurée selon la sélection effective : passer d'un masquage hérité au masquage explicite ne les réaffiche pas par erreur. Les chargements ciblés utilisent cette même sélection, évitant de charger un remplacement quand le masquage global le rend inutile.

## Chemin conservé et limites

Réutilisation de la fabrique native déjà interceptée, avec un seul appel par effet émis, même ABI, même cycle de vie et mêmes paramètres d'origine. Aucun nouveau hook, aucun changement d'animation, de coût, de dégâts, de mouvement, de caméra ou d'input. Les ressources expirées gardent le repli natif et le chargement ciblé borné existants. Les effets de flou, de réception, de chute et de corps sont exclus de cette sélection. Léviathan et les autres apparences gardent leurs réglages.

Les animations peuvent déjà émettre deux couches de poussière. La variante sélectionnée est appliquée à chaque émission correspondante ; le nombre d'émissions n'est pas modifié. Une intensité ou une superposition différente reste donc possible et doit être évaluée en jeu. On ne supprime pas une émission native pour corriger une impression visuelle sans observation.

## Contrôles et essais

Les tests existants sont étendus : 64 couples source/cible de poussière, pas d'interférence avec les flous Strife/War ou les effets de corps, Native exact, cible expirée, ancien INI, priorité du masquage et restauration lors du changement entre masquages. Le test de configuration vérifie le défaut, la persistance du choix et l'indépendance de l'ancienne clé. Les anciens mappings sont comparés par nom et index avant publication.

Statut initial : **EXPÉRIMENTAL**, compilation GitHub et rendu en jeu à vérifier. Aucun build local, aucun lancement ou remplacement des fichiers du jeu par l'agent. La livraison cumulera V1.153 et ces nouveaux choix uniquement ; main restera sur V1.153.

Test groupé : à pied avec Strife puis War, comparer Native / Water / Metal sparks / Snow / Hidden en vue native et TPS, puis les autres choix. Revenir à Native, changer de niveau et vérifier la sélection sauvegardée. Si Other Strife effects était sur Hidden, vérifier qu'il masque toujours et le remettre sur Native pour comparer les variantes. Vérifier aussi un dash aérien court et maintenu : son comportement doit rester celui de V1.153. Coop/P2 reste différé.

Rollback : ZIP V1.153 immuable dans le reçu de promotion main ; conserver son INI personnel et ses raccourcis.
