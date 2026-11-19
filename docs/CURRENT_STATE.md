# État courant — V1.172 debug disponible

Base fonctionnelle V1.171 VALIDÉE par l’utilisateur ; main/release V1.168 inchangées. V1.172 construite sur GitHub : 86/86 tests, zéro avertissement/erreur C++, ZIP deux binaires et lien vérifiés, Defender sans détection. Aucune installation. [Livraison](V1_172_DELIVERY_RECEIPT.md).

Nouvelle case Summon horse in the air juste sous Horse Everywhere, OFF par défaut. Action native conservée, résultat de sol adapté uniquement pendant un appel aérien local ; règles de zone/déblocage inchangées. 259 clés dont 258 anciennes valeurs préservées, 55 actions inchangées. Fonction EXPÉRIMENTALE jusqu’au retour en jeu. [Périmètre et tests](V1_172_REVIEW.md).

Petite optimisation CPU du suivi des shaders cumulée, gain non mesuré. Micro-saccades toujours ouvertes : compteurs 3D 51–65 %, environ 3,2 Gio dédiés pendant le relevé ; 20 secondes DXGI sans intervalle API >25 ms, pas de temps d’affichage effectif mesuré. [Diagnostic et limites](GPU_FREEZE_REVIEW_2026_11_19.md). Identifiant GPU vérifié séparément : Intel Arc B580, pas Radeon 780M.

INI/raccourcis personnels et installation intacts. [File active](ACTIVE_QUEUE.md).
