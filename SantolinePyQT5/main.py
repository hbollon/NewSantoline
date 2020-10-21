#! python3
# credits : 
#           - https://www.flaticon.com/authors/pixel-perfect 
#           - http://www.famfamfam.com/lab/icons/
#           - CreativeCommons 3.0

import sys
import os
from qgis.core import QgsApplication, QgsPoint
from core.view import santoline_view
import random

import core.libs

import threading

def main():
    current_path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(current_path)
    l = list(map(lambda arg: bytes(arg, 'utf-8'), sys.argv))
    QgsApplication.setPrefixPath("C:\\Program Files\\QGIS 3.2\\apps\\qgis", True)
    app = QgsApplication(l, True)
    app.initQgis()
    santoline_view.Santoline()
    sys.exit(app.exec_())
            
if (__name__ == "__main__"):
    main()