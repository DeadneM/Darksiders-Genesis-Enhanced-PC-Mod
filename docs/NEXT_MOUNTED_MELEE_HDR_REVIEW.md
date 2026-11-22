> Note historique avant reprise : accès rétabli et suite dans V1_178_REVIEW.md / HDR_NATIVE_BACKEND_REVIEW.md.

# Suite V1.177 — mêlée montée et proposition HDR

Retour utilisateur : V1.177 et autres fonctions validées. Défaut de réduction des effets de mêlée Strife exclusivement à cheval ; fonctionnement à pied conservé, y compris les deux lames OFF. Pas de nouvelle build demandée à ce stade.

## Mêlée — cause démontrée dans le code
MeleeEffectFeature.cpp::Prepare exige simultanément classe Strife_Blueprint_C, PoseKind::Foot, c.pawn == actor == c.character et contrôleur possédant cet actor. Liste d'animations et classification d'offsets limitées aux attaques de Strife à pied. Le test MeleeEffectTests.cpp attend même explicitement que le cas Horse reste natif : lacune de couverture fonctionnelle historique, pas régression du dernier lot rendu.

LocalPlayerLayout.h établit au contraire que le pawn possédé à cheval est la monture, le personnage normal restant c.character. Le chemin monté déjà validé de PistolVisualFeature.cpp::Mounted vérifie Despair_Blueprint_C et le mesh de c.pawn. Le squelette en cache v167-queue-assets/Strife_Horse_Mesh_Skeleton contient aussi Bone_Strife_Hand_L/R et Bone_Strife_Root : les FX montés doivent donc être liés au cavalier du mesh monté, pas au mesh à pied par simple suppression d'une garde.

Prochaine correction : identifier les animations/notifies, particules et offsets réellement utilisés par la mêlée montée, puis étendre le filtre existant avec propriété locale réciproque et os vérifiés. Conserver le chemin Foot, War, ultime, dégâts et autres particules. Aucun nouveau hook/scan répétitif ne se justifie. Tests à ajouter pour cheval, gauche OFF/droite OFF/les deux OFF et indépendance P1/P2. Aucun code spéculatif ni correctif partiel annoncé comme fonctionnel.

## HDR — proposition, non implémentée
Documentation Epic UE4.27 : https://dev.epicgames.com/documentation/en-us/unreal-engine/high-dynamic-range-display-output?application_version=4.27
Préférer Native par défaut / HDR expérimental avec application au redémarrage et retour SDR contrôlé. Vérifier d'abord CVars enregistrées ET sortie réelle (format de swapchain, espace couleur, écran compatible), rendu des menus et de l'overlay. Ne pas confondre simple modification du tonemapper avec une sortie HDR. La documentation indique que D3D11 impose le plein écran exclusif, et décrit des profils 1000/2000 nits ; ce n'est pas une preuve que Darksiders les implémente correctement. Aucun choix de nits arbitraire ni CVar HDR ajouté avant cette vérification. Aucun support spécifique au jeu annoncé.

## Limite d'accès actuelle
La lecture de l'exécutable local destinée au relevé HDR n'a PAS été exécutée : contrôle automatique d'autorisation impossible pour limite d'utilisation atteinte (pas une décision d'insécurité). Aucun contournement, capture mémoire, lancement du jeu, installation, build ni téléchargement. Analyse des sources et fichiers déjà en cache uniquement ; documentation locale actualisée. Vérification des assets montés et du chemin HDR natif encore requise avant implémentation/livraison.
