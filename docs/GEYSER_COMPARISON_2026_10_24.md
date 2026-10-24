# Geyser — comparaison réelle V1.93, 24 octobre 2026

**DIAGNOSTIC OBSERVÉ. Geyser sur Strife reste REJETÉ dans cet essai : « rester figé sans effet ». Aucun correctif ni nouvelle build annoncé.** Base validée V1.87 dans les cas rapportés ; cumul installé V1.93. P2 toujours reporté. [Mesures](evidence/V1_93_GEYSER_RUNTIME_COMPARISON.json).

## Ce que le journal démontre

Le journal annonce V1.93 au démarrage. À 18:14:51, Geyser de War (Native) entre réellement dans `SpawnBlast`, puis retourne normalement. À 18:14:54, le transfert vers Strife suit le même passage, environ 0,499 s après le début de son montage compatible. Pour chaque appel, quatre événements imbriqués concernent **un seul objet d’attaque observé**, dont le propriétaire correspond au personnage qui lance le pouvoir. Ce ne sont pas quatre attaques distinctes. La fonction est donc bien appelée ; l’hypothèse d’un notify jamais exécuté est écartée pour ces deux activations.

Ces traces ne mesurent ni dégâts, ni contacts sur ennemis, ni position de l’effet, ni consommation effective de Courroux. Le compte global d’objets n’est pas utilisé comme preuve de création ou de dégâts. Les emplacements sont revenus à Native dans l’INI lu après l’essai ; cela n’altère pas ce que le journal a enregistré.

Journal privé conservé avant une éventuelle prochaine session ; SHA256 `234d608e59326df65afdb7bcf22cae4cc2b7a754198bd56d902e032f3915a9bb`. Aucun lancement, installation ou changement de mémoire du jeu. Le jeu n’était plus ouvert au moment de la tentative de relevé des composants visuels ; aucune lecture live de leurs paramètres n’est donc revendiquée.

## Défaut concret trouvé, contrôlé deux fois

1. Lecture du code et du test : `ForeignMontage::Geyser` remplace la séquence War par `Strife_Idle`, recopie uniquement l’événement `SpawnBlast` puis impose un total d’un événement au montage privé. Le test existant exige lui aussi ce total : il vérifiait le contrat programmé, mais pas la conservation de la présentation native.
2. Nouvelle extraction en lecture seule des cinq assets natifs depuis le PAK installé, dix fichiers uasset/uexp avec SHA1 de payload vérifié : la séquence War contient des notifications de lames, sons et effets d’épée ; les quatre montages contiennent également leurs sons et effets nova/lames supplémentaires selon le niveau. Ces notifications sont omises dans le montage privé livré. Les classes de blast inspectées précédemment sont des Blueprints de données, pas un remplacement de ces événements visuels.

**L’absence de ces événements explique l’absence de cette présentation native dans le transfert.** La pose de repos explique aussi l’absence de geste de lancement pendant le cast ; elle n’établit pas un verrouillage persistant après le cast. Rien ne prouve ici que restaurer les effets résoudrait une absence de dégâts.

## Correction désormais ciblée

Conserver le déclenchement unique `SpawnBlast` et le lifecycle déjà observés ; transférer séparément la présentation compatible, avec ses temps natifs et ses variantes. Les notifications de la séquence utilisent notamment `Bone_War_Root`, `VFX_Root` et `VFX_Sword_Base` : elles ne doivent pas toutes être copiées aveuglément vers Strife. Les points d’attache et les effets propres à l’épée de War doivent être filtrés ou adaptés dans des objets privés, sans modifier les assets natifs/CDO ni ajouter un second blast ou une minuterie de dégâts.

Le prochain candidat devra tester réellement la conservation des effets et du son, leur position et l’unicité de l’attaque ; l’animation de cast Strife reste un sujet distinct. Dégâts et fin normale seront vérifiés en mission, puis retour Native et Geyser propre de War. Pas de nouvelle activation demandée pour reconfirmer le passage déjà démontré. Aucun changement Caltrops, Stoneskin, caméra, tir, entrées, INI ou raccourcis dans ce relevé.
