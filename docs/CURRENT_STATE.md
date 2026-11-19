# État courant — V1.172 debug disponible

Base fonctionnelle V1.171 VALIDÉE par l’utilisateur ; main/release V1.168 inchangées. V1.172 construite sur GitHub : 86/86 tests, zéro avertissement/erreur C++, ZIP deux binaires et lien vérifiés, Defender sans détection. Aucune installation. [Livraison](V1_172_DELIVERY_RECEIPT.md).

Nouvelle case Summon horse in the air juste sous Horse Everywhere, OFF par défaut. Action native conservée, résultat de sol adapté uniquement pendant un appel aérien local ; règles de zone/déblocage inchangées. 259 clés dont 258 anciennes valeurs préservées, 55 actions inchangées. Fonction EXPÉRIMENTALE jusqu’au retour en jeu. [Périmètre et tests](V1_172_REVIEW.md).

Petite optimisation CPU du suivi des shaders cumulée, gain non mesuré. Micro-saccades toujours ouvertes : compteurs 3D 51–65 %, environ 3,2 Gio dédiés pendant le relevé ; 20 secondes DXGI sans intervalle API >25 ms, pas de temps d’affichage effectif mesuré. [Diagnostic et limites](GPU_FREEZE_REVIEW_2026_11_19.md). Identifiant GPU vérifié séparément : Intel Arc B580, pas Radeon 780M.

INI/raccourcis personnels et installation intacts. [File active](ACTIVE_QUEUE.md).


## GPU — comparaison native du 19 novembre

Comparaison autorisée V1.172 / sans ASI, carte Arc B580 identique et premier plan suivi. Retour utilisateur : charge plus basse et jeu plus fluide sans mod ; utilisation 3D médiane Windows 50 % / 47,5 %, mémoire dédiée environ 3,35 / 2,85 Gio, intervalles API proches sur ces fenêtres courtes. Cause non isolée. Deux fichiers V1.172 restaurés, INI et sauvegardes intacts ; aucun fichier ne reste désactivé. Prochain essai proposé : seul NativeCustomDOF OFF avec le mod, sans build et après préparation utilisateur. [Résultats et limites](GPU_NATIVE_COMPARISON_2026_11_19.md).


## TPS — effet du flou confirmé par retour utilisateur

L’essai suivant a porté sur TPSCustomDOF OFF, pas sur NativeCustomDOF : préparation lue TPS=1, NativeCustomDOF=1, TPSCustomDOF=0. 15 observations toutes au premier plan ; retour utilisateur répété : OFF plus fluide, ON moins fluide. Le chiffre 100 % pourrait selon lui relever du pilote, sans confirmation technique. Aucun relevé ON contrôlé capturé car jeu fermé. Choix demandé avant changement : uniquement TPSCustomDOF OFF dans le profil actuel et les futurs défauts ; garder le profil natif, la correction de mise au point et l’option réactivable. Aucun défaut/profil changé automatiquement à ce stade. Fichiers V1.172 restaurés, aucun fichier ne reste désactivé. [Analyse et limites](GPU_NATIVE_COMPARISON_2026_11_19.md).
