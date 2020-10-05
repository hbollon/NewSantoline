import os
import subprocess

from qgis._core import *

from ..model import algorithm_model, canvas_model
from ..libs import file_connector
from . import controller, canvas_controller
from ..model import canvas_model

import json
import threading

class AlgorithmController(controller.AController):
    def __init__(self, view, canvasModel, santoView):
        super().__init__(view)
        self.algorithmModel_ = algorithm_model.AlgorithmModel(self)
        self.algorithmModel_.addObserver(view)
        self.canvasModel_=canvasModel
        self.santo_view_= santoView
        
    def algorithm(self, algorithm):
        self.algorithmModel_.algorithm(algorithm)
        
    def heure(self, heure):
        self.algorithmModel_.heure(heure)

    def minute(self, minute):
        self.algorithmModel_.minute(minute)

    def heureIntervalle(self, heure):
        self.algorithmModel_.heureIntervalle(heure)

    def minuteIntervalle(self, minute):
        self.algorithmModel_.minuteIntervalle(minute)
        
    def waterReserve(self, waterReserve):
        self.algorithmModel_.waterReserve(waterReserve)
        
    def temperature(self, temperature):
        self.algorithmModel_.temperature(temperature)

    def process(self, process):
        self.algorithmModel_.process(process)
        
    def accept(self):

        with open(b"..\\data\\reglage.json", 'r', encoding='utf-8') as f:
            reglage = json.load(f)
        if self.santo_view_.propagationLayer_ in self.santo_view_.layers:
            self.santo_view_.layers.remove(self.santo_view_.propagationLayer_)
        if self.santo_view_.propagationLayer2_ in self.santo_view_.layers:
            self.santo_view_.layers.remove(self.santo_view_.propagationLayer2_)
        self.canvasModel_.propagation_=[]
        self.canvasModel_.propagation2_=[]
        duree =  str(self.algorithmModel_.heure_) +":"+ str(self.algorithmModel_.minute_ )+":00"
        intervalle =  str(self.algorithmModel_.heureIntervalle_) +":"+ str(self.algorithmModel_.minuteIntervalle_ )+":00"
        #if os.path.isfile("..\\data\\communication\\resultatSimulation.json"):
            #os.remove("..\\data\\communication\\resultatSimulation.json")
        print("Simulation lancee \n")
        with open("..\\src\\Epilobe\\params.json ") as g:
            params= json.load(g)
        largeur=params['dimension'][1]
        hauteur=params['dimension'][0]

        with open('..\\data\\communication\\parametreAlgo.json', 'w') as outfile:
            json.dump({"algorithm": self.algorithmModel_.algorithm_,
                        "waterReserve": self.algorithmModel_.waterReserve_,
                        "duree":duree,
                       "intervalle":intervalle,
                        "temperature": self.algorithmModel_.temperature_,
                        "nbProcess": 2,
                        "dimension": 25,
                       "exentricite" : reglage['exentricite']/100,
                       "rateOfSpread" : reglage['rateOfSpread'] / 100,
                       "coef": reglage['coef']/100,
                       "generations" : reglage['generations'],
                       "segment" : reglage['segment'],
                       "angle" : reglage['angle'],
                       "nbDivisionCellule": 25,
                        "contourInitial":self.canvasModel_.jsonify(),
                       "largeur":largeur,
                       "hauteur":hauteur,
                       "listeObstacle": self.canvasModel_.obstacleJsonify()}, outfile)
        self.close()
        # lancer le programme algo avec les parametre algo et la carte de vent et la sortie est le resultat  de simulation

        if self.algorithmModel_.algorithm_=="3":
            var = subprocess.Popen([
                "..\\Algo3\\cmake-build-debug\\Algo3.exe",
                "..\\data\\communication\\parametreAlgo.json",
                "..\\data\\maps\\map.json",
                "..\\data\\communication\\resultatSimulation.json",
                "..\\data\\communication\\sortie.json",
                "..\\data\\communication\\points.json",
                "..\\data\\communication\\infos.json",
				"..\\data\\communication\\listeSommets.json"
            ],
            shell=True,
            stdout=subprocess.PIPE,
            universal_newlines=True).stdout
            
            data = var.readlines()
        else:
            var = subprocess.Popen([
                "..\\src\\algo\\cmake-build-debug\\algo.exe",
                "..\\data\\communication\\parametreAlgo.json",
                "..\\data\\maps\\map.json",
                "..\\data\\communication\\resultatSimulation.json"
            ],
            shell=True,
            stdout=subprocess.PIPE,
            universal_newlines=True).stdout

            data = var.readlines()

        # recuperer le resultat de simulation
        resultatSimulation = []
        with open(b"..\\data\\communication\\resultatSimulation.json", 'r', encoding='utf-8') as f:
            resultatSimulation = json.load(f)
            if resultatSimulation!=None:
                # print(resultatSimulation)

                # renvoyer le resultat au santoline_view
                for l in resultatSimulation:
                    liste=[]
                    for p in l:
                        qp = QgsPointXY(p[0], p[1])
                        liste.append(qp)
                        # self.canvasModel_.propagation_.append(qp)

                    self.canvasModel_.propagation_.append(liste)
                # with open(b"..\\data\\communication\\resultatSimulation2.json", 'r', encoding='utf-8') as k:
                #     resultatSimulation = json.load(k)
                #     if resultatSimulation != None:
                #         for l in resultatSimulation:
                #             liste = []
                #             for p in l:
                #                 qp = QgsPointXY(p[0], p[1])
                #                 liste.append(qp)
                #             self.canvasModel_.propagation2_.append(liste)
                print("\nSimulation terminee \n")
                #☺self.santo_view_.propagationLayer_ = self.santo_view_.propagationLayerInit(self.canvasModel_.propagation_,[255,0,0],[0,0,255])
                self.santo_view_.propagationLayer_ = self.santo_view_.propagationLayerInit2(self.canvasModel_.propagation_,"255,0,0,255")
                #En inversant les deux lignes (commenter l'une decommenter l'autre) on peut passer du mode lisse ou du mode point
                self.santo_view_.propagationLayer_.updateExtents()
                # self.santo_view_.propagationLayer2_ = self.santo_view_.propagationLayerInitLine(self.canvasModel_.propagation2_)
                # print("Propagation: "+str(len(self.canvasModel_.propagation_)))
                # self.santo_view_.propagationLayer2_.updateExtents()
                self.santo_view_.layers.insert(0, (self.santo_view_.propagationLayer_))
                # self.santo_view_.layers.insert(0, (self.santo_view_.propagationLayer2_))
                # self.santo_view_.propagationLayer2_.triggerRepaint()
                # self.santo_view_.propagationLayer2_.reload()
                self.santo_view_.propagationLayer_.triggerRepaint()
                self.santo_view_.propagationLayer_.reload()
                self.santo_view_.canvas_.setLayers(self.santo_view_.layers)
                self.santo_view_.canvas_.refresh()
                self.santo_view_.update(self.canvasModel_)
                if self.algorithmModel_.algorithm_ == "double":
                    with open(b"..\\data\\communication\\resultatSimulation.json", 'r', encoding='utf-8') as k:
                        resultatSimulation = json.load(k)
                        if resultatSimulation != None:
                            for l in resultatSimulation:
                                liste = []
                                for p in l:
                                    qp = QgsPointXY(p[0], p[1])
                                    liste.append(qp)
                                self.canvasModel_.propagation2_.append(liste)
                            print("\nSimulation terminee \n")
                            self.santo_view_.propagationLayer2_ =self.santo_view_.propagationLayerInit2(self.canvasModel_.propagation2_,'0,0,255,255')
                            self.santo_view_.propagationLayer2_.updateExtents()
                            self.santo_view_.layers.insert(0, (self.santo_view_.propagationLayer2_))
                            self.santo_view_.propagationLayer2_.triggerRepaint()
                            self.santo_view_.propagationLayer2_.reload()
                            self.santo_view_.canvas_.setLayers(self.santo_view_.layers)
                            self.santo_view_.canvas_.refresh()
                            self.santo_view_.update(self.canvasModel_)
                else:
                    print("self.algorithmModel_.algorithm_ est differentde double")
            else:
                print("Simulation echouee")
        
    def close(self):
        self.view_.hide()
