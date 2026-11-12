# Suite après V1.164 validée — envers de l'écharpe

**CANDIDAT PRÉPARÉ sur debug ; NON CONSTRUIT, NON VALIDÉ en jeu.** Main conserve exactement la V1.164 validée. Priorité restante : afficher les deux faces de l'écharpe Stoneskin Strife en gardant son feu, sa forme complète et la restauration du matériau natif.

## Comparaison locale effectuée

42 matériaux déjà en cache ont été examinés ; neuf portent un override double face. Recherche élargie ciblée dans le PAK original : 57 en-têtes de matériaux dont le nom évoque tissu/pierre/feu/lave, 27 de la famille MM_Basic, dix double face. Les deux ensembles se recouvrent ; ce n'est pas un inventaire exhaustif de tous les shaders du jeu. En-têtes et exports des candidats vérifiés contre les blocs du PAK local avec contrôle SHA1 ; aucune copie HTTP ni modification des fichiers du jeu. Douze paires manquantes extraites localement, autres fichiers réutilisés.

Les candidats double face trouvés dans cette sélection sont masqués. Aucun remplacement opaque + double face + feu défilant directement équivalent au matériau actuel n'est démontré. Les variantes Abyssales Stoneskin n'apportent pas l'override recherché.

War_Cloth_Gold_Inst_A et War_Cloth_Silver_Inst_A : parent MM_Basic_Character, double face, BlendMode Masked, Emissive_Multi_Color et Emissive_Multi_Color_Scroll activés dans leur shader déjà cuit. Angel_Inst_B_Cloth_01 combine aussi ces deux commutateurs. Ces permutations constituent des candidats, pas une correction validée.

War_Inst_StoneSkin_Molten_A actuel : parent MM_Basic, opaque, feu via Emissive_Scroll. Les paramètres Emissive_Power_Scroll et Emissive_Scroll_Speed existent aussi dans la famille des tissus ; cela fournit un point d'entrée commun. En revanche les tissus Gold utilisent une texture Emissive_Color_Texture et leurs propres Base_Color/MRE/Normal. Les appliquer directement importerait les UV/masques de War et pourrait refaire disparaître des morceaux de Strife.

## Périmètre prévu avant un futur build

1. Garder le chemin minéral du corps et la logique d'explosion/cendres intacts. Préserver la référence forte du matériau d'origine, l'identité du mesh/skin et la restauration déjà validées.
2. Essayer un MID privé de tissu double face déjà cuit, en gardant le masque/textures propres à l'écharpe équipée de Strife. Vérifier les paramètres et l'API de copie avant de l'ajouter ; ne pas supposer qu'une copie d'uniformes transfère un commutateur statique.
3. Transférer seulement les valeurs compatibles nécessaires au feu, sans les textures corporelles ou le masque étranger. La variante sans feu doit rester distincte. Vérifier aussi les sources de texture par défaut héritées du parent, pas seulement les overrides du matériau.
4. Avant acceptation : face/envers, écharpe entière, feu actif puis extinction, armures Native/Abyssale/bonus et changement de skin. Une face réparée avec un feu cassé ne répond pas à l'objectif.

Pas de culling global désactivé, de duplication du personnage, de recompilation de shader supposée disponible, ni de nouvelle case sans effet. Le remplacement direct par Gold n'est pas appliqué tant que le problème d'alpha n'est pas résolu. Les analyses privées v293-cloth-parameters.json, v293-two-sided-scan.json et v293-expanded-cloth.json conservent les détails ; la synthèse exportée est dans evidence/V1_164_SCARF_NEXT_CHECK.json.

Le prochain build reste expressément différé. Aucun nouveau binaire, ZIP, tag ou release créé pour cette recherche. [Promotion main contrôlée](evidence/V1_164_MAIN_PROMOTION_CHECK.json).


## Candidat préparé le 12 novembre 2026 — compilation toujours différée

Le code de `StoneSkinVisual.cpp` utilise facultativement un MID privé dont le parent est `War_Cloth_Gold_Inst_A`. Son shader déjà cuit est Masked et double face ; ses commutateurs de feu défilant sont conservés. Aucun override global de culling ni modification d'un matériau partagé.

