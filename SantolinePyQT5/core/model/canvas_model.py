from . import model
from qgis.core import *

class CanvasModel(model.AModel):
    def __init__(self, controller):
        super().__init__(controller)
        self.roseVents_ = QgsPointXY(0,0)
        self.contour_feu_ = []
        self.obstacle_ = [[]]
        self.jalonnement_ = [[]]
        self.appui_ = [[]]
        self.map_ = ""
        self.mapchange_ = False
        self.propagation_ = []
        self.propagation2_ = []
        self.largage_eau_HBE = [[]]
        self.largage_retardant_HBE = [[]]
        self.largage_eau_ABE = [[]]
        self.largage_retardant_ABE = [[]]
        self.listObstacle_ = []

    def setRoseVents(self,point):
        self.roseVents_=point
        self.notifyObservers()

    def addPointContour(self, point, button):
        self.contour_feu_.append(point)
        self.notifyObservers()

    def addPointLargageEauABE(self, point, button):
        if button == 1:
            self.largage_eau_ABE[len(self.largage_eau_ABE)-1].append(point)
            self.notifyObservers()
        else:
            self.largage_eau_ABE.append([])

    def addPointLargageEauHBE(self, point, button):
        if button == 1:
            self.largage_eau_HBE[len(self.largage_eau_HBE)-1].append(point)
            self.notifyObservers()
        else:
            self.largage_eau_HBE.append([])

    def addPointLargageCRetartant(self, point, button):
        if button == 1:
            self.largage_retardant_ABE[len(self.largage_retardant_ABE)-1].append(point)
            self.notifyObservers()
        else:
            self.largage_retardant_ABE.append([])

    def addPointLargageHRetardant(self, point, button):
        if button == 1:
            self.largage_retardant_HBE[len(self.largage_retardant_HBE)-1].append(point)
            self.notifyObservers()
        else:
            self.largage_retardant_HBE.append([])

    def addPointObstacle(self, point, button):
        if button == 1:
            self.obstacle_[len(self.obstacle_)-1].append(point)
            self.notifyObservers()
        else:
            self.obstacle_.append([])

    def addPointJalonnement(self, point, button):
        if button == 1:
            self.jalonnement_[len(self.jalonnement_)-1].append(point)
            self.notifyObservers()
        else:
            self.jalonnement_.append([])

    def addPointAppui(self, point, button):
        if button == 1:
            self.appui_[len(self.appui_)-1].append(point)
            self.notifyObservers()
        else:
            self.appui_.append([])

    def afficheVent(self):
        self.notifyObservers()

    def removePointContour(self, point):
        try:
            self.contour_feu_.remove(point)
            self.notifyObservers()
        except Exception:
            pass
        
    def removePointObstacle(self, point):
        try:
            self.obstacle_.remove(point)
            self.notifyObservers()
        except Exception:
            pass
        
    def removePointJalonnement(self, point):
        try:
            self.jalonnement_.remove(point)
            self.notifyObservers()
        except Exception:
            pass
        
    def removePointAppui(self, point):
        try:
            self.appui_.remove(point)
            self.notifyObservers()
        except Exception:
            pass

    def clearPointContour(self):
        self.contour_feu_ = []
        self.propagation_ = []
        self.notifyObservers()

    def clearPointAction(self):
        self.listObstacle_ = []
        self.obstacle_ = [[]]
        self.jalonnement_ = [[]]
        self.appui_ = [[]]
        self.largage_eau_ABE=[[]]
        self.largage_eau_HBE=[[]]
        self.largage_retardant_ABE=[[]]
        self.largage_retardant_HBE=[[]]
        self.notifyObservers()
        
    def map(self, departement):
        self.mapchange_ = self.map_ != departement
        self.map_ = departement
        self.notifyObservers()
        
    def change(self):
        change = self.mapchange_
        if (self.mapchange_):
            self.mapchange_ = False
        return change
        
    def addPropagation(self, points):
        qgspoints = []
        for point in points:
            qgspoints.append(QgsPointXY(point[0], point[1]))
        self.propagation_.append(qgspoints)
        self.notifyObservers()
        
    def centroids(self):
        result = [0,0]
        for point in self.contour_feu_:
            result[0] += point.x()
            result[1] += point.y()
        if len(self.contour_feu_) > 0:
            result[0] /= len(self.contour_feu_)
            result[1] /= len(self.contour_feu_)
        return result

    def jsonify(self):
        self.listObstacle_.append(self.obstacle_)
        self.listObstacle_.append(self.jalonnement_)
        self.listObstacle_.append(self.appui_)
        self.listObstacle_.append(self.largage_retardant_HBE)
        self.listObstacle_.append(self.largage_retardant_ABE)
        self.listObstacle_.append(self.largage_eau_HBE)
        self.listObstacle_.append(self.largage_eau_ABE)
        contour = []
        for point in self.contour_feu_:
            contour.append([point.x(), point.y()])
        return contour

    def obstacleJsonify(self):
        liste = []
        for obstacle in self.listObstacle_:
            for segment in obstacle:
                if segment:
                    seg = []
                    for point in segment:
                        seg.append([point.x(), point.y()])
                    liste.append(seg)
        return liste
