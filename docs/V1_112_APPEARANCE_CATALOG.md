# Inventaire des apparences natives - releve V1.112

Etat : inventaire de fichiers et exports, pas une liste d'options deja ajoutees au menu. Source : PAK principal installe, lu sans modification ; son index actuel est identique au releve precedent (SHA1 `5d0982cd46a5221290cb6ed24696e3962e9dddff`). Le PAK mod de skins ne sert pas de source a cet inventaire.

Le PAK contient 86 641 entrees et 39 560 fichiers uasset. Le catalogue cible joueurs/chevaux/armes/outils/effets/coeurs : **1 502 references indexees, toutes les tables d'exports lues**, dont **269 assets analyses en detail**. Deux exports de cette analyse detaillee ne sont que partiellement decodes ; leur limite reste indiquee dans la preuve. Cela ne demontre pas que chaque texture est un skin selectionnable, ni l'absence absolue d'un asset hors de ce perimetre. Les recherches par familles peuvent inclure des decors ou des objets de ramassage : ils restent des candidats, pas des skins de joueur.

- [Index des fichiers, categorie, taille et SHA1](evidence/V1_112_APPEARANCE_ASSET_INDEX.csv)
- [Classes exportees, references et affectations visuelles controlees](evidence/V1_112_APPEARANCE_EXPORTS.json)
- [References et affectations des familles completes](evidence/V1_112_APPEARANCE_REFERENCES.json)

## Personnages, montures, armes et outils

Les noms suivants viennent des exports natifs, pas d'un nom invente pour un menu.

| Famille | Variantes confirmees dans les exports | Interpretation |
| --- | --- | --- |
| War | War_Mesh, War_Abyssal, War_Mesh_Gold, War_Silver | Base et trois modeles d'armure ; disponibilite distincte de la selection sauvegardee. |
| Strife | Strife, Strife_Abyssal, Strife_Gold, Strife_Silver | Base et trois modeles d'armure. |
| Formes/montures | Strife_Mayhem_Mesh, War_Ruin_Mesh, Strife_Despair_Mesh, War_Rampage_Mesh ; materiaux Ruin/Despair/Rampage | Certains sont des ensembles de transformation/monture, pas des skins interchangeables de personnage. Ne pas remplacer leur squelette par celui d'une armure. |
| Pistolets Strife | Strife_Gun_Mesh_R / Strife_Gun_Mesh_L ; R_Stowed / L_Stowed | Deux modeles tenus et leurs deux representations rangees, deja pris en charge. Stowed ne remplace pas un modele en main. |
| Epees Strife | Strife_Weapon_Dagger_R / Strife_Weapon_Dagger_L | Deux modeles de lame, masquage deja pris en charge. |
| Epee War | War_Weapon_Chaoseater + Chaoseater_Inst_A | Modele principal ; les autres couleurs/etats sont surtout materiaux/effets, pas une liste prouvee de nouvelles epees. |
| Bras War | War_Gauntlet / War_TremorGauntlet, War_TremorGauntlet_Mesh | Versions de base/augmentee deja prises en charge ; le dernier est un StaticMesh, pas un second bras anime equivalent. |
| Vorpal Blade | Gear_VorpalBlade + VorpalBlade_Inst_A | Modele d'outil confirme. Les etats elementaires demandent de separer rendu et comportement. |
| Caltrops | Strife_Caltrop_A + Strife_Caltrop_Inst | Un modele de base confirme ; plusieurs variantes de glyphes/explosion. |
| Ghost Hook / Void Bomb / Aether Spark | references d'outils et effets indexees | Pas encore tous reconcilies avec un composant reel. Un decor AetherSpark ou VoidBomb n'est pas une apparence de l'outil du joueur. |

Les materiaux de dissolution/summon, reflets dupliques et effets d'ombre ne sont pas comptes comme skins supplementaires. Un nom `TEST` ne prouve pas non plus un contenu abandonne utilisable. Les huit identifiants de skins natifs et leur acces sont deja documentes depuis V1.67, corriges ensuite ; cet inventaire ne modifie aucun droit, selection ou sauvegarde.

## Caltrops : premiere famille exploitable pour un choix visuel

