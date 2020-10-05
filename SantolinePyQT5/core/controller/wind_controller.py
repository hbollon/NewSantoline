import re
from pathlib import Path

from ..model import *
from ..libs import file_io
from . import controller

import json
import os


class WindController(controller.AController):
    def __init__(self, view,santolineView):
        super().__init__(view)
        self.windModel_ = wind_model.WindModel(self)
        self.windModel_.addObserver(view)
        self.santoline_view_ = santolineView

    # def width(self, width):
    #     self.windModel_.width(width)
    #
    # def height(self, height):
    #     self.windModel_.height(height)

    def north(self,distance):
        self.windModel_.north(distance)

    def south(self,distance):
        self.windModel_.south(distance)

    def east(self,distance):
        self.windModel_.east(distance)

    def west(self,distance):
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
        newArchPath = os.path.abspath("..\\src")
        if os.path.isfile("..\\data\\maps\\map.json"):
            os.remove("..\\data\\maps\\map.json")

        filename = newArchPath + "\\Epilobe\\params.json"
        connector.write(filename, json.dumps(self.windModel_.jsonify()))

        parser_commande = newArchPath + "\\Epilobe\\cmake-build-debug\\Epilobe.exe " \
                          +  "..\\paths.json " \
                          + newArchPath + "\\Epilobe\\params.json " \
                          + str((self.windModel_.direction_ + 180.)%360.) \
                          + " " \
                          + str(round(self.windModel_.speed_, 1))
        os.system(parser_commande)

        if os.path.isfile("..\\data\\maps\\map.json"):
            print("Carte des vents générée")

            if self.santoline_view_.roseLayer_ in self.santoline_view_.layers:
                self.santoline_view_.layers.remove(self.santoline_view_.roseLayer_)
                self.santoline_view_.roseLayer_= None

            if self.santoline_view_.windLayer_ in self.santoline_view_.layers:
                self.santoline_view_.layers.remove(self.santoline_view_.windLayer_)

            if self.santoline_view_.slopeLayer_ in self.santoline_view_.layers:
                self.santoline_view_.layers.remove(self.santoline_view_.slopeLayer_)
            if self.santoline_view_.windSlopeLayer_ in self.santoline_view_.layers:
                self.santoline_view_.layers.remove(self.santoline_view_.windSlopeLayer_)


            self.santoline_view_.windMatrix_=self.santoline_view_.windMatrixInit('..\\data\\maps\\map.json')
            self.santoline_view_.slopeLayer_ = self.santoline_view_.setWindLayer('255,150,0,255', self.santoline_view_.densite, "slope")
            self.santoline_view_.windLayer_ = self.santoline_view_.setWindLayer('0,0,255,255', self.santoline_view_.densite, "wind")
            self.santoline_view_.windSlopeLayer_ = self.santoline_view_.setWindLayer('255,0,0,255', self.santoline_view_.densite, "windslope")
            self.santoline_view_.canvas_.setLayers(self.santoline_view_.layers)
            self.santoline_view_.afficheVentsPentes.setChecked(False)
            self.santoline_view_.afficheVents.setChecked(False)
            self.santoline_view_.canvas_.refresh()
            for p in Path("..\\data").glob("subzone*"):
                p.unlink()
        else:
            print("Carte des vents non générée")
            emptyJson = {

                "axeorigine": "est",
                "direction": 0,
                "force": 0,
                "nbProcess": 0,
                "dimension": [0, 0],
                "origine": [0, 0]

            }
            connector.write(filename, json.dumps(emptyJson))

        self.close()

    def close(self):
        self.view_.hide()
