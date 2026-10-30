# V1.134 - acquisition native et suppression des corrections ajoutees

30 octobre 2026. PREPAREE / CONTROLES GITHUB EN ATTENTE ; NON VALIDEE EN JEU. Objectif : retrouver le cycle de Statique natif et proposer un ajustement de ses angles d'acquisition, avec100% comme reference. Reussite a confirmer : portee/chaines natives revenues, difference50/100/150sans fausser trajectoires ou controles. V1.133 reste installee ; aucun nouveau fichier de jeu remplace.

## Preuves avant changement

Le releve en lecture seule a enregistre1785echantillons sans erreur, dont Standard et Statique. Chaque action possede un MayhemTargetFinder distinct (propriete m_pTargetFinder a218), qui reprend ici le profil partage7500unites /90vertical /45horizontal /poids distance0,75 /poids cible actuelle1. L'arme est dans la capacite a1D8, le proprietaire du finder a58 et sa capacite Outer a20. Le getter natif5A1620 et les metadonnees MayhemTargetingProperties confirment ces liens. Les declarations d'assets Standard1200/30 et Statique16500/30 ne suffisaient donc pas a trouver les valeurs actives. [Contrat](evidence/NATIVE_AIM_PARAMETER_CONTRACT_2026_10_30.json), [audit compare](evidence/V1_134_STATIC_REVIEW.json).

## Remplacement et nettoyage

Suppression du filtre AssignTarget/PrimaryStatic/BeamContact de V1.130, qui annulait2637/2637cibles de la session rejetee et changeait le cycle des chaines. Le setter natif617B90 n'est plus intercepte. Suppression du recentrage ajoute V1.131..133 (cone6degres, Pull, nouvelle requete de visibilite et point interpole). Suppression de l'observateur de contact171E560 devenu sans consommateur. Deux hooks retires, un nouveau point d'entree natif6C3520, garde par signature ; aucun scan d'acteurs dans l'ASI.

Un seul appel au selecteur natif conserve owner, callback, filtres de classes, cache et choix du moteur. La direction vient du fournisseur Camera Forward TPS existant. Une transaction sur les DEUX angles du finder applique le pourcentage, puis restaure exactement les valeurs avant de sortir. A100% ou caseOFF, aucune ecriture de ces angles. SearchRadius, poids, longueur physique du faisceau, degats et donnees de chaine restent inchanges. La reference100% est le profil natif dans l'orientation TPS, pas le retour a la camera isometrique.

Les consommateurs de tir partagent le point choisi par le moteur : resolution native66A620 avec son TargetBoneName, qui utilise le socket ou les bornes du modele, au lieu de viser arbitrairement ses pieds. Cache bref lie au joueur, generation, arme et identite actuelle de la cible ; refus des identites expirees et des points derriere la bouche. Sans choix attribuable, le rayon TPS libre existant reste le repli. Solveur vitesse/pitch Lava, hauteur3D ON/OFF et exclusions WorldEnder inchanges. Aucune rotation d'acteur ou nouvelle liaison a une touche.

## Menu et compatibilite

Camera -> TPS controls and aiming -> Native TPS aim assist. Case Adjust native acquisition, OFF par defaut. Global ou Per ammunition, sept valeurs, curseur/saisie0..200%, Default100.0reduit les angles a l'axe exact ; il ne supprime pas une cible APRES le choix du moteur.100est la reference,50plus restrictif,150plus permissif, plafond angulaire180degres. Les valeurs sont memorisees entre les deux modes.

Nouvelles cles TPSNativeAimAssist* pour ne pas interpreter un ancien0d'aide AJOUTEE comme0d'acquisition native. Les neuf anciennes cles experimentales sont ignorees puis retirees uniquement lors d'une sauvegarde des reglages ; les nouveaux curseurs demarrent a100et la caseOFF.218autres valeurs et55Hotkeys conserves,228cles livrees. Aucun INI personnel modifie par l'agent.

## Perimetre et limites

Ajustement du selecteur pour Strife NORMAL A PIED, P1/P2. Monture/ultime, vue native, pause/roues/cinematiques et proprietaires non verifies gardent le selecteur natif. Les controles ne sont pas modifies. En mode par munition, le finder commun reste a100 ; seul celui de l'action/arme attribuee est module. Un helper inconnu ou une source ambigue ne recoit pas de parametre devine. L'attribution des autres munitions suit le meme ABI garde, mais n'a pas encore ete relevee en jeu ; coop a tester en dernier.

Le moteur garde ses classes cibles natives : ce changement ne promet pas de donner la priorite aux pots/caisses face a un ennemi admissible. Une acquisition plus faible peut naturellement revenir au tir libre natif et a sa longueur, ce qui n'est pas une reduction artificielle de BeamMaxLength. Le journal compte requetes, ajustements et points natifs, pas les degats ni impacts. La restitution lors d'un unwindC++ est testee en fixture ; cela n'est pas une preuve contre toute faute moteur.

## Controles et essai

Nouvelle suite NativeAimAssist : proprietes de vrais consommateurs sur graphe borne, original appele une fois, restauration, sept classes d'armes, generations/identites expirees, P2, contexte natif/pause/roue/acteur inconnu. Suite config revue sur schema neuf, migration experimentale et raccourcis ; suite ProjectileSpawn conserve les consommateurs valides et couvre le point natif vers tir/release/Lava. Fixtures de l'ancien filtre supprimees avec le code. Deux passes : preuve native/proprietaires, puis suppression des autorites superposees et comparaison des acquis. Cela ne remplace pas le jeu.

Essai groupe : garder son INI. TPS, caseOFF puisON100 : Statique doit retrouver portee ressentie et chaines natives. Comparer ensuite50/100/150sur le meme ennemi, puis mode par munition Standard50/Static150. Verifier libre/cible proche/lointaine, Hotstreak et munitions chargees/Lava, vue native, main vide pause/roues ; coop ensuite. Rapporter les resultats et fermer normalement pour ecrire les bilans. En cas de defaut, caseOFF remet les angles natifs ; rollback complet V1.133 avec sa sauvegardeINI disponible. Pas de nouvelle installation ni lancement sans demande.
