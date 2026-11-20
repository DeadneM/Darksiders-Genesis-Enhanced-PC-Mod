# GPU — comparaison native avec et sans mod

Suite au [premier diagnostic](GPU_FREEZE_REVIEW_2026_11_19.md). Test explicitement autorisé par l’utilisateur, en vue native dans les deux sessions, avec la même scène et les mêmes réglages graphiques demandés. V1.172 installée ; main/release V1.168 intactes. Aucun nouveau build ou patch appliqué pour cet essai.

## Déroulement et restauration

Premier relevé avec le mod : processus moteur identifié et ThirdPerson=0 lu avant mesure. Compteurs Windows pendant environ 30 secondes, événements DXGI pendant 20 secondes, avec état de premier plan consigné.

Après confirmation de fermeture, DarksidersGenesisMod.asi et dxgi.dll ont été renommés temporairement dans le même dossier. Leurs empreintes correspondaient aux binaires V1.172 déjà conservés localement ; aucun téléchargement ni fichier supprimé. L’INI a gardé la même empreinte pendant ces opérations, et aucune sauvegarde de partie n’a été touchée.

Second lancement par l’utilisateur : absence de DarksidersGenesisMod dans les modules vérifiée, dxgi.dll chargée depuis System32. Relevé identique sans le mod. Les deux fichiers V1.172 ont ensuite été rétablis à leurs noms d’origine avec vérification des empreintes ; aucun fichier du mod ne reste désactivé. Les deux sessions de trace Windows sont arrêtées.

## Résultats observés

Même identifiant GPU dans les deux relevés, associé directement par DXGI à **Intel Arc B580**. Le numéro/identifiant de cette session a été revérifié, sans réutiliser celui d’un précédent démarrage Windows.

| Mesure au premier plan | Avec V1.172 | Sans le mod |
| --- | ---: | ---: |
| Observations GPU retenues | 9 | 14 |
| Utilisation 3D médiane Windows | 50 % | 47,5 % |
| Utilisation 3D min–max | 48–54 % | 43–58 % |
| Mémoire dédiée médiane | 3,35 Gio | 2,85 Gio |
| Intervalles de présentation retenus | 939 | 890 |
| Intervalle API médian | 16,68 ms | 16,75 ms |
| Intervalle API p99 | 24,65 ms | 24,98 ms |
| Intervalle API maximal | 31,36 ms | 26,16 ms |

Le trou de 225 ms du premier enregistrement se situe à la transition vers une observation hors premier plan et n’est pas retenu comme freeze de gameplay. Les fenêtres conservées sont bornées par des observations de premier plan espacées d’environ deux secondes ; elles ne constituent pas un suivi exact du focus pour chaque image.

**Retour utilisateur : charge plus basse et jeu plus fluide sans le mod.** Ce retour justifie l’isolation des fonctions du mod. Les métriques d’API des deux fenêtres restent proches et ne mesurent pas les images effectivement affichées, leurs duplications ou la complétion GPU. Un seul passage par condition, dans l’ordre avec puis sans : le réchauffement des caches et les différences de parcours/effets ne sont pas éliminés. Ne pas annoncer une cause unique, une fuite mémoire ou une boucle GPU prouvée. La différence d’environ 0,5 Gio n’est pas, à elle seule, une fuite : le mod peut retenir des ressources/options supplémentaires.

## Essai TPS complémentaire, sans build

La préparation suivante visait initialement NativeCustomDOF OFF, mais la lecture préalable a montré ThirdPerson=1, NativeCustomDOF=1 et TPSCustomDOF=0. La mesure a donc été explicitement reclassée comme **TPS avec personnalisation du flou TPS OFF**, pas comme un troisième passage natif.

15 observations sur environ 31 secondes, toutes au premier plan : utilisation 3D médiane 49 %, plage 43–84 %, mémoire dédiée médiane 3,04 Gio. Intervalles API retenus : médiane 16,76 ms, p99 25,20 ms, maximum 29,68 ms. Ces chiffres ne sont pas une comparaison chiffrée TPS OFF/ON : aucun passage ON contrôlé correspondant n’a été capturé.

L’INI a ensuite été retrouvé sur TPSCustomDOF=1 ; sa dernière écriture était postérieure au relevé. L’utilisateur a confirmé directement : **OFF plus fluide, ON moins fluide**. Il précise séparément que le chiffre de 100 % pourrait être un problème de pilote ; cette hypothèse n’est pas vérifiée. Le dernier relevé ON n’a pas démarré car le jeu était fermé : pas de trace incomplète laissée active.

TpsCameraFocus.h::ApplyBlur remplace explicitement la méthode/zone de profondeur de champ quand le profil est personnalisé. Le résultat utilisateur oriente donc vers cette personnalisation comme facteur de coût ou de perception de fluidité ; cela n’établit pas une boucle GPU, un temps GPU précis ou une correction de pilote. La correction de mise au point TPS peut rester active sans cette personnalisation.

Choix demandé à l’utilisateur : désactiver uniquement TPSCustomDOF dans son profil et les futurs défauts, en laissant l’option disponible. NativeCustomDOF et la correction de mise au point resteraient inchangés. À ce stade de rédaction, aucune modification de défauts ni de profil n’a encore été appliquée automatiquement.

[Synthèse et empreintes](evidence/GPU_NATIVE_COMPARISON_2026_11_19.json). Journaux bruts, profils personnels et traces détaillées conservés uniquement dans le cache privé local.


## Optimisation caméra sans changement de réglages — 19 novembre 2026

Décision utilisateur : REJETÉ, le passage automatique de TPSCustomDOF sur OFF. Ne modifier ni le profil personnel ni les défauts. La suite porte exclusivement sur du travail CPU évitable, à rendu conservé.

Depuis debug 3505391 / binaire V1.172 : les trois chemins TPS réutilisent leur compensation de mise au point réussie pour éviter son second calcul dans ApplyBlur. La projection des bornes est omise lorsque la région choisie atteint déjà 20000 unités, plafond du résultat qui aurait été calculé. Aucun hook, réglage, fréquence de service, qualité GPU ou ressource persistante ajouté. Le chemin natif conserve son contrat. Retour arrière : rétablir TpsCameraFocus.h et les trois appels de CameraTraceFeature.cpp depuis 3505391.

CONTRÔLE À EFFECTUER sur GitHub uniquement : tests existants et 20 480 combinaisons comparant tous les octets de la sortie caméra avec l'ancienne implémentation, pour les chemins TPS et natif. Aucun gain FPS/GPU ni suppression des saccades annoncé ; validation en jeu encore requise. Pas de nouveau ZIP ni installation pour ce contrôle.
