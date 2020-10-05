//
// Created by Skaldr on 7/12/2019.
//

#ifndef ALGO3_ELLIPSEOPT_HPP
#define ALGO3_ELLIPSEOPT_HPP


#include "Point.h"
#include "Vecteur.h"

class EllipseOpt {
public:
    Point2D coordonne(){return m_coordonne;}
    Vector2D vecteur(){return m_vecteur;}
    float a(){return m_a;}
    float b(){return m_b;}
    EllipseOpt(Point2D p, Vector3D r, float epsilon, float alpha, float phi);
    EllipseOpt(Point2D p);
    EllipseOpt(const EllipseOpt & other);
    EllipseOpt();
    EllipseOpt operator=(const EllipseOpt& other);
    friend std::ostream &operator<<(std::ostream &os, EllipseOpt &v);


private:
    Point2D m_coordonne{};
    Vector2D m_vecteur;
    float m_a;
    float m_b;
};


#endif //ALGO3_ELLIPSEOPT_HPP
