# État courant — V1.174 debug, main V1.173

V1.173 confirmée par l'utilisateur et publiée sur main. V1.174 : réglages d'image Native/personnalisés construits et contrôlés 86/86, zéro avertissement ; rendu à tester en jeu. Aucun changement automatique de qualité ni installation. [Livraison](V1_174_DELIVERY_RECEIPT.md).

Nouvelle case Summon horse in the air juste sous Horse Everywhere, OFF par défaut. Action native conservée, résultat de sol adapté uniquement pendant un appel aérien local ; règles de zone/déblocage inchangées. 259 clés dont 258 anciennes valeurs préservées, 55 actions inchangées. Fonction EXPÉRIMENTALE jusqu’au retour en jeu. [Périmètre et tests](V1_172_REVIEW.md).

Petite optimisation CPU du suivi des shaders cumulée, gain non mesuré. Micro-saccades toujours ouvertes : compteurs 3D 51–65 %, environ 3,2 Gio dédiés pendant le relevé ; 20 secondes DXGI sans intervalle API >25 ms, pas de temps d’affichage effectif mesuré. [Diagnostic et limites](GPU_FREEZE_REVIEW_2026_11_19.md). Identifiant GPU vérifié séparément : Intel Arc B580, pas Radeon 780M.

INI/raccourcis personnels et installation intacts. [File active](ACTIVE_QUEUE.md).


## GPU — comparaison native du 19 novembre

Comparaison autorisée V1.172 / sans ASI, carte Arc B580 identique et premier plan suivi. Retour utilisateur : charge plus basse et jeu plus fluide sans mod ; utilisation 3D médiane Windows 50 % / 47,5 %, mémoire dédiée environ 3,35 / 2,85 Gio, intervalles API proches sur ces fenêtres courtes. Cause non isolée. Deux fichiers V1.172 restaurés, INI et sauvegardes intacts ; aucun fichier ne reste désactivé. Prochain essai proposé : seul NativeCustomDOF OFF avec le mod, sans build et après préparation utilisateur. [Résultats et limites](GPU_NATIVE_COMPARISON_2026_11_19.md).


## TPS — effet du flou confirmé par retour utilisateur

L’essai suivant a porté sur TPSCustomDOF OFF, pas sur NativeCustomDOF : préparation lue TPS=1, NativeCustomDOF=1, TPSCustomDOF=0. 15 observations toutes au premier plan ; retour utilisateur répété : OFF plus fluide, ON moins fluide. Le chiffre 100 % pourrait selon lui relever du pilote, sans confirmation technique. Aucun relevé ON contrôlé capturé car jeu fermé. Choix demandé avant changement : uniquement TPSCustomDOF OFF dans le profil actuel et les futurs défauts ; garder le profil natif, la correction de mise au point et l’option réactivable. Aucun défaut/profil changé automatiquement à ce stade. Fichiers V1.172 restaurés, aucun fichier ne reste désactivé. [Analyse et limites](GPU_NATIVE_COMPARISON_2026_11_19.md).


## Optimisation caméra sans changement de réglages — 19 novembre 2026

Décision utilisateur : REJETÉ, le passage automatique de TPSCustomDOF sur OFF. Ne modifier ni le profil personnel ni les défauts. La suite porte exclusivement sur du travail CPU évitable, à rendu conservé.

Depuis debug 3505391 / binaire V1.172 : les trois chemins TPS réutilisent leur compensation de mise au point réussie pour éviter son second calcul dans ApplyBlur. La projection des bornes est omise lorsque la région choisie atteint déjà 20000 unités, plafond du résultat qui aurait été calculé. Aucun hook, réglage, fréquence de service, qualité GPU ou ressource persistante ajouté. Le chemin natif conserve son contrat. Retour arrière : rétablir TpsCameraFocus.h et les trois appels de CameraTraceFeature.cpp depuis 3505391.

CONTRÔLE À EFFECTUER sur GitHub uniquement : tests existants et 20 480 combinaisons comparant tous les octets de la sortie caméra avec l'ancienne implémentation, pour les chemins TPS et natif. Aucun gain FPS/GPU ni suppression des saccades annoncé ; validation en jeu encore requise. Pas de nouveau ZIP ni installation pour ce contrôle.


Contrôle terminé : Windows x64 Release, 86/86 tests réussis, zéro avertissement/erreur C++. Les 20 480 combinaisons du test de parité comparent le chemin TPS combiné et le chemin natif indépendant, octet par octet, avec le calcul précédent. Source 4e3265180592a447d5de3c74c12504895b9b6fbe. [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/44600037723). Statut : CONSTRUIT et CONTRÔLÉ, gain de fluidité EXPÉRIMENTAL, non validé en jeu. Vérifier dans la même scène, avec les mêmes réglages : aspect/mise au point inchangés et éventuelle différence de saccades, à pied/cheval/ultime et pendant les roues. Aucun ZIP publié ou installé ; V1.172 reste la dernière livraison. Main/archive/v1.168 inchangés après vérification distante.


