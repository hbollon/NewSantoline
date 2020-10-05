from . import model
from qgis.PyQt.QtCore import QTime
class AlgorithmModel(model.AModel):
    def __init__(self, controller):
        super().__init__(controller)
        self.algorithm_ = "2"
        self.heure_ = 0
        self.minute_ = 0
        self.heureIntervalle_ = 0
        self.minuteIntervalle_ = 0
        self.waterReserve_ = 40
        self.temperature_ = 30
        self.process_ = 0
        self.dimension_ = 25
        self.nbDivisionCellule_ = 25
        
    def algorithm(self, algorithm):
        self.algorithm_ = algorithm
        self.notifyObservers()
        
    def heure(self, heure):
        self.heure_ = heure
        self.notifyObservers()

    def minute(self, minute):
        self.minute_ = minute
        self.notifyObservers()

    def heureIntervalle(self, heure):
        self.heureIntervalle_ = heure;
        self.notifyObservers()

    def minuteIntervalle(self, minute):
        self.minuteIntervalle_=minute;
        self.notifyObservers()
        
    def waterReserve(self, waterReserve):
        self.waterReserve_ = waterReserve
        self.notifyObservers()
        
    def temperature(self, temperature):
        self.temperature_ = temperature
        self.notifyObservers()

    def process(self, process):
        self.process_ = process
        self.notifyObservers()
    
    def dimension(self, dimension):
        self.dimension_ = dimension
        self.notifyObservers()

    def nbDivisionCellule(self, nbDivisionCellule):
        self.nbDivisionCellule_ = nbDivisionCellule
        self.notifyObservers()
        
    def jsonify(self):
        duree = self.duree_.hour() + self.duree_.minute() / 60.
        return {
            "type" : "parametrealgorithme",
            "json" : {
                "paramSim" : {
                    "temperature" : round(self.temperature_,1),
                    "waterReserve" : round(self.waterReserve_,1),
                    "duration" : round(duree,1),
                    "intervalle": self.heureIntervalle_,
                    "nbProcess" : round(self.process_,1),
                    "algorithm" : self.algorithm_,
                    "dimension" : round(25.0,1)
                }
            }
        }
    