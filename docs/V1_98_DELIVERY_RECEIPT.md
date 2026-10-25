# V1.98 - Livraison des gestes du receveur

CONSTRUITE / CONTROLEE : [55/55 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40135148178), zero avertissement ou erreur MSVC. [ZIP cumulatif immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9f21be738b703e1cc6f40c5422531e4130b57486/downloads/DarksidersGenesis_V1.98_RECEIVER_CASTING_GESTURES_TEST.zip). Gestes et transitions EXPERIMENTAUX, non valides en jeu.

Base cumulative V1.97 `6a0301cde7d3d8d3f1fe19035f2596adcb7c69a4` ; acquis utilisateur retenus : effets/degats Geyser et Caltrops V1.95, effet Clone V1.96. V1.96 complete rejetee ; V1.97 repli controle, pas une version pretendue validee par l'utilisateur.

- Geyser Strife : geste Strife_Aether_Channel_Start. Clone/Caltrops War : War_Channel_Start. Squelettes propres ; animations sources et leurs notifications non modifiees. Geste facultatif selon presence des assets, repli idle conserve. Chronologie des effets/callbacks et durees des casts conservees par le montage prive et sa vitesse.
- Pas de nouveau hook, loader, timer ou fonction de gameplay. Deux identites d'assets dans le scan borne existant. Rampage Strife et World Ender War restent retires ; pouvoirs propres natifs conserves. Code des autres acquis inchange.
- INI : 168 cles, defauts et 55 ID Hotkeys inchanges. 162 anciennes archives verifiees par SHA256, 163 ZIP au total. main/archive/v1.43 verifies intacts ; debug uniquement.
- Source compilee : `5bcc2d78a5594512f1b7a9cb8f69b176c58d6513`. Commit archive : `9f21be738b703e1cc6f40c5422531e4130b57486`.
- ZIP : 954000 octets ; SHA256 `D96B3C8F990CFD05AACC2867CEC9C0FDA051CC55519DFB8C90C5C872F74E0196`. Quatre fichiers racine, CRC valide, ASI/dxgi AMD64, README/INI compares aux sources, lien public HTTP 200 et contenu identique verifies.

Installation manuelle jeu ferme en conservant son INI, raccourcis, PAK et sauvegardes ; aucun fichier installe ni jeu lance par l'agent. Tests : Geyser Strife puis Clone/Caltrops War, vue native/TPS, effets/degats/cout hors hub, fin du pouvoir, deplacement et retour Native. P2 differe. Rollback : ASI/dxgi V1.97 jeu ferme, INI personnel conserve. [Contrat et liste detaillee](V1_98_RECEIVER_CASTING_GESTURES_TEST.md).

Stoneskin : cristal identifie comme l'effet du coeur Stinger par VM_READ et confirme disparu par l'utilisateur lorsqu'il retire ce coeur. Effet attache a la capsule, poussiere au pied du modele ; aucun correctif de position applique. Cette provenance ne prouve pas l'absence d'influence historique du mod. La nouvelle demande de suivi de l'orientation du modele sur tous les axes reste OUVERTE, releve de rotations demande, pas incluse dans ce ZIP.

### Retour utilisateur du 25 octobre 2026 - Stinger cloture

Sur V1.98, l'utilisateur confirme que le cristal du coeur Stinger est dans le dos, bien attache au personnage, et accepte ce rendu comme le comportement vanilla retrouve. **Apparence Stinger VALIDEE par l'utilisateur ; demande de deplacement/orientation vers l'avant CLOTUREE.** Conserver cet effet natif, sans nouvelle adaptation, reattache ou decalage. Le releve de rotations est arrete. Cette validation concerne uniquement Stinger/Stoneskin, pas les gestes Geyser/Clone/Caltrops V1.98, qui restent experimentaux et a tester. Aucun code ASI, INI, raccourci, PAK ou fichier du jeu modifie ; aucun nouveau build.
