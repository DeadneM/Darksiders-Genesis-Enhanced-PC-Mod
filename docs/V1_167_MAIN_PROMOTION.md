# V1.167 — validation globale et promotion main sans build

Le 13 novembre 2026, l'utilisateur confirme « je valide tout, pousse sur main mais ne build pas encore ». Cette validation globale remplace les réserves précédentes de la file ; aucun détail d'essai individuel supplémentaire n'est inventé.

Base debug acceptée : `4904fa7d157527a614cfe4bcf62b312cdfabf8f5`. Source compilée `df38da77fc8fe347c539567197e642584acba86b`, archive `700335bc5fa08bd4b579b512459dd738e9cfe77c` ; 84/84 suites Windows réussies, aucun avertissement C++ détecté. Sources, tests, configuration, updater et workflows identiques au build livré.

Main précédent `e0ac8a44a9fc79425218c452efab7128acecb56f` (V1.164) reste dans l'historique. Promotion par avance rapide, sans force, avec commit de documentation `[skip ci]`. Aucun dispatch, tag, release ou changement permanent de workflow. [Mécanisme GitHub pour éviter les workflows push](https://docs.github.com/en/actions/how-tos/manage-workflow-runs/skip-workflow-runs).

Le ZIP V1.167 déjà testé est conservé octet pour octet : `DarksidersGenesis_V1.167_NATIVE_MOLTEN_CLOTH_TEST.zip`, 2 481 902 octets, SHA256 `C2521B0FB13032C891AB2C260D7D97D78231969F522B9FA8B041039EFDDFB93E`. Son nom historique TEST ne justifie ni renommage, ni réemballage, ni nouvelle compilation. Les deux binaires restent seuls à la racine, sans INI/README ; 252 valeurs et 55 actions Hotkeys conservées.

Branche archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, tag v1.43 et toutes les anciennes archives protégés. Aucun fichier de l'installation personnelle, INI, raccourci, PAK ou sauvegarde modifié.

Suite sur debug : recherche des correctifs communautaires applicables au jeu et à son moteur, avec sources et preuves locales. Aucun nouveau patch ou build autorisé implicitement par la recherche ; le report de build demandé reste actif.

Après publication : vérifier les références distantes, l'identité du ZIP/sources et l'absence de nouvelle exécution Actions pour le commit promu. [Livraison existante](V1_167_DELIVERY_RECEIPT.md).

## Résultat distant vérifié

Main et debug publiés au commit `6795f478f2a43be490e0b69ed6f3dc71ebdcb95a` au contrôle de promotion. ZIP et sources compilées inchangés, 232 archives intactes ; requête GitHub Actions pour ce SHA : zéro exécution. Archive et tag v1.43 inchangés. La suite de recherche ajoute ensuite seulement des documents sur debug. [Preuve](evidence/V1_167_MAIN_PROMOTION_CHECK.json).
