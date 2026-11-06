# V1.158 — finition et essai de l'aura Molten, 6 novembre 2026

## Résultat

L'utilisateur situe l'absence intermittente de lave sur **Strife**, puis confirme après l'essai : « ca a marcher a chaque fois ». **VALIDÉ sur cet essai**, sans nouvelle correction. L'ancien symptôme reste une intermittence non reproduite ; aucune cause n'est déclarée réparée.

Écharpe et menu déjà VALIDÉS. Cheval : retour favorable « a l'air OK », réserve conservée. GPU accepté précédemment ; coop différée. Base binaire inchangée V1.158, source `5f1591a85630ea0af128b4bd5c25996072bd5e3a`. Aucun build, installation, INI ou raccourci modifié pour cette finition. Main reste V1.153.

## Preuves de l'essai

Lecture autorisée après le signal « prêt » ; prélecture confirmée avant de demander les activations. Accès externe QUERY_LIMITED_INFORMATION | VM_READ ; aucun appel moteur ou écriture mémoire. Capture bornée à trois minutes, propriétaire local Strife vérifié à chaque échantillon, identités UObject et réciprocité contrôleur/personnage contrôlées.

- 733 échantillons acceptés, 127 refusés ; 98 états modifiés enregistrés. Les refus ne sont pas interprétés comme une panne du jeu ni une absence d'effet. 3 états comportent une lecture partielle de descendants ; cette limite est conservée dans le reçu.
- Cœur natif Molten niveau 2 présent, aura `P_MoltenFallen_CreatureCore_Status_Lvl2` attachée à `VFX_Aura` : 63 observations de particules Molten, toutes actives, visibles et non masquées dans les états acceptés. Cela ne prouve pas à lui seul leur rendu ; le retour visuel utilisateur le complète sur cet essai.
- Stinger est masqué, conformément au réglage personnel `StingerCoreAppearance=1` ; ne pas confondre son cristal et l'aura Molten.
- Observation complémentaire réussie sur `Strife_Silver` : socket natif `VFX_Aura` sur `Bone_Strife_Root`, hauteur locale 125. Le premier relevé complémentaire, refusé, n'est pas utilisé comme preuve d'absence de socket.

[Reçu agrégé, empreintes et limites](evidence/V1_158_MOLTEN_AURA_RETEST.json). Données brutes privées : `audit-v131/v284-aura-20261106-123820.jsonl`, `v284-aura-detail-20261106-124005.json` ; les scripts et adresses mémoire ne sont pas ajoutés à l'ASI.

## Contrat natif et choix de finition

Exports lus et SHA1 vérifiés depuis le PAK natif : le statut Molten porte séparément sa particule et une classe de blast. Au niveau 2, ce blast traite une zone de rayon 300 à intervalle d'une seconde. Le script natif SpawnAura l'attache au mesh par VFX_Aura et le retire par DestroyAura. Ces dégâts ne doivent pas être recréés pour traiter un défaut purement visuel. Les deux autres niveaux ont leurs propres rayons 250 et 400 ; aucun de ces paramètres n'est modifié.

La relecture ciblée croise usages et lifecycle : matériaux privés et restauration dans StoneSkinVisual, trois particules de transition dans StoneTransitionFx, disponibilité/invalidations dans EffectAppearanceFeature, reprise bornée des passifs du cheval dans MountedPassiveFx et routage du menu dans GameplayMenuPolicy. Les caches, compteurs de borne, compatibilités de masquage et originaux de restauration ont des usages réels. **Aucun nouveau reste démontré inutile retiré.** Ceci n'est pas un nouvel audit exhaustif du mod ni une mesure de FPS.

Les 82/82 contrôles GitHub sont ceux de la source binaire V1.158 déjà livrée ; aucun contrôle C++ relancé pour ces seuls documents. Les scènes coop restent à essayer en dernier. Si l'aura manque à nouveau, observer ce cas avant de réactiver ou recréer quoi que ce soit ; les états sains de cet essai ne justifient pas un hook supplémentaire.
