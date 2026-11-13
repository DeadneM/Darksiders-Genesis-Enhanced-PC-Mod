# V1.167 — reçu de livraison debug

**CONSTRUIT / CONTRÔLÉ / PUBLIÉ ; rendu EXPÉRIMENTAL en jeu.** Base debug V1.166 : synergie et sons des chevaux validés par l'utilisateur, conservés sans modification de leur code. Contre-esquive reste non testée. Main V1.164 reste inchangée. Source `df38da77fc8fe347c539567197e642584acba86b`, archive `700335bc5fa08bd4b579b512459dd738e9cfe77c`. Aucun build local, installation ou lancement du jeu par l'agent.

## Correction et nettoyage

Le log confirme que Gold était appliqué en V1.166, contrairement au repli opaque de V1.165. Le résultat est rejeté : plus de lave sur l'écharpe. Gold appartient à une autre famille de branches d'émission ; le transfert partiel ne démontrait pas une équivalence du feu/masque.

Nouvel essai : shader double face natif de la même famille **MM_Basic**, avec les branches de feu Molten déjà cuites. Le MID privé reçoit le **revêtement Stoneskin natif complet**, incluant textures et masques, en une seule copie à sa création. Sans Molten, ses puissances de feu sont explicitement coupées. Suppression du parent Gold et de sa traduction partielle ; 37 lignes nettes en moins dans le module. Aucun shader recompilé ni changement global de culling. Si la ressource ou le contrat n'est pas disponible, repli vers le revêtement classique, sans chargement ou copie en boucle.

Corps minéral, explosion/cendres, restauration des skins, synergie, sabots, mouvements, visée, GPU, Moloch et réglages conservés. Aucun autre bug natif annoncé corrigé sans reproduction actuelle. [Diagnostic, inventaire local, limites et deux revues](V1_167_REVIEW.md) · [Contrat et extraits du journal](evidence/V1_167_NATIVE_CLOTH_CONTRACT.json).

## Contrôles

GitHub Windows x64 : **84/84 suites réussies**, zéro erreur et aucun avertissement C++ détecté dans le journal final. [Exécution](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/43412107713). Régression testée : la source copiée doit être Molten complet, pas l'écharpe non illuminée ; variante sans feu, mauvais master, ABI, disponibilité tardive, aucune copie par observation, Native/Abyssal et restauration exacte. Les cas propres aux anciens alias Gold supprimés sont retirés ; les contrôles du padding et du corps sont conservés. Les dépréciations externes CMake/Actions restent consignées, sans masquage.

Cela ne prouve pas le rendu face/envers ou le feu en jeu. Le candidat est Masked et n'active pas Detail_Normal : il faut contrôler les bords et l'éclairage. Pas de gain FPS mesuré.

ZIP : [DarksidersGenesis_V1.167_NATIVE_MOLTEN_CLOTH_TEST.zip](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/700335bc5fa08bd4b579b512459dd738e9cfe77c/downloads/DarksidersGenesis_V1.167_NATIVE_MOLTEN_CLOTH_TEST.zip), 2,481,902 octets, SHA256 `C2521B0FB13032C891AB2C260D7D97D78231969F522B9FA8B041039EFDDFB93E`. CRC et PE AMD64 contrôlés ; ASI + dxgi.dll seuls à la racine, sans INI/README. **252 valeurs et 55 actions Hotkeys inchangées** ; INI personnel, PAK et sauvegardes non modifiés. 231 anciennes archives intactes, 232 au total. Main/archive/v1.43 protégés. Classement updater debug et lien public vérifiés. Archive nouvelle récupérée une fois par Git, pas d'ancien paquet retéléchargé.

Defender : aucune menace détectée dans le ZIP et ses deux binaires, protection inchangée. [CI](evidence/V1_167_CI_CHECK.json) · [Paquet](evidence/V1_167_PACKAGE_CHECK.json) · [Defender](evidence/V1_167_DEFENDER_CHECK.json).

## Essais et suite

1. Strife + Molten : Stoneskin, feu, envers et extrémités de l'écharpe, puis extinction/restauration.
2. Sans Molten : revêtement pierre sans feu.
3. Skins Native et Abyssal, puis changement de niveau : même rendu et restauration du skin exact.
4. Courte régression synergie/cheval ; Contre-esquive OFF à vérifier quand l'utilisateur le souhaite. Coop complète toujours réservée au passage final convenu.

Rollback : remettre les deux binaires V1.166 en gardant l'INI ; synergie et sons y sont validés, écharpe y est connue défectueuse. La V1.164 stable reste disponible sur main. Suite : résultat visuel de ce candidat, ressources intermittentes si reproduites, autres apparences utiles et nettoyage démontré. Aucun état ou paramètre du jeu n'a été modifié par un lecteur externe pendant cette étude.
