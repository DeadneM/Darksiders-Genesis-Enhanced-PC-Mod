# V1.155 — continuité des règles de gameplay sous le menu

Le lot final est **CONSTRUIT et CONTRÔLÉ**, 81/81 tests Windows GitHub. Le comportement en jeu reste **EXPÉRIMENTAL**. [Téléchargement et reçu final](V1_155_DELIVERY_RECEIPT.md).

Base de travail : V1.154 sur debug ; main reste sur V1.153 acceptée. L'utilisateur confirme finalement les poussières de dash sur Strife à pied : il les avait prises pour des effets de stomp. Le défaut restant concerne les comportements de code qui cessent pendant l'affichage du menu du mod.

## Cause établie dans le code et correction préparée

Le même état overlayVisible était utilisé pour deux décisions différentes : réserver les commandes à l'éditeur et désactiver les transformations du gameplay. Il désactivait notamment IsLiveTPSLookActor, la production du rayon de visée, la transformation XInput TPS et certaines reprises d'état/mouvement. Il ne désactivait pas tous les modules : jauges, effets et maintien de certaines apparences continuaient déjà leurs callbacks.

La politique commune GameplayMenuPolicy sépare désormais Runtime (fenêtre du jeu active, non réduite, hors fermeture) et Commands (mêmes conditions, menu du mod fermé). Les règles de visée et la transformation des lectures manette du jeu restent actives sous le menu. Les reprises de pose et de mouvement utilisent Runtime, sans dépendre de l'autorisation de chargement des effets.

Les nouveaux appuis de raccourcis, les gestes différés de dash, le saut de cheval, les raccourcis de recentrage/vue native, les réaffectations et la rotation volontaire de la caméra restent réservés aux commandes du jeu. Les mouvements de souris utilisés pour modifier l'éditeur sont vidés et ne sont pas rejoués à sa fermeture. Les références natives, pause du jeu, roues et cinématiques conservent leurs propres conditions.

L'éditeur lit les manettes à l'intérieur d'un PhysicalRead, comme les lectures déjà utilisées par la caméra. Sa navigation reçoit donc les axes physiques alors que les lectures suivantes du jeu conservent leur transformation TPS. En coop, la fraîcheur et l'identité P2 restent suivies sous l'éditeur, avec un indicateur distinct de capture des commandes pour éviter rotation, raccourcis et impulsions d'équipement déclenchés par la navigation.

Cette correction ne nécessite aucun nouveau hook, aucun changement de touche et aucune nouvelle clé INI. La désactivation hors fenêtre du jeu, pendant Alt-Tab, réduction ou fermeture reste effective.

## Contrôles

Le premier passage GitHub utilise **[check only]** : compilation réussie sans nouveau ZIP, 80/81 tests réussis. Le scénario ajouté à CoopCameraIsolation avait relâché le tir avant l'ancienne assertion de tir ; l'entrée de ce scénario est rétablie avec un delta nul pour reprendre le test existant. Le nouveau contrôle complet doit confirmer cette correction. Les tests vérifient la séparation runtime/commandes, l'isolation des lectures de navigation, le maintien du traitement manette du jeu et celui de P2 sans rotation ou recentrage provenant du menu. Ce ne sont pas des validations de gameplay.

## Effets de dash vérifiés avec l'utilisateur

Le dernier journal V1.154 confirme un passage de substitution de la famille 45 mais pas son rendu final. La sélection finale de ce journal était Native ; cela ne remet pas en cause le retour utilisateur. L'analyse native confirme la chaîne : notification de surface 0x5D7760 → sélection dans la table m_SurfaceOverrides → notification parente 0x16977C0 → émission 0x169D0D0 → fabrique existante 0x187E600, via les fonctions attachée ou mondiale. Aucun nouveau point d'interception n'est ajouté sur cette seule base.

Le relevé externe en lecture seule confirme Strife à pied, Water=7, OtherStrifeFx=0 et les huit exports chargés. Il a ensuite observé chaque choix 1 à 8 avec le bon template actif et visible, Hidden avec les templates natifs actifs mais masqués, et Native avec les templates de surface du jeu. Capture de 180 secondes : 1 124 échantillons, aucune erreur. Le lecteur n'a appelé aucune API moteur et n'a modifié aucun état du jeu. [Preuve compacte et retour utilisateur](evidence/V1_154_DASH_USER_VALIDATION.json). L'absence de serial sur les particules non masquées est native : le jeu initialise ce serial à la création d'une référence faible ; aucune faiblesse de CaptureLive n'a été démontrée. Le code des effets reste inchangé.

## Nouveaux signalements ajoutés au suivi

Des flammes manquent à la première apparition du cheval dans le premier niveau ; un rappel du cheval en rétablit une partie. Sous Stoneskin, certains éléments de l'écharpe de Strife paraissent transparents. La qualité d'effets enregistrée du jeu vaut 3 et les choix MountFx/OtherMountFx sont Native. Le mod n'a pas de correctif démontré pour ces symptômes. Vérifier disponibilité, parents/activité des particules et matériaux d'opacité avant de modifier le rendu, sans confondre ces signalements avec la poussière de dash validée.

Statut : préparation expérimentale, pas de nouveau ZIP livré à ce stade. Main/archive et les anciennes archives restent inchangés.
