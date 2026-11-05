# Recherche d'apparences natives — 5 novembre 2026

Priorité demandée après acceptation de V1.153. Lecture du PAK principal installé, sans modification du jeu, de ses réglages ou de ses sauvegardes. L'index a été relu et son SHA1 vérifié : `5d0982cd46a5221290cb6ed24696e3962e9dddff`, identique au catalogue précédent. Ce relevé étend la recherche existante ; il ne prétend pas recenser de nouveaux skins cachés dans tout le jeu.

## Huit apparences de poussière de dash confirmées

Les huit exports ci-dessous sont de vrais ParticleSystem, avec modules entièrement décodés dans le périmètre examiné. Ils sont déjà reconnus dans la bibliothèque du mod, mais ne disposent pas d'un choix individuel. Ils sont actuellement couverts par Native/Hidden de la famille Other Strife effects.

| Apparence proposée | Export natif |
| --- | --- |
| Poussière de base | P_StrifeDash_Default |
| Pièces | P_StrifeDash_Coins |
| Métal / étincelles | P_StrifeDash_Metal |
| Sable | P_StrifeDash_Sand |
| Neige | P_StrifeDash_Snow |
| Pierre / débris | P_StrifeDash_Stone |
| Eau | P_StrifeDash_Water |
| Bois / débris | P_StrifeDash_Wood |

**Ils sont partagés par Strife ET War.** Les tables m_SurfaceOverrides des notifications natives les affectent aux surfaces du sol. Par exemple, Strife_DashTo_Back_Montage et War_Dash_F_Level1_Montage utilisent tous deux eau, pièces, métal, pierre, bois, neige et sable. Le nom du fichier Strife ne suffit donc pas à attribuer l'effet à un seul héros.

Le relevé porte sur 57 packages d'animation et 36 notifications pertinentes, sans limite de décodage restante. Le premier lecteur ignorait MapProperty ; il refusait alors les tables de surfaces. Le lecteur privé a été complété pour leur couple ByteProperty/ObjectProperty, avec contrôle exact de taille et de références. Les résultats incomplets ont été remplacés après relecture.

Ces effets sont des éléments de rendu ; ils ne constituent ni un nouveau dash ni un changement de durée, de collision ou de dégâts. Les effets de chute, de réception et le flou de déplacement sont distincts et ne doivent pas être remplacés avec la poussière. Les différentes couches d'une même animation peuvent émettre plusieurs effets : le futur choix devra en tenir compte pour éviter des nuages superposés.

## Autres candidats et exclusions

Les exports P_WarDash_WS, P_WarDash_Coins_WS et P_WarDash_Water_WS sont vérifiés, mais leur consommateur natif n'a pas été établi par cette recherche bornée d'animations. P_WarDash est une couche attachée de flou avec durée différente ; ce n'est pas l'équivalent de leur poussière. Pas d'option interchangeable annoncée pour ces fichiers à ce stade.

Ghost Hook conserve ses particules d'attache, poignet et rayon ; les variantes actives/passives trouvées autour de ses points d'accroche concernent les éléments du décor. Pour Void Bomb, projectile, ciblage, rebond, explosion et rayon sont des phases différentes. Les matériaux Beam_01/02, les éléments de chaîne ou les matériaux de dissolution ne prouvent pas l'existence d'un skin complet supplémentaire. Aether Spark possède déjà ses variantes idle et hip dans la bibliothèque. Les variantes Vorpal, Tremor, Caltrops et les huit armures natives déjà disponibles ne sont pas comptées comme de nouvelles découvertes.

## Implémentation préparée, non livrée

Ajout concret possible : un choix **Ground dash dust (both heroes)**, Native / les huit rendus / Hidden, dans la bibliothèque d'apparences. Réutiliser uniquement la substitution de ParticleSystem existante, sans nouveau hook et sans changer les paramètres du dash validé. Le réglage Native doit rester le défaut et les anciens numéros/clés de choix doivent garder leur sens. En particulier, déplacer ces huit assets hors d'Other Strife effects sans compatibilité ferait perdre le masquage demandé par un ancien INI : cette migration doit être traitée avant toute livraison.

**Aucun nouveau choix au menu ou ZIP n'est publié par ce relevé.** Les fichiers existants et Léviathan sont inchangés. La substitution, le nombre de couches et le rendu final restent à valider en jeu dans les deux vues ; les exports ne prouvent pas le résultat visuel. [Preuves compactes : exports, SHA1, modules et notifications natives](evidence/NATIVE_DASH_APPEARANCES_2026_11_05.json).
