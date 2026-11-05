# Effets natifs et écharpe : suivi après V1.155

L'utilisateur précise que les effets manquants sont ceux du cheval d'origine : flammes/particules à son apparition dans le premier niveau. Un rappel du cheval en rétablit une partie. Il signale aussi des parties d'écharpe transparentes pendant Stoneskin sur Strife. Il ne s'agit pas d'un échec de choix de la palette d'apparences ajoutée par le mod. Une interférence du mod avec le rendu ou le chargement natif reste à examiner.

Les options MountFxAppearance et OtherMountFxAppearance sont Native dans le dernier INI lu, avec une qualité d'effets enregistrée à 3. Aucun réglage de qualité particulaire imposé par EngineIniFeature n'a été trouvé dans le périmètre examiné. Cela ne suffit pas à attribuer le défaut au jeu ou à innocenter les hooks du mod.

Les huit poussières de dash ne remplacent aucun template de cheval. Elles sont maintenant validées par le retour utilisateur sur Strife à pied et la capture des composants. Le code de substitution et le filtre de shaders ne changent pas entre V1.154 et V1.155. Ne pas élargir ou désactiver le filtre de shaders sans comparaison contrôlée.

## Piste précise pour l'écharpe

StoneSkinVisual utilise War_Inst_StoneSkin_B et War_Inst_StoneSkin_Molten_B pour le tissu. Les exports natifs ont pour parent MM_Basic, avec BlendMode=BLEND_Masked, OpacityMaskClipValue=0.3333 et Base_Color=War_StoneSkin_D_A. Le code ne remplace pas cette texture par celle de l'écharpe de Strife. La correspondance du masque avec les UV de Strife est donc une piste concrète pour les découpes observées, pas une cause confirmée par l'image ou un correctif testé. [Paramètres exportés](evidence/V1_155_CLOTH_MATERIAL_FINDINGS.json).

Le changement reste limité à des MID propres au personnage, avec restitution des matériaux natifs capturés ; les sources partagées ne sont pas modifiées. Toute correction doit préserver cette limite et le feu déjà accepté, sans copier l'ensemble des paramètres de War sur Strife ni modifier un matériau partagé.

## Prochaine observation utile

Comparer les templates, leur activité/visibilité, parent d'attache et références natives lors de la première apparition du cheval puis d'un rappel dans le même niveau. Comparer ensuite les matériaux natifs de l'écharpe avant Stoneskin, pendant le pouvoir et après expiration, avec l'armure concernée. Le lecteur externe doit rester en lecture seule. Aucun correctif de ces deux symptômes n'est inclus ou annoncé validé dans V1.155.
