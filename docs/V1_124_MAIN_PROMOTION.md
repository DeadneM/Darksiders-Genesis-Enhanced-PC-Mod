# Promotion V1.124 vers main - 28 octobre 2026

Demande explicite de l'utilisateur : « oui push en main notre derniere version puis on sattaque au point plus haut et refaire deux passes d'audit ».

main avance sans force de `2bc3122c1ae07b7333798462f85c1d9fd3a739f7` a `e2e519f7ea84d4b773fd7a31611779ae68308696`, le commit de validation V1.124. Son ancien sommet appartient a l'historique retenu. Aucune nouvelle compilation/release/installation : meme archive cumulative et memes octets que ceux testes par l'utilisateur.

Le listing de l'updater main selectionne V1.124. Telechargement HTTP200, SHA256 `9E342FB93CAA9472BAB026D90271666A14AF00BF8EA537AA011E0D5DF242CD36`, quatre fichiers racine/CRC/stockage sans compression controles. Le nom historique du ZIP contient TEST ; il n'a pas ete renomme ou reconstruit pour la promotion. [Preuve](evidence/V1_124_MAIN_PROMOTION.json).

Les suites de livraison sont celles de la source compilee V1.124,65/65. Validation utilisateur bornee a ses essais rapportes, pas une certification exhaustive des apparences ou de P2. L'utilisateur signale maintenant des variantes d'apparence sans effet/peu interessantes ; leur experimentation reste ouverte. Ancien HUD general absent : plus reproduit. Autres effets P2 : « cetait ok il me semble », surveillance sans nouveau bug confirme ; coop differée.

archive et v1.43 inchanges. main reste sur V1.124 ; les nouvelles recherches et nettoyages vont seulement dans debug. Pour retour eventuel a l'ancien main, son commit reste dans l'historique ; aucune branche archive reecrite.
