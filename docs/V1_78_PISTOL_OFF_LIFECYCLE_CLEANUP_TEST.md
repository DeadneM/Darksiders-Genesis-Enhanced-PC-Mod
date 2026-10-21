# V1.78 — OFF persistant, interface et nettoyage

## Base, objectif, statut

Base cumulative V1.77 (`a0dee7cd6e2eeb45da33c55da867966dd278cb50`), debug uniquement. Visée manette sans tir, réglages, updater et seul pistolet droit visible à pied validés par l’utilisateur. Les nouveautés de ce lot sont **CONSTRUITES / CONTRÔLÉES : [47/47 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39669799213), zéro avertissement MSVC ; EXPÉRIMENTALES, non validées en jeu**.

Objectif : rendre le choix OFF cohérent avec le lifecycle des modèles, sans le faire dépendre de la capture d’une pose ; retirer les leftovers revérifiés et simplifier l’ouverture du menu. Critère : une main OFF reste masquée dans les roues, à cheval et au retour de l’ultime, Native rend les intentions de visibilité natives les plus récentes, et les tirs/commandes de jeu restent natifs.

## Changements préparés

- OFF du modèle est indépendant de la disponibilité de la pose. Une transformation peut invalider le cache de bras sans réafficher le pistolet. Capturer les bras rangés reste nécessaire pour leur présentation à pied.
- La fonction native de visibilité reçoit toujours sa demande et sa propagation d’origine. Pour un composant exact encore loué par OFF, la visibilité de ce seul composant est réaffirmée avant soumission du rendu ; aucun parent, roue, son, projectile ou composant étranger masqué. Les intentions natives sont enregistrées et restaurées au retour Native. Roues/pause n’annulent plus ce choix.
- À cheval, quatre `StaticMeshComponent` précis de Despair sont traités par côté : main `BA0/BA8`, rangés `BF8/C00`, propriétaire réciproque et parent `mesh+380`, classe exacte, rôle local et identité native contrôlés. Les poses montées, leurs effets de tir et les holsters en cuir du modèle monté restent natifs dans ce candidat ; seuls les quatre modèles de pistolets montés sont masqués.
- Bras droit OFF : sa clavicule de visée faisait encore partie de la pose native active. Le candidat repose aussi cet os dans le paquet de rendu, par rapport à `Bone_Strife_Spine2`. Le bras gauche garde exactement sa base précédente. Aucun os/sous-système de sockets natif écrit.
- Holsters à pied : hiérarchie de 80 os vérifiée en lecture seule, feuilles 78/79 `Bone_Strife_Holster_R/L`, parent pelvis 1. Réduire les triangles du côté OFF dans les matrices de rendu courantes et précédentes ; aucun masquage du personnage ou du parent. Résultat esthétique encore à tester.
- Trait/arc : nouveau choix **Never (always visible)**, ID 3, lecture/sauvegarde testées ; anciens ID 0/1/2 et valeur fournie TPS Only inchangés.
- Les quatre groupes auparavant ouverts par défaut deviennent repliés, comme les autres. Le mod désactive déjà l’INI ImGui : les ouvertures choisies dans une session restent utilisables, nouveau démarrage fermé par défaut.
- [Patch confronté des audits](AUDIT_DOUBLE_NETTOYAGE_V1_77.md) appliqué : 12 fichiers, 172 lignes retirées et cinq ajoutées, 167 lignes nettes en moins. Pas de gain de FPS ou de taille de l’ASI annoncé. Les changements fonctionnels du présent lot sont comptés séparément.

## World Ender : constat établi, réparation encore ouverte

L’utilisateur confirme animation et coût de Courroux, quel que soit le modèle de pistolet. Six choix de capacités sur Native dans l’INI relevé. Première capture : 2 932 échantillons, aucune erreur ; deux activations, chaque rayon créé pendant environ 2,8 s. Une lecture initiale limitée à la fin de l’action avait produit une conclusion erronée, corrigée après comparaison de tous les échantillons. Le rayon n’est pas absent du lifecycle.

L’utilisateur confirme les dégâts, son absence visuelle en TPS et sa présence en vue normale. Masquage trait/arc réglé Default Only pendant le test TPS : aucun effet sur le défaut, piste écartée. Deuxième capture : projectile World Ender Size1, template `P_Strife_worldender_scale1` et `MayhemBeamEffect` présents ; acteur non masqué et volume visible. La référence interne d’effet dans le composant d’arme n’est pas celle du projectile : ne pas confondre leurs états. Les objets d’effet devenus périmés sont rejetés par le lecteur externe, sans écriture ni relance.

Montage natif : notifications StartBeam et DestroyBeam présentes, probabilités 1 ; une notification de particule séparée possède nativement une probabilité 0, retrouvée identique dans le PAK et en mémoire. Ne pas l’activer arbitrairement. Inventaire privé SHA vérifié de trois effets, 1 096 exports décodés, matériaux et parent natif : pas de nouveau hook ni changement d’asset sur une hypothèse d’orientation. Prochaine étape : relier le wrapper d’effet à son composant de rendu pendant une activation, distinguer attache, orientation, taille, bornes et visibilité caméra. Aucun diagnostic ajouté périodiquement dans l’ASI.

## Ce qui reste intact

Visée sans tir validée, entrées natives et rebinds, XInput, RT/RB/R3, roues et commandes, projectiles et direction TPS, caméra, dash, ressources, sauts, skins, grappin, délais de rangement, updater et toutes les valeurs fournies. Le nettoyage retire des copies diagnostiques sans lecteur, pas leurs effets de bord. INI et 55 ID Hotkeys conservés ; anciennes archives immuables, main/archive/v1.43 préservés. Installation manuelle ultérieure.

## Essais groupés

1. Comparer seul pistolet droit visible (acquis), seul gauche visible (candidat), deux OFF puis Native. Ranger/rester immobile pour la pose ; viser et tirer ensuite. Vérifier chaque holster à pied et les skins utilisés.
2. Avec un pistolet OFF, ouvrir/fermer les trois roues, tirer, monter/descendre, passer en ultime puis revenir à pied sans attendre immobile. Le modèle OFF doit rester absent ; le cache de bras peut demander une nouvelle pose rangée. Contrôler séparément bras à cheval et holsters en cuir : encore natifs.
3. Vérifier Never sur le trait/arc en vue native et TPS, puis remettre sa préférence et sauvegarder/recharger. Vérifier les groupes repliés au démarrage et leur ouverture libre ensuite.
4. Contrôle court : visée sans tir, Standard/spéciaux/Hotstreak/dash, ressources, cheval/roues, menu et updater. La coop reste à tester quand l’utilisateur le souhaite, y compris personnages P1/P2 inversés.

World Ender TPS, échange libre des six pouvoirs, réception lourde après chute encore incertaine, suivi vertical des exécutions à faire plus tard. HUD général en surveillance ; autres FX P2 expressément reportés. Rollback : Native par main, ou reprendre V1.77 en conservant son INI personnel. Aucun fichier du jeu remplacé automatiquement.