La simple substitution par Gold n'est pas utilisée : `K2_CopyMaterialInstanceParameters(Source=matériau natif équipé, bQuickParametersOnly=false)` recopie d'abord les paramètres de la hiérarchie native de Strife, y compris ses textures et valeurs héritées. Le but est de conserver son alpha au lieu du masque de War. Ensuite seuls trois scalaires de puissance d'émission (dont l'alias Emissive_Power) et trois vecteurs couleur/vitesse compatibles sont réglés sur le MID privé. Sans Molten, les trois puissances valent zéro. Le corps minéral et les particules de lancement/expiration gardent leur chemin existant.

Cette copie complète est volontairement effectuée une seule fois par création du MID d'écharpe, jamais à chaque observation. L'API est connue pour être lente : ce n'est PAS une optimisation de performance démontrée. Son coût au lancement et l'apparence doivent encore être mesurés en jeu. La texture de tissu équipée remplace celle du revêtement opaque historique : aspect minéral du tissu et répartition du feu restent donc expérimentaux.

Le matériau facultatif est chargé après les ressources de peau/explosion/cendres dans le budget existant d'une demande par service. Son absence n'accélère pas le scan global des assets manquants. S'il arrive tard, seuls les slots de tissu sont reconstruits. Une déclaration native incompatible, des paramètres absents ou une création refusée restaurent le matériau précédent avant repli vers l'ancien revêtement. Un essai de candidat refusé n'est pas répété toutes les 50 ms pour un slot actif. Restauration exacte des références natives et contrôles propriétaire/mesh/skin/monde conservés.

### Preuves et contrôles réalisés sans compilation

- EXE UE4.22.2 local : K2_CopyMaterialInstanceParameters RVA 0x1D96D80 appelle le répartiteur 0x197FDF0 ; quick=false mène à 0x196E930, qui énumère les paramètres du matériau cible et interroge la source. SetVectorParameterValue RVA 0x1D97420 transmet un FName et un vecteur de 16 octets. Les types, tailles, offsets et adresses natives sont revérifiés avant appel en jeu.
- Master MM_Basic_Character extrait du PAK local existant : paramètres d'émission et valeurs de texture par défaut examinés, sans téléchargement. Les commutateurs statiques sont ceux du shader Gold cuit, jamais prétendument transférés par la copie d'uniformes.
- Scénarios de test préparés : copie limitée au tissu privé, corps intact, absence de recopies pendant l'état actif, arrivée tardive du template, restauration à l'expiration et repli si contrat natif invalide. **Ils ne sont ni compilés ni exécutés à ce stade.** Aucun résultat 83/83 de V1.164 n'est attribué à ce candidat.
- `git diff --check` et comparaison du périmètre seulement ; pas de test de rendu, FPS ou stabilité annoncé. [Résumé des preuves](evidence/SCARF_TWO_SIDED_PREPARATION_CHECK.json).

La [documentation Epic de la copie](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/Materials/UMaterialInstanceDynamic/K2_CopyMaterialI-?application_version=5.5) explique la hiérarchie et son coût. Elle concerne une version ultérieure ; la présence et le branchement utilisés ici proviennent de l'EXE exact du jeu, pas de cette documentation seule.

### Réussite et retour arrière du futur essai

Critère : face ET envers visibles, silhouette complète sans trous étrangers, feu sur l'écharpe Molten, extinction/restauration normales, skins Native/Abyssal/bonus et changement de skin, sans nouvelle saccade sensible. Contrôler également pause/roues, rappel de monture et fin d'effet. Tant que cela n'est pas testé, l'ancien ZIP V1.164 reste la base validée ; aucun numéro de build, INI, raccourci, ZIP, tag ou release n'a changé.


[Seconde revue avant compilation : skins, sources exactes et repli](SCARF_PREBUILD_SECOND_REVIEW.md). Les nouveaux scénarios restent préparés, non exécutés.
