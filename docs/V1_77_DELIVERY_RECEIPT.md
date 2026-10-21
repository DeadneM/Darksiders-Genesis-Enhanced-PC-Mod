# Reçu V1.77 — 21 octobre 2026

**CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE, NON VALIDÉE EN JEU ; NON INSTALLÉE PAR L'AGENT.** Base cumulative V1.76, base explicitement validée V1.73 pour les essais rapportés. « Continue comme si j'avais validé » autorise l'avancement sans attendre les tests ; cela ne crée aucune observation en jeu. ASI installée relue : V1.75, SHA256 `0F41EA56889E553494DFAE77F392A62CD4C8B933F4C3293DE08E9CE5D6AECD56`.

## Livrable vérifié

[ZIP cumulatif immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b5da4721104b0b5a08911bf8cc51af88a4559f6f/downloads/DarksidersGenesis_V1.77_NATIVE_WRATH_SLOTS_TEST.zip) — HTTP200, **856818 octets**, SHA256 `47507C0A5901B8C2E44DFBC16EFB5964A6523C35759EE2FBF5D0C9625CEE15C1`. Téléchargement public comparé au ZIP CI et local, octet pour octet via empreinte et taille. Les quatre entrées sont à la racine ; CRC et les deux PE AMD64 vérifiés.

| Fichier | Octets | SHA256 |
| --- | ---: | --- |
| DarksidersGenesisMod.asi | 1513984 | `52D12480EE5572DEECD3CFB0A465C34483C5AF7777B7DA5B288489C786F843A4` |
| DarksidersGenesisMod.ini | 4587 | `6D35759332C234D21867F509A542F9EE97046648DDDF15B4C80D5555B8BA4345` |
| dxgi.dll | 155136 | `EF7E9F5D20A718EB258B8F1470A8704E7846A672CE94EE5EA65CB789BBBF598C` |
| README.md | 17864 | `710EF8B82BE000FFED0FC8469AEDD9E47E955F0FAA8447B4251998892731E566` |

Le README embarqué contient la préparation V1.77 avant CI ; le présent reçu et le README du dépôt donnent le statut après contrôle. L'archive numérotée n'est pas réécrite pour changer cette documentation.

## Source et contrôles

- Branche **debug seulement**. Source construite `1c8d4210101b97fa9277b24335f8e6dcc41d37d5` ; commit de l'archive `b5da4721104b0b5a08911bf8cc51af88a4559f6f`.
- [Actions run 39632822621](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39632822621), job119335320776 : **47/47 suites réussies, zéro avertissement MSVC**. Nouvelle suite WrathSlots passée dès la première construction. Tests de frontière isolés, pas exécution du jeu.
- ABI à cinq arguments et retour destination ; 18 combinaisons natives de personnages/emplacements/pouvoirs, résultats natifs coût/déverrouillage, indices P1/P2 inversés, ID réordonnés, réentrance, contextes invalides et formes, absence d'écriture des acteurs/ressources, arrêt et signatures. Six clés INI round-trip et valeurs inconnues, raccourcis personnalisés conservés ; toutes les anciennes suites repassées.
- 163 clés INI, six nouvelles Native ; **157 anciennes valeurs identiques sémantiquement**, 55 ID Hotkeys conservés. Compilé/INI fourni contrôlés ensemble.
- **141 anciennes archives identiques**. Quinze modules identiques à la base : ProjectileAimFeature, AmmoConsumptionFeature, NativeTpsFeature, CoopCameraFeature, AirJumpFeature, CosmeticUnlockFeature, GrappleRangeFeature, ManualAimFeature, HeavyLandingFeature, HealthFeature, PotionFeature, HotstreakFeature, PistolVisualFeature, DashCombatFeature, MountedHolsterFeature. Le point d'intégration ajoute seulement initialization/shutdown ; aucun nouveau callback par frame.
- Références distantes relues : main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, tagv1.43 `902861eb1ff34c14dc5ded092581b360767da817`, inchangés. Fichier de reprise préexistant non suivi conservé, SHA256 `8872360A13AD7F0C6ADE23E5A7D9DF2FBBD65C04DCA03F6BCDFACC893BC77E01`.

## Ce qui change et ce qui reste ouvert

Gameplay → **Wrath ability slots** : trois choix par personnage, parmi ses trois pouvoirs natifs, tous Native par défaut. Une frontière fonctionnelle réaffecte le même ID pour la classe, le déverrouillage et le coût. Ni touche, ressource persistante, sauvegarde, CDO, animation ou composant modifiés par ce candidat. Pas de cache d'objets.

**Demande de choix parmi les six capacités non terminée.** Les squelettes, notifications des montages/séquences et composants du clone/rayon doivent être adaptés avant lancement étranger. 40/40 fonctions Blueprint lues complètement ; aucune classe étrangère lancée. Indications natives de roue éventuellement anciennes : vérifier leur affichage séparément. Les pouvoirs cheval/ultime restent natifs.

OFF universel par pistolet reste ouvert ; candidat à pied V1.76 conservé. Visée sans tir et réception lourde V1.75 conservées, non déclarées validées par CI. HUD général en surveillance, autres FX P2 reportés par l'utilisateur, notification updater à confirmer.

## Essai et rollback

[Plan groupé complet](V1_77_NATIVE_WRATH_SLOTS_TEST.md) : permuter les capacités de Strife puis War ; vérifier effets, maintien/fin d'action et coût en mission à zéro Courroux ; rétablir les six Native. Puis vues native/TPS, souris/manette, P1/P2 et personnages inversés, roues, cheval/ultime, chargement. Cumuler avec visée sans tir, réception lourde des deux personnages, OFF à pied et contrôles courts tir/dash/rangement.

Installation manuelle ultérieure, conserver l'INI personnel/raccourcis ; aucune fermeture ou relance demandée, aucun fichier du jeu/PAK/sauvegarde écrit par l'agent. Retour aux six Native, ou ZIP V1.76/V1.73 en gardant l'INI personnel. Aucun résultat en jeu déduit de la compilation, des tests ou des empreintes.
