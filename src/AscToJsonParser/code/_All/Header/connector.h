#ifndef __Connector__
#define __Connector__

#include "ellipse.h"

class Connector {
public:
  virtual AEllipse get_dataset(Point2D p) = 0;

  virtual Vector3D get_wind(Point2D p) = 0;
  virtual Vector3D get_slope(Point2D p) = 0;

  virtual double get_aspect(Point2D p) = 0;
  virtual double get_slope_value(Point2D p) = 0;

  virtual bool is_center(Point2D p) = 0;
  virtual bool is_border(Point2D p) = 0;
  virtual bool is_angle(Point2D p) = 0;
};

#endif //__Connector__
