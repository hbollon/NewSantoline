#include "stdafx.h"
#include "AltimetricsDatas.h"
#include "Test.h"
#include "useful.h"

#include <cmath>
#include <fstream>
#include <string>
#include <cmath>

AltimetricsDatas::AltimetricsDatas()
{
}

AltimetricsDatas::~AltimetricsDatas()
{
}

/* La fonction slope renvoie le vecteur pente au point p en 
utilisant les angles 'aspect' et 'slope_angle'.
On defini d'abord x, y et z tel que v(x, y, z) ai pour norme 1 puis on 
calcule la norme et on renvoie enfin le vecteur v * norme.
Reference pour le calcul de vecteur a partir de deux angles:
https://math.stackexchange.com/questions/1385137/calculate-3d-vector-out-of-two-angles-and-vector-length
Reference pour le calcul de la norme:
Forest fire spread modelling in practice
de Mr Yves Dumond
page 4
*/
Vector3D AltimetricsDatas::slope(Point2D p)
{
	/*
	double x = -(sin(aspect(p)) * cos(slope_angle(p)));
	double y = -(cos(aspect(p)) * cos(slope_angle(p)));
	double z = sin(slope_angle(p));
	double norm = 10 * tan(slope_angle(p)) * tan(slope_angle(p));
	if (norm > 10) norm = 10;
	
	return Vector3D(x, y, z) * norm;
	*/
	

	/*
	double a = aspect(p);
	double s = slope_angle(p);

	double x = (sin(a) * 10 * tan(s)*tan(s))/sqrt(1+tan(s)*tan(s));
	double y = (-cos(a) * 10 * tan(s) * tan(s)) / sqrt(1 + tan(s) * tan(s));
	double z = (10 * tan(s) * tan(s) * tan(s)) / sqrt(1 + tan(s) * tan(s));

	return Vector3D(x,y,z);
	
	*/
	Vector3D s = 10 * tanalpha(p) * Vector3D(g(p), h(p), tanalpha(p) * tanalpha(p)).normalized();
		

	if (s.norm() > 10) {
		s = s.normalized() * 10;
	}


	double b = g(p) * s.x() + h(p) * s.y();

	if (!compare(s.z(), b,0.0000001)) {
		std::cout << std::fixed << "le vecteur de pente " << s << "au point "<<p<<" n'est pas dans le plan z ="<<g(p)<<" X + "<<h(p)<<" Y \n";
		std::cout << std::fixed << "z: " << s.z() << " != " << g(p) * s.x() + h(p) * s.y() << "\n";
	}

	return s;

}



/* La fonction slope_angle retourne la valeur de l'angle de la pente en radian
de la cellule situe au point p. Les fonctions g et h sont detaillees plus bas.
ref:
https://www.e-education.psu.edu/natureofgeoinfo/c7_p10.html
*/
double AltimetricsDatas::slope_angle(Point2D p)
{
	//Decommenter " * (180/M_PI); " ci-dessous permet d'obtenir l'angle en degres plutot qu'en radian
	double percent_angle = sqrt(g(p) * g(p) + h(p) * h(p)) * 100;
	double degree_angle = (360 / (2 * M_PI) * 100 * atan(percent_angle / 100))/100;
	double radian_angle = degree_angle * (M_PI / 180);

	return radian_angle; 
}


/* La fonction aspect retourne la valeur de l'orientation de la pente en radian
de la cellule situe au point p. Les fonctions g et h sont detaillees plus bas.
Se referer a:
https://www.asprs.org/wp-content/uploads/pers/1987journal/aug/1987_aug_1109-1111.pdf
Attention ce document presente un calcul en degres.
*/
double AltimetricsDatas::aspect(Point2D p)
{
	if (g(p) == 0)
	{
		return 0;
	}
	else 
	{

		if (g(p) < 0) {
			return atan(h(p) / g(p)) - (M_PI/2) ;
		}
		else {
			return atan(h(p) / g(p)) - (3*M_PI / 2);
		}
		
	}
}

double AltimetricsDatas::get(Point2D p)
{
	double result;
	if (m_datas.find(index(p)) != m_datas.end())
	{
		result = m_datas[index(p)];
	}
	else
	{
		result = 0;
	}
	return result;
}

double AltimetricsDatas::height(Point2D p) {
	return get(p);
};

