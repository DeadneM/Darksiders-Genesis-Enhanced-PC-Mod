# V1.93 — reçu du prototype Caltrops vers War

24 octobre 2026. **CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL, NON TESTÉ EN JEU.** Base validée V1.87 `c20319f93413f849a41b10fdfd96f7842a172ac6` pour les cas rapportés ; cumul V1.91 et code Caltrops V1.92 corrigé `dac4d9d5a0e68b493d639cce67c50e18ddc364ac`. L’accord pour avancer sans lancer le jeu n’est pas une validation de gameplay. Geyser sur Strife reste NON CORRIGÉ et son diagnostic V1.91 en attente ; P2 reporté.

- Sources compilées : `359683a77a4ebd5a64312ca45c9c9885bb9ca235` ; archive : `85f4d9a893ddfe85e6aef2799e339b2b7d567102`.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40058379301) : run 40058379301, job 120766401827, success, **54/54 suites**, zéro erreur et zéro avertissement MSVC, analyse native activée. Messages de dépendances/outillage consignés séparément.
- [ZIP public HTTP200 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/85f4d9a893ddfe85e6aef2799e339b2b7d567102/downloads/DarksidersGenesis_V1.93_WAR_CALTROPS_GROUND_TEST.zip) : **931353 octets**, SHA256 `3318C82C62287B61AE1AE79114CC92AC77EDE9E611278F18DC4F793174A4F491`. Contenu distant identique au local, CRC valides et deux binaires AMD64.

| Fichier racine | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1710592 | `B71E4699109FE18ECB7D7B153C562B66E9DF08A0D276EE4D10379D2CC3806998` |
| `DarksidersGenesisMod.ini` | 4691 | `99E3EECAD438A6735DD4389D544535FF798BB7F9E9EF0E4E243BC40CC4F13524` |
| `dxgi.dll` | 155136 | `074A5EDB61EF71DAC1D0CEFE229661F76B9402BE023AC528A4715F2426136290` |
| `README.md` | 54550 | `C803705ADD1C09CD88AF53B319952ED7BFC37FF10D9DF93470FF04947532A707` |

## Ce que contient le candidat

War peut sélectionner Caltrops dans chacun des trois emplacements existants, uniquement au sol. Le prototype utilise sa pose War_Idle compatible et son propre bassin Bone_War_Pelvis, puis demande au helper natif de Caltrops de créer son spawner au démarrage. Résolveur/coût et déblocage de l’emplacement restent natifs. Aucun nouveau hook/minuterie de gameplay, entrée physique, composant partagé, actor/ressource d’un autre joueur ou CDO modifié. La pose de lancer de Strife et ses variantes aériennes ne sont pas transférées. Apparition, dégâts, sons, cœurs, coût en mission, fin et annulation restent **non validés en jeu**.

Recherche des assets déjà chargés bornée à 512 objets par observation ; identités faibles, owner/controller/pawn réciproques et autorité, War=2 à pied en Walking, classe/package du projectile, animation/squelette/bassin exacts. Réentrance limitée à l’instance exacte et contexte précédent restauré. Identités recontrôlées après factory et play, propriétaire relu avant spawn ; pas d’appel supplémentaire sur un cast détruit ou réattribué. Contrat de cast indisponible : Finish normal du seul cast puis arrêt du transfert pour la session ; le premier coût peut déjà avoir été consommé. Remettre Native en cas d’échec.

Preuves hors jeu : Blueprint Caltrops entièrement décodé, assets extraits du PAK avec SHA1 contrôlé, War_Idle sans notifications 5,333333 s et montages Caltrops sol 2,333333 s, helper 5AE2B0 appelé par le thunk 7AF460 avec trois arguments (ability, subclass, FName huit octets), création liée au Character propre. Ces preuves ne certifient pas les mises à jour/dégâts du projectile sur War. [Contrat complet et limites](V1_93_WAR_CALTROPS_GROUND_TEST.md).

Suite ForeignCaltrops : préparation sans création, huit arguments du montage, trois arguments du spawner, durée, bassin/propriétaire, réentrance par instance et restauration, nonlocal/Strife/cheval/ultime, factory/play/sections invalides, état arrêté, destruction/réaffectation pendant les appels, cache borné et shutdown. WrathSlots : trois emplacements War, choix 3 persistant avec raccourcis personnalisés, Native et pouvoirs propres conservés. Pas d’essai joueur ni de validation coop déduite de ces tests.

## Archives et provenance

V1.92 initiale a compilé/passé 54 suites depuis 6161055 et publié un ZIP alors que son annulation arrivait. Elle précède la correction de scope dac4d9d, malgré le parent Git plus récent de son commit d’archive. Elle reste immuable, documentée comme intermédiaire et remplacée par V1.93. La publication CI refuse désormais une archive si les sources compilées ont changé sur debug ; le test local du garde a distingué un changement de sources d’un commit portant seulement l’archive. La garde s’est exécutée avec succès sur ce build final. Aucun écrasement de ZIP.

**157 anciennes archives intactes (158 ZIP au total)**, **168 clés INI et défauts**, **55 ID Hotkeys**. 116 anciens fichiers source inchangés depuis la base validée ; code des caméras, visée/tirs, XInput/roues, cheval, bras/holsters, lames, réticule et Léviathan conservé. Ancien cast Stoneskin, classificateur natif Strife et code/trace Geyser V1.91 inchangés. ConfigStore/RuntimeSettings et INI inchangés. Le module principal et ForeignMontage ne changent pas dans ce lot.

## Essais différés et suite

Installation **manuelle** en conservant INI/raccourcis/PAK/sauvegardes personnels. Aucun jeu lancé, aucune capture demandée, aucun fichier du jeu installé. Le README embarqué décrit la préparation ; ce reçu confirme compilation/ZIP sans réécrire l’archive.

Plus tard : War au sol en mission, tester effet/dégâts/coût, mouvements avant/arrière, fin/annulation, trois emplacements puis Native. En l’air, le prototype doit rester inactif sans coût. Courte régression Caltrops propre de Strife et pouvoirs propres de War. Si aucun effet, verrouillage ou autre défaut, revenir à Native et garder le journal sans multiplier les échecs. P2 lors d’une autre session seulement. Le diagnostic Geyser demeure séparé et non résolu ; aucun test demandé maintenant.

Rollback : Native dans le sélecteur, ou ASI V1.87 validé en gardant son INI. Suite : Clone/World Ender vers War exigent des composants de réception et leur nettoyage ; Rampage vers Strife exige de couvrir ses quatre fins Land/Slam. Ces trois transferts restent indisponibles, pas annoncés comme livrés. Les 40 fonctions Blueprint de référence ont été revérifiées sur leurs tailles exactes ; [inventaire des contrats restant ouverts](evidence/V1_92_OFFLINE_POWER_CONTRACTS.json). Réception lourde incertaine et autres FX P2 reportés, HUD général non reproduit.

main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, v1.43 `902861eb1ff34c14dc5ded092581b360767da817` vérifiés intacts ; fichier de reprise non suivi préservé.
