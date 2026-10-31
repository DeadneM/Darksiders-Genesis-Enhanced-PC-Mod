# Préparation de la réparation native — 31 octobre 2026

**SOURCE PRÉPARÉE ; contrôle Windows GitHub requis ; réparation non livrée.** Base cumulative d741677 / V1.136 rejetée pour la visée, autres fonctions validées conservées. Le code retiré vient uniquement de l'expérience V1.136 ; pas de retour global à une version antérieure.

## Objectif et critère de cette étape

Retirer le mécanisme de point synchrone dont les observations et la lecture du vrai chemin du pistolet ont invalidé le modèle. Réussite de cette préparation : compilation et suites Windows réussies, un seul hook dans NativeAimAssistFeature, aucun cache/point de cible conservé, mêmes résultats des fixtures de tir libre, relâchement Charge Shot, modes verticaux et solveur Lava. Cela ne constitue pas la correction du verrouillage Statique ni une validation en jeu.

Le hook `66A620`, sa signature, ses compteurs et les portées ShotScope sont supprimés. ProjectileAimFeature n'accepte plus de point assisté alternatif transmis par cette observation. Les fixtures qui forçaient artificiellement `59D3BD` dans les retours originaux sont retirées ; les contrôles d'arguments, d'angles temporaires/restaurés et de propriétaires du sélecteur restent.

Les corrections déjà validées du tir libre, du relâchement chargé, de la paire angle/vitesse Lava et de l'exclusion World Ender ne changent pas. Aucun nouveau résolveur, appel moteur, mutation de faisceau ou filtre de portée n'est ajouté. Aucun changement XInput, caméra, menus, poses, ressources, INI ou Hotkeys.

## Vérification sans livraison

Le workflow debug accepte le marqueur de commit `[check only]` : compilation et tous les contrôles s'exécutent normalement, les trois étapes de conditionnement/publication sont omises. Aucun ZIP ni binaire n'est publié sous l'ancien numéro. Les autres commits gardent leur livraison numérotée habituelle. L'identité de livraison reste V1.136 jusqu'à la prochaine véritable version ; l'installation existante n'est pas remplacée.

## Limite à résoudre avant de choisir une nouvelle correction

Le profil actif observé 7500 / 90 / 45 et les chemins natifs affectés ne suffisent pas à démontrer ce que ferait le jeu en vue du dessus dans la même scène. L'utilisateur n'avait testé que TPS souris lors du premier signalement. La comparaison native/TPS, réglage d'acquisition OFF, a été demandée pendant ce travail. Ne pas transformer son absence en confirmation que le sélecteur du jeu est défectueux, ni attribuer au seul pourcentage des essais où les cibles diffèrent.

Le défaut actuel de cohérence reste documenté dans [le relevé corrélé](V1_136_CORRELATED_NATIVE_PATH.md). Le nettoyage élimine une expérience sans mécanisme de remplacement démontré ; il ne promet aucune modification visible de Statique. Aucune répétition du test 200 % déjà réalisé n'est demandée.
