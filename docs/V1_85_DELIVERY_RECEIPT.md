# V1.85 — reçu du lot cumulatif contrôlé

22 octobre 2026. Branche **debug** uniquement. Base cumulative V1.84 `6e27cf78983f196683d786dae5c7372b9a4b1351`. Acquis V1.81 et suivi TPS des exécutions V1.82 validés par l'utilisateur dans les essais rapportés, conservés sans changement. **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE** pour les nouveaux effets et la protection étrangère, aucune validation visuelle déduite des contrôles.

- Sources compilées : `38d4d7aae30e0640b2ec8dcad1de98c20473fe76` ; archive : `6f05855e2c3ba54b904d78116bc9562895ff997a`.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39845333890) : run 39845333890, job 120056987651, success, **51/51 suites**, zéro erreur et zéro avertissement MSVC. Taille privée de Plan=128 octets contrôlée à la compilation ; neuf arguments du spawn attaché, rotation/échelle non uniforme, refus des contextes incompatibles et chemins natifs vérifiés.
- **C4324 de V1.84 réellement supprimé** par réorganisation des champs, sans masquage. Historique et archive intermédiaire conservés. Seuls cette disposition, le test de taille et les métadonnées/logs changent depuis V1.84 ; aucun calcul ou contrat natif changé.
- Avertissements d'outillage inchangés : minimum CMake MinHook ancien, Node20 forcé24 et dépréciations punycode/url.parse. Aucun avertissement du code ASI masqué.
- [ZIP public HTTP200 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/6f05855e2c3ba54b904d78116bc9562895ff997a/downloads/DarksidersGenesis_V1.85_COMPACT_BARE_BLADE_FX_TEST.zip) : **899037 octets**, SHA256 `751DCE13BC9223313E7801C4E0F7E0672672B7D430DAF34512FE495EF96B0FC3`. Téléchargement public identique à l'archive locale.

| Fichier racine | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1633280 | `6623EE084E0D0DD1DBFD8A9AC97335C801642F765FFCFEE9D8A5DF329CC5B41D` |
| `DarksidersGenesisMod.ini` | 4647 | `03D37FB4E6D4C21DF3BE5596FA9451EEEECE21B3A7FF276A8027C49161B30B22` |
| `dxgi.dll` | 155136 | `7E2E8D639EEC5891EF15B225BE146378D15A31F702A7143F988E81C1A141C487` |
| `README.md` | 35355 | `E3DB9BB4C3F6FB80FA4F3B664089F95BAB4B509AEB2C39838DEAB1BD04DF3541` |

CRC, AMD64, version, README/INI des sources compilées contrôlés. **149 anciennes archives inchangées**, 150 ZIP au total. **166 réglages INI et 55 ID Hotkeys préservés**. **120 anciens fichiers source inchangés** depuis V1.84, y compris toutes les caméras, bras, tirs, inputs/roues, cheval et ressources. Signature de l'EXE et des trois frontières natives contrôlée en V1.84 ; elles sont strictement identiques en V1.85.

## Lot et essais

Effets de mêlée Strife : huit notifications slash/stab attachées à Bone_Strife_Root étaient exclues par V1.82. Elles sont maintenant traitées dans le même contexte possédé que les douze notifications détachées des troisième coup/variante `_03_b`. Copies privées de position à la main et taille ×0,4 ; parent, socket, orientation et lifecycle natifs conservés. Dégâts, portée et poses inchangés. Arcs communs ajustés seulement avec les deux épées masquées. Le relevé des objets ne prouve pas les arguments du callback : passage et rendu à confirmer en jeu. [Contrats VM_READ](evidence/V1_84_MELEE_ATTACHED_CONTRACT.json).

Capacités : prototype V1.83 inclus sans modification, protection Stoneskin de War vers Strife normal à pied dans un slot débloqué, coût natif. **Partiel / non validé en jeu** : pas de transfert de l'onde de choc de lancement, des sons ou de l'apparence de pierre. Les cinq autres transferts restent ouverts. Le mélange complet des six capacités n'est pas livré. [Contrats et limites V1.83](V1_83_DELIVERY_RECEIPT.md).

Test groupé : lames visibles puis gauche/droite/deux masquées, combos au sol et aériens ; contrôle court des bras, roues, tirs et suivi d'exécution TPS. Puis, facultativement, Stoneskin : vérifier coût/protection temporaire/reprise des contrôles/expiration, Unlimited Wrath/Health désactivés et variantes Molten retirées pour comparer le statut de base, puis retour Native. L'aura n'est pas un critère de ce premier transfert. [Détail des essais](V1_85_COMPACT_BARE_BLADE_FX_TEST.md).

main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` et v1.43 `902861eb1ff34c14dc5ded092581b360767da817` conservés. Fichier de reprise non suivi intact et hors commit. Aucun fichier du jeu installé, aucune relance ni écriture externe de mémoire. INI, PAK et sauvegardes personnels préservés. Installation manuelle ; rollback ASI V1.82 conservé pour le dernier suivi d'exécution validé, même INI. README emballé figé à sa préparation ; ce reçu ajoute les résultats sans réécrire l'archive.

Suite : retour visuel des arcs, compléter Stoneskin puis adapter Caltrops vers War. Réception lourde incertaine, autres FX P2 reportés, HUD général non reproduit, aucune nouvelle validation coop supposée.
