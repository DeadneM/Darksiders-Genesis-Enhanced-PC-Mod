# V1.145 - livraison contrôlée

**CONSTRUIT / CONTRÔLÉ, EXPÉRIMENTAL en jeu, non installé par l’agent.** Base V1.144 ; Stinger validé conservé. [ZIP direct immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/91c3ef4bf765d1bab7911b33095053376cb81f21/downloads/DarksidersGenesis_V1.145_OWNED_SHIELD_LIFECYCLE_TEST.zip). Source ca590ab9c1911b6fc6898ce23713f9ce420f8e2f, archive 91c3ef4bf765d1bab7911b33095053376cb81f21.

## Modifications et limites

Rampage/Chaos retirés des choix du bouclier : Belial=2, Dagon=3, Borrower=4 conservé, sphère d’eau=5. Native=0/Hidden=1 inchangés. Borrower réduit à 60 % de l’échelle d’attachement précédente ; Belial et eau 35 %, Dagon 25 %. Les apparitions natives de ces effets sur les ennemis restent exclues des modifications. Ressources chargées seulement, prochaines apparitions ; pas de chargement synchrone.

Nouveau suivi privé des composants substitués : liaison au DamageShield du joueur local, arrêt visuel lorsque le jeu retire sa référence de particules. Cas d’un coup unique avant la première observation pris en compte par l’attachement et le statut unique. Pas de calcul de protection ni d’écriture de HP/cooldown/statut. Table bornée, observation à 100 ms, garde de signatures et identités ; pool restauré avant le nouveau Hidden, notamment Stinger. Taille transmise par l’interception d’attachement déjà en place, sans nouveau hook/API moteur.

**À tester** : Borrower plus petit en deux vues ; épuisement en un coup et plusieurs coups, disparition puis recharge ; proportions/continuité des trois nouvelles variantes ; Native/Hidden et Stinger ; cheval/ultime et P2 restent non validés pour ce nouveau suivi. Les fixtures P1/P2 ne sont pas une validation coop. [Contrat et essais complets](V1_145_OWNED_SHIELD_TEST.md).

## Contrôles

- [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41496324365) : 71/71 suites réussies, aucun avertissement C/CS/LNK/D détecté. La première tentative ne liait pas le module dans le test LavaMuzzle ; corrigée avant livraison, aucun ZIP issu de cet essai.
- ZIP 2304238 octets, deux fichiers racine ASI et dxgi.dll, PE AMD64/CRC vérifiés, sans compression, sans INI/README. SHA256 829C2C46A6D495B8E26A81C7C4A863815EF207BFDE9F1D17B66F5281F8A7AA92.
- Catalogue updater debug : V1.145 sélectionnée et téléchargement HTTP 200 conforme. Interface du jeu et installation post-fermeture non observées pour ce lot.
- 229 clés/defaults de référence et 55 ID Hotkeys conservés ; 209 anciennes archives Git inchangées, 210 au total. Main/archive/tag v1.43 intacts. Pas de build local, lancement du jeu ou remplacement de fichiers de jeu/INI/PAK/sauvegardes par l’agent.
- Defender : ZIP et fichiers sans menace détectée ; protections actives et inchangées.

## Sujets ouverts

Stoneskin pierre/feu : défaut intermittent signalé puis lien avec le choix de bouclier retiré par l’utilisateur. Le journal montre des surcharges de matériaux refusées par le garde existant ; ce garde et la source StoneSkinVisual restent inchangés. Préflight lecture seule préparé mais non exécuté, conformément à la demande d’attendre. Aucune correction de ce défaut annoncée. Pose bloquée non reproduite au dernier essai, toujours ouverte ; dash aérien reporté.

Repli : V1.144 en conservant l’INI et en choisissant Native/Hidden ou Borrower, sans les variantes Rampage/Chaos rejetées.
