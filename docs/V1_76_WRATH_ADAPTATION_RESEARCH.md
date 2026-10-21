# Capacités — adaptation native, état du 21 octobre 2026

**RECHERCHE CONTRÔLÉE, aucune capacité étrangère lancée ni sélecteur livré.** Choix demandé conservé : Native ou l'une des six capacités pour chacun des trois emplacements de Strife et War. Les observations ne sont pas une preuve de compatibilité entre personnages.

Le GetWrathAbilityById Blueprint de chaque personnage est entièrement décodé et limite le lookup à sa famille. La fonction native à 0x673600 lit les FName de ressources+3E4/+3EC/+3F4 pour les actions 37/39/41 ; il ne faut pas modifier une touche fixe ni les sauvegardes.

## Point de lancement enfin identifié

La référence 0x10EEC6 à la chaîne de nom était un constructeur global de FName, pas le getter de capacité. En suivant son stockage à 0x380F4F0 et en vérifiant les références sur de vraies frontières d'instructions/.pdata, l'événement natif est identifié à **7AAC30**. Observé : RCX personnage, RDX destination du conteneur de classe, R8 FName de 64 bits. La fonction prépare 16 octets de paramètres (FName+classe nulle), trouve la fonction via AB7F90, appelle ProcessEvent vtable+200 puis écrit la classe dans la destination et retourne cette destination. Ne pas appeler comme un simple getter à deux arguments retournant directement UClass. Aucune interception ajoutée à ce point dans V1.76.

## World Ender : inconnue précédente levée

La lecture de StartBeam est désormais complète : **660 octets sérialisés et 1040 octets en mémoire**, limites exactes atteintes, script précédent de recherche conservé. L'ancien décodeur confondait MetaCast (0x13) avec une affectation à deux expressions, puis interprétait une référence d'import négative comme un opcode. Le nouveau décodage traite la référence de classe avant l'expression ; les deux sélecteurs de projectile, les classes et les sauts sont cohérents jusqu'à EndOfScript.

Dans les deux branches, le Blueprint appelle **Instigator.GetComponentByClass(MayhemWorldEnder_Blueprint_C)** puis SpawnBeam avec ce composant. C'est une dépendance explicite, pas seulement une supposition liée aux noms de montages. Il faut contrôler/créer correctement un composant équivalent et son lifecycle avant d'autoriser ce pouvoir sur un personnage qui ne le possède pas. Cette lecture ne démontre ni qu'un pointeur nul serait sans danger ni qu'une copie brute de composant fonctionnerait.

Les montages Strife et War référencent toujours deux squelettes distincts. Forcer seulement le FName, le UClass retourné ou l'acceptation du squelette ne résout pas les notifications, composants, origines et fins d'action. Prochaine approche : adaptateur de lancement et montages compatibles pour chaque pouvoir étranger, en préservant coûts, relâchements, annulations et les deux joueurs. Pas de hook expérimental qui déclenche une classe étrangère sans ces contrats.

Les fichiers binaires du jeu et exports privés ne sont pas publiés. Les preuves de rendu OFF et son périmètre séparé figurent dans [V1.76](V1_76_FOOT_PISTOL_OFF_TEST.md). V1.75 visée/HeavyLand restent en attente des essais groupés, comme demandé.
