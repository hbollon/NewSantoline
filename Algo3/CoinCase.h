//
// Created by dubos on 12/02/2020.
//

#ifndef ALGO3_COINCASE_H
#define ALGO3_COINCASE_H


#include <string>
#include "Vecteur.h"

class CoinCase {
public:

    std::string numCoin;// coordonnees du coin sous forme x_y
    double tIgnitionCoin;//temps auquel le coin va bruler
    std::string indicePointAllume;//indice du point qui va bruler le coin
    bool indic;
    Point2D pts;

    CoinCase(std::string newNumCoin, double newT, std::string newIndicePointAllume, bool newIndic, int x, int y);
    CoinCase(std::string newNumCoin, double newT, std::string newIndicePointAllume, bool newIndic, Point2D newPts);



};


#endif //ALGO3_COINCASE_H
