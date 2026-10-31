# Préparation de la réparation native — 31 octobre 2026

**CONSTRUIT / CONTRÔLÉ : 70/70 contrôles Windows x64, zéro avertissement/erreur MSVC ; réparation native TPS non livrée.** Base cumulative d741677 / V1.136 rejetée pour la visée, autres fonctions validées conservées. Le code retiré vient uniquement de l'expérience V1.136 ; pas de retour global à une version antérieure.

## Objectif et critère de cette étape

Retirer le mécanisme de point synchrone dont les observations et la lecture du vrai chemin du pistolet ont invalidé le modèle. Réussite de cette préparation : compilation et suites Windows réussies, un seul hook dans NativeAimAssistFeature, aucun cache/point de cible conservé, mêmes résultats des fixtures de tir libre, relâchement Charge Shot, modes verticaux et solveur Lava. Cela ne constitue pas la correction du verrouillage Statique ni une validation en jeu.

Le hook `66A620`, sa signature, ses compteurs et les portées ShotScope sont supprimés. ProjectileAimFeature n'accepte plus de point assisté alternatif transmis par cette observation. Les fixtures qui forçaient artificiellement `59D3BD` dans les retours originaux sont retirées ; les contrôles d'arguments, d'angles temporaires/restaurés et de propriétaires du sélecteur restent.

Les corrections déjà validées du tir libre, du relâchement chargé, de la paire angle/vitesse Lava et de l'exclusion World Ender ne changent pas. Aucun nouveau résolveur, appel moteur, mutation de faisceau ou filtre de portée n'est ajouté. Aucun changement XInput, caméra, menus, poses, ressources, INI ou Hotkeys.

## Vérification sans livraison

Le workflow debug accepte le marqueur de commit `[check only]` : compilation et tous les contrôles s'exécutent normalement, les trois étapes de conditionnement/publication sont omises. Aucun ZIP ni binaire n'est publié sous l'ancien numéro. Les autres commits gardent leur livraison numérotée habituelle. L'identité de livraison reste V1.136 jusqu'à la prochaine véritable version ; l'installation existante n'est pas remplacée.

## Limite à résoudre avant de choisir une nouvelle correction

L'utilisateur confirme maintenant que le verrouillage excessif se produit **seulement en TPS**, ajustement OFF. Cette comparaison oriente la réparation vers l'adaptation TPS, sans établir encore quel point/direction/cible diffère entre les deux chemins. Le profil actif observé 7500 / 90 / 45 ne prouve pas à lui seul un défaut du sélecteur original. Ne pas attribuer au seul pourcentage des essais où les cibles diffèrent.

Un lecteur comparatif est préparé : réglage ThirdPerson, rotation native enregistrée, méthodes des capacités actives, regard natif, acteur sélectionné et cible/point/direction du vrai faisceau. Il reste externe, en lecture seule, sans appel moteur ni écriture. À la tentative de reprise le jeu était fermé : aucune capture n'a été lancée et aucun jeu ouvert par l'agent. Attente du nouveau signal prêt avant la comparaison des tirs dans les deux vues.

Le défaut actuel de cohérence reste documenté dans [le relevé corrélé](V1_136_CORRELATED_NATIVE_PATH.md). Le nettoyage élimine une expérience sans mécanisme de remplacement démontré ; il ne promet aucune modification visible de Statique. Aucune répétition du test 200 % déjà réalisé n'est demandée.

## Résultat du contrôle

Commit source `588af564207674421469f73bc723825e8e2334c4`. [GitHub Actions réussi](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41236714587) : compilation Release x64 avec analyse MSVC et 70/70 suites réussies. Les trois étapes package/artifact/publication sont bien omises ; aucune archive ajoutée et aucun binaire téléchargé/installé. Les avertissements d'outils externes MinHook/CMake et Actions/Node sont distincts des zéro avertissement MSVC. [Reçu CI](evidence/NATIVE_AIM_CLEANUP_CI_2026_10_31.json).

La première passe vérifie la suppression du hook et du transport du point supposé, ainsi que l'absence de cache de rechange. La seconde confronte le diff, 150 autres fichiers source inchangés en contenu Git, les corps conservés du calcul de direction/solveur Lava/faisceau libre et les tests Windows réels. Les résultats concordent sur le nettoyage ; aucun ne prouve une correction du symptôme Statique. [Périmètre contrôlé](evidence/NATIVE_AIM_CLEANUP_2026_10_31.json).
