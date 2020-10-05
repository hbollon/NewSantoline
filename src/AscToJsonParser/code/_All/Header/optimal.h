#ifndef _OPTIMALH
#define _OPTIMALH

#include <iostream>
#include <math.h>
#include <vector>

#include "connector.h"
#include "ellipse_projetee.h"
#include "vectorielle.h"

class Optimal : public Vectorielle {
private:
  std::vector<Point2D> contour;
  double step_time;
  int discretisation;
    bool AFFICHAGE = false;

public:
  Optimal(Connector *connector, double duration, int cell_dim,
          std::vector<Point2D> contour_arg, double step_time_arg,
          int discretisation);

  std::vector<Point2D> get_contour();
  void set_contour(std::vector<Point2D> new_contour);
  double get_step_time();
  void set_step_time(double new_step_time);
  int get_discretisation();
  void set_discretisation(int new_discretisation);
  EllipseProjetee parametres_ellipse(AEllipse e);
  void point_sur_front(EllipseProjetee e, Point2D m, Vector2D t_new,double delta_temps, Point2D &m_new);
  void indice_cellule(Point2D m, Vector2D t, Point2D &Pi_old, Point2D &Pi_new);
  void valeurs_critiques(EllipseProjetee e, Vector2D t, double &a_minus,double &a_plus);
  void nabla_evtau_nablaevv(EllipseProjetee e, Vector2D t, Vector2D v, double &a);
  void suivant(Point2D m_old, Vector2D v, Point2D &m_new, Vector2D &tau, double &time_new);
  void vitesse_max(EllipseProjetee e, Vector2D v, Vector2D &V);
  void vitesse_optimale(EllipseProjetee e, Vector2D t, Vector2D &V);
  void vitesse_choc(EllipseProjetee e, Vector2D t_minus, Vector2D t_plus,Vector2D &v_choc);
  void v_new(EllipseProjetee e, double a, Vector2D t, Vector2D &v_new);
  void new_vitesse_optimale(EllipseProjetee e_old, EllipseProjetee e_new,Vector2D tau, Vector2D v_old, Vector2D &v_new);
  void tangente_opt(EllipseProjetee e, Vector2D v, Vector2D &t);
  void trajectoire_opt(Point2D m, Vector2D t, double delta_temps, Point2D &m_new,Vector2D &t_new, double &time_new);
  std::vector<Point2D> propagation_1step();
  std::vector<Point3D> propagation();
};

#endif