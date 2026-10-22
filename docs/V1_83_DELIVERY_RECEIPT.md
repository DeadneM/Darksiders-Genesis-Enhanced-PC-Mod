# V1.83 — livraison contrôlée du prototype de protection Stoneskin

22 octobre 2026. Base V1.82, acquis V1.81 et suivi TPS des exécutions V1.82 validés dans les essais rapportés. Effets des épées V1.82 inefficaces, correction ouverte. **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE ; protection étrangère non validée en jeu.**

**Limite du prototype : seule la protection native et son coût sont transférés.** L'animation compatible de repos ne porte pas les notifications de War : onde de choc de lancement, sons, particules et apparence de pierre ne sont pas encore transférés. Les variantes de cœurs et la protection réelle nécessitent un essai ; ne pas attendre une aura visible du statut de base. Le mélange complet des six capacités reste ouvert.

- Sources compilées : `5f946c208e2f6797db9aa8620dd5ba34b7c47254` ; archive : `aa9a096a232dca76de9456387a7565b69de8925d`.
- [CI](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39841420585) : run 39841420585, job 120043133664, success, **51/51 suites**, zéro avertissement/erreur MSVC.
- Avertissements d'outillage conservés : minimum CMake MinHook ancien, Node20 forcé24, dépréciations punycode/url.parse ; aucun avertissement du code ASI masqué.
- [ZIP public HTTP200 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/aa9a096a232dca76de9456387a7565b69de8925d/downloads/DarksidersGenesis_V1.83_STRIFE_STONESKIN_TRANSFER_TEST.zip) : 896397 octets, SHA256 `95BA3A3667423D27940F94DA07303CE9CE0AF28B1939D05FFDB3CCF94C01AFCB`. Téléchargement public identique à l'archive locale.

| Fichier racine | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1629696 | `5F5B05E1C49762092AF06551C9552BA44C43743DA0D845B6E56E5D02239319D9` |
| `DarksidersGenesisMod.ini` | 4647 | `03D37FB4E6D4C21DF3BE5596FA9451EEEECE21B3A7FF276A8027C49161B30B22` |
| `dxgi.dll` | 155136 | `6352EF2E084F7747A1FC4F53B5F28BAEC61D237132D916169B12A06A893BB0BF` |
| `README.md` | 30205 | `86BC0CF0C612181D2109226D8EBA2996F7985BF246BDE081E246E36616CFDA17` |

CRC, architecture AMD64, version, README et INI comparés aux sources compilées. 147 anciennes archives inchangées, 148 ZIP au total. 166 valeurs INI et 55 ID Hotkeys conservés, Native par défaut dans les six slots. 114 anciens fichiers source inchangés, dont caméra/exécutions, mêlée, bras, holsters, visée, cheval et ressources. Deux hooks facultatifs locaux au pouvoir, signatures exactes ; aucun hook ProcessEvent, écriture de sauvegarde/CDO ou événement d'entrée synthétique.

L'emplacement Strife d'origine doit être débloqué ; coût de la classe Stoneskin native. Strife normal à pied uniquement. En cas d'absence d'asset compatible, seul le slot choisi reste inactif ; pas de substitution silencieuse par un autre pouvoir. Tester protection réelle, coût, expiration et reprise des contrôles ; puis Native et pouvoirs propres. Coop/P2 non validés. Les roues peuvent conserver leurs icônes de départ.

main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` et v1.43 `902861eb1ff34c14dc5ded092581b360767da817` inchangés. Aucun fichier installé, aucune relance, INI/PAK/sauvegardes personnels conservés. Rollback manuel : ASI V1.82 conservé, même INI. Le README emballé reflète la préparation ; ce reçu ajoute les résultats et limites sans modifier l'archive numérotée.

Suite : corriger les effets de mêlée attachés, conserver le suivi d'exécutions validé ; compléter Stoneskin puis étudier Caltrops vers War. Réception lourde incertaine, autres FX P2 reportés, HUD général non reproduit.
