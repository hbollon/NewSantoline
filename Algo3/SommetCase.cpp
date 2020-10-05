//
// Created by dubos on 12/02/2020.
//

#include "SommetCase.h"


SommetCase::SommetCase(std::string newNumSommet, double newT, std::string newIndicePointAllume, bool newIndic, int x, int y)
:numSommet(newNumSommet),tIgnitionSommet(newT),indicePointAllume(newIndicePointAllume),indic(newIndic)
{
    pts = Point2D(x,y);
}
SommetCase::SommetCase(std::string newNumSommet, double newT, std::string newIndicePointAllume, bool newIndic, Point2D newPts)
        :numSommet(newNumSommet),tIgnitionSommet(newT),indicePointAllume(newIndicePointAllume),indic(newIndic),pts(newPts)
{

}

