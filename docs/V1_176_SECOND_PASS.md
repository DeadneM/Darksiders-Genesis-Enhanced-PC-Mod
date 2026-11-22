# V1.176 — seconde passe ciblée, 22 novembre 2026

Retour utilisateur : ensemble satisfaisant, quelques paramètres sans effet apparent ; noms encore demandés. Ne pas assimiler ce retour à une validation de tous les effets.

Deuxième lecture indépendante du PE local : 17 champs, neuf recoupements chacun (nom/pointeur/type/offset float ; nom/pointeur/type bool ; pointeur setter à +48 ; instruction et masque exacts). Tous concordent avec le code. Lecture du journal du 21 novembre : 17/17 contrats acceptés, chromatique et maximum motion blur acceptés ; écritures de catalogue observées au poids caméra 1. Le diagnostic est volontairement borné à 64 lignes et ne prouve ni toutes les valeurs testées ni leur consommation finale par le rendu.

Relecture du trajet UI → Save/Load → Snapshot → publication atomique → callback caméra : aucun nouvel oubli trouvé. Native ne force aucune valeur ; sorties au poids différent de 1 laissées natives. Aucun scan par image ajouté. Les tests CI déjà réussis ne sont pas relancés sans changement de code.

Limites à ne pas masquer : overrides appliqués à la sortie caméra, pas après toutes les couches du rendu. Volumes du niveau, CVar de qualité/showflags et contraintes du renderer peuvent limiter/neutraliser un réglage. Dirt/cubemap/LUT demandent une texture ; SSR demande une surface compatible ; rayon/seuil/roughness ne sont pas des intensités. Exposition 0 est neutre, pas OFF. Les plafonds ne sont pas des maxima moteur prouvés. Impossible d'attribuer les quelques paramètres inertes sans leurs noms ; pas de correctif global spéculatif.

Aucune erreur supplémentaire démontrée, aucune modification de production, build, téléchargement, installation ou capture mémoire. Main/archive et profil personnel intacts. [Preuves](evidence/V1_176_SECOND_PASS_CHECK.json).
