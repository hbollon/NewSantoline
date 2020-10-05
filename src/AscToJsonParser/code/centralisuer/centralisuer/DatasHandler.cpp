#include "stdafx.h"
#include "DatasHandler.h"
#include "Test.h"
#include "AltimetricsDatas.h"

#include <fstream>
#include <string>
#include <algorithm>

#include <exception>

DatasHandler::DatasHandler() : m_temperature(30), m_waterLevel(40)
{
}

DatasHandler::~DatasHandler()
{
}

AEllipse DatasHandler::get_dataset(Point2D p)
{
	return AEllipse(get_wind(p), get_slope(p), get_aspect(p), get_slope_value(p), m_temperature, m_waterLevel, p);
}

Vector3D DatasHandler::get_wind(Point2D p)
{
	return m_datas[index(p)].wind;
}

Vector3D DatasHandler::get_slope(Point2D p)
{
	return m_datas[index(p)].slope;
}

double DatasHandler::get_aspect(Point2D p)
{
	return m_datas[index(p)].aspect;
}

double DatasHandler::get_slope_value(Point2D p)
{
	return m_datas[index(p)].slope_angle;
}

bool DatasHandler::is_center(Point2D p)
{
	return int(fmod((p + Point2D(12.5, 12.5)).x(), 25)) == 0 && int(fmod((p + Point2D(12.5, 12.5)).y(), 25)) == 0;
}

bool DatasHandler::is_border(Point2D p)
{
	return int(fmod(p.x(), 25)) == 0 || int(fmod(p.y(), 25)) == 0;
}

bool DatasHandler::is_angle(Point2D p)
{
	return int(fmod(p.x(), 25)) == 0 && int(fmod(p.y(), 25)) == 0;
}

Vector3D wind(double angle, double speed, double z) {
	Vector2D temps(0, 1);
	temps = temps.rotate(fmod(angle, 360)) * speed;
	return Vector3D(temps.x(), temps.y(),z);
}


/*La fonction "load" charge les donnees contenues dans les fichiers wind_speed et wind_angle
(nommes comme ci apres) dans l'attribut 'm_datas' de l'objet DatasHandler.
L'attribut m_data est une "unordered_map" qui reference pour chaque point de la 
futur carte des vents les donnes:
vecteur vent ,
vecteur pente, 
vecteur vent/pente, 
aspect de la pente,
angle de la pente ,
coordonne x ,
coordonne y  */
void DatasHandler::load(int angle, int speed, int size,std::string path,std::string test)
{
	std::string wind_speed;
	std::string wind_angle;
	AltimetricsDatas ad;
	if (test == "wind") {
		ad.load(path+"\\Test\\testWind");
		wind_speed = path + "\\Test\\windTest\\vel.asc";
		wind_angle = path + "\\Test\\windTest\\ang.asc";
	}
	else if(test=="slope"){
		ad.load(path + "\\Test\\slopeTest");
		wind_speed = path + "\\Test\\slopeTest\\vel.asc";
		wind_angle = path + "\\Test\\slopeTest\\ang.asc";
	}
	else{
		ad.load(path);
		wind_speed = path+"\\subzone_" + std::to_string(angle) + "_" + std::to_string(speed) + "_" + std::to_string(size) + "m_vel.asc";
		wind_angle = path+"\\subzone_" + std::to_string(angle) + "_" + std::to_string(speed) + "_" + std::to_string(size) + "m_ang.asc";
	}
	
	std::string tmp;
	int width, height; 
	double xcorner, ycorner, dsize, nodata;
	std::ifstream speed_file(wind_speed);
	if (!speed_file.is_open()) 
	{
		std::cout << "speed file not opened";
		throw std::exception(("Erreur fichier " + wind_speed).c_str());
	}
	speed_file >> tmp >> width >> tmp >> height >> tmp >> xcorner >> tmp >> ycorner >> tmp >> dsize >> tmp >> nodata;
	std::ifstream angle_file(wind_angle);
	if (!angle_file.is_open()) 
	{
		std::cout << "angle file not opened";
		throw std::exception(("Erreur fichier " + wind_angle).c_str());
	}
	angle_file >> tmp >> width >> tmp >> height >> tmp >> xcorner >> tmp >> ycorner >> tmp >> dsize >> tmp >> nodata;
	double dspeed, dangle;

	int windtest = 0;

	for (int x = 0; x < height; ++x) 
	{
		for (int y = 0; y < width; ++y) 
		{
			speed_file >> dspeed;
			angle_file >> dangle;

			Point2D p(xcorner + x * dsize, (ycorner + height * size) - (y + 1) * dsize);

			double aspect = ad.aspect(p);
			double slope = ad.slope_angle(p);

			Vector3D slope1 = ad.slope(p);
			if (test == "wind") {
				if (!(slope1 == Vector3D(0,0,0))) {
					std::cout << "Le terrain est plat mais le vecteur pente vaut: " << slope1 << std::endl;
				}
			}
			Vector3D wind1 = wind(dangle, dspeed,0);
			double norm = wind1.norm();
			double vz = ad.g(p) * wind1.x() + ad.h(p) * wind1.y();
			wind1 = Vector3D(wind1.x(),wind1.y(),vz);
			wind1 = wind1.normalized()*norm;

			if (test == "slope") {
				if (!(wind1 == Vector3D(0, 0, 0))) {
					std::cout << "Il n'y a pas de vent mais le vecteur vent vaut: " << wind1 << std::endl;
				}
			}
			Vector2D plan = Vector2D(ad.g(p),ad.h(p));
			Vector3D windSlope = Vector3D(
				wind1.x()+slope1.x(),
				wind1.y()+slope1.y(),
				wind1.z()+slope1.z());
			
			if (test != "none") {
				double vx = cos(fmod((dangle + 90), 360) * M_PI / 180.0);
				double vy = sin(fmod((dangle + 90), 360) * M_PI / 180.0);	
				vz = ad.g(p) * vx + ad.h(p) * vy;

				Vector3D vtest = Vector3D(vx, vy, vz).normalized()*norm;

				if (!(compare(wind1, vtest,0.0000001))&&!(wind1==Vector3D(0,0,0))) {
					std::cout << std::fixed << "Le vecteur vent " << wind1 << " devrait valoir : " << vtest << std::endl;
				}
				double b = ad.g(p) * wind1.x() + ad.h(p) * wind1.y();
				if (!compare(wind1.z(),b,0.000001)) {
					std::cout << std::fixed << "le vecteur vent " << wind1 << "au point " << p << " n'est pas dans le plan z =" << ad.g(p) << " X + " << ad.h(p) << " Y \n";
					std::cout << std::fixed << "z: " << wind1.z() << " != " << ad.g(p) * wind1.x() + ad.h(p) * wind1.y() << "\n";
				}
				b = ad.g(p) * windSlope.x() + ad.h(p) * windSlope.y();
				if (!compare(windSlope.z(), b, 0.000001)) {
					std::cout << std::fixed << "le vecteur vent/pente " << windSlope << "au point " << p << " n'est pas dans le plan z =" << ad.g(p) << " X + " << ad.h(p) << " Y \n";
					std::cout << std::fixed << "z: " << windSlope.z() << " != " << ad.g(p) * windSlope.x() + ad.h(p) * windSlope.y() << "\n";
				}
			}

			m_datas[index(p)] = {wind1, slope1,windSlope, aspect, slope,p.x() ,p.y(),ad.height(p),plan  };
			windtest++;
		}
	}
	speed_file.close();
	angle_file.close();
}

