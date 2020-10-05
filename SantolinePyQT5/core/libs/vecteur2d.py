import math
from . import useful

class Vecteur2D:
    def __init__(self, x = 0, y = 0):
        self.x_ = x
        self.y_ = y
        
    def dot(self, vect2d):
        return self.x_ * vect2d.x_ + self.y_ * vect2d.y_
        
    def sqnorm(self):
        return self.dot(self)
        
    def norm(self):
        return math.sqrt(self.sqnorm())
    
    def determinant(self, vect2d):
        return self.x_ * vect2d.y_ - self.y_ * vect2d.x_
        
    def normalise(self):
        if (self.norm() != 0):
            return self / self.norm()
        return self
        
    def rotate(self, degrees):
        radian = useful.degreesToRadian(degrees)
        return Vecteur2D(self.x_ * math.cos(rad) - self.y_ * math.sin(rad), 
                         self.x_ * math.sin(rad) + self.y_ * math.cos(rad))
    
    def __mul__(self, scalar):
        return Vecteur2D(self.x_ * scalar, self.y_ * scalar)
        
    def __imul__(self, scalar):
        return self * scalar
    
    def __add__(self, vect2d):
        return Vecteur2D(self.x_ + vect2d.x_, self.y_ + vect2d.y_)
        
    def __iadd__(self, vect2d):
        return self + vect2d
        
    def __sub__(self, vect2d):
        return Vecteur2D(self.x_ - vect2d.x_, self.y_ - vect2d.y_)
        
    def __isub__(self, vect2d):
        return self - vect2d

    def __truediv__(self, scalar):
        return Vecteur2D(self.x_ / scalar, self.y_ / scalar)

    def __floordiv__(self, scalar):
        return Vecteur2D(self.x_ // scalar, self.y_ // scalar)