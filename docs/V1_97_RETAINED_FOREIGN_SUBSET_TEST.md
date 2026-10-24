# V1.97 — repli après les échecs V1.96

## Résultat utilisateur

V1.96 REJETÉE : Rampage sur Strife bloque le personnage et World Ender sur War ne fonctionne pas. Clone produit son effet ; son animation est incorrecte. Caltrops/Geyser : effets et dégâts confirmés précédemment, animations incorrectes. Le cristal de Stoneskin reste incorrect et n'apparaît que pendant ce pouvoir.

## Objectif et changements

Repartir des chemins retenus de V1.95, avec seulement Clone de V1.96. Supprimer le code des transferts rejetés plutôt que superposer d'autres exceptions. Leurs classes, composants, configuration de rayon, suites Rampage et notifications d'impact ne font plus partie de l'adaptateur. Les anciennes valeurs de transfert reviennent à Native ; World Ender de Strife et Rampage de War restent disponibles comme avant. Les identifiants des options gardent leur numéro.

Clone/Caltrops sur War et Geyser sur Strife restent partiellement fonctionnels. Leurs animations ne sont pas corrigées : les montages privés utilisent une pose idle pour déclencher les événements. Cette technique ne réalise pas le transfert des mouvements d'un squelette à l'autre ; elle ne suffit pas à Rampage. La reprise des animations et du cycle complet du pouvoir reste à faire avant réintroduction.

## Stoneskin : preuve et limites

L'adaptation du mod est absente depuis V1.96. Aucun correctif visuel supplémentaire ni masquage du cristal dans V1.97. Le dossier installé contient seulement le PAK d'origine, avec le même index SHA1 que la référence étudiée. Le journal V1.96 est conservé localement, sans diffusion de son contenu personnel.

Le Blueprint natif de Stoneskin vérifie notamment un cœur et applique `MayhemStatusEffect_Core_Stinger_*`. Les données d'origine associent `P_Stinger_CreatureCore_StoneSkin` au modèle `Dn_VFX_Rocks_Single_J` et au matériau `M_VFX_StoneSkin_Thorns`. D'autres particules natives utilisent aussi un matériau de cristaux. **C'est une piste, pas l'identification du cristal signalé.** Aucun composant de cet effet n'a pu être relevé en direct : le jeu était fermé. Le défaut reste NON CORRIGÉ et sa cause inconnue.

La prochaine comparaison devra identifier le composant réellement visible et distinguer Stoneskin avec/sans son cœur, puis le comportement du mod et du jeu d'origine, sur la même scène/configuration. Ne pas conclure que le cœur explique le défaut sans cette comparaison, ni supprimer l'effet en prétendant l'avoir réparé.

## Contrôle et essai du repli

Critère : les deux transferts rejetés ne peuvent plus être sélectionnés, résolus ou construits ; les valeurs d'un ancien INI reviennent à Native, sans changer les pouvoirs propres. Clone/Caltrops/Geyser conservent leurs effets sans nouvelle régression. Animations et cristal restent ouverts.

Essai court : après redémarrage, vérifier les choix des deux personnages et le retour Native des anciennes sélections ; tester Clone/Caltrops sur War et Geyser sur Strife. Le test P2 reste différé.

Installation manuelle, jeu fermé, en conservant INI, raccourcis, PAK et sauvegardes. Aucun lancement du jeu ni modification de sa mémoire. 168 clés, 55 ID Hotkeys, 161 anciennes archives conservées ; main/archive/v1.43 intacts. Retour arrière vers V1.95 possible ; V1.96 reste archivée mais rejetée en jeu.