## Audit global des optimisations — 19 novembre 2026

Sources debug 0499f6f : inventaire 62 C++ / 125 headers, deux passes comparées (chemins fréquents puis garde-fous/invalidation/mesures). Dernier journal V1.172 lu, sans capture : gameplay inclusif avg858,62µs/max22,45ms ; appearance-discovery max8,23ms ; 78 589 432 draws comptés, deux shaders reconnus, 54 masqués. Pas de causalité freeze ni temps GPU déduits. Priorités : télémétrie/gestion des shaders, lectures d'identité groupées, contexte gameplay partagé, scans encore sans budget temps ; petits candidats CRC, focusP2 et fallbackDOF. Aucune suppression de production démontrée sûre. Propositions non implémentées ; aucun profil/défaut, source, binaire ou ZIP modifié. Dernier contrôle 86/86 reste celui de 4e32651. [Audit, comparaison et prochaines étapes](FULL_OPTIMIZATION_AUDIT_2026_11_19.md).


## V1.173 — lot CPU demandé, réactivité à préserver

Depuis V1.172/c11ea66 : comptage atomique par draw retiré (filtrage conservé), CRC/tailles shaders, lectures d'identité groupées avec fallback, focus P2 partagé. Aucun changement de réglage/défaut, rythme de scan/service/input ou interception. Pas de cache inter-image. PRÉPARÉ / EXPÉRIMENTAL avant CI GitHub ; aucun build local ni installation. Critère : même comportement/rendu et transitions skins/effets immédiates, puis gain à mesurer. [Périmètre, risques et tests](V1_173_REVIEW.md).


## V1.173 — livraison vérifiée, réactivité à tester

Source 28ab24f958084aa5982404888c3cebb1866685cc, archive d84168bd5184247f09088e61bbee529438eaf51f. GitHub :86/86 tests, zéro erreur C++, un avertissement C28020 de génération constexpr revu et documenté (aucun masquage). ZIP deux binaires contrôlé ; Defender aucune détection ; 259 clés/défauts/bindings préservés ; 237 anciennes archives et branches protégées inchangées. Aucune temporisation ou nouvelle réduction de cadence ; aucune installation. Gain de fluidité EXPÉRIMENTAL. Suite : essai groupé réactivité skins/effets/transitions, rendu/fluidité à réglages constants, coop en dernier. [Livraison et rollback](V1_173_DELIVERY_RECEIPT.md).


## Publication V1.173 demandée — 20 novembre

Promotion main autorisée, binaires Release V1.173 réutilisés sans recompilation. Aucun gain de fluidité ni correction du crash GPU déclarés validés. Archives/configuration conservées. [Périmètre et limites](V1_173_MAIN_RELEASE.md).


Publication V1.173 CONFIRMÉE sur main et release v1.173 : asset vérifié par empreinte GitHub identique au build existant ; pas de recompilation. 86/86 tests antérieurs, un avertissement constexpr documenté ; crash GPU non attribué. [Preuve](evidence/V1_173_RELEASE_CHECK.json).


## V1.174 — Native et réglages des deux effets d'image

Retour utilisateur : V1.173/coop/appel aérien acceptés ; TPS encore gourmand, crash GPU non attribué. Lot limité aux curseurs motion blur / limite chromatique, Native par défaut, 0 désactive, sauvegarde et migration anciennes cases. Aucune modification de DOF/cadence/skins. Table CRC générée sans indice pour l'analyseur. PRÉPARÉ avant CI GitHub ; pas de build local ni installation. [Détail et tests](V1_174_REVIEW.md).


## V1.174 — livraison contrôlée

Source a4f27706d89118ac9c40066f2b0c59058aa79716, archive 69fe6bef946cfa5306bcfbace9fee6966fa1836c : 86/86 tests Windows, zéro avertissement/erreur C++, ZIP deux binaires vérifié, Defender aucune détection. Mode Native par défaut ; slider/saisie motion blur0–1 et limite chromatique0–10. 259 anciennes valeurs conservées,2nouvelles ; aucune cadence ou DOF changé, aucune installation. Nouvelle image à valider en jeu. Main V1.173/archives préservées. Prochaine priorité TPS : mesure ciblée des passes GPU, pas de changement de qualité à l'aveugle. [Livraison et rollback](V1_174_DELIVERY_RECEIPT.md).
