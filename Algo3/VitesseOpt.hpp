//
// Created by Skaldr on 7/11/2019.
//

#ifndef ALGO3_VITESSEOPT_HPP
#define ALGO3_VITESSEOPT_HPP

#include <string>
#include "Point.h"
#include "Vecteur.h"

class VitesseOpt
{
private:
    Point2D m_coordonne; /*Indication de la cellule dans laquelle la vitesse est calculée*/
    Vector2D m_tau;      /*Vecteur tan*/
    Vector2D m_vecteur;  /*Vrai vecteur vitesse*/
    double m_indicv;     /*Indicateur*/
public:
    VitesseOpt(Vector2D vecteur, Vector2D tau, Point2D coordonne, double indicv);
    VitesseOpt();
    Point2D coordonne() const { return m_coordonne; }
    Vector2D vecteur() const { return m_vecteur; }
    Vector2D tau() const { return m_tau; }
    double indice() const { return m_indicv; }
    void setIndice(int indice) { m_indicv = indice; }
    friend std::ostream &operator<<(std::ostream &os, VitesseOpt &v);
    void affiche() const
    {
        std::cout << "Vecteur : " << vecteur() << std::endl
                  << "Tau : " << tau() << std::endl
                  << "Coordonne : " << coordonne()
                  << "Indic : " << indice() << std::endl;
    }
};

#endif //ALGO3_VITESSEOPT_HPP
