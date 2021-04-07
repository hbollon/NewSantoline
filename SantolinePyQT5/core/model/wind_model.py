from . import model, canvas_model
from osgeo import gdal
class WindModel(model.AModel):
    def __init__(self, controller):
        super().__init__(controller)
        self.north_ = 0
        self.south_=0
        self.east_=0
        self.west_=0
        self.direction_ = 0
        self.speed_ = 0.
        self.process_ = 1

    def north(self,distance):
        self.north_=distance
        self.notifyObservers()

    def south(self, distance):
        self.south_ = distance
        self.notifyObservers()

    def east(self, distance):
        self.east_ = distance
        self.notifyObservers()

    def west(self, distance):
        self.west_ = distance
        self.notifyObservers()
        
    def direction(self, direction):
        self.direction_ = direction
        self.notifyObservers()
        
    def speed(self, speed):
        self.speed_ = speed
        self.notifyObservers()

    def process(self, process):
        self.process_ = process
        self.notifyObservers()
        
    def jsonify(self, tifPath):
        ds = gdal.Open(tifPath)
        width = ds.RasterXSize
        height = ds.RasterYSize
        gt = ds.GetGeoTransform()
        minx = gt[0]
        miny = gt[3] + width * gt[4] + height * gt[5]
        maxx = gt[0] + width * gt[1] + height * gt[2]
        maxy = gt[3]
        point = canvas_model.CanvasModel(self.controler_).roseVents_
        widthtemp=self.east_+self.west_
        heighttemp=self.north_+self.south_
        xtemp = (point.x() - self.west_)+(widthtemp/2)
        ytemp = (point.y() - self.south_)+(heighttemp/2)
        xfinal = xtemp - ((xtemp-minx)%25)
        yfinal = ytemp - ((ytemp-miny)%25)
        widthfinal = widthtemp-(widthtemp%50)+50
        heightfinal = heighttemp-(heighttemp%50)+50
        origin = (self.direction_ + 180.)%360.
        print("\n h:"+str(heightfinal)+" w:"+str(widthfinal)+"\n")
        print("\n east:"+str(self.east_)+" west:"+str(self.west_)+"\n")
        print("\n north:"+str(self.north_)+" south:"+str(self.south_)+"\n")

        json = {
            #"type": "parametreepilobe",
            #"epilobe": {
                "axeorigine": "est",
                "direction": round(origin, 1),
                "force": round(self.speed_, 1),
                "nbProcess": 2,
                "dimension": [heightfinal, widthfinal],
                "origine": [xfinal-5, yfinal-5]
                #canvas_model.CanvasModel(self.controler_).centroids()
            #}
        }
        return json
