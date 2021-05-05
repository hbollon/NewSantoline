//
// Created by Skaldr on 7/11/2019.
//

#include "PointOpt.hpp"

#include <utility>

using namespace std;

PointOpt::PointOpt(const Point3D& p, Vector2D tau, std::string indice, int *compt, std::string conditionInsertion) : tau(std::move(tau)), indice(std::move(indice)), coordonne(Point2D(p.x(), p.y())), t(p.z()), id(std::to_string(*compt)), creation(std::move(conditionInsertion))
{
    ++(*compt);
}

PointOpt::PointOpt(const Point2D& p, double t, Vector2D tau, std::string ancetre, std::string suivant, std::string precedent,
                   std::string indice, VitesseOpt vit, int *compt, string conditionInsertion)
    : coordonne(p), t(t), tau(std::move(tau)), indice(std::move(indice)), ancetre(std::move(ancetre)),
      precedent(std::move(precedent)), suivant(std::move(suivant)), id(std::to_string(*compt)), creation(std::move(conditionInsertion))
{
    vector<VitesseOpt> vits;
    vits.push_back(vit);
    listeVitesse.push_back(vits);
    ++(*compt);
}
PointOpt::PointOpt(const Point2D& p, double t, Vector2D tau, std::string ancetre, std::string suivant, std::string precedent,
                   std::string indice, vector<vector<VitesseOpt>> vit, int *compt, string conditionInsertion)
    : coordonne(p), t(t), tau(std::move(tau)), indice(std::move(indice)), ancetre(std::move(ancetre)),
      precedent(std::move(precedent)), suivant(std::move(suivant)), id(std::to_string(*compt)), creation(std::move(conditionInsertion))
{
    for (auto vits : vit)
    {
        listeVitesse.push_back(vits);
    }
    ++(*compt);
}

PointOpt::PointOpt(const Point2D& p, double t, Vector2D tau, std::string indice, int *compt, std::string conditionInsertion) : tau(std::move(tau)), indice(std::move(indice)), coordonne(p), t(t), id(std::to_string(*compt)), creation(std::move(conditionInsertion)) { ++(*compt); }

PointOpt::PointOpt(int *compt) : tau(Vector2D()), indice("0"), coordonne(Point2D()), t(0), id(std::to_string(++(*compt))) { ++(*compt); }

PointOpt::PointOpt() : tau(), indice(), coordonne(), t(), id()
{
}

PointOpt::PointOpt(const PointOpt &other) : tau(other.tau), indice(other.indice), coordonne(other.coordonne), t(other.t),
                                            id(other.id), ancetre(other.ancetre), suivant(other.suivant), precedent(other.precedent), listeVitesse(other.listeVitesse), creation(other.creation)
{
}

PointOpt &PointOpt::operator=(const PointOpt &other)
{
    this->listeVitesse = other.listeVitesse;
    this->precedent = other.precedent;
    this->suivant = other.suivant;
    this->ancetre = other.ancetre;
    this->id = other.id;
    this->coordonne = other.coordonne;
    this->indice = other.indice;
    this->tau = other.tau;
    this->t = other.t;
    this->creation = other.creation;

    return *this;
}

void PointOpt::addVitesse(vector<VitesseOpt> v)
{
    listeVitesse.push_back(v);
}

std::string PointOpt::getId() const { return id; }

bool PointOpt::operator<(const PointOpt &p)
{
    return (this->t < p.t);
}
