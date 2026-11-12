# Suite après V1.164 validée — envers de l'écharpe

**PRÉPARATION sur debug, sans build ni modification du code de production.** Main conserve exactement la V1.164 validée. Priorité restante : afficher les deux faces de l'écharpe Stoneskin Strife en gardant son feu, sa forme complète et la restauration du matériau natif.

## Comparaison locale effectuée

42 matériaux déjà en cache ont été examinés ; neuf portent un override double face. Recherche élargie ciblée dans le PAK original : 57 en-têtes de matériaux dont le nom évoque tissu/pierre/feu/lave, 27 de la famille MM_Basic, dix double face. Les deux ensembles se recouvrent ; ce n'est pas un inventaire exhaustif de tous les shaders du jeu. En-têtes et exports des candidats vérifiés contre les blocs du PAK local avec contrôle SHA1 ; aucune copie HTTP ni modification des fichiers du jeu. Douze paires manquantes extraites localement, autres fichiers réutilisés.

Les candidats double face trouvés dans cette sélection sont masqués. Aucun remplacement opaque + double face + feu défilant directement équivalent au matériau actuel n'est démontré. Les variantes Abyssales Stoneskin n'apportent pas l'override recherché.

War_Cloth_Gold_Inst_A et War_Cloth_Silver_Inst_A : parent MM_Basic_Character, double face, BlendMode Masked, Emissive_Multi_Color et Emissive_Multi_Color_Scroll activés dans leur shader déjà cuit. Angel_Inst_B_Cloth_01 combine aussi ces deux commutateurs. Ces permutations constituent des candidats, pas une correction validée.

War_Inst_StoneSkin_Molten_A actuel : parent MM_Basic, opaque, feu via Emissive_Scroll. Les paramètres Emissive_Power_Scroll et Emissive_Scroll_Speed existent aussi dans la famille des tissus ; cela fournit un point d'entrée commun. En revanche les tissus Gold utilisent une texture Emissive_Color_Texture et leurs propres Base_Color/MRE/Normal. Les appliquer directement importerait les UV/masques de War et pourrait refaire disparaître des morceaux de Strife.

## Implémentation à isoler avant un futur build

1. Garder le chemin minéral du corps et la logique d'explosion/cendres intacts. Préserver la référence forte du matériau d'origine, l'identité du mesh/skin et la restauration déjà validées.
2. Essayer un MID privé de tissu double face déjà cuit, en gardant le masque/textures propres à l'écharpe équipée de Strife. Vérifier les paramètres et l'API de copie avant de l'ajouter ; ne pas supposer qu'une copie d'uniformes transfère un commutateur statique.
3. Transférer seulement les valeurs compatibles nécessaires au feu, sans les textures corporelles ou le masque étranger. La variante sans feu doit rester distincte. Vérifier aussi les sources de texture par défaut héritées du parent, pas seulement les overrides du matériau.
4. Avant acceptation : face/envers, écharpe entière, feu actif puis extinction, armures Native/Abyssale/bonus et changement de skin. Une face réparée avec un feu cassé ne répond pas à l'objectif.

Pas de culling global désactivé, de duplication du personnage, de recompilation de shader supposée disponible, ni de nouvelle case sans effet. Le remplacement direct par Gold n'est pas appliqué tant que le problème d'alpha n'est pas résolu. Les analyses privées v293-cloth-parameters.json, v293-two-sided-scan.json et v293-expanded-cloth.json conservent les détails ; la synthèse exportée est dans evidence/V1_164_SCARF_NEXT_CHECK.json.

Le prochain build reste expressément différé. Aucun nouveau binaire, ZIP, tag ou release créé pour cette recherche. [Promotion main contrôlée](evidence/V1_164_MAIN_PROMOTION_CHECK.json).
