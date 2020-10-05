from ..model import *
from . import controller

import re

class Departements_Choices_Controller(controller.AController):
    def __init__(self, view):
        super().__init__(view)
        self.model_ = departements_choices_model.Departements_Choices_Model(self)
        self.initDepartements()
    
    def initDepartements(self):
        self.model_.addDepartement("Alpes Maritimes (06)")
        self.model_.addDepartement("Var (83)")
        
    def itemSelectionChanged_event(self):
        matches = re.search('[0-9]+',self.view_.listWidget.selectedItems()[0].text())
        if matches : 
            departement = matches.group(0)
            self.model_.departementCourant(departement)
    
    def accept(self, departement):
        self.close()
    
    def close(self):
        self.view_.hide()
    