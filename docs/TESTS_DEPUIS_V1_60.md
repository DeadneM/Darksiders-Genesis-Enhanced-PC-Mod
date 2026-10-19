# Actualisation V1.64 en préparation

Grappin V1.63 désormais confirmé fonctionnel dans l'essai utilisateur. Prochaine vérification ciblée : trois délais de rangement et leurs Never séparés, raccourcis communs conservés ; durée ultime 0–10, OFF/0/0.5/1/2/10 et priorité Unlimited. [Manipulations et statut](V1_64_WEAPON_TIMERS_ULTIMATE_TEST.md). Double saut et auto-check toujours ouverts ; les passages historiques qui attendent le premier retour grappin sont supersédés.

---

# État après le retour utilisateur du 19 octobre 2026

**Plus à traiter comme non validés :** raccourcis de rangement, netteté TPS, Standard limité, disparition du son et des effets parasites pendant le dash : confirmés fonctionnels par l'utilisateur. La liste d'essais précédente est conservée ci-dessous comme historique, sans lui redemander ces validations.

**Double saut en échec :** Always on permet trois sauts supplémentaires puis s'arrête ; les autres modes ne donnent pas le résultat attendu. Ne pas présenter Always on comme infini. Reprise de l'analyse après le retour V1.63.

**En attente :** grappin V1.63 et auto-check. Aucun résultat de V1.63 n'est encore déduit du retour préparé avant le dernier message. Nouvelle demande à la file : Ultimate decrease rate 0–10, sens durée et ×2 par défaut. [Retour exact](USER_FEEDBACK_2026_10_19.md).

---

# Essais cumulatifs depuis V1.60

Cette liste n'implique aucune validation en jeu. V1.63 remplace le chemin grappin défectueux de V1.62 ; ne pas reproduire volontairement le crash sur V1.62.

1. **Grappin — V1.63 prioritaire :** OFF/×1, viser puis lancer, annuler et réutiliser avec War puis Strife ; ensuite ON ×2 sur une cible plus lointaine et une traction/relais. Coop P1/P2 et obstacles. La traversée/tyrolienne reste à démontrer.
2. **Rangement — raccourcis V1.60 :** assigner/effacer/recharger le clavier ; bouton manette 1 seul, 2 seul, puis combinaison. Maintien = un seul déclenchement. Coop : chaque manette agit sur son personnage. Même F-touche dans Hotkeys et la ligne dédiée : pas de double déclenchement. Menu/Alt-Tab/reconnexion : pas de rangement différé. Le rangement natif était déjà validé ; contrôle court du délai/jamais automatique.
3. **Flou — focus V1.61 :** chapitre 8, angles auparavant flous puis nets, personnage immobile, TPS à pied/cheval/ultime ; solo et P1/P2. Comparer TPS lens focus ON/OFF, vérifier personnage net, profondeur visible et fluidité. Vue native/cinématique : effet natif.
4. **Standard limité — V1.62 :** activer, capacité 10, Reduce Ammo et Unlimited OFF ; vider et vérifier le refus à zéro. Fill Ammo, réduction ÷2 et priorité Unlimited ; diminuer la capacité borne la réserve, l'augmenter ne remplit pas. OFF rend l'infini natif. Deux slots/P1/P2, changement de personnage et chargement. Hotstreak conserve ses règles natives de coût.
5. **Double saut — V1.62 :** Off natif ; Always = au moins quatre sauts successifs ; After gliding = pas de troisième saut avant un plané, un seul nouveau double saut après chaque nouveau plané. Atterrissage/mort/changement de personnage remettent le droit à zéro. Caméra native/TPS et P1/P2 ; cheval/ultime exclus du correctif.
6. **Tir pendant dash — encore ouvert :** déplacement + visée + tir + dash, munition normale puis spéciale/Hotstreak, TPS puis vue native. Relever séparément projectile Standard parasite, son, flash et animation. La reprise de visée après dash sans tirer était validée ; simple contrôle de régression.
7. **Mise à jour — V1.63 :** Auto check update OFF = pas de check au démarrage ; ON = un seul check du canal choisi. Version strictement plus récente : notification 8 secondes visibles puis bandeau jaune au-dessus des onglets. Menu fermé/Alt-Tab : aucune capture des commandes, notice encore visible au retour. Même version, version antérieure ou réseau indisponible : pas de faux bandeau. Changer de canal efface le résultat. L'installation reste manuelle.
8. **Contrôles courts des acquis :** tirs Lava/Charge/Static/Beam/Graviton à l'arrêt et en reculant ; roues et équipements P1/P2, sticks échangés, TPS à cheval/ultime et cinématiques. Ne pas requalifier ces fonctions en panne sans nouveau résultat.

Non livrés : correction des holsters montés, autres sprites P2 à identifier, modèles 3D alternatifs compatibles, nouvelle Action Recovery. Zoom In/Clear et les prompts d'interaction ne sont pas à refaire comme des nouveautés : déjà validés.
