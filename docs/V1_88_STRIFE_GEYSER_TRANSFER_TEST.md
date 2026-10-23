# V1.88 — transfert expérimental de Blade Geyser vers Strife

**V1.88 intermédiaire CONSTRUITE / CONTRÔLÉE : [53/53 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40008159794), zéro erreur, un avertissement d’analyse MSVC C6386 restant sur le nettoyage du marqueur compagnon. Archive conservée, remplacée à la livraison par V1.89 après correction de cet accès. Geyser sur Strife EXPÉRIMENTAL, non validé en jeu.** [Archive immuable vérifiée](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/4309438f8a87d95ef33ab14fe0a4bdc0f4dda245/downloads/DarksidersGenesis_V1.88_STRIFE_GEYSER_TRANSFER_TEST.zip).

## Base, objectif et réussite

Base cumulative V1.87 `c20319f93413f849a41b10fdfd96f7842a172ac6`. Correctif réticule V1.87 validé par le retour utilisateur « c’est bon c’est validé continue », sans prétendre valider toute la coop. Bras War, Léviathan, bras OFF, effets des lames et suivi d’exécutions déjà validés dans les cas rapportés, inchangés.

Objectif de ce build : premier pouvoir offensif de War utilisable sur Strife normal à pied. Réussite en jeu : emplacement débloqué réglé sur Blade Geyser, consommation de Courroux attendue, apparition de l’attaque et dégâts, fin normale et retour aux commandes, sans régression du Geyser propre de War. Vérifier vue native/TPS, puis P2 séparément.

## Contrat retenu

Lecture seule du jeu exact SHA256 `9F4702024DF5EEA1D51DF7745B0AD1EA95B97009982F73DDC1218C53DFF33D54`, classes/packages, graphes Blueprint et quatre montages chargés (base et niveaux 1/2/3). Séquence War : durée 2,3666666 s, notification unique CallAbilityFunction → SpawnBlast à 0,4880667 s, fonction native `5A7C60`. Les tableaux de branchement/états/marqueurs sont vides. Strife_Idle : 2,6666667 s, aucune notification, squelette et slot TPS vérifiés. [Contrat public résumé](evidence/V1_88_GEYSER_CONTRACT.json). Aucun dump, sauvegarde ou pointeur de session publié.

L’adaptateur existant de classe/montage est étendu, sans hook supplémentaire. Le moteur construit un UAnimMontage transitoire depuis le montage Geyser d’origine (StaticConstructObject_Internal `AC57A0`, neuf arguments contrôlés ; package transitoire `A7B1B0`, SetSkeleton `16C7130`). **Avant toute modification : copie distincte, classe/outer/flags et tableaux événements/tracks/segments/section distincts du modèle.** Seules ces données privées sont éditées. Segment compatible Strife_Idle ajusté à la durée d’origine ; une notification SpawnBlast exacte est conservée à son temps natif, avec liens du montage privé. Aucune édition des assets/CDO/squelettes globaux, aucun allocateur artisanal, aucune minuterie ni surveillance du tick. Le moteur gère pause, interruption, exécution de la notification et fin.

Sélection/contrôle d’emplacement, coût natif, variantes de Geyser et nettoyage de son Blueprint sont conservés. Uniquement acteur Strife normal possédé par un joueur local réciproquement vérifié, rôle autorité et thread jeu ; pas de prêt de composant entre joueurs. War, acteurs inconnus, monture/ultime et autres requêtes de montage gardent leur chemin natif.

Le Geyser natif peut demander le compagnon War rising slash à sa fin/interruption. Son Blueprint inspecté n’applique pas de mouvement avant PlayMontage ; ses branches d’enchaînement ne concernent pas la cause 0. Sur le Strife exact ayant joué ce Geyser seulement, ce cast compagnon non joué est terminé par Finish(0), différé nativement pendant OnStart. Marqueur consommé une fois, identité/serial contrôlés ; War et appels sans marqueur sont transmis. **Ce compagnon de mêlée ne fait pas partie du transfert.**

## Limites et arrêt

Animation de repos compatible, pas de retarget complet du geste de War. Sons/particules propres à son montage et fenêtre de cancel notifiée non transférés ; l’acteur d’attaque natif garde son propre comportement. Le résultat visuel, les dégâts, les améliorations et P2 restent EXPÉRIMENTAUX jusqu’au retour joueur. Les assets doivent déjà être chargés ; aucun chargement forcé de package. Si le contrat échoue pendant le cast, celui-ci est terminé normalement et les demandes Geyser suivantes sont bloquées pour la session. Consulter le journal, revenir à Native et ne pas répéter les casts en échec. Le premier cast peut déjà avoir consommé son coût. Le blocage Geyser n’affecte pas Stoneskin ni les pouvoirs propres.

Stoneskin : transfert partiel conservé, protection et coût natifs ; apparence du statut à comparer avec le jeu d’origine. L’utilisateur ne confirme plus l’existence antérieure du cristal. Le Blueprint natif contient déjà des changements de matériaux de War et un effet de crumble à l’expiration ; cela ne prouve ni que tout aspect observé est normal, ni un correctif du mod. Aucun changement de matériaux Stoneskin dans ce build. Rampage → Strife et les trois pouvoirs Strife → War ne sont pas livrés.

## Essai groupé et rollback

1. Strife à pied, un emplacement débloqué → Blade Geyser. Essayer en vue native puis TPS près d’ennemis : apparition de l’attaque, dégâts, coût et fin du cast. Essayer chaque emplacement séparément ; les icônes de roue peuvent encore décrire le pouvoir remplacé.
2. Revenir à Native, vérifier les trois pouvoirs Strife ; passer à War, vérifier son propre Geyser et Stoneskin. Vérifier qu’aucune commande ne reste bloquée après interruption/pause. Si échec ou crash, arrêter ce transfert et conserver le journal.
3. Si disponible, même test sur Strife P2 avec War P1 : attaque et coût du bon joueur. Aucun résultat coop déjà supposé.
4. Courte régression des acquis : réticule/menus, bras War, Léviathan, lames/bras OFF et exécutions TPS.

Installer manuellement après fermeture du jeu, **conserver son INI**. Six réglages d’emplacement inchangés et Native par défaut ; le choix 4 existant devient accepté pour Strife. 168 clés et leurs valeurs par défaut, 55 ID Hotkeys, souris/manette, caméras et ressources validées conservés. Aucune installation, relance, écriture de sauvegarde ou modification du PAK automatique.

Rollback du transfert : choisir Native dans l’emplacement. Rollback binaire : ASI V1.87 validé conservé dans son archive, même INI personnel (le choix Geyser non supporté y revient à Native). V1.86 avec son défaut ABI de curseur n’est pas un repli recommandé. 152 archives antérieures et main/archive/v1.43 gardées intactes.


Premier contrôle CI du candidat `f1994aa` (run 40007454113) : compilation réussie, 52/53 suites passées, archive non publiée. Le nouveau test Geyser utilisait `Name{}` (index -1) au lieu de la section native None `{0,0}` ; le garde rejetait correctement cet appel. Test corrigé sans assouplir le garde. Avertissement d’analyse C6386 sur l’index joueur : borne unsigned explicite avant l’écriture du marqueur, sans désactiver l’analyseur. Nouvelle compilation complète requise avant livraison.
