from random import randrange

from qgis.gui import *#QgsMapCanvas
from qgis.core import *#QgsRasterLayer, QgsApplication
from qgis.PyQt.QtWidgets import *#QMainWindow, QWidget, QTabWidget, QPushButton
from qgis.PyQt.QtCore import *#QSize
from qgis.PyQt.QtGui import *#QFrame
import math

from core.view import reglage_view
from . import wind_view, algorithm_view, departements_choices_view, reglage_view
from ..controller import *
from ..libs import *
from win32api import GetSystemMetrics

import threading
import json
import os

class Santoline(QMainWindow, observable.Observer):
    def __init__(self):
        super().__init__()
        self.keys = []
        self.firstToolbar_ = None
        self.secondToolbar_ = None 
        self.left_toolbar_ = None
        self.simulation = None
        self.simuler = None
        self.actionEmitPoint = None 
        self.actionCancelContour = None
        self.actions = None 
        self.actionLargageCEau = None 
        self.actionLargageHEau = None
        self.actionLargageCRetardant = None
        self.actionLargageHRetardant = None
        self.actionAttaqueJalonnement = None
        self.actionLigneAppui = None
        self.actionObstacle = None
        self.actionCancelAction = None
        self.vents = None
        self.roseVents = None
        self.calculVents = None
        self.afficheVents = None
        self.affichePente = None
        self.afficheVentsPentes = None
        self.affichePlusDense = None
        self.afficheMoinsDense = None
        self.reloadMap = None

        self.statusbar_ = None
        self.progressbar_ = None
        self.canvas_ = None
        self.wind_window_ = None
        self.parameter_window_ = None
        self.reglage_window_ = None
        self.departements_choices_window_ = None
        self.thread_ = None
        self.fireLayer_ = None
        self.rubber_contour_feu_ = None
        self.rubber_jalonnement_ = None
        self.rubber_appui1_ = None
        self.rubber_appui2_ = None
        self.rubber_obstacle_ = None
        self.rubber_retardant_HBE_=None
        self.rubber_retardant_ABE_=None
        self.rubber_eau_ABE_ = None
        self.rubber_eau_HBE_ = None

        self.toolAttaqueJalonnement_ = None
        self.toolLigneAppui_ = None
        self.lastToolUsed_ = None

        self.propagations_rubber_ = []
        self.controller_ = canvas_controller.CanvasController(self)
        self.addTrackTool = None
        self.eau_HBE_maker = None
        self.retardant_HBE_maker = None
        self.eau_ABE_marker = None
        self.eau_ABE_marker2 = None
        self.retartant_ABE_marker = None
        self.retartant_ABE_marker2 = None
        self.obstacle_marker = None
        self.windLayer_ = None
        self.slopeLayer_ = None
        self.windSlopeLayer_ = None
        self.roseLayer_ = None
        self.propagationLayer_ = None
        self.propagationLayer2_ = None
        self.windMatrix_=None
        self.densite=2
        self.layers = []
        self.windmapdisplayed = False

        self.initUI()

    def initUI(self):
        self.setWindowTitle("Santoline")
        self.setGeometry(90, 90, 1280, 960)
        self.showMaximized()
        self.hide()

        # Ribbon bar / Barre ruban
        self.firstToolbar_ = self.addToolBar("First tool bar")
        self.firstToolbar_.setMovable(False)
        self.firstToolbar_.setFloatable(False)

        self.addToolBarBreak()
        self.secondToolbar_ = self.addToolBar("Second tool bar")
        self.secondToolbar_.setMovable(False)
        self.secondToolbar_.setFloatable(False)

        
        
        self.addToolBarBreak()
        self.left_toolbar_ = self.addToolBar("Left tool bar")


        self.left_toolbar_.setMovable(False)
        self.left_toolbar_.setFloatable(False)
        self.addToolBar(QtCore.Qt.LeftToolBarArea,self.left_toolbar_)  # On positionne la toolbar à gauche

        self.statusbar_ = QStatusBar(self)
        self.setStatusBar(self.statusbar_)
        self.progressbar_ = QProgressBar(self.statusbar_)
        self.statusbar_.addWidget(self.progressbar_)
        

        self.parameter_window_ = algorithm_view.ParameterWindow(self.controller_.canvasModel_,self)
        self.reglage_window_ = reglage_view.ReglageWindow(self.controller_.canvasModel_, self)
        self.departements_choices_window_ = departements_choices_view.Departements_Choices_Window()



        
        # Ribbon bar Departement / Barre ruban Département
        departement = QToolButton(self.secondToolbar_)
        departement.setCheckable(False)
        departement.setAutoExclusive(True)
        departement.setText("Departements")
        self.secondToolbar_.addWidget(departement)
        departement.clicked.connect(self.controller_.departements_choices)

        # Interface body / Corps de l'interface
        self.canvas_ = QgsMapCanvas(self)
        self.canvas_.setCachingEnabled(True)
        self.canvas_.setParallelRenderingEnabled(True)
        self.setCentralWidget(self.canvas_)

        self.wind_window_ = wind_view.WindWindow(self)

        # Map tools
        toolPan = QgsMapToolPan(self.canvas_)
        toolZoomIn = QgsMapToolZoom(self.canvas_, False)
        toolZoomOut = QgsMapToolZoom(self.canvas_, True)
        toolEdit = QgsMapToolEmitPoint(self.canvas_)
        
        actionPan = QAction(QIcon("icons\\hand.png"), "Naviguer", self)
        actionPan.setCheckable(True)
        toolPan.setAction(actionPan)
        actionPan.triggered.connect(lambda : self.canvas_.setMapTool(toolPan))
        
        actionZoomIn = QAction(QIcon("icons\\zoom_in.png"), "Zoom avant", self)
        actionZoomIn.setCheckable(False)
        actionZoomIn.triggered.connect(self.zoomAvant )

        actionZoomOut = QAction(QIcon("icons\\zoom_out.png"), "Zoom arriere", self)
        actionZoomOut.setCheckable(False)
        toolZoomOut.setAction(actionZoomOut)
        actionZoomOut.triggered.connect(self.zoomArriere )

        # Simulation
        self.simulation = QToolButton(self.secondToolbar_)
        self.simulation.setText("Simulation")
        self.simulation.setCheckable(True)
        self.simulation.setAutoExclusive(True)
        #widget ajouté plus bas, après les actions et les vents
        self.simulation.clicked.connect(self.affichageToolsSimulation)
        
        self.simuler = QAction(QIcon("icons\\validation.png"), "Simuler", self)
        self.simuler.setCheckable(False)
        toolEdit.setAction(self.simuler)
        self.simuler.triggered.connect(self.controller_.algorithm)
        self.simuler.setVisible(False)

        toolContour = QgsMapToolEmitPoint(self.canvas_)
        toolContour.canvasClicked.connect(self.controller_.addPointContour)
        self.actionEmitPoint = QAction(QIcon("icons\\map_edit.png"), "Dessiner Contours", self)
        self.actionEmitPoint.setCheckable(True)
        self.actionEmitPoint.triggered.connect(lambda: self.canvas_.setMapTool(toolContour))
        self.actionEmitPoint.setVisible(False)

        self.actionCancelContour = QAction(QIcon("icons\\cancel.png"), "Détruire Contours", self)
        self.actionCancelContour.setCheckable(False)
        self.actionCancelContour.triggered.connect(self.controller_.clearPointContour)
        self.actionCancelContour.setVisible(False)

        self.firstToolbar_.addAction(actionPan)
        self.firstToolbar_.addAction(actionZoomIn)
        self.firstToolbar_.addAction(actionZoomOut)
        self.left_toolbar_.addAction(self.simuler)
        self.left_toolbar_.addAction(self.actionEmitPoint)
        self.left_toolbar_.addAction(self.actionCancelContour)

        # Actions pompiers

        self.actions = QToolButton(self.secondToolbar_)
        self.actions.setText("Actions")
        self.actions.setCheckable(True)
        self.actions.setChecked(True)
        self.actions.setAutoExclusive(True)
        self.secondToolbar_.addWidget(self.actions)
        self.actions.clicked.connect(self.affichageToolsActions)

        toolLargageCEau = QgsMapToolEmitPoint(self.canvas_)
        toolLargageCEau.canvasClicked.connect(self.controller_.addPointLargageEauABE)
        self.actionLargageCEau = QAction(QIcon("icons\\actions\\ceau.png"), "Largage eau ABE", self)
        self.actionLargageCEau.setCheckable(True)
        self.actionLargageCEau.triggered.connect(lambda: self.canvas_.setMapTool(toolLargageCEau))
        self.left_toolbar_.addAction(self.actionLargageCEau)

        toolLargageHEau = QgsMapToolEmitPoint(self.canvas_)
        toolLargageHEau.canvasClicked.connect(self.controller_.addPointLargageEauHBE)
        self.actionLargageHEau = QAction(QIcon("icons\\actions\\heau.png"), "Largage eau HBE", self)
        self.actionLargageHEau.setCheckable(True)
        self.actionLargageHEau.triggered.connect(lambda: self.canvas_.setMapTool(toolLargageHEau))
        self.left_toolbar_.addAction(self.actionLargageHEau)

        toolLargageCRetardant = QgsMapToolEmitPoint(self.canvas_)
        toolLargageCRetardant.canvasClicked.connect(self.controller_.addPointLargageCRetartant)
        self.actionLargageCRetardant = QAction(QIcon("icons\\actions\\cretardant.png"), "Largage retardant ABE", self)
        self.actionLargageCRetardant.setCheckable(True)
        self.actionLargageCRetardant.triggered.connect(lambda: self.canvas_.setMapTool(toolLargageCRetardant))
        self.left_toolbar_.addAction(self.actionLargageCRetardant)

        toolLargageHRetardant = QgsMapToolEmitPoint(self.canvas_)
        toolLargageHRetardant.canvasClicked.connect(self.controller_.addPointLargageHRetardant)
        self.actionLargageHRetardant = QAction(QIcon("icons\\actions\\hretardant.png"), "Largage retardant HBE", self)
        self.actionLargageHRetardant.setCheckable(True)
        self.actionLargageHRetardant.triggered.connect(lambda: self.canvas_.setMapTool(toolLargageHRetardant))
        self.left_toolbar_.addAction(self.actionLargageHRetardant)


        self.toolAttaqueJalonnement_ = QgsMapToolEmitPoint(self.canvas_)
        self.toolAttaqueJalonnement_.canvasClicked.connect(self.controller_.addPointJalonnement)
        self.actionAttaqueJalonnement = QAction(QIcon("icons\\actions\\jalonnementS.png"), "Attaque de jalonnement", self)
        self.actionAttaqueJalonnement.setCheckable(True)
        self.actionAttaqueJalonnement.triggered.connect( self.controller_.setJalonnement)
        self.left_toolbar_.addAction(self.actionAttaqueJalonnement)

        self.toolLigneAppui_ = QgsMapToolEmitPoint(self.canvas_)
        self.toolLigneAppui_.canvasClicked.connect(self.controller_.addPointAppui)
        self.actionLigneAppui = QAction(QIcon("icons\\actions\\ligneappui.png"), "Ligne d'appui", self)
        self.actionLigneAppui.setCheckable(True)
        self.left_toolbar_.addAction(self.actionLigneAppui)
        self.actionLigneAppui.triggered.connect(self.controller_.setAppui)

        toolObstacle = QgsMapToolEmitPoint(self.canvas_)
        toolObstacle.canvasClicked.connect(self.controller_.addPointObstacle)
        self.actionObstacle = QAction(QIcon("icons\\actions\\obstacle.png"), "Obstacle naturel", self)
        self.actionObstacle.setCheckable(True)
        self.left_toolbar_.addAction(self.actionObstacle)
        self.actionObstacle.triggered.connect(lambda: self.canvas_.setMapTool(toolObstacle))

        self.actionCancelAction = QAction(QIcon("icons\\cancel.png"), "Effacer actions", self)
        self.actionCancelAction.setCheckable(False)
        self.actionCancelAction.triggered.connect(self.controller_.clearPointAction)
        self.left_toolbar_.addAction(self.actionCancelAction)

        alignmentGroup = QActionGroup(self); # for auto-exclusive actions
        alignmentGroup.addAction(self.actionLargageCEau)
        alignmentGroup.addAction(self.actionLargageHEau)
        alignmentGroup.addAction(self.actionLargageCRetardant)
        alignmentGroup.addAction(self.actionLargageHRetardant)
        alignmentGroup.addAction(self.actionAttaqueJalonnement)
        alignmentGroup.addAction(self.actionLigneAppui)
        alignmentGroup.addAction(self.actionObstacle)
        
        toolEdit.canvasClicked.connect(self.controller_.addPointContour)

        # Vents
        self.vents = QToolButton(self.secondToolbar_)
        self.vents.setText("Vents")
        self.vents.setCheckable(True)
        self.vents.setAutoExclusive(True)
        self.vents.clicked.connect(self.affichageToolsVents)
        self.secondToolbar_.addWidget(self.vents)
        
        self.secondToolbar_.addWidget(self.simulation)

        toolRoseVents = QgsMapToolEmitPoint(self.canvas_)
        toolRoseVents.canvasClicked.connect(self.controller_.setRoseVents)
        self.roseVents = QAction(QIcon("icons\\roseVents.png"), "Point de reference pour le calcul des vents", self)
        self.roseVents.setCheckable(False)
        self.roseVents.setVisible(False)
        self.roseVents.triggered.connect(lambda: self.canvas_.setMapTool(toolRoseVents))

        self.calculVents = QAction(QIcon("icons\\MancheAir.png"), "Calculer la carte des vents", self)
        self.calculVents.setCheckable(False)
        self.calculVents.setVisible(False)
        self.calculVents.triggered.connect(self.controller_.wind)
        
        self.afficheVents = QAction(QIcon("icons\\Vent.png"), "Afficher la carte des vents", self)
        self.afficheVents.setCheckable(True)
        self.afficheVents.setVisible(False)
        self.afficheVents.triggered.connect(self.afficheCanvasVents)

        self.affichePente = QAction(QIcon("icons\\Pente.png"), "Afficher la carte des pentes", self)
        self.affichePente.setCheckable(True)
        self.affichePente.setVisible(False)
        self.affichePente.triggered.connect(self.afficheCanvasSlope)
        
        self.afficheVentsPentes = QAction(QIcon("icons\\VentPentes.png"), "Afficher la carte vents-pentes", self)
        self.afficheVentsPentes.setCheckable(True)
        self.afficheVentsPentes.setVisible(False)
        self.afficheVentsPentes.triggered.connect(self.afficheCanvasWindSlope)

        self.affichePlusDense = QAction(QIcon("icons\\densePlus.png"), "Plus de marqueurs de vent", self)
        self.affichePlusDense.setCheckable(False)
        self.affichePlusDense.setVisible(False)
        self.affichePlusDense.triggered.connect(self.affichagePlusDense)

        self.afficheMoinsDense = QAction(QIcon("icons\\denseMoins.png"), "Moins de marqueurs de vent", self)
        self.afficheMoinsDense.setCheckable(False)
        self.afficheMoinsDense.setVisible(False)
        self.afficheMoinsDense.triggered.connect(self.affichageMoinsDense)

        self.reloadMap = QAction(QIcon("icons\\reload.png"), "Recharger la carte des vents", self)
        self.reloadMap.setCheckable(False)
        self.reloadMap.setVisible(False)
        self.reloadMap.triggered.connect(self.reload)

        alignmentGroup2 = QActionGroup(self)
        alignmentGroup2.addAction(self.afficheVents)
        alignmentGroup2.addAction(self.afficheVentsPentes)
        alignmentGroup2.addAction(self.affichePente)


        self.left_toolbar_.addAction(self.calculVents)
        self.left_toolbar_.addAction(self.roseVents)
        self.left_toolbar_.addAction(self.afficheVents)
        self.left_toolbar_.addAction(self.affichePente)
        self.left_toolbar_.addAction(self.afficheVentsPentes)
        self.left_toolbar_.addAction(self.affichePlusDense)
        self.left_toolbar_.addAction(self.afficheMoinsDense)
        self.left_toolbar_.addAction(self.reloadMap)

        
        self.reglages = QToolButton(self.secondToolbar_)
        self.reglages.setText("Réglages")
        self.reglages.setCheckable(False)
        self.reglages.setAutoExclusive(True)
        self.reglages.clicked.connect(self.controller_.reglage)
        
        self.secondToolbar_.addWidget(self.reglages)
        
        self.departements_choices_window_.controller_.view_.valider_.clicked.connect(
            lambda: self.controller_.map(self.departements_choices_window_.controller_.model_.departement_courant))

        self.windMatrix_=self.windMatrixInit('..\\data\\maps\\map.json')

        self.slopeLayer_ = self.setWindLayer('255,150,0,255', self.densite, "slope")
        self.windLayer_ = self.setWindLayer('0,0,255,255', self.densite, "wind")
        self.windSlopeLayer_ = self.setWindLayer('255,0,0,255', self.densite, "windslope")

        self.roseLayerInit(self.controller_.canvasModel_)

        self.show()


    ###---Procedures d'affichage des barres d'outils---###

    # def afficheCanvasVents(self):
    #     print(self.canvas_.scale())
    #     self.layers.reverse()
    #     self.canvas_.setLayers(self.layers)
    #     self.canvas_.refresh()
    #     if (self.canvas_.scale() < 1400000000):
    #         self.windmapdisplayed =True

    def cleanToolbar(self, toolbar) :
        for action in toolbar.actions():
            action.setVisible(False)
            self.canvas_.setMapTool(QgsMapToolPan(self.canvas_))

    def affichageToolsActions(self) :
        self.cleanToolbar(self.left_toolbar_)
        self.actionLargageCEau.setVisible(True)
        self.actionLargageCEau.setChecked(False)
        self.actionLargageHEau.setVisible(True)
        self.actionLargageHEau.setChecked(False)
        self.actionLargageCRetardant.setVisible(True)
        self.actionLargageCRetardant.setChecked(False)
        self.actionLargageHRetardant.setVisible(True)
        self.actionLargageHRetardant.setChecked(False)
        self.actionAttaqueJalonnement.setVisible(True)
        self.actionAttaqueJalonnement.setChecked(False)
        self.actionLigneAppui.setVisible(True)
        self.actionLigneAppui.setChecked(False)
        self.actionObstacle.setVisible(True)
        self.actionObstacle.setChecked(False)
        self.actionCancelAction.setVisible(True)
    
    def affichageToolsVents(self):
        self.cleanToolbar(self.left_toolbar_)
        self.roseVents.setVisible(True)
        self.roseVents.setChecked(False)
        self.calculVents.setVisible(True)
        self.afficheVents.setVisible(True)
        self.affichePente.setVisible(True)
        self.afficheVentsPentes.setVisible(True)
        self.affichePlusDense.setVisible(True)
        self.afficheMoinsDense.setVisible(True)
        self.reloadMap.setVisible(True)



    
    def affichageToolsSimulation(self) : 
        self.cleanToolbar(self.left_toolbar_)
        self.actionEmitPoint.setVisible(True)
        self.actionEmitPoint.setChecked(False)
        self.actionCancelContour.setVisible(True)
        self.simuler.setVisible(True)

    ###---Procedures d'affichages des elements de la carte---###

    def reload(self):
        print("Reload!")
        self.windMatrix_ = self.windMatrixInit('..\\data\\maps\\map.json')

        if self.windLayer_ in self.layers:
            self.layers.remove(self.windLayer_)

        if self.slopeLayer_ in self.layers:
            self.layers.remove(self.slopeLayer_)
        if self.windSlopeLayer_ in self.layers:
            self.layers.remove(self.windSlopeLayer_)

        self.slopeLayer_ = self.setWindLayer('255,150,0,255', self.densite, "slope")
        self.windLayer_ = self.setWindLayer('0,0,255,255', self.densite, "wind")
        self.windSlopeLayer_ = self.setWindLayer('255,0,0,255', self.densite, "windslope")

    def zoomAvant(self):
        self.canvas_.zoomByFactor(0.8)

    def zoomArriere(self):
        self.canvas_.zoomByFactor(1.2)


    def affichagePlusDense(self):
        if self.densite>1:
            self.densite-=1
            self.windMapRefresh()

    def affichageMoinsDense(self):
        if self.densite < 9:
            self.densite += 1
            self.windMapRefresh()

    def windMapRefresh(self):
        if self.slopeLayer_ in self.layers:
            self.layers.remove(self.slopeLayer_)
            self.slopeLayer_ = self.setWindLayer('255,150,0,255', self.densite, "slope")
            self.windLayer_ = self.setWindLayer('0,0,255,255', self.densite, "wind")
            self.windSlopeLayer_ = self.setWindLayer('255,0,0,255', self.densite, "windslope")
            self.layers.insert(0, self.slopeLayer_)
            self.canvas_.setLayers(self.layers)
        elif self.windLayer_ in self.layers:
            self.layers.remove(self.windLayer_)
            self.slopeLayer_ = self.setWindLayer('255,150,0,255', self.densite, "slope")
            self.windLayer_ = self.setWindLayer('0,0,255,255', self.densite, "wind")
            self.windSlopeLayer_ = self.setWindLayer('255,0,0,255', self.densite, "windslope")
            self.layers.insert(0, self.windLayer_)
            self.canvas_.setLayers(self.layers)
        elif self.windSlopeLayer_ in self.layers:
            self.layers.remove(self.windSlopeLayer_)
            self.slopeLayer_ = self.setWindLayer('255,150,0,255', self.densite, "slope")
            self.windLayer_ = self.setWindLayer('0,0,255,255', self.densite, "wind")
            self.windSlopeLayer_ = self.setWindLayer('255,0,0,255', self.densite, "windslope")
            self.layers.insert(0, self.windSlopeLayer_)
            self.canvas_.setLayers(self.layers)
        else:
            self.slopeLayer_ = self.setWindLayer('255,150,0,255', self.densite, "slope")
            self.windLayer_ = self.setWindLayer('0,0,255,255', self.densite, "wind")
            self.windSlopeLayer_ = self.setWindLayer('255,0,0,255', self.densite, "windslope")

    def afficheCanvasSlope(self):
        if self.slopeLayer_ in self.layers:
            self.layers.remove(self.slopeLayer_)
        else:
            self.layers.insert(0,self.slopeLayer_)

        if self.windLayer_ in self.layers :
            self.layers.remove(self.windLayer_)

        if self.windSlopeLayer_ in self.layers :
            self.layers.remove(self.windSlopeLayer_)

        self.canvas_.setLayers(self.layers)

    def afficheCanvasWindSlope(self):
        if self.windSlopeLayer_ in self.layers:
            self.layers.remove(self.windSlopeLayer_)
        else:
            self.layers.insert(0,self.windSlopeLayer_)

        if self.windLayer_ in self.layers :
            self.layers.remove(self.windLayer_)

        if self.slopeLayer_ in self.layers :
            self.layers.remove(self.slopeLayer_)

        self.canvas_.setLayers(self.layers)

    def afficheCanvasVents(self):
        if self.windLayer_ in self.layers:
            self.layers.remove(self.windLayer_)
        else:
            self.layers.insert(0,self.windLayer_)

        if self.slopeLayer_ in self.layers :
            self.layers.remove(self.slopeLayer_)
        if self.windSlopeLayer_ in self.layers :
            self.layers.remove(self.windSlopeLayer_)

        self.canvas_.setLayers(self.layers)



    def roseLayerInit(self,model):
        self.roseLayer_= QgsVectorLayer("Point", "Random Layer", "memory")
        symbol = QgsMarkerSymbol.createSimple({'name': 'star', 'color': 'white', 'size': '5','outline_color': 'black', 'outline_width': '1'})
        self.roseLayer_.renderer().setSymbol(symbol)
        provider1 = self.roseLayer_.dataProvider()
        fields1 = provider1.fields()
        features1=[]
        feature1 = QgsFeature()
        feature1.setGeometry(QgsGeometry.fromPointXY(model.roseVents_))
        feature1.setFields(fields1)
        features1.append(feature1)
        provider1.addFeatures(features1)
        self.roseLayer_.updateExtents()

    def propagationLayerInit(self, points,color1,color2):

        layer = QgsVectorLayer("Point?field=contour:double", "Propagation Layer", "memory")
        symbol = QgsMarkerSymbol.createSimple({'name': 'point', 'color': '255,0,0,255','size':'0.6', 'outline_color': '255,0,0,255', 'outline_width': '0'})
        layer.renderer().setSymbol(symbol)
        provider = layer.dataProvider()
        fields = provider.fields()
        features=[]
        list=[]
        i = 0
        for contour in points:
            for point in contour:
                feature = QgsFeature()
                feature.setGeometry(QgsGeometry.fromPointXY(point))
                list.append(point)
                feature.setFields(fields)
                feature.setAttribute("contour",i)
                features.append(feature)
            i+=1

        provider.addFeatures(features)

        fni = provider.fieldNameIndex('contour')
        unique_values = layer.dataProvider().uniqueValues(fni)
        categories = []
        color = color1
        r = int((color2[0]-color1[0])/i)
        v = int((color2[1]-color1[1])/i)
        b = int((color2[2]-color1[2])/i)


        for i in unique_values:
            symbol = QgsMarkerSymbol.createSimple(
                {'name': 'point', 'color': (str(color[0])+','+str(color[1])+','+str(color[2])+',255'),
                 'size':'0.65', 'outline_color': (str(color[0])+','+str(color[1])+','+str(color[2])+',255'), 'outline_width': '0'})
            category = QgsRendererCategory(i, symbol, str(i))
            # entry for the list of category items
            categories.append(category)
            color[0]=color[0]+r
            color[1]=color[1]+v
            color[2]=color[2]+b

        renderer = QgsCategorizedSymbolRenderer('contour', categories)
        layer.setRenderer(renderer)
        # self.propagationLayer_=layer
        # self.propagationLayer_.updateExtents()
        return layer

    def propagationLayerInit2(self, points,color):
        layer = QgsVectorLayer('Polygon?crs=epsg:4326', 'polygon', 'memory')
        properties = {}
        properties['color']= '0,0,0,0'
        properties['color_border']=color
        properties['width_border']='1'

        sym = QgsFillSymbol.createSimple(properties)
        layer.renderer().setSymbol(sym)
        prov = layer.dataProvider()
        # Add a new feature and assign the geometry
        feat = QgsFeature()

        # truc = self.trier(truc)
        feat.setGeometry(QgsGeometry.fromMultiPolygonXY([points]))
        prov.addFeatures([feat])
        # Update extent of the layer
        layer.updateExtents()
        # Add the layer to the Layers panel
        return layer

    def propagationLayerInitLine(self, points):
        layer = QgsVectorLayer('LineString?crs=epsg:4326', 'polygon', 'memory')
        # Set the provider to accept the data source
        prov = layer.dataProvider()
        # Add a new feature and assign the geometry
        feat = QgsFeature()

        # truc = self.trier(truc)
        feat.setGeometry(QgsGeometry.fromMultiPolylineXY(points))
        prov.addFeatures([feat])
        # Update extent of the layer
        layer.updateExtents()
        # Add the layer to the Layers panel
        return layer

    def distance(self,p0,p1):
        x = p0.x()-p1.x()
        y = p0.y()-p1.y()
        return math.sqrt(x * x + y * y)

    def trier(self,list):
        result = []
        p0= list[0]
        result.append(p0)
        list.remove(p0)
        while len(list)>1:
            p1 = list[0]
            for p in list:
                if(self.distance(p0,p)<self.distance(p0,p1)):
                    p1=p
            if(self.distance(p0,p1)<25 and self.distance(p0,p1)>3):
                result.append(p1)
                p0=p1
            list.remove(p1)

        return result



    def matrixInit(self,largeur, hauteur):
        matrix=[]
        for i in range(0,int(largeur)):
            matrix.append([])
            for j in range(0,int(hauteur)):
                matrix[i].append(0)
        return matrix

    def vector_to_angle(self,x,y):
        if x == 0 and y == 0:
            alpha = 0

        elif x == 0:
            alpha = 90 - ((y / abs(y)) * 90)

        elif y == 0:
            alpha = (x / abs(x)) * 90
        elif y < 0:
            alpha = 180 + math.degrees(math.atan(x / y))
        else:
            alpha = math.degrees(math.atan(x / y))

        return (alpha + 180) % 360

    def windMatrixInit(self,path):
        with open("..\\src\\Epilobe\\params.json ") as g:
            params= json.load(g)
        largeur=params['dimension'][1]
        hauteur=params['dimension'][0]

        windMatrix = self.matrixInit(largeur/25, hauteur/25)
        if largeur>0:
            with open(path, 'r') as f:
                windMap = json.load(f)

            xOrigin=params['origine'][0]-(largeur/2)
            yOrigin=params['origine'][1]-(hauteur/2)
            for wind in windMap:
                x = wind['wind'][0]
                y = wind['wind'][1]
                x1 = wind['windSlope'][0]
                y1 = wind['windSlope'][1]
                x2 =wind['slope_vector'][0]
                y2 =wind['slope_vector'][1]
                point = QgsPointXY(wind['x'], wind['y'])
                alpha = self.vector_to_angle(x,y)
                alpha1 = self.vector_to_angle(x1,y1)
                alpha2 = self.vector_to_angle(x2,y2)
                windMatrix[int((wind['x'] - xOrigin) / 25)-1][int((wind['y'] - yOrigin) / 25)-1] = [point,alpha,alpha1,alpha2]
        return windMatrix

    def setWindLayer(self,color,densite, type):
        layer = QgsVectorLayer("Point?field=angle:double", "Wind Layer", "memory")
        provider = layer.dataProvider()
        fields = provider.fields()
        features = []
        symbol = QgsMarkerSymbol.createSimple({'name': 'arrow', 'color': color, 'outline_color': 'black', 'outline_width': '0'})
        layer.renderer().setSymbol(symbol)
        i=0

        for col in self.windMatrix_:
            j = 0
            if i%densite==0:
            # print("i: " + str(i) + "%" + str(densite) + " = " + str(i % densite))
                for case in col:
                    print(case)
                    if j%densite==0 and (case is not None and case != 0):
                        print("Process: ")
                        print(case)
                        # print("j: " + str(j) + "%" + str(densite) + " = " + str(j % densite))
                        feature = QgsFeature()
                        feature.setGeometry(QgsGeometry.fromPointXY(case[0]))
                        feature.setFields(fields)
                        if type=="wind":
                            feature.setAttribute("angle", case[1])
                        elif type=="slope":
                            feature.setAttribute("angle", case[3])
                        elif type=="windslope":
                            feature.setAttribute("angle", case[2])
                        features.append(feature)
                    j+=1
            i+=1

        provider.addFeatures(features)
        layer.updateExtents()
        fni = provider.fieldNameIndex('angle')
        unique_values = layer.dataProvider().uniqueValues(fni)
        categories = []

        for i in unique_values:
            symbol = QgsMarkerSymbol.createSimple(
                {'name': 'arrow', 'color': color, 'outline_color': 'black', 'outline_width': '0'})
            symbol.setAngle(180 + i)
            symbol.setSize(4)
            category = QgsRendererCategory(i, symbol, str(i))
            # entry for the list of category items
            categories.append(category)

        renderer = QgsCategorizedSymbolRenderer('angle', categories)
        layer.setRenderer(renderer)
        return layer

    def update(self, model):
        if model.change():
            self.change(model.map_)
        self.update_contour(model)
        self.update_largage_eau_ABE(model)
        self.update_largage_eau_HBE(model)
        self.update_largage_retartant_HBE(model)
        self.update_largage_retartant_ABE(model)
        self.update_appui(model)
        self.update_jalonnement(model)
        self.update_obstacle(model)
        self.updateRose(model)

    def clear(self):
        self.canvas_.setLayers([])

    def updateRose(self, model):
        if self.roseLayer_ in self.layers:
            self.layers.remove(self.roseLayer_)
        self.roseLayerInit(model)
        self.layers.insert(0, (self.roseLayer_))
        self.roseLayer_.triggerRepaint()
        self.roseLayer_.reload()
        self.canvas_.setLayers(self.layers)
        # self.canvas_.refresh()

    def update_contour(self, model):
        list_points = model.contour_feu_

        if self.rubber_contour_feu_ == None:
            self.rubber_contour_feu_ = QgsRubberBand(self.canvas_, QgsWkbTypes.PolygonGeometry)
        polygon = QgsGeometry.fromPolygonXY([list_points])

        self.rubber_contour_feu_.setToGeometry(polygon, None)
        self.rubber_contour_feu_.setColor(QColor(255, 0, 0, 255))
        self.rubber_contour_feu_.setFillColor(QColor(255, 128, 0, 128))
        self.rubber_contour_feu_.setWidth(3)


    def update_largage_eau_ABE(self, model):
        list_points = model.largage_eau_ABE[len(model.largage_eau_ABE) - 1]
        if self.rubber_eau_ABE_ == None:
            self.rubber_eau_ABE_ = QgsRubberBand(self.canvas_, QgsWkbTypes.PolygonGeometry)
        polygon = QgsGeometry.fromMultiPolylineXY(model.largage_eau_ABE)
        self.rubber_eau_ABE_.setToGeometry(polygon, None)
        self.rubber_eau_ABE_.setColor(QColor(0, 110, 255, 255))
        self.rubber_eau_ABE_.setWidth(3)
        self.rubber_eau_ABE_.setLineStyle(Qt.DotLine)


        if len(list_points) > 1:

            dernier_point2 = QgsPointXY(list_points[len(list_points) - 2].x(), list_points[len(list_points) - 2].y())
            dernier_point = QgsPointXY(list_points[len(list_points) - 1].x(), list_points[len(list_points) - 1].y())
            p1 = QgsPointXY(dernier_point2.x(), dernier_point2.y())
            p2 = QgsPointXY(dernier_point.x(), dernier_point.y())
            largeur = p2.x() - p1.x()
            hauteur = p2.y() - p1.y()

            dernierSegment = []
            dernierSegment.append(p1)
            dernierSegment.append(p2)
            longueur = math.sqrt(largeur ** 2 + hauteur ** 2)
            nb_point = int(longueur / 15)



            self.eau_ABE_marker2 = QgsVertexMarker(self.canvas_)
            self.eau_ABE_marker2.setCenter(p1)
            self.eau_ABE_marker2.setColor(QColor(255, 255, 255))
            self.eau_ABE_marker2.setIconSize(4)
            self.eau_ABE_marker2.setIconType(QgsVertexMarker.ICON_BOX)
            self.eau_ABE_marker2.setPenWidth(4)
            self.eau_ABE_marker2.updatePosition()

            self.eau_ABE_marker = QgsVertexMarker(self.canvas_)
            self.eau_ABE_marker.setCenter(p1)
            self.eau_ABE_marker.setColor(QColor(0, 110, 255))
            self.eau_ABE_marker.setFillColor(QColor(0, 110, 255))
            self.eau_ABE_marker.setIconSize(8)
            self.eau_ABE_marker.setIconType(QgsVertexMarker.ICON_BOX)
            self.eau_ABE_marker.setPenWidth(2)
            self.eau_ABE_marker.updatePosition()


            for i in range(0, nb_point):
                dx = largeur / nb_point
                dy = hauteur / nb_point
                p = QgsPointXY(dernier_point.x() - dx * i, dernier_point.y() - dy * i)

                self.eau_ABE_marker2 = QgsVertexMarker(self.canvas_)
                self.eau_ABE_marker2.setCenter(p)
                self.eau_ABE_marker2.setColor(QColor(255, 255, 255))
                self.eau_ABE_marker2.setIconSize(4)
                self.eau_ABE_marker2.setIconType(QgsVertexMarker.ICON_BOX)
                self.eau_ABE_marker2.setPenWidth(4)
                self.eau_ABE_marker2.updatePosition()

                self.eau_ABE_marker = QgsVertexMarker(self.canvas_)
                self.eau_ABE_marker.setCenter(p)
                self.eau_ABE_marker.setColor(QColor(0, 110, 255))
                self.eau_ABE_marker.setFillColor(QColor(0, 110, 255))
                self.eau_ABE_marker.setIconSize(8)
                self.eau_ABE_marker.setIconType(QgsVertexMarker.ICON_BOX)
                self.eau_ABE_marker.setPenWidth(2)
                self.eau_ABE_marker.updatePosition()

    def update_largage_eau_HBE(self, model):
        list_points = model.largage_eau_HBE[len(model.largage_eau_HBE)-1]

        if self.rubber_eau_HBE_ == None:
            self.rubber_eau_HBE_ = QgsRubberBand(self.canvas_, QgsWkbTypes.PolygonGeometry)
        polygon = QgsGeometry.fromMultiPolylineXY(model.largage_eau_HBE)
        self.rubber_eau_HBE_.setToGeometry(polygon, None)
        self.rubber_eau_HBE_.setColor(QColor(0, 110, 255, 255))
        self.rubber_eau_HBE_.setWidth(3)
        self.rubber_eau_HBE_.setLineStyle(Qt.DotLine)


        if len(list_points) > 1:
            dernier_point2 = QgsPointXY(list_points[len(list_points) - 2].x(), list_points[len(list_points) - 2].y())
            dernier_point = QgsPointXY(list_points[len(list_points) - 1].x(), list_points[len(list_points) - 1].y())
            p1 = QgsPointXY(dernier_point2.x(), dernier_point2.y())
            p2 = QgsPointXY(dernier_point.x(), dernier_point.y())
            largeur = p2.x() - p1.x()
            hauteur = p2.y() - p1.y()

            dernierSegment = []
            dernierSegment.append(p1)
            dernierSegment.append(p2)

            longueur = math.sqrt(largeur ** 2 + hauteur ** 2)
            nb_point = int(longueur / 15)

            self.ligne_jalonnement_marker = QgsVertexMarker(self.canvas_)
            self.ligne_jalonnement_marker.setCenter(p1)
            self.ligne_jalonnement_marker.setColor(QColor(0, 110, 255))
            self.ligne_jalonnement_marker.setFillColor(QColor(255, 255, 255))
            self.ligne_jalonnement_marker.setIconSize(10)
            self.ligne_jalonnement_marker.setIconType(QgsVertexMarker.ICON_CIRCLE)
            self.ligne_jalonnement_marker.setPenWidth(2)
            self.ligne_jalonnement_marker.updatePosition()


            for i in range(0, nb_point):
                dx = largeur / nb_point
                dy = hauteur / nb_point
                p = QgsPointXY(dernier_point.x() - dx * i, dernier_point.y() - dy * i)
                self.ligne_jalonnement_marker = QgsVertexMarker(self.canvas_)
                self.ligne_jalonnement_marker. setCenter(p)
                self.ligne_jalonnement_marker.setColor(QColor(0, 110, 255))
                self.ligne_jalonnement_marker.setFillColor(QColor(255, 255, 255))
                self.ligne_jalonnement_marker.setIconSize(10)
                self.ligne_jalonnement_marker.setIconType(QgsVertexMarker.ICON_CIRCLE)
                self.ligne_jalonnement_marker.setPenWidth(2)
                self.ligne_jalonnement_marker.updatePosition()

    def update_largage_retartant_ABE(self, model):
        list_points = model.largage_retardant_ABE[len(model.largage_retardant_ABE) - 1]

        if self.rubber_retardant_ABE_ == None:
            self.rubber_retardant_ABE_ = QgsRubberBand(self.canvas_, QgsWkbTypes.PolygonGeometry)
        polygon = QgsGeometry.fromMultiPolylineXY(model.largage_retardant_ABE)
        self.rubber_retardant_ABE_.setToGeometry(polygon, None)
        self.rubber_retardant_ABE_.setColor(QColor(255, 0, 0, 255))
        self.rubber_retardant_ABE_.setWidth(3)
        self.rubber_retardant_ABE_.setLineStyle(Qt.DotLine)

        if len(list_points) > 1:

            dernier_point2 = QgsPointXY(list_points[len(list_points) - 2].x(), list_points[len(list_points) - 2].y())
            dernier_point = QgsPointXY(list_points[len(list_points) - 1].x(), list_points[len(list_points) - 1].y())

            p1 = QgsPointXY(dernier_point2.x(), dernier_point2.y())
            p2 = QgsPointXY(dernier_point.x(), dernier_point.y())
            largeur = p2.x() - p1.x()
            hauteur = p2.y() - p1.y()

            dernierSegment = []
            dernierSegment.append(p1)
            dernierSegment.append(p2)

            longueur = math.sqrt(largeur ** 2 + hauteur ** 2)
            nb_point = int(longueur / 15)



            self.retartant_ABE_marker2 = QgsVertexMarker(self.canvas_)
            self.retartant_ABE_marker2.setCenter(p1)
            self.retartant_ABE_marker2.setColor(QColor(255, 255, 255))
            self.retartant_ABE_marker2.setIconSize(4)
            self.retartant_ABE_marker2.setIconType(QgsVertexMarker.ICON_BOX)
            self.retartant_ABE_marker2.setPenWidth(4)

            self.retartant_ABE_marker = QgsVertexMarker(self.canvas_)
            self.retartant_ABE_marker.setCenter(p1)
            self.retartant_ABE_marker.setColor(QColor(255, 0, 0))
            self.retartant_ABE_marker.setFillColor(QColor(255, 255, 255))
            self.retartant_ABE_marker.setIconSize(8)
            self.retartant_ABE_marker.setIconType(QgsVertexMarker.ICON_BOX)
            self.retartant_ABE_marker.setPenWidth(2)

            for i in range(0, nb_point):
                dx = largeur / nb_point
                dy = hauteur / nb_point
                p = QgsPointXY(dernier_point.x() - dx * i, dernier_point.y() - dy * i)

                self.retartant_ABE_marker2 = QgsVertexMarker(self.canvas_)
                self.retartant_ABE_marker2.setCenter(p)
                self.retartant_ABE_marker2.setColor(QColor(255, 255, 255))
                self.retartant_ABE_marker2.setIconSize(4)
                self.retartant_ABE_marker2.setIconType(QgsVertexMarker.ICON_BOX)
                self.retartant_ABE_marker2.setPenWidth(4)

                self.retartant_ABE_marker = QgsVertexMarker(self.canvas_)
                self.retartant_ABE_marker. setCenter(p)
                self.retartant_ABE_marker.setColor(QColor(255, 0, 0))
                self.retartant_ABE_marker.setFillColor(QColor(255, 255, 255))
                self.retartant_ABE_marker.setIconSize(8)
                self.retartant_ABE_marker.setIconType(QgsVertexMarker.ICON_BOX)
                self.retartant_ABE_marker.setPenWidth(2)

    def update_largage_retartant_HBE(self, model):
        list_points = model.largage_retardant_HBE[len(model.largage_retardant_HBE) - 1]
        if self.rubber_retardant_HBE_ == None:
            self.rubber_retardant_HBE_ = QgsRubberBand(self.canvas_, QgsWkbTypes.PolygonGeometry)
        polygon = QgsGeometry.fromMultiPolylineXY(model.largage_retardant_HBE)
        self.rubber_retardant_HBE_.setToGeometry(polygon, None)
        self.rubber_retardant_HBE_.setColor(QColor(255, 0, 0, 255))
        self.rubber_retardant_HBE_.setWidth(3)
        self.rubber_retardant_HBE_.setLineStyle(Qt.DotLine)

        if len(list_points) > 1:

            dernier_point2 = QgsPointXY(list_points[len(list_points) - 2].x(), list_points[len(list_points) - 2].y())
            dernier_point = QgsPointXY(list_points[len(list_points) - 1].x(), list_points[len(list_points) - 1].y())

            p1 = QgsPointXY(dernier_point2.x(), dernier_point2.y())
            p2 = QgsPointXY(dernier_point.x(), dernier_point.y())
            largeur = p2.x() - p1.x()
            hauteur = p2.y() - p1.y()
            dernierSegment = []
            dernierSegment.append(p1)
            dernierSegment.append(p2)

            longueur = math.sqrt(largeur ** 2 + hauteur ** 2)
            nb_point = int(longueur / 15)

            self.eua_retartant_maker = QgsVertexMarker(self.canvas_)
            self.eua_retartant_maker.setCenter(p1)
            self.eua_retartant_maker.setColor(QColor(255, 0, 0))
            self.eua_retartant_maker.setFillColor(QColor(255, 255, 255))
            self.eua_retartant_maker.setIconSize(10)
            self.eua_retartant_maker.setIconType(QgsVertexMarker.ICON_CIRCLE)
            self.eua_retartant_maker.setPenWidth(2)

            for i in range(0, nb_point):
                dx = largeur / nb_point
                dy = hauteur / nb_point
                p = QgsPointXY(dernier_point.x() - dx * i, dernier_point.y() - dy * i)
                self.eua_retartant_maker = QgsVertexMarker(self.canvas_)
                self.eua_retartant_maker.setCenter(p)
                self.eua_retartant_maker.setColor(QColor(255, 0, 0))
                self.eua_retartant_maker.setFillColor(QColor(255, 255, 255))
                self.eua_retartant_maker.setIconSize(10)
                self.eua_retartant_maker.setIconType(QgsVertexMarker.ICON_CIRCLE)
                self.eua_retartant_maker.setPenWidth(2)

    def update_jalonnement(self, model):
        list_points = model.jalonnement_

        if self.rubber_jalonnement_ == None:
            self.rubber_jalonnement_ = QgsRubberBand(self.canvas_, QgsWkbTypes.PolygonGeometry)
        polygon = QgsGeometry.fromMultiPolylineXY(list_points)
        self.rubber_jalonnement_.setToGeometry(polygon, None)
        self.rubber_jalonnement_.setColor(QColor(0, 0, 0, 255))
        self.rubber_jalonnement_.setLineStyle(Qt.DashLine)
        self.rubber_jalonnement_.setWidth(3)


    def update_appui(self, model):
        list_points = model.appui_

        if self.rubber_appui1_ == None:
            self.rubber_appui1_ = QgsRubberBand(self.canvas_, QgsWkbTypes.PolygonGeometry)
        if self.rubber_appui2_==None:
            self.rubber_appui2_ = QgsRubberBand(self.canvas_, QgsWkbTypes.PolygonGeometry)

        polygon = QgsGeometry.fromMultiPolylineXY(list_points)
        self.rubber_appui1_.setToGeometry(polygon, None)
        self.rubber_appui1_.setColor(QColor(0, 0, 0, 255))
        self.rubber_appui1_.setLineStyle(Qt.SolidLine)
        self.rubber_appui1_.setWidth(8)

        self.rubber_appui2_.setToGeometry(polygon, None)
        self.rubber_appui2_.setColor(QColor(255, 255, 255, 255))
        self.rubber_appui2_.setLineStyle(Qt.SolidLine)
        self.rubber_appui2_.setWidth(4)



    def update_obstacle(self, model):
        list_points = model.obstacle_[len(model.obstacle_) - 1]

        if self.rubber_obstacle_ == None:
            self.rubber_obstacle_ = QgsRubberBand(self.canvas_, QgsWkbTypes.PolygonGeometry)

        polygon = QgsGeometry.fromMultiPolylineXY(model.obstacle_)
        self.rubber_obstacle_.setToGeometry(polygon, None)
        self.rubber_obstacle_.setColor(QColor(170, 60, 170, 255))
        self.rubber_obstacle_.setLineStyle(Qt.DotLine)
        self.rubber_obstacle_.setWidth(3)

        if len(list_points)>1:
            dernier_point2 = QgsPointXY(list_points[len(list_points) - 2].x(), list_points[len(list_points) - 2].y())
            dernier_point = QgsPointXY(list_points[len(list_points) - 1].x(), list_points[len(list_points) - 1].y())
            p1 = QgsPointXY(dernier_point2.x(), dernier_point2.y())
            p2 = QgsPointXY(dernier_point.x(), dernier_point.y())

            largeur = p2.x() - p1.x()
            hauteur = p2.y() - p1.y()
            longueur = math.sqrt(largeur ** 2 + hauteur ** 2)
            nb_point = int(longueur / 25)


            dernierSegment = []
            dernierSegment.append(p1)
            dernierSegment.append(p2)
            for i in range(0, nb_point):
                dx = largeur / nb_point
                dy = hauteur / nb_point
                p = QgsPointXY(dernier_point.x() - dx * i, dernier_point.y() - dy * i)
                self.obstacle_marker = QgsVertexMarker(self.canvas_)
                self.obstacle_marker.setCenter(p)
                self.obstacle_marker.setColor(QColor(170, 60, 170))
                self.obstacle_marker.setFillColor(QColor(255, 255, 255))
                self.obstacle_marker.setIconSize(10)
                self.obstacle_marker.setIconType(QgsVertexMarker.ICON_X)
                self.obstacle_marker.setPenWidth(2)
                self.obstacle_marker.updatePosition()

    ###---Procedures de chargement du fond de carte---###

    def change(self, departement):
        datas = os.path.abspath(os.path.join(os.path.dirname( __file__ ), '../../..', "data/altimetrics/departements", departement))
        ext = ".jp2"
        self.thread_ = MapLoader(self, datas, ext)
        self.thread_.end.connect(self.finish)
        self.thread_.progress.connect(self.progress)
        self.thread_.start()
        
    def finish(self, layers):
        self.layers = layers
        self.canvas_.setExtent(layers[1].extent())
        self.canvas_.setLayers(layers)
        self.canvas_.refresh()
        self.progress(0)
        
    def progress(self, percent):
        self.progressbar_.setValue(percent)


    #Classe responsable du chargemnt de la carte
class MapLoader(QThread):
    end = pyqtSignal(list)
    progress = pyqtSignal(int)

    def __init__(self, santoline_view, datas, ext):
        QThread.__init__(self)
        self.datas_ = datas
        self.ext_ = ext
        
    def __del__(self):
        self.wait()
        
    def run(self):
        list = useful.allFiles(self.datas_, self.ext_)
        if (len(list) == 0 ):
            return
            
        layers = []
        total_len = len(list)
        def runner():
            try:
                while (len(list) > 0):
                    i = list.pop()
                    layer = QgsRasterLayer(i, i)
                    if (not layer.isValid()):
                        raise IOError("Fail to open the layer {}".format(i))
                    layers.append(layer)
                    self.progress.emit(100 - int((len(list) / total_len) * 100.))
            except IndexError: # catch if we pop an empty list.
                pass
                
        threads = [threading.Thread(target=runner) for i in range(useful.thread_quantity() - 1)]
        for thread in threads: thread.start()
        for thread in threads: thread.join()
        self.end.emit(layers)
