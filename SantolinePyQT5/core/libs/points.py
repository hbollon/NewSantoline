from . import geometry

class Points(geometry.Geometry):
    def __init__(self, points = []):
        self.points_ = points
        
    def setPoints(self, points):
        self.points_ = points
        
    def addPoint(self, point):
        self.points_.append(point)
    
    def removePoint(self, point):
        try:
            self.points_.remove(point)
        except ValueError:
            pass
            
    def convexHull(self):
        return []
        
    def concaveHull(self, rect_width, rect_height):
        global_rect = self.boundingBox()
        x_rect = Rectangle(global_rect.origin_, global_rect.dimension_.x_, d)
        y_rect = Rectangle(global_rect.origin_, d, global_rect.dimension_.y_)
        points.sort(key=lambda p: p.y_)
        lignes = lines(points, x_rect)
        points.sort(key=lambda p: p.x_)
        colonnes = columns(points, y_rect)
        result = []

        for ligne in lignes:
            ex = extremity(ligne, lambda l: l.x_)
            result.append(ex[0])
            result.append(ex[1])
            
        for colonne in colonnes:
            ex = extremity(colonne, lambda l: l.y_)
            result.append(ex[0])
            result.append(ex[1])
        temporary_dict = {}
        for point in result:
            temporary_dict["{}_{}".format(point.x_, point.y_)] = point
        return list(map(lambda n: temporary_dict[n], temporary_dict))
        
    def lines(self, height):
        result = []
        tampon = []
        last = self.boundingBox()
        last.dimension_.y_ = height
        displacement = Vecteur2D(0,last.dimension_.y_)
        for point in self.points_:
            if (not last.contain(point)):
                result.append(tampon)
                tampon = []
                last.origin_ += displacement
        return result        
        
    def columns(self, width):
        result = []
        tampon = []
        last = self.boundingBox()
        last.dimension_.x_ = width
        displacement = Vecteur2D(last.dimension_.x_,0)
        for point in self.points_:
            if (not last.contain(point)):
                result.append(tampon)
                tampon = []
                last.origin_ += displacement
        return result
        
    def boundingBox(self):
        min_x = min(self.points_, key=lambda p: p.x_).x_
        min_y = min(self.points_, key=lambda p: p.y_).y_
        max_x = max(self.points_, key=lambda p: p.x_).x_
        max_y = max(self.points_, key=lambda p: p.y_).y_
        origin = Point2D(min_x, min_y)
        width = max_x - min_x
        height = max_y - min_y
        return rectangle.Rectangle(origin, width, height)
