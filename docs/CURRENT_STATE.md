# État courant — V1.171 validée, freezes en diagnostic

L’utilisateur valide les fonctions V1.171 sur debug le 19 novembre. Main/release restent V1.168 ; aucune promotion demandée ici. V1.171 : 86/86 tests Windows, ZIP vérifié et conservé. [Livraison](V1_171_DELIVERY_RECEIPT.md).

Nouveau sujet : freezes et 100 % GPU signalés, ancienneté inconnue. Relevé Windows autorisé de 31 secondes avec freezes confirmés : moteur 3D 51–65 %, mémoire dédiée environ 3,2 Gio. Pas de mesure de chaque image ; les pointes brèves ne sont pas exclues. 4K, qualité élevée, VSync ON. [Diagnostic et limites](GPU_FREEZE_REVIEW_2026_11_19.md).

Optimisation CPU préparée, non construite : ne pas verrouiller/mettre à jour le suivi des liaisons PS avant la reconnaissance d’un shader de visée. Liaison native conservée, comportement de filtrage cible inchangé. Pas de gain mesuré ni de correction des freezes annoncée. Aucun INI, réglage, fichier du jeu ou installation modifié pendant le diagnostic.

SourceControl, autre chargeur, streaming/DXVK : pas de nouveau patch. [File](ACTIVE_QUEUE.md).

Mesure DXGI complémentaire : 1204 présentations en 20 s, intervalle médian 16,68 ms, maximum 19,63 ms. Pas de pause de soumission >25 ms dans cette fenêtre distincte ; affichage effectif/GPU non mesurés. Micro-saccades toujours ouvertes. Session de trace arrêtée.
