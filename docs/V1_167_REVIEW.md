# V1.167 — écharpe : shader de la famille Molten

## Base, retours et objectif

Debug V1.166 `cbf9fb0f81a7f47ef1d0c628a8f55c1912149a33` ; main V1.164 validée inchangée. Retour utilisateur du 13 novembre : **synergie et sons des chevaux VALIDÉS**, Contre-esquive NON TESTÉE (impression favorable seulement), écharpe V1.166 REJETÉE : plus de lave/feu visible. Les contrôles précédents ne constituaient pas une validation du rendu.

Objectif : retrouver le feu du revêtement Molten sur l'écharpe et afficher ses deux faces, sans modifier le corps, les explosions/cendres, les capacités, les sons ou les commandes. Critère en jeu : feu visible avec Molten, aucune émission sans Molten, envers/extrémités complets, restauration exacte à l'expiration et après changement de skin.

## Diagnostic appuyé par le journal et les fichiers locaux

Le journal de V1.166 montre l'ancien revêtement avant disponibilité du candidat, puis deux installations réussies de Gold. Le refus par padding de V1.165 est donc bien levé. Le défaut visuel rapporté concerne maintenant le candidat réellement appliqué.

Gold utilise MM_Basic_Character et les branches Emissive_Multi_Color / Emissive_Multi_Color_Scroll. Molten utilise MM_Basic et Emissive / Emissive_Scroll. Copier le matériau Strife non illuminé puis quelques puissances/couleurs ne transférait pas l'ensemble des masques et textures d'émission Molten. Le partage de noms de paramètres ne démontrait pas l'équivalence de ces deux shaders. Ne pas annoncer une cause de rendu mesurée pixel par pixel : la différence de branches et de sources est prouvée, le nouveau rendu reste à tester.

Inventaire local étendu : 39 560 en-têtes du PAK, 948 MaterialInstanceConstant dépendant de MM_Basic, 41 avec double face explicitement activée, zéro erreur de parsing. Lectures/décompression/SHA1 locales seulement ; aucune lecture de processus, modification du jeu ou ressource téléchargée. Ce n'est pas un catalogue exhaustif des autres familles de shaders.

Le candidat `Dn_HL_Inst_DemonBanners_B_Singed` est de la **même famille MM_Basic**, avec Emissive et Emissive_Scroll activés, TwoSided activé, BlendMode Masked. Il ne comporte pas la branche de vent du candidat arbre, ni le mélange translucide des sols. Le master MM_Basic est déjà utilisé avec les meshes squelettiques. Cela justifie un essai ciblé ; les différences Masked/Detail_Normal imposent encore le contrôle des bords et de l'éclairage en jeu.

## Implémentation et nettoyage

- MID privé sur les seuls slots d'écharpe reconnus de Strife.
- Vérifier que le matériau du candidat et le revêtement de source ont le même parent natif MM_Basic.
- Une seule copie complète depuis le revêtement Stoneskin natif — Molten si actif — incluant ses masques, textures, couleurs et paramètres hérités. Aucun paramètre visuel de bannière n'est conservé volontairement.
- La permutation du candidat a ses branches de feu cuites ON : sans Molten, les deux puissances sont donc explicitement mises à zéro sur le seul MID privé.
- Retrait du parent Gold et de la traduction partielle alias/puissances/vecteurs ; aucune modification de matériau partagé, CDO, drapeau de rendu global ou shader recompilé à l'exécution.
- La référence du matériau équipé sert toujours à restaurer exactement le skin/MID natif, indépendamment de la source d'émission. Corps minéral et effets de transition conservés.
- Ressource optionnelle demandée dans le budget existant. Si absente ou contrat refusé, l'ancien revêtement Molten reste disponible ; pas de boucle de chargement ou de copie par frame.

## Deux revues et contrôles

Première passe : comparaison des branches/parents/textures et log d'application ; la correction ne se limite plus à augmenter une puissance. Deuxième passe : variante sans Molten (risque d'allumer le feu par les switches du candidat, neutralisé explicitement), Master incompatible, ABI, ressource tardive, wrapper natif, skin Abyssal et restauration. Les mocks et cas spécifiques à l'alias Gold supprimé sont retirés ; les régressions du corps et du padding restent couvertes.

Les tests vérifient que la copie prend **le revêtement Molten complet**, pas les valeurs non illuminées de l'écharpe équipée ; ancien chemin V1.166 échouerait à cette condition. Coût constant par création, aucune copie lors des observations suivantes. Compilation/tests Windows sur GitHub à venir ; voir le reçu final pour le résultat effectif.

252 valeurs et 55 actions Hotkeys inchangées, INI personnel/PAK/sauvegardes non modifiés, anciennes archives conservées. Aucune modification du code de synergie, Contre-esquive, sons des chevaux, visée, GPU, Moloch ou récupération. Aucun autre correctif de bug natif annoncé sans reproduction actuelle ; les anciens compteurs de logs à zéro ne suffisent pas à rouvrir un défaut déjà accepté.

**PRÉPARÉ / EXPÉRIMENTAL, NON VALIDÉ en jeu.** Pas d'installation ni lancement du jeu par l'agent. Pas de gain FPS mesuré. En cas de défaut de ce candidat, comparer avec le revêtement classique Molten avant un autre remplacement ; ne pas empiler de nouveaux transferts partiels.

## Test groupé

1. Strife + cœur Molten : Stoneskin, face/envers/bouts, feu visible, fin complète puis restauration.
2. Sans Molten : peau de pierre sans feu sur l'écharpe.
3. Skins de base et Abyssal, puis changement de niveau : même séparation des variantes et restauration du skin exact.
4. Petite régression synergie/cheval ; Contre-esquive OFF reste à vérifier quand l'utilisateur le souhaite, sans réaffecter de commande.

Suite : retours visuels, surveillance des intermittences de ressources, puis autres apparences utiles et audit des ajouts. La coop complète reste réservée au passage final convenu.
