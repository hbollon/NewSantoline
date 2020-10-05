#ifndef __CELL__
#define __CELL__
#include "vector.h"
class Cell {
  Vector3D m_wind;
  Vector3D m_slope;

  double m_aspect;
  double m_slope_value;

public:
  Cell();
  Cell(Vector3D wind, Vector3D slope, double aspect, double slope_value);

  Vector3D &get_wind();
  Vector3D get_wind() const;

  Vector3D &get_slope();
  Vector3D get_slope() const;

  double &get_aspect();
  double get_aspect() const;

  double &get_slope_value();
  double get_slope_value() const;
};

#endif //__CELL__
