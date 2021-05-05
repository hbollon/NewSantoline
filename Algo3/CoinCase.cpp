//
// Created by dubos on 12/02/2020.
//

#include "CoinCase.h"

#include <utility>


CoinCase::CoinCase(std::string newNumCoin, double newT, std::string newIndicePointAllume, bool newIndic, int x, int y)
:numCoin(std::move(newNumCoin)),tIgnitionCoin(newT),indicePointAllume(std::move(newIndicePointAllume)),indic(newIndic),pts(Point2D(x,y)) 
{

}

CoinCase::CoinCase(std::string newNumCoin, double newT, std::string newIndicePointAllume, bool newIndic, Point2D newPts)
        :numCoin(std::move(newNumCoin)),tIgnitionCoin(newT),indicePointAllume(std::move(newIndicePointAllume)),indic(newIndic),pts(newPts)
{

}

