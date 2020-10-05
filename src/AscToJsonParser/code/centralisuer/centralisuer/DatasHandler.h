#pragma once
#include <unordered_map>
#include "useful.h"
#include "ellipse.h"

struct Datas 
{
	Vector3D wind;
	Vector3D slope;
	Vector3D windSlope;
	double aspect;
	double slope_angle;
	double x;
	double y;
	double z;
	Vector2D plan;
};

std::ostream& operator<<(std::ostream& os, const Datas& d);

class DatasHandler
{
public:
	DatasHandler();
	~DatasHandler();

	virtual AEllipse get_dataset(Point2D p);

	virtual Vector3D get_wind(Point2D p);
	virtual Vector3D get_slope(Point2D p);

	virtual double get_aspect(Point2D p);
	virtual double get_slope_value(Point2D p);

	virtual bool is_center(Point2D p);
	virtual bool is_border(Point2D p);
	virtual bool is_angle(Point2D p);

	void load(int angle, int speed, int size,std::string path,std::string test);

	friend std::ostream& operator<<(std::ostream& os, const DatasHandler& dh);

private:
	std::string index(Point2D p);
	double m_temperature;
	double m_waterLevel;

	std::unordered_map<std::string, Datas> m_datas;
};

