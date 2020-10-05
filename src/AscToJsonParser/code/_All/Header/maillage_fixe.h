#ifndef __Maillage_Fixe__
#define __Maillage_Fixe__

#include "connector.h"
#include "raster.h"

class MaillageFixe : public Raster {

protected:
  std::vector<Point3D> get_neighbor(Point2D p);

public:
  MaillageFixe(Connector *connector, double duration, int cell_dim);
};

#endif //__Maillage_Fixe__
