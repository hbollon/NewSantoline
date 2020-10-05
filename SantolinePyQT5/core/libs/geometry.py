class Geometry:
    def __init__(self, origin):
        self.origin_ = origin
        
    def contain(self, point):
        pass
        
    def boundingBox(self):
        return rectangle.Rectangle()
        
class Rectangle(Geometry):
    def __init__(self, origin, width = 0, height = 0):
        super().__init__(origin)
        self.dimension_ = vecteur2d.Vecteur2D(width, height)
        
    def contain(self, point):
        return ( point.x_ >= self.origin_.x_ and 
                 point.x_ <= (self.origin_ + self.dimension_).x_ and 
                 point.y_ >= self.origin_.y_ and
                 point.y_ <= (self.origin_ + self.dimension_).y_ )