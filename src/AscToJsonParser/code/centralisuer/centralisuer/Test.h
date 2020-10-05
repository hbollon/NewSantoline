#pragma once
#include "DatasHandler.h"

class Test
{
public:
	Test();

	void tester();
}
;
double round(double x, double precision);
Vector3D round(Vector3D v, double precision);
bool compare(Vector3D a, Vector3D b, double precision);
bool compare(double a, double b, double precision);


