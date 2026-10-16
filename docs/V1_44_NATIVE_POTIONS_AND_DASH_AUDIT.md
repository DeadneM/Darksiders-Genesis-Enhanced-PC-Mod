# V1.44 — potions natives TEST et capture du dash

16 octobre 2026. Base livrée V1.43 debug `5fecde05404727c48594b3b41ee6d8dc3d8f2cf5`, avec défauts utilisateur conservés. **Sources préparées / EXPÉRIMENTAL** : construction, livraison et résultat en jeu à vérifier. Travail sur debug ; la release publique V1.43, main/archive et les 107 anciennes archives sont conservés.

## Objectif et réussite

Ajouter le remplissage de la réserve de potions de soin, un multiplicateur de recharge et Unlimited, chacun assignable dans Hotkeys → F1-F12. Conserver toutes les anciennes valeurs et affectations, les plafonds natifs et les effets du soin. Réussite technique : tests Windows de politique/persistance/parité INI et archive AMD64 à quatre fichiers contrôlés. Réussite en jeu : réserve visible remplie, conversion du surplus de soin accélérée, Unlimited activé/désactivé sans modifier soin ni cooldown. Cette réussite en jeu reste à confirmer par l'utilisateur.

## Fonctionnement et preuves natives

L'exécutable contrôlé est le Shipping x64 de 62113280 octets, SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. Les quatre entrées natives utilisées sont vérifiées par signatures avant installation des deux hooks.

- Joueur local issu du callback déjà validé ; PlayerState via pawn+0x330, type MayhemPlayerState (slot0x38115E0), autorité3 et retour PlayerState+0x5C0 vers le même joueur. Les références reçues sont relues et typées ; les identités précédentes servent seulement aux comparaisons de génération/fraîcheur.
- Données natives PlayerState+0x580 : réserve de soin +0xC et surplus de soin +0x10. Une observation externe en lecture seule a trouvé quatre potions. Le maximum n'est pas déduit de ce nombre : `0x56FAF0` calcule le plafond à partir de la base et des améliorations acquises.
- Remplissage : ajout exact des charges manquantes par `SERVER_AddHealthPotions_Implementation` `0x580390`. Cette fonction garde autorité, clamp0/plafond et notification native `0x674BD0`. Requête limitée au joueur/génération observés, expirant après5s et appliquée sur le thread du jeu hors pause/cinématique. Aucun soin ni monnaie ajouté.
- Recharge : le soin natif `0x65D870` remet d'abord la vie, puis convertit le surplus en progression de potion. Son appel `0x65DA90` demande le seuil `0x56DCF0` : moyenne des vies maximales actives × pourcentage natif PlayerState+0x4AC. Le hook retourne seuilNatif/multiplicateur pour le seul joueur local. OFF et x1 retournent exactement la valeur native ; aucun champ de vie, capacité, cooldown ou pourcentage permanent n'est écrit. Le taux est borné1–100, défaut2 avec option désactivée.
- Unlimited : `ConsumeHealthPotions` `0x566ED0` reçoit normalement le coût1. Seuls les deux appels de consommation du soin `0x65BF1B` et `0x65D78D`, avec contexte local typé, deviennent coût0. Le retour entier et les étapes suivantes de soin/cooldown restent natifs. Un appoint via l'ajout natif maintient aussi le stock plein lorsque l'option est activée à zéro. OFF reprend le coût original ; aucun drapeau natif d'infini n'est laissé actif.

Le code contient aussi une ancienne réserve de potions de Courroux : son implémentation d'ajout `0x5805A0` aboutit ici à zéro, sans chemin de remplissage utilisable démontré. Elle n'est donc pas exposée comme fonctionnalité acquise. Le menu vise la potion de soin réellement utilisée, sans option factice supplémentaire.

## Menu, raccourcis et compatibilité

Gameplay → Combat/resource controls → Health potions : **Fill health potions**, **Potion recharge rate**, **Unlimited potions**, réserve et charge suivante. Recharge accélérée et Unlimited sont OFF par défaut. Trois identifiants d'action ajoutés en fin d'enum37/38/39 ; anciens identifiants0–36 et affectations F1–F12 conservés. Aucun raccourci existant n'est réaffecté : sélectionner librement ces actions dans Hotkeys.

