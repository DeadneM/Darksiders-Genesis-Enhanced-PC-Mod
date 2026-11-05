# V1.153 — promotion de la bêta validée sur main

Le 5 novembre 2026, l'utilisateur demande explicitement de publier la dernière bêta sur main et de garder les nouveaux effets dans une bêta distincte.

Base acceptée : `018df88a3518f935b2b29ad7304ed579e6944ff0`, source compilée `1a4bbe29f65e8533ba8741cd9a93e3ee30b42a28`. Le retour utilisateur ne signale aucun problème majeur sur ses derniers essais ; les récupérations coffre et dash sont acceptées avec sa réserve sur le résultat. Coop/P2 reste différé et aucun gain FPS n'est déclaré mesuré.

La promotion reprend les sources, les réglages par défaut et le ZIP existants de V1.153, sans recompilation ni remplacement de ses octets. Le ZIP contient seulement DarksidersGenesisMod.asi et dxgi.dll à la racine ; ni INI personnel ni README n'y sont inclus. GitHub Windows avait réussi 81/81 contrôles sur cette source. [Livraison et limites](V1_153_DELIVERY_RECEIPT.md).

[Téléchargement immuable V1.153](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/6063efc6a06e2f760dec404fe67d5a19ae451fb9/downloads/DarksidersGenesis_V1.153_STONESKIN_LIFECYCLE_COST_TEST.zip). SHA256 : `BAAB9A9D41096983349B5650F4FE01F1DF7E073FDD9B42795FD3A0F52E67A9CD`.

Main précédent : `e2e519f7ea84d4b773fd7a31611779ae68308696`, conservé dans l'historique. Archive reste `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, tag v1.43 reste `902861eb1ff34c14dc5ded092581b360767da817`. Aucun ancien ZIP n'est supprimé. Cette promotion ne crée pas de tag ou de nouvelle release GitHub.

Les nouveaux choix de poussière de dash seront développés et contrôlés uniquement sur debug après cette promotion. Ils ne font pas partie de V1.153 sur main.
