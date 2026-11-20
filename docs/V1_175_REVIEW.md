# V1.175 — véritable intensité chromatique

Base debug V1.174 d64041d ; main/release V1.173 conservées. Retour utilisateur : motion blur fonctionne, chromatique sans effet apparent. Motion blur retenu et inchangé. Aucun gain GPU ou résolution du crash HUNG déduit.

## Cause démontrée et correction préparée

Le consommateur natif de r.SceneColorFringe.Max (RVA1B22BCE) effectue un minimum avec l'intensité de la scène : c'est un plafond, pas une commande d'intensité. Une scène à zéro reste donc à zéro en augmentant le curseur V1.174. Le champ réel SceneFringeIntensity est identifié dans les métadonnées FPostProcessSettings : offset0x210, bit d'override+5/mask0x20, setter natif 1DD2CA0. Nom, offset et setter sont vérifiés avant activation, indépendamment des autres options TAA/motion blur. [Contrat hors jeu](evidence/V1_175_CHROMATIC_BOUNDARY.json).

Le callback caméra EXISTANT applique maintenant le montant explicite au champ de sa sortie copiée, avant les retours Native/TPS/P2. Aucun nouveau hook, timer ou scan. La valeur vient d'un atomique publié lors du chargement/enregistrement des réglages ; aucun accès non synchronisé au menu depuis la caméra. Native (-1) n'écrit rien. 0 conserve aussi l'ancienne coupure globale vérifiée. Pour une valeur positive, tout plafond appartenant au mod est restauré, afin de ne pas contrer la nouvelle intensité.

Garde-fous : sortie non nulle, valeur finie0–10, champ natif cohérent et mélange caméra exactement1. Une transition au mélange partiel reste native : on ne force pas le poids de tous les autres effets. L'option ne contourne pas un effet supprimé par les paramètres de rendu/qualité du jeu. Le contrat est désactivé à l'arrêt ; aucune restauration par pointeur UObject conservé.

## Menu, migration et tests

Camera → Image effects : Customize chromatic aberration, Amount0–10 avec saisie ; Native décoché par défaut, valeur d'essai1 lors de l'activation. Nouvelle clé ColorFringeAmount=-1. Ancienne ColorFringeMax lue si la nouvelle est absente, et maintenue pour rollback ; anciens DisableColorFringe toujours migrés. Les 261 anciennes valeurs de référence sont inchangées,262 au total. Aucune écriture dans l'INI personnel par le développement.

Tests préparés : tous les octets de caméra conservés sauf le float et son bit ; Native/invalides/mélanges partiels, hausse depuis zéro, P1/P2 indépendants ; migration V1.174, priorité nouvelle clé, publication runtime et restauration. Contrôles Windows uniquement GitHub ; pas de build local. État avant CI : PRÉPARÉ / EXPÉRIMENTAL, aucune réussite visuelle annoncée.

À tester : dans la même scène, en Native puis TPS, valeurs0/1/5 puis retour Native. Regarder les franges sur les bords de l'image. Vérifier motion blur inchangé et un changement de niveau. Les transitions partielles conservent le rendu natif par sécurité.

## Diagnostic GPU séparé

WPR détaillé refusé par Windows (0xc5585011), aucun enregistrement WPR laissé actif. Premier relevé TPS : 12 échantillons,0 au premier plan, EXCLU de toute conclusion. Les attentes suivantes de vue native n'ont produit aucun fichier de mesure ; aucun comparatif TPS/native valable disponible. Firefox a été observé au premier plan. L'autorisation automatique a ensuite temporairement refusé une commande pour quota, sans l'exécuter ; reprise autorisée après résolution. Aucun défaut GPU/driver ni passe coûteuse identifiés. Ne pas lancer de nouvelle mesure avant de garantir le premier plan ; ne pas remplacer un profilage détaillé par une fausse conclusion issue des compteurs.

ZIP cumulatif prévu : DarksidersGenesis_V1.175_CHROMATIC_INTENSITY_TEST.zip (deux binaires racine, sans INI/README). Pas d'installation ou lancement automatique. Rollback : deux binaires V1.174 avec INI conservé. Main/archive et anciens ZIP préservés.


Premier contrôle Windows44829828986 : échec de compilation, valeur chromatique absente de Snapshot. Corrigé en ajoutant le float à Snapshot, puis sa publication atomique dans runtime::Publish ; aucun ZIP issu du contrôle échoué. Nouveau contrôle requis avant livraison.
