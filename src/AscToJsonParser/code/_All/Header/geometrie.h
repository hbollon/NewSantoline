#ifndef __GEOMETRIE__
#define __GEOMETRIE__
#include <vector>

#include "useful.h"

bool isLeft(Point2D a, Point2D b, Point2D c);
Point2D leftestPoint(std::vector<Point2D> points);
std::vector<Point2D> convexeHull(std::vector<Point2D> entryPoint);
Point2D intersectionPoint(Point2D a, Point2D b, Point2D c, Point2D d);
bool intersect(const Point2D& a, const Point2D& b, const Point2D& c, const Point2D& d);
#endif //__GEOMETRIE__
