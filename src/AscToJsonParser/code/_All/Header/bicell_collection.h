#ifndef __BICELL_COLLECTION__
#define __BICELL_COLLECTION__

#include <vector>

#include "cell.h"
#include "useful.h"
#include "vector.h"

class BicellCollection {

public:
  BicellCollection();
  BicellCollection(int cell_dim);
  BicellCollection(int ws, double wa);
  BicellCollection(double ws, double wa, int cell_dim);
  Vector3D get_wind(Point2D p) const;
  Vector3D get_slope() const;
  double get_aspect() const;
  double get_slope_value() const;
  double& cut_position();
  double cut_position() const;
  bool& vertical_cut();
  bool vertical_cut() const;
  Vector3D& second_wind();
  Vector3D second_wind() const;
  bool is_border_point(const Point2D &p) const;
  bool is_angle_point(const Point2D &p) const;
  bool is_center_point(const Point2D &p) const;

private:
  void set_all_wind(double wind_speed, double wind_angle);
  int m_cell_dimension;
  Vector3D m_wind;
  double m_cut_position;
  bool m_vertical_cut;
  Vector3D m_second_wind;
};

#endif //__BICELL_COLLECTION__
