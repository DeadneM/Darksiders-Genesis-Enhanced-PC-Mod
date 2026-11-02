# V1.146 — réglages des effets et reprise après chargement

Base : V1.145 (`b339287`), Water sphere retenue par le joueur ; Stinger validé conservé. Objectif : régler séparément la taille et la position du bouclier/Léviathan pour Strife et War, et retrouver les ressources sélectionnées chargées tardivement. Critère de réussite en jeu : réglages visibles sur l'effet actif sans dérive, conservés après chargement, aucun effet sur la protection ou les autres fonctions.

## Changements et périmètre

- Appearance : profils Strife/War indépendants, taille ×0,05–5 et position X/Y/hauteur ±500 unités pour chacun des deux effets. Saisie manuelle et bouton Default habituels. Valeurs initiales 1/0/0/0 ; préréglages de taille V1.145 conservés. 16 nouvelles clés, aucune ancienne valeur/raccourci modifiée.
- Le suivi borné du bouclier applique le réglage à son seul composant de remplacement, après vérification du statut DamageShield, du propriétaire, de l'attache et de l'identité faible. Supprime l'ancien ajustement de création dans le chemin mêlée/Lava. Léviathan garde son attache validée ; les deux utilisent le même petit adaptateur de transformation des seuls composants privés.
- Paramètres vérifiés par réflexion avant appel : SetRelativeScale3D, exec 1DD9E30, vecteur 12 octets ; K2_SetRelativeLocation, exec 1DD8A90, tampon 156 octets, NewLocation0, bSweep12=false, SweepHitResult16/136, bTeleport152=true. Déclarations de l'exécutable inspectées hors ligne ; pas de nouveau hook, aucune écriture directe de transformation. Appels moteur sur le fil jeu uniquement. Contrat manquant : aucun appel. Identités faibles et champs mis en cache ; appels uniquement si valeurs différentes. Baseline distincte des valeurs appliquées : pas de multiplication/translation cumulative.
- Recherche des assets : après un passage complet, nouvelle passe bornée si le choix manque, avec intervalle minimum 5 s ; aucun chargement synchrone ni parcours continu quand le choix est disponible. Corrige le cas démontré par fixture où un asset arrive dans un slot réutilisé sans augmentation du nombre d'UObjects. Invalidation de la classe particule relance aussi la passe.
- Un asset réellement absent reste indisponible : bouclier natif conservé, choix INI inchangé, message explicite. La recherche ne crée pas les ressources de Belial/Dagon/eau. Le remplacement de bouclier est appliqué à la prochaine activation native, pas par réécriture d'un statut existant. Le symptôme après chargement n'est donc pas annoncé intégralement corrigé avant essai réel.

## Préservé / reporté

Stinger, statistiques, coûts, recharge, Stoneskin, poses des bras, visée, XInput, caméras, monture, raccourcis et PAK inchangés. Aucun relevé mémoire du jeu lancé cette reprise. Aucune installation autorisée ; la dernière demande avait été annulée. Main/archive/v1.43 et anciennes archives protégées. ZIP prévu : ASI et dxgi.dll uniquement ; INI personnel conservé, documentation sur GitHub.

Stoneskin intermittent, pose après munition et dash aérien restent ouverts/reportés. Belial/Dagon non chargés dans le niveau observé : leur rendu n'est pas validé. Water sphere retenue ne signifie pas que son retour après chargement est validé.

## Vérifications

Construction uniquement GitHub Actions. Tests nouveaux : transformation sans dérive, reset, changement d'attache natif, refus ABI, identité expirée ; chargement tardif à nombre d'objets constant et délai de relance ; sauvegarde/relecture des quatre profils et bornes. Les résultats de CI/package seront consignés après exécution. Ces tests ne prouvent ni rendu ni performance en jeu.

## Essai groupé

1. Water sphere : modifier taille et X/Y/hauteur pendant que le bouclier est actif, pour Strife puis War. Garder une position immobile 10 s : pas de grossissement/déplacement progressif.
2. Recharger la partie et changer de niveau. Vérifier choix conservé, message de disponibilité, réapparition lors de la prochaine recharge/activation du bouclier. Signaler le niveau et si le message demeure après 15 s.
3. Vérifier que le remplacement disparaît quand le bouclier natif est épuisé, puis revient à sa recharge. Native/Hidden restent fonctionnels.
4. Léviathan : mêmes réglages indépendants, reset, changement de personnage et rechargement ; vérifier pied/cheval/ultime. L'option désactivée restaure le natif.
5. Régression courte : Stinger masqué et Stoneskin pierre/feu ; les deux vues. Coop plus tard selon disponibilité du joueur.

Statut initial : EXPÉRIMENTAL, préparation ; aucune validation en jeu prétendue.
