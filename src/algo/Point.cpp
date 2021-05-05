#include "Point.h"

Point2D Point2D::operator+(const Point2D &p) const
{
    return {this->_x + p.x(), this->_y + p.y()};
}

Point2D Point2D::operator-(const Point2D &p) const
{
    return {this->_x - p.x(), this->_y - p.y()};
}

bool Point2D::operator==(const Point2D &p)
{
    return (this->_x == p.x()) && (this->_y == p.y());
}

double Point2D::distanceCarree(const Point2D &p)
{
    return this->_x * p.x() + this->_y * p.y();
}

double Point2D::distance(const Point2D &p)
{
    return sqrt(distanceCarree(p));
}

double Point3D::distanceCarree(const Point3D &p)
{
    return this->_x * p.x() + this->_y * p.y() + this->_z * p.x();
}

double Point3D::distance(const Point3D &p)
{
    return sqrt(distanceCarree(p));
}

Point3D Point3D::operator+(const Point3D &p) const
{
    return {this->_x + p.x(), this->_y + p.y(), this->_z + p.z()};
}

Point3D Point3D::operator-(const Point3D &p) const
{
    return {this->_x - p.x(), this->_y - p.y(), this->_z - p.z()};
}

bool Point3D::operator==(const Point3D &p)
{
    return (this->_x == p.x()) && (this->_y == p.y()) && (this->_z == p.z());
}

bool Point3D::operator!=(const Point3D &p)
{
    return (this->_x != p.x()) || (this->_y != p.y()) || (this->_z != p.z());
}

bool Point3D::operator<=(const Point3D &p)
{
    if (this->_x != p.x())
        return this->_x <= p.x();
    else if (this->_y != p.y())
        return this->_y <= p.y();
    else
        return this->_z <= p.z();
}

bool Point3D::operator>=(const Point3D &p)
{
    if (this->_x != p.x())
        return this->_x >= p.x();
    else if (this->_y != p.y())
        return this->_y >= p.y();
    else
        return this->_z >= p.z();
}

bool Point3D::operator>(const Point3D &p)
{
    if (this->_x != p.x())
        return this->_x > p.x();
    else if (this->_y != p.y())
        return this->_y > p.y();
    else
        return this->_z > p.z();
}

bool Point3D::operator<(const Point3D &p)
{
    if (this->_x != p.x())
        return this->_x < p.x();
    else if (this->_y != p.y())
        return this->_y < p.y();
    else
        return this->_z < p.z();
}

bool Point3D::operator==(const Point3D &p) const
{
    return (this->_x == p.x()) && (this->_y == p.y()) && (this->_z == p.z());
}

bool Point3D::operator!=(const Point3D &p) const
{
    return (this->_x != p.x()) || (this->_y != p.y()) || (this->_z != p.z());
}

bool Point3D::operator<=(const Point3D &p) const
{
    if (this->_x != p.x())
        return this->_x <= p.x();
    else if (this->_y != p.y())
        return this->_y <= p.y();
    else
        return this->_z <= p.z();
}

bool Point3D::operator>=(const Point3D &p) const
{
    if (this->_x != p.x())
        return this->_x >= p.x();
    else if (this->_y != p.y())
        return this->_y >= p.y();
    else
        return this->_z >= p.z();
}

bool Point3D::operator>(const Point3D &p) const
{
    if (this->_x != p.x())
        return this->_x > p.x();
    else if (this->_y != p.y())
        return this->_y > p.y();
    else
        return this->_z > p.z();
}

bool Point3D::operator<(const Point3D &p) const
{
    if (this->_x != p.x())
        return this->_x < p.x();
    else if (this->_y != p.y())
        return this->_y < p.y();
    else
        return this->_z < p.z();
}

Point3D Point3D::operator*(double scalar) const
{
    return {this->x() * scalar, this->y() * scalar, this->z() * scalar};
}

Point3D Point3D::operator/(double scalar) const
{
    return *this * (1.0 / scalar);
}

std::ostream &operator<<(std::ostream &os, const Point3D &p)
{
    os << "[" << p.x() << " , " << p.y() << " , " << p.z() << "]" << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Point2D &p)
{
    os << "[" << p.x() << " , " << p.y() << "]" << std::endl;
    return os;
}

std::vector<Point2D> Points3DtoPoints2D(std::vector<Point3D> input)
{
    std::vector<Point2D> output;
    for (Point3D point : input)
    {
        Point2D point2d = Point2D(point.x(), point.z());
        output.push_back(point2d);
    }
    return output;
}

std::vector<Point3D> Points2DtoPoints3D(std::vector<Point2D> input)
{
    std::vector<Point3D> output;
    for (Point2D point : input)
    {
        Point3D point3d = Point3D(point.x(), 0, point.y());
        output.push_back(point3d);
    }
    return output;
}