# V1.136 — chemin natif confirmé après les essais groupés

31 octobre 2026. **OBSERVÉ EN LECTURE SEULE / REVUE SOURCE ; NON CORRIGÉ.** L'utilisateur demande de rétablir d'abord le comportement natif avant de poursuivre le réglage optionnel. V1.136 reste rejetée pour la visée ; aucun nouveau build, hook, fichier de jeu ou INI remplacé. La base des autres fonctions validées reste protégée.

## Résultat des relevés

Le premier fichier contient 862 échantillons sur 87,3 secondes. Il s'arrête lors d'un verrouillage temporaire de l'INI par le jeu : ses échantillons précédents restent exploitables, mais il ne couvre pas les trois minutes prévues. Le lecteur a ensuite été corrigé pour signaler une lecture INI indisponible sans interrompre la lecture mémoire.

- Case ON / Global 0 : 34 échantillons de faisceau Statique sans cible de composant affectée.
- Case ON / Global 100 : 35 échantillons de faisceau, tous dirigés vers l'ennemi également sélectionné dans le personnage. Direction alignée sur le vecteur volume→cible à moins de 0,025°. Écart de 15,03 à 22,75° avec le vecteur natif de regard brut transformé en coordonnées monde. **Ce dernier écart n'est pas une mesure de l'angle au réticule.**
- Le journal relève ensuite Global 200, `scaled=1`, pour le helper commun, Statique et Standard. Les valeurs atteignent donc le sélecteur dans cette session. Cela ne valide ni leur effet perceptible ni une correction du défaut signalé.

Le relevé complémentaire contient 1 185 échantillons sur 120 secondes, sans erreur de lecture mémoire, mais aucun faisceau : il s'est terminé avant les tirs suivants. Ces derniers sont présents dans le journal à 20:40:36–38 ; aucune trajectoire à 200 % n'est donc attribuée à ce fichier. Ne pas demander une répétition en prétendant que l'essai utilisateur n'a pas été réalisé.

Ces essais comportent des positions/cibles différentes. L'absence de cible à 0 et sa présence à 100 sont des observations corrélées aux réglages, pas une comparaison contrôlée isolant leur seule influence. La longueur 4 417,6 observée avec une cible affectée et un maximum sérialisé de 1 200 correspond au chemin natif qui calcule la distance à sa cible ; ce n'est pas une preuve de changement de portée par le mod.

## Deux vérifications concordantes, sans nouvel appel moteur

**Lecture des vrais pointeurs du jeu.** L'interface `MayhemRangedWeaponUser` du Strife observé est à l'offset `B08`. Sa méthode `+20` est `66A100`. Cette fonction passe par la méthode virtuelle `+6A8` du personnage, effectivement `56CAC0` dans ce jeu. La lecture externe vérifie propriétaire et identité ; elle n'appelle aucune de ces fonctions.

**Vérification indépendante du code de l'EXE et des sorties du mod.** `56CAC0` rend le rotateur déjà enregistré en `95C`. Le chemin `GetProjectileSpawnRotation` (`6A9F20` → interface `+20` → `66A100` → `56CAC0`) ne traverse pas le résolveur `59D3BD` attendu par l'observateur V1.136. Un test simulant expressément ce résolveur à cet endroit ne représente donc pas ce chemin réel. Le compteur nul est cohérent avec cette erreur de modèle.

Dans `ProjectileAimFeature::Rotation`, l'absence de point observé entraîne ensuite le recalcul depuis la caméra. `Finish` et Lava possèdent aussi leurs propres sorties finales ; elles ne doivent pas être déclarées identiques au chemin du pistolet sans preuve. À l'inverse, le Tick natif de Statique utilise directement sa cible affectée en `620` et contourne la branche libre corrigée par `BeamForward`. La correspondance cible sélectionnée / cible réelle du faisceau est désormais observée, et plus seulement supposée depuis le code.

## Conséquence pour la réparation

La panne ne se réduit pas à un curseur déconnecté. Le sélecteur, le regard natif enregistré et les dernières corrections TPS ne forment pas actuellement une référence commune. Dire « OFF/100 = natif » sur la seule conservation des angles était insuffisant.

Priorité retenue : rétablir la cohérence de cette intégration native, puis vérifier le réglage optionnel autour de sa référence. Ne pas ajouter un second filtre de contact Statique, raccourcir sa portée, supprimer ses cibles/chaînes ou réintroduire le cache de candidats V1.135. L'observateur V1.136 ne doit pas être élargi arbitrairement à d'autres appelants pour fabriquer un point assisté.

Le critère de réussite est un tir TPS cohérent avec la visée et les collisions proches à la valeur de référence, sans changement de portée/chaînes ni régression Charge Shot, Lava, faisceaux et World Ender. Le réglage optionnel ne passe ensuite que s'il modifie réellement l'acquisition sans réécrire un résultat dans un autre chemin. Aucun retour global d'une ancienne version ne doit écraser les autres fonctions validées.

La cause historique exacte du premier signalement sur V1.126 reste ouverte : aucune comparaison TPS avec le jeu non modifié n'a été effectuée. Cette incohérence actuelle explique pourquoi les correctifs récents ne constituent pas une restauration démontrée ; elle ne prouve pas à elle seule l'origine initiale du défaut.

[Relevés, empreintes, réglages et méthodes vérifiées](evidence/V1_136_CORRELATED_NATIVE_PATH.json). [Rejet et limites de la première session](V1_136_REJECTION_AND_NEXT_EVIDENCE.md).
