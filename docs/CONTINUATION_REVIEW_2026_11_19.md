# Suite hors jeu — 19 novembre 2026

Base stable : V1.168 VALIDÉE. Beta V1.170 construite et livrée, en attente du test utilisateur. Travail effectué depuis le cache local et le code ; aucun téléchargement binaire, capture, lancement, installation ou nouvelle compilation.

## Deux passes sur les ajouts récents

Première passe : delta V1.168 → source V1.170, fonctions et chemin d’exécution. Les filtres réutilisent le service existant ; OFF et sans restauration en attente, ils quittent avant verrou et accès aux données natives. Le correctif des cœurs réutilise ProcessEvent, sans nouveau detour, scan global ou timer. Le rafraîchissement est consommé avant l’appel et ne se répète pas pour la même identité.

Deuxième passe : suivi des trois nouvelles clés depuis les défauts jusqu’au menu, à la sauvegarde et au consommateur, puis revue des tests et des identités. Les branchements sont présents ; aucune clé ajoutée inutilisée ou fonction morte démontrée. La page racine possède bien une identité faible non nulle dans les deux captures conservées ; les sous-nœuds sans serial ne sont pas utilisés comme racines différées. Les contrôles de durée de vie restent nécessaires après le retour du Tick natif.

Comparaison : les deux passes ne justifient aucun retrait de code fonctionnel récent. Elles ne mesurent pas le coût CPU réel. Des vérifications d’identité restent effectuées sur les Tick de la page même après son rafraîchissement ; cela concerne le menu des cœurs et ne démontre pas un ralentissement permanent en mission. Ne pas supprimer ces gardes pour un gain hypothétique. Aucun test C++ supplémentaire annoncé ; les 86/86 contrôles restent ceux de la source livrée V1.170.

## Correction préparée du journal du chargeur

Dans LoadAsiPlugins, le code Windows de LoadLibraryW était lu après StartupTimeline. Cette fonction écrit le journal avec des appels Win32 : elle peut remplacer la dernière erreur du thread. GetLastError est désormais capturé immédiatement après LoadLibraryW, avant ce journal.

Changement limité à l’ordre de deux instructions et à un commentaire. Aucun changement de DLL recherchée, point de chargement, export, Skip Logos, chemin d’INI ou hook. Statut : **PRÉPARÉ, relu statiquement, NON CONSTRUIT et NON VALIDÉ en jeu**. Le ZIP V1.170 existant n’est pas remplacé. Ce défaut de diagnostic n’explique pas à lui seul l’ancien échec d’Ultimate ASI Loader et ne le corrige pas.

## SourceControl : chemin natif précisé

Les deux assets SoulBarrier déjà extraits sont réutilisés, avec leurs empreintes d’origine. Les 13 fonctions sont décodées complètement ; tailles sérialisées et tailles de bytecode en mémoire vérifiées séparément. Les offsets suivants sont ceux du bytecode en mémoire, pas des adresses EXE.

| Chemin | Comportement observé dans le script |
| --- | --- |
| SetInitialAnimState → 0x421 | Appel SourceControlHelpers.IsEnabled |
| Résultat faux → 0x272 | Test bStartEnabled puis état de l’objectif ; activation éventuelle de P_SoulCache_Destroyed |
| Résultat vrai → 0x37D | Effet P_Execute_Burst_Impact, rotation native de 90°, échelle 2, puis audio de l’orbe |
| ReceiveBeginPlay → 0x2E7 → 0x140 | Branche distincte qui lie le délégué OnTakeAnyDamage |

L’appel concerne donc une décision d’état/effet de cet orbe, pas une acquisition de cible de Strife ni un réglage du pool de textures. Retirer seulement l’import ou substituer arbitrairement la branche n’est pas un nettoyage neutre. Même un patch du bytecode ne prouverait pas le retrait de la dépendance de chargement sans vérifier les tables d’import et de dépendances du package. Aucune modification native retenue pour cette occurrence unique non chronométrée. [Preuves synthétiques](evidence/CONTINUATION_REVIEW_2026_11_19.json).

## Suite

Tester V1.170 plus tard comme prévu, sans nouvelle installation demandée ici. Pour le chargeur alternatif, il manque toujours un essai séparé montrant sa présence, le chargement de l’ASI et le démarrage du worker ; pas de remplacement improvisé. Budget de textures/DXVK restent des pistes sans correction démontrée. Conserver la correction de journal pour un futur lot cohérent, sans nouvelle build pour elle seule.
