# V1.135 - acquisition native TPS

30 octobre 2026. CONSTRUITE / CONTROLEE, EXPERIMENTALE ; NON VALIDEE EN JEU. Base cumulative V1.133, dont le filtre Statique V1.130 et l'aide ajoutee sont maintenant rejetes ; autres acquis confirmes conserves. Compilation GitHub seulement, aucune installation ou relance par l'agent.

## Controle de livraison

Source `209a25e0ce3d592cf771d12035b9eef02b85d93e` ; archive `f4a32cf5a72015c99cecb146ff960526df2ea834`. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41117665955) :69/69suites, zero avertissement/erreur MSVC. V1.134 intermediaire avait69/69mais4avertissements d'analyse des bornes du nouveau selecteur ; gardes explicites ajoutees, avertissements non masques, ancienne archive conservee et non installee par l'agent.

[ZIP cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/f4a32cf5a72015c99cecb146ff960526df2ea834/downloads/DarksidersGenesis_V1.135_NATIVE_ACQUISITION_TEST.zip), 2425004 octets, SHA256 `3863D6F78C3AAA6991A6D983A00396E3645139705C1EA869BC6D8838CAB81578`. Quatre fichiers a la racine ASI/DXGI/INI/README, methode0 etCRC verifies, binairesAMD64, INI/README compares a la source compilee.199anciens ZIP Git identiques,200au total ; main/archive/v1.43 inchanges. Updater debug choisit et telecharge ces memes octets. Defender ZIP et payload sans menace, protections actives et non modifiees.

## Changement et preservation

Le setter de cible n'est plus intercepte : cycle et chaines Statique rendent la main au moteur. Ancien filtre de contact, recentrage ajoute et observateur de trace associe supprimes. Deux hooks retires, un hook natif garde pour son selecteur ; aucune nouvelle commande d'entree, rotation d'acteur ou recherche d'acteurs dans l'ASI. Les neuf consommateurs compares (dont solveur Lava) et les modules camera/controles/pose restent identiques hors nouveau choix de point commun. [Revue comparee](evidence/V1_135_STATIC_REVIEW.json).

Capture prealable en lecture seule1785echantillons sans erreur : Standard et Statique ont chacun un finder et une arme attribuables, mais utilisent le meme profil effectif7500/90/45. Le code conserve moteur/callback/cache/poids/rayon de recherche, ne module que les deux angles pendant l'appel natif et les restaure avant retour. A100/OFF, angles non ecrits. Le point socket/bornes calcule par le moteur sert aux consommateurs, avec controle d'identite/generation et point devant la bouche. Aucun essai d'impact ou de portee effectue sur cette version.

## Menu et limites

Camera -> TPS controls and aiming -> Native TPS aim assist. Case Adjust native acquisition OFF par defaut ; Global ou Per ammunition,0..200%, saisie/Default100.100=reference native dans l'orientation TPS,50plus restrictif,150plus permissif ;0limite l'acquisition a l'axe exact. Ce pourcentage regle les angles d'acquisition, pas une force d'interpolation, la portee ni les degats.

Ajustement pour Strife normal a pied, P1/P2 ; chevaux/ultime et contextes inconnus conservent le selecteur natif. En mode par munition, le finder commun reste a100 ; le finder attribue a l'action/arme est module. Attribution des autres munitions et coop restent a tester en jeu. Les profils/classes cibles natifs restent prioritaires : aucune promesse de preference pour un destructible devant un ennemi. Une acquisition plus restrictive peut naturellement revenir au tir libre et a sa longueur native.

228cles livrees,218autres valeurs et55ID Hotkeys conserves. Schema TPSNativeAimAssist* neuf : anciennes valeurs d'aide ajoutee ignorees puis neuf anciennes cles retirees lors d'une sauvegarde du menu. Les nouveaux curseurs partent a100, caseOFF. Garder l'INI personnel ; aucun INI du jeu n'a ete remplace par l'agent.

## Essai groupe

1. TPS a pied, Statique : comparer caseOFF etON100, verifier portee ressentie et chaines normales/Hotstreak.
2. Meme scene :50/100/150, puis mode par munition Standard50/Static150. Verifier choix de cible et rappel des valeurs.
3. Autres munitions, surtout Charge/Graviton/Lava ; main vide pause/roues et vue native. Coop en dernier.

Fermer normalement le jeu pour ecrire les bilans. Les compteurs ne prouvent pas les impacts. CaseOFF retrouve les angles natifs ; rollback complet V1.133 possible avec sauvegardeINI. [Perimetre et tests](V1_135_NATIVE_ACQUISITION_TEST.md), [CI](evidence/V1_135_CI_CHECK.json), [ZIP](evidence/V1_135_PACKAGE_CHECK.json), [updater](evidence/V1_135_UPDATER_CHECK.json), [scan](evidence/V1_135_DEFENDER_CHECK.json).
