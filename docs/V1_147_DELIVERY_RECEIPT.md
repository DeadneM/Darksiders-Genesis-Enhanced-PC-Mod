# V1.147 — livraison expérimentale du bouclier

**CONSTRUIT / CONTRÔLÉ, EXPÉRIMENTAL en jeu. Non installé par l'agent.** Base V1.146 ; son bouclier est repris après le retour non validé, Léviathan/Stinger et les fonctions acquises conservés. [ZIP direct immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/8149f5a99c5c8ecc795bc2975419a781c4a44bb9/downloads/DarksidersGenesis_V1.147_SHIELD_NATIVE_STATUS_PRESENTATION_TEST.zip). Source `28266fdfe2609e19497ea0c4c7d27e994084c317`, archive `8149f5a99c5c8ecc795bc2975419a781c4a44bb9`.

## Changements et preuve

- Taille minimale **0** pour bouclier et Léviathan ; zéro masque le seul composant visuel, taille positive le réaffiche. Profils Strife/War et position XYZ conservés.
- **Native (game behavior)** laisse le jeu contrôler son rendu. **Original effect (adjustable)** utilise l'effet d'origine dans le chemin réglable. Hidden reste1, les anciennes valeurs0..5 sont conservées. **28 entrées, dont 26 effets candidats** : quatre variantes précédentes, original réglable et 21 autres candidats de la bibliothèque. Les effets déjà rejetés Rampage/Chaos/Fire aura ne sont pas remis.
- Capture VM_READ autorisée : vraie référence du composant+0x78, propriétaire+0x68, template natif et attache identifiés. +0x58 est nul et le précédent code le testait à tort comme un pointeur de particule ; ceci empêchait toute liaison et tout réglage. L'ancien test reproduisait l'hypothèse erronée ; il est remplacé par le cas réel avec source nulle.
- Le statut et le composant natifs ne sont plus substitués dans le factory. Le mod présente son seul composant privé sur la véritable attache et masque l'original quand tout est prêt. Choix et paramètres appliqués au bouclier déjà actif, suivi du retrait/recharge natifs, Native restaure l'original. Aucun changement de protection, dégâts, recharge, matériau de personnage ou cœur accordé. Deux contextes bornés, cache de réflexion ; ancien suivi à 32 composants retiré.
- Chargement à la demande du **seul template choisi** s'il manque, une requête par choix/contexte, hors pause/cinématique et après stabilisation. Conversion et chargement par API moteur réfléchie, contrat exact vérifié ; classe/nom/package du résultat revalidés. Première utilisation pouvant provoquer une pause brève. Aucune précharge des26 variantes. En cas de refus : original conservé et échec explicite dans le menu/journal. Le rendu de chaque candidat reste à tester.
- Signature de l'API commune vérifiée avant l'interception mêlée/Lava ; ordre contrôlé par une suite dédiée. Le reste de l'initialisation, la visée, les contrôles, les capacités et la caméra restent inchangés. [Contrat, cause et essais détaillés](V1_147_SHIELD_NATIVE_STATUS_PRESENTATION_TEST.md).

## Crash analysé — toujours ouvert

Journaux et minidump récupérés localement, copie privée conservée. V1.146 confirmée ; **3 novembre 2026 à 14:01:59**, RenderThread1, lecture0x20, instruction native àRVA D5D0A1 avec pointeur nul. Sortie de cinématique 14:01:57.999, Level_17 ; utilisateur confirme reprise sans toucher aux réglages. Bouclier Native 0, Léviathan 3, flou personnalisé actif. Avertissement Bokeh et nettoyage de matériaux des cristaux du niveau juste avant, sans preuve de causalité. Aucun appel ASI dans la pile publiée, mais une influence indirecte du mod n'est pas exclue. **Cause non confirmée et aucun correctif du rendu annoncé dans V1.147.** Aucun journal supplémentaire à fournir. Même transition à comparer de façon contrôlée si elle se reproduit ; pas de modification du profil utilisateur ni de relance par l'agent.

## Contrôles

- [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41646034085) : **74/74 suites**, aucun avertissement de compilation C/CS/LNK/D détecté. Un premier essai avait échoué sur une ambiguïté C++ du helper de vecteur ; corrigée avant publication, aucun ZIP issu de cet essai.
- ZIP **2351342 octets**, ASI et dxgi.dll seulement à la racine, PE AMD64/CRC et absence de compression vérifiés ; **sans INI ni README**. SHA256 `CD019F7E03A68B62236ACFEAC98841C0E356BE6D9CF74210019F190070017E04`. Catalogue updater debug et téléchargement HTTP200 de mêmes octets vérifiés ; interface/installation en jeu non observées.
- 245 clés de référence, valeurs par défaut et 55 actions Hotkeys conservées, aucune nouvelle clé. **211 anciennes archives identiques**, 212 au total. Main/archive/tag v1.43 intacts. Analyse Defender : aucun élément détecté, protections actives et inchangées.
- Deux passes de vérification comparées : référence native observée contre ancien garde ; puis ABI, propriétaires, restauration, suppression des anciens mécanismes et ordre de démarrage. Les fixtures ne valent pas validation en jeu, ni mesure FPS/GPU.
- Aucun build local, lancement, installation ou écriture d'INI personnel/PAK/sauvegarde. Les deux relevés externes étaient strictement en lecture seule ; tous leurs handles ont été fermés.

## À tester et suite

1. **Original effect (adjustable)** : taille0 puis1, déplacement XYZ, Strife puis War. Native doit retrouver le rendu d'origine sans appliquer les réglages.
2. Belial, Dagon, Borrower, Water sphere puis quelques nouveaux candidats : vérifier chargement et effet visible, taille et position. Certains émetteurs spéciaux peuvent nécessiter une adaptation ; aucun candidat supplémentaire n'est déclaré validé.
3. Rechargement de partie/niveau, choix conservé ; épuisement puis recharge du bouclier ; Native et Hidden.
4. Léviathan : taille0 puis positive et position, puis régression Stinger/peau Stoneskin. Coop en dernier selon disponibilité.

Suite : trier et adapter les candidats selon retour de jeu ; traiter le crash de rendu séparément si reproductible. Pose après munition, dash aérien et Stoneskin intermittent restent ouverts/reportés. Repli V1.146 disponible ; l'INI est compatible, les nouveaux choix (6 à 27) sont ramenés àNative par cette ancienne version s'ils ne sont pas reconnus.
