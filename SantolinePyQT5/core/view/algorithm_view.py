from qgis.gui import *#QgsMapCanvas
from qgis.core import *#QgsRasterLayer, QgsApplication
from qgis.PyQt.QtWidgets import *#QMainWindow, QWidget, QTabWidget, QPushButton
from qgis.PyQt.QtCore import *#QSize
from qgis.PyQt.QtGui import *#QFrame

from . import input_popup_view
from ..controller import *

class ParameterWindow(input_popup_view.InputPopup):
    def __init__(self,canvasModel,santoView):
        self.algorithme_nom_ = None
        self.algorithme_duree_heure_ = None
        self.algorithme_duree_minute_ = None
        self.algorithme_intervalle_heure_ = None
        self.algorithme_intervalle_minute_ = None
        
        self.algorithme_reserve_spinbox_ = None
        self.algorithme_temperature_spinbox_ = None
        self.algorithme_processus_spinbox_ = None
        
        self.valider_ = None
        self.annuler_ = None
        
        super().__init__(algorithm_controller.AlgorithmController(self,canvasModel,santoView))
        
    def initUI(self):
        self.setWindowTitle ("Paramètres Algorithmes")
        
        body = QVBoxLayout(self)
        
        algorithme = QWidget(self)
        algorithme_layout = QHBoxLayout(algorithme)
        algorithme_label = QLabel("Algorithme", algorithme)
        self.algorithme_nom_ = QLineEdit(algorithme)
        algorithme_layout.addWidget(algorithme_label)
        algorithme_layout.addWidget(self.algorithme_nom_)
        algorithme.setLayout(algorithme_layout)
        
        algorithme_box = QGroupBox("Paramètres", self)
        algorithme_box_layout = QVBoxLayout(algorithme_box)
        
        algorithme_duree = QWidget(algorithme_box)
        algorithme_duree_layout = QHBoxLayout(algorithme_duree)
        
        algorithme_duree_label = QLabel("Durée de propagation", algorithme_duree)
        self.algorithme_duree_heure_ = QSpinBox(algorithme_duree)
        self.algorithme_duree_heure_.setRange(0,24)
        self.algorithme_duree_minute_ = QSpinBox(algorithme_duree)
        self.algorithme_duree_minute_.setRange(0,59)
        algorithme_duree_layout.addWidget(algorithme_duree_label)
        algorithme_duree_layout.addWidget(self.algorithme_duree_heure_)
        algorithme_duree_layout.addWidget(QLabel("h",algorithme_duree))
        algorithme_duree_layout.addWidget(self.algorithme_duree_minute_)
        algorithme_duree_layout.addWidget(QLabel("min",algorithme_duree))

        algorithme_intervalle = QWidget(algorithme_box)
        algorithme_intervalle_layout = QHBoxLayout(algorithme_intervalle)

        algorithme_intervalle_label = QLabel("Intervalle", algorithme_duree)
        self.algorithme_intervalle_heure_ = QSpinBox(algorithme_duree)
        self.algorithme_intervalle_heure_.setRange(0, 24)
        self.algorithme_intervalle_minute_ = QSpinBox(algorithme_duree)
        self.algorithme_intervalle_minute_.setRange(0, 59)
        algorithme_intervalle_layout.addWidget(algorithme_intervalle_label)
        algorithme_intervalle_layout.addWidget(self.algorithme_intervalle_heure_)
        algorithme_intervalle_layout.addWidget(QLabel("h", algorithme_intervalle))
        algorithme_intervalle_layout.addWidget(self.algorithme_intervalle_minute_)
        algorithme_intervalle_layout.addWidget(QLabel("min", algorithme_duree))

        algorithme_intervalle.setLayout(algorithme_intervalle_layout)



        
        algorithme_duree.setLayout(algorithme_duree_layout)
        
        algorithme_reserve = QWidget(algorithme_box)
        algorithme_reserve_layout = QHBoxLayout(algorithme_reserve)
        
        algorithme_reserve_label = QLabel("Réserve en eau du sol", algorithme_reserve)
        self.algorithme_reserve_spinbox_ = QSpinBox(algorithme_reserve)
        algorithme_reserve_label_unite = QLabel("mm", algorithme_reserve)
        
        algorithme_reserve_layout.addWidget(algorithme_reserve_label)
        algorithme_reserve_layout.addWidget(self.algorithme_reserve_spinbox_)
        algorithme_reserve_layout.addWidget(algorithme_reserve_label_unite)
        algorithme_reserve.setLayout(algorithme_reserve_layout)
        
        algorithme_temperature = QWidget(algorithme_box)
        algorithme_temperature_layout = QHBoxLayout(algorithme_temperature)
        
        algorithme_temperature_label = QLabel("Température", algorithme_temperature)
        self.algorithme_temperature_spinbox_ = QSpinBox(algorithme_temperature)
        algorithme_temperature_label_unite = QLabel("°C", algorithme_temperature)

        algorithme_temperature_layout.addWidget(algorithme_temperature_label)
        algorithme_temperature_layout.addWidget(self.algorithme_temperature_spinbox_)
        algorithme_temperature_layout.addWidget(algorithme_temperature_label_unite)
        
        algorithme_temperature.setLayout(algorithme_temperature_layout)
        
        algorithme_processus = QWidget(algorithme_box)
        algorithme_processus_layout = QHBoxLayout(algorithme_processus)
        
        algorithme_processus_label = QLabel("Nombre de processeur", algorithme_processus)
        self.algorithme_processus_spinbox_ = QSpinBox(algorithme_processus)
        
        algorithme_processus_layout.addWidget(algorithme_processus_label)
        algorithme_processus_layout.addWidget(self.algorithme_processus_spinbox_)

        algorithme_processus.setLayout(algorithme_processus_layout)
        
        algorithme_box_layout.addWidget(algorithme_duree)
        algorithme_box_layout.addWidget(algorithme_intervalle)
        algorithme_box_layout.addWidget(algorithme_reserve)
        algorithme_box_layout.addWidget(algorithme_temperature)
        algorithme_box_layout.addWidget(algorithme_processus)
        algorithme_box.setLayout(algorithme_box_layout)
        
        boutons = QWidget(self)
        boutons_layout = QHBoxLayout(boutons)
        self.valider_ = QPushButton("Valider", boutons)
        self.annuler_ = QPushButton("Annuler", boutons)
        boutons_layout.addWidget(self.valider_)
        boutons_layout.addWidget(self.annuler_)
        boutons.setLayout(boutons_layout)
        
        body.addWidget(algorithme)
        body.addWidget(algorithme_box)
        body.addWidget(boutons)
        
        self.setLayout(body)

        # Event binding
        self.algorithme_nom_.textEdited.connect(self.controller_.algorithm)
        self.algorithme_duree_heure_.valueChanged.connect(self.controller_.heure)
        self.algorithme_duree_minute_.valueChanged.connect(self.controller_.minute)
        self.algorithme_intervalle_heure_.valueChanged.connect(self.controller_.heureIntervalle)
        self.algorithme_intervalle_minute_.valueChanged.connect(self.controller_.minuteIntervalle)

        self.algorithme_reserve_spinbox_.valueChanged.connect(self.controller_.waterReserve)
        self.algorithme_temperature_spinbox_.valueChanged.connect(self.controller_.temperature)
        self.algorithme_processus_spinbox_.valueChanged.connect(self.controller_.process)
        self.valider_.clicked.connect(self.controller_.accept)
        self.annuler_.clicked.connect(self.controller_.close)
        self.update(self.controller_.algorithmModel_)
        
    def update(self, observable):
        self.algorithme_nom_.setText(observable.algorithm_)
        self.algorithme_duree_heure_.setValue(observable.heure_)
        self.algorithme_duree_minute_.setValue(observable.minute_)
        self.algorithme_reserve_spinbox_.setValue(observable.waterReserve_)
        self.algorithme_temperature_spinbox_.setValue(observable.temperature_)
        self.algorithme_processus_spinbox_.setValue(observable.process_)