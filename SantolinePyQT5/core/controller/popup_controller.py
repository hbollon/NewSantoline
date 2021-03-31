from ..model import *
from . import controller

class Popup_Controller(controller.AController):
    def __init__(self, view):
        super().__init__(view)
        self.model_ = popup_model.Popup_Model(self)
    
    def close(self):
        self.view_.hide()
    