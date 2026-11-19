# File active — base V1.168 validée

Dernier point : 19 novembre 2026. V1.170 beta cumulative disponible, 86/86 tests : [livraison, arbre et options d’image](V1_170_DELIVERY_RECEIPT.md). Validation en jeu attendue, sans installation automatique. V1.169 intermédiaire conservée. V1.168 est entièrement VALIDÉE, publiée sur main et en release v1.168 ; 85/85 contrôles Windows. TAA/netteté terminé. Aucun ancien défaut validé n’est rouvert sans nouveau retour.

## Suite, par ordre de preuve disponible

1. **Dépendance native SourceControl** — une erreur au chargement Level_00, référence confirmée dans SoulBarrier_Orb_Blueprint du PAK d’origine. Les 13 fonctions sont décodées : IsEnabled de SourceControl est réellement appelé. Pas de retrait aveugle, correctif ou gain mesuré. [Diagnostic](NATIVE_STREAMING_REVIEW_2026_11_18.md).
2. **Autre ASI loader** — cause de l’ancien échec inconnue. Prochaine preuve nécessaire : journal du chargeur et jalons ASI lors d’un essai réversible explicitement préparé. Aucun nouveau chargeur installé ; [constats existants](V1_168_TAA_LOADER_REVIEW.md).
3. **Budget de textures** — 1000 Mo dans le dernier journal, aucune alerte de dépassement trouvée dans celui-ci. Pas de valeur forcée ni de curseur supplémentaire sans problème ou mesure correspondants.
4. **DXVK** — expérience séparée éventuelle ; conflit de rôle autour de dxgi.dll à résoudre avant essai. DX12 demanderait un backend de rendu du mod adapté. [Étude](COMMUNITY_FIX_REVIEW_2026_11_13.md).

5. **Niveaux des cœurs dans l’arbre** — affichage incomplet au premier accès, rétabli en rouvrant ; deux relevés comparés : données stables, indicateurs cachés puis visibles. Rafraîchissement natif unique expérimental dans V1.170 ; validation en jeu attendue.

## Règles de reprise

Travailler sur debug, préserver main/release, INI et raccourcis. Build cumulative V1.170 autorisée par la demande utilisateur ; aucun lancement/installation automatique. Captures du menu effectuées en lecture seule ; aucun remplacement de fichiers pendant que le jeu est ouvert. Réutiliser les reçus et le cache local ; lire seulement les différences. Un diagnostic n’est pas une correction validée.

L’ancienne file chronologique est conservée intégralement dans [l’historique](history/ACTIVE_QUEUE_THROUGH_V1_168_2026_11_18.md). Ses anciens statuts ne remplacent pas la validation globale V1.168. Les autres idées historiques restent consultables et ne deviennent pas automatiquement du travail actif.

## Préparé depuis la livraison V1.170

- Journal du chargeur : capturer GetLastError avant l’écriture de la chronologie. Correction préparée seulement ; aucun nouvel essai, build ou diagnostic de compatibilité annoncé.
- SourceControl : flux SetInitialAnimState entièrement précisé ; le résultat choisit une branche d’état/effet. Aucun retrait natif retenu pour cette occurrence unique non chronométrée.
- Deux passes sur le delta récent : aucune suppression fonctionnelle justifiée. [Revue et suite](CONTINUATION_REVIEW_2026_11_19.md).

## V1.171 demandée

Construire le lot cumulatif et la correction de journal, puis vérifier CI/ZIP/lien. Aucun autre défaut ne dispose de preuve suffisante pour un patch supplémentaire. Pas d’installation automatique ; V1.170 et main V1.168 préservées.

## Livraison V1.171

Construite et contrôlée : 86/86 tests. Correction de journal incluse ; archive V1.170 préservée. Tests utilisateur reportés sur le lot cumulatif V1.171, sans nouveau test de gameplay ajouté. [Livraison](V1_171_DELIVERY_RECEIPT.md). Les autres pistes restent sans patch démontré.
