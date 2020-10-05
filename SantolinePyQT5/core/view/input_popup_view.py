from qgis.gui import *#QgsMapCanvas
from qgis.core import *#QgsRasterLayer, QgsApplication
from qgis.PyQt.QtWidgets import *#QMainWindow, QWidget, QTabWidget, QPushButton
from qgis.PyQt.QtCore import *#QSize
from qgis.PyQt.QtGui import *#QFrame

from ..libs import observable

class InputPopup(QWidget, observable.Observer):
    def __init__(self, controller = None):
        super().__init__(None)
        self.setWindowModality(QtCore.Qt.ApplicationModal)
        self.controller_ = controller
        self.initUI()
        
    def initUI(self):
        # To implement in subclass
        pass