#ifndef __Uniform_Connector__
#define __Uniform_Connector__

#include "uniform_cell_collection.h"
#include "connector.h"
#include "ellipse.h"

class UniformConnector : public Connector {
  
  UniformCellCollection m_cells;

public:
  UniformConnector();
  UniformConnector(UniformCellCollection cells);

  AEllipse get_dataset(Point2D p);

  Vector3D get_wind(Point2D p);
  Vector3D get_slope(Point2D p);

  double get_aspect(Point2D p);
  double get_slope_value(Point2D p);

  bool is_center(Point2D p);
  bool is_border(Point2D p);
  bool is_angle(Point2D p);
};

#endif //__Uniform_Connector__
