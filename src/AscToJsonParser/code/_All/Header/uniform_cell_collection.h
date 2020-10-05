#ifndef __UNIFORM_CELL_COLLECTION__
#define __UNIFORM_CELL_COLLECTION__

#include <vector>

#include "useful.h"
#include "vector.h"

class UniformCellCollection {

public:
  UniformCellCollection();
  UniformCellCollection(int cell_dim);
  UniformCellCollection(int ws, double wa);
  UniformCellCollection(double ws, double wa, int cell_dim);
  Vector3D get_wind() const;
  Vector3D get_slope() const;
  double get_aspect() const;
  double get_slope_value() const;
  bool is_border_point(const Point2D &p) const;
  bool is_angle_point(const Point2D &p) const;
  bool is_center_point(const Point2D &p) const;

private:
  void set_all_wind(double wind_speed, double wind_angle);

  int m_cell_dimension;
  Vector3D m_wind;
};

#endif //__UNIFORM_CELL_COLLECTION__
