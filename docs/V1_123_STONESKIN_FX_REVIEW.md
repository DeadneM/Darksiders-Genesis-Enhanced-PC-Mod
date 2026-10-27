# Effets Stoneskin de Strife - proposition precise et autorisation

Geyser V1.122 et le geste Stoneskin1.25s sont VALIDES par l'utilisateur et doivent rester inchanges. Caltrops War emploiera le geste complet de Stoneskin et son signal de coup de pied via le hook de particules existant et trois gardes de notifications du geste donneur. Ces trois gardes Caltrops sont distincts de la presente autorisation visuelle Stoneskin. Autorisation humaine explicite recue : « Autoriser cet essai visuel limite ». Elle porte sur l'API publique ci-dessous dans l'ASI, uniquement pour les explosions/cendres de Strife. La methode de constructeur/notification directe reste abandonnee. Le lecteur externe demeure en lecture seule.

## Methode abandonnee

Le controle automatique a refuse la creation d'un AnimNotify interne avec un constructeur a adresse fixe, modification de ses champs puis invocation native directe : risque de crash/corruption et absence d'accord explicite. Cette methode n'est pas appliquee, ni executee indirectement. Ses fichiers proposes n'ont pas ete crees. Il a aussi refuse la proposition d'API publique ci-dessous pour manque d'autorisation explicite d'appel runtime. Aucun de ces refus n'est contourne.

## Proposition soumise

Uniquement dans l'ASI, reutiliser le dispatcher ProcessEvent deja employe par les materiaux pour demander a l'API publique du moteur `GameplayStatics.SpawnEmitterAttached` de jouer trois effets connus :

- `P_WarStoneSkin_Base` ou `P_WarStoneSkin_Molten`, une seule explosion lors de l'apparition effective du revetement sur Strife.
- `P_War_StoneSkin_Base_Crumble_Expiration`, une seule fois a la disparition complete du statut, quand le revetement est restaure.

Aucun appel de fonction de particules par adresse fixe, constructeur interne d'AnimNotify, ecriture de champ interne d'objet, nouveau hook, modification de sauvegarde, cout/degat/duree de statut ou geste. Le lecteur externe du jeu reste strictement en lecture seule : il ne devra jamais declencher ces effets ou ecrire de la memoire.

Le relevé du jeu ouvert PID38944 confirme en lecture seule la classe native `GameplayStatics`, son CDO et la fonction native/statique `SpawnEmitterAttached`, flags0x4822401, parametres72 octets. Contrat a verifier de nouveau en jeu par reflexion avant chaque appel :

| Parametre | Type/taille | Position |
|---|---|---:|
| EmitterTemplate | Object8 | 0 |
| AttachToComponent | Object8 | 8 |
| AttachPointName | Name8 | 16 |
| Location | Vector12 | 24 |
| Rotation | Rotator12 | 36 |
| Scale | Vector12 | 48 |
| LocationType | Byte1 | 60 |
| bAutoDestroy | Bool1 | 61 |
| PoolingMethod | Enum1 | 62 |
| ReturnValue | Object8 | 64 |

Utiliser uniquement le mesh actuel reciprocally owned de Strife local, a pied, meme monde/joueur et identites faibles encore valides. Point d'attache None, position relative0 pour lancement, Z180 pour expiration comme le Blueprint natif, rotation0, echelle1, KeepRelativeOffset0, auto-destruction vraie, pooling0. Arguments dans un tampon local borne, types/tailles/positions et absence de chevauchement controles. Aucun appel si le contrat manque ou change. Pas d'effet de fin sur changement de monde/mesh/personnage, transformation, deconnexion, lecture inconnue ou shutdown. War Stoneskin/Stinger restent natifs hors du montage prive Caltrops de War, dont les notifications donneuses sont filtrees separement. Les effets conservent leur duree native et s'auto-detruisent.

## Preuves et limites

Le Blueprint natif de statut caste son destinataire en War avant les cendres ; cette condition exclut Strife. Son OnRemove utilise exactement l'API et le modele d'expiration ci-dessus. La profondeur du carnet et des tests ne vaut pas validation visuelle. Une mauvaise declaration runtime pourrait faire planter le jeu ; l'accord demande concerne cet essai visuel borne, avec sauvegarde/rollback et aucune perte de progression volontaire. L'ancienne methode de constructeur/notification directe reste abandonnee meme si cette API est autorisee.

Le relevé confirme Attached a0x80, bit0, et SocketName a0x84. L'ancien garde lisait le padding0x8C. Dans la session observee, ce padding etait zero : l'erreur est reelle mais ne prouve pas a elle seule la cause de tous les refus du lancement. Aucune affirmation de correction en jeu avant essai utilisateur.
