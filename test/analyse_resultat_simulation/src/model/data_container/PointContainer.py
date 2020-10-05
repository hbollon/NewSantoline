def coords_in_range(point, point_size, coords: (float, float)) -> bool:
    p_coords = point["coordonnees"]
    return p_coords[0] - point_size <= coords[0] <= p_coords[0] + point_size \
        and p_coords[1] - point_size <= coords[1] <= p_coords[1] + point_size


class PointContainer:

    def __init__(self, points=None):
        if points is None:
            self.points = []
        else:
            self.points = points

    def set(self, points=None):
        if points is None:
            self.points = []
        else:
            self.points = points

    def get(self, x: float, y: float, radius: float):
        points = []
        for point in self.points:
            if coords_in_range(point, radius, (x, y)):
                points.append(point)
        return points

    def is_empty(self):
        return len(self.points) == 0

    def clear(self):
        self.points = []
