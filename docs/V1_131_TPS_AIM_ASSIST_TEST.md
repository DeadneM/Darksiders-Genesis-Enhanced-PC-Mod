# V1.131 - aide a la visee TPS globale ou par munition

29 octobre 2026. CONSTRUITE / CONTROLEE ; NOUVELLE AIDE NON VALIDEE EN JEU. Base V1.130 explicitement validee par l'utilisateur («1.130 valider»), apres installation verifiee. Le masquage/pose de main vide et le controle de cible de Statique valides restent la reference. main reste V1.124 ; pas de promotion demandee.

## Reglage

Camera -> TPS controls and aiming -> Added TPS aim assist, groupe ferme initialement. Choix Global ou Per ammunition ; curseur0..100%, saisie manuelle et Default. Mode Global : une valeur pour les sept munitions. Mode Per ammunition : Standard, Charge, Graviton, Lava, Beam, Nature et Static independants. Les deux jeux de valeurs sont conserves quand on change de mode. Meme valeur pour la munition normale et son Hotstreak.0 partout par defaut conserve le comportement V1.130 ; ce pourcentage designe une correction AJOUTEE, pas un pourcentage du systeme vanilla.

## Calcul et perimetre

Un seul ajustement de cible3D partage par les consommateurs de projectile, fin de charge, faisceau et preparation balistique Lava. Lire seulement la cible deja selectionnee par le moteur (FWeakObjectPtr joueur+E98), verifier serie/objet, racine reciproque et position finie. Candidat devant le rayon et a6degres maximum ; interpolation a sa profondeur, qui evite de melanger avec un point ciel a100000unites.25/50/100% corrigent progressivement l'ecart,100% centre sur la cible eligible. Une collision plus proche deja rapportee par le rayon original garde la priorite ; la ligne vers la cible doit confirmer le meme acteur via le trace natif existant. Aucune recherche d'acteurs, aucun nouveau hook ni ecriture d'angle/corps/input. Reutilise le contact natif valide en V1.130.

Identification de la munition par classe et package exacts, dans les sept assets natifs verifies. Les faisceaux utilisent leur arme primaire enregistree, ou l'arme unique de leur capacite active native ; plusieurs consommateurs differents simultanes ne sont pas devines. Les instances inconnues gardent le chemin existant. Le filtre physique Static V1.130 consulte le meme rayon assiste puis conserve ses gardes de contact et exclusions des enfants. Le solveur natif Lava vitesse/pitch reste identique, seul son point d'entree peut changer. En mode3D OFF, la hauteur/arc natif reste prioritaire comme avant ; le centrage vertical n'est pas force. World Ender et autres pouvoirs restent exclus.

Souris/manette, P1/P2 et formes passent par les autorisations TPS Camera Forward existantes ; vue native, pause/roues, cinematiques et autres modes ne recoivent pas cette correction. Pas de nouveau lien a une touche. Si aucune cible native n'est disponible (ou si elle est hors cone/occultee), aucune aide ajoutee. Sur monture/coop ou certains Hotstreak, disponibilite de la cible/arme reste a confirmer en jeu ; refus plutot qu'attribution arbitraire. Les parametres de degats, portee et rebonds restent identiques ; Lava conserve son couple vitesse/angle calcule par les formules natives. Pas de promesse de FPS.

## Controles et essai

Deux passes confrontees : anciens fichiers/valeurs et chemins acceptes, puis geometrie/visibilite/identites et consommateurs natifs. Nouvelle suite AimAssist : interpolation progressive3D, cone, obstacle proche, valeurs invalides, anciens INI, mode et sept valeurs independantes, conservation des raccourcis. ProjectileSpawn etendu sur les sept classes avec aide positive, source de faisceau, entree du solveur Lava et exclusions WorldEnder/vue native/pause. Cela ne prouve pas l'effet en jeu. Neuf nouvelles cles,227 au total ;218 anciennes valeurs/55ID Hotkeys conserves. [Audit](evidence/V1_131_STATIC_REVIEW.json).

Essai groupe : comparer0 puis25/50/100% avec ennemi juste a cote du reticule ; une cible en bord d'ecran ou derriere un obstacle ne doit pas attirer. Tester les sept munitions normales/Hotstreak, surtout Lava/Charge/Graviton et rebonds Static. Passer en Per ammunition, laisser un type a0 et un autre a75, verifier leur difference et le rappel des valeurs au retour Global/rechargement. Verifier que0 retrouve V1.130, puis vue native et main vide en pause/roue restent valides. Coop/monture a confirmer quand disponible. Le journal AIM_ASSIST compte les applications, pas les impacts.

Compilation uniquement GitHub ; aucune installation locale, garder son INI. Retour a0 ou rollback V1.130 conserve la base validee. main/archive et anciennes archives preserves.
