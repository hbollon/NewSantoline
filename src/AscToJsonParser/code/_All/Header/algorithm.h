#ifndef __ALGORITHM__
#define __ALGORITHM__

#include <vector>

#include "connector.h"

class Algorithm {

protected:
  Connector *m_connector;

  std::vector<Point2D> m_starting_points;

  double m_simulation_duration;

  int m_cell_dim;

public:
  Algorithm(Connector *connector, double duration, int cell_dim);

  virtual std::vector<Point3D> propagation() = 0;

  double &get_simulation_duration();
  double get_simulation_duration() const;

  void add_starting_point(Point2D p);

  std::vector<Point2D> &get_starting_points();
  std::vector<Point2D> get_starting_points() const;
};

#endif //__ALGORITHM__
