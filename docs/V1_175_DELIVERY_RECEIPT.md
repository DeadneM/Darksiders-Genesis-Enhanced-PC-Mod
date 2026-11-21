> Rectification V1.176 : le bit chromatique décrit ci-dessous était erroné (0x20 au lieu de 0x10). Les tests V1.175 ne prouvaient pas le rendu. Voir V1_176_REVIEW.md.

# V1.175 — intensité chromatique livrée sur debug

**CONSTRUIT / CONTRÔLÉ, rendu EXPÉRIMENTAL.** Base V1.174, motion blur retenu après retour utilisateur. Main/release V1.173 inchangées. Aucune installation automatique.

## Correction et limites

V1.174 réglait un plafond natif, insuffisant pour augmenter l'effet d'une scène. V1.175 règle le champ réel d'intensité dans la sortie du callback caméra existant : aucun nouveau hook, aucune fréquence modifiée, pas d'écriture dans un UObject persistant. Native ne modifie aucun octet de caméra ; le mélange partiel et les données invalides restent natifs. 0 garde également la coupure globale antérieure vérifiée. L'option ne force pas les paramètres qualité/showflags qui supprimeraient cet effet. [Preuve native et périmètre](V1_175_REVIEW.md).

La nouvelle clé ColorFringeAmount est prioritaire ; en son absence, ColorFringeMax de V1.174 est migrée et reste tenue à jour pour rollback. Les anciennes cases Disable* restent compatibles. Les261 anciennes valeurs de référence sont inchangées,262 clés au total. Le motion blur et le DOF ne sont pas modifiés.

## Contrôles

- [GitHub Actions Windows x64 Release](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/44831087633) : **86/86 tests, zéro avertissement et zéro erreur C++**.
- Tests caméra : exactitude du montant, hausse depuis0, bit d'override, conservation de tous les autres octets, Native/invalides/poids partiels, indépendance des sorties P1/P2. Tests configuration : migration V1.174, priorité du nouveau nom, publication via Snapshot et atomique, retour Native.
- Premier run44829828986 rejeté en compilation (membre Snapshot oublié), corrigé avant ce run réussi ; aucun ZIP n'avait été produit par le run échoué. Ne pas considérer cet échec comme un essai en jeu.
- [ZIP direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/f32bdd5a94046def8a7d6a44cd8c976a5ed348ea/downloads/DarksidersGenesis_V1.175_CHROMATIC_INTENSITY_TEST.zip) : 2495214 octets, SHA256 `B1E7EA5E422ACB69C06F439A1ECD681F6A17D60887A7952F46BFEFCE20BD031C`.
- Deux binaires AMD64 seuls à la racine, sans INI/README. CRC, empreintes, blob distant et lien HTTP200 vérifiés. Defender : analyse terminée, aucune détection correspondante. Un seul nouveau ZIP récupéré via Git ; aucun ancien binaire retéléchargé.
- 239 anciennes archives préservées,240 cumulées. Main/archive et réglages de référence inchangés. Aucun fichier du jeu/INI personnel remplacé.

## Essai court

Installer manuellement en conservant l'INI, ou utiliser l'updater debug. Dans une même scène, regarder les bords de l'image en valeurs **0,1,5**, puis décocher Customize chromatic aberration pour revenir en Native. Faire une comparaison vue native/TPS. Vérifier le motion blur inchangé. Les transitions de caméra au mélange partiel gardent le rendu natif par sécurité.

## GPU : diagnostic toujours ouvert

Aucun comparatif exploitable : premier TPS12 échantillons dont0 au premier plan ; aucune mesure native enregistrée. Le profil détaillé WPR a été refusé par Windows, et WPR a été confirmé non actif après cet échec. Pas de conclusion sur les passes coûteuses, le pilote ou le crash HUNG. Aucun correctif GPU inventé dans ce lot. Une future mesure devra garantir le premier plan et disposer d'un accès de profilage adapté ; ne pas multiplier les relevés identiques sans ces conditions.

Rollback : remettre les deux binaires V1.174, INI personnel conservé. [Preuves](evidence/V1_175_DELIVERY_CHECK.json).
