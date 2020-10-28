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
    double temps;
    Choc(std::string num, std::string indic, Point2D point, double test):
    num(num),indic(indic),point(point),temps(test)
    {}
    Choc():num("-1"),indic("-1"),temps(-1)
    {
        point = Point2D(0,0);
    };


};


#endif //ALGO3_CHOC_HPP
