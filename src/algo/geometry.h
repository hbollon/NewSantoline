#ifndef ALGO_GEOMETRY_H
#define ALGO_GEOMETRY_H

#include <algorithm>
#include "Vecteur.h"
#include "Utils.h"

bool isLeft(const Point2D& a, const Point2D& b, const Point2D& c);
Point2D leftestPoint(std::vector<Point2D> points);
std::vector<Point2D> convexeHull(std::vector<Point2D> entryPoint);
Point2D intersectionPoint(const Point2D& a, const Point2D& b, const Point2D& c, const Point2D& d);
bool intersect(const Point2D &a, const Point2D &b, const Point2D &c, const Point2D &d);

#endif //ALGO_GEOMETRY_H
