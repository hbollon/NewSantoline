#ifndef __EllipseProjetee__
#define __EllipseProjetee__

#include "useful.h"
#include "vector.h"

class EllipseProjetee {

	//TODO check if its okay to have put all in 2D
	Vector2D m_e;
	Vector2D m_eperp;

  Point2D m_c;

  double m_a;
  double m_b;

public:
  EllipseProjetee();
  EllipseProjetee(Vector2D e, Point2D c, double a, double b);

  Vector2D get_e_vec() const;
  Vector2D get_eperp() const;

  Point2D get_c() const;

  double get_a() const;
  double get_b() const;
};

#endif //__EllipseProjetee__
