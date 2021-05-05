from PyQt5.QtWidgets import *

from core.controller import reglage_controller
from core.view import input_popup_view


class ReglageWindow(input_popup_view.InputPopup):
    def __init__(self, canvasModel, santoView):
        self.rateOfSpread_value_ = None
        self.exentricite_value_ = None
        self.segment_value_ = None
        self.ellipseCoef_value_ = None
        self.angle_value_ = None
        self.generations_value_ = None
        self.valider_ = None
        self.annuler_ = None

        super().__init__(reglage_controller.ReglageController(self))

    def initUI(self):
        self.setWindowTitle("Paramètres")

        body = QVBoxLayout(self)

        exentricite = QWidget(self)
        exentricite_layout = QHBoxLayout(exentricite)
        exentricite_label = QLabel("Exentricite", exentricite)
        self.exentricite_value_ = QSpinBox(exentricite)
        self.exentricite_value_.setRange(1, 200)
        self.exentricite_value_.setSuffix("%")
        exentricite_layout.addWidget(exentricite_label)
        exentricite_layout.addWidget(self.exentricite_value_)

        rateOfSpread = QWidget(self)
        rateOfSpread_layout = QHBoxLayout(rateOfSpread)
        rateOfSpread_label = QLabel("Rate of Spread", rateOfSpread)
        self.rateOfSpread_value_ = QSpinBox(rateOfSpread)
        self.rateOfSpread_value_.setRange(1, 200)
        self.rateOfSpread_value_.setSuffix("%")
        rateOfSpread_layout.addWidget(rateOfSpread_label)
        rateOfSpread_layout.addWidget(self.rateOfSpread_value_)

        ellipseCoef = QWidget(self)
        ellipseCoef_layout = QHBoxLayout(ellipseCoef)
        ellipseCoef_label = QLabel("Coeficient correcteur", ellipseCoef)
        self.ellipseCoef_value_ = QSpinBox(ellipseCoef)
        self.ellipseCoef_value_.setRange(1, 100)
        self.ellipseCoef_value_.setSuffix("%")
        ellipseCoef_layout.addWidget(ellipseCoef_label)
        ellipseCoef_layout.addWidget(self.ellipseCoef_value_)

        generations = QWidget(self)
        generations_layout = QHBoxLayout(generations)
        generations_label = QLabel("Nombre de generations", generations)
        self.generations_value_ = QSpinBox(generations)
        self.generations_value_.setRange(1, 3)
        generations_layout.addWidget(generations_label)
        generations_layout.addWidget(self.generations_value_)

        segment = QWidget(self)
        segment_layout = QHBoxLayout(segment)
        segment_label = QLabel("Nombre de segment par bordure", segment)
        self.segment_value_ = QSpinBox(segment)
        self.segment_value_.setRange(1, 25)
        segment_layout.addWidget(segment_label)
        segment_layout.addWidget(self.segment_value_)

        angle = QWidget(self)
        angle_layout = QHBoxLayout(angle)
        angle_label = QLabel("Angle entre les arcs", angle)
        self.angle_value_ = QSpinBox(angle)
        self.angle_value_.setRange(1, 10)
        angle_layout.addWidget(angle_label)
        angle_layout.addWidget(self.angle_value_)
        angle_layout.addWidget(QLabel("degree", angle))

        boutons = QWidget(self)
        boutons_layout = QHBoxLayout(boutons)
        self.valider_ = QPushButton("Valider", boutons)
        self.annuler_ = QPushButton("Annuler", boutons)
        boutons_layout.addWidget(self.valider_)
        boutons_layout.addWidget(self.annuler_)
        boutons.setLayout(boutons_layout)

        body.addWidget(exentricite)
        body.addWidget(rateOfSpread)
        body.addWidget(ellipseCoef)
        body.addWidget(generations)
        body.addWidget(segment)
        body.addWidget(angle)
        body.addWidget(boutons)

        self.exentricite_value_.valueChanged.connect(
            self.controller_.exentricite)
        self.rateOfSpread_value_.valueChanged.connect(
            self.controller_.rateOfSpread)
        self.angle_value_.valueChanged.connect(self.controller_.angle)
        self.ellipseCoef_value_.valueChanged.connect(self.controller_.coef)
        self.generations_value_.valueChanged.connect(
            self.controller_.generations)
        self.segment_value_.valueChanged.connect(self.controller_.segment)

        self.valider_.clicked.connect(self.controller_.accept)
        self.annuler_.clicked.connect(self.controller_.annuler)

        self.setLayout(body)
        self.update(self.controller_.model_)

    def update(self, observable):

        self.exentricite_value_.setValue(observable.exentricite_)
        self.rateOfSpread_value_.setValue(observable.rateOfSpread_)
        self.ellipseCoef_value_.setValue(observable.coef_)
        self.generations_value_.setValue(observable.generations_)
        self.angle_value_.setValue(observable.angle_)
        self.segment_value_.setValue(observable.segment_)
