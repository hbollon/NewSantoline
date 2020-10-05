#ifndef __USEFUL__
#define __USEFUL__

#ifndef M_PI 
#define M_PI 3.14159265358979323846
#endif // M_PI
//seriously windows, what the actual fuck? when did you come up with the idea that PI is useless?

#include "vector.h"

typedef Vector2D Point2D;
typedef Vector3D Point3D;

/**
 * Permet d'effectuer la conversion de valeurs en degrés vers des radians.
 * @param degrees la valeur en degrés que l'on souhaite convertir en radian
 */
double deg_to_rad(double degrees);

/**
 * Permet d'effectuer la conversion de valeurs en radian vers des degrés.
 * @param radian la valeur en radians que l'on souhaite conertir en degrés
 */
double rad_to_deg(double radian);

/**
 * Permet de determiner si il y a une intersection entre deux demi-droites, et
 * si c'est le cas, de trouver le point d'intersection entre les deux
 * demi-droites.
 * @param o point d'origine de la première demi-droite
 * @param dir direction de la première demi-droite
 * @param a point d'origine de la seconde demi-droite
 * @param dir_a direction de la seconde demi-droite
 * @param result point resultant de l'intersection si celle-ci a lieu
 * @return un boolean qui indique vrai s'il existe une intersection, faux sinon.
 */
bool intersect(Point2D o, Vector2D dir, Point2D a, Vector2D dir_a, Point2D &result);

/**
 * Fait une interpolation linéaire entre A et B. Celà correspond à faire ( 1 - c ) * a + c * b
 * Pour C compris dans l'intervalle [0, 1] cela retourne une valeur comprise entre a et b.
 * @param a valeur minimal de l'interpolation linéaire
 * @param b valeur maximal de l'interpolation linéaire
 * @param c "position" de l'interpolation linéaire
 * @return retourne ( 1 - c ) * a + c * b
 */
double lerp(double a, double b, double c);

/**
 * Projette un point 3dimension en un point 2 dimension ( retire juste la troisième composante)
 * @param p le point à projeter
 * @return le point projeter
 */ 
Point3D from_3D_to_2D(Point2D p);

/**
 * Projette tout un ensemble de points 3dimensions en points 2dimensions
 * @param points tableau de points  que l'on souhaite projeter
 * @result tableau de points projeté
 */
std::vector<Point2D> from_3D_to_2D(std::vector<Point3D> points);

#endif //__USEFUL__
