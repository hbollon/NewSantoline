#ifndef __RASTER__
#define __RASTER__

#include "algorithm.h"
#include "useful.h"

class Raster : public Algorithm {

protected:
  std::vector<Point3D> m_all;
  std::vector<Point3D> m_burning;
  std::vector<Point3D> m_vicinity;
  std::vector<Point3D> m_others;

  virtual std::vector<Point3D> get_neighbor(Point2D p) = 0;

public:
  Raster(Connector *connector, double duration, int cell_dim);

  std::vector<Point3D> propagation();
};

#endif //__RASTER__
