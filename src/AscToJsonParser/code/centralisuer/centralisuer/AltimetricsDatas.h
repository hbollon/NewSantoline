#pragma once
#include "useful.h"
#include <unordered_map>

class AltimetricsDatas
{
public:
	AltimetricsDatas();
	~AltimetricsDatas();
	Vector3D slope(Point2D p);
	double slope_angle(Point2D p);
	double aspect(Point2D p);
	double get(Point2D p);
	double height(Point2D p);
	void load(std::string path);
	friend std::ostream &operator<<(std::ostream &os, const AltimetricsDatas &ad);
	double g(Point2D p);
	double h(Point2D p);

private:
	std::string index(Point2D p);

	double tanalpha(Point2D p);
	double phi(Point2D p);
	std::unordered_map<std::string, double> m_datas;
};