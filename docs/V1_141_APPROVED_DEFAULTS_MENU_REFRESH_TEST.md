# V1.141 — valeurs par défaut approuvées et menu

**CONSTRUITE / CONTRÔLÉE : [70/70 suites Windows, zéro avertissement/erreur MSVC](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41332088332). Présentation EXPÉRIMENTALE, à vérifier en jeu.** Les valeurs de configuration sont approuvées par l'utilisateur ; cela ne valide pas les fonctions encore expérimentales. Base cumulative V1.140, debug uniquement.

## Objectif et réussite

Intégrer les 72 choix confirmés, sans écraser les préférences existantes, et rafraîchir la lisibilité du menu. Réussite : création et reset d'INI identiques au profil approuvé, valeurs personnelles explicites préservées, menu lisible aux tailles de police/fenêtre supportées, groupes et raccourcis fonctionnels.

## Configuration

48 différences avec V1.140 sur 228 clés ; les 180 autres valeurs restent identiques. Les 156 clés hors de la liste de choix ne changent pas. Insert et police 13 sont conservés, canal debug aussi. Les constantes partagées sont utilisées par initialisation, reset, repli et boutons Default. Les héritages de compatibilité des anciens INI pour le FOV coop et les délais de War sont conservés.

- ON : auto-check updates, saut/holsters/skins bonus du cheval, rangement automatique personnalisé, trois recoveries, deux flous personnalisés, ajustement d'acquisition native.
- OFF : skins de progression, override Léviathan, durée ultime personnalisée, Standard limité, portée du grappin, réduction baisse Hotstreak, recharge potions personnalisée, plané/sprint cheval illimités, double saut infini et visée sans tir par raccourci manette.
- Capacités et modèles natifs ; réticule souris masqué en TPS ; Léviathan natif par override OFF (le sélecteur mémorise 0/OFF).
- HUD interaction 0,50 ; délais pistolets Strife/épée War 10 s ; FOV coop 80 ; quantité de flou 0,5 dans chaque vue, distance TPS 250 m ; acquisition globale 50 %.
- Neuf gains/durées/portées/diviseurs/recharges choisis à 1. Leurs cases d'activation restent celles approuvées ; les réglages non concernés, dont dégâts/mouvement, ne changent pas.
- F1 : trois jauges ; F2 : munitions ; F3 : Hotstreak ; F4 : potions ; F5–F11 : aucun ; F12 : HUD. Huit raccourcis caméra native désassignés. Rangement L ou LB+L3. Aucun changement des identifiants d'action.

[Profil exhaustif des 228 valeurs](evidence/V1_141_APPROVED_PROFILE.json) ; [48 changements et deux revues](evidence/V1_141_DEFAULTS_REVIEW.json).

Un INI existant reste prioritaire, y compris OFF/None. L'updater le conserve : installer la version ne suffit donc pas à remplacer ses valeurs par les nouveaux défauts. Aucun reset du profil de jeu n'est exécuté par l'agent. La révision INI reste 2140 ; aucune migration destructive ajoutée.

## Présentation

Palette bleu sombre plus contrastée, descriptions secondaires plus lisibles, cases cochées/curseurs accentués, bordures et espacements homogènes, barre de défilement plus large mais bornée pour les fenêtres étroites. Groupes plus hauts et sous-titres colorés avec séparateur. États disponibles/indisponibles distingués par couleur et texte existant. Pas de fonction nouvelle, d'animation ni scan ajouté.

Identifiants des onglets/groupes/contrôles conservés ; groupes fermés par défaut, état ouvert conservé si l'utilisateur le mémorise. Aide courte et détails au survol maintenus. Toutes les couleurs/variables de style sont restaurées à la sortie de l'overlay.

## Validation et essai

Contrôle Windows `UserDefaultsConfig` : égalité entre INI généré et livré, assertions indépendantes des choix approuvés, reset complet, sauvegarde/rechargement, OFF personnels et raccourcis conservés. Fixtures existantes adaptées aux nouveaux défauts ; politique native des fonctions inchangée. Contrôle ImGui réel sur 12 combinaisons de police/largeur : limites, bouton Default, groupes fermés et restauration du style complet. Compilation et analyse MSVC exclusivement GitHub.

À vérifier en jeu : lisibilité des onglets, groupes, descriptions et valeurs à sa police habituelle ; fermeture/ouverture des groupes et saisie numérique ; raccourcis personnels inchangés après update. Pour vérifier les nouveaux défauts, créer un profil séparé ou utiliser volontairement le reset après sauvegarde de son INI. La remise à zéro est une action de l'utilisateur, pas une étape automatique.

Main/archive, anciennes archives, sauvegardes et PAK conservés. Aucun changement de code des hooks de tir, pouvoirs, caméra, contrôles ou performances. Rollback : fichiers V1.140, INI personnel conservé. Après validation du menu/defaults, préparer séparément la promotion stable demandée auparavant ; aucune promotion dans ce lot debug.
