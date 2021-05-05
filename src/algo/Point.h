#ifndef ALGO_POINT_H
#define ALGO_POINT_H

#include <math.h>
#include <utility>
#include <tuple>
#include <iostream>
#include <vector>

class Point2D
{
protected:
    double _x;
    double _y;

public:
    uint64_t id = 0;

    Point2D() = default;
    Point2D(double x, double y) : _x(x), _y(y){};
    Point2D(std::pair<double, double> pair) : _x(pair.first), _y(pair.second) {}

    double distanceCarree(const Point2D &p);
    double distance(const Point2D &p);
    std::pair<double, double> toPair() const { return {this->_x, this->_y}; }

    double x() const { return _x; }
    double y() const { return _y; }
    double &x() { return _x; }
    double &y() { return _y; }

    Point2D operator+(const Point2D &p) const;
    Point2D operator-(const Point2D &p) const;
    bool operator==(const Point2D &p);
    friend std::ostream &operator<<(std::ostream &os, const Point2D &p);
};

class Point3D
{
protected:
    double _x = 0.0;
    double _y = 0.0;
    double _z = 0.0;

public:
    uint64_t id = 0;

    Point3D() = default;
    Point3D(double x, double y) : _x(x), _z(y) {}
    Point3D(double x, double y, double z) : _x(x), _y(y), _z(z) {}

    double distanceCarree(const Point3D &p);
    double distance(const Point3D &p);
    //std::tuple<double, double, double> toTuple() const{return {this->_x, this->_y, this->_z};}
    std::tuple<double, double, double> toTuple() const { return std::tuple<double, double, double>(this->_x, this->_y, this->_z); }

    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }
    double &x() { return _x; }
    double &y() { return _y; }
    double &z() { return _z; }

    Point3D operator+(const Point3D &p) const;
    Point3D operator-(const Point3D &p) const;
    Point3D operator*(double scalar) const;
    Point3D operator/(double scalar) const;
    bool operator==(const Point3D &p);
    bool operator!=(const Point3D &p);
    bool operator>(const Point3D &p);
    bool operator<(const Point3D &p);
    bool operator>=(const Point3D &p);
    bool operator<=(const Point3D &p);
    bool operator==(const Point3D &p) const;
    bool operator!=(const Point3D &p) const;
    bool operator>(const Point3D &p) const;
    bool operator<(const Point3D &p) const;
    bool operator>=(const Point3D &p) const;
    bool operator<=(const Point3D &p) const;
    friend std::ostream &operator<<(std::ostream &os, const Point3D &p);
};

std::vector<Point2D> Points3DtoPoints2D(std::vector<Point3D> input);
std::vector<Point3D> Points2DtoPoints3D(std::vector<Point2D> input);

/*** Un Burning Point est comme un point 2D mais avec une date d'igntion et une cellule d'ignition ***/
class BurningPoint : public Point2D
{
private:
    double _dateIgnition;
    Point2D _celluleQuiAMisFeu; // c'est de la que vient le feu (on ne propagera donc pas dans cette cellule)
                                // (nb: on remarquera qu'ici, une cellule est représentée par une coordonée)
    // TODO create ID for burning point which corresponding to the subdivided segment it belongs
public:
    BurningPoint(double x, double y, double dateIgnition, Point2D celluleQuiAMisFeu = Point2D(-1, -1)) : Point2D(x, y), _dateIgnition(dateIgnition), _celluleQuiAMisFeu(celluleQuiAMisFeu){};
    BurningPoint(Point2D p, double dateIgnition, Point2D celluleQuiAMisFeu = Point2D(-1, -1)) : Point2D(p), _dateIgnition(dateIgnition), _celluleQuiAMisFeu(celluleQuiAMisFeu){};

    double dateIgnition() const { return _dateIgnition; }
    double &dateIgnition() { return _dateIgnition; }
    Point2D celluleQuiAMisFeu() const { return _celluleQuiAMisFeu; }
    Point2D &celluleQuiAMisFeu() { return _celluleQuiAMisFeu; }
};

#endif //ALGO_POINT_H
