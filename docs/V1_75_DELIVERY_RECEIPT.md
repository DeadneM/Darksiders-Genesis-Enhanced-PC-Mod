# V1.75 — reçu de livraison, 21 octobre 2026

**CONSTRUITE / CONTRÔLÉE / NON INSTALLÉE. Nouveautés EXPÉRIMENTALES, non validées en jeu.** Branche **debug** exclusivement, base V1.73 validée dans les essais rapportés. Installation manuelle par l'utilisateur à la fin de sa session ; aucun fichier du jeu remplacé pour cette livraison.

## Contenu et critère de réussite

- **Gameplay → Aim without firing** : maintien configurable d'un ou deux boutons de manette, OFF/OFF initialement. Appelle AIM/AIM_RELEASE natifs, sans simuler une touche ni ajouter un tir. Chaque joueur utilise sa manette et sa caméra. Réussite à confirmer : maintien/relâchement propre en vue native et TPS, seul et en coop, sans modifier les commandes déjà configurées.
- **Gameplay → Player movement → Heavy landing movement recovery (experimental)** : nouveau candidat, OFF par défaut, pour la réception lourde automatique après une chute assez haute. Après contact au sol, ouvre les deux permissions natives de déplacement de cette action seulement. Réussite à confirmer : se déplacer plus tôt après impact sans régression d'animation, de visée ou de contrôle. Trois réceptions Strife observées dans la capture ; aucune War_HeavyLand, donc chemin War particulièrement à vérifier. Les autres blocages natifs peuvent encore empêcher le mouvement.

L'ancien Recovery du dash reste supprimé ; ses ID Hotkeys restent réservés et inertes. Aucun nouveau hook pour HeavyLand, aucune fin d'animation forcée. Fonctions validées de caméra, visée/tir/dash, roues, ressources, apparences, saut et grappin conservées. Les validations historiques restent limitées aux essais utilisateur rapportés.

## Preuves de construction et paquet

[CI Windows x64 réussie](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39537334409) : **45/45 suites**, zéro avertissement MSVC. Avis de dépréciation CMake/MinHook et Node des outils distincts des diagnostics du code ASI/DXGI. Source `36308182d26d85290da15c9a05571a6fbe25ce3b`, archive `a816cc3db3c446fac407b93dc881058c19e7e43d`.

[Télécharger le ZIP cumulatif immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/a816cc3db3c446fac407b93dc881058c19e7e43d/downloads/DarksidersGenesis_V1.75_PAD_AIM_HEAVY_LANDING_TEST.zip) : **836203 octets**, SHA256 `B6D1F0CE0D3ACE5B5CF27A3C75373778707CC84EA9FA41764B4036B5ABB6EE08`. Téléchargement public HTTP 200, taille et hash vérifiés. CRC, architecture AMD64 des deux binaires et quatre fichiers racine contrôlés :

| Fichier | Octets | SHA256 |
| --- | ---: | --- |
| DarksidersGenesisMod.asi | 1455104 | `0F41EA56889E553494DFAE77F392A62CD4C8B933F4C3293DE08E9CE5D6AECD56` |
| dxgi.dll | 155136 | `1EA87EFC5972BC8BE62DC0C9EE1A94E27880CCAA9C51123F37827C85EE19D005` |
| DarksidersGenesisMod.ini | 4303 | `B6608512171FC90632A408A89AA1EB2FF264ECDC027DA8DBD665F8713965B16E` |
| README.md | 14708 | `9AA02AE6EAAD21A723FA38485FFF0A9729CC70D38947E0CD180ADB7E1D674DDB` |

157 clés dans l'INI fourni ; les valeurs des 156 clés précédentes et les 55 ID Hotkeys sont conservés. Les 139 anciennes archives sont inchangées (140 ZIP au total). main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, tag v1.43 `902861eb1ff34c14dc5ded092581b360767da817` inchangés. Fichier de reprise préexistant conservé hors commit.

Le README contenu dans le ZIP décrit la préparation V1.75 avant CI ; le présent reçu complète son statut sans modifier l'archive immuable. Le binaire installé a été relu : toujours V1.73, SHA256 `FC08C15C7321505D68AC8C23FC383F740A11A9E8632E24219FA190BD2316474C`. Aucun lancement du jeu, aucune installation V1.75, aucun PAK, fichier de sauvegarde ou INI du jeu écrit par l'agent.

## Essais groupés pour l'utilisateur

1. Conserver l'INI personnel et ses raccourcis à l'installation. Configurer la visée seule avec un bouton puis deux, tenir/relâcher en vue native et TPS, Strife/War ; P1/P2 si disponible. Vérifier absence de tir ajouté et retour après menu, roue, Alt-Tab ou changement de personnage. Choisir une combinaison qui ne gêne pas les commandes natives.
2. Comparer la réception lourde **OFF puis ON** depuis la même hauteur, en maintenant une direction après l'impact : Strife puis War, vue native puis TPS. Vérifier le délai avant déplacement, la pose, les sons/effets et le contrôle suivant. Une petite chute ou une attaque plongeante ne teste pas cette option.
3. Contrôle court des acquis : déplacement + visée + tir spécial + dash ; roues et équipement maintenu, rangement, échange des sticks et P2. Le dash n'est pas accéléré par ce nouveau Recovery.

Rollback des nouveautés : Heavy landing movement recovery OFF et les deux entrées Aim without firing OFF. Rollback complet : restaurer les binaires V1.73 sauvegardés en conservant l'INI personnel. [Détails, limites et gardes](V1_75_PAD_AIM_HEAVY_LANDING_TEST.md).

## Suite encore ouverte

- **Six capacités au choix pour chaque emplacement des deux personnages : non implémenté.** Les lookups Blueprint acceptent chacun trois identifiants ; les montages et composants dépendent du personnage. Une adaptation est nécessaire avant de proposer des sélecteurs fonctionnels.
- **OFF complet par pistolet : non implémenté.** Les modèles gauche/droite restent acquis. Une frontière de rendu a été identifiée, mais ni bras ni effets ne sont encore isolés sans risque sur les tirs.
- HUD général après refus d'arène verrouillée : surveillance, non reproduit dans le dernier essai. Autres FX P2 reportés expressément. Notification Auto check update à confirmer visuellement.

[Recherche native et limites](V1_75_WRATH_HEAVY_LANDING_PISTOL_RESEARCH.md) · [File active](ACTIVE_QUEUE.md).