Trois nouvelles clés uniquement : Features/PotionRecharge=0, Features/UnlimitedPotions=0 et Values/PotionRechargeMultiplier=2.000. Les 112 valeurs antérieures restent inchangées. Chargement d'un ancien INI : options nouvelles OFF ; valeurs explicites personnelles prioritaires. Génération, reset, runtime et INI livré suivent les mêmes valeurs. Les deux hooks sont propres aux potions ; entrées/souris/manette, visée, caméra, cheval, roues, jauges, munitions et warnings conservent leurs chemins existants.

## Action Recovery : nouvelle preuve, aucun correctif annoncé

Deux captures externes en **lecture seule** ont été armées avant les tests ; aucun code exécuté ni mémoire écrite dans le jeu. Première :9877 échantillons,1203 observations DASH,22 instances distinctes. Seconde, test confirmé avec **Strife** :9289 échantillons,975 observations DASH,17 instances. Les comptes d'instances ne sont pas un compte des pressions de touche. Les droits du processus sont QUERY_LIMITED_INFORMATION | VM_READ ; les captures sont arrêtées après les réponses utilisateur.

Le type natif MayhemPlayerDashAbility est démontré par son class slot0x3810F90 ; vtable observée0x27D31B8. Le Tick réel est `0x5B72B0`, qui appelle le Tick de base `0x68D050` puis sa mise à jour spécifique. Le délai global MoveInterruptDelaySec reste0 dans les deux captures : le modifier ne peut pas corriger ce cas. Dans la seconde capture, les instances de montage sont résolues depuis le véritable helper de taille0x38, le weak pointer natif et la table de l'AnimInstance ; les anciennes lectures helper+D8/+194 hors taille sont exclues.

Les actions ordinaires durent ici environ0,72s ; le mode de mouvement observé reste(6,5) durant l'action. Les deux champs d'instance de montage +0x148/+0x150 passent de0/1 à1/1, sans transition de fin de fondu observable avant la fin native. Une ancienne condition de « blend tail » ne constitue donc pas un correctif fondé pour ces dashs. Quelques actions sont interrompues plus tôt ou restent actives davantage ; les captures seules ne permettent pas d'attribuer ces cas à une entrée précise.

**Action Recovery reste indisponible, ancien essai retiré, coffres reportés.** La vélocité échantillonnée n'établit pas le déplacement physique de ce mouvement personnalisé ; elle ne sert pas à décider d'une fin anticipée. Prochaine étape : suivre le déplacement réel/les événements de fin propres au dash et leur libération native, puis intervenir seulement après le mouvement utile. Pas de permission MOVE forcée, de rotation, de patch de touche ou de durée arbitraire annoncé comme réparation.

## Test et rollback

1. Avec une réserve incomplète, remplir via bouton puis via un raccourci choisi ; vérifier le nombre visible et le plafond natif.
2. Unlimited OFF : consommer une potion sur un personnage blessé, constater coût et soin. ON : répéter après le cooldown normal ; stock plein. Repasser OFF et constater une nouvelle consommation.
3. Avec réserve incomplète et vie pleine, comparer les mêmes gains de soin à recharge OFF/x1 puis x2/x10. Le surplus doit faire progresser les potions plus vite ; le soin, la capacité et le cooldown ne changent pas.
4. Alt-Tab/menu : une touche maintenue ne doit pas lancer un remplissage à la fermeture du menu. Vérifier les fonctions acquises pied/cheval/ultime, roues et jauges.

Installation uniquement jeu fermé, sauvegarde des quatre fichiers et du journal, INI personnel conservé. Retour arrière : restaurer les quatre fichiers sauvegardés ou les binaires V1.43 ; les trois nouvelles clés sont ignorées par V1.43. Aucun lancement automatique du jeu. ZIP cumulatif : ASI, dxgi.dll, INI, README, directement à la racine.
