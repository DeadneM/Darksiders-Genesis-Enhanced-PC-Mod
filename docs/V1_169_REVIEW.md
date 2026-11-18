# V1.169 — options d’image et recherches natives

## Base et objectif

Base : V1.168 entièrement VALIDÉE, main/release intactes. Nouvelle demande du 18 novembre : poursuivre la recherche, construire une beta puis fournir les tests. Statut avant CI : **PRÉPARÉ / EXPÉRIMENTAL**. Aucun nouveau réglage activé par défaut, installation non demandée pour cette version.

## Lot livré

Camera → Image effects, groupe fermé par défaut comme les autres :

- **Disable motion blur**, OFF par défaut : force seulement l’intensité native du flou de mouvement à zéro ; OFF restaure la valeur capturée, notamment le sentinelle natif négatif.
- **Disable chromatic aberration**, OFF par défaut : plafonne à zéro les franges colorées des bords ; OFF restaure le plafond natif capturé. La différence n’est visible que si la scène utilise cet effet.

Ces commandes sont globales : vues native/TPS, joueurs locaux et cinématiques. Elles sont indépendantes de Customize temporal AA et des profils de profondeur de champ. La documentation [Epic UE4 sur le post-traitement](https://dev.epicgames.com/documentation/en-us/unreal-engine/post-process-effects?application_version=4.27) explique les couches d’effets ; l’applicabilité est vérifiée dans notre EXE UE4.22, pas déduite de la documentation plus récente.

Réutilisation du service CVar float existant, sans nouveau hook ni écriture Engine.ini. Chaque canal inactif est ignoré ; aucun accès aux données natives quand tout est OFF. Contrats exacts r.MotionBlur.Amount / r.SceneColorFringe.Max : type float, drapeaux 0x60, enregistrement, slots et consommateurs de post-traitement vérifiés. Restauration conditionnelle et identité d’allocation conservées. 255 anciennes valeurs inchangées, deux nouvelles clés Features/DisableMotionBlur et DisableColorFringe : 257 au total. 55 actions Hotkeys conservées. [Précontrôle](evidence/V1_169_PREBUILD_CHECK.json).

Aucun gain de FPS ou correctif de streaming annoncé. Ce sont des options de rendu à valider, pas une optimisation moteur universelle.

## Quel est SoulBarrier_Orb_Blueprint ?

Le code le décrit comme un **orbe/cache d’âmes destructible du décor lié aux barrières d’âmes** : classe native SoulBarrier, collision OrbCollision, composant d’objectif, santé/barrière et effets P_SoulCache_Main/Destroyed/Vulnerable, sons d’attente et de destruction. Son apparence exacte à l’écran n’a pas été observée ici.

Les 13 fonctions Blueprint ont été décodées intégralement. La dépendance SourceControl n’est pas seulement un nom résiduel : ExecuteUbergraph appelle réellement Default__SourceControlHelpers.IsEnabled(), puis teste son résultat dans une branche. La suppression aveugle est donc écartée. Aucun asset/PAK natif n’est modifié dans cette build ; la recherche de cette erreur unique de chargement reste distincte des options d’image. [Diagnostic antérieur](NATIVE_STREAMING_REVIEW_2026_11_18.md).

## Niveaux des cœurs dans l’arbre

Nouveau retour utilisateur : les niveaux manquent au premier affichage, puis apparaissent après retour au menu précédent et réouverture ; l’utilisateur pense que cela concerne l’arbre d’équipement. Objets natifs examinés : CharacterPage_CreatureCoresFullscreen, CoreTreeNodeMajor/Minor. Fonctions identifiées : OnShowing, HandleShowing, RefreshNodes, initializeData et Refresh. Le filtre trait/arc vérifie déjà le shader réellement lié avant de bloquer un draw ; le code src ne contient pas de remplacement des niveaux de cœurs.

**Défaut ouvert, pas annoncé corrigé.** Une comparaison en lecture seule du widget cassé puis rétabli est demandée ; le jeu était fermé au contrôle. Sans état lisible, ni rafraîchissement forcé, ni changement de propriété/visibilité, ni hook d’interface supplémentaire ajouté au hasard. L’origine mod/native reste à établir. Ce point reste dans la file après le lot d’image, sans bloquer les travaux indépendants.

## Test utilisateur après construction

1. Avec les deux nouvelles cases OFF, comparer au rendu V1.168.
2. En vue native puis TPS, tourner rapidement la caméra : activer Disable motion blur, puis OFF. Vérifier la suppression et le retour du flou de mouvement ; le flou de profondeur de champ reste géré séparément.
3. Sur une scène dont les bords ont des franges colorées, comparer Disable chromatic aberration ON/OFF. S’il n’y a pas de franges natives dans la scène, une faible différence est normale.
4. Tester les cases séparément et ensemble, avec Customize temporal AA ON puis OFF. Les filtres doivent rester indépendants ; vérifier un chargement et une cinématique. Un redémarrage permet de vérifier la mémorisation des cases.
5. Ouvrir l’arbre des cœurs dès le premier accès et noter si les niveaux manquent encore. Ce contrôle sert à reproduire le défaut ouvert ; aucune correction n’est promise par cette version.

Retour : décocher les nouvelles cases ; si nécessaire réinstaller les deux binaires V1.168 validés, en conservant l’INI. Main et les anciennes archives restent protégées.
