//
// Created by dubos on 12/02/2020.
//

#include "SommetCase.h"

SommetCase::SommetCase(const std::string& newNumSommet, const double& newT, const std::string& newIndicePointAllume, const bool& newIndic, const int& x, const int& y)
    : numSommet(newNumSommet), tIgnitionSommet(newT), indicePointAllume(newIndicePointAllume), indic(newIndic), pts(Point2D(x, y))
{
}
SommetCase::SommetCase(const std::string& newNumSommet, const double& newT, const std::string& newIndicePointAllume, const bool& newIndic, const Point2D& newPts)
    : numSommet(newNumSommet), tIgnitionSommet(newT), indicePointAllume(newIndicePointAllume), indic(newIndic), pts(newPts)
{
}
