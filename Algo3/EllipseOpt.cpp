//
// Created by Skaldr on 7/12/2019.
//

#include "EllipseOpt.hpp"

EllipseOpt::EllipseOpt(Point2D p, Vector3D rateOfSpread, float epsilon, float alpha, float phi) : m_coordonne(p)
{

    double phi_c = phi * M_PI / 180;
    double alpha_c = alpha * M_PI / 180;

    Vector3D plan = Vector3D(sin(phi_c) * sin(alpha_c), -cos(phi_c) * sin(alpha_c), cos(alpha_c));

    double testPlan = rateOfSpread.dot(plan);
    if (testPlan != 0)
    {
        rateOfSpread = Vector3D(rateOfSpread.x(), rateOfSpread.y(), rateOfSpread.z() - testPlan / cos(alpha_c));
    }

    Vector3D petitAxe = Vector3D((rateOfSpread.y() * plan.z()) - (rateOfSpread.z() * plan.y()),
                                 (rateOfSpread.z() * plan.x()) - (rateOfSpread.x() * plan.z()),
                                 (rateOfSpread.x() * plan.y()) - (rateOfSpread.y() * plan.x()));
    // pointIgnition
    Point3D pointIgnition3D = Point3D(-(epsilon / (1 + epsilon)) * rateOfSpread.x(),
                                      -(epsilon / (1 + epsilon)) * rateOfSpread.y(),
                                      -(epsilon / (1 + epsilon)) * rateOfSpread.z());

    Point2D pointIgnition2D = Point2D(pointIgnition3D.x(), pointIgnition3D.y());

    Vector2D vecteurAuxPetit = Vector2D(sqrt((1 - epsilon) / (1 + epsilon)) * petitAxe.x(),
                                        sqrt((1 - epsilon) / (1 + epsilon)) * petitAxe.y());
    Vector2D vecteurAuxGrand = Vector2D(1 / (1 + epsilon) * rateOfSpread.x(),
                                        1 / (1 + epsilon) * rateOfSpread.y());

    double aux1 = vecteurAuxPetit.dot(vecteurAuxPetit) - vecteurAuxGrand.dot(vecteurAuxGrand);
    double aux2 = vecteurAuxPetit.dot(vecteurAuxGrand);

    // cas du cercle
    if (abs(aux1) <= 0.0000001)
    {
        double normr = vecteurAuxPetit.norm();
        m_coordonne = Point2D(0, 0);
        m_vecteur = Vector2D(1, 0);
        m_a = normr;
        m_b = normr;
    }
    else
    {
        double theta = atan(aux2 / aux1) / 2;

        Vector2D ax1 = cos(theta) * vecteurAuxGrand + sin(theta) * vecteurAuxPetit;
        Vector2D ax2 = -sin(theta) * vecteurAuxGrand + cos(theta) * vecteurAuxPetit;

        double normax1 = ax1.norm();
        double normax2 = ax2.norm();

        ax1 = ax1.normalized();
        ax2 = ax2.normalized();

        if (normax1 > normax2)
        {
            m_coordonne = pointIgnition2D;
            // e1
            m_vecteur = ax1;
            // a
            m_a = normax1;
            // b
            m_b = normax2;
        }
        else
        {
            m_coordonne = pointIgnition2D;
            m_vecteur = ax2;
            m_a = normax2;
            m_b = normax1;
        }
    }
}

EllipseOpt::EllipseOpt(Point2D p)
{
    Vector2D vtemp = Vector2D(p);
    Vector3D rateOfSpread = Vector3D((p.x() + 1) / vtemp.norm(), (p.y() + 2) / vtemp.norm(), 1);

    float alpha = 0;
    float phi = 0;
    float epsilon = 0.9;

    Vector3D plan = Vector3D(sin(phi) * sin(alpha), -cos(phi) * sin(alpha), cos(alpha));

    double testPlan = rateOfSpread.dot(plan);
    if (testPlan != 0)
    {
        rateOfSpread = Vector3D(rateOfSpread.x(), rateOfSpread.y(), rateOfSpread.z() - testPlan / cos(alpha));
    }

    Vector3D petitAxe = Vector3D((rateOfSpread.y() * plan.z()) - (rateOfSpread.z() * plan.y()),
                                 (rateOfSpread.z() * plan.x()) - (rateOfSpread.x() * plan.z()),
                                 (rateOfSpread.x() * plan.y()) - (rateOfSpread.y() * plan.x()));
    Point3D pointIgnition3D = Point3D(-(epsilon / (1 + epsilon)) * rateOfSpread.x(),
                                      -(epsilon / (1 + epsilon)) * rateOfSpread.y(),
                                      -(epsilon / (1 + epsilon)) * rateOfSpread.z());

    Point2D pointIgnition2D = Point2D(pointIgnition3D.x(), pointIgnition3D.y());

    Vector2D vecteurAuxPetit = Vector2D(sqrt((1 - epsilon) / (1 + epsilon)) * petitAxe.x(),
                                        sqrt((1 - epsilon) / (1 + epsilon)) * petitAxe.y());
    Vector2D vecteurAuxGrand = Vector2D(1 / (1 + epsilon) * rateOfSpread.x(),
                                        1 / (1 + epsilon) * rateOfSpread.y());

    double aux1 = vecteurAuxPetit.dot(vecteurAuxPetit) - vecteurAuxGrand.dot(vecteurAuxGrand);
    double aux2 = vecteurAuxPetit.dot(vecteurAuxGrand);
    if (abs(aux1) > 0.0)
    {
        double normr = vecteurAuxPetit.norm();
        m_coordonne = Point2D(0, 0);
        m_vecteur = Vector2D(1, 0);
        m_a = normr;
        m_b = normr;
    }
    else
    {

        double theta = atan(aux2 / aux1) / 2;

        Vector2D ax1 = cos(theta) * vecteurAuxGrand + sin(theta) * vecteurAuxPetit;
        Vector2D ax2 = -sin(theta) * vecteurAuxGrand + cos(theta) * vecteurAuxPetit;

        double normax1 = ax1.norm();
        double normax2 = ax2.norm();

        ax1 = ax1.normalized();
        ax2 = ax2.normalized();

        if (normax1 > normax2)
        {
            m_coordonne = pointIgnition2D;
            m_vecteur = ax1;
            m_a = normax1;
            m_b = normax2;
        }
        else
        {
            m_coordonne = pointIgnition2D;
            m_vecteur = ax2;
            m_a = normax2;
            m_b = normax1;
        }
    }
}

std::ostream &operator<<(std::ostream &os, EllipseOpt &o)
{
    return os << "Coordonne : " << o.coordonne() << "Vecteur : " << o.vecteur() << std::endl
              << "a : " << o.a() << " | "
              << " b : " << o.b() << std::endl;
}

EllipseOpt::EllipseOpt(const EllipseOpt &other) : m_vecteur(other.m_vecteur), m_coordonne(other.m_coordonne), 
m_a(other.m_a), m_b(other.m_b)
{
    
}

EllipseOpt EllipseOpt::operator=(const EllipseOpt &other)
{
    this->m_a = other.m_a;
    this->m_b = other.m_b;
    this->m_coordonne = other.m_coordonne;
    this->m_vecteur = other.m_vecteur;

    return *this;
}

EllipseOpt::EllipseOpt() : m_coordonne(), m_vecteur(), m_b(), m_a() {}
