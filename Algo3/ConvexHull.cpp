#include <algorithm>
#include "ConvexHull.hpp"

using std::vector;

vector<Point3D> makeConvexHull(const vector<Point3D> &points)
{
	vector<Point3D> newPoints = points;
	std::sort(newPoints.begin(), newPoints.end());
	return makeConvexHullPresorted(newPoints);
}

vector<Point3D> makeConvexHullPresorted(const vector<Point3D> &points)
{
	if (points.size() <= 1)
		return vector<Point3D>(points);

	// Andrew's monotone chain algorithm. Positive y coordinates correspond to "up"
	// as per the mathematical convention, instead of "down" as per the computer
	// graphics convention. This doesn't affect the correctness of the result.

	vector<Point3D> upperHull;
	for (const Point3D &p : points)
	{
		while (upperHull.size() >= 2)
		{
			const Point3D &q = *(upperHull.cend() - 1); // Same as .back()
			const Point3D &r = *(upperHull.cend() - 2);
			if ((q.x() - r.x()) * (p.y() - r.y()) >= (q.y() - r.y()) * (p.x() - r.x()))
				upperHull.pop_back();
			else
				break;
		}
		upperHull.push_back(p);
	}
	upperHull.pop_back();

	vector<Point3D> lowerHull;
	for (vector<Point3D>::const_reverse_iterator it = points.crbegin(); it != points.crend(); ++it)
	{
		const Point3D &p = *it;
		while (lowerHull.size() >= 2)
		{
			const Point3D &q = *(lowerHull.cend() - 1); // Same as .back()
			const Point3D &r = *(lowerHull.cend() - 2);
			if ((q.x() - r.x()) * (p.y() - r.y()) >= (q.y() - r.y()) * (p.x() - r.x()))
				lowerHull.pop_back();
			else
				break;
		}
		lowerHull.push_back(p);
	}
	lowerHull.pop_back();

	if (!(upperHull.size() == 1 && upperHull == lowerHull))
		upperHull.insert(upperHull.end(), lowerHull.cbegin(), lowerHull.cend());
	return upperHull;
}