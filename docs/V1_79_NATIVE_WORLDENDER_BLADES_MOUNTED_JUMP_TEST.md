# V1.79 — bras OFF, épées, World Ender natif et saut à cheval

Branche debug seulement. Base cumulative V1.78 (`5d4883e`), en conservant les acquis rapportés sur V1.73 et les versions suivantes. Objectif : un lot cohérent d'apparence et d'actions natives, sans cumuler des corrections contradictoires de visée. Les nouveautés sont **EXPÉRIMENTALES** jusqu'aux essais utilisateur. Le reçu de livraison distingue compilation, contrôles et validation en jeu.

## Retours pris en compte

- V1.78 : maintien de OFF dans les roues et après ultime jugé bon ; groupes fermés par défaut et Never acceptés. Coop différée.
- Bras OFF encore incorrects, surtout à gauche ; côté des holsters à inverser.
- Demande de masquer les deux épées de Strife indépendamment.
- World Ender : dégâts et effet créés ; visible en vue native et avec caméra TPS reculée. En TPS, rayon pouvant partir à l'opposé des canons lorsque la caméra tourne. Le recul confirme une dépendance à la vue/proximité ; il ne prouve pas à lui seul une cause précise de matériau ou de culling.
- Demande de saut à cheval par la commande native, avec case dédiée.

## Changements construits dans le candidat

1. **Bras OFF** : les deux branches complètes, clavicule comprise, sont calculées de la même façon relativement à Spine2. La gauche suivait encore sa clavicule animée de visée. Matrices de rendu courantes/précédentes seulement ; aucun changement de pose native, socket, trajectoire ou action. Capturer la pose rangée à pied en restant brièvement immobile. Les animations de corps à corps, pouvoirs et monture gardent leur chemin natif. Le résultat après toutes les actions reste à tester.
2. **Holsters** : inversion des deux feuilles de rendu selon le côté réellement vu par l'utilisateur. Noms natifs inchangés ; masquage limité aux triangles de ces deux feuilles.
3. **Épées de Strife** : Appearance → Strife melee blades → Native / Hide left blade / Hide right blade / Hide both blades. `StrifeBladeVisibility=0` par défaut. Deux composants exacts `Dagger_R` / `Dagger_L`, propriétaires/parents/classes contrôlés ; intentions natives de visibilité restaurées séparément. Attaques, dégâts, effets et animation des mains conservés. À pied, forme normale, pour chaque joueur local.
4. **World Ender** : le hook de direction des faisceaux ordinaires exclut maintenant le projectile réellement créé par le composant `MayhemWorldEnder_Blueprint_C`. Le rayon reprend sa direction native, cohérente avec l'animation et les canons, plutôt qu'une direction de caméra indépendante du corps. La provenance couvre aussi les tailles/améliorations Ice Beam. Même règle en 3D aiming ON/OFF. Les munitions électriques/magiques ordinaires gardent leur correction validée. Aucun recul automatique, modification de caméra, matériau, largeur ou dégâts ajouté. **La visibilité à caméra proche reste à confirmer ; aucune réparation complète du rendu intérieur n'est prétendue.**
5. **Saut monté** : Gameplay → Movement → Jump on horseback (experimental), `MountedJump=0` par défaut. Lecture de l’action réaffectable `Jump` dans les InputSettings natifs et des états de commande du PlayerInput de chaque contrôleur. Un nouvel appui au sol demande le DoJump natif du composant de déplacement de la monture. Aucun nouveau hook : la capture a montré que le chemin d’action JUMP 30 de la monture n’était pas appelé par les commandes existantes, et la première implémentation fondée sur ce chemin a été supprimée avant construction. Relâchement exigé après activation, changement de monture, menu, focus ou réaffectation. Les associations avec modificateurs ne sont pas élargies en touches simples ; la page native utilise des associations simples dans les 120 entrées relevées. Commandes clavier/manette réaffectables du jeu conservées. Pas de touche imposée, d'entrée simulée, de compteur de saut ou de vitesse écrit par le mod. Contrôles de propriété, possession réciproque, classe, rôle local, sol, despawn, pause/roue/cinématique. Animations/atterrissage et les deux montures restent à valider en jeu.

## Preuves et contrôles

