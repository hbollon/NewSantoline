# Projet Santoline

## Récupérer le projet

### Dépendances

Il faut installer les logiciels suivants:
* [QGIS 3.2](https://qgis.org/downloads/)
* [windNinja](https://firelab.org/document/windninja-software)

### Le récupérer sur sa machine en local

Avec un terminal, tapez la commande:

```git clone git@gitlab.com:alascenci/newsantoline.git```

Si vous avez l'erreur suivante:

Permission denied (publickey).
Fatal: Could not read from remote repository.

Vous avez sûrement oublié de créer une clé SSH.
Veuillez vous reportez à [la page d'aide pour générer une clé SSH](https://gitlab.com/help/ssh/README#generating-a-new-ssh-key-pair).

### Configuration

Actuellement, le projet n'est pas opérationnel. Vous aurez beau tourner votre ordinateur dans tous les sens, ça ne marchera pas.

Au lieu de vous appitoyer sur votre sort, nous vous conseillons de suivre les étapes suivantes.

#### Copie de fichiers

Il faut **créer une copie** des fichiers suivants, avec un **nouveau nom**:
* paths.copy.json -> paths.json
* data/communication/parametreAlgo.copy.json -> data/communication/parametreAlgo.json
* data/maps/map-copy.json -> data/maps/map.json
* Algo3/cmake-build-debug-copy -> Algo3/cmake-build-debug
* src/algo/cmake-build-debug-copy -> src/algo/cmake-build-debug

Ces fichiers dépendent de l'ordinateur et l'on a choisi de les ignorer (voir .gitignore), afin d'éviter de les intégrer dans de futurs commits.

#### Changer les liens

Les fichiers suivants contiennent des liens absolus, à changer selon la configuration de votre machine:
* paths.json
* SantolinePyQT5/main.py
* src/algo/main.cpp (pour l'instant, le lien n'est pas vraiment utile)
* save/run.bat

### Choix sur l'IDE

Il faut générer la carte des vents depuis l'interface avant de générer un exécutable, et il faut aussi penser à compiler les algorithmes avant de lancer une simulation. Cette dernière se contente d'exécuter le dernier exécutable généré.

C'est à cet endroit qu'est précisée l'exécution des algorithmes:

* [Ceci est un lien (promis, c'est pas un truc louche).](SantolinePyQT5/core/controller/algorithm_controller.py#L88)

Nous utilisions et vous conseillons **CLion** pour bosser sur le C++.

On avait juste besoin d'ajouter les fichiers utilisés par l'algorithme en question dans la configuration de débogage/lancement.

Pour le Python, c'était **PyCharm**.

Lorsqu'il s'agissait de visualiser et/ou de faire des modifications sur l'ensemble du projet, un IDE plus léger comme **Visual Studio Code** est conseillé.

Après, à vous de voir. On ne viendra pas vous fouetter, promis.

#### Petite astuce

Si vous trouvez compliqué le fait d'avoir à gérer autant d'IDE à la fois, Jetbrains propose un outil de gestion d'IDE.

https://www.jetbrains.com/toolbox-app/

## En savoir plus

Nous avons créé quelques jolis petits tutoriels pour que vous puissiez bien démarrer.
Ils sont accessibles au lien suivant:
https://docs.google.com/document/d/1veqeDs8Zyw250rieCrRZwh_tw0p3qs4MB4GoI7dEW5c/edit?usp=sharing

Si vous avez d'autres questions, n'hésitez pas à nous contacter.