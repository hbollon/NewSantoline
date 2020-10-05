La fonction main se situe dans le fichier Centraliseur.cpp.

Pour gérer les différents type de message, j'utilise le patron de conception chaîne de responsabilité.
La fonction qui se charge de gérer la reception et le passage à la chaîne de responsabilité est la fonction "run" dans le fichier Handler.cpp

Si vous ne voulez pas utiliser les pipes (j'ai cru comprendre qu'y'avais des soucis avec) il faut hériter et spécialiser de la classe Reader, et modifier le main en conséquence (l'instanciation du handler)

Pour le lien avec des librairies externe, je sais plus exactement lesquels sont concerné, donc dans le doute, il faut lié avec toute les librairies dans le dossier "_All", et inclure tout les headers. 

