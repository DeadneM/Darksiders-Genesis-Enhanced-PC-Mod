# V1.188 — choix HDR au viewport et lames de Strife

Base expérimentale V1.187 sur debug. Retours joueur du 23 novembre. L'orientation à pied est meilleure. Le plat doit épouser les hanches et la pose de rangement doit être identique sur les deux montures. À cheval, la sélection des lames doit s'appliquer pendant les deux attaques natives.

## HDR10

Le journal V1.187 indique un buffer DXGI 10 (RGBA16F), le moteur 1/0/0 (activation demandée, encodage SDR), la présentation historique (SwapEffect 0) et le refus 0x887A0004. Aucune sélection du format RGB10A2 au démarrage n'apparaît dans ce relevé. Le branchement de V1.187 dépendait du passage tardif de l'ASI au point de configuration initiale des CVars, avant le démarrage du rendu.

V1.188 traite le format au moment où FSceneViewport interroge r.HDR.EnableHDROutput. Les deux appels reconnus sont à 0x145D23B (création) et 0x145922C (création/redimensionnement). La fonction à 0x12F5800 renvoie son résultat natif. Quand l'option HDR10 avec signalisation DXGI est demandée, elle sélectionne PF_A2B10G10R10 à cette limite, sur le RHI DX11 dont la méthode RHICreateViewport est à 0xD58AF0. Les deux chemins lisent le format juste après cette interrogation. Le chemin V1.187 de négociation DXGI flip et le repli SDR reprennent ensuite. Le chargement des hooks groupés, le format reçu dans le constructeur et le résultat de la négociation sont journalisés.

## Lames de Strife

Les maillages Mayhem et Despair et le squelette à pied ont les mêmes positions de holster par rapport au bassin. Les deux copies rangées utilisent ces positions croisées depuis le bassin sur les trois modèles. Cela garde le placement indépendant du masquage des pistolets et de leurs os de holster. L'axe fin des deux maillages de dague est leur axe local Y. La rotation reprend la direction arrière et descendante du holster et présente le plat contre la hanche. La géométrie des deux pointes a été vérifiée sur les sommets des modèles.

Une attaque montée reconnue ouvre les deux mains au choix de visibilité. Native montre les deux lames, Hide left/right ne garde que le côté autorisé, Hide both retire les deux. Les mêmes choix s'appliquent aux lames rangées et tenues. Les lames reviennent au bassin à la fin de l'attaque. Les effets et les dégâts suivent les fonctions existantes.

## Essai

- HDR10 1000 avec **Match HDR profile**, DXGI coché et Windows HDR actif. Après relance, vérifier **10-bit**, **DXGI flip**, **accepted** et **Native HDR render path: enabled**.
- Avec **Show Strife blades when sheathed**, vérifier le plat contre les hanches à pied et le même placement sur Mayhem et Despair. Vérifier Native puis chaque côté masqué, au repos et durant les deux attaques montées. Le masquage des pistolets est indépendant.

V1.188 reste expérimentale jusqu'à l'essai joueur. V1.187 reste disponible comme ZIP de retour.
