import qgis
from . import controller
from ..model import canvas_model
from ..libs import file_connector
from qgis.core import *
import json


class CanvasController(controller.AController):
    def __init__(self, view):
        super().__init__(view)
        self.canvasModel_ = canvas_model.CanvasModel(self)
        self.canvasModel_.addObserver(view)
        self.view_ = view

    def addPointContour(self, point, button):
        self.canvasModel_.addPointContour(
            QgsPointXY(point.x(), point.y()), button)
        # self.accept()

    def addPointLargageEauABE(self, point, button):
        self.canvasModel_.addPointLargageEauABE(
            QgsPointXY(point.x(), point.y()), button)
        # self.accept()

    def addPointLargageEauHBE(self, point, button):
        self.canvasModel_.addPointLargageEauHBE(
            QgsPointXY(point.x(), point.y()), button)
        # self.accept()

    def addPointLargageCRetartant(self, point, button):
        self.canvasModel_.addPointLargageCRetartant(
            QgsPointXY(point.x(), point.y()), button)
        # self.accept()

    def addPointLargageHRetardant(self, point, button):
        self.canvasModel_.addPointLargageHRetardant(
            QgsPointXY(point.x(), point.y()), button)
        # self.accept()

    def addPointObstacle(self, point, button):
        self.canvasModel_.addPointObstacle(
            QgsPointXY(point.x(), point.y()), button)
        # self.accept()

    def addPointJalonnement(self, point, button):
        self.canvasModel_.addPointJalonnement(
            QgsPointXY(point.x(), point.y()), button)
        # self.accept()

    def addPointAppui(self, point, button):
        self.canvasModel_.addPointAppui(
            QgsPointXY(point.x(), point.y()), button)
        # self.accept()

    def setRoseVents(self, point):
        self.canvasModel_.setRoseVents(QgsPointXY(point.x(), point.y()))
        # self.accept()

    def afficheVent(self):
        self.canvasModel_.afficheVent()
        # self.accept()

    def removePointContour(self, point, button):
        self.canvasModel_.removePointContour(
            QgsPointXY(point.x(), point.y()), button)
        # self.accept()

    def removePointObstacle(self, point, button):
        self.canvasModel_.removePointObstacle(
            QgsPointXY(point.x(), point.y()), button)
        # self.accept()

    def removePointJalonnement(self, point, button):
        self.canvasModel_.removePointJalonnement(
            QgsPointXY(point.x(), point.y()), button)
        # self.accept()

    def removePointAppui(self, point, button):
        self.canvasModel_.removePointAppui(
            QgsPointXY(point.x(), point.y()), button)
        # self.accept()

    def clearPointContour(self):
        self.canvasModel_.clearPointContour()
        if self.view_.propagationLayer_ in self.view_.layers:
            self.view_.layers.remove(self.view_.propagationLayer_)
        self.view_.update(self.canvasModel_)
        # self.accept()

    def clearPointAction(self):
        vertex_items = [i for i in self.view_.canvas_.scene(
        ).items() if issubclass(type(i), qgis.gui.QgsVertexMarker)]
        for ver in vertex_items:
            if ver in self.view_.canvas_.scene().items():
                self.view_.canvas_.scene().removeItem(ver)
        self.canvasModel_.clearPointAction()
        # self.accept()

    def clearPointObstacle(self):
        self.canvasModel_.clearPointObstacle()
        # self.accept()

    def clearPointJalonnement(self):
        self.canvasModel_.clearPointJalonnement()
        # self.accept()

    def clearPointAppui(self):
        self.canvasModel_.clearPointAppui()
        self.view_.update(self.canvasModel_)
        # self.accept()

    def setJalonnement(self):
        if(self.view_.lastToolUsed_):
            self.view_.lastToolUsed_.append([])
        self.view_.canvas_.setMapTool(self.view_.toolAttaqueJalonnement_)
        self.view_.lastToolUsed_ = self.canvasModel_.jalonnement_

    def setAppui(self):
        if (self.view_.lastToolUsed_):
            self.view_.lastToolUsed_.append([])
        self.view_.canvas_.setMapTool(self.view_.toolLigneAppui_)
        self.view_.lastToolUsed_ = self.canvasModel_.appui_

    def setObstacle(self):
        if (self.view_.lastToolUsed_):
            self.view_.lastToolUsed_.append([])
        self.view_.canvas_.setMapTool(self.view_.toolLigneAppui_)
        self.view_.lastToolUsed_ = self.canvasModel_.appui_

    def map(self, departement):
        self.canvasModel_.map(departement)

    def wind(self):
        self.view_.wind_window_.show()

    def algorithm(self):
        self.view_.parameter_window_.show()

    def departements_choices(self):
        self.view_.departements_choices_window_.show()

    def reglage(self):
        self.view_.reglage_window_.show()

    def legend(self):
        self.view_.legend_window_.show()

    def accept(self):
        connector = file_connector.FileConnector()
        connector.write(json.dumps(self.canvasModel_.jsonify()))

    def showPopup(self, textContent, title):
        self.view_.popup_window_.update(textContent, title)
        self.view_.popup_window_.show()

    def switchContent(self, boolean):
        # main bar button
        self.view_.departement.setEnabled(boolean)
        self.view_.reglages.setEnabled(boolean)
        self.view_.actions.setEnabled(boolean)
        self.view_.vents.setEnabled(boolean)
        self.view_.simulation.setEnabled(boolean)
        # Simulation button
        self.view_.simuler.setEnabled(boolean)
        self.view_.actionEmitPoint.setEnabled(boolean)
        self.view_.actionCancelContour.setEnabled(boolean)
        # navigable button
        self.view_.actionPan.setEnabled(boolean)
        self.view_.actionZoomIn.setEnabled(boolean)
        self.view_.actionZoomOut.setEnabled(boolean)
