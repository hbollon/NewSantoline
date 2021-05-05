from . import model

class Departements_Choices_Model(model.AModel):
    def __init__(self, controller):
        super().__init__(controller)
        self.liste_departements = []
        self.departement_courant = None
        
    def addDepartement(self, departement):
        self.liste_departements.append(departement)
        
    def departementCourant(self, departement):
        self.departement_courant = departement