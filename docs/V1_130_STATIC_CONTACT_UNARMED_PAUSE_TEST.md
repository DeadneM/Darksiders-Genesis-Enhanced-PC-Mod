# V1.130 - Static contact et pose de main vide pendant les interfaces

29 octobre 2026. CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU. Base V1.129 cumulative ; main reste V1.124 validee. Retour utilisateur : Statique toujours attire ; clarification explicite, le deuxieme pistolet NE revient PAS, seule la main vide se leve. Le masquage modele V1.129 est conserve ; la pose en interface et le filtre initial sont insuffisants.

## Cause prouvee et remplacement

Le journal V1.129 confirme READY et2 filtrages initiaux,159 corrections de vecteur libre. Cela prouve l'execution, pas l'efficacite. Le code natif montre que l'update de visee 5B9F80 retrouve le faisceau courant de l'arme, obtient l'ennemi assiste et termine par un saut direct 5B9FF5 vers SetTargetComponent617B90. Ce saut repete passe hors du filtre de provenance initial6981D0. L'ancien predicat de caller est retire, pas double.

Le meme setter est maintenant filtre sur tout le cycle du faisceau PRINCIPAL Static possede par Strife local a pied en TPS Camera Forward. Une cible n'est conservee que si son acteur est rencontre par le rayon camera courant ET par le dernier contact bloquant du volume du faisceau. Les identites faibles sont comparees aux index/series vivants. Le contact camera reutilise la requete native existante ; un observateur optionnel lit seulement l'acteur du FHitResult, sans modifier canal19, parametres, resultat ni retour. Sept arguments transmis une seule fois, appel571F6D/retour571F72 et ABI171E560 verifies sur l'EXE supporte. Pas de nouveau trace de type Damage, proxy d'acteur, rotation forcee ou valeur de portee/degats.

Ce double contact evite qu'une ancienne cible decalee conserve son verrou, et qu'un ennemi derriere le premier obstacle du faisceau soit accepte. La cible reelle reste utilisable par le mecanisme natif de rebonds : vider toutes les cibles aurait court-circuite61FC90. Les enfants ne sont pas des racines d'arme et restent hors du filtre. Si l'observation de collision est indisponible, assignation native conservee et compteur queryUnavailable ; l'ancienne visee reste installee. Limites a tester : geometries/overlaps, cadence des contacts, rebonds normaux et Hotstreak. Cheval/ultime non revendiques.

## Pose du bras sans modele

V1.129 conservait volontairement les gardes de poses de gameplay, qui abandonnaient le bras desarme en pause/roue et exposaient le bras arme natif. V1.130 conserve la derniere presentation desarmee valide pendant la suspension native UI/focus, sans nouvelle evaluation moteur pendant cette suspension. Meme acteur, corps, squelette, generation et identites vivantes ; seul le paquet de rendu du bras OFF est traite. Les matrices de socket/gameplay ne sont pas ecrites. Le cote equipe, les poses de melee/pouvoir deja natives et les cinematiques ne sont pas remplaces. A la fermeture UI, un echantillon gameplay frais redevient obligatoire ; changement de personnage/rig rejette la pose retenue. En absence d'un echantillon desarme valide avant suspension, aucun bras artificiel n'est invente.

## Verification et suite

Trois suites etendues : cible initiale/reassignee/reflechie, cible physique et identite recyclee ; capture de contact sans mutation, sept arguments, scopes imbriques ; vrai remplacement d'une matrice armee en pause au-dela des TTL, expiration au retour gameplay et aucune evaluation native ajoutee en UI. Deux passes confrontees et preuves natives jointes. Cela ne remplace pas l'essai utilisateur.

Config/UI,218 valeurs INI /55 ID Hotkeys, modeles/masquage V1.129 confirme, autres munitions, camera/blur, RawInput/XInput, capacites, Leviathan et Recovery preserves. Construction seulement GitHub, pas d'installation/lancement local. Anciennes archives et main/archive conserves.

Essai : Statique caisse/pot proche avec ennemi lateral/lointain, puis faire tourner la camera en maintenant le tir ;3D aiming ON/OFF, rebonds et Hotstreak. Comparer vue native. Avec un seul pistolet, ouvrir pause et roues apres avoir vise/tire ; la main vide doit garder la pose desarmee. Refaire cote inverse, perte de focus puis melee/pouvoirs et retour gameplay. Garder l'INI personnel ; rollback V1.129 possible pour conserver le masquage valide. Les compteurs STATIC_CONTACT et UI_POSE indiquent seulement le chemin execute, pas une validation en jeu.
