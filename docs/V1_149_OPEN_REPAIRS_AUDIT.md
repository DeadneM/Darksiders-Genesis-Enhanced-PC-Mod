# Réparations ouvertes après V1.149

## Pose de tir

L'utilisateur confirme le défaut occasionnel après changement de munition et préfère prévenir quand il revient naturellement. Les relevés V1.253/V1.255 existent ; ils n'établissent pas de commande AIM maintenue fautive. Réutiliser ces relevés, ne pas refaire une succession de tests génériques. L'animation privée du bras sans pistolet n'est active que pour un modèleOFF ; le fait que le défaut soit aussi signalé avec deux pistolets impose de garder la piste native du haut du corps ouverte. Aucun correctif appliqué dans V1.149.

## Stoneskin / disponibilité d'effets

Dans la capture VM_READ cinq minutes, des MID privés de War_Inst_StoneSkin_A et War_Inst_StoneSkin_Molten_B sont bien présents avec Silver puis Abyssal. Le statut SteamRoller3 hérite du statut de base ; ce cœur n'est donc pas une classe manquante dans le test d'ascendance actuel. Cela ne valide pas l'image à l'écran ni les autres situations : défaut intermittent toujours ouvert. Le dernier retour « parfois indisponible » nécessite de distinguer matériau absent, effet non chargé et capacité indisponible. Aucun nouveau chargeur de matériaux et aucun changement de statut/rendu appliqué à l'aveugle. [Observation résumée](evidence/V1_149_STONESKIN_OBSERVATION.json).

## Dash aérien : point d'entrée identifié

Règle utilisateur : stomp sans direction maintenue ; dash directionnel si déplacement maintenu à l'atterrissage. Ce sujet n'est plus reporté à sa demande.

Le script natif MayhemAbility_Strife_AirDash_Blueprint est entièrement décodé hors ligne : fonction185 octets mémoire,121 sérialisés, décodage complet. OnStart joue Strife_AirDash_Fall_Montage ; OnFinish, pour la causeLAND38, appelle CreateDashInfoForPlayer(false) puis lance MayhemAbility_Strife_Ground_Dash_Start_Level1_Blueprint. Ce chaînage n'a pas de test explicite du déplacement maintenu dans ce script. War utilise une capacité d'atterrissage distincte ; ne pas lui appliquer mécaniquement un correctif Strife.

Les tables natives de l'exécutable supporté confirment EAbilityInterrupt::LAND=38 et ECharacterActions::MOVE=29. Le MOVE des interruptions est un autre enum. Le futur garde doit vérifier l'intention de déplacement native lors du lien d'atterrissage, puis préserver le stomp et le chemin d'atterrissage si aucune direction n'est demandée. Ne pas simuler une touche, utiliser un ancien vecteur de visée ou modifier tous les dashs au sol. Vérifier aussi que les effets/dégâts et le nettoyage natifs sont conservés avant d'intercepter ce lien. Aucun code de dash aérien changé dans V1.149.

## Trait/arc de Moloch

Le défaut persiste selon l'utilisateur avec Hide Everywhere ; ce n'est pas seulement le choix du mode TPS. Deux objets de shaders correspondant à la liste admise sont capturés dans la session ; leur compteur ne prouve pas que les deux CRC distincts soient présents. V1.149 corrige l'invalidation manquante du cache après/before un replay graphique, mais l'hypothèse d'un autre effet/variant dans ce niveau reste ouverte. Aucun CRC supplémentaire deviné. Résultat en jeu à vérifier.
