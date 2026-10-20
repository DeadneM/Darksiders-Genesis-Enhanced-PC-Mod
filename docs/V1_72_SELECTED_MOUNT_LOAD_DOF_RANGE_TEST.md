# V1.72 — choix de monture au chargement et distance DOF

20 octobre 2026, debug uniquement. Base V1.71 construite/installée, acquis validés préservés. Statut de préparation : EXPÉRIMENTAL, compilation Windows et rendu à contrôler.

## Preuves du défaut

Session V1.71 lancée par l'utilisateur, lectures externes QUERY_LIMITED_INFORMATION | VM_READ seulement. À 19:23:51 : Strife Silver, War Gold et Despair figurent ensemble dans le PlayerState, son getter (pas d'alias), le MayhemSaveGame actif et le fichier disque de la partie en cours. Après rechargement à 19:25 : les deux armures restent dans le nouveau PlayerState ; Despair manque aussi au SaveGame actif. À 19:27:46, le fichier disque contient encore Despair. Retour humain : cheval désélectionné, armure conservée. Il ne s'agit donc pas d'une simple erreur de modèle monté ou d'une absence systématique d'enregistrement.

Le corps natif 6A2460–6A261A parcourt SaveGame+240, entrées de 24 octets, à rebours. Il résout chaque ligne ; si bUnlockedByDlc (+60) est vrai et le DLC (+61) absent de l'ensemble lu via le second paramètre+AD0, il enlève l'entrée. Appel direct 6C3F49, retour 6C3F4E. Les copies 582887 (SaveGame vers profil) et 574FFB (profil vers PlayerState) sont de simples copies ; OnRep 578250 diffuse et nettoie des références faibles, pas ce filtre DLC. Le hook de disponibilité IsSkinUnlocked ne couvre pas ce chemin.

## Candidat limité

Un seul nouveau hook, sur ce filtre, distinct du hook IsSkinUnlocked validé. Exécutable exact exigé, contrôles du prologue, du test DLC interne, de l'appel direct et du setter SetSkin 617A50. Contrôle des classes MayhemSaveGame et MayhemGameInstance, listes bornées à 32 et types uniques. Les valeurs natives des lignes sont MAYHEM_BP=8, RUIN_BP=6.

Avant l'appel, retenir seulement les choix Despair/Rampage exacts (table, package, nom et type) déjà présents. Le filtre original s'exécute entièrement, exactement une fois. Ensuite, si l'option est toujours cochée, restaurer seulement les types devenus absents via SetSkin sur cette même liste. Une autre sélection native de ce type reste prioritaire. Listes incohérentes, classes inconnues, mauvais appel, réentrance ou option OFF : aucune restauration. Aucun pointeur conservé après le callback ; aucune recherche par image, écriture de fichier, modification des droits DLC ou des tables partagées. Le travail reste dans l'appel synchrone qui modifie déjà cette liste et utilise le même allocateur natif ; pas de service de personnage ni d'appel retardé.

Le périodique SELECTION_READ_ONLY V1.71 et son en-tête sont retirés maintenant que la cause est localisée. Un message borné SELECTED_BONUS_KEPT confirme une réinsertion, sans certifier le rendu.

## Distance et contrôles

Limite centralisée 5 000 m, mêmes bornes au menu, chargement INI et sortie DOF. Conversion m × 100 vers la région native ; la mise au point du personnage, la géométrie et les garde-fous de distance réelle de caméra restent inchangés. Ne pas relever les garde-fous de pose en confondant région nette choisie et distance caméra/personnage.

Les simulations existantes couvrent maintenant : filtre une fois, deux bonus, conservation armure, retrait d'un DLC étranger, OFF, choix déjà admis nativement/PAK sans doublon, autre choix natif prioritaire, types ambigus/corrompus, mauvais appel/classe, réentrance, désactivation pendant l'appel, état DLC/tables inchangé. Les contrôles DOF couvrent 1 000 / 1 250,5 / 5 000 m au consommateur et sauvegarde/relecture. Ces simulations ne sont pas une validation du jeu.

## Limites et essais

Reprendre la liste groupée en tête de CURRENT_STATE. Sélection de monture, nouveau maximum de distance et callback de chargement encore non validés dans cette version. Retour sur le flou V1.71 : « ça a l'air ok », à limiter à cet essai. Recovery sans nouveau verdict. OFF complet pistolets non livré ; visée et notifies intacts. HUD général intermittent, autres FX P2 reportés.

Rollback : archive V1.71 préservée, INI personnel à conserver. Aucun lancement/rechargement par l'agent. Aucun asset original extrait n'entre dans Git ou le ZIP. Références privées des captures et désassemblages conservées dans l'audit local.
