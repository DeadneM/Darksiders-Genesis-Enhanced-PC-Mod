# Charge GPU et freezes — diagnostic du 19 novembre

L’utilisateur valide les fonctions V1.171 puis signale des freezes et 100 % GPU dans le Gestionnaire des tâches et Steam, possiblement anciens. V1.171 devient la dernière base fonctionnelle VALIDÉE sur debug ; main/release restent V1.168. Cela n’établit ni la date d’apparition ni la cause du problème de performance.

## Éléments recueillis

Journaux récents copiés uniquement dans le cache de diagnostic local, sans téléchargement ni modification du jeu. Configuration lue : 3840 × 2160, qualité 3, VSync ON, FrameRateLimit=0 ; cette dernière valeur ne signifie pas que la VSync est désactivée. TPS FOV 110, personnalisation TAA ON. Aucun changement de réglage effectué.

Lecture des journaux de la session V1.171 précédente : aucune alerte de dépassement du pool de textures ou perte de périphérique GPU trouvée. Pool final 1000 Mo, sans justification actuelle pour le modifier. Une attente audio de 51 ms apparaît au démarrage et la dépendance SourceControl reste signalée une fois au chargement ; aucune des deux n’est synchronisée aux freezes rapportés. Aucun spam permanent du journal du mod démontré. L’avertissement MM_Basic/mesh particles existe, sans coût associé mesuré.

Après « prêt », relevé autorisé des **compteurs Windows seulement** sur DarksidersGenesis-Win64-Shipping, en excluant son lanceur Steam séparé. 15 observations sur 31,2 secondes, environ une toutes les deux secondes ; chaque groupe de requêtes prend environ une seconde. Aucun handle mémoire du jeu, écriture, injection, appel moteur ou capture graphique. L’utilisateur confirme un ou plusieurs freezes pendant cette fenêtre.

| Compteur du jeu | Minimum | Médiane | Maximum |
| --- | ---: | ---: | ---: |
| Moteur GPU 3D | 51 % | 58 % | 65 % |
| Mémoire vidéo dédiée | 3,19 Gio | 3,22 Gio | 3,24 Gio |
| Mémoire vidéo partagée | 0,06 Gio | 0,06 Gio | 0,06 Gio |
| RAM système disponible | 11,85 Gio | 11,89 Gio | 11,94 Gio |

Les autres moteurs GPU du processus relevés restent à 0 %. Un instantané global ultérieur retrouve 51 % sur le même moteur, entièrement attribués au jeu. Pas de mesure FPS, temps de chaque image, température, fréquence GPU ou durée précise de freeze. Les intervalles espacés et les requêtes séquentielles peuvent manquer des pointes ; ils ne prouvent pas une absence de saturation instantanée ou de pression de résidence. Les défauts de page système ne sont pas attribuables à ce jeu seul et les E/S du processus ne sont pas uniquement des accès disque.

## Revue du mod

- Present transmet syncInterval et flags au jeu sans changement. L’interface ne dessine que lorsque le menu ou une notification est visible : pas de second rendu permanent de la scène dans ce chemin.
- TAA et filtres d’image modifient des paramètres existants ; pas de shader additionnel ou rendu complet ajouté par ces réglages.
- Léviathan conserve son composant ; créations bornées, contrôle espacé de 100 ms. Cheval : réparation passive unique par identité avec fenêtre d’attente bornée. Stoneskin : contrôles espacés de 50/250 ms, créations/copies conditionnelles. Aucun respawn systématique par image démontré dans ces chemins ; cela ne mesure pas le coût graphique des effets choisis.
- Service gameplay appelé depuis la caméra et GetMaxSpeed : plusieurs appels synchrones sont donc possibles par image. Ne pas supprimer arbitrairement l’un de ces chemins : ils garantissent aussi les actions/possessions et états validés. C’est une piste de coût CPU à attribuer précisément, pas une boucle infinie démontrée.
- Télémétrie CPU précédente, échantillonnée 1/64 : service gameplay moyen 1,051 ms et maximum 22,871 ms sur 550 observations. Temps inclusifs comprenant des appels natifs, pas des temps GPU/image ; ne pas additionner les sous-étapes et ne pas convertir ces chiffres en FPS ou en durée des freezes.

