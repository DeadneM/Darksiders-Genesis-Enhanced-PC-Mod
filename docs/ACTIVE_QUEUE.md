# File active — base V1.168 validée

Dernier point : 18 novembre 2026. V1.170 beta cumulative en préparation : [arbre et options d’image](V1_170_REVIEW.md). V1.169 intermédiaire conservée. V1.168 est entièrement VALIDÉE, publiée sur main et en release v1.168 ; 85/85 contrôles Windows. TAA/netteté terminé. Aucun ancien défaut validé n’est rouvert sans nouveau retour.

## Suite, par ordre de preuve disponible

1. **Dépendance native SourceControl** — une erreur au chargement Level_00, référence confirmée dans SoulBarrier_Orb_Blueprint du PAK d’origine. Les 13 fonctions sont décodées : IsEnabled de SourceControl est réellement appelé. Pas de retrait aveugle, correctif ou gain mesuré. [Diagnostic](NATIVE_STREAMING_REVIEW_2026_11_18.md).
2. **Autre ASI loader** — cause de l’ancien échec inconnue. Prochaine preuve nécessaire : journal du chargeur et jalons ASI lors d’un essai réversible explicitement préparé. Aucun nouveau chargeur installé ; [constats existants](V1_168_TAA_LOADER_REVIEW.md).
3. **Budget de textures** — 1000 Mo dans le dernier journal, aucune alerte de dépassement trouvée dans celui-ci. Pas de valeur forcée ni de curseur supplémentaire sans problème ou mesure correspondants.
4. **DXVK** — expérience séparée éventuelle ; conflit de rôle autour de dxgi.dll à résoudre avant essai. DX12 demanderait un backend de rendu du mod adapté. [Étude](COMMUNITY_FIX_REVIEW_2026_11_13.md).

5. **Niveaux des cœurs dans l’arbre** — affichage incomplet au premier accès, rétabli en rouvrant ; deux relevés comparés : données stables, indicateurs cachés puis visibles. Rafraîchissement natif unique expérimental dans V1.170 ; validation en jeu attendue.

## Règles de reprise

Travailler sur debug, préserver main/release, INI et raccourcis. Build V1.169 autorisée par la nouvelle demande utilisateur ; aucun lancement/installation automatique. Captures du menu effectuées en lecture seule ; aucun remplacement de fichiers pendant que le jeu est ouvert. Réutiliser les reçus et le cache local ; lire seulement les différences. Un diagnostic n’est pas une correction validée.

L’ancienne file chronologique est conservée intégralement dans [l’historique](history/ACTIVE_QUEUE_THROUGH_V1_168_2026_11_18.md). Ses anciens statuts ne remplacent pas la validation globale V1.168. Les autres idées historiques restent consultables et ne deviennent pas automatiquement du travail actif.
