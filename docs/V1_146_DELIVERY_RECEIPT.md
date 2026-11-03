# V1.146 — livraison contrôlée

**CONSTRUIT / CONTRÔLÉ, EXPÉRIMENTAL en jeu, non installé par l'agent.** Base V1.145 ; Water sphere retenue et Stinger validé conservés. [ZIP direct immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/2e5ab444d145f31a78bf3fb556182729660f3ee8/downloads/DarksidersGenesis_V1.146_CORE_EFFECT_TRANSFORM_RELOAD_TEST.zip). Source `7ca33f0366c0a3d5d9a950bb8f78a113cc3e4b00`, archive `2e5ab444d145f31a78bf3fb556182729660f3ee8`.

## Changements

Appearance : dans Bouclier et Léviathan, choix du personnage Strife/War puis taille et position X/Y/hauteur. Les quatre profils sont indépendants du numéro de joueur. Taille ×0,05–5, offsets ±500 unités d'attachement, saisie manuelle et Default. 16 clés ajoutées avec taille1 et offsets0 ; les 229 valeurs antérieures et 55 actions Hotkeys sont conservées.

Le réglage porte sur le seul composant privé de l'effet, après vérification de son identité, propriétaire, attache et du contrat moteur. Mise à jour à la cadence existante de 100 ms, sans multiplication ou déplacement cumulatif ; l'application à la création n'est pas instantanée. L'ancien ajustement de taille dans le chemin mêlée/Lava est supprimé, ainsi que son horodatage devenu inutile. Les deux effets partagent le même adaptateur court. Pas de nouveau hook, aucun changement de matériau, statistique, dégâts, recharge ou protection.

Après une recherche complète, le catalogue reprend une passe bornée si le choix sélectionné manque, au minimum 5 s après la fin de la précédente. Ceci couvre la réutilisation d'un emplacement UObject sans augmentation du nombre d'objets. Le choix sauvegardé reste intact. Aucune ressource n'est chargée de force.

**Limite importante :** Belial/Dagon et parfois Water sphere ont été signalés « Effect assets not ready ». Si l'effet est réellement absent du niveau, le bouclier natif reste visible. La reprise permet de retrouver les assets chargés tardivement ; elle ne garantit pas leur présence. Le remplacement du bouclier sera appliqué à la prochaine activation native. Le retour à la sphère après chargement n'est pas annoncé complètement résolu avant essai en jeu. [Périmètre et essais détaillés](V1_146_CORE_EFFECT_TRANSFORM_RELOAD_TEST.md).

## Contrôles réalisés

- [GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41505383162) : **72/72 suites**, aucun avertissement C/CS/LNK/D détecté. Contrats natifs examinés hors ligne et vérifiés à nouveau par réflexion avant tout appel. Fixtures : absence de dérive, reset, changement natif de base, refus d'un ABI incorrect, identité expirée, chargement tardif à nombre d'objets constant, bornes et persistance des quatre profils.
- ZIP **2327278 octets**, exactement ASI et dxgi.dll à la racine, PE AMD64 et CRC vérifiés, sans compression, sans INI ni README. SHA256 : `E6957C55EC43AF9319BB7496D69C47CEECEA22B02AEECB74C4965C42C93F4EDC`.
- Catalogue updater debug : V1.146 sélectionnée, téléchargement HTTP200 de mêmes octets. Interface et installation de l'updater en jeu non observées pour cette version.
- **210 anciennes archives identiques**, 211 au total ; main/archive/tag v1.43 inchangés. Chemins visée, caméra, dash, pouvoirs, pistolets et Stoneskin identiques à la base. Defender : aucun élément détecté sur le ZIP et ses deux fichiers, protections actives et inchangées.
- Aucun build local, relevé de mémoire du jeu, lancement ou installation. INI personnel, raccourcis, sauvegardes et PAK non écrits par l'agent. La lecture du journal lors de la préparation confirmait V1.145 et le fichier personnel conservait Water sphere=5, Léviathan3 et Stinger Hidden1.

## Essai groupé et suite

1. Water sphere : taille et X/Y/hauteur pour Strife puis War, rester 10 s sans bouger ; pas de dérive. Vérifier Default.
2. Recharger et changer de niveau : choix et réglages conservés ; vérifier le message après environ 15 s puis la prochaine recharge/activation du bouclier. S'il reste indisponible, noter le niveau.
3. Vérifier la disparition à l'épuisement et le retour à la recharge du bouclier ; Native/Hidden et Stinger.
4. Léviathan : taille/position et reset, Strife/War puis pied/cheval/ultime ; désactiver l'override restaure le natif.

Le rendu, les proportions exactes des émetteurs et les performances de ce lot ne sont pas validés en jeu. Stoneskin intermittent, pose après changement de munition et dash aérien restent ouverts/reportés. Suite : intégrer le retour du lot puis approfondir le chargement des ressources si elles restent absentes. Repli V1.145 disponible, INI conservé (les nouvelles clés y sont simplement ignorées).
