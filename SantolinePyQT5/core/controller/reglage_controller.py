import json

from core.model import reglage_model
from . import controller


class ReglageController(controller.AController):
    def __init__(self, view):
        super().__init__(view)
        self.model_ = reglage_model.ReglageModel(self)


    def accept(self):
        with open('..\\data\\reglage.json', 'w') as outfile:
            json.dump({"exentricite": self.model_.exentricite_,
                        "rateOfSpread": self.model_.rateOfSpread_,
                       "angle": self.model_.angle_,
                       "segment": self.model_.segment_,
                       "coef": self.model_.coef_,
                       "generations": self.model_.generations_

                       }, outfile)
        self.view_.hide()

    def annuler(self):
        self.model_.back()
        self.view_.update(self.model_)
        self.view_.hide()

    def rateOfSpread(self, value):
        self.model_.rateOfSpread(value)

    def exentricite(self, value):
        self.model_.exentricite(value)

    def angle(self, value):
        self.model_.angle(value)

    def coef(self, value):
        self.model_.coef(value)

    def segment(self, value):
        self.model_.segment(value)

    def generations(self,value):
        self.model_.generations(value)