//
// Created by dubos on 12/02/2020.
//

#ifndef ALGO3_SOMMETCASE_H
#define ALGO3_SOMMETCASE_H


#include <string>
#include "Vecteur.h"

class SommetCase {
public:

    std::string numSommet;// coordonnees du sommet sous forme x_y
    double tIgnitionSommet;//temps auquel le sommet va bruler
    std::string indicePointAllume;//indice du point qui va bruler le sommet
    bool indic;
    Point2D pts;

    SommetCase(std::string newNumSommet, double newT, std::string newIndicePointAllume, bool newIndic, int x, int y);
    SommetCase(std::string newNumSommet, double newT, std::string newIndicePointAllume, bool newIndic, Point2D newPts);

};


#endif //ALGO3_SOMMETCASE_H
