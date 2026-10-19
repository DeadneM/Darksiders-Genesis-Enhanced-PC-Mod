# État courant — V1.66 debug

19 octobre 2026. **Base de travail retenue : V1.66, CONSTRUITE / CONTRÔLÉE ; sauts et holsters VALIDÉS par l’utilisateur dans son essai.** Retour explicite reçu après les tests ; le canal reste debug. Cette mise à jour enregistre la validation sans modifier le code, la configuration ni le paquet publié.

## Livraison vérifiée

- Source : `f01f70d563dc4e3716464718fb6642bbdd8f9702`.
- Archive : `12061a956b9257284c8b1779503cf058b75fab1f`.
- [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39217494013) : 41/41 suites, aucun avertissement MSVC ASI/DXGI. Avis de dépréciation des outils distincts.
- [ZIP cumulatif](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/12061a956b9257284c8b1779503cf058b75fab1f/downloads/DarksidersGenesis_V1.66_NATIVE_JUMP_CYCLE_TEST.zip) : 934413 octets, SHA256 `CC15584E210717E4F173967BE47024CA949983FD24870B03190191936888F67F`. CRC, AMD64, quatre fichiers racine, marqueurs et téléchargement HTTP 200 contrôlés.
- 139 réglages et 53 actions conservés ; 130 anciens ZIP vérifiés intacts à la livraison. Main/archive et tag v1.43 protégés. Aucun remplacement du ZIP V1.66 par ce travail documentaire.

## Derniers retours faisant autorité

État antérieur V1.65 : holsters montés correctement placés, petit délai ; trois délais de rangement validés ; durée ultime validée après activation de sa case (l’utilisateur a rétracté son premier échec). Le double saut était encore défectueux : deux remontées sans animation puis blocage.

V1.66 remplace la substitution isolée du getter par un réarmement natif conjoint au point de lecture d’animation. L’animation voit le vrai compteur disponible puis le saut natif republie deux. Recherche des modèles montés anticipée à pied. **Retour V1.66 : « pour les sauts c'est bon bravo », « pour les holsters c'est bon bravo », puis « donc tout a lair bon/ ». Sauts et holsters VALIDÉS dans cet essai, code retenu.** Le retour ne détaille ni nombre de sauts, ni modes/personnages testés, ni mesure du délai initial ; aucune couverture exhaustive n’est déduite. [Détails et essais](V1_66_NATIVE_JUMP_CYCLE_TEST.md).

Les acquis précédents de visée, reprise AIM après dash, retrait des projectiles/sons/effets Standard parasites, netteté TPS, Standard limité, grappin, ressources et raccourcis sont conservés. Aucun nouveau retour ne les invalide. Autres effets et icônes P2 reportés sur demande explicite.

## Travail poursuivi pendant l’essai

Nouvel audit du chemin d’interruption natif de Recovery et inventaire élargi des modèles de Strife. **Aucune modification de source runtime, de configuration, de binaire ou de mémoire du jeu dans cette reprise.** Le relevé externe préparé n’a pas été exécuté sur le jeu. L’ancien Recovery reste retiré. [Rapport et limites](NEXT_RECOVERY_AND_MODELS.md).

README, file active et roadmap séparés des anciennes préparations ; les trois anciens documents sont conservés octet pour octet dans `docs/history/`, avec manifest de signatures. Le carnet technique reste cumulatif. Les versions et références du présent état n’impliquent aucune promotion de main ni de release publique.

## Reprise suivante

Repartir de V1.66 validée pour les sauts et holsters. Consulter la [file active](ACTIVE_QUEUE.md) et les logs réellement disponibles. Action Recovery et modèles alternatifs restent des travaux ouverts ; autres effets P2 différés, Auto check update sans confirmation visuelle distincte. Préserver l’INI personnel, les raccourcis, les sauvegardes et les acquis. Pas de lancement/rechargement du jeu par l’agent sans demande explicite. Construire une version distincte seulement pour un nouveau lot retenu.

[Ancien état intégral jusqu’à V1.66](history/CURRENT_STATE_THROUGH_V1_66.md) · [Carnet technique](TECHNICAL_NOTEBOOK.md) · [Roadmap actuelle](ROADMAP.md).
