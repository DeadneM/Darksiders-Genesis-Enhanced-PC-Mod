# V1.123 - Verified Stoneskin transition FX / Caltrops stomp delivery

**CONSTRUIT / CONTROLE ; NOUVEAUX EFFETS ET CALTROPS EXPERIMENTAUX, NON VALIDES EN JEU.** Base cumulative V1.122 `6979c8dd6e193dda2409dcb9430318fbf2e23588`. Geyser explicitement valide par utilisateur, inchange. Gestes Stoneskin1.25s, fondus, materiaux et duree independante du statut conserves. Pas de compilation, installation ou lancement du jeu local. P2 en dernier.

Source `2ede45b86bae9c5fd6c15f123cb3523554225e4e`, archive `6091b34fdafa759d6b37d75b17ab7ac7cc2b5e97`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40560388902) : **65/65 suites**, zero avertissement/erreur MSVC. Deux relectures de code concordantes : Geyser et tous les autres helpers de montage retenus identiques ; chemins valides de Clone/Rampage/WorldEnder, camera/manette/cheval/menus/updater/config inchanges. Ancien copier de particules en echec et getter Caltrops inutilise supprimes avec leur test obsolete. Ces controles ne prouvent pas l'affichage en jeu.

## Changements et limites

- Strife Stoneskin : une explosion native Base/Molten a l'apparition effective du revetement apres le geste accepte, cendres au retrait complet du statut, pas a la fin du geste. API publique SpawnEmitterAttached via dispatcher existant, contrat72 octets/types/offsets/classe/CDO et mesh proprietaire verifies avant chaque appel. Emetteur renvoye non-null requis ; auto-destruction native. Pas de fausse cendre sur transformation, autre monde/mesh/proprietaire, lecture inconnue, shutdown, ni Molten->Base. Delai possible si asset decouvert tardivement ; pas d'affirmation de rendu reussi avant essai.
- Autorisation humaine explicite « Autoriser cet essai visuel limite » recue apres refus initial du controle automatique. Methode interne constructeur/notification directe abandonnee ; seul l'appel public borne est retenu. Lecteur externe strictement en lecture seule, aucun appel moteur ni ecriture ; pas de sauvegarde/PAK modifie. [Perimetre](V1_123_STONESKIN_FX_REVIEW.md).
- War Caltrops : sequence complete War_Atk_Stoneskin,1.8333s a vitesse native, pics au signal du pied0.740429/0.741106s ; une seule creation par le spawner Caltrops existant. Pas de spawn initial ni minuterie. Trois nouveaux gardes exacts de notifications du geste emprunte (peau Blueprint0x1697780, acteur0x5D7C00, blast0x5D7DF0), signatures16 octets et rollback partiel. Scope faible acteur/mesh/cast/montage/sequence/monde/index/courante capacite : supprime seulement les actions donneuses dans le montage prive Caltrops. War Stoneskin ordinaire transmet ses notifications natives. Hook particule existant utilise pour le pied ; ancien hook de projectile dash entierement inchange.
- Le Blueprint Caltrops garde son lifecycle physique natif vol/chute. Le nouveau geste peut encore laisser un rebond lie a ce lifecycle ; cela reste ouvert jusqu'au test. Aucun ancien garde VM/mouvement refuse n'est ajoute. [Contrat et tests](V1_123_STONESKIN_TRANSITIONS_CALTROPS_STOMP_TEST.md).

Tests complementaires : exact ABI public, refus de layout/possession/thread/identites, absence de mutation source/CDO/mesh, resultat null, Base/Molten/P2 ; vrai retrait du statut et non repetition ; apparition/recast/dependances tardives ; cue Caltrops differe, une creation, physique native conservee, les trois chemins donneurs supprimes uniquement dans le montage prive, transmission native ailleurs. Les trois nouvelles gardes restent experimentales en situation reelle.

Premier essai GitHub : compilation reussie,64/65 tests et un avertissement C28020, aucune archive publiee. Le nouveau test a revele que Name par defaut est invalide (-1,0), pas None. Argument socket explicitement corrige a(0,0), test conserve ; affectation des deux joueurs rendue explicite pour l'analyse. Deuxieme compilation propre ci-dessus. [Trace du premier essai](evidence/V1_123_CI_FIRST_ATTEMPT.json).

## Livraison

[ZIP public immuable a quatre fichiers racine](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/6091b34fdafa759d6b37d75b17ab7ac7cc2b5e97/downloads/DarksidersGenesis_V1.123_STONESKIN_TRANSITIONS_CALTROPS_STOMP_TEST.zip), 2332848 octets, SHA256 `14269800F3ADF5DF640EBF15AC36F7215BF3FCA79D87B07B252399507CF0F279`. ASI SHA256 `76A3C904D48EDE96794C0FF2A1733DC1C465EFEABBB9627FE365934736230B82`. NoCompression, CRC valide, ASI/DXGI AMD64, INI/README embarques conformes au commit de compilation.212 cles INI et55 actions Hotkeys conservees ; garder l'INI personnel.187 archives anterieures identiques dans Git (188 total), main/archive/v1.43 intacts. Suppression locale deja presente de V1.108 et audit hors perimetre non touches/non commis.

Updater debug selectionne V1.123 blob `e8f5e2b18c6a3dab13debaaf896e4ee230d43a7b`, HTTP200 et octets/hash exacts. Interface du jeu non manipulee. Defender ZIP et quatre fichiers extraits : exit0/aucune menace detectee, protections actives avant/apres, sans exclusion ni changement de reglage. Plateforme 4.18.26090.9, moteur 1.1.26090.9, signatures 1.461.225.0. Diagnostic ponctuel, pas certification universelle.

## Test utilisateur / rollback

1. Strife Stoneskin Base/Molten : geste et rendu acceptes, explosion a l'apparition, cendres a la vraie fin ; duree/protection et controle normaux.
2. War Caltrops au repos/en mouvement TPS/vue native : coup de pied complet, un seul jet au pied, plus de petit saut, aucun revetement/cristal/blast emprunte, cout/degats/audio et deplacement/saut normaux apres.
3. War Stoneskin natif, puis bref controle des autres capacites validees. Geyser inchange. Coop en dernier.

Non installee localement pendant le jeu ouvert. Conserver INI personnel et V1.122 ZIP/sauvegarde preinstallation pour rollback.

Preuves : [CI](evidence/V1_123_CI_CHECK.json), [ZIP](evidence/V1_123_PACKAGE_CHECK.json), [updater](evidence/V1_123_UPDATER_CHECK.json), [Defender](evidence/V1_123_DEFENDER_CHECK.json), [deux relectures](evidence/V1_123_STATIC_REVIEW.json), [contrat observe en lecture seule](evidence/V1_123_READ_ONLY_CONTRACT.json).

## Installation locale sur demande ulterieure

V1.123 installee apres demande explicite « installe la », jeu ferme. Sauvegarde V1.122 complete verifiee avant remplacement : `before-V1.123-20261027-224229-e9a7da`. ASI, DXGI et README remplaces depuis le ZIP verifie ; leurs empreintes et leur architecture ont ete relues independamment apres copie. INI personnel identique octet pour octet a sa sauvegarde, raccourcis conserves. EXE natif identique ; aucun lancement du jeu, aucune modification volontaire de PAK ou de sauvegarde. Inventaire PAK (chemins/tailles/dates) inchange. Validation en jeu encore attendue. [Recu d'installation](evidence/V1_123_INSTALL_CHECK.json).
