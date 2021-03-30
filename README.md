# Projet Santoline

## Récupérer le projet

Dans un premier temps, quand vous clonez le projet, mettez le projet à un lien comme ça :

```batch
C:\Users\[username]\
```

### Architecture

Voici un schéma expliquant l'architecture de ce projet, afin de bien comprendre quels sont les différents éléments :

[schéma](doc/schema.pdf)

### Dépendances

Il faut installer les logiciels suivants:

* [QGIS 3.2](https://qgis.org/downloads/)
* [windNinja](https://firelab.org/document/windninja-software)
* [mingw](https://sourceforge.net/projects/mingw/) (cocher MinGW Base System/Autotools)
* [cmake](https://cmake.org/download/) -> faire attention à ce que cmake soit ajouter dans le path

Bien faire attention à laisser les chemins par défaut de ces applications :

* WindNinja : racine de C:
* Qgis : dans ProgramFiles

Ajouter Python dans la variable d'environnement "Path", si vous ne savez pas faire aller [ici](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/).

Faire de même pour mingw et cmake, vérifiez s'ils ont été ajouté dans la variable "Path".

### Installation

* Télécharger les départements, faire attention de bien garder l'architecture mise en place pour les départements, c'est à dire :
  
```batch
C:\users\[username]\NewSantoline\data\departement\XX - nom\tif\
```

* Lancer le fichier **build.bat** dans le dossier **Scripts** : qui permet de build les sous-programmes C++
* Lancer le fichier **run.bat**

### Notice d'utilisation

#### 1. Choix du département

![screen_departement](doc/screens/screen_departements.png)
![screen_departement](doc/screens/departement_v2.png)

#### 2. Carte des Vents

![screen_vents](doc/screens/vents.png)

Placer ensuite un point, qui sera le centre de la carte des vents représenté par une étoile noire.
Une fois le point placé, appuyer sur l'icône du drapeau.

![screen_vents_point](doc/screens/vents_v2.png)

Sélectionner les paramètres souhaités, à noter que :

* la zone est carrée de taille ("distance" x "distance"), _ex : ici zone de 1000 par 1000_
* orientation du vent : voir la chose comme une boussole
* vitesse du vent : plus le vent sera rapide, plus le calcul sera long

![screen_vents_point](doc/screens/settings_vent.png)

En fonction des performances sur votre PC, ce calcul peut être plus ou moins long, prendre le temps de regarder la console ouverte en parallèle pour voir si des problèmes ont été rencontrés. Voici un exemple de ce que vous devriez obtenir, si tout s'est bien passé :

![screen_vents_point](doc/screens/console.png)

Pour afficher la carte des vents, il vous suffit d'appuyer sur le bouton "V" dans l'onglet "Vents".

![screen_vents_point](doc/screens/final_vents.png)

Voici un résultat final que vous devriez obtenir.

#### 3. Simulation des contours de feu

Après avoir calculer les vents, l'étape qui suit est de réaliser une simulation pour calculer le contour des vents :

![screen_vents_point](doc/screens/simulation.png)

Appuyer sur le bouton "Dessiner contours" :

![screen_vents_point](doc/screens/draw_contours_simulation.png)

Dessiner ensuite le contour, chaque clic que vous faites **(ATTENTION LES POINTS QUE VOUS DEVEZ PLACER DOIVENT ETRE DANS LA ZONE DES VENTS)** est un point, et le clic d'après reliera le point précédent avec celui que vous venez de faire, pour alors tracer des segments :

* **Etape 1 :**

![screen_vents_point](doc/screens/etape1_contour.png)

* **Etape 2 :**
  
![screen_vents_point](doc/screens/etape2_contour.png)

* **Etape 3 :**
  
![screen_vents_point](doc/screens/etape3_contour.png)

Continuez ainsi jusqu'à ce que le contour initial soit suffisamment précis. Il est important de noter que chaque segment que vous faites, augmente le temps de calcul final de la simulation.

Si vous commetez une erreur sur le contour, il est possible d'effacer le contour en cours en appuyant sur la croix rouge :

![screen_vents_point](doc/screens/efface_contour.png)

Une fois le contour realisé, il vous suffira de sélectionner les arguments pour finaliser la situation :

![screen_vents_point](doc/screens/simulation_contour_final.png)

Quelques petites informations sur les arguments de la simulation :

* Saisir dans le champ "Algorithme", le numéro de l'algorithme que vous voulez utiliser : 2 ou 3
* Durée de propagation : temps de propagation, si vous dépassez 1h30, les temps de calcul risquent d'être longs
* Les 3 derniers champs ne sont pas utilisés, il n'est pas nécessaire de les remplir

![screen_vents_point](doc/screens/arguments_simulation.png)

Appuyer sur "Valider" et attendez maintenant que la simulation se finisse, en consultant la console pour voir si tout s'est bien passé. Voici un exemple de ce à quoi pourrait ressembler votre console.

![screen_vents_point](doc/screens/console_sim.png)

Et voici un exemple du résultat final, après que la simulation soit terminée.

![screen_vents_point](doc/screens/contour_final_sim.png)
