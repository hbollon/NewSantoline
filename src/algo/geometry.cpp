#include "geometry.h"

bool isLeft(Point2D a, Point2D b, Point2D c) {
    return Vector2D(b - a).determinant(c - a) > 0;
}

Point2D leftestPoint(std::vector<Point2D> points) {
    std::stable_sort(points.begin(), points.end(), [](Point2D a, Point2D b) -> bool {return a.y() < b.y();});
    std::stable_sort(points.begin(), points.end(), [](Point2D a, Point2D b) -> bool {return a.x() < b.x();});
    return points[0];
}

std::vector<Point2D> convexeHull(std::vector<Point2D> entryPoint) {
    Point2D p = leftestPoint(entryPoint);
    Point2D currentPoint = p;
    std::vector<Point2D> hull;
    do {
        hull.push_back(p);
        currentPoint = entryPoint[0];
        for ( auto it = entryPoint.begin() + 1; it != entryPoint.end(); ++it ){
            if ( ( p == currentPoint ) || ( isLeft( p, currentPoint, *it ) ) ){
                currentPoint = *it;
            }
        }
        p = currentPoint;
    } while ( !(currentPoint == hull[0]));
    return hull;
}

Point2D intersectionPoint(Point2D a, Point2D b, Point2D c, Point2D d)
{
    Vector2D u = (b - a);
    Vector2D v = (d - c);
    double x = v.determinant(c - a) / v.determinant(u);
    return a + (u * x);
}

bool ccw(Point2D a, Point2D b, Point2D c) {
    return (c.y() - a.y()) * (b.x() - a.x()) >= (b.y() - a.y()) * (c.x() - a.x());
}

/* Alors, est ce que ces quatres points s'intersectent...?
 *
 *          *
 *      *
 *             *     *
 */
bool intersect(const Point2D& a, const Point2D& b, const Point2D& c, const Point2D& d)  // ben voyons, l'intersection entre 4 points...
                                                                                        // et comment je sais lesquels sont déjà reliés entre eux?
{
    return ccw(a, c, d) != ccw(b, c, d) && ccw(a, b, c) != ccw(a, b, d);
}

