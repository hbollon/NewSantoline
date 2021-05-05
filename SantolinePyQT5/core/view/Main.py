import sys
import math
from qgis.gui import *  # QgsMapCanvas
from qgis.core import *  # QgsRasterLayer, QgsApplication
from qgis.PyQt.QtWidgets import *  # QMainWindow, QWidget, QTabWidget, QPushButton
from qgis.PyQt.QtCore import *  # QSize
from qgis.PyQt.QtGui import *  # QFrame
# from Point import Point
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QWidget, QApplication, QGridLayout, QLabel, QPushButton
from PyQt5.QtGui import QMouseEvent, QPainter


class Point(object):
    def __init__(self, xParam=0.0, yParam=0.0):
        self.x = xParam
        self.y = yParam

    def distance(self, pt):
        xDiff = self.x - pt.x
        yDiff = self.y - pt.y
        return math.sqrt(xDiff ** 2 + yDiff ** 2)


class Example(QgsMapCanvasItem):

    def __init__(self):
        super().__init__()

        x = 0
        y = 0
        self.nbGraphe = 0  # c'est le nombre de graphes
        self.isNewGraph = False  # sert à determiner si on commence un nouveau graphe ou non
        self.listeLargages = [[]]  # liste contenant les graphes des largages
        # le texte qui va aller dans le label
        self.text = "x: {0},  y: {1}".format(x, y)
        # le label dans lequel on affiche le texte
        self.labelPositionCurseur = QLabel(self.text, self)
        # position du curseur (contient un champ x et un champ y)
        self.pos = None
        self.redb = QPushButton('Red', self)

        self.initUI()

    def initUI(self):
        grid = QGridLayout()    # notre espace de jeu
        grid.addWidget(self.labelPositionCurseur, 0, 0, Qt.AlignTop)

        grid.addWidget(self.redb)
        self.redb.setCheckable(True)
        # self.redb.move(10, 10)
        self.redb.clicked[bool].connect(self.paint_line)

        self.setMouseTracking(True)
        self.setLayout(grid)
        # taille par défaut de la fenetre
        self.setGeometry(200, 100, 1000, 600)
        self.setWindowTitle('Zhang Lei')
        self.show()

    # fonction qui réagit à l'évènement : la souris bouge
    def mouseMoveEvent(self, e):
        self.pos = e.pos()
        if self.isNewGraph:
            x = e.x()
            y = e.y()
            text = "x: {0},  y: {1}".format(e.x(), e.y())
            self.labelPositionCurseur.setText(text)
            self.update()

    # fonction qui réagit à l'évènement : appuis sur n'importe quel bouton de la souris
    def mousePressEvent(self, event: QMouseEvent) -> None:
        if event.button() == Qt.LeftButton:
            if not self.isNewGraph:
                self.listeLargages.append([])
                self.nbGraphe += 1

            self.isNewGraph = True
            text = "click"
            self.labelPositionCurseur.setText(text)
            b = Point(event.x(), event.y())
            self.listeLargages[self.nbGraphe - 1].append(b)
        elif event.button() == Qt.RightButton:
            self.isNewGraph = False
            text = "fini"
            self.labelPositionCurseur.setText(text)

    # fonction qui réagit à l'évènement : raffraichissement de la fenetre
    def paintEvent(self, event):
        # pour dessiner tous les graphes precedents
        if self.nbGraphe >= 2:
            for i in range(0, self.nbGraphe - 1):
                for j in range(1, len(self.listeLargages[i])):
                    self.trace_segment_typo_circle(self.listeLargages[i][j - 1],
                                                   self.listeLargages[i][j])

        # le graphe courant
        if len(self.listeLargages[self.nbGraphe - 1]) >= 0:
            for i in range(1, len(self.listeLargages[self.nbGraphe - 1])):
                self.trace_segment_typo_circle(self.listeLargages[self.nbGraphe - 1][i - 1],
                                               self.listeLargages[self.nbGraphe - 1][i])

        # Si on a au moins un point dans notre listeLargages de points alors on trace un segment entre le dernier
        # point placé et le curseur pour prévisualiser le tracé
        if self.isNewGraph:
            if len(self.listeLargages[self.nbGraphe - 1]) >= 1:
                ptmp = Point(self.pos.x(), self.pos.y())
                self.trace_segment_typo_circle(
                    ptmp, self.listeLargages[self.nbGraphe - 1][len(self.listeLargages[self.nbGraphe]) - 1])

    def trace_segment_typo_circle(self, p1, p2):
        '''
        prend en parametre deux points et le contexte et trace une ligne entre les deux
        avec la typo: cercle
        :param self: self
        :param x1: abscisse du premier point
        :param y1: ordonnée du premier point
        :param x2: abscisse du deuxieme point
        :param y2: ordonnée du deuxieme point
        :return: nothing
        '''
        q = QPainter(self)

        # LARGEUR du segmet dessiné
        largeur = p2.x - p1.x

        # HAUTEUR du segment dessiné
        hauteur = p2.y - p1.y

        # calcul de la longueur du segment en cours
        longueur = p2.distance(p1)

        # Nombre de symboles dessinés sur le segment en cours
        nb_graphe_rond = int(longueur // 8)

        # Dans cette boucle nous dessinons nbGraphe symboles le long du segment
        for s in range(0, nb_graphe_rond):
            # distance en abscisse entre le symbole dessiné et le point listeLargages[i-1]
            dx = s * (largeur / nb_graphe_rond)

            # distance en ordonnée entre le symbole dessiné et le point listeLargages[i-1]
            dy = s * (hauteur / nb_graphe_rond)

            # on dessine un symbole aux coordonnées x= x de listeLargages[i] + dx -5
            #                                       y= y de listeLargages[i] + dy -5
            # on retire 5 à chaque coordonnées pour centrer le cercle qui
            # est inclu dans un carré de 10 de coté
            q.drawEllipse(p1.x + dx - 5, p1.y + dy - 5, 10, 10)

    def paint_line(x, y):
        print("hello")


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())
