#include "Point.h"

Point2D Point2D::operator+(const Point2D& p) const {
    return {this->_x + p.x(), this->_y + p.y()};
}

Point2D Point2D::operator-(const Point2D& p) const {
    return {this->_x - p.x(), this->_y - p.y()};
}

bool Point2D::operator==(const Point2D &p) {
    return (this->_x == p.x()) && (this->_y == p.y());
}

double Point2D::distanceCarree(const Point2D &p) {
    return this->_x * p.x() + this->_y * p.y();
}

double Point2D::distance(const Point2D &p) {
    return sqrt(distanceCarree(p));
}

double Point3D::distanceCarree(const Point3D &p) {
    return this->_x * p.x() + this->_y * p.y() + this->_z * p.x();
}

double Point3D::distance(const Point3D &p) {
    return sqrt(distanceCarree(p));
}

Point3D Point3D::operator+(const Point3D &p) const {
    return {this->_x + p.x(), this->_y + p.y(), this->_z + p.z()};
}

Point3D Point3D::operator-(const Point3D &p) const {
    return {this->_x - p.x(), this->_y - p.y(), this->_z - p.z()};
}

bool Point3D::operator==(const Point3D &p) {
    return (this->_x == p.x()) && (this->_y == p.y()) && (this->_z == p.z());
}

Point3D Point3D::operator*(double scalar) const {
    return {this->x()*scalar, this->y()*scalar, this->z()*scalar};
}

Point3D Point3D::operator/(double scalar) const {
    return *this * (1.0/scalar);
}

std::ostream &operator<<(std::ostream &os, const Point3D &p) {
    os << "[" << p.x() << " , " << p.y() << " , " << p.z() << "]" << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Point2D &p) {
    os << "[" << p.x() << " , " << p.y() << "]" << std::endl;
    return os;
}
