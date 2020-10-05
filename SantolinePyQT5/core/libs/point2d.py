from . import vecteur2d

class Point2D:
    def __init__(self, x = 0, y = 0):
        self.x_ = x
        self.y_ = y
        
    def __mul__(self, scalar):
        return Point2D(self.x_ * scalar, self.y_ * scalar)
        
    def __imul__(self, scalar):
        return self * scalar
    
    def __add__(self, vect2d):
        return Point2D(self.x_ + vect2d.x_, self.y_ + vect2d.y_)
        
    def __iadd__(self, vect2d):
        return self + vect2d
        
    def __sub__(self, vect2d):
        return Vecteur2D(self.x_ - vect2d.x_, self.y_ - vect2d.y_)
        
    def __isub__(self, vect2d):
        return self - vect2d

    def __truediv__(self, scalar):
        return Point2D(self.x_ / scalar, self.y_ / scalar)

    def __floordiv__(self, scalar):
        return Point2D(self.x_ // scalar, self.y_ // scalar)

    def distance(self, point):
        return (self - point).norm()