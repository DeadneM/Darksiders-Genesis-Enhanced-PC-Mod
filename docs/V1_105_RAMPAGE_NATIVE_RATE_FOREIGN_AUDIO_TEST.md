# V1.105 - Rampage natif et audio des transferts

**Objectif** : corriger le refus de Rampage Strife mesure en V1.104 et restaurer les sons des capacites transferees, sans changer leurs effets deja acceptes. **Critere de reussite** : Rampage lance deux fois, deplace/atteint, respecte obstacle/relachement et rend les commandes ; les cinq autres transferts gardent leurs effets/couts avec sons audibles sans doublon. **EXPERIMENTAL, non valide en jeu.**

## Base et retour

Base validee V1.103, reception lourde incluse. V1.104 corrigeait un garde FName mais l'essai utilisateur echoue : aucun Rampage, puis gris. Journal 25 octobre : refus de construction/shape en phase 0, abandon natif et desactivation pour la session. Le gris est ce repli, pas une preuve de cout/cooldown natif. Aucun CAST Rampage reussi dans cet essai. Les cinq autres transferts restent acceptes pour leurs effets selon les retours anterieurs ; leur audio devient un sujet ouvert.

## Premiere revue : preuves natives

Lecture externe autorisee QUERY_LIMITED_INFORMATION|VM_READ, aucun appel ni ecriture au jeu. Les cinq montages Rampage sont lus : depart RateScale 1,5, Land et trois Slam 1. Le garde V1.104 refusait le depart a 1,5 avant toute construction : erreur de constante reproduite sur les octets charges. V1.105 corrige le contrat a cette valeur exacte, conserve les valeurs de la copie et les clips de mouvement natifs. RootMotionMode=3, identites/proprietaires/serials, slot DefaultSlot non rendu et repli d'abandon restent en place. Les erreurs de source, copie et event sont distinguees. Les autres controles de forme/copie restent actifs ; un echec ulterieur reste possible.

26 notifications sonores inspectees sur Rampage, Stoneskin, Caltrops, World Ender, Clone et Geyser : SoundCue ou SoundWave, attache None, follow natif, volumes/pitch valides, entree Notify 0x16977D0. Reflection AnimNotifyEvent : TriggerTimeOffset +0x34, EndTriggerTimeOffset +0x38, poids +0x3C, Notify +0x48, duree +0x58, EndLink +0x60. EXE exact : Notify prend RCX=notify, RDX=mesh, R8=animation ; bFollow +0x48 utilise SpawnSoundAttached 0x189B3E0, sinon PlaySoundAtLocation 0x18911C0. Ce chemin conserve le traitement natif du son. Deux objets n'ont pu etre inspectes pendant le scan : aucune exhaustivite des objets charges n'est annoncee.

Caltrops et Stoneskin utilisaient un montage cree depuis le geste du receveur sans sons source : perte visible dans le code. Geyser/Clone/World Ender copiaient deja des notifies de son. Les sons de lancement observes sont a t=0 ; leur absence exacte dans la queue/mixeur n'est pas capturee. L'adaptation proposee retire seulement les sons initiaux pris en charge de la copie rendue et execute leur notify natif apres confirmation du cast. Ce choix evite de dependre du poids initial du blend, sans pretendre avoir mesure ce poids.

## Implementation et limites

Une adaptation audio commune dans ForeignMontage, aucun nouveau module/hook/timer. Plans bornes de 32 cues, assets source autorises, classe et signature native, volume/pitch/socket verifies. Proprietaires reciproques locaux, role autoritaire, mesh propre et ability active ; identites faibles revalidees entre appels. Source ou signature audio absente = audio ignore, pouvoir deja valide conserve. Aucun chargement force de son, aucun changement des sources partagees.

Geyser/Clone/World Ender : son initial retire du plan prive seulement si le notify natif est pris en charge ; autres sons/callbacks conserves. Caltrops/Stoneskin : sons initiaux apres succes, sons plus tardifs copies dans le montage prive vide, temps et offsets convertis selon sa duree ; volume/pitch/Cue source inchanges. Les callbacks/status/blasts ne sont jamais recopies par ce helper audio. Rampage utilise la meme entree audio initiale quand son montage est effectivement joue ; ses clips d'origine sont conserves. Risque a verifier : doublon de son initial sur ce slot non rendu, sons de boucle et sons portes par d'autres composants. Le rendu sonore reel et le cycle Rampage complet restent a tester.

Voix d'effort du proprietaire original du pouvoir conservees, aucune nouvelle voix du receveur inventee. L'animation Rampage reste une pose de locomotion Strife ; phase de charge/frappe War non affichee. P2 et deux Rampage simultanes non valides. Les sons emis plus tard par projectiles/effets natifs restent sous leur lifecycle natif ; aucune garantie d'exhaustivite audio avant retour de jeu.

## Seconde revue et livraison

Fixtures : vitesse native 1,5 depart conservee, taux incorrects et autres phases refuses ; audio initial une fois au point d'appel, sons ulterieurs convertis, liens prives, absence d'ecriture source, signature/socket/pitch invalide, acteur distant/mesh emprunte, ability inactive, expiration de serial entre appels, hors thread, absence de signature et fallback. Tests de gameplay existants gardes, leurs stubs audio ne prouvent pas l'audibilite du moteur. Resultats Windows et ZIP a consigner apres GitHub Actions.

Aucun nouveau reglage ; 169 cles INI, 55 ID Hotkeys ; HeavyLandingFeature/tests compares a V1.103. Anciennes archives et main/archive/v1.43 intacts. Compilation uniquement GitHub ; aucune installation locale. L'utilisateur installe via l'updater debug en gardant son INI, raccourcis, PAK et sauvegardes. Repli V1.103 avec emplacements Strife Rampage sur Native ; garder le journal si echec.

## Essais

1. Strife a pied, Rampage sur un emplacement debloque : native puis TPS, espace libre puis obstacle ; deplacement/degats, relachement, marche/attaque apres fin et deuxieme lancement. Si refuse, conserver la ligne precise du journal, puis Native.
2. Geyser et Stoneskin Strife ; Clone, Caltrops et World Ender War : sons au lancement puis pendant et apres effet ; pas de double son, bruit en boucle qui persiste ou changement des effets/couts.
3. Pouvoirs originaux sur leur personnage habituel, surtout Rampage War : comportement conserve. P2 plus tard selon la decision utilisateur.

Suite : retour groupe Rampage/audio, traiter le premier refus reel ou son encore absent. La V1.104 n'est pas requalifiee en validee.
