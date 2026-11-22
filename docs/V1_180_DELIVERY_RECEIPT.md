# V1.180 — profils HDR identiques sur toutes les cartes

**CONSTRUIT / CONTRÔLÉ ; rendu EXPÉRIMENTAL.** Acquis V1.178 conservés, présentation Graphics de V1.179 cumulée. Main V1.173 intacte, aucune installation automatique.

Graphics garde les cinq groupes déplacés et leurs noms. HDR propose Native (défaut), SDR, HDR10 1000/2000 nits, scRGB 1000/2000 nits et luminosité facultative du HUD. Aucun filtre Intel/AMD/NVIDIA : mêmes profils et même code. Les anciennes branches natives observées ne servent plus de motif de restriction. [Périmètre, six CVars, sauvegarde et rollback](V1_180_REVIEW.md).

Choix enregistrés dans deux clés HDR Native par défaut ; application au fichier Engine.ini après fermeture et effet au lancement suivant. Native restaure les valeurs possédées par le mod, sans écraser une édition externe. Aucun nouveau hook/shader, patch de capacité ou réglage Windows forcé. Demandes au moteur, pas preuve de transport ou rendu HDR correct : l'utilisateur testera.

- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/45072460653) : Windows x64 Release, **86/86 tests, zéro avertissement/erreur C++**. Tests des quatre profils et des six CVars, SDR, idempotence, valeurs invalides, restauration Native, sauvegarde/rechargement, fichiers temporaires UTF8/UTF16. Aucun de ces tests ne valide l'image HDR réelle.
- Source `fd47993dab8851761be9ac653365f6a3d4b2ada8` ; archive `d4a14e458d183a429902bd7703af78e611da9792`.
- [ZIP direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/d4a14e458d183a429902bd7703af78e611da9792/downloads/DarksidersGenesis_V1.180_NATIVE_HDR_REQUEST_TEST.zip) : 2516718 octets, SHA256 `71401679653BE5E94E2CA4BBB92ACF00F1A1124C3E8C9F265506B6978B2C0141`.
- Deux binaires AMD64 à la racine sans INI/README ; CRC/empreintes/blob distant/lien HTTP200 contrôlés. Defender ZIP et binaires : aucune menace détectée, protection intacte. Le blocage d'analyse V1.179 était historique ; l'analyse V1.180 a bien été exécutée.
- 244 anciennes archives intactes, 245 cumulées. 286 anciennes valeurs de référence identiques ; deux ajouts Native, total288 / 13 sections. Mêlée, modèles et raccourcis intacts. Aucun fichier personnel/Engine.ini modifié pendant le développement. Un seul nouveau ZIP récupéré, aucun ancien retéléchargé, aucun build local ou capture mémoire.

## Essai
Dans Graphics > HDR : HDR10 1000 nits, fermer complètement puis relancer avec Windows HDR actif. Comparer couleurs, hautes lumières et menus ; éventuellement plein écran exclusif. Revenir Native avec le même cycle puis essayer scRGB 1000 nits. Garder la luminosité HUD Native au premier essai. Rapporter résultat correct/incorrect/inchangé et mode d'affichage. La profondeur du buffer et HDR Windows ne suffisent pas seuls à valider le transport HDR.

Avant downgrade : choisir Native et fermer le jeu pour retirer les six CVars possédées avant de remplacer les binaires. Le journal .DGE-render-owned.json et la sauvegarde .DGE-render.bak conservent le retour contrôlé. Les anciens helpers ne comprennent pas les nouveaux champs HDR. [Preuves](evidence/V1_180_DELIVERY_CHECK.json).
