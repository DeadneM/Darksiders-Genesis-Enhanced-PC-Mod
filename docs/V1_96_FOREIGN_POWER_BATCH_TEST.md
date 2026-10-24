# V1.96  contrat du lot et test group

Base : debug V1.95 / 5b35a2be7f6a53d51f9b953f9d14a8bab93f14fc. Objectif : proposer tous les transferts restants dans le mme candidat, conserver Caltrops/Geyser confirms pour effets/dgts et retirer compltement notre adaptation de Stoneskin.

## Implmentation et limites

- Clone sur War : son propre ShadowCharacterComponent, issu du CDO natif, enregistr dans son monde ; callback natif SpawnShadowClone.
- World Ender sur War : son propre composant natif de rayon, configur sur son propre Chaoseater et sans socket de pistolet tranger ; callbacks natifs StartBeam/DestroyBeam. Lorigine, la visibilit  faible distance et la direction ncessitent un essai rel.
- Rampage sur Strife : dpart puis quatre fins Land/Slam Level1/2/3 ; callbacks/blasts du montage original. Le mouvement enracin du squelette War nest pas transfr, donc la charge et les poses ne sont pas promises correctes.
- Montages privs copis via constructeur natif ; arrays non partags, rserve UE, vnements remis sur la chronologie globale. Seuls callbacks de pouvoir, particules sur racine/aucun socket, sons ponctuels et SpawnBlast sont conservs. tats de notification, sons en boucle, shakes, fentres dinterruption/rotation et sockets darmes trangers ne sont pas transfrs. Pose idle du receveur : animations encore  amliorer.
- Construction des composants uniquement au lancement effectif, jamais pendant une simple lecture des emplacements. Propritaire local, rle autoritaire, personnage  pied, identit/serial vrifis. Prparation au sol uniquement. Les composants appartiennent  lacteur et suivent son cycle natif ; chec = fin native du seul lancement identifi et dsactivation du transfert concern pour la session. Un composant natif construit avant un chec peut rester dormant jusquau remplacement du personnage/fin de session.
- Stoneskin : aucune recherche/clonage/paramtre/visualisation de ce pouvoir dans notre adaptateur. Ancien choix 6 normalis  Native ; War garde son pouvoir vanilla via Native. Aucun cache vivant dune ancienne ASI ne doit rester : redmarrer pour comparer le cristal. Aucun lien causal prtendu.
- Nettoyage : trace DispatchEvent Geyser provisoire supprime, aucun nouveau hook et aucun changement des tirs, camras, contrles, autres visuels, INI ou hotkeys.

## Critre de russite et essais groups

1. Aprs redmarrage, War sur Native : comparer Stoneskin/cristal ; noter si le dfaut persiste. Ne pas dduire la cause  lavance.
2. War : Clone puis World Ender, un emplacement  la fois. Vrifier effet, dgts, cot de Courroux en mission, fin et reprise du mouvement ; comparer native/TPS pour le rayon.
3. Strife : Rampage, appui bref puis maintenu ; vrifier dpart, impacts, fin, cot et reprise du contrle. Des poses incorrectes sont une limitation connue, une immobilisation persistante est un chec.
4. Refaire un Caltrops sur War et Geyser sur Strife ; effets/dgts dj confirms, vrifier absence de rgression. Revenir ensuite aux trois Native et confirmer pouvoirs propres.

P2 diffr. Les vrifications automatiques ne prouvent ni rendu ni dgts en jeu.

## Installation et retour arrire

ZIP cumulatif Windows x64, quatre fichiers  la racine. Installation manuelle jeu ferm ; conserver son INI personnel et une copie de lancienne ASI/dxgi.dll. Pas de lancement du jeu ni dinstallation automatise. Retour arrire : remettre V1.95 ; les 160 anciennes archives restent intactes. Les six cls de choix restent les mmes ; les autres rglages et sauvegardes ne sont pas modifis.
