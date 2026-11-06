# V1.158 — retour utilisateur du 6 novembre 2026

Base installée inchangée : source `5f1591a85630ea0af128b4bd5c25996072bd5e3a`, archive `39acc77193470acf40fc0f6e8fd253fbcdca24f0`. Aucun nouveau build ni changement de code.

## Validation et réserves

- Écharpe : **VALIDÉ par l'utilisateur**, « écharpe OK ».
- Menu : **VALIDÉ par l'utilisateur**, « menu OK ».
- Cheval : **retour favorable avec réserve**, « cheval a l'air OK ». Ne pas convertir ce retour en preuve de tous les skins, niveaux ou chargements.
- GPU : validation précédente conservée. Coop : reportée par l'utilisateur.
- Nouveau défaut : effet de lave au sol parfois absent pendant Stoneskin. Ne pas rouvrir le matériau de l'écharpe validé pour ce symptôme distinct.

## Piste native de l'aura au sol — diagnostic, pas correction

Les données natives déjà extraites identifient `MayhemStatusEffect_Core_FallenDog_Molten_1_Blueprint` : `AuraBlastClass = MayhemBlast_StoneSkin_Aura_Level1_Blueprint_C`, groupe `StoneSkinMolten`, attache `VFX_Aura`, particule `P_MoltenFallen_CreatureCore_Status_Lvl1`. Le script `SpawnAura` valide la classe, accepte `MayhemPlayerCharacter` puis appelle `SpawnBlastAttached` sur le mesh du personnage. La destruction de l'aura est distincte.

Cela correspond à la piste du cœur Molten Hound. Le cœur effectivement équipé lors du défaut et la cause de l'intermittence ne sont pas établis par ce retour. Le matériau pierre/feu et les trois particules de transition gérés par notre module ne constituent pas cette aura persistante. Aucun réattachement forcé ni réactivation à chaque image ajouté.

Preuves réutilisées, sans nouveau relevé : `audit-v131/v182-stoneskin-status-contracts.json` et `audit-v131/v223-stoneskin-status-script.txt` (hors dépôt). Lecture ponctuelle de l'INI le 6 novembre : `StoneskinFxAppearance=0`, `OtherStrifeFxAppearance=0`, `OtherWarFxAppearance=0` ; aucune modification de l'INI.

Prochaine investigation : comparer le statut Molten, son instance d'aura, son attache et la disponibilité des ressources lorsque l'effet manque. Préserver les matériaux et transitions acceptés. Un résultat statique ne prouve pas la présence visuelle de l'aura en jeu.

## Images comparatives des quatre chevaux

Images réelles inspectées ; aucune image générée. La première est une capture de Genesis, les deux suivantes des visuels promotionnels du contenu bonus.

- Ruin (War, à gauche) et Mayhem (Strife, à droite), capture diffusée par THQ Nordic via [RPG Site](https://www.rpgsite.net/news/8953-darksiders-genesis-introducing-war-trailer) : [image](https://assets.rpgsite.net/images/images/000/080/124/original/Darksiders-Genesis_20190827_04.jpg).
- Despair, bonus de Strife : [visuel Microsoft](https://store-images.s-microsoft.com/image/apps.55449.13726720340404085.e2c64139-3d5f-4c92-8532-43fa6d7f110e.6c0e5ae6-6010-40f8-8ced-cb5c31800ddc).
- Rampage, bonus de War : [visuel Microsoft](https://store-images.s-microsoft.com/image/apps.30359.13726720340404085.e2c64139-3d5f-4c92-8532-43fa6d7f110e.be66d61e-8697-42e7-a34a-9b28a65f91cc).

[Xbox confirme les attributions des deux bonus](https://news.xbox.com/en-us/2019/12/06/darksiders-genesis-is-now-available-for-digital-pre-order-and-pre-download-on-xbox-one/). Les flammes diffèrent selon l'apparence : ne pas prendre un bonus comme référence identique pour toutes les montures.
