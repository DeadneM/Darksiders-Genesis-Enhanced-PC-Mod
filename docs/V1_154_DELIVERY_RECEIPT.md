# V1.154 — bêta cumulative sur debug

**CONSTRUIT et CONTRÔLÉ** sur GitHub Windows x64 : 81/81 tests réussis, aucun avertissement de compilation détecté. **EXPÉRIMENTAL en jeu** pour les nouveaux effets. Base V1.153 acceptée et publiée sur main au commit `563ae9ca8c2647689af8464d320544afe55b1894` ; ce téléchargement main a été relu et son ZIP correspond aux octets validés. [Promotion stable](V1_153_MAIN_PROMOTION.md).

## Nouveaux choix

Appearance → Effect appearances → Other effects → **Ground dash dust (both heroes)**. Native, Dry dust, Coins, Metal sparks, Sand, Snow, Stone debris, Water, Wood debris, Hidden. Défaut Native ; une nouvelle clé Values.DashSurfaceAppearance=0. Les effets sont natifs et partagés par les deux héros. L'ancien Other Strife effects=Hidden garde la priorité ; l'interface le signale. [Périmètre et compatibilité](V1_154_NATIVE_DASH_APPEARANCES_REVIEW.md).

La même fabrique conserve le nombre d'émissions, leurs paramètres et leur cycle de vie. Les animations peuvent émettre plusieurs couches et l'intensité du rendu sélectionné doit être évaluée en jeu. Aucun nouveau hook et aucun changement du code du dash, de la visée, de Stoneskin, du filtre Moloch ou du bouclier. Les anciennes 45 familles, leurs cibles et les 460 identités d'assets sont conservées ; seuls les huit assets de poussière passent au nouveau rôle avec compatibilité de masquage.

## Livraison vérifiée

[Télécharger V1.154](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/4d5791c1ec86dd10e7a0570bc6869249ce4f15f0/downloads/DarksidersGenesis_V1.154_NATIVE_DASH_APPEARANCES_TEST.zip). Source `9bc7441735f37acb21390a04a1ae44334e7cca23`, archive `4d5791c1ec86dd10e7a0570bc6869249ce4f15f0`. ZIP de 2419950 octets, SHA256 `26B0913C00F71FC03D9FDC95F7016AC91965B40DAB4E64CEB16A802B37291CFE`. Deux modules AMD64 à la racine : DarksidersGenesisMod.asi et dxgi.dll ; aucun INI ou README. Les 218 anciens ZIP sont conservés. Les 247 anciennes valeurs par défaut sont inchangées ; le modèle INI compte 248 clés. Les raccourcis et les 55 actions ne changent pas.

Téléchargement immuable HTTP200, CRC, architecture et sélection par les règles de l'updater debug vérifiés. Main reste sur V1.153 ; archive et le tag v1.43 sont inchangés. Defender n'a trouvé aucune menace dans le ZIP et les deux modules ; ses protections sont restées actives. Ces vérifications ne démontrent pas le rendu ou les performances en jeu. [Preuve du ZIP](evidence/V1_154_PACKAGE_CHECK.json) · [GitHub](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/42048756445) · [Defender](evidence/V1_154_DEFENDER_CHECK.json).

Pas de build local, de lancement du jeu, de capture mémoire ou d'installation par l'agent.

## Essai groupé

1. À pied, Strife puis War : comparer Native, Water, Metal sparks, Snow et Hidden, puis les autres variantes, en TPS et en vue native.
2. Revenir à Native pour retrouver les surfaces du jeu. Changer de niveau et vérifier la persistance du choix.
3. Si Other Strife effects était sur Hidden, vérifier son masquage conservé puis le remettre sur Native pour voir les variantes.
4. Vérifier un dash aérien court et maintenu : son fonctionnement doit rester celui de V1.153. Coop/P2 reste différé.

Pour revenir à la base acceptée, utiliser le ZIP V1.153 du reçu de promotion, en conservant son INI et ses raccourcis. Prochaine étape : trier les rendus selon le retour utilisateur, sans modifier les acquis validés.
