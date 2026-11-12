# V1.165 — reçu de livraison debug

**CONSTRUIT / CONTRÔLÉ / PUBLIÉ ; EXPÉRIMENTAL en jeu.** Base validée V1.164 sur main, inchangée. Source `f716eb65cfa222055d0b136a0f8d5ae368aa7c76` ; archive `78c13a4f6240fd7c5e9b730188e31f7087a6ecf1`. Aucune compilation locale ni installation/lancement du jeu.

Optimisation de la découverte des capacités : lecture objet/flags regroupée, chunk conservé uniquement pendant une tranche, classe ParticleSystem identifiée par index/serial. Même budget/cursor/validation des ressources ; capacités, coûts et animations conservés. Reprise des assets Stoneskin sans contrôles inutiles en délai/épuisement. Candidat écharpe double face cumulé, corps/FX/restauration existants protégés et repli vers ancien revêtement si incompatible. [Périmètre et risques](V1_165_REVIEW.md).

Recherche des bugs natifs effectuée : sources officielles examinées, aucun nouveau patch moteur justifié par les preuves disponibles. Moloch validé conservé. Les anciens défauts déjà annoncés corrigés officiellement, DX12 non utilisé et D3D device lost multi-cause sont distingués. [Revue détaillée](V1_165_NATIVE_BUG_REVIEW.md). Ne pas présenter cette build comme corrigeant de nouveaux bugs UE4 non reproduits.

GitHub Windows x64 : **83/83 suites réussies**, zéro erreur et aucun avertissement C++ détecté dans le journal. [Exécution](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/43280608528). Les cas d'écharpe, MID natif/Abyssal, repli après copie, temporisation des ressources et tranche de découverte sont maintenant exécutés, et non plus seulement préparés. Cela ne démontre pas le rendu ni un gain de FPS. Avertissements externes conservés dans la preuve : dépréciations CMake MinHook et actions Node ; aucun workflow/dépendance modifié pour les masquer.

ZIP : [DarksidersGenesis_V1.165_DISCOVERY_CLOTH_TEST.zip](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/78c13a4f6240fd7c5e9b730188e31f7087a6ecf1/downloads/DarksidersGenesis_V1.165_DISCOVERY_CLOTH_TEST.zip), 2,467,054 octets, SHA256 `396576D96164900B8EB4A917E109E7387263C18CE49D3F3457C1FF1A471D675F`. CRC et PE AMD64 vérifiés. Deux fichiers seuls à la racine : ASI 2,311,680 octets, dxgi 155,136 octets ; sans INI/README. 229 anciennes archives préservées, 230 au total. 250 valeurs et 55 actions Hotkeys inchangées ; main/archive/tag v1.43 protégés, défauts identiques. Nouveau ZIP récupéré UNE fois par Git ; aucune copie HTTP supplémentaire ni ancien paquet retéléchargé.

Découverte updater debug : V1.165 est la plus récente archive valide, lien public contrôlé. Defender : aucune menace détectée dans le ZIP et les deux binaires, protection inchangée. [CI](evidence/V1_165_CI_CHECK.json) · [Paquet](evidence/V1_165_PACKAGE_CHECK.json) · [Defender](evidence/V1_165_DEFENDER_CHECK.json).

## Test groupé à faire en jeu

1. Strife à pied : comparer fluidité immobile, déplacement/tir, puis Stoneskin dans une même scène. Observer aussi le chargement et le premier lancement ; gain réel non mesuré à ce stade.
2. Écharpe Native puis Abyssale : avant/arrière, intégrité, feu avec Molten, extinction et restauration. Le changement de shader est expérimental ; un repli automatique peut garder l'ancien rendu si le contrat échoue.
3. Après changement de niveau/personnage : disponibilité et fonctionnement des capacités transférées. Courte régression cheval (marche aérienne/saut/chute), TPS/menu/roues.

Conserver l'INI personnel. Rollback : V1.164 validée disponible sur main. Aucun gain FPS ni rendu correct annoncé avant ces essais. La recherche d'apparences supplémentaires reste dans la file, hors du lot demandé 1/2.
