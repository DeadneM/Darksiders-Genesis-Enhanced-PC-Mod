# V1.144 - livraison contrôlée

**CONSTRUIT / CONTRÔLÉ, nouvelles variantes EXPÉRIMENTALES en jeu, non installées par l’agent.** Base V1.143 ; Stinger crystal VALIDÉ par l’utilisateur et conservé. Fire aura pour ShieldDemon REJETÉE et retirée.

[ZIP direct immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/f606c1da0552b1b2b0b8b6d59aec813f6f35b1c9/downloads/DarksidersGenesis_V1.144_SHIELD_BARRIER_VARIANTS_TEST.zip) ; source 3f18a8ea98e1e856e9e7fed0aa4a6325b2d034af, archive f606c1da0552b1b2b0b8b6d59aec813f6f35b1c9. [Contrôles GitHub](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41481881615) : 70/70 suites passent, aucun avertissement C/CS/LNK/D détecté dans le journal final. La première tentative e250dcb a échoué : copie d’un compteur atomique dans le test, et borne de sélection à expliciter. Corrigées en 3f18a8e avant publication ; aucun ZIP issu du premier essai.

## Contenu et essais

Même groupe en bas d’Appearance, après Leviathan : Legion shieldbearer propose Native=0, Hidden=1, Rampage shield=2, Chaos shield=3 et Sphere shield (Borrower)=4, en vues native et TPS. L’ancien choix Fire aura=2 devient intentionnellement Rampage shield. Stinger Native/Hidden inchangé. Seuls les templates visuels nouvellement créés sont remplacés, sans changement du statut/protection. La sphère Borrower est une cible seulement : son apparition sur l’ennemi reste native.

Tester les trois variantes disponibles à la prochaine apparition du bouclier : orientation, taille, centrage, continuité et lisibilité près de la caméra. Vérifier Native/Hidden et la disparition du visuel à la fin du statut, dans les deux vues. Une ressource non chargée laisse le visuel natif, avec indication dans le menu ; aucun chargement synchrone ajouté. P2 et qualité visuelle de ces variantes non validés. [Contrat et limites](V1_144_SHIELD_VARIANTS_TEST.md).

## Vérifications de livraison

- ZIP 2282222 octets, exactement ASI et dxgi.dll à la racine, PE AMD64, CRC valides, sans compression. Pas d’INI ou README inclus. SHA256 30C9E92086D16323F78AE4493056A78D3FAA40995F75D1509D73727D2574694D.
- Updater debug : V1.144 sélectionnée dans le catalogue ; téléchargement HTTP 200 et empreinte conforme. Interface du jeu et installation post-fermeture non observées sur ce lot.
- 229 valeurs du profil de référence et 55 ID Hotkeys conservés. Source des timings/pouvoirs, tirs, visée, caméra, masquage des bras, observateur Stoneskin et UI inchangée par rapport à V1.143 ; seuls catalogue/choix visuels, garde cible-seulement, version et fixtures changent.
- 208 anciennes archives Git inchangées, 209 au total ; main/archive/tag v1.43 intacts. Aucun remplacement de fichier de jeu/INI/raccourci/PAK/sauvegarde par l’agent. Les empreintes locales observées correspondent à V1.143 installée par l’utilisateur.
- Defender : aucune menace détectée sur ZIP et fichiers, protections actives et inchangées.

## Diagnostic poursuivi

L’utilisateur n’a pas reproduit la pose bloquée dans ce dernier essai. Le lecteur VM_READ a enregistré 1 371 échantillons pendant environ 139 s et les montages de tir/retour, puis s’est arrêté après dix lectures rejetées. Les deux bras peuvent être concernés, avec un ou deux pistolets visibles ; cela ne justifie pas un correctif spécifique au bras OFF. Aucun correctif de pose appliqué, bug toujours ouvert. Dash aérien reporté comme demandé.

Étude hors jeu poursuivie : le chemin natif GetCurrentStateName confirme le champ d’état +1C et la table d’états du graphe ; un lecteur plus précis est préparé pour une future reproduction, sans appel moteur ni écriture. Aucun appel à ce chemin n’est ajouté à l’ASI. La préparation du lecteur ne prouve pas la cause du défaut.

Le dernier journal V1.143 mesure stoneskin-observe moyen 278,08 µs contre 1 025,51 µs dans l’ancienne session V1.142, avec scènes/états non contrôlés. Baisse du coût CPU échantillonné observée ; aucun gain FPS/GPU annoncé. Repli : V1.143 en gardant l’INI et en évitant Fire aura rejetée.
