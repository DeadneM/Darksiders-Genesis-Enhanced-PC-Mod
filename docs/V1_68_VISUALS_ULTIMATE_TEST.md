# V1.68 — apparences, continuité de focus et durée ultime

20 octobre 2026. **CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL** : 43/43 suites Windows x64, zéro avertissement MSVC ; résultat en jeu non validé. [Reçu de livraison et signatures](CURRENT_STATE.md). Base : V1.66 validée pour sauts/holsters, plus les sélecteurs de pistolets V1.67 explicitement validés par l’utilisateur. Les nouveaux comportements ci-dessous attendent un essai dans le jeu. Branche debug exclusivement.

## Objectif et critère de réussite

Corriger la disponibilité des skins sans toucher à leur état équipé, maintenir la mise au point du cadrage TPS pendant un changement de personnage, proposer une durée ultime infinie distincte du remplissage et les trois visuels natifs de Léviathan sans leur bonus de gameplay. Réussite en jeu : boutons bonus sélectionnables, aucune pulsation de flou au changement, arrêt/reprise du débit ultime à niveau courant, choix visuel indépendant et retour natif sans doublon.

## Correction de l’erreur V1.67

La table d’enregistrement native contient des couples **nom puis fonction**. V1.67 a lu la fonction précédant `IsSkinUnlocked`, donc a intercepté le chemin `IsSkinEquipped` : wrapper `7C4390`, pont `574860`, manager `60BF90`. Les tests de liste blanche et d’ABI ne suffisaient pas à distinguer ces deux fonctions. L’erreur de provenance est reconnue ; les affirmations contraires du rapport V1.67 sont remplacées par cette correction.

V1.68 **retire ce branchement** et utilise seulement `574870(PlayerState, const RowHandle*) -> bool`. Chaîne vérifiée : nom à `27D7870`, wrapper suivant à `27D7878 = 7C4440`, appel direct `7C44C7 -> 574870`. La garde compare nom, couple d’enregistrement, destination de l’appel et prologue. Les tests rejettent explicitement le voisin « équipé » et une destination erronée sous le bon nom. Liste blanche inchangée : deux montures bonus et six variantes Abyssal/Silver/Gold ; groupes indépendants, aucun grant, statistique ou droit DLC écrit.

**Preuve en lecture seule sur la nouvelle partie du 19 octobre :** quatre tables natives reconnues, deux lignes bonus avec `UnlockedByDefault=1`, PAK existant effectivement chargé. Le journal seul ne permettait donc pas de conclure à son absence. La mauvaise réponse « équipé » du mod peut expliquer le bouton indisponible ; la sélection réparée n’est pas encore validée. Le PAK et l’INI personnels restent intacts. Tant que ce PAK reste actif, un essai réussi ne prouvera pas l’autonomie de l’ASI.

## Flou : portée actuelle et candidat limité

Le mod corrige la distance focale des vues TPS. Avec **TPS lens focus**, il force aussi CircleDOF=2, ce qui peut rendre les décors plus nets que les régions Gaussian prévues par un niveau. Décocher cette option conserve la correction de distance mais rend la méthode native. Ni l’ouverture ni les volumes du niveau ne sont modifiés. L’ancien secours peut couper globalement le DOF pour le zoom natif modifié ou un layout TPS indisponible ; ce secours n’était pas actif dans la capture (qualité jeu/rendu **2/2**).

La seconde capture a suivi le contrôleur après chargement. Lors d’un changement, environ **0,64 s** de géométrie TPS (FOV 110) ont conservé la focale native 1000 alors que le personnage était à une profondeur proche de 329. Le candidat applique le focus au **même point joueur déjà adopté pour émettre cette géométrie**, même si le composant caméra appartient encore brièvement au personnage précédent. Aucun nouveau hook ni changement de FOV, angle, méthode ou aperture. Le résultat visuel reste à tester ; cela ne prouve pas que toutes les causes de flou sont résolues. Le signalement concerne désormais plusieurs niveaux, pas seulement l’arène.

## Durée ultime infinie

Case **Infinite Ultimate duration**, indépendante du multiplicateur 0–10 et d’**Unlimited Ultimate**. Elle transmet zéro au seul débit positif de la forme déjà active, au niveau courant de jauge. Elle ne remplit rien, ne rend pas l’activation gratuite et ne modifie pas les gains. Décocher reprend le multiplicateur si sa case est active, sinon le débit natif. P1/P2, pause, cinématique, propriété réciproque et retour float natif conservés. Nouvelle action Hotkeys **53**, sans renumérotation des anciennes (54 identifiants au total), non attribuée par défaut.

## Léviathan : visuel seul

Case **Override Leviathan lightning** décochée par défaut : comportement natif. Une fois cochée, liste **OFF / Effect 1 / Effect 2 / Effect 3**. Les trois classes natives de status référencent respectivement `P_Dagon_CreatureCore_Status_Lvl1/2/3`, dans le paquet `/Game/VFX/Particles/Characters/Dagon/`. Aucun octet d’asset n’est livré.

