# V1.185 — résolution et reflets en direct, lames en mêlée montée

Candidat cumulatif sur la livraison debug V1.184. La dernière version stable reste V1.173. Le chantier HDR attend le retour du joueur sur V1.184. Aucun nouveau ZIP pendant cet essai.

## Qualité de rendu

- `r.ScreenPercentage` est un flottant enregistré à `0x124000`, relu et converti en fraction par `0x1908270`. L’appel `0x18801BE` transmet la fraction au pilote de résolution de la vue. L’activation d’un autre pilote de résolution par le jeu peut prendre la priorité.
- `r.SSR.Quality` est un entier enregistré à `0x194300`. Le rendu relit sa copie à `0x103C1B1` pour la qualité et à `0x104A290` pour l’activation des reflets.
- Les deux contrôles utilisent les setters natifs de l’exécutable exact, avec la priorité actuelle de chaque variable, au prochain suivi du joueur sur le thread du jeu. L’option **Native** restaure uniquement la valeur dont la session est encore propriétaire. L’accès normal après une demande traitée compare les choix sans réécrire les variables chaque image.
- Le helper de sortie conserve les cinq paramètres qui utilisent encore Engine.ini. Il libère les anciens réglages de résolution/reflets qu’il possédait, à la première fermeture avec ce candidat. Les modifications externes ainsi que les valeurs personnalisées du mod INI sont conservées. Pendant cette première session, **Native** retrouve la valeur chargée au lancement, qui peut encore inclure l’ancien override sur disque.
- L’anisotropie est lue lors de la création des samplers D3D11. Les samplers existants gardent leur réglage. La mise à jour en session demande un renouvellement vérifié de ces objets. Les autres paramètres d’ombres et de visibilité gardent leur chemin après fermeture.

Critère en mission. Passer par exemple de 100 à 70 % et de 4 à 0 pour les reflets, puis revenir à **Native**, dans une scène comportant des reflets visibles. Observer sans redémarrage après le prochain tick de jeu. Contrôler que la première fermeture restitue les deux clés précédemment gérées dans Engine.ini et que les préférences du mod restent enregistrées.

## Lames de Strife

Le Blueprint `MayhemAbility_Mayhem_MeleeAttack_Blueprint` référence les montages `Strife_Horse_Attack_L_Montage` et `Strife_Horse_Attack_R_Montage`. Le squelette monté contient `Bone_Strife_Weapon_Dagger_L` et `Bone_Strife_Weapon_Dagger_R`. Le getter natif `GetCurrentActiveMontage` à `0x166AE50` parcourt les instances récentes et confirme la lecture à `0x150`. Le suivi existant des joueurs locaux utilise cette disposition après vérification des signatures, du propriétaire et des paquets de montage.

Le candidat emprunte le montage et l’os associés au côté indiqué par le nom de l’asset. Une copie visuelle de la lame de ce côté s’affiche pendant l’attaque et respecte l’option de masquage. L’option des lames rangées régit leur présence au repos, avec les attaches croisées existantes. Les modèles visuels réutilisent des composants sans collision, avec un cache par cavalier local. Les effets de mêlée, dégâts, armes natives et contrôles gardent leur chemin existant.

Critère sur les deux montures. À l’attaque à gauche et à droite, vérifier la position de la lame, son orientation et sa disparition à la fin, puis les choix OFF. Avec les lames rangées activées, vérifier le retour visuel au holster et la transition à pied. La préparation des attaches à partir des assets ne confirme pas encore le résultat visuel.
