# V1.155 — continuité des règles de gameplay sous le menu

Base de travail : V1.154 sur debug ; main reste sur V1.153 acceptée. Retour utilisateur : les effets au sol ne fonctionnent pas et de nombreux comportements de code cessent pendant l'affichage du menu du mod. V1.154 n'est donc pas validée pour les nouvelles poussières.

## Cause établie dans le code et correction préparée

Le même état overlayVisible était utilisé pour deux décisions différentes : réserver les commandes à l'éditeur et désactiver les transformations du gameplay. Il désactivait notamment IsLiveTPSLookActor, la production du rayon de visée, la transformation XInput TPS et certaines reprises d'état/mouvement. Il ne désactivait pas tous les modules : jauges, effets et maintien de certaines apparences continuaient déjà leurs callbacks.

La politique commune GameplayMenuPolicy sépare désormais Runtime (fenêtre du jeu active, non réduite, hors fermeture) et Commands (mêmes conditions, menu du mod fermé). Les règles de visée et la transformation des lectures manette du jeu restent actives sous le menu. Les reprises de pose et de mouvement utilisent Runtime, sans dépendre de l'autorisation de chargement des effets.

Les nouveaux appuis de raccourcis, les gestes différés de dash, le saut de cheval, les raccourcis de recentrage/vue native, les réaffectations et la rotation volontaire de la caméra restent réservés aux commandes du jeu. Les mouvements de souris utilisés pour modifier l'éditeur sont vidés et ne sont pas rejoués à sa fermeture. Les références natives, pause du jeu, roues et cinématiques conservent leurs propres conditions.

L'éditeur lit les manettes à l'intérieur d'un PhysicalRead, comme les lectures déjà utilisées par la caméra. Sa navigation reçoit donc les axes physiques alors que les lectures suivantes du jeu conservent leur transformation TPS. En coop, la fraîcheur et l'identité P2 restent suivies sous l'éditeur, avec un indicateur distinct de capture des commandes pour éviter rotation, raccourcis et impulsions d'équipement déclenchés par la navigation.

Cette correction ne nécessite aucun nouveau hook, aucun changement de touche et aucune nouvelle clé INI. La désactivation hors fenêtre du jeu, pendant Alt-Tab, réduction ou fermeture reste effective.

## Contrôles

Le premier passage GitHub de cette préparation utilise **[check only]** : compilation et tests Windows, sans publier de ZIP. Les tests vérifient la séparation runtime/commandes, l'isolation des lectures de navigation, le maintien du traitement manette du jeu et celui de P2 sans rotation ou recentrage provenant du menu. Ce ne sont pas des validations de gameplay.

## Effets au sol encore en cours d'analyse

Le dernier journal V1.154 confirme un passage de substitution de la famille 45 mais pas son rendu final. La sélection finale de ce journal était Native ; cela ne remet pas en cause le retour utilisateur. L'analyse native confirme la chaîne : notification de surface 0x5D7760 → sélection dans la table m_SurfaceOverrides → notification parente 0x16977C0 → émission 0x169D0D0 → fabrique existante 0x187E600, via les fonctions attachée ou mondiale. Aucun nouveau point d'interception n'est ajouté sur cette seule base.

L'utilisateur a préparé une nouvelle session : le relevé externe en lecture seule confirme Strife à pied, Water=7, OtherStrifeFx=0 et les huit exports chargés. La comparaison Water/Hidden est en cours sur les composants natifs réels, avant de décider d'un correctif. Le lecteur n'appelle aucune API moteur et ne modifie aucun état du jeu.

Statut : préparation expérimentale, pas de nouveau ZIP livré à ce stade. Main/archive et les anciennes archives restent inchangés.
