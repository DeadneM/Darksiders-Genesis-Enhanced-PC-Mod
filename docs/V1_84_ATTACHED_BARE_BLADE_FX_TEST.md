# V1.84 — effets de mêlée attachés aux mains nues

**V1.84 intermédiaire CONSTRUITE / CONTRÔLÉE : [51/51 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39844030589), zéro erreur, un avertissement MSVC C4324 de padding de Plan. Nouvelle compilation V1.85 prévue après correction de disposition, sans masquer l'avertissement. Effets EXPÉRIMENTAUX, non validés en jeu.** [Archive conservée](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/c58304fd20466e173e0960ca09e47b399455aa7a/downloads/DarksidersGenesis_V1.84_ATTACHED_BARE_BLADE_FX_TEST.zip) · [Reçu](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_84_DELIVERY_RECEIPT.md). Installation manuelle.

22 octobre 2026. Base cumulative V1.83 `81d61dd`, debug uniquement. V1.81 validée dans les trois essais rapportés, suivi TPS des exécutions V1.82 désormais **VALIDÉ par l'utilisateur**. Caméra et bras OFF conservés sans modification. Effets des épées cachées V1.82 inefficaces selon l'utilisateur.

## Objectif et preuve de la cause

**EXPÉRIMENTAL**, compilation et paquet à consigner après CI. Critère : les arcs visuels des épées cachées sont compacts et commencent aux mains, sans changer les effets d'une épée visible, les dégâts ou la portée. Aucun résultat de compilation ne vaut validation visuelle.

Lecture externe VM_READ du jeu déjà ouvert, sans appel natif ni écriture : huit notifications ciblées des deux premiers coups et attaques aériennes portent `bAttached=1`, socket `Bone_Strife_Root`. Elles étaient explicitement exclues par V1.82. Six notifications du troisième coup sont détachées ; sa variante `_03_b` comporte les mêmes offsets et templates mais n'était pas reconnue. Les deux os des mains, leurs indices 20/43, le mesh possédé et ses fonctions virtuelles sont confirmés. Les logs précédents contiennent READY sans trace d'ajustement : cela prouve l'installation du hook, pas son passage effectif sur les coups.

## Correctif limité au rendu

Le contexte existant de notification de particule et de propriétaire local est conservé. Un hook facultatif supplémentaire couvre `SpawnEmitterAttached` `189A510`, ABI de **neuf arguments** vérifié dans l'EXE et son appel natif depuis `169D0D0`. Il préserve template, mesh, FName du socket, rotation, type d'attachement, auto-destruction, pooling et retour. Seules des copies privées de position et d'échelle changent pour un effet exact déjà reconnu et un côté masqué. Aucun objet de notification ou asset partagé n'est modifié.

La transform mondiale de `Bone_Strife_Root` est obtenue par `17A2590`, virtuel mesh `3F0`, ABI mesh/out-transform/FName/space=0. Signature et entrée virtuelle vérifiées, mesh réciproquement possédé et mains contrôlées. L'origine mondiale de la main est exprimée dans ce repère racine par translation, rotation inverse et échelle inverse. Ainsi le parent/socket et l'orientation d'origine de l'arc sont conservés ; sa taille est multipliée par 0,4. Quaternions invalides, échelle singulière, offset inconnu ou parent différent : appel natif inchangé. Le chemin détaché reste limité à la même liste et conserve son transform original hors contexte.

Couverture : cinq séquences de mêlée reconnues plus `_03_b`, trois templates slash/stab exacts. Arc attribuable à une main : seule la lame cachée est concernée. Arc commun : compacté seulement quand les deux lames sont masquées. Effets de pieds, dash, autres classes et autres systèmes laissés natifs. Pas de promesse de couverture de toutes les particules d'un combo. Le contexte exige toujours que l'animation fournie soit l'asset propriétaire de la notification ; ce contrat de callback reste à confirmer lors du nouvel essai. Logs d'ajustement bornés à douze.

## Préservation, nettoyage et lot cumulatif

Pas de second candidat concurrent ni service par frame ajouté. Le chemin manquant rejoint le module existant ; nettoyage des hooks si leur activation échoue. **Exécutions TPS validées, bras OFF animés, modèles et holsters, World Ender, tirs, souris/manette/roues, cheval/saut/sons, ressources et réglages sont inchangés.** Aucun nouveau réglage : 166 valeurs INI et 55 ID Hotkeys conservés, ainsi que 148 anciennes archives, main/archive/v1.43.

Le [prototype V1.83](V1_83_DELIVERY_RECEIPT.md) est inclus sans changement : protection Stoneskin sur Strife normal à pied dans chacun des trois slots. Il reste **partiel/non validé** : aucune onde de choc de lancement, sons ou apparence de pierre transférés. Le slot Strife d'origine doit être débloqué ; son coût est celui de Stoneskin. Les autres cinq transferts sont ouverts. Ne pas présenter le mélange complet comme terminé.

## Essai groupé et rollback

1. Strife à pied, lames gauche/droite/deux masquées successivement : combo au sol et attaques aériennes. Observer origine et taille des arcs, puis remettre Native pour comparer. Avec une seule lame cachée, les arcs communs restent natifs.
2. Contrôle court des bras pistolets OFF, tirs, roues et suivi d'exécution TPS validé. Les effets et la caméra ne doivent pas changer avec les lames Native.
3. Si souhaité, essai du prototype Stoneskin : slot débloqué, Unlimited Wrath et Unlimited Health désactivés, variantes Molten retirées pour la comparaison de base. Vérifier coût, protection réelle temporaire, reprise des contrôles, puis remettre Native. L'aura n'est pas un critère de ce premier transfert.

Installation manuelle, garder son INI personnel. Rollback : ASI V1.82 pour les dernières exécutions validées, ou V1.81 ; toutes les archives restent accessibles. Aucun fichier du jeu remplacé, aucune sauvegarde/PAK modifiée, aucune relance par l'agent. Suite : retour sur les effets, compléter le transfert Stoneskin puis adapter Caltrops vers War. Coop et autres FX P2 non validés, réception lourde encore incertaine.