- EXE Steam x64 exact : `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`.
- Capture externe VM_READ : **2 378 échantillons, zéro erreur**, poses et effet World Ender actif. Composant natif confirmé : arme → SpawnedProjectile → MayhemBeamEffect → m_pParticleSystemComponent. Visibilité, activation et attachement à la mesh de Strife présents. Le moteur crée le rayon ; le défaut n’est pas une absence de création. Corrélation horodatée : première référence de rayon à 23:38:34.061, logs BEAM_FORWARD n=1 à 12 entre 23:38:34.010 et .198. Notre correction de faisceau intervient donc bien pendant World Ender ; ce n’est pas seulement une hypothèse fondée sur le nom des classes.
- Deux épées confirmées sur le joueur : `+1010 Dagger_R`, `+1018 Dagger_L`, MayhemWeaponComponent dérivé d'un composant de mesh, vtable `2724250`, parent mesh et sockets propres.
- Montures : classes et CDO lus ; mouvement `MayhemHorseCharacterMovementComponent`, vtable `270ABF0`, DoJump `667510`. Lecture de routage `681B40` / `669D20` : aucune action JUMP 30 reçue pendant les appuis à cheval. Source retenue : InputSettings CDO (`3A5E1C0`, ActionMappings `+80`, entrées 40 octets), action `Jump` ; PlayerInput du contrôleur `+448`, KeyStateMap `+260`, lookup natif en lecture `1C75E60` utilisé par IsPressed. Le FKey emprunté ne passe jamais dans une ABI par valeur qui libérerait sa référence partagée. Bytecode complet des sauts/atterrissages Strife et War étudié depuis huit assets SHA1 vérifiés ; aucun PAK modifié.
- Tests : branches de rendu, absence de modification des sockets/acteurs, visibilité et restauration des épées, ancien INI, nouvelles valeurs et raccourcis ; World Ender vs faisceaux ordinaires ; garde-fous du saut monté. Résultats de CI et empreintes dans le reçu après construction.

## Essais du lot

1. À pied, ranger les armes et attendre une seconde. Tester seulement le pistolet droit visible, puis seulement le gauche, puis les deux OFF : viser/tirer, corps à corps, dash, saut et retour aux tirs. Vérifier particulièrement le bras masqué après chaque action.
2. Vérifier le bon côté de chaque holster. Ouvrir la roue des munitions, passer à cheval, activer l'ultime puis revenir : les modèles OFF doivent rester OFF ; Native doit restaurer le modèle natif.
3. Tester les quatre choix d'épées : gauche, droite, les deux, Native ; attaquer dans chaque cas. Les dégâts et animations doivent rester présents.
4. World Ender avec 3D aiming ON puis OFF, en TPS à distance habituelle : caméra devant/derrière/sur le côté ; le rayon doit rester dans l'axe des canons et de l'animation. Comparer avec la vue native. Signaler séparément direction, visibilité et dégâts. Vérifier aussi les faisceaux électriques/magiques ordinaires.
5. Cocher Jump on horseback : sauter à l'arrêt, en avançant, au sprint ; relâcher entre les appuis. Tester Despair puis Ruin, saut/atterrissage, tirs et descente du cheval ; aucun second saut en l'air. Décocher pour retrouver le comportement natif. Une commande réaffectée doit continuer à fonctionner.
6. Vérifier sauvegarde des deux nouveaux choix, raccourcis personnels et fermeture initiale des groupes. La coop reste un essai ultérieur, aucune validation supposée.

## Limites et file conservée

Les sept points du retour sont suivis, mais les points déjà acceptés ne sont pas réécrits et la coop différée n'est pas annoncée corrigée. Bras montés OFF et autres effets P2 restent natifs/reportés. Le recovery de réception lourde garde son statut incertain. Les six capacités entre personnages restent à adapter : aucun faux choix de pouvoir étranger présenté comme fonctionnel. Suivi vertical pendant les exécutions reste ouvert. HUD d'arène non reproduit, surveillance seulement.

Installation **manuelle**, ZIP cumulatif à quatre fichiers racine. Conserver l'INI personnel ; nouvelles clés ajoutées automatiquement lors d'une sauvegarde des réglages. Aucun fichier du jeu, PAK ou sauvegarde remplacé, aucune relance par l'agent. Rollback : remettre l'ASI V1.78 avec le même INI ; ses deux nouvelles clés sont ignorées par V1.78. Toutes les anciennes archives, main/archive et v1.43 sont protégées.
