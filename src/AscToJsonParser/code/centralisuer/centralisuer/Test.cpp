#include "stdafx.h"
#include "Test.h"

Test::Test()
{
}

void Test::tester()
{
	DatasHandler d = DatasHandler();
	std::cout << "Test" << std::endl;
	d.load(0, 0, 25, "C:\\Users\\Skaldr\\Desktop\\NewSantolineCedric\\newArchitecture\\Data\\", "wind");
	std::cout << "Wind Test Done" << std::endl;
	d.load(0, 0, 25, "C:\\Users\\Skaldr\\Desktop\\NewSantolineCedric\\newArchitecture\\Data\\", "slope");
	std::cout << "Slope Test Done" << std::endl;
}

double round(double x, double precision)
{
	return (int)(x * precision) / precision;
}

bool compare(double a, double b, double precision)
{
	return abs(a - b) < precision;
}

bool compare(Vector3D a, Vector3D b, double precision)
{
	return compare(a.x(), b.x(), precision) && compare(a.y(), b.y(), precision) && compare(a.z(), b.z(), precision);
}

Vector3D round(Vector3D v, double precision)
{
	return Vector3D(round(v.x(), precision), round(v.y(), precision), round(v.z(), precision));
}
