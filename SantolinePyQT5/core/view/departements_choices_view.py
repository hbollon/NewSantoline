from qgis.PyQt.QtWidgets import *

from . import input_popup_view
from ..controller import *

class Departements_Choices_Window(input_popup_view.InputPopup):
    def __init__(self):
        
        self.valider_ = None
        self.annuler_ = None
        
        super().__init__(departements_choices_controller.Departements_Choices_Controller(self))

    def initUI(self):
        self.setWindowTitle("Choix du département")
        body = QVBoxLayout(self)
        
        self.listWidget = QListWidget()
        for departement in self.controller_.model_.liste_departements:
            item = QListWidgetItem(departement)
            self.listWidget.addItem(item)
        self.listWidget.itemSelectionChanged.connect(self.controller_.itemSelectionChanged_event)
        
        boutons = QWidget(self)
        boutons_layout = QHBoxLayout(boutons)
        self.valider_ = QPushButton("Valider", boutons)
        self.annuler_ = QPushButton("Annuler", boutons)
        
        boutons_layout.addWidget(self.valider_)
        boutons_layout.addWidget(self.annuler_)
        boutons.setLayout(boutons_layout)
        
        body.addWidget(self.listWidget)
        body.addWidget(boutons)
        
        self.setLayout(body)
        
        self.valider_.clicked.connect(self.controller_.accept)
        self.annuler_.clicked.connect(self.controller_.close)
        self.listWidget.show()
        
        