std::string DatasHandler::index(Point2D p)
{
	return std::to_string(int(p.x() - fmod(p.x(), 25))) + "_" + std::to_string(int(p.y() - fmod(p.y(), 25)));
}

/* Cette fonction renvoie ce qui doit etre ecrit dans le fichier json pour chaque cellule.
Il faut donc respecter scrupuleusement la syntaxe du format json.*/
std::ostream& operator<<(std::ostream& os, const Datas& d)
{
	

	double sx = d.slope.x();
	double sy = d.slope.y();
	double sz = d.slope.z();
	
	double wx = d.wind.x();
	double wy = d.wind.y();
	double wz = d.wind.z();

	double wsx = d.windSlope.x();
	double wsy = d.windSlope.y();
	double wsz = d.windSlope.z();
	
	return os
		<< "{ \"slope\" : " << d.slope_angle << ","
		<< " \"slope_vector\" : "
		<< "[" << sx << "," << sy << "," << sz << "]" << ","
		<< " \"plan\" : "
		<< "[" << d.plan.x() << "," << d.plan.y() << "]" << ","
		<< " \"aspect\" : " << d.aspect << ","
		<< "\"x\":" << std::fixed << d.x << ","
		<< "\"y\":" << std::fixed << d.y << ","
		<< "\"z\":" << std::fixed << d.z << ","
		<< "\"wind\" : " << "[" << wx << "," << wy << "," << wz <<"]"<<","
		<<"\"windSlope\" : "<<"[" << wsx << "," << wsy << "," << wsz << "]}";
}

/* Cette fonction est appelee pour ecrire les informations contenues dans une instance de 'DatasHandler'
dans un fichier json, elle appelle pour chaque cellule la fonction ci-dessus*/
std::ostream& operator<<(std::ostream& os, const DatasHandler& dh)
{
	std::cout << "write machin \n";

	os << "[";

	for (auto it = dh.m_datas.begin(); it != --dh.m_datas.end(); ++it)
	{
		os << it->second<< "," << std::endl ;
	}
	os << (--dh.m_datas.end())->second << "]";
	
	return os;
}

