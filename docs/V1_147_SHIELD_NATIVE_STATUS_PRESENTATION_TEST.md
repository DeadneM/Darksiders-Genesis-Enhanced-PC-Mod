# V1.147 — bouclier indépendant du statut natif

Base : V1.146 (`44ec0ba`), lot bouclier jugé non fonctionnel par le joueur. Léviathan/Stinger conservés ; minimum des deux tailles abaissé à 0, qui masque effectivement leur seul rendu.

## Cause démontrée et correction

La capture en lecture seule du 3 novembre montre `MayhemStatusEffect_Core_ShieldDemon_3_Blueprint_C`, destinataire+0x68, référence de PSC+0x78 et véritable template `P_CreatureCore_ShieldDemon`, attaché au CharacterMesh0 appartenant à Strife. Le champ+0x58 vaut **zéro** ; V1.145/V1.146 le prenaient à tort pour une référence de particule. La liaison échouait et le réglage ne s'appliquait jamais. Les anciens tests remplissaient ce champ avec une valeur supposée : leur réussite ne reproduisait pas le vrai jeu. Nouvelle fixture avec +0x58=0.

Le bouclier conserve désormais son composant, son statut, ses dégâts absorbés et sa recharge natifs. L'ASI crée uniquement un composant visuel privé sur la véritable attache du composant natif et masque l'original seulement quand le remplacement et son réglage sont prêts. Choix/paramètres applicables au bouclier déjà actif ; disparition quand le jeu retire sa référence native ; réapparition à la recharge. Native restaure le rendu et le comportement du jeu. Hidden masque l'original sans créer d'effet. **Original effect (adjustable)** sélectionne le template d'origine dans le nouveau chemin réglable. Aucun statut, santé, capacité ou timer n'est écrit. Aucune substitution du factory natif pour cette famille ; ancien suivi à 32 enregistrements et ses hooks auxiliaires retirés. Deux contextes locaux bornés, propriétés réfléchies mises en cache. Signature de l'émetteur vérifiée avant l'installation du detour mêlée/Lava : ordre d'initialisation contrôlé par test dédié, état indisponible explicite si refus.

## Ressources et liste de test

28 entrées : Native, Hidden et **26 effets**. Valeurs0..5 antérieures conservées, Original réglable=6 ; 21 autres candidats ajoutés à partir de templates ParticleSystem déjà identifiés dans la bibliothèque. Grasping Shadows, Nature healing aura, trois Molten cores, quatre Vampiric, Hotstreak, deux body charges, deux charge auras, Vortex charge, Tremor ground charge, glyph Caltrops, deux revive loops, Earthsplitter charge, Tremor fist charge. Les variantes rejetées Rampage/Chaos/Fire aura ne sont pas réintroduites. Les apparitions natives ennemies/pouvoirs et Léviathan restent indépendantes. Aucun rendu de ces candidats n'est déclaré validé.

Si la ressource choisie manque, une requête charge **ce seul effet**, après 500 ms de contexte stable et hors pause/cinématique. API moteur réfléchie `MakeSoftObjectPath` (exec1D79110, PathString16 à0, retour24 à16, tampon40) puis `LoadAsset_Blocking` (exec1D78A90, SoftObjectProperty40 à0, retour8 à40, tampon48). CDO natif observé et revérifié par classe/slot/nom, y compris son serial paresseux0, aucune adresse CDO devinée. Noms, types, tailles, offsets, entrées natives et résultat ParticleSystem/package vérifiés. Réentrée refusée ; une tentative par choix/monde, pas de chargement de toute la liste ou de boucle de retry. Le premier chargement peut provoquer une pause brève. En cas de refus ou d'échec, original conservé et message de journal. Les effets chargés sont ensuite référencés par le seul PSC privé, sans root permanent d'assets.

SpawnEmitterAttached/DestroyComponent et transformation du composant privé utilisent les mêmes API déjà vérifiées pour Léviathan. Taille0 masque aussi les émetteurs qui ignoreraient l'échelle ; remettre une taille positive réaffiche seulement notre propre composant. Taille/position par Strife/War conservées, pas de nouvelles clés ou actions Hotkeys. Aucun chargement ni création pendant une cinématique.

## Crash du 3 novembre — sujet distinct ouvert

V1.146 confirmée par le hash de l'ASI ; à 14:01:59, RenderThread1, lecture0x20, exécutableRVA D5D0A1. L'instruction lit `[rax+0x20]` avec rax0 ; pile native incluant l'exécuteur de commandes RHI, aucun appel ASI dans la pile publiée. Dernière sortie de cinématique 14:01:57.999, Level_17 ; utilisateur confirme reprise sans toucher au menu. INI : bouclier Native 0, Léviathan 3, profils DOF personnalisés actifs. Avertissement Bokeh DOF et nettoyage de matériaux des cristaux du niveau juste avant. Ces proximités ne prouvent ni une cause caméra, ni un défaut pilote, ni une innocence du mod. Journaux, contexte et minidump copiés en privé avant relance. Aucun patch de rendu ou changement de profil utilisateur pour ce crash. Si reproductible, comparer la même transition avec un seul facteur visuel désactivé, puis sans ASI si nécessaire. Aucune résolution annoncée.

## Contrôles et essai

Construction uniquement GitHub Actions, tests/package à consigner à la livraison. Nouvelles vérifications : véritable statut avec source nulle, choix sur effet actif, taille/position sans dérive, taille0, fin/recharge, Native/Hidden, chargement demandé une fois/refus/identité, pause/cinématique/P2, ABI du loader et isolation du factory. Ces fixtures ne prouvent pas le rendu ni les FPS.

Tester Original effect réglable et taille0 d'abord, puis quelques variantes, déplacement XYZ, rechargement et épuisement/recharge du bouclier. Vérifier Native, Hidden et Léviathan à0 puis positif. Les variantes seront triées d'après ce retour. Crash de rendu et sujets antérieurs (Stoneskin intermittent, pose, dash aérien) restent ouverts. Aucun lancement, installation, écriture d'INI/PAK/sauvegarde ni compilation locale.

Statut final : CONSTRUIT / CONTRÔLÉ, 74/74 suites GitHub ; EXPÉRIMENTAL en jeu. [Livraison et preuves](V1_147_DELIVERY_RECEIPT.md).