## Optimisation limitée préparée

Dernière session : 509 créations de pixel shaders réussies, **aucun shader de visée reconnu**, mais 24 683 182 appels Draw observés. Le compteur Draw n’indique ni des dessins supplémentaires ni le nombre de changements de shader.

HookSetPS mettait malgré tout à jour sa table de contexte à chaque liaison, avec un verrou partagé et un verrou exclusif, alors que l’ensemble des shaders de visée était vide. Candidat : après transmission native de PSSetShader, quitter avant ces verrous/table tant que g_matchCount vaut zéro. Dès qu’un shader cible est créé et reconnu, le chemin existant reprend. Aucune modification du catalogue, CRC, empreinte, mode de masquage, suivi lorsque des cibles existent, ou vérification PSGetShader avant suppression d’un dessin.

Deux relectures : (1) compteur incrémenté seulement après insertion d’un shader cible conservé par AddRef ; (2) Skip quitte déjà sur zéro cible, et la liaison native reste toujours transmise avant le nouveau test. Ce travail de suivi est donc inutile dans ce cas précis. Cela supprime un coût CPU, **pas du travail GPU démontré**. Gain non mesuré ; freezes non annoncés corrigés. Statut : PRÉPARÉ / contrôlé statiquement, NON CONSTRUIT, NON INSTALLÉ, NON VALIDÉ en jeu. Pas de nouveau test qui reproduit seulement cette condition ; les tests et rendu existants devront être vérifiés lors d’un futur build GitHub.

## Suite

Préciser durée/contexte des freezes puis mesurer les temps d’image autour d’un défaut, avant de choisir entre rendu, attente CPU/streaming ou effets. Aucun plafond FPS, qualité, budget VRAM ou paramètre du moteur forcé. Un essai comparatif devra garder la même scène et ne changer qu’un facteur. La petite optimisation préparée ne remplace pas cette attribution.

[Synthèse sans données personnelles](evidence/GPU_FREEZE_REVIEW_2026_11_19.json). Les journaux bruts et relevés détaillés restent dans le cache privé local.

Références de méthode : [Epic — Performance and Profiling Overview](https://dev.epicgames.com/documentation/en-us/unreal-engine/performance-and-profiling-overview?application_version=4.27), distinction des limites CPU/GPU et impact de la scène/résolution ; [Microsoft — GPUs in the task manager](https://devblogs.microsoft.com/directx/gpus-in-the-task-manager/), pourcentage global fondé sur le moteur le plus occupé. Ces documents ne prouvent pas la cause du défaut de ce PC.

Précision utilisateur : les freezes durent une fraction de seconde et sont surtout perceptibles en mouvement ; leur présence à l’arrêt n’est pas déterminée. Mesure DXGI Windows de 20 secondes effectuée, sans téléchargement, injection ou modification du jeu. La session temporaire propre a été arrêtée avec succès ; aucun autre enregistreur arrêté.


## Résultat de la mesure de présentation

1204 paires début/fin complètes, une seule swapchain, SyncInterval=1 et flags=0 sur toutes les présentations. Intervalles d’entrée API : médiane 16,681 ms, p99 18,373 ms, maximum 19,626 ms ; aucun intervalle supérieur à 25 ms. Durée dans Present natif : médiane 8,752 ms, maximum 9,750 ms. Temps entre son retour et l’appel suivant : médiane 7,913 ms, maximum 13,830 ms. Ne pas interpréter le temps dans Present comme du temps d’exécution GPU : la synchronisation verticale peut y faire attendre.

Ces 20 secondes ne capturent pas une pause longue de soumission. Elles ne mesurent ni les images réellement affichées/dupliquées ni la complétion GPU ; elles n’invalident donc pas le ressenti utilisateur ou les freezes confirmés pendant le premier relevé distinct. Aucun diagnostic de cause certaine. Trace locale bornée : 1 507 328 octets ; aucun outil installé, aucun binaire téléchargé, aucune nouvelle build. [Principe des événements DXGI également utilisé par PresentMon](https://github.com/GameTechDev/PresentMon/blob/main/PresentData/PresentMonTraceConsumer.cpp).
