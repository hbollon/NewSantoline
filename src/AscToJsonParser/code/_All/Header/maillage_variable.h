#ifndef __Maillage_Variable__
#define __Maillage_Variable__

#include "raster.h"

class MaillageVariable : public Raster {

protected:
	virtual std::vector<Point3D> get_neighbor(Point2D p);
	std::vector<Point2D> arcs(const Point2D& p, const AEllipse& e);

public:
  MaillageVariable(Connector *connector, double duration, int cell_dim);
  std::vector<Point3D> propagation();
};
#endif //__Maillage_Variable__