La chaine native est explicite : MayhemAbility_SpawnCaltrops -> MayhemCaltropSpawnerProjectile_Base -> MayhemProjectile_Caltrop_Base -> MayhemBlastComponent_Caltrops_Base. Le projectile garde son StaticMesh Strife_Caltrop_A, un effet d'attente, un glyphe de depart et un glyphe en boucle.

| Choix propose pour une prochaine implementation | Assets visuels verifies | Champs a isoler |
| --- | --- | --- |
| Native | Aucune substitution | Respecte completement le coeur/equipement natif. |
| Base | P_Strife_Caltrop_Idle_01, P_Strife_Caltrop_Glyph_Init_1B, P_Strife_Caltrop_Glyph_Loop_1B, P_Strife_Caltrop_Trigger_1B | Template du composant, VFX_Glyph_Start/Loop, ExplosionParticleEffect. |
| Givre 1 | P_Strife_Caltrop_Glyph_Init_Frost_01 + P_Strife_Caltrop_Trigger_Frost_Lvl1 | Meme rendu uniquement ; ne pas remplacer le projectile ou le blast complet. |
| Givre 2 | Meme glyphe + P_Strife_Caltrop_Trigger_Frost_Lvl2 | Effet d'explosion uniquement. |
| Givre 3 | Meme glyphe + P_Strife_Caltrop_Trigger_Frost_Lvl3 | Effet d'explosion uniquement. |

Le givre vient ici de la famille interne **SteamRoller** (nom affiche du coeur a confirmer), niveaux 1/2/3. Les Blueprints correspondants changent AUSSI les rayons a 200/300/400 et le DamageEventRecord. Copier leur classe entiere changerait les statistiques : ce n'est pas un skin. Le projectile Frost met son glyphe en boucle a None. Le choix visuel devra le gerer sans toucher aux collisions, degats, statuts ou couts.

La famille **Stinger** possede trois blasts Caltrops distincts, mais leurs exports controles n'affectent pas une nouvelle explosion directement : ils ajoutent d'autres donnees/statuts avec un enregistrement NoDamage. Pas de choix "skin Stinger" annonce avant d'avoir suivi ces statuts jusqu'a leur composant visuel.

## Autres familles visuelles deja confirmees

- Stoneskin War : base / Molten / variantes Abyssal, avec materiaux propres et effets poussiere/aura. Stinger est un effet separe, pas une texture Stoneskin supplementaire. Le transfert de revetement Strife V1.112 reste experimental pour le rendu.
- Leviathan : les trois niveaux d'eclairs existent deja dans le chemin visuel retenu du mod.
- Strife clone : materiaux d'ombre de corps et d'armes ; ce sont des effets de Clone, pas des armures sauvegardees dans le catalogue de skins.
- Les effets elementaires Vorpal/Tremor et les variantes Ghost Hook/Void Bomb/Aether sont references dans l'index ; leur separation exacte apparence/statistiques reste a finir avant implementation.

Les autres affectations visuelles confirmees comprennent les effets Tremor `P_Gear_TG_Forward_LvL1_01`, `LvL2_01`, `LvL3_01`, `Forward_Wind` et `Impact_Forward_01`, ainsi que Ghost Hook `P_Gear_GhostHook_Beam_01` / `Attach_01` et son modele `Dn_GhostHook_A`. Des coeurs referencent aussi `P_CreatureCore_Astarte`, `P_CreatureCore_ScarabAcid`, les trois variantes TidehunterRanged et les trois WickedK. Ce sont des effets natifs reels ; les remplacer ne doit pas copier les classes de degats. Les correspondances niveau/nom parfois surprenantes sont conservees telles que trouvees, sans deduire une correction a partir du seul nom.

## Suite concrete

Apres validation du lot courant, commencer par **Caltrops Native/Base/Givre 1/Givre 2/Givre 3**, en remplaçant seulement les references de rendu sur les instances possedees. Conserver les degats/coeurs/couts/temps natifs et restaurer les propres changements. Ce menu n'est PAS livre dans V1.112. Les autres outils seront proposes lorsque la chaine composant/effet et sa restauration seront aussi prouvees.
