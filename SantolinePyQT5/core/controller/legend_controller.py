from ..model import *
from . import controller

import re

class Legend_Controller(controller.AController):
    def __init__(self, view):
        super().__init__(view)
    
    def close(self):
        self.view_.hide()
    