# Capacités étrangères — contrats suivants, étude séparée

22 octobre 2026. Complément pendant la CI V1.85. Aucun de ces contrats supplémentaires n'est une nouvelle fonction de l'ASI ; aucun appel natif ni écriture externe n'a été réalisé dans le jeu.

## Compléter Stoneskin

Le prototype V1.83/V1.85 conserve le statut protecteur et son coût, mais la séquence de repos privée ne contient pas les notifications du cast War. Les séquences War Stoneskin et Molten portent notamment `AnimNotify_SpawnBlast` → `MayhemBlast_Stoneskin_Level1_Blueprint_C`, particules, sons et notifications de matériaux propres à War. On ne peut donc pas annoncer onde de choc ou pétrification visuelle à partir de la seule application du statut. Les casts spécifiques de matériaux War doivent rester exclus du receveur Strife. Étape suivante : mesurer la protection et la fin du cast, puis adapter séparément les notifications compatibles sans toucher aux animations partagées.

## Caltrops vers War

Le relevé des timelines distingue les séquences sol avant/arrière :

| Séquence | Création du spawner | SetFalling |
| --- | ---: | ---: |
| Strife_Atk_Glyph_F | 0,285939 s | 1,301404 s |
| Strife_Atk_Glyph_B | 0,262978 s | 1,301404 s |

TriggerTimeOffset=0 pour ces notifications. Ces instants viennent des assets ; ils ne sont pas des durées observées en jeu. Les variantes aériennes restent à traiter séparément.

`SpawnCaltropsSpawner` : l'exec `7AF460` lit deux paramètres et appelle le corps `5AE2B0(ability, spawnerClass, FName)`. Son désassemblage dans l'EXE supporté confirme un contrôle Character `+1B0`, rôle 3, classe de spawner valide ou défaut `+1E0`, puis appel `687B30`. Ce corps ne contient pas de cast spécifique vers Strife. Cela ne prouve ni les autres arguments de `687B30`, ni le rendu final sur War.

Le Blueprint `DoSpawnCaltropsSpawner` compare le montage courant aux objets originaux Air_F/Air_B et utilise `Bone_Strife_Pelvis`. Le remplacer simplement par un repos compatible ferait perdre les notifications et la distinction aérienne. OnStart conserve la vélocité puis active le mode Flying, SetFalling et OnFinish gèrent le retour : omettre ce lifecycle risquerait de bloquer le déplacement.

Avant d'exposer ce transfert : source d'animation et os du receveur vérifiés, variantes sol/air explicitement transportées, création unique du spawner au bon instant, retour du mouvement et coût natifs, annulation propre, ownership P1/P2 indépendant. Garder les ID du slot d'origine et le composant du receveur ; ne pas prendre ceux de l'autre joueur. Aucun sélecteur Caltrops→War présenté fonctionnel dans V1.85.
