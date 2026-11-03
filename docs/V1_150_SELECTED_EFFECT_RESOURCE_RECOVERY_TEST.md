# V1.150 — reprise des ressources visuelles sélectionnées

**Base cumulative : V1.149 ; correctif sous-titres V1.148 VALIDÉ conservé. Préparation EXPÉRIMENTALE sur debug, pas encore validée en jeu.**

Objectif : reprendre un effet sélectionné et le revêtement Stoneskin de Strife quand leurs ressources manquent après un chargement ou un changement d'armure. Réussite en jeu : choix conservé, effet retrouvé à la prochaine utilisation, pierre/feu présents après changement d'armure et restaurés à l'expiration, sans ralentissement permanent.

## Changement limité

L'ancienne bibliothèque cherchait seulement les objets déjà chargés. Elle ne pouvait donc pas rendre disponible un asset absent du niveau. Le nouveau chemin utilise la même API publique vérifiée MakeSoftObjectPath/LoadAsset_Blocking que le bouclier validé, pour les seuls chemins du catalogue sélectionné. Retour vérifié par classe, nom, paquet et identité UObject. Native/Hidden n'entraînent pas de chargement. Les émetteurs déjà vivants gardent leur cycle natif et peuvent conserver leur aspect jusqu'à la prochaine utilisation.

Pour Stoneskin : corps minéral de base, tissu du statut actif, explosion correspondante et cendres seulement. La variante inutilisée n'est plus une dépendance de disponibilité. Demandes séparées par joueur, acteur, armure, monde et variante ; protections des matériaux natifs/utilisateur et restauration inchangées.

Chargements sur le thread du jeu, hors pause/cinématique/roue/menu du mod/Alt-Tab. Stabilisation de750 ms, au plus un nouveau chargement partagé toutes les250 ms ; trois tentatives par ressource espacées de5 secondes. Une nouvelle sélection, une nouvelle identité ou une ressource observée puis déchargée réarme le budget. Le chargeur du bouclier conserve son cycle validé ; seul son contexte moteur déjà vérifié est partagé. Aucun hook supplémentaire, aucun préchargement massif au démarrage, aucun chargement dans la fabrique des émetteurs.

## Preuves et limites

Les460 chemins du catalogue et les6 ressources Stoneskin sont présents dans l'index natif hors ligne. Cela prouve leur existence, pas leur rendu ni leur compatibilité parfaite. Un chargement synchrone initial peut provoquer une courte pause ; aucune mesure FPS ni garantie de correction universelle du défaut intermittent. Un matériau remplacé par le jeu reste protégé. Un échec garde le natif et le menu l'indique après les tentatives limitées.

Deux passes : (1) comparaison du périmètre et des valeurs conservées, (2) revue des appels moteur, identités, budget et invalidations. La seconde a conduit à séparer les demandes Stoneskin P1/P2, limiter les recherches de reprise à250 ms et retirer la dépendance à l'explosion inutilisée. Pas de retrait de code validé.

Correction documentaire : dans la preuve V1.148,0x128 octets valait296 et non312 en décimal. Le code a toujours copié0x128 ; garde sous-titres strictement inchangé.

## Test groupé

1. Après chargement, garder un effet de cœur/bibliothèque sélectionné, fermer le menu, attendre quelques secondes et utiliser son pouvoir. Vérifier l'absence de « assets not ready » durable, puis recharger et recommencer. Léviathan et bouclier sont des contrôles de non-régression.
2. Strife à pied : Stoneskin, expiration, changement d'armure (dont Abyssal), nouvelle activation. Vérifier pierre/feu, explosion/cendres et retour au skin normal. Comparer un niveau où le défaut existait.
3. Vérifier qu'une ressource déjà prête ne provoque pas de pause répétée. En cas d'échec, conserver le journal ; les tentatives sont nommées et bornées.

Coop toujours à vérifier en dernier. Pose de tir après changement de munition : ouverte, attendre le prochain défaut spontané sans imposer de reproduction. Dash aérien : règle demandée confirmée, stomp sans déplacement, dash directionnel au contact si déplacement maintenu ; chaîne native Strife identifiée, aucun changement mouvement ajouté dans ce lot. V1.149 GPU et trait/arc Moloch restent à tester. Aucun fichier du jeu remplacé, INI/raccourcis/sauvegardes/PAK conservés ; aucune promotion main/archive.
