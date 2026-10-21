# V1.77 — réaffectation native des capacités de Courroux

## Base, objectif et statut

Base cumulative V1.76, construite/contrôlée ; base explicitement validée V1.73 pour les essais rapportés. L'utilisateur autorise de poursuivre « comme si » le lot précédent était validé, sans attendre ses essais groupés : cette instruction autorise la suite mais ne fournit pas de nouvelle observation en jeu.

**PRÉPARÉ / EXPÉRIMENTAL : construction et contrôles à venir, aucune validation en jeu.** Objectif de ce build : trois choix indépendants par personnage parmi **ses trois pouvoirs natifs**, via une seule frontière fonctionnelle, sans remplacement d'entrée physique ni d'animation. Critère : le jeu reçoit le bon ID pour la classe, son déverrouillage et son coût ; désactivation/Native rend le comportement initial, et les autres fonctions restent inchangées.

**Le choix libre parmi les six capacités demandé par l'utilisateur n'est pas terminé.** Cette étape rend testable la réaffectation interne. Aucun choix de pouvoir étranger inopérant n'est affiché. L'adaptation Strife ↔ War reste ouverte.

## Menu et configuration

Gameplay → **Wrath ability slots (experimental)**. Chaque emplacement choisit Native ou :

| Personnage | Choix disponibles |
| --- | --- |
| Strife | Shadow Clone, World Ender, Caltrops |
| War | Blade Geyser, Rampage, Stoneskin |

Les six valeurs `[Values]` `StrifeWrathSlot1/2/3` et `WarWrathSlot1/2/3` valent **0 = Native** par défaut. ID stables : 1 Clone, 2 World Ender, 3 Caltrops, 4 Blade Geyser, 5 Rampage, 6 Stoneskin. Dans cette étape, un ID étranger au personnage ou invalide revient à Native. Aucun nouveau raccourci, ancienne valeur INI changée ou touche imposée. Les choix suivent le personnage, y compris lorsque les joueurs coop inversent Strife et War.

Un pouvoir verrouillé conserve son verrouillage natif. Dupliquer un pouvoir peut partager ses limitations natives ; aucun nouveau système de cooldown n'est ajouté. Les icônes/indications natives de roue peuvent encore décrire l'emplacement initial : leur mise à jour complète reste à examiner. Fermer/réouvrir la roue après un changement. Les capacités cheval/ultime restent natives.

## Frontière native contrôlée

Sur l'exécutable supporté SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54` :

- `663220(actor, destinationClasse, FName64, disponible*, coûtAccepté*)` appelle le lookup Blueprint `7AAC30`, vérifie le déverrouillage du même ID via `6AF360`, puis le coût natif du CDO via `7DD610`. La destination TSubclassOf est aussi le retour ; le cinquième argument passe sur la pile x64.
- L'action native à `664D31…664E0B` lit les trois FName dans ressources+3E4/+3EC/+3F4, passe par `663220`, vérifie les deux booléens, puis lance **la classe retournée** via `67C7B0`. Cette fonction conserve le lancement natif et la déduction du coût du CDO. Aucun hook ajouté à ces étapes.
- Intercepter seulement le getter `7AAC30` aurait gardé le contrôle de déverrouillage de l'ancien ID dans son appelant. V1.77 intercepte **663220 seulement**, et transmet le nouvel ID à la fonction native complète, une fois. Pas de modification des FName de ressources, du CDO, de la pose, des composants, de l'input, du montage, des sauvegardes ou du débit de Courroux par le mod.
- L'emplacement vient de la correspondance unique avec les trois ID vivants ; aucune supposition de touche ou de correspondance numéro FName/bouton. Familles et classes Blueprint exactes, propriétaire local réciproque, pied normal, type natif de ressources et thread du jeu contrôlés. Réentrance, disposition ambiguë, contexte étranger et signature changée restent natifs. Aucun objet conservé entre deux appels.

## Adaptation étrangère : nouvelle preuve, approche séparée

Inventaire privé SHA vérifié de 73 assets de référence, incluant variantes/chains et animations : les données de référence de Strife et War comportent **87 et 135 os sérialisés**, avec racines `Bone_Strife_Root` et `Bone_War_Root` et noms distincts. Cela dépasse la seule différence de nom d'asset de squelette. Aucun contournement du contrôle de squelette ajouté.

Les effets de gameplay sont aussi déclenchés depuis les montages **et** séquences : `SpawnShadowClone`, `DoSpawnCaltropsSpawner`, `StartBeam/DestroyBeam`, `SpawnBlast`, `SetFalling`, `EnableStopOnCollision`. Transférer seulement les notifications d'un montage ignorerait celles des séquences et les transitions. World Ender exige toujours son composant propre, comme établi en V1.76. Prochaine étape : adapter l'animation et le lifecycle par pouvoir, en gardant ces notifications et les origines d'effet cohérentes. Les exports privés et binaires du jeu ne sont pas publiés. 63 exports auxiliaires n'ont pas de propriétés décodées complètement dans cet inventaire ; aucune conclusion de compatibilité n'en est tirée.

## Essais groupés ultérieurs

1. Avec les pouvoirs déverrouillés, permuter les trois choix de Strife entre les trois emplacements ; répéter avec War. Vérifier pouvoir réel, animation, son, effets, fin d'action et coût. Tester à zéro Courroux **en mission**, car le Néant peut rendre son utilisation gratuite.
2. Tester une capacité maintenue/relâchée, Rampage en mouvement et Caltrops en l'air ; revenir aux six Native. Vérifier les roues et leurs indications séparément.
3. Répéter en vue native et TPS, souris/manette puis coop ; inverser les personnages entre P1/P2 ; vérifier cheval/ultime et chargement.
4. Dans le même lot : visée sans tir V1.75, réception lourde après chute avec Strife **et** War, OFF droit/gauche/deux V1.76 puis retour Native ; contrôle tirs spéciaux/Hotstreak/dash, roues et rangement. Ces candidats restent non validés en jeu.

Installation **manuelle ultérieure**, conserver l'INI personnel et ses raccourcis. Le mod lit les six nouvelles clés absentes comme Native. Aucun lancement ni fichier du jeu modifié par l'agent. Rollback : six choix Native, ou reprendre le ZIP V1.76/V1.73 en conservant son INI personnel. Anciennes archives immuables, branche debug seulement ; HUD général en surveillance, autres effets P2 reportés comme demandé.
