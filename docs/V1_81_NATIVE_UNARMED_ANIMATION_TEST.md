# V1.81 — bras OFF animés par la locomotion désarmée native

**V1.81 CONSTRUITE / CONTRÔLÉE : [48/48 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39803846206), zéro erreur et zéro avertissement MSVC. Bras animés EXPÉRIMENTAUX, non validés en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/36d31209abfbf4c0fea6b5afd64184855f1f964b/downloads/DarksidersGenesis_V1.81_NATIVE_UNARMED_ANIMATION_TEST.zip) · [Reçu et empreintes](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_81_DELIVERY_RECEIPT.md). Installation manuelle, garder son INI et ses raccourcis.

## Base, objectif, statut

Base cumulative V1.80 `3533b3d070a6c41b7976203c7d3e2e27501c9dd3`, debug exclusivement. Le cheval V1.80 est confirmé bon dans l'essai rapporté. Son code, ses sons, les fonctions de tir/visée et de manette restent inchangés. Le repos figé V1.80 est rejeté pour les bras ; il est remplacé, sans empilement d'un second correctif figé.

Objectif : avec un pistolet OFF, son bras suit le repos/la marche/la course désarmés pendant visée et tir, des deux côtés. L'autre bras conserve son animation. Mêlée, pouvoirs et autres actions utilisent leurs animations natives, puis le bras OFF reprend la locomotion désarmée. Réussite = confirmation visuelle de ces cas par l'utilisateur. **Nouveau rendu EXPÉRIMENTAL et non validé en jeu** ; lectures, fixtures et compilation ne valent pas validation visuelle.

## Preuves natives et relevés

EXE supporté : 62 113 280 octets, SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. Réflexion et assets du PAK relus. BlendSpace1D `Strife_Locomotion` : Run index 0, Idle index 1, Walk index 2, positions 1/0/⅓, durées 0,866667 / 2,666667 / 1,066667 s, RateScale=1, aucune interpolation par os. Les indices du BlendSpace armé sont différents et ne sont pas réutilisés.

Lecture VM_READ uniquement du jeu ouvert : capture du 22 octobre 17:08:32, **3 498 échantillons et deux lectures incohérentes rejetées**. Repos/déplacement, ranged-idle et melee-idle observés. Les états ne prouvent pas chaque pouvoir individuellement. Instance Strife normale, racine `+318`, vtable `2A77628`, évaluation `13C0F50` confirmées. Proxy à anim `+240`, retours anim `+A0`, Skeleton `+B0`, mesh `+B8` ; ces liens sont vérifiés pendant l'évaluation, car certains sont nettoyés en dehors. Mapping compact observé : 80 indices 0 à 79. Pas de master-pose ni d'override de référence dans le relevé.

Contrats désassemblés : Root Evaluate `13C0F50` appelle par saut FPoseLink Evaluate `168AFC0` ; contexte local FPoseContext 0x48, pose +8, courbes +20, additive +40. Init contexte `1694890`, extraction BlendSpace `16B4C30`, allocation des transforms/courbes par MemStack natif. Le lecteur externe ne les appelle jamais. Le futur ASI n'effectue l'extraction que dans le callback natif d'évaluation, après l'original, avec le proxy exact du joueur. [Résumé de preuve public](evidence/V1_81_ANIMATION_CONTRACT.json).

## Implémentation et limites

Un hook facultatif d'évaluation de racine, limité à l'instance Strife possédée, est ajouté avec signatures et EXE exacts. Tous les appels originaux sont conservés. Le BlendSpace désarmé est échantillonné dans un contexte privé : aucun Update d'action, notification ou montage n'est relancé. Un tampon réutilisé par worker contient les copies ; aucun pointeur MemStack n'est conservé. La phase utilise le temps du monde, gelé en pause ; la vitesse scalaire tient compte aussi du recul et du déplacement latéral. Les clips natifs fournissent le mouvement, sans rotation sinusoïdale ajoutée.

Les transformations locales privées passent par la hiérarchie vérifiée et les inverse-bind pour obtenir les matrices de peau. Seules clavicule et branche du bras OFF rejoignent le paquet de rendu avant soumission, sur son Spine2 courant. Les positions de tir/socket, poses de gameplay et autre bras ne sont pas écrits. Historique du paquet précédent conservé entre deux échantillons pour éviter de fabriquer une ancienne pose armée. Ancien Idle/capture immobile/cache de repos conservé après transformation supprimés. Aucun besoin de ranger et attendre pour enregistrer une pose.

Cas non supporté, identité périmée, autre action, contexte additif, mapping compact différent des 80 os observés ou échantillon vieux de plus de 100 ms : rendu natif des bras. Les modèles/holsters OFF conservent leur chemin validé, indépendamment de l'échantillonnage. Bras de monture et forme ultime restent natifs ; le rendu à pied se réinitialise à leur retour. **Transitions, phase de marche, retargeting d'autres skins et résultat en coop restent à tester**. Aucun gain de performances ou réduction binaire revendiqué.

## Essai groupé

1. À pied : seul pistolet droit visible, puis seul gauche, puis les deux OFF. Rester immobile, marcher/courir, viser puis tirer en avançant, reculant et de côté. Le bras OFF doit rester animé et détendu, sans perturber le bras équipé.
2. Pour chaque côté, faire mêlée puis un pouvoir (dont World Ender), et revenir à la visée/tir. Animations des actions conservées, puis reprise désarmée ; tirs et dégâts inchangés.
3. Roue, dash/saut, cheval puis descente, ultime puis retour : pas de pistolet OFF réapparu, aucun besoin de recapturer le repos. Vérifier brièvement saut/sons montés validés.
4. Remettre les modèles Native : animations et effets habituels. Coop/P2 lorsque l'utilisateur le souhaite, sans validation anticipée.

Installation **manuelle**, conserver l'INI personnel et les raccourcis. Aucune nouvelle clé ; 166 valeurs et 55 ID d'actions Hotkeys conservés. Rollback par l'ASI du ZIP V1.80 déjà conservé, même INI. Les 145 archives précédentes, main, archive et v1.43 restent protégées. Livraison à quatre fichiers racine, contrôle du ZIP et CI à consigner dans un reçu après build. Aucun fichier du jeu ou sauvegarde modifié, aucune relance par l'agent.
