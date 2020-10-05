//
// Created by Skaldr on 7/11/2019.
//

#include "VitesseOpt.hpp"
VitesseOpt::VitesseOpt(Vector2D vecteur, Vector2D tau, Point2D coordonne, double indicv):
m_tau(tau),m_vecteur(vecteur),m_coordonne(coordonne),m_indicv(indicv){}

VitesseOpt::VitesseOpt():
        m_tau(Point2D()),m_vecteur(Vector2D()),m_coordonne(Point2D()),m_indicv(0){}

std::ostream &operator<<(std::ostream &os,  VitesseOpt &v)
{
return os << "Coordonne : " << v.coordonne() << std::endl
<< "Vecteur : " << v.vecteur()<<std::endl
<< "Tau : " << v.tau()<<std::endl
<< "Indic : "<<v.indice()<<std::endl;

}