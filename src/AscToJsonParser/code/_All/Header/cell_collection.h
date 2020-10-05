#ifndef __CELL_COLLECTION__
#define __CELL_COLLECTION__

#include <vector>

#include "cell.h"
#include "useful.h"
#include "vector.h"

class CellCollection {
  std::vector<Cell> m_cells;
  int m_w;
  int m_h;
  double m_cell_dimension;

public:
  CellCollection();
  CellCollection(int w, int h);
  CellCollection(int w, int h, double cell_dimension);
  CellCollection(int w, int h, double wind_angle, double wind_speed);
  CellCollection(int w, int h, double wind_angle, double wind_speed,
                 double cell_dimension);

  Vector3D get_wind(const Point2D &p) const;
  Vector3D get_slope(const Point2D &p) const;
  double get_aspect(const Point2D &p) const;
  double get_slope_value(const Point2D &p) const;
  bool is_border_point(const Point2D &p) const;
  bool is_angle_point(const Point2D &p) const;
  bool is_center_point(const Point2D &p) const;

private:
  int index(int row, int column) const;
  void set_all_wind(double wind_speed, double wind_angle);

  Cell &at(int i);
  Cell at(int i) const;

  Cell &at(Point2D p);
  Cell at(Point2D p) const;
};

#endif //__CELL_COLLECTION__
