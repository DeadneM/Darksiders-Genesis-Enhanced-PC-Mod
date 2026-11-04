# Trait/arc Moloch — variantes natives de matériau, V1.151

**Essai expérimental demandé le 4 novembre 2026.** Les réglages de visibilité, interfaces de dessin et chemins de visée restent identiques. Aucun shader arbitraire, paramètre de projectile ou nouveau hook ajouté. Le résultat dans Moloch reste à confirmer par l'utilisateur.

## Preuve hors jeu

Les deux CRC32 capturés historiquement avec ShaderToggler sont retrouvés exactement dans les shaders natifs extraits du PAK, avec le même calcul sur le bytecode DXBC complet :

| CRC historique | Matériau natif propriétaire | Variantes pixel natives |
|---|---|---:|
| 1212955419 (0x484C3B1B) | Particles/Materials/VFX_beam_aimArc | 112 |
| 2294978555 (0x88CA97FB) | Particles/Materials/VFX_beam_aimBeam | 56 |

Les anciens noms de constantes line/arc étaient inversés par rapport à ces propriétaires. L'ancien filtre ne connaissait que ces deux CRC : les autres variantes du même matériau restaient visibles. Cela fournit une explication vérifiable de sa couverture partielle, sans prouver quelle variante exacte a été affichée dans Moloch.

Extraction des entrées validée par taille et SHA1 PAK. Les 40163 entrées natives .uexp/.bin/.ushaderbytecode ont été contrôlées, flux zlib internes compris : 1157203 en-têtes DXBC trouvés, tous les 168 shaders cibles retrouvés, aucun de leurs CRC présent dans une autre entrée. Scan en lecture seule, aucun fichier du jeu modifié. PAK externes/modifiés exclus de cette preuve. [Catalogue, provenance et limites](evidence/V1_151_AIM_SHADER_CATALOG.json).

## Changement

À CreatePixelShader seulement, recherche dans la table triée des 168 CRC. La taille et une empreinte FNV1a64 du bytecode complet doivent également correspondre avant enregistrement du shader COM. Aucun bytecode du jeu n'est distribué. La capacité COM bornée est portée de64 à336 pour couvrir les variantes ; la vérification de durée de vie par AddRef et le filtrage par contexte sont conservés. Aucun calcul de nouvelle empreinte ajouté par appel de dessin.

Les modes Hide Everywhere / Default Only / TPS Only / Never, le contrôle du shader réellement actif avant de masquer et les sept hooks de dessin restent identiques. La reconnaissance des deux anciens shaders est conservée avec preuve de propriétaire ; compteurs distinguant désormais Beam et Arc correctement.

Suite AimShaderCatalog préparée pour GitHub : table triée, deux captures conservées, nombre par matériau, CRC inconnu refusé, taille/empreinte et modification d'un octet refusées. Cela ne remplace pas l'observation de l'image.

## Test prioritaire

Dans Moloch, Strife à pied : TPS Only doit masquer le trait ET l'arc en TPS, puis les rétablir en vue native. Hide Everywhere doit les masquer dans les deux vues ; Never doit les conserver. Vérifier que tirs, effets des capacités et éléments de scène restent présents. Comparer un autre niveau déjà validé. Si le défaut persiste, le journal distingue les variantes reconnues : ne pas élargir à d'autres matériaux à l'aveugle.
