#ifndef __DISPLAYER__
#define __DISPLAYER__

#include "color.h"
#include "image.h"

#define DIV 1
#define COLOR_SWITCH false



/**
 * Fonction dont le but est de definir l extremité de la map
 * @param v listes de tout les points de la map contenus dans le fichier
 * @param min_X minimum en X qui sera retourné
 * @param max_X maximum en X qui sera retourné
 * @param min_Y minimum en Y qui sera retourné
 * @param max_X maxiumum en Y qui sera retourné
 **/
void extrem(std::vector<Point3D> v, double& min_X, double& max_X, double& min_Y, double& max_Y);

/**
 * Fonction qui retourne une couleur à partir d'un point 3dimension, et d'une valeur maximal
 * @param p point d'entrée
 * @param max temps maximal parmis les points
 * @return une couleur
 */
Color color(const Point3D& p, double max);

/**
 * Trace l'enveloppe de points sur une image à partir d'un ensemble de points
 * @param image image sur laquel on va dessiner l'enveloppe
 * @param hull enveloppe à tracer sur l'image
 */
void drawHull(Image& image, const std::vector<Point2D>& hull, const Color& c );
void drawHull(Image& image, const std::vector<Point2D>& hull, int thickness, const Color& c );


/**
 * Lit un ensemble de points à partir d'un fichier, en appliquant un multiplieur sur les points lu
 * @param filename nom du fichier à lire
 * @param modifieur vecteur par lequel on va multiplier chaque point.
 * @return les points lu modifier
 */
std::vector<Point3D> readfile(std::string filename, Vector3D modifieur);

/**
 * Ecrit une image dans un fichier
 * @param filename fichier de sortie
 * @param image image que l'on souhaite sortir
 */
void writeFile(std::string filename, const Image& img);
#endif //__DISPLAYER__
