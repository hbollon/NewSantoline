/**
* \file stringify.h
* \brief Permet de convertir en chaine de caractères.
* \author Antoine Porcheron-Roche - antoine.pr@live.fr
* \version 0.1
* \date 22 Juillet 2018
*
* Dans ce fichier je définie des fonctions pour faire des conversion d'un 
* objet vers une chaine de caractères.
*
*/

#ifndef __STRINGIFY_CONVERTER__
#define __STRINGIFY_CONVERTER__

#include <vector>
#include <string>

namespace converter {
	/**
	 * Permet de merger les éléments d'un vecteur avec pour séparateur entre chaque
	 * élément étant le separateur.
	 * @param vector Le vecteur à merger.
	 * @param separator Le séparateur entre chaque élément.
	 * @return Le vecteur merger au format chaine de caractères.
	 */
	std::string to_string(std::vector<std::string> vector, char separator);

}

#endif // __STRINGIFY_CONVERTER__