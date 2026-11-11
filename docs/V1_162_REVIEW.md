# V1.162 — marche aérienne des quatre chevaux

Base : V1.161 debug `96388c7f9265fb13d4c16cdf513d5fa7cfb4287e`. Retour favorable avec réserve sur la première explosion ; V1.160 saut/raccourci et coop acceptés conservés. Main V1.159 et archives préservées.

## Objectif et critère de réussite

Une option indépendante OFF par défaut dans Gameplay → Horse. Action Jump réaffectable, P1/P2 indépendants : saut normal ; maintien jusqu'au sommet ou nouvel appui en chute pour marcher dans le vide ; relâchement puis chute après 0,2 seconde (réglable 0–1 seconde) ; nouvel appui pendant le délai pour un saut natif supplémentaire. Relâcher avant l'acquisition garde la chute normale. Aucun appui virtuel ni bouton imposé.

Réussite EN JEU : quatre apparences testées, déplacement horizontal avec collisions, alternance saut/marche/chute, retour normal après OFF/pause/Alt-Tab/changement de propriétaire. Vitesses et poses du mode aérien natif à vérifier : la preuve de présence des fonctions ne démontre pas leur rendu en jeu. **EXPÉRIMENTAL, pas VALIDÉ utilisateur.**

## Mécanisme et limites

Le lecteur externe a uniquement lu propriétés, héritage et paramètres ; aucun appel moteur ni écriture. [Contrat sans adresses de processus](V1_162_NATIVE_MOUNT_CONTRACT.json). Mode natif Flying acquis uniquement sur cheval local réciproquement possédé et authority, depuis Falling. StopMovementImmediately une fois à l'entrée arrête la vitesse verticale ; aucun changement brut de vitesse/gravité, pas de noclip. DoJump existant conservé pour les sauts. Aucun hook ajouté, aucune recherche globale d'objets/chargement d'assets supplémentaire.

Les appels ASI utilisent la frontière ProcessEvent déjà en place sur le thread du jeu. Vérification du nom, package Engine, classe déclarante, drapeaux, entrée native, taille/offset des paramètres et Velocity ; identités faibles avec numéro de série. Cache des méthodes par classe/identité, abandon sur contrat absent. Le lecteur externe n'appelle jamais ces méthodes.

Lease de mode par joueur : sortie Falling seulement sur le même monde, mêmes objets vivants et cheval encore local/non transféré. Un propriétaire différent ou un objet recyclé interdit l'appel. Un remplacement natif de mode met fin à notre lease. Mod menu ouvert : maintien du mode acquis, commandes réservées à l'éditeur ; pas de nouvel appui synthétique à sa fermeture. Pause/roue/cinématique/focus annulent le maintien. Si l'objet/monde est déjà détruit, abandon de la référence sans appel tardif.

Deux nouvelles clés uniquement : `[Features] MountedAirWalk=0`, `[Values] MountedAirWalkReleaseDelay=0.200`. Anciennes valeurs et touches inchangées ; aucun INI personnel modifié. Le ZIP garde les deux binaires seuls à la racine. Retour arrière : décocher la nouvelle option ou réinstaller V1.161 en gardant l'INI.

Contrôles prévus : suites de saut existantes, adaptateurs natifs simulés (mauvaise entrée/paramètres, recyclage, possession modifiée pendant rappel), parcours réel du service en fixture (Jump → apex → Flying → nouveau saut), relâchement/délai/OFF/focus/menu et configuration sauvegarde/chargement/non-finis. Ils ne constituent pas une validation du moteur en jeu.

## Écharpe et optimisation

Envers d'écharpe toujours ouvert : le matériau War utilisé ne fournit pas le double-face natif du tissu Strife. Il faut une permutation cuite compatible avec le feu ; pas de faux paramètre scalaire TwoSided ni de désactivation globale du culling. Aucun changement de Stoneskin dans cette build. Les mesures existantes du service ASI n'établissent pas une limite GPU/FPS ; aucune promesse de gain moteur ni réglage graphique aveugle.
