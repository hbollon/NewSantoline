//
// Created by Skaldr on 7/11/2019.
//

#ifndef ALGO3_POINTOPT_HPP
#define ALGO3_POINTOPT_HPP

#include "VitesseOpt.hpp"
#include "Point.h"

#include <vector>

class PointOpt
{
private:
    std::string id;

public:
    Point2D coordonne;
    double t;
    Vector2D tau;

    std::string suivant = "0";
    std::string precedent = "0";
    std::string ancetre = "0";
    std::string indice = "";
    std::string creation = "";
    std::vector<std::vector<VitesseOpt>> listeVitesse;

    PointOpt(Point2D p, double t, Vector2D tau, std::string indice, int *compt, std::string conditionInsertion);
    PointOpt(Point3D p, Vector2D tau, std::string indice, int *compt, std::string conditionInsertion);
    PointOpt(Point2D p, double t, Vector2D tau, std::string ancetre, std::string suivant, std::string precedent, std::string indice, std::vector<std::vector<VitesseOpt>> vit, int *compt, std::string conditionInsertion);
    PointOpt(Point2D p, double t, Vector2D tau, std::string ancetre, std::string suivant, std::string precedent, std::string indice, VitesseOpt vit, int *compt, std::string conditionInsertion);
    PointOpt(int *compt);
    PointOpt();
    PointOpt(const PointOpt &other);

    PointOpt &operator=(const PointOpt &other);
    std::string getId() const;
    void setPointeurs(PointOpt *p, PointOpt *s, PointOpt *a);
    bool operator<(const PointOpt &p);

    void addVitesse(std::vector<VitesseOpt> v);
};

#endif //ALGO3_POINTOPT_HPP
