#include "ellipse.h"

AEllipse::AEllipse(const Vector3D& wind,
                   const Vector3D& slope,
                   const Vector3D& windSlope,
                   const double& aspect,
                   const double& slope_value,
                   const double& exentricite,
                   const double& rateOfSpread,
                   const double& temp, const double& swl, const Point2D& top_left, const Point2D& plan)
    : m_wind(wind), m_slope(slope), m_windSlope(windSlope), m_aspect(aspect),
      m_slope_value(slope_value), m_exentricite(exentricite), m_rateOfSpread(rateOfSpread),
      m_temperature(temp), m_water_level(swl), m_plan(plan),
      m_top_left(Point2D((int)(top_left.x() - fmod(top_left.x(), 25.0)), (int)(top_left.y() - fmod(top_left.y(), 25.0))))
{
}

AEllipse::AEllipse(const Vector3D& wind,
                   const Vector3D& slope,
                   const Vector3D& windSlope,
                   const double& aspect,
                   const double& slope_value,
                   const double& exentricite,
                   const double& rateOfSpread,
                   const Point2D& top_left,
                   const Point2D& plan)
    : m_wind(wind), m_slope(slope), m_windSlope(windSlope), m_aspect(aspect),
      m_slope_value(slope_value), m_exentricite(exentricite), m_rateOfSpread(rateOfSpread),
      m_temperature(30), m_water_level(40), m_plan(plan),
      m_top_left(Point2D((int)(top_left.x() - fmod(top_left.x(), 25.0)), (int)(top_left.y() - fmod(top_left.y(), 25.0))))
{
    
}

AEllipse::AEllipse() {}

Vector3D AEllipse::get_wind() const
{
    return m_wind;
}

Vector3D AEllipse::get_slope() const
{

    return m_slope;
}

double AEllipse::get_aspect() const { return m_aspect; }

double AEllipse::get_slope_value() const { return m_slope_value; }

double AEllipse::get_slope_norm() const
{
    return (10.0 * tan(get_slope_value()) * tan(get_slope_value()) < 10.0 ? 10.0 * tan(get_slope_value()) * tan(get_slope_value()) : 10.0);
}

double AEllipse::time(const Point2D &a, const Point2D &b) const
{
    Vector3D ab(get_3d(b - a));
    return 3600 * ab.norm() / get_rc_norm(get_cos_theta(ab));
}

Vector3D AEllipse::get_c() const
{
    return m_windSlope;
}

Vector3D AEllipse::get_rc() const
{
    return (get_c().norm() == 0 ? Vector3D() : (get_c() / get_c().norm()) * get_rc_norm());
}

double AEllipse::get_lb() const
{
    return m_exentricite * (1.0 + 0.0012 * pow(2.237 * get_c().norm(), 2.154));
}

double AEllipse::get_e() const { return sqrt(1 - (1 / pow(get_lb(), 2))); }

double AEllipse::get_rc_norm() const
{
    return m_rateOfSpread * (180.0 * exp(0.06 * m_temperature) *
                             tanh((100.0 - m_water_level) / 150.0) *
                             (1.0 + 2.0 * (0.8483 + tanh(((get_c().norm() * 3.6) / 30.0) - 1.25))));
}

double AEllipse::get_rc_norm(double cos_theta) const
{
    return ((1 - get_e()) / (1 - get_e() * cos_theta)) * get_rc_norm();
}

double AEllipse::get_cos_theta(Vector3D ab) const
{
    if (ab.norm() == 0 || get_rc_norm() == 0)
        // si la norme de ab == 0, il y a probablement une erreur, si rc_norm == 0,
        // on est dans un cas parfaitement circulaire ( la somme du vecteur vent et
        // pente donne 0 ), et donc quelque soit la direction dans laquel on va, on
        // ira toujours à la même vitesse.
        return 0.0;
    return ab.dot(get_rc()) / (ab.norm() * get_rc_norm());
}

Point3D AEllipse::get_3d(const Point2D &p) const
{
    Point3D result;
    result.x() = p.x();
    result.y() = p.y();
    result.z() = p.x() * m_plan.x() + p.y() * m_plan.y();
    return result;
}

Point2D AEllipse::get_topleft() const
{
    return m_top_left;
}
