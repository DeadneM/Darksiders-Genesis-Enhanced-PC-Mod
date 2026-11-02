# V1.143 - reçu de livraison

**CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL en jeu. Non installé.** Base V1.142 installée ; source c3f3b67200746bafc913db546339587acb969a0e, archive 5d822f7127b09ff60ee0bbfe17932be6cfd81c83. [ZIP direct immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/5d822f7127b09ff60ee0bbfe17932be6cfd81c83/downloads/DarksidersGenesis_V1.143_STONESKIN_CORE_VISUALS_TEST.zip).

- Windows x64 compilé uniquement sur GitHub : [exécution](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41436489123), 70/70 contrôles, aucun avertissement C/CS/LNK/D détecté. Le cache seul avait également passé sa première passe séparée.
- ZIP : 2282222 octets, exactement DarksidersGenesisMod.asi et dxgi.dll à la racine, PE AMD64, CRC valides, sans compression, sans INI/README. SHA256 A0AB45769222DBF03ED530735D5B53A5F91E6CECFE3D08AF1AC682C244303A59.
- Catalogue debug de l’updater : V1.143 sélectionnée, téléchargement HTTP 200 et empreinte conforme. Interface du jeu et installation post-fermeture non observées pour ce lot.
- 207 anciennes archives Git inchangées, 208 au total. Main/archive/tag v1.43 intacts. Les deux fichiers installés correspondent encore aux empreintes V1.142 ; aucun remplacement effectué. INI/raccourcis personnels non écrits par cette intervention.
- 228 anciennes clés/defaults de référence conservées ; seule nouvelle clé StingerCoreAppearance=0, soit 229 clés dans le profil de référence. 55 ID Hotkeys inchangés. Leviathan séparé et conservé.
- Defender : aucune menace détectée sur ZIP et deux fichiers, protections actives avant/après et inchangées.

## Ce qui est livré

Cache borné et validé par identités pour le suivi Stoneskin Strife. En bas d’Appearance après Leviathan, groupes fermés initialement Legion shieldbearer (Native/Hidden/Fire aura) et Stinger crystal (Native/Hidden), disponibles dans les deux vues. Pas de doublon dans la bibliothèque générale. Hidden=1 reste Hidden pour les anciens réglages ShieldDemon. Le nom français Porte-bouclier de la Légion est confirmé par la localisation native ; le cœur Legion ordinaire n’est pas modifié.

**À vérifier en jeu** : fluidité et rendu Stoneskin normal/Abyssal ; aura au prochain déclenchement du bouclier lorsque sa ressource est chargée ; cristal Stinger caché avec les autres visuels Stoneskin conservés, puis retour Native ; emplacement des deux groupes et comportement en vues TPS/normale. Les effets existants peuvent conserver leur rendu jusqu’à la prochaine activation. Aucun gain FPS/GPU ni résultat visuel déclaré validé. P2 différé.

Pose après changement de munition encore ouverte : captures comparatives en lecture seule, aucune commande AIM bloquée démontrée, pas de correctif spéculatif. Dash aérien explicitement reporté. Les timings/pouvoirs, tirs, caméra, touches/manette, roues et autres fonctions validées conservent leurs chemins précédents. [Contrat complet et repli](V1_143_STONESKIN_CORE_VISUALS_TEST.md).
