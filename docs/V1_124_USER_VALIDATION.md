# V1.124 - validation utilisateur et suite

28 octobre 2026. Retour explicite : « super cest valider, quest ce qui nous reste a faire, que voudrais tu faire toi ».

**VALIDE PAR L'UTILISATEUR dans son essai rapporte : correctif du petit saut final de Caltrops sur War, V1.124.** Le sujet est clos ; conserver l'appel natif unique borne et le geste, les pics et les sons acceptes. Geyser et Stoneskin deja valides restent figes. Ce retour ne fournit ni protocole detaille, ni validation exhaustive P2, annulation/bord de plateforme ou degats compares.

Base retenue pour la suite : V1.124 cumulative, source compilee `3bab35a50a37a6b6dfa5f8ac4c68a47418f9548b`, archive immuable au commit `c6751b4ef8835f25e9a72cd6c424d4ea833bcff6`. [Recu de livraison](V1_124_DELIVERY_RECEIPT.md). Aucune nouvelle build, installation ou modification runtime pour cette consignation. L'utilisateur n'a pas demande de nouvelle fonctionnalite dans ce retour.

## Restant reel

- **Apparences : essais incomplets.** La bibliotheque de particules est livree et l'onglet est visible ; la validation famille par famille des variantes, du retour Native et du masquage reste a faire. Elle ne remplace pas tous les modeles/materiaux/composants persistants. [Perimetre et essais](V1_116_NATIVE_EFFECT_LIBRARY_TEST.md). Les autres modeles/outils de l'inventaire ne sont pas tous des options selectionnables prouvees.
- **Rampage sur Strife : degats pendant la charge non compares explicitement sur ennemis.** Fin/geste et autres retours acceptes conserves ; ce manque de mesure n'est pas un nouveau bug demontre.
- **Coop/P2 : essais finaux differes par l'utilisateur.** Capacites transferees simultanees, proprietaires/effets/sons, puis roues, profils camera pied/cheval/ultime et reglages gameplay P2. Tests source/fixtures ne valent pas validation en jeu.
- **Autres icones/effets plats en P2 : differes explicitement.** Ne pas modifier avant identification ; indications d'interaction deja corrigees conservees.
- **HUD general apres refus d'arene verrouillee : surveillance seulement.** Derniers essais sans reproduction ; aucun correctif aveugle ni cause etablie.
- **Anciennes demandes Recovery dash/coffres : pistes differees.** L'ancien correctif dash rejete reste retire. La reception lourde V1.103 a ete validee ; ne pas la presenter a nouveau comme absente ni rouvrir les gestes valides.

## Suite recommandee, pas encore executee

1. Consolider V1.124 : deux passes de revue du code actuel des transferts, confronter les candidats retirables, conserver callbacks/restaurations/compatibilite ; ne promettre aucun gain FPS ou taille avant mesure. Le nettoyage de167 lignes V1.77 a deja ete integre en V1.78 : ne pas le recompter comme nouveau travail.
2. Faire un essai groupe des apparences et des degats de charge, puis corriger uniquement les cas observes. Eventuelle simplification de presentation du menu sans changement des valeurs/INI/raccourcis.
3. Terminer par la coop selon la decision utilisateur ; ajouter les autres FX P2 seulement quand leur cas precis sera prepare. Une promotion stable reste une decision distincte.

Debug uniquement ; main/archive, anciennes archives, INI personnel, raccourcis, sauvegardes et PAK conserves. Les anciens paragraphes de preparation/livraison restent historiques : ils ne remettent pas cette validation en attente.