void AltimetricsDatas::load(std::string path)
{
	
	std::string altimetrie = path+"\\subzone.asc";
	std::ifstream altimetrics(altimetrie);
	std::string tmp;
	double width, height, xcorner, ycorner, size, nodata, data; 
	altimetrics >> tmp >> width >> tmp >> height >> tmp >> xcorner >> tmp >> ycorner >> tmp >> size >> tmp >> nodata;
	for (int y = 0; y < height; ++y) 
	{
		for (int x = 0; x < width; ++x) 
		{
			altimetrics >> data;
			Point2D p(xcorner + x * size, (ycorner + height * size) - (y + 1) * size);
			m_datas[index(p)] = data;
		}
	}
}

std::string AltimetricsDatas::index(Point2D p)
{
	int x = (int)(p.x() - fmod(p.x(), 25));
	int y = (int)(p.y() - fmod(p.y(), 25));
	return std::to_string(x) + "_" + std::to_string(y);
}

/* La fonction g retourne une valeur necessaire au calcul de la pente et de l'aspect.
Elle prend en argument un point p qui corespond a une cellule de la carte des altitudes.
On calcule 'sumEast', la somme des altitudes des point situes a l'est de p.
On calcule 'sumWest', la somme des altitudes des point situes a l'ouest de p.
(pour les 2 sommes precedentes il est important de noter que les valeurs 'east'
pour sumEast et 'west' pour sumWest sont doublees, c'est simplement parce qu'elles 
sont plus importantes)

On retourne ensuite la difference entre sumWest et sumEast divisee par
8 * la largeur d'une cellule.
Ci-dessus '8' corespond au nombre de cellules utilisees pour le calcul.

Pour plus de details voir le liens suivant:
https://www.e-education.psu.edu/natureofgeoinfo/c7_p10.html
*/
double AltimetricsDatas::g(Point2D p)
{

	double northEast = get(p + Point2D(25, 25));
	if (northEast == 0) northEast = get(p);
	double east = get(p + Point2D(25, 0));
	if (east == 0) east = get(p);
	double southEast = get(p + Point2D(25, -25));
	if (southEast == 0) southEast = get(p);

	double northWest = get(p + Point2D(-25, 25));
	if (northWest == 0) northWest = get(p);
	double west = get(p + Point2D(-25, 0));
	if (west == 0) west = get(p);
	double southWest = get(p + Point2D(-25, -25));
	if (southWest == 0) southWest = get(p);

	double sumEast = northEast + 2 * east + southEast;
	double sumWest = northWest + 2 * west + southWest;

	return (sumEast - sumWest) / (8 * 25);

}


/* La fonction h est simillaire a la fonction g (ci-dessus) sur un axe nord/sud.
Elle prend en argument un point p qui corespond a une cellule de la carte des altitudes.
On calcule 'sumNorth', la somme des altitudes des point situes au nord de p.
On calcule 'sumSouth', la somme des altitudes des point situes au sud de p.

On retourne ensuite la difference entre sumNorth et sumSouth divisee par
8 * la largeur d'une cellule.
Pour plus de details voir le liens suivant:
https://www.e-education.psu.edu/natureofgeoinfo/c7_p10.html
*/

double AltimetricsDatas::h(Point2D p)
{

	double northEast = get(p + Point2D(25, 25));
	if (northEast == 0) northEast = get(p);
	double north = get(p + Point2D(0, 25));
	if (north == 0) north = get(p);
	double northWest = get(p + Point2D(-25, 25));
	if (northWest == 0) northWest = get(p);

	double southEast = get(p + Point2D(25, -25));
	if (southEast == 0) southEast = get(p);
	double south = get(p + Point2D(0, -25));
	if (south == 0) south = get(p);
	double southWest = get(p + Point2D(-25, -25));
	if (southWest == 0) southWest = get(p);

	double sumNorth = northEast + 2 * north + northWest;
	double sumSouth = southEast + 2 * south + southWest;

	return (sumNorth - sumSouth) / (8 * 25);

}

double AltimetricsDatas::tanalpha(Point2D p) {
	return sqrt(g(p) * g(p) + h(p) * h(p));
}

double AltimetricsDatas::phi(Point2D p) {
	double cosphi = -h(p) / tanalpha(p);
	double sinphi = g(p) / tanalpha(p);
	if (sinphi < 0) {
		return -acos(-g(p) / tanalpha(p));
	}
	else {
		return acos(-g(p) / tanalpha(p));
	}
}

std::ostream& operator<<(std::ostream& os, const AltimetricsDatas& ad)
{
	for (std::unordered_map<std::string, double>::const_iterator it = ad.m_datas.begin(); it != ad.m_datas.end(); ++it) 
	{
		os << it->first << " = " << it->second << std::endl;
	}
	return os;
}