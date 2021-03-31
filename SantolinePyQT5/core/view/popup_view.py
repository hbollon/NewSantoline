from qgis.PyQt.QtWidgets import *

from . import input_popup_view
from ..controller import *

class Popup_Window(input_popup_view.InputPopup):
    def __init__(self):
        self.close_ = None
        super().__init__(popup_controller.Popup_Controller(self))

    def initUI(self):
        self.setWindowTitle(None)
        body = QVBoxLayout(self)
        self.label = QLabel(None)
        self.label.setContentsMargins(50, 20, 50, 20)
        boutons = QWidget(self)
        boutons_layout = QHBoxLayout(boutons)
        self.close_ = QPushButton("OK", boutons)
        boutons_layout.addWidget(self.close_)
        boutons.setLayout(boutons_layout)
        body.addWidget(self.label)
        body.addWidget(boutons)
        self.setLayout(body)
        self.close_.clicked.connect(self.controller_.close)

    def update(self, textContent, title):
        self.setWindowTitle(title)
        self.label.setText(textContent)
        