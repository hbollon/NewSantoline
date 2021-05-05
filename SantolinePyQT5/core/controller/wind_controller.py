from pathlib import Path
from ..model import *
from ..libs import file_io
from . import controller
from subprocess import PIPE
import subprocess
import json
import os


class WindController(controller.AController):
    def __init__(self, view, santolineView):
        super().__init__(view)
        self.windModel_ = wind_model.WindModel(self)
        self.windModel_.addObserver(view)
        self.santoline_view_ = santolineView

    def distance(self, distance):
        self.windModel_.north(distance)
        self.windModel_.south(distance)
        self.windModel_.east(distance)
        self.windModel_.west(distance)

    def direction(self, direction):
        self.windModel_.direction(direction)

    def speed(self, speed):
        self.windModel_.speed(speed)

    def process(self, process):
        self.windModel_.process(process)

    def origin(self, origin):
        self.direction((origin + 180.0) % 360)

    def speedkmh(self, value):
        self.speed(value / 3.6)

    def accept(self):
        connector = file_io.FileIO()
        if os.path.isfile("..\\data\\maps\\map.json"):
            os.remove("..\\data\\maps\\map.json")

        departementTifPath = None
        departementDir = "..\\data\\altimetrics\\departements\\{}\\tif\\".format(
            self.santoline_view_.controller_.canvasModel_.map_)
        for file in os.listdir(departementDir):
            if file.endswith(".tif"):
                departementTifPath = os.path.join(departementDir, file)
                break

        if departementTifPath == None:
            print("Aucun .tif trouvé, impossible de générer la carte des vents")
            self.close()
            return
        filename = "..\\src\\Epilobe\\params.json"
        connector.write(filename, json.dumps(
            self.windModel_.jsonify(departementTifPath)))

        parser_commande = "..\\src\\Epilobe\\cmake\\Epilobe.exe " \
                          + "..\\paths.json " \
                          + "..\\src\\Epilobe\\params.json " \
                          + "\"{}\" ".format(departementTifPath) \
                          + str((self.windModel_.direction_ + 180.) % 360.) \
                          + " " \
                          + str(round(self.windModel_.speed_, 1))
        proc = subprocess.Popen(
            parser_commande, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
        while proc.poll() is None:
            line = proc.stdout.readline()
            output = line.decode('utf-8').strip()
            if "Run 0 (solver)" in output:
                number = ''.join([n for n in output if n.isdigit()])
                self.santoline_view_.progressbar_.setValue(int(number[1:]))

        if os.path.isfile("..\\data\\maps\\map.json"):
            print("Carte des vents générée")
            if self.santoline_view_.roseLayer_ in self.santoline_view_.layers:
                self.santoline_view_.layers.remove(
                    self.santoline_view_.roseLayer_)
                self.santoline_view_.roseLayer_ = None
            if self.santoline_view_.windLayer_ in self.santoline_view_.layers:
                self.santoline_view_.layers.remove(
                    self.santoline_view_.windLayer_)
            if self.santoline_view_.slopeLayer_ in self.santoline_view_.layers:
                self.santoline_view_.layers.remove(
                    self.santoline_view_.slopeLayer_)
            if self.santoline_view_.windSlopeLayer_ in self.santoline_view_.layers:
                self.santoline_view_.layers.remove(
                    self.santoline_view_.windSlopeLayer_)
            self.santoline_view_.windMatrix_ = self.santoline_view_.windMatrixInit(
                '..\\data\\maps\\map.json')
            self.santoline_view_.slopeLayer_ = self.santoline_view_.setWindLayer(
                '255,150,0,255', self.santoline_view_.densite, "slope")
            self.santoline_view_.windLayer_ = self.santoline_view_.setWindLayer(
                '0,0,255,255', self.santoline_view_.densite, "wind")
            self.santoline_view_.windSlopeLayer_ = self.santoline_view_.setWindLayer(
                '255,0,0,255', self.santoline_view_.densite, "windslope")
            self.santoline_view_.canvas_.setLayers(self.santoline_view_.layers)
            self.santoline_view_.afficheVentsPentes.setChecked(False)
            self.santoline_view_.afficheVents.setChecked(False)
            self.santoline_view_.canvas_.refresh()
            # récuprération du chemin absolu du dossier data
            subzonePath = Path(__file__).resolve().parents[3] / "data"
            # pour tous les fichier subzone dans le dossier data
            for p in subzonePath.glob("subzone*"):
                # supression du fichier
                p.unlink()
            # affichier la carte des vents
            self.santoline_view_.afficheCanvasVents()
            # remettre la progressbar à 0
            self.santoline_view_.progressbar_.setValue(0)
        else:
            print("Carte des vents non générée")
            # affichage d'une popup d'erreur
            self.santoline_view_.controller_.showPopup(
                "Carte des vents non générée", "Erreur")
            # on créé un json avec des valeurs par défault
            emptyJson = {
                "axeorigine": "est",
                "direction": 0,
                "force": 0,
                "nbProcess": 0,
                "dimension": [0, 0],
                "origine": [0, 0]
            }
            # on écrit le json dans params.json pour remplacer ce qu'y a été commencer
            connector.write(filename, json.dumps(emptyJson))
        self.close()

    def close(self):
        self.view_.hide()
