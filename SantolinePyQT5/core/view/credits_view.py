from qgis.gui import *#QgsMapCanvas
from qgis.core import *#QgsRasterLayer, QgsApplication
from qgis.PyQt.QtWidgets import *#QMainWindow, QWidget, QTabWidget, QPushButton
from qgis.PyQt.QtCore import *#QSize
from qgis.PyQt.QtGui import *#QFrame

class CreditsPopup(QWidget):
    def __init__(self):
        super().__init__(None)
        self.initUI()
        
    def initUI(self):
        # Ajouter les crédits ici
        pass