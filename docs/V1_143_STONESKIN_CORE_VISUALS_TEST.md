# V1.143 - Stoneskin CPU et visuels de cœurs

**CONSTRUIT / CONTRÔLÉ, EXPÉRIMENTAL en jeu, non installé.** Base installée V1.142. Le cache CPU seul du commit a391b5a a déjà passé 70/70 contrôles Windows ; ce résultat ne valide pas les ajouts suivants, leur rendu ou les FPS.

## Objectif et modifications

Réduire le coût CPU du suivi Stoneskin Strife sans changer ses matériaux, effets, durée ou geste validés. Le journal V1.142 mesurait environ 1 ms par passage dans stoneskin-observe ; cache borné des classes de statut avec identités vérifiées. [Analyse initiale](V1_143_STONESKIN_STATUS_CACHE_REVIEW.md).

Ajouter en bas d’Appearance, après Leviathan, deux groupes fermés par défaut : **Legion shieldbearer** et **Stinger crystal**. Un seul choix pour les deux vues, normale et TPS, sans lignes dupliquées dans la bibliothèque générale. Leviathan reste séparé.

Le nom français Porte-bouclier de la Légion correspond à la clé native ShieldDemon, confirmée dans la localisation française. Les trois statuts de ce cœur référencent P_CreatureCore_ShieldDemon ; la famille Legion ordinaire reste inchangée. Choix du bouclier : Native=0, Hidden=1 conservé, Fire aura=2 (P_flamebrand_aura). Une valeur Hidden enregistrée dans un ancien INI reste donc Hidden. Le sélecteur connaît désormais cet index explicite plutôt que de déplacer une valeur persistée.

Stinger : nouvelle famille ajoutée après les 44 anciennes, Native=0 par défaut, Hidden=1. Cible uniquement P_Stinger_CreatureCore_StoneSkin, référencé par ses trois statuts. Ne masque pas la peau de pierre/feu ni l’explosion/cendre de Stoneskin, ne change pas les dégâts du cœur. Une seule nouvelle clé StingerCoreAppearance ; toutes les anciennes gardent leur valeur et identifiant.

Chemin existant de création de particules conservé : aucun nouveau hook ou ABI moteur, ni changement des classes/statuts/coûts. Effets nouvellement créés seulement ; si l’aura n’est pas chargée, effet natif conservé avec indication dans le menu. Pas de chargement synchrone ajouté. Le rendu/placement de l’aura en TPS reste à valider.

## Pose et sujets différés

Utilisateur : pose parfois figée en visée, rétablie en tirant ; changement de munition reproduit, dash initialement suspecté ; Stoneskin ne déclenche pas ce défaut. Deux captures externes VM_READ uniquement ont été effectuées après signal prêt, puis reproduction/comparaison demandées. Les échantillons enregistrent des périodes sans AIM maintenu, sans tir et sans capacité active : ils ne démontrent pas une commande AIM bloquée, ni une cause dans le montage Stoneskin. La seconde capture a rejeté 87 lectures ; leurs causes détaillées n’étaient pas enregistrées et elles ne sont pas exploitées comme preuve du défaut. Aucun déblocage forcé, réinitialisation d’animation ou patch de touche appliqué ; bug ouvert. Dash aérien explicitement remis à plus tard par l’utilisateur.

## Critères et essais à faire

1. Stoneskin Strife, armure habituelle puis Abyssale : comparer la fluidité et les mesures CPU avec V1.142, vérifier apparition, maintien et restauration des matériaux. CI ne mesure ni FPS ni GPU.
2. Avec le cœur Porte-bouclier de la Légion : Native, Hidden puis Fire aura en TPS et vue normale. Vérifier le positionnement de l’aura lorsqu’elle est disponible et le retour Native au prochain déclenchement. Le bouclier et sa protection doivent rester actifs quand le visuel est caché/remplacé.
3. Avec Stinger : Hidden puis relancer Stoneskin, sans cristal mais avec peau/feu/explosion/cendres. Revenir à Native et relancer pour retrouver le cristal ; ses dégâts restent natifs.
4. Confirmer les deux groupes en bas d’Appearance après Leviathan, fermés initialement. Choix conservés après sauvegarde/rechargement ; pas de doublon dans la bibliothèque.

P2 visuel non validé. Caméras, tirs, manette/rebinds, roues, pouvoirs et timings validés, HUD, jauges, skins, holsters, sauts et updater préservés. INI personnel et fichiers du jeu non modifiés. Rollback : V1.142 et sauvegarde d’installation existante. ZIP prévu : exactement ASI et dxgi.dll à la racine, sans INI ni README ; toutes les anciennes archives et branches main/archive conservées.

## Livraison contrôlée

Le lot complet passe 70/70 contrôles Windows, sans avertissement détecté ; ZIP et catalogue debug contrôlés, Defender sans menace détectée. [Reçu, lien direct et limites](V1_143_CORE_DELIVERY_RECEIPT.md). V1.142 demeure installée.
