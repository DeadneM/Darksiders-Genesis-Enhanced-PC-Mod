# V1.163 — réponse au sol de la marche aérienne

Base : V1.162 debug `8c2330877edc86a81b3685c29b62a228717efa6c`. Retour utilisateur : marche aérienne appréciée, mais cheval qui flotte mollement. Conserver le maintien, les sauts, le relâchement et le délai déjà acceptés ; changer uniquement sa réponse au déplacement pendant la marche aérienne.

## Objectif et critère de réussite

En marche aérienne acquise : vitesse de marche, accélération, arrêts et virages répondent comme au sol ; sprint natif conservé. À vérifier en jeu sur les quatre apparences. Aucun réglage ni raccourci supplémentaire, option toujours OFF par défaut. **EXPÉRIMENTAL pour cette réponse**, aucune égalité exhaustive de physique/animations affirmée avant essai.

## Cause et mécanisme

Le relevé V1.162 sur Despair montrait MaxFlySpeed=600 et BrakingDecelerationFlying=0. L'EXE confirme que le getter de vitesse du cheval utilise SprintingMaxSpeed si son sprint est actif, sinon le getter moteur dépendant du mode ; Flying sélectionne donc la vitesse de vol. Le calcul de vitesse de vol reçoit aussi une friction et un freinage distincts du sol. Ces différences expliquent une réponse plus lente/inertielle ; l'observation utilisateur reste la preuve du ressenti.

Deux adaptations natives limitées au cheval sous notre lease Flying :

- HorseCharacterMovementComponent::CalcVelocity, vtable +0x670, entrée 0x6625E0 : transmettre GroundFriction et BrakingDecelerationWalking courants, bFluid=false. DeltaTime, accélération et calcul natif conservés, fonction originale appelée une fois.
- GetMaxSpeed, vtable +0x3D0, entrée 0x670210 : résultat MaxWalkSpeed courant pour marche aérienne sans sprint. Si sprint actif, résultat original exact conservé ; ce getter fournit déjà SprintingMaxSpeed.

ABI CalcVelocity vérifiée dans le thunk natif 0x1CF06D0 : RCX composant, XMM1 DeltaTime, XMM2 friction, R9 booléen, cinquième paramètre float sur pile. Entrées, slots et prologues exacts vérifiés avant installation ; si l'un des deux hooks manque, marche aérienne indisponible et saut au sol conservé. Le contrôle du code est limité à l'EXE supporté ; aucun hook global du moteur, aucun accès à un autre jeu.

Les valeurs de sol sont lues par réflexion nommée et mises à jour à chaque utilisation, donc les réglages Horse Speed existants restent pris en compte. Cache uniquement des offsets par classe/identité, pas de scan global ni chargement d'assets. Option ON, thread du jeu, lease récente, identités faibles, possession réciproque, authority, absence de despawn et Flying/custom0 requis ; sinon arguments et résultat originaux transmis. Monde/forme/focus/menus et sorties gardent la gestion V1.162. Aucun champ de mouvement, gravité, vitesse, sol, volume physique ou animation modifié par ces adaptations.

Le jeu est fermé lors de la tentative de lecture ponctuelle : aucun nouveau relevé en jeu obtenu, aucun appel externe ni lancement. Les contrats sont étudiés sur l'EXE local déjà présent ; les offsets de friction/freinage/vitesse sont résolus dans la réflexion du jeu et refusés si absents/non finis. [Rôle des paramètres, documentation Epic](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/Engine/GameFramework/UCharacterMovementComponent/CalcVelocity?application_version=4.27) : principes uniquement, ABI exacte tirée de l'EXE 4.22.2 du jeu.

## Contrôles et limites

Tests de la fixture étendus : adaptation des trois arguments, conservation du DeltaTime et appel original unique, vitesse de marche modifiée en direct, sprint inchangé, valeurs non finies, option OFF, sol natif/autre acteur et lease P2. Tests V1.162 du maintien/saut/restauration conservés. Vérifications Windows GitHub à venir ; pas de build local. Aucun nouveau paramètre INI, valeur par défaut ou action Hotkeys.

Test utilisateur : même trajet au sol et dans le vide, départ/arrêt, demi-tour/virage serré, marche puis sprint ; conserver deux sauts successifs et retour en chute après relâchement. Vérifier le saut normal avec la marche aérienne OFF. Si les animations restent différentes, ce sera un constat distinct : aucun faux état Walking ni sol synthétique ajouté ici. Rollback V1.162 ou option OFF. Écharpe et optimisation moteur restent séparées ; main V1.159 et anciennes archives intactes.
