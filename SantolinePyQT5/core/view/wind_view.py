from qgis.gui import *#QgsMapCanvas
from qgis.core import *#QgsRasterLayer, QgsApplication
from qgis.PyQt.QtWidgets import *#QMainWindow, QWidget, QTabWidget, QPushButton
from qgis.PyQt.QtCore import *#QSize
from qgis.PyQt.QtGui import *#QFrame

from . import input_popup_view
from ..controller import *

class WindWindow(input_popup_view.InputPopup):
    def __init__(self,santolineView):
        self.nord_spinbox_ = None
        self.sud_spinbox_ = None
        self.est_spinbox_ = None
        self.ouest_spinbox = None
        self.direction_left_direction_spinbox_ = None
        self.vitesse_ms_spinbox_ = None
        self.vitesse_kmh_spinbox_ = None

        self.direction_dial_ = None

        self.vitesse_slider_ = None

        self.valider_ = None
        self.annuler_ = None

        super().__init__(wind_controller.WindController(self,santolineView))

    def initUI(self):
        self.setWindowTitle ("Paramètres Épilobe")
        body = QVBoxLayout(self)
        parametres_zone = QGroupBox("Zone", self)
        parametres_layout = QVBoxLayout(parametres_zone)




        nord_zone = QWidget(self)
        nord_layout = QHBoxLayout(nord_zone)
        nord_label = QLabel("Distance nord (mètres)", nord_zone)
        self.nord_spinbox_ = QSpinBox(nord_zone)
        self.nord_spinbox_.setMaximum(1000000)
        nord_layout.addWidget(nord_label)
        nord_layout.addWidget(self.nord_spinbox_)
        nord_zone.setLayout(nord_layout)

        sud_zone = QWidget(self)
        sud_layout = QHBoxLayout(sud_zone)
        sud_label = QLabel("Distance sud (mètres)", sud_zone)
        self.sud_spinbox_ = QSpinBox(sud_zone)
        self.sud_spinbox_.setMaximum(1000000)
        sud_layout.addWidget(sud_label)
        sud_layout.addWidget(self.sud_spinbox_)
        sud_zone.setLayout(sud_layout)

        est_zone = QWidget(self)
        est_layout = QHBoxLayout(est_zone)
        est_label = QLabel("Distance est (mètres)", est_zone)
        self.est_spinbox_ = QSpinBox(est_zone)
        self.est_spinbox_.setMaximum(1000000)
        est_layout.addWidget(est_label)
        est_layout.addWidget(self.est_spinbox_)
        est_zone.setLayout(est_layout)

        ouest_zone = QWidget(self)
        ouest_layout = QHBoxLayout(ouest_zone)
        ouest_label = QLabel("Distance ouest (mètres)", ouest_zone)
        self.ouest_spinbox_ = QSpinBox(ouest_zone)
        self.ouest_spinbox_.setMaximum(1000000)
        ouest_layout.addWidget(ouest_label)
        ouest_layout.addWidget(self.ouest_spinbox_)
        ouest_zone.setLayout(ouest_layout)

        parametres_layout.addWidget(nord_zone)
        parametres_layout.addWidget(sud_zone)
        parametres_layout.addWidget(est_zone)
        parametres_layout.addWidget(ouest_zone)
        parametres_zone.setLayout(parametres_layout)

        direction_vent = QGroupBox("Direction", self)
        direction_layout = QHBoxLayout(direction_vent)

        direction_left = QWidget(direction_vent)
        direction_left_layout = QVBoxLayout(direction_left)


        direction_left_direction = QWidget(direction_left)
        direction_left_direction_layout = QHBoxLayout(direction_left_direction)
        direction_left_direction_label = QLabel("Direction", direction_left_direction)
        self.direction_left_direction_spinbox_ = QSpinBox(direction_left_direction)
        self.direction_left_direction_spinbox_.setMaximum(359)
        self.direction_left_direction_spinbox_.setWrapping(True)
        direction_left_direction_layout.addWidget(direction_left_direction_label)
        direction_left_direction_layout.addWidget(self.direction_left_direction_spinbox_)
        direction_left_direction.setLayout(direction_left_direction_layout)

        direction_left_layout.addWidget(direction_left_direction)

        direction_left.setLayout(direction_left_layout)

        self.direction_dial_ = QDial(direction_vent)
        self.direction_dial_.setWrapping(True)
        self.direction_dial_.setMinimum(0)
        self.direction_dial_.setMaximum(359)
        self.direction_dial_.setInvertedAppearance(True)

        direction_layout.addWidget(direction_left)
        direction_layout.addWidget(self.direction_dial_)
        direction_vent.setLayout(direction_layout)

        vitesse_vent = QGroupBox("Vitesse", self)
        vitesse_layout = QVBoxLayout(vitesse_vent)

        self.vitesse_slider_ = QSlider(Qt.Horizontal, vitesse_vent)
        self.vitesse_slider_.setMaximum(100)
        self.vitesse_slider_.setSingleStep(1)

        vitesse_ms = QWidget(vitesse_vent)
        vitesse_ms_layout = QHBoxLayout(vitesse_ms)
        self.vitesse_ms_spinbox_ = QDoubleSpinBox(vitesse_ms)
        self.vitesse_ms_spinbox_.setMaximum(100)
        vitesse_ms_label = QLabel("m/s", vitesse_ms)
        vitesse_ms_layout.addWidget(self.vitesse_ms_spinbox_)
        vitesse_ms_layout.addWidget(vitesse_ms_label)
        vitesse_ms.setLayout(vitesse_ms_layout)

        vitesse_kmh = QWidget(vitesse_vent)
        vitesse_kmh_layout = QHBoxLayout(vitesse_kmh)
        self.vitesse_kmh_spinbox_ = QDoubleSpinBox(vitesse_kmh)
        self.vitesse_kmh_spinbox_.setMaximum(360)
        vitesse_kmh_label = QLabel("km/h", vitesse_kmh)
        vitesse_kmh_layout.addWidget(self.vitesse_kmh_spinbox_)
        vitesse_kmh_layout.addWidget(vitesse_kmh_label)
        vitesse_kmh.setLayout(vitesse_kmh_layout)

        vitesse_layout.addWidget(self.vitesse_slider_)
        vitesse_layout.addWidget(vitesse_ms)
        vitesse_layout.addWidget(vitesse_kmh)
        vitesse_vent.setLayout(vitesse_layout)

        boutons = QWidget(self)
        boutons_layout = QHBoxLayout(boutons)
        self.valider_ = QPushButton("Valider", boutons)
        self.annuler_ = QPushButton("Annuler", boutons)
        boutons_layout.addWidget(self.valider_)
        boutons_layout.addWidget(self.annuler_)
        boutons.setLayout(boutons_layout)

        body.addWidget(parametres_zone)
        body.addWidget(direction_vent)
        body.addWidget(vitesse_vent)
        body.addWidget(boutons)

        self.setLayout(body)

        # Event binding
        self.nord_spinbox_.valueChanged.connect(self.controller_.north)
        self.sud_spinbox_.valueChanged.connect(self.controller_.south)
        self.est_spinbox_.valueChanged.connect(self.controller_.east)
        self.ouest_spinbox_.valueChanged.connect(self.controller_.west)
        self.direction_left_direction_spinbox_.valueChanged.connect(self.controller_.direction)
        self.vitesse_ms_spinbox_.valueChanged.connect(self.controller_.speed)
        self.vitesse_kmh_spinbox_.valueChanged.connect(self.controller_.speedkmh)
        self.direction_dial_.valueChanged.connect(self.controller_.direction)
        self.vitesse_slider_.valueChanged.connect(self.controller_.speed)
        self.valider_.clicked.connect(self.controller_.accept)
        self.annuler_.clicked.connect(self.controller_.close)

    def update(self, observable):

        self.direction_left_direction_spinbox_.setValue(observable.direction_)
        self.vitesse_ms_spinbox_.setValue(observable.speed_)
        self.vitesse_kmh_spinbox_.setValue(observable.speed_ * 3.6)
        self.direction_dial_.setValue(observable.direction_)
        self.vitesse_slider_.setValue(observable.speed_)