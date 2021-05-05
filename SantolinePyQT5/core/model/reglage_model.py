import json

from . import model


class ReglageModel(model.AModel):
    def __init__(self, controller):
        super().__init__(controller)
        self.exentricite_ = None
        self.rateOfSpread_ = None
        self.angle_ = None
        self.segment_ = None
        self.coef_ = None
        self.generations_ = None
        self.back()

    def exentricite(self, value):
        self.exentricite_ = value
        self.notifyObservers()

    def rateOfSpread(self, value):
        self.rateOfSpread_ = value
        self.notifyObservers()

    def angle(self, value):
        self.angle_ = value
        self.notifyObservers()

    def segment(self, value):
        self.segment_ = value
        self.notifyObservers()

    def coef(self, value):
        self.coef_ = value
        self.notifyObservers()

    def generations(self, value):
        self.generations_ = value
        self.notifyObservers()

    def back(self):
        with open(b"..\\data\\reglage.json", 'r', encoding='utf-8') as f:

            reglages = json.load(f)
        self.exentricite_ = reglages['exentricite']
        self.rateOfSpread_ = reglages['rateOfSpread']
        self.angle_ = reglages['angle']
        self.segment_ = reglages['segment']
        self.coef_ = reglages['coef']
        self.generations_ = reglages['generations']
