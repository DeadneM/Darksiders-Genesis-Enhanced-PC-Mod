# V1.160 — saut des quatre chevaux, visée et explosion Stoneskin

Base : V1.159 stable `d6414404ba268add00834323c6da064af5d5ce42`. Le 11 novembre 2026, l'utilisateur autorise la finalisation et la build bêta. Coop/P2 VALIDÉE par son retour précédent ; aucun nouvel essai coop exigé pour rouvrir les acquis. Main reste V1.159.

## Objectif et changements

- Saut : Mayhem et Rampage ajoutés au filtre de classes qui acceptait seulement Despair/Ruin. Même possession locale réciproque, même composant de mouvement et même appel DoJump natif. Aucun code de touche imposé : action Jump réaffectable conservée.
- Visée sans tir : défaut D-pad bas seul, seconde entrée OFF. Même constante pour nouveau profil, reset et clé absente ; un OFF ou raccourci existant reste conservé. Aucun INI personnel modifié.
- Explosion Stoneskin Strife : une création refusée n'est plus abandonnée immédiatement. Au maximum trois requêtes cosmétiques, espacées de 150 ms ; abandon après une seconde depuis la première tentative. Un émetteur accepté n'est jamais rejoué. Les pauses/roues/cinématiques natives suspendent les demandes ; expiration, perte de propriétaire, changement de monde/forme annulent le ticket. Un numéro par cast protège des callbacks réentrants. La découverte tardive des ressources avant la première tentative garde son chemin existant.

L'explosion utilise uniquement l'API visuelle déjà autorisée. Aucun nouveau hook, API, chargement d'effet supplémentaire ou activation de blast n'est ajouté. Geste, peau/écharpe, cendres, aura Molten et dégâts restent inchangés. La piste d'abandon sur création refusée est démontrée dans le code ; elle n'est pas annoncée comme la cause unique de toutes les absences visuelles. Un composant accepté mais invisible nécessiterait un autre diagnostic.

## Vérification et livraison

Tests adaptés : quatre classes de cheval et refus sans possession ; défaut D-pad et maintien d'un OFF personnel ; échec de création puis succès unique, intervalle, budget, pause/cinématique/roue, timeout, expiration, changement de monde/forme et recast réentrant. Relecture croisée des références et du lifecycle avant compilation GitHub Windows x64. Pas de build local.

Statut initial : **PRÉPARÉ / EXPÉRIMENTAL**, en attente de la CI et du test visuel utilisateur. ZIP cumulatif deux binaires à la racine, sans INI/README, disponible sur debug après contrôles. Anciennes archives conservées. Pas d'installation automatique ni de téléchargement de ressources déjà présentes.

Test ciblé : (1) saut avec les quatre apparences, dont Mayhem et Rampage ; (2) D-pad bas après nouveau profil/reset, ou réglage manuel dans un INI existant ; (3) plusieurs activations Stoneskin sur Strife, comparer explosion, peau/écharpe et cendres. Pas de nouvelle promesse de FPS ni de réparation de l'aura Molten. Retour arrière : ZIP V1.159 stable local déjà disponible.
