#ifndef ALGO_VECTEUR_H
#define ALGO_VECTEUR_H

#include <cmath>
#include <iostream>
#include <vector>
#include <utility>

#include <tuple>
#include "Utils.h"
#include "Point.h"

class Vector2D {
    std::pair<double, double> m_body;
public:
    Vector2D();
    Vector2D(double x, double y);
    Vector2D(const Point2D& p): m_body(p.x(), p.y()){};

    /*produit scalaire
     * @param v est le vector avec lequel on va faire notre produit
     */
    double dot(const Vector2D &v) const;

    /*norme au carré*/
    double squaredNorm() const;

    /* norme du vector */
    double norm() const;

    /* retourn un vector dont la norme fait un*/
    Vector2D normalized() const;

    /* determinant , calcule l'air signé obtenu avec ces 2 parallelogramme*/
    double determinant(const Vector2D &v) const;

    /*effectue une rotation de x degrees sur le vecteur
     * double degrees dont une veut faire rotate le vecteur
     */
    Vector2D rotate(double degrees) const;

    std::pair<double, double> toPair() const{return {m_body.first, m_body.second};}

    operator Point2D() const;
    Vector2D operator+(const Vector2D &v) const;
    Vector2D operator-(const Vector2D &v) const;
    Vector2D operator+(const Point2D &p) const;
    Vector2D operator-(const Point2D &p) const;
    Vector2D operator*(const double &scalar) const;
    Vector2D operator/(const double &scalar) const;
    bool operator==(const Vector2D &v) const;
    double &x();
    double x() const;
    double &y();
    double y() const;

    friend std::istream &operator>>(std::istream &is, Vector2D &v);
    friend std::ostream &operator<<(std::ostream &os, const Vector2D &v);

    friend Vector2D operator*(const double &scalar, const Vector2D &v);

    std::string toString();
};

class Vector3D {

    std::tuple<double, double, double> m_body;
public:
    Vector3D();
    Vector3D(double x, double y, double z);
    Vector3D(const Point3D& p): m_body(p.x(), p.y(), p.z()){};

    /*produit scalaire
    * @param v est le vector avec lequel on va faire notre produit
    */
    double dot(const Vector3D &v) const;

    /*norme au carré*/
    double squaredNorm() const;

    /* norme du vector */
    double norm() const;

    /* retourn un vector dont la norme fait un*/
    Vector3D normalized() const;

    /*effectue une rotation de x degrees sur le vecteur
    * double degrees dont une veut faire rotate le vecteur
    */

    operator Point3D() const;
    Vector3D operator+(const Vector3D &v) const;
    Vector3D operator-(const Vector3D &v) const;
    Vector3D operator*(const double &scalar) const;
    Vector3D operator/(const double &scalar) const;
    bool operator==(const Vector3D &v) const;
    double &x();
    double x() const;
    double &y();
    double y() const;
    double &z();
    double z() const;

    friend std::istream &operator>>(std::istream &is, Vector3D &v);
    friend std::ostream &operator<<(std::ostream &os, const Vector3D &v);

    friend Vector3D operator*(const double &scalar, const Vector3D &v);
};

#endif //ALGO_VECTEUR_H
