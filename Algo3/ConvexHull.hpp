#ifndef ALGO3_CONVEXHULL_H
#define ALGO3_CONVEXHULL_H

#include "Point.h"
#include <vector>

// Returns a new list of points representing the convex hull of
// the given set of points. The convex hull excludes collinear points.
// This algorithm runs in O(n log n) time.
std::vector<Point3D> makeConvexHull(const std::vector<Point3D> &points);

// Returns the convex hull, assuming that each points[i] <= points[i + 1]. Runs in O(n) time.
std::vector<Point3D> makeConvexHullPresorted(const std::vector<Point3D> &points);

#endif