L’ASI utilise seulement le chemin natif de particule attachée `execSpawnEmitterAttached 1D19B50 -> 189A510`, avec ses trois vecteurs par référence et ses neuf arguments vérifiés. Pas de cœur, status, statistique, récompense ou sauvegarde accordé. Un composant visuel propre par joueur/pose ; destruction limitée à ce composant et son acteur propriétaire par le chemin natif `163EFA0`. Les seules particules natives masquées sont les trois templates exacts de Léviathan appartenant au même joueur ; visibilité originale rendue à la désactivation. Aucun parent ni autre effet P2 masqué.

Recherche des seuls assets déjà chargés, bornée à un passage de table par changement de sélection/propriétaire, sans chargement forcé. Si l’effet demandé manque, avis dans le menu et visuel natif conservé. Trois tentatives de création maximum par sélection/pose, puis arrêt et retour natif. Aucune nouvelle interception. **EXPÉRIMENTAL** : rendu, placement et cycle pied/cheval/ultime non validés en jeu ; les contrôles d’ABI et les fixtures ne les valident pas.

## Tests groupés et rollback

1. **Skins :** conserver le PAK actuel pour le premier essai ; fermer/rouvrir la page, activer Bonus mount skins, sélectionner les deux montures puis une autre apparence. Progression séparément. OFF ne doit pas faire croire que tout est équipé. Choisir une apparence possédée avant de retirer son groupe.
2. **Focus :** dans l’arène puis un autre niveau, rester immobile en TPS et changer Strife/War quatre fois, sans bouger la caméra. Comparer ensuite TPS lens focus ON/OFF et vue native. Signaler flou bref au changement et rendu général séparément. Une capture externe ne sera lancée qu’après confirmation que le jeu est prêt.
3. **Ultime :** Unlimited Ultimate OFF ; transformer avec une jauge non vide, cocher Infinite Ultimate duration, attendre, décocher. La jauge doit rester à son niveau courant puis recommencer à baisser. Vérifier le raccourci assigné et le multiplicateur retenu, puis P2 si disponible.
4. **Léviathan :** tester les trois effets puis OFF à pied ; décocher pour rendre le visuel natif. Tester ensuite cheval/ultime et P2 si disponible. Vérifier absence de doublons et statistiques inchangées. Si un effet est annoncé indisponible, le résultat n’est pas une validation du rendu ; changer la sélection/réactiver permet un nouveau passage borné.
5. **HUD, diagnostic ouvert :** depuis un HUD visible, ouvrir le choix des missions, sélectionner une arène verrouillée, revenir en jeu et relever ce qui manque. Comparer l’état avant/après et la valeur native `ui.HideHud` ; pas de réaffichage global forcé. Le signalement d’autres éléments manquants reste incertain.

Revenir au ZIP V1.67 pour retirer le nouveau lot **en désactivant ses deux cases de skins défectueuses**, ou V1.66 pour retrouver la base avant les apparences. Toujours conserver l’INI personnel. Aucun lancement ou installation automatique dans cette préparation. Toutes les anciennes archives et les branches main/archive sont conservées.

## File restant ouverte

- HUD après sélection d’arène verrouillée : reproductible selon l’utilisateur, cause non observée.
- Rendu général du DOF : impression de flou désactivé, à comparer indépendamment de la transition ; pas de revendication de fidélité native en mode lentille.
- OFF des pistolets : demande complète = garder les projectiles, retirer modèle/effets et bras tendu. Pas livré ici. Les couches d’animation inspectées partent de la colonne entière ; aucun contrôle par bras sûr identifié. Ne pas ajouter une option prétendant tout faire alors qu’elle masque seulement le modèle.
- Recovery natif : étude ouverte, ancien système retiré. Autres icônes/effets P2 toujours reportés sur demande, sauf le nouveau sélecteur Léviathan explicitement demandé.
- Auto check update : confirmation visuelle distincte toujours absente.

## Reproduction HUD du 20 octobre après la construction

La panne a été reproduite et observée sur la **V1.67 encore chargée**, indépendamment de la compilation V1.68. Valeurs natives HideHud 0/0 et booléen mod masqué 0, bShowHUD vrai, racine générale présente/visible/opacité 1. Les contrôles déjà présents n’expliquent pas la disparition. Étude du rattachement/affichage du HUD à poursuivre. Aucun correctif HUD ajouté au paquet immuable déjà publié. Captures privées uniquement, aucune modification de mémoire.

Contrôle complémentaire : IsInViewport natif renvoie encore vrai dans l’état cassé (lecture des deux pointeurs et du compteur partagé = 1, sans appel du jeu). La piste « HUD simplement détaché du viewport » ne suffit pas ; poursuivre les états d’affichage et bindings/couches Slate.
