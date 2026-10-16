# Darksiders Genesis Enhanced PC Mod V1.43

Build Windows x64 en configuration **Release**, cumulatif depuis V1.42. Cette version reprend le profil enregistré par l'utilisateur dans l'INI comme valeurs par défaut de l'ASI, de l'INI livré et des boutons Default/reset.

## Réglages caméra

| Vue TPS | Distance | Décalage vertical |
| --- | ---: | ---: |
| À pied | 0,15× | 0 |
| À cheval | **0,25×** | **+40 unités** |
| Forme ultime | 0,30× | +80 unités |

Pitch commun : −12°. FOV TPS :110°. Seuls la distance et le décalage vertical à cheval ont changé par rapport aux défauts V1.42 ; les110 autres valeurs sont conservées. Les profils existants gardent la priorité et les anciennes clés absentes conservent leur héritage compatible.

Les contrôles souris/manette, rôles des sticks, RawInput/Alt-Tab, cheval, roues, cadrages indépendants, munitions, gains/remplissages de jauges, Synergy et Skip Warning gardent leurs chemins existants. La version affichée dans Updates suit automatiquement V1.43. Les calculs de gameplay et les hooks natifs n'ont pas changé pour cette release de défauts.

**Action Recovery reste indisponible.** La prochaine investigation se concentre uniquement sur le **dash** ; les coffres sont reportés. Aucun retour de l'ancien hook sans effet, aucune réparation recovery annoncée dans cette release.

## Installation

Archive cumulative à quatre fichiers, directement à la racine :

- DarksidersGenesisMod.asi
- dxgi.dll
- DarksidersGenesisMod.ini
- README.md

Fermer le jeu, sauvegarder les fichiers actuels puis installer dans `DarksidersGenesis/ProjectMayhem/Binaries/Win64`. Conserver son INI personnel lors d'une mise à jour ; les nouveaux défauts servent aux nouvelles installations, resets et boutons Default. Le programme de mise à jour intégré conserve l'INI existant.

La release est publiée depuis **debug** ; les branches main/archive et les archives précédentes sont conservées. Le profil sélectionne le canal de mise à jour debug. La compilation Release, les tests de configuration/politiques et les contrôles d'archive établissent la construction et l'intégrité ; ils ne remplacent pas une nouvelle validation en jeu de tous les chemins.

Le fichier SHA-256 joint permet de vérifier le ZIP téléchargé. Cette publication ne prétend pas corriger les limites de gameplay déjà documentées ni apporter un gain de FPS mesuré.
