from ..libs import *

class AModel(observable.Observable, metaclass=singleton.Singleton):
    def __init__(self, controler):
        super().__init__()
        self.controler_ = controler
        
    def jsonify(self):
        pass
    