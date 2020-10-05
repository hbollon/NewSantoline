#include "Vecteur.h"

Vector2D::Vector2D() : m_body(0, 0)
{
}

Vector2D::Vector2D(double x, double y) : m_body(x, y)
{
}

double Vector2D::dot(const Vector2D &v) const
{
    return x() * v.x() + y() * v.y();
}

double Vector2D::squaredNorm() const
{
    return dot(*this);
}

double Vector2D::norm() const
{
    return sqrt(squaredNorm());
}

Vector2D Vector2D::normalized() const
{
    if (norm() > 0)
        return *this / norm();
    else
        return *this;
}

double Vector2D::determinant(const Vector2D &v) const
{
    return x() * v.y() - y() * v.x();
}

Vector2D Vector2D::rotate(double degrees) const
{
    double radians = deg_to_rad(degrees);
    return {x() * cos(radians) - y() * sin(radians),
                    x() * sin(radians) + y() * cos(radians)};
}

Vector2D Vector2D::operator+(const Vector2D &v) const
{
    return {x() + v.x(), y() + v.y()};
}

Vector2D Vector2D::operator-(const Vector2D &v) const
{
    return {x() - v.x(), y() - v.y()};
}

Vector2D Vector2D::operator*(const double &scalar) const
{
    return {x() * scalar, y() * scalar};
}
Vector2D Vector2D::operator/(const double &scalar) const
{
    return {x() / scalar, y() / scalar};
}

bool Vector2D::operator==(const Vector2D &v) const
{
    return m_body == v.m_body;
}

double & Vector2D::x()
{
    return m_body.first;
}

double Vector2D::x() const {
    return m_body.first;
}

double & Vector2D::y()
{
    return m_body.second;
}

double Vector2D::y() const
{
    return m_body.second;
}



std::string Vector2D::toString()
{
    return "[" + std::to_string(x()) + ", " + std::to_string(y()) + "]";
}

std::istream &operator>>(std::istream &is, Vector2D &v)
{
    return is >> v.x() >> v.y();
}

std::ostream &operator<<(std::ostream &os, const Vector2D &v)
{
    return os << "(" << v.x() << ", " << v.y() << ")";
}

Vector2D operator*(const double &scalar, const Vector2D &v)
{
    return {v.x() * scalar, v.y() * scalar};
}

Vector2D Vector2D::operator+(const Point2D &p) const {
    return {this->x() + p.x(), this->y() + p.y()};
}

Vector2D Vector2D::operator-(const Point2D &p) const {
    return {this->x() - p.x(), this->y() - p.y()};
}

Vector2D::operator Point2D() const {
    return {this->x(), this->y()};
}


Vector3D::Vector3D() : m_body(0, 0, 0)
{
}

Vector3D::Vector3D(double x, double y, double z) : m_body(x, y, z)
{
}

double Vector3D::dot(const Vector3D &v) const
{
    return x() * v.x() + y() * v.y() + z() * v.z();
}

double Vector3D::squaredNorm() const
{
    return dot(*this);
}

double Vector3D::norm() const
{
    return sqrt(squaredNorm());
}

Vector3D Vector3D::normalized() const
{
    if (norm() > 0)
        return *this / norm();
    else
        return *this;
}

Vector3D Vector3D::operator+(const Vector3D &v) const
{
    return {x() + v.x(), y() + v.y(), z() + v.z()};
}

Vector3D Vector3D::operator-(const Vector3D &v) const
{
    return {x() - v.x(), y() - v.y(), z() - v.z()};
}

Vector3D Vector3D::operator*(const double &scalar) const
{
    return {x() * scalar, y() * scalar, z() * scalar};
}

Vector3D Vector3D::operator/(const double &scalar) const
{
    return {x() / scalar, y() / scalar, z() / scalar};
}

bool Vector3D::operator==(const Vector3D &v) const
{
    return m_body == v.m_body;
}

double & Vector3D::x()
{
    return std::get<0>(m_body);
}

double Vector3D::x() const
{
    return std::get<0>(m_body);
}

double & Vector3D::y()
{
    return std::get<1>(m_body);
}

double Vector3D::y() const
{
    return std::get<1>(m_body);
}

double & Vector3D::z()
{
    return std::get<2>(m_body);
}

double Vector3D::z() const
{
    return std::get<2>(m_body);
}

std::istream &operator>>(std::istream &is, Vector3D &v)
{
    return is >> v.x() >> v.y() >> v.z();
}

std::ostream &operator<<(std::ostream &os, const Vector3D &v)
{
    return os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
}

Vector3D operator*(const double &scalar, const Vector3D &v)
{
    return {v.x() * scalar, v.y() * scalar, v.z() * scalar};
}

Vector3D::operator Point3D() const {
    return {this->x(), this->y(), this->z()};;
}
