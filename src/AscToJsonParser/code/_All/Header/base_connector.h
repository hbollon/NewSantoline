#ifndef __BaseConnector__
#define __BaseConnector__

#include "cell_collection.h"
#include "connector.h"
#include "ellipse.h"

class BaseConnector : public Connector {

  CellCollection m_cells;

public:
  BaseConnector();
  BaseConnector(CellCollection cells);

  AEllipse get_dataset(Point2D p);

  Vector3D get_wind(Point2D p);
  Vector3D get_slope(Point2D p);

  double get_aspect(Point2D p);
  double get_slope_value(Point2D p);

  bool is_center(Point2D p);
  bool is_border(Point2D p);
  bool is_angle(Point2D p);
};

#endif //__BaseConnector__
