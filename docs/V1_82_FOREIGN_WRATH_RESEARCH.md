# V1.82 — contrats de transfert des pouvoirs, étude non livrée

Les six choix libres restent ouverts. Les réaffectations des trois pouvoirs natifs de chaque famille sont conservées telles quelles ; aucune option inerte n'est ajoutée. Cette étude n'est ni un adaptateur compilé ni une validation en jeu.

## Première piste : Stoneskin sur Strife

Le Blueprint appelle StopGliding, applique Steamroller puis Molten ou le statut Stoneskin de base **avant** son appel PlayMontage. Les appels de statut utilisent le Character/Instigator de l'aptitude. Le montage War ne peut pas être joué directement sur le squelette de Strife. Réutiliser un montage Clone de Strife transférerait aussi des notifications de création de clone ; ce n'est pas un substitut sûr.

Contrats natifs supplémentaires vérifiés dans l'EXE supporté :

| Élément | Contrat vérifié |
| --- | --- |
| Lookup complet `663220` | Résolution Blueprint `7AAC30`, puis déverrouillage de l'ID sélectionné `6AF360` et coût `7DD610`. Ne pas laisser l'ID original pour ces deux contrôles. |
| PlayMontage de MayhemPlayerMontageAbility | Exec `7BDF50` appelle `5AA190(ability, montage, rate, section)`. Celui-ci crée l'instance sur Character, l'enregistre à `+1D8` et installe la fin native via délégué. |
| Création d'un montage dynamique | Exec AnimInstance `1CC4B20` → wrapper `1679710` → factory `168A410(sequence, slot, blendIn, blendOut, rate, loops, blendOutTriggerTime, startTime)`. La factory seule ne lance pas l'animation ; le wrapper la lance ensuite séparément. |
| Source compatible chargée | Idle de Strife_Locomotion, durée 2,666667 s, même squelette que le personnage. Le slot TPS est lu par son FName dans le node Strife `+3720`, champ `+20` ; son index n'est pas une constante portable. |
| Source native Stoneskin | Montage de War chargé, durée 1,833333 s. Un adaptateur temporaire devrait conserver la durée de cast et les délégués de fin, sans partager ce montage incompatible. |
| ID étranger | Le CDO de War n'a pas de resource component utilisable. Le script de son GetWrathAbilityById contient les trois NameConst War_Wrath, numéros 2/3/4. Un FScriptName occupe **12 octets** (comparison, display, number), contrairement au FName vivant de 8 octets. |
| Fin de l'aptitude | Exec Finish `761DD0` appelle `59B080(ability, byte cause)`. Le contrat de cause et le traitement sûr d'un échec de montage restent à vérifier avant utilisation. |

Piste : créer un montage transitoire depuis une séquence propre au receveur et passer par PlayMontage natif de l'aptitude, avec coût et statut natifs. Cela évite de modifier les assets/CDO ou de prendre l'acteur/composant de l'autre joueur. **Non implémenté dans l'ASI V1.82** : la fin sur échec, les statuts et leurs variantes doivent être contrôlés avant d'exposer ce transfert. Le simple fait que la factory et la séquence existent ne prouve pas le pouvoir sur Strife.

## Suite requise

Complément de l'étude pendant la CI : sept classes de statuts Stoneskin/Molten/Steamroller extraites avec SHA1 vérifié. Les **huit fonctions du statut Stoneskin de base sont entièrement décodées**. Durée du statut de base 10 s, DamageModifier 0,6 ; aura attachée via MayhemPlayerCharacter et socket VFX_Aura. Le graphe contient aussi un cast explicite vers War_Blueprint pour remettre les matériaux de Chaoseater, VorpalBlade, Mesh et TremorGauntletMesh, ainsi qu'un effet de fin propre à War. Il faut vérifier la symétrie application/retrait sur Strife et les variantes héritées ; la compatibilité du montage ne règle donc pas tout le contrat visuel. Aucun de ces statuts n'a été appliqué par l'agent dans le jeu.

Stoneskin : vérifier les statuts de base/Molten/Steamroller sur le receveur, annulation et fin ; construire un premier adaptateur isolé avec garde de signature et retour natif en cas d'indisponibilité. Ensuite Caltrops/Geyser avec leurs notifications ; Clone/World Ender/Rampage ont en plus leurs composants ou phases propres. Les preuves et contraintes [V1.80](V1_80_FOREIGN_WRATH_CONTRACTS.md) restent applicables. Aucun lancement d'aptitude étrangère, chargement d'asset ou écriture de mémoire externe pendant cette étude.
