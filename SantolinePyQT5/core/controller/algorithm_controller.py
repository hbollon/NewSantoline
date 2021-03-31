import os
import subprocess

from qgis._core import *

from ..model import algorithm_model, canvas_model
from ..libs import file_connector
from . import controller, canvas_controller
from ..model import canvas_model
from subprocess import PIPE

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
            sub = subprocess.run([
                "..\\Algo3\\cmake\\Algo3.exe",
                "..\\data\\communication\\parametreAlgo.json",
                "..\\data\\maps\\map.json",
                "..\\data\\communication\\resultatSimulation.json",
                "..\\data\\communication\\sortie.json",
                "..\\data\\communication\\points.json",
                "..\\data\\communication\\infos.json",
				"..\\data\\communication\\listeSommets.json"
            ], 
            shell=True, stdout=PIPE, stderr=PIPE)

            print(f"Output:\n{sub.stdout}\nErr:\n{sub.stderr}\nReturnCode: {sub.returncode}")
        else:
            sub = subprocess.Popen([
                "..\\src\\algo\\cmake\\algo.exe",
                "..\\data\\communication\\parametreAlgo.json",
                "..\\data\\maps\\map.json",
                "..\\data\\communication\\resultatSimulation.json"
            ], 
            stdout=subprocess.PIPE,bufsize=1)
            lines_iterator = iter(sub.stdout.readline, b"")
            while sub.poll() is None:
                for line in lines_iterator:
                    nline = line.rstrip().decode("latin")
                    if nline.isnumeric():
                        print(nline)
                        self.santo_view_.progressbar_.setValue(int(nline))
            self.santo_view_.progressbar_.setValue(100)
        resultatSimulation = []
        with open(b"..\\data\\communication\\resultatSimulation.json", 'r', encoding='utf-8') as f:
            resultatSimulation = json.load(f)
            if resultatSimulation!=None:
                for l in resultatSimulation:
                    liste=[]
                    for p in l:
                        qp = QgsPointXY(p[0], p[1])
                        liste.append(qp)
                    self.canvasModel_.propagation_.append(liste)
                self.santo_view_.propagationLayer_ = self.santo_view_.propagationLayerInit2(self.canvasModel_.propagation_,"255,0,0,255")
                self.santo_view_.propagationLayer_.updateExtents()
                self.santo_view_.layers.insert(0, (self.santo_view_.propagationLayer_))
                self.santo_view_.propagationLayer_.triggerRepaint()
                self.santo_view_.propagationLayer_.reload()
                self.santo_view_.canvas_.setLayers(self.santo_view_.layers)
                self.santo_view_.canvas_.refresh()
                self.santo_view_.update(self.canvasModel_)
            else:
                self.santo_view_.controller_.showPopup("Simulation échouée", "Erreur")
        self.santo_view_.progressbar_.setValue(0)
        
    def close(self):
        self.view_.hide()
