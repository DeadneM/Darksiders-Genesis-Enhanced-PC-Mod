# V1.98 - Gestes propres au receveur

Objectif : remplacer les poses de repos des trois transferts retenus par des gestes natifs du personnage qui recoit le pouvoir, sans reintroduire les transferts rejetes. Critere de reussite : geste visible, effets/degats et duree conserves, deplacement/attaques recuperes normalement apres le pouvoir. **EXPERIMENTAL, NON VALIDE EN JEU.** La reussite des tests automatises ne validera pas le rendu.

Base cumulative V1.97 `6a0301cde7d3d8d3f1fe19035f2596adcb7c69a4`. Acquis utilisateur retenus : effets/degats Geyser et Caltrops V1.95, effet Clone V1.96. V1.96 complete rejetee ; V1.97 repli controle mais non declare valide en jeu. Les animations V1.98 ne sont pas celles du donneur : gestes de lancement compatibles, pas un retarget des squelettes.

## Changement limite

- Strife : Strife_Aether_Channel_Start, squelette Strife_Skeleton, 1 seconde, root motion desactive nativement, trois notifications de son.
- War : War_Channel_Start, squelette War_Mesh_Skeleton, 0,5666667 seconde, racine constante dans le PAK d'origine (une cle de translation et une de rotation). Le drapeau root motion natif reste intact ; les seules notifications sont un son et deux pas.
- Deux identites faibles d'assets ; decouverte via le scan deja present, 512 objets maximum par passage. Aucun nouveau scan, hook, chargement force, minuterie ou modification d'asset partage. La pose precedente reste le repli si le geste est absent, expire ou incompatible.
- Geyser/Clone : seulement la sequence du montage prive, sa plage et sa vitesse changent ; callback natif et effets conservent leurs temps. Caltrops : meme usine native a huit arguments, duree de cast conservee par la vitesse, un seul spawner depuis le bassin War.
- Aucun reglage, raccourci ou identifiant Hotkeys ajoute. Rampage sur Strife, World Ender sur War et adaptation Stoneskin restent absents. Pouvoirs propres, souris/manette, tirs/cameras, cheval, roues et effets valides inchanges.

[Contrats des animations et preuves du PAK/jeu](evidence/V1_98_RECEIVER_GESTURE_CONTRACTS.json). Les gestes peuvent etre moins naturels qu'une animation dediee ; ce rendu et leur interruption restent a valider. P2 reporte.

## Cristal Stoneskin : diagnostic termine pour sa provenance

Lecture externe uniquement, QUERY_LIMITED_INFORMATION|VM_READ, sans appel natif ni ecriture. 1184 echantillons, 90 changements en cinq minutes. Le statut MayhemStatusEffect_Core_Stinger_2_Blueprint_C possede P_Stinger_CreatureCore_StoneSkin, attache au CollisionCylinder de War (socket None). Effet a la hauteur de la capsule ; poussiere StoneSkin a Bone_War_Root, 97 unites plus bas. Deux activations avec Stinger, puis une activation avec les effets Stoneskin ordinaires et sans Stinger. L'utilisateur confirme : « bien vu le crystal nest plus » apres retrait du coeur.

Cela identifie le visuel signale et le coeur qui le cree. Cela ne prouve pas que le mod n'a jamais affecte son positionnement historique ; aucune mesure sans ASI n'a ete faite. Aucun repositionnement ou masquage global n'est ajoute. Stoneskin natif reste intact. [Observation compacte et limites](evidence/V1_98_STONESKIN_STINGER_OBSERVATION.json).

## Essais groupes apres installation manuelle

1. Strife a pied : Geyser en vue native puis TPS. Verifier geste, lames/nova, degats, consommation hors hub, fin et retour au deplacement ; refaire une fois apres changement de personnage.
2. War a pied : Clone puis Caltrops, dans les deux vues. Verifier geste, un seul effet/attaque, cout, fin normale, puis essayer une interruption autorisee par le jeu. Aucun blocage durable attendu.
3. Revenir aux emplacements Native : pouvoirs originaux inchanges. Rampage Strife et World Ender War restent indisponibles ; ne pas les considerer corriges.
4. Facultatif : Stoneskin avec/sans Stinger pour reconnaitre son effet ; aucun nouveau correctif visuel dans ce lot. Acquis et P2 a verifier separement si necessaire, sans validation supposee.

Rollback : jeu ferme, remettre l'ASI/dxgi de V1.97 en gardant son INI. Revenir a Native pour un transfert insatisfaisant. Toutes les anciennes archives sont conservees ; installation et lancement laisses a l'utilisateur.

### Retour utilisateur du 25 octobre 2026 - Stinger cloture

Sur V1.98, l'utilisateur confirme que le cristal du coeur Stinger est dans le dos, bien attache au personnage, et accepte ce rendu comme le comportement vanilla retrouve. **Apparence Stinger VALIDEE par l'utilisateur ; demande de deplacement/orientation vers l'avant CLOTUREE.** Conserver cet effet natif, sans nouvelle adaptation, reattache ou decalage. Le releve de rotations est arrete. Cette validation concerne uniquement Stinger/Stoneskin, pas les gestes Geyser/Clone/Caltrops V1.98, qui restent experimentaux et a tester. Aucun code ASI, INI, raccourci, PAK ou fichier du jeu modifie ; aucun nouveau build.

