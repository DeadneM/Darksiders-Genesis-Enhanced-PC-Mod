# V1.104 - Rampage Strife : livraison GitHub controlee

**CONSTRUITE / CONTROLEE : [59/59 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40203015804), zero avertissement/erreur MSVC. [ZIP cumulatif immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/acd4ba2cf79577a3c8aa9b8cf09e40f4eee2193b/downloads/DarksidersGenesis_V1.104_STRIFE_RAMPAGE_NATIVE_IDENTITY_TEST.zip). Rampage EXPERIMENTAL, NON VALIDE EN JEU.**

Base acceptee `07818091c719d4aeda2901ef08a0bbfb2282560a` : V1.103 reception lourde validee dans les essais rapportes par l'utilisateur ; Stoneskin/police V1.102, Geyser/Clone/Caltrops V1.98 et World Ender War V1.100 conserves. Source compilee `7f4c0d4830b8146668d076e99bc49347db4f77cd` ; archive GitHub `acd4ba2cf79577a3c8aa9b8cf09e40f4eee2193b`. Compilation exclusivement par GitHub Actions, aucune compilation locale ni installation ou relance du jeu par l'agent. L'utilisateur telecharge via l'updater du jeu, canal debug.

## Changement cible et preuve

L'ancien garde de slots reproduit un refus sur le jeu actuel : Upperbody_Flinches exige au lieu d'UpperBody_Flinches expose. Les 17 autres contrats lus sont vrais, dont proprietaires reciproques, assets Strife, classes de phases et RootMotionMode=3. Nouveau garde par identites natives FName index/numero, cinq slots uniques sans DefaultSlot rendu ; aucun changement global des comparaisons de noms. Le releve reste passif : pas d'appel Prepare ou de lancement natif observe. Il ne reconstitue pas tous les etats d'une ancienne session.

Le montage prive conserve les segments War de mouvement, boucle et fins ; squelette propre Strife et slot non rendu. Le chemin EXE exact confirme l'extraction non ponderee en mode 3, qui est exige et jamais modifie. Pas de nouveau hook, vitesse manuelle, rotation forcee, touche simulee ou fin par timer. Refus de preparation et abandons natifs bornes journalises. La pose reste la locomotion de Strife, sans animation de charge/frappe dediee ; effets, sons et points d'attache des clips sources peuvent demander un travail ulterieur.

## Deux revues et conservation

Premiere passe : refus de casse reproduit, identites/mode de mouvement releves en lecture seule et branche moteur examinee hors ligne. Deuxieme passe : fixtures Windows des cinq phases, proprietaires/serials, refus/abandons, graphes invalides, boucles/copies privees et relance, parmi les 59 suites reussies. Leurs resultats ne prouvent pas mouvement, degats, collisions ou fin effective dans le moteur.

- Prefixe de montages accepte et fonctions Stoneskin du wrapper compares a V1.103 ; 186 autres fichiers source/tests/config compares inchanges avant ajout des nouveaux fichiers Rampage.
- HeavyLandingFeature et tests, police, camera/visee, entrees souris/XInput, roues, horse, ressources et fonctions validees conserves. Aucun nouveau reglage INI : 169 cles, 55 ID Hotkeys et defauts conserves.
- 168 anciennes archives comparees par SHA256, 169 ZIP au total. main/archive/tag v1.43 verifies intacts.
- ZIP 992208 octets ; SHA256 `2516EE8D6EB220117BEC8A7DC126DF521DEE04A285DFB65983DFF7470A70B31A`. Quatre fichiers a la racine, CRC valide, ASI/dxgi AMD64 ; README/INI compares a la source compilee. HTTP 200 et octets du lien public verifies.
- API downloads/debug verifiee avec les regles de selection de l'updater actuel : V1.104 est la derniere archive admissible ; telechargement debug compare au meme SHA256. La fenetre du jeu n'a pas ete observee pour cette verification.

## Essai cible et suite

Depuis l'updater : canal debug, rechercher la mise a jour, conserver INI/raccourcis/PAK/sauvegardes et laisser l'installation intervenir jeu ferme. Aucun remplacement local par l'agent. Le README embarque est celui de la source compilee, anterieur a ce recu.

Strife a pied, Rampage sur un emplacement debloque : espace libre puis obstacle ; verifier deplacement et degats, relachement puis marche/attaque et second lancement. Comparer vue native/TPS puis les trois emplacements. Remettre Native si anomalie et garder le journal. Verifier ensuite le Rampage original de War. READY indique l'initialisation, CAST le passage au montage ; aucun ne prouve a lui seul les degats. P2 et simultaneite des deux Rampage restent differes.

Repli : V1.103 et emplacements Strife sur Native, en conservant l'INI. La reception lourde validee reste inchangee. Prochaine etape : retour de jeu cible sur Rampage, ou raison PREPARE/abandon du journal ; rester sur cette capacite jusqu'a resolution.
