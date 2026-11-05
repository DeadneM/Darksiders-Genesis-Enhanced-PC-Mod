# Double audit du code récent — 5 novembre 2026

Base actuelle : debug `17946db24b5454d0777b2d4fed4207377570fcab`, source binaire V1.153 `1a4bbe29f65e8533ba8741cd9a93e3ee30b42a28`. Le retour utilisateur est « tout est bien, je n'ai pas vu de problème majeur par rapport à ce qu'on faisait ». Il accepte ensuite les récupérations coffre et dash, tout en précisant ne pas être sûr à 100 % du résultat. Ces options sont donc **VALIDÉES par l'utilisateur avec réserve** ; aucun gain de temps n'a été mesuré par ce retour. Les essais coop/P2 restent différés.

## Périmètre et comparaison

Deux passes par le même agent, selon deux méthodes : références et usages, puis cycles de création, invalidation et restauration. Périmètre borné : 11 fichiers, 1 121 lignes physiques, autour d'AirDashFeature, RangedStateFeature, StoneSkinVisual, StoneTransitionFx, SelectedAssetRecovery, SelectedParticleLoader et AimShaderCatalog. Ce n'est pas une analyse exhaustive de tout le mod ni un graphe d'appels C++ calculé par compilateur. Le nettoyage V1.125 reste acquis et n'est pas compté comme travail nouveau.

| Candidat de la première passe | Vérification de la seconde passe | Décision |
| --- | --- | --- |
| `g_events`, `g_requests`, deux occurrences chacun | Incrémentation puis comparaison pour limiter les logs | Conserver |
| Originaux et identités des matériaux | Restauration exacte de l'instance native et refus des réutilisations d'adresse | Conserver |
| Forget / Exhausted du chargement ciblé | Réarmement après invalidation et borne des demandes natives | Conserver |
| Sauvegarde des paramètres de dash et marqueur de geste | Consommés à des étapes distinctes de la fin native | Conserver |
| Anciennes clés / actions | Compatibilité des réglages et raccourcis existants | Conserver |
| Test négatif de copie des matériaux | Empêche la réintroduction d'une copie incompatible pierre/feu | Conserver dans les tests |

Les anciens essais `SuppressAirLandingEvent`, `ObserveAirLanding` et `ClearReleasedAim` sont absents du code de production. Aucun global de ce périmètre ne se réduit à une seule occurrence. **Aucune suppression supplémentaire démontrée.** Les deux passes concordent : pas de suppression pour réduire artificiellement le nombre de lignes, pas de changement de production et pas de nouveau binaire pour cet audit.

[Empreintes des fichiers et résultats des deux passes](evidence/V1_153_DOUBLE_AUDIT.json).

## Limites et suite active

Le dernier journal accessible contient le démarrage V1.153 et la vérification du setter de visée. Il ne fournit pas une comparaison de FPS ni une mesure CPU exploitable de Stoneskin. Les contrôles GitHub déjà reçus restent 81/81 ; aucune recompilation n'est nécessaire pour les seuls documents de cet audit.

La priorité choisie par l'utilisateur est la recherche d'autres apparences natives. [Résultats vérifiés dans le PAK et les animations](NATIVE_APPEARANCE_RESEARCH_2026_11_05.md). Léviathan et les apparences validées restent séparés et inchangés. Le résultat du test de persistance GPU après fermeture n'a pas été explicitement confirmé. Coop/P2 se fera en dernier, comme convenu.
