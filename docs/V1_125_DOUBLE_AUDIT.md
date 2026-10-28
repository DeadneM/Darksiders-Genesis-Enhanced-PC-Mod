# Deux passes de nettoyage apres V1.78 - V1.125

Base examinee : V1.124 validee, commit `e2e519f7ea84d4b773fd7a31611779ae68308696`. Dernier nettoyage de167lignes integre en V1.78 ; il n'est pas reapplique/recompte.145fichiers de production,52cpp,24020lignes physiques. [Resultats et confrontation](evidence/V1_125_CLEANUP_COMPARISON.json).

Deux passes par le meme agent, avec deux methodes ; aucun second auditeur humain/agent independant pretendu. A examine graphe CMake/inclusions, declarations et references ; B examine producteurs/consommateurs, copies de telemetrie et etats. B ne lit pas la liste de candidats A. Puis confrontation et revue des appels/constantes des nouvelles capacites.

| Passe | Resultat | Decision apres verification |
| --- | --- | --- |
| A : graphe et references | Aucun fichier hors graphe,0global a usage unique/stockage seul ;224noms de fonctions uniques dans leur TU,32declarations sans appel qualifie detecte. | Comptages lexicaux seulement. Entrees de modules/callbacks/API locales conserves ; namespace forward-declare et methodes ConfigStore donnent des faux positifs. Aucun module entier inutile prouve. |
| B : donnees sans consommateur | Une copie `lastHorseNativeTick`, aucun nouvel etat runtime ecrit sans lecteur, aucune atomique de diagnostic constante trouvee. | Champ et copie retires. Horodatage producteur `g_lastLiveHorseCallbackTick` et consommateurs natifs cheval conserves : ce heartbeat n'est pas mort. |
| Confrontation manuelle post-capacites | Clone sur War conserve une structure de l'ancien adaptateur multi-pouvoirs : trois tableaux d'une entree, deux listes d'un nom, boucle d'un tour et branches Strife sans appel possible. | Identites/noms scalaires et branche War directe. Tous les appelants Owner/Source utilisaient deja War=true ; garde acteur/animation/os/type conserve. Les autres pouvoirs ont leurs propres adaptateurs, conserves. |

Le retrait certain ne represente que **deux lignes physiques nettes de production**, plus simplification des tableaux/boucles/branches sur des lignes conservees. Aucun gros gain de taille ou FPS promis. Les six appels natifs/lecture de montage/son de l'adaptateur Clone gardent leur ordre et arguments normalises ; checks d'identite et de proprietaire, P2, refus natif et nettoyage sont conserves. Tests existants retenus ; cas Strife natif et trois incoherences acteur/animation/os ajoutes au meme executable de test. Pas de nouvelle suite artificielle.

Les gardes Caltrops et les retours prives de montage ne sont pas des leftovers : appels verifies. ForeignMontage/Geyser, Stoneskin, Rampage, WorldEnder, input/camera, HeavyLanding, schemas INI/Hotkeys et workflow inchanges. Les ID Hotkeys retires restent reserves pour compatibilite. Les assets, boucles de son, fallbacks et restaurations ne sont pas supprimes pour leur seul age. Les candidats non prouvables sont conserves ; cet audit ne certifie pas l'absence de bugs ou la securite exhaustive de chaque offset.

## Apparences : retour utilisateur et limite actuelle

L'utilisateur rapporte des options sans effet ou sans choix interessant. Les43familles ne sont donc pas certifiees fonctionnelles. **11groupes n'ont que Native/Hidden** (effets au sol Caltrops, Ghost Hook, Void Bomb, autres Lava, Astarte, Scarab, Shield Demon, Legion et trois groupes Other). Leur manque de skin alternatif est reel, pas un bug de menu a compenser par des noms inventes.

Le remplacement actuel ne voit que les nouvelles particules passant par la fabrique187E600, avec asset cible deja charge et monde local reconnu. Les composants persistants/precrees et les modeles/materiaux peuvent contourner ce chemin ; un asset absent garde le rendu natif. Cela explique des limites possibles, **pas la cause prouvee de chaque essai utilisateur**. Options/valeurs sauvegardees conservees ; aucune suppression globale ou nouveau hook de rendu impose sans cas precis. Reprendre par les familles utiles et leur composant reel, pas ajouter des dizaines de choix en aveugle.

## Statut

V1.125 PREPAREE : nettoyage sans nouveau comportement intentionnel ; GitHub Windows x64/65suites/avertissements/ZIP/updater/Defender a controler avant livraison. Recovery est une carte documentee, pas un nouveau correctif en jeu. main reste exactement sur la V1.124 validee ; suite debug seulement. Pas de compilation, installation ou lancement local. INI personnel, raccourcis, PAK et sauvegardes inchanges.
