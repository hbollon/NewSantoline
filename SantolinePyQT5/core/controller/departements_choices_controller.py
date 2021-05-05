from ..model import *
from . import controller

import re
import os


class Departements_Choices_Controller(controller.AController):
    def __init__(self, view):
        super().__init__(view)
        self.model_ = departements_choices_model.Departements_Choices_Model(
            self)
        self.initDepartements()

    def initDepartements(self):
        for dep in next(os.walk('../data/altimetrics/departements'))[1]:
            self.model_.addDepartement(dep)

    def itemSelectionChanged_event(self):
        matches = self.view_.listWidget.selectedItems()[0].text()
        if matches:
            self.model_.departementCourant(matches)

    def accept(self, departement):
        self.close()

    def close(self):
        self.view_.hide()
