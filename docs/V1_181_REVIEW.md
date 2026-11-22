# V1.181 — présentation montée et transport HDR expérimental

Base cumulative debug V1.180 (95c847d). Main V1.173 préservée. FX de mêlée à pied/cheval VALIDÉ utilisateur et module inchangé. Changements EXPÉRIMENTAUX : CI et tests visuels encore requis.

## Présentation des armes
Lecture VM_READ : Despair, main RightPistol BA0 / LeftPistol BA8 ; rangement RightPistolHolstered BF8 sur Bone_Strife_Holster_R / LeftPistolHolstered C00 sur Bone_Strife_Holster_L. Corps 170 os, cavalier séparé 80 os. Ancien masque limité au squelette normal. Désormais une correspondance commune modèles/visibilité : main droite → holster gauche, inversement. Mayhem et Despair, les deux classes de cheval de Strife, sont admises avec propriétaire Strife local contrôlé réciproque. Masque des holsters intégrés résolu par noms d'os feuilles, cache par identité complète, matrices privées des paquets courants/précédents seulement ; mains, sockets de tir et animations inchangés. Refus d'un paquet invalide avant toute écriture.

Appearance → Character model : Show Strife blades when sheathed (OFF par défaut), près du masquage des lames. Copies décoratives StaticMeshComponent sans collision ; source = modèle de lame native. Composants de combat et traces jamais déplacés. Contrats réfléchis observés en lecture seule : AddComponent 80 octets, attache SceneComponent 21, collision 1, destruction Actor 8. API appelées uniquement par la candidate sur le thread jeu, avec fonction/paramètres/identités vérifiés. Création bornée, contrat de destruction exigé avant allocation, échec d’attache mémorisé sans boucle de retry, attaches/asset en cache. Copies masquées si côté OFF ou lame native utilisée. Placement à tester. Cela ne prouve pas la réparation des lames natives tenues pendant les attaques à cheval : sujet encore ouvert, dernier lecteur non exécuté car jeu fermé.

## HDR
Retour V1.180 : HDR10 1000 délavé, buffer 16 bits rapporté. Incohérence possible, pas preuve isolée d'espace couleur. EXE : r.DefaultBackBufferPixelFormat, aide native 3 Float RGBA / 4 RGB10 Alpha2. Le helper demande désormais un buffer accordé au profil, avec transaction de sauvegarde/propriété/restauration existante après fermeture. Options matched/native/RGB10/float16.

DXGI : CheckColorSpaceSupport puis SetColorSpace1 sur buffer concordant, fenêtre Unreal du processus seulement. HDR10 PQ Rec2020, scRGB linéaire Rec709. Aucun filtre constructeur ni ResizeBuffers forcé. Une tentative par chaîne/redimensionnement, pas d'appels COM en boucle ; état accepté/refusé/incohérent visible. Métadonnées HDR10 auto 1000/2000 ou mastering peak/black, MaxCLL/FALL bornés. Elles décrivent le contenu au moniteur, ne recalibrent pas la courbe native et ne constituent pas une conversion SDR→HDR. HUD natif réglable conservé. Native par défaut. Fermeture/reprise requise pour ces demandes couplées.

Sources : [Microsoft Advanced Color](https://learn.microsoft.com/en-us/windows/win32/direct3darticles/high-dynamic-range), [SetColorSpace1](https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_4/nf-dxgi1_4-idxgiswapchain3-setcolorspace1), [SetHDRMetaData](https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_5/nf-dxgi1_5-idxgiswapchain4-sethdrmetadata).

## Graphismes en direct
TAA, Image effects, post-processing et DOF sont déjà en direct. Les sept Rendering quality et HDR gardent leur chemin de redémarrage : setters/flags/reconstruction de ressources à étudier avant modification à chaud. Recherche ajoutée à la file, aucune disponibilité à chaud inventée.

## Essai groupé et rollback
CI Windows x64 à effectuer. Tests ajoutés : côtés croisés, deux classes de cheval, paquet 170 os / refus transactionnel, profil de buffer, métadonnées bornées, roundtrip INI, entrée C# invalide sans mutation.

Tester droite seule/gauche seule/deux/OFF à pied et sur les deux chevaux, repos/tir/roues/pause. Puis case des lames rangées, côtés masqués, combat/retour au repos, montée/descente. HDR : matched + signaling ON + métadonnées auto, HDR10 1000 puis scRGB 1000, fermeture/reprise à chaque changement ; vérifier format, signaling, couleurs et menus. Aucun succès HDR visuel annoncé. Avant downgrade : Native puis fermeture pour rendre les clés possédées. Aucun lancement, installation, édition d'INI personnel/Windows/sauvegarde. ZIP deux binaires uniquement.
