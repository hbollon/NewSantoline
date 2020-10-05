//
// Created by Skaldr on 7/24/2019.
//

#ifndef ALGO3_CHOC_HPP
#define ALGO3_CHOC_HPP

#include "Point.h"
#include <string>

class Choc {

public:
    std::string num;
    std::string indic;
    Point2D point;
    double test;
    Choc(std::string num, std::string indic, Point2D point, double test):
    num(num),indic(indic),point(point),test(test)
    {}


};


#endif //ALGO3_CHOC_HPP
