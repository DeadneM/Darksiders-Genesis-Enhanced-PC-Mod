# V1.94 — présentation de Blade Geyser sur Strife

**V1.94 INTERMÉDIAIRE CONSERVÉE : 54/54 suites et zéro avertissement MSVC, mais suffixe d’archive resté sur Caltrops. Remplacée par V1.95 avec le même code de jeu et un nom de ZIP centralisé. Aucun résultat en jeu supposé.** Base expressément validée V1.87 pour les acquis rapportés, cumul V1.93. Objectif unique : rétablir la présentation compatible omise par le montage Geyser privé ; succès attendu : son de lancement et lames/nova visibles au bon endroit, une seule attaque, fin normale, Geyser de War intact. La pose reste celle de repos Strife_Idle ; ce candidat ne prétend pas transférer le geste de War ni résoudre une absence de dégâts. Installation manuelle, garder son INI/raccourcis/PAK/sauvegardes. P2 plus tard.

## Preuve et changement

[Comparaison réelle du 24 octobre](GEYSER_COMPARISON_2026_10_24.md) : entrée/sortie SpawnBlast War puis Strife, événements d’un objet de blast par cast avec propriétaire correct ; aucune mesure de dégâts. Relecture du code et nouvelle extraction vérifiée des assets : les notifications de présentation étaient perdues lorsque le montage ne conservait que SpawnBlast sur Strife_Idle. Le test synthétique exigeait lui aussi un seul événement, sans tester la conservation de ces effets.

Le nouveau montage réunit le callback exact SpawnBlast et une liste précise d’effets BladeGeyser/lames lointaines/nova avec le seul son char_war_wrath_bladegeyser. Ordre stable et temps natifs conservés sur la durée 2,366667 s ; pas de second blast, minuterie ou écriture de dégâts/coût. Les voix, balayages et effets sur l’épée de War sont exclus. Les effets sans socket restent natifs. Seuls ceux attachés à Bone_War_Root sont copiés en objets transitoires privés, appartenant au montage, puis liés au Bone_Strife_Root du propre mesh de Strife. Template, offsets, taille, attachement et quaternion de rotation natifs sont conservés. Les assets et notifications sources, CDO et corps de War ne sont pas modifiés.

La liste privée accepte au plus 64 événements. Son tableau est agrandi uniquement via le helper natif 2C42C0 : deux arguments tableau/nouvelle capacité, taille d’élément 184 octets et réallocation UE 83ED00 avec alignement 0, démontrés hors jeu dans l’exécutable exact et dans le dispatcher de notifications 167D020. Signature contrôlée avant disponibilité du transfert. Aucun buffer CRT n’est donné au destructeur Unreal. Les pointeurs des tableaux source/copieur restent distincts ; seuls tableaux/notifications privés sont écrits. Un contrat indisponible termine nativement le cast et bloque ce seul transfert jusqu’au redémarrage, comme avant ; son premier coût peut déjà avoir été consommé.

Les deux hooks Wrath existants ne changent pas ; aucun nouvel hook ou timer. Le diagnostic borné SpawnBlast reste temporairement présent pour vérifier l’unicité du callback dans ce candidat et sera retiré après comparaison. Source Stoneskin, Caltrops et pouvoirs propres inchangés. Aucun réglage ni raccourci ajouté ou modifié.

## Contrôles et limites

ForeignMontage vérifie conservation son/nova/lame et temps/ordre, un callback, exclusion des effets d’épée, racine propre et offsets/quaternion, sources inchangées, allocation privée native, échec de réserve/alias/clone/CDO, sockets inconnus, durée/état/timing invalides, doublons et borne. ForeignWrath vérifie transmission de la racine du propre mesh et arrêt isolé sans affecter Stoneskin si elle n’est pas prouvée. Les fixtures testent des contrats ; elles ne prouvent ni affichage ni dégâts en jeu. Les autres suites Windows et l’analyse MSVC restent requises.

L’animation de lancement est encore une pose de repos pendant le cast ; ne pas la confondre avec un blocage persistant après la fin. Le coût doit être testé en mission et non déduit du Néant. Caltrops sur War V1.93 est conservé expérimental, sans validation supplémentaire supposée. Clone/World Ender vers War et Rampage vers Strife restent indisponibles ; réception lourde incertaine, autres FX P2 reportés.

## Essai après installation manuelle

1. Garder son INI et préparer un emplacement Strife sur Blade Geyser, à pied en mission près d’ennemis.
2. Vérifier son/lames/nova, position autour de Strife, dégâts et Courroux consommé, puis déplacement/attaque de nouveau disponibles après le cast. Signaler séparément présentation, dégâts et fin.
3. Vérifier une variante améliorée si disponible ; faire ensuite Geyser avec War sur Native, puis remettre Strife sur Native et essayer son pouvoir habituel.

Si aucun effet, crash, mauvais placement ou blocage persistant, remettre Native et garder le journal sans multiplier les activations. Aucun jeu lancé ni fichier installé automatiquement. Rollback : Native, ou restaurer son ASI antérieur en gardant l’INI ; V1.87 demeure la base validée dans les cas rapportés. ZIP cumulatif de quatre fichiers racine : ASI, dxgi.dll, INI, README. Archives et branches main/archive/v1.43 conservées